
#include "drvsys.h"

// reference extern variable
extern PDRIVER_OBJECT	g_pHGDrvObj;

NTSTATUS EnumDriver(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pNumberOfBytesReturned)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pOutItp = (PITP)pOutBuffer;

	EnumDriver_LDR_DATA_TABLE_ENTRY(pOutItp);
	EnumDriver_DeviceObjectType(pOutItp);
	EnumDriver_DriverObjectType(pOutItp);
	EnumDriver_ObjectDirectory(pOutItp);
#if defined(_WIN64)
#else
	EnumDriver_Memory(pOutItp);
#endif

	if (pOutItp->driver.nMaxCount > pOutItp->driver.nCount){
		RepairDriverPath(pOutItp);
	}

	return status;
}

VOID EnumDriver_LDR_DATA_TABLE_ENTRY(
	__inout	PITP	pItp)
{
	PLDR_DATA_TABLE_ENTRY	Entry = NULL;
	PLDR_DATA_TABLE_ENTRY	FirstEntry = NULL;
	SIZE_T i = 0;
	SIZE_T nLen = 0;

	DrvLog(("[drvsys.c].[EnumDriver_LDR_DATA_TABLE_ENTRY]: Enter!\n"));
	Entry = FirstEntry = (PLDR_DATA_TABLE_ENTRY)g_hg_map[eHG_Ntoskrnl_LDR_DATA_TABLE_ENTRY];

	if (Entry == NULL){
		return;
	}

	while ((PLDR_DATA_TABLE_ENTRY)Entry->InLoadOrderLinks.Flink != FirstEntry)
	{
		if (pItp->driver.nMaxCount > pItp->driver.nCount)
		{
			pItp->driver.pDriver[pItp->driver.nCount].DriverType = eDT_HaveOrder;
			pItp->driver.pDriver[pItp->driver.nCount].LoadOrder = i++;
			pItp->driver.pDriver[pItp->driver.nCount].Base = (SIZE_T)Entry->DllBase;
			pItp->driver.pDriver[pItp->driver.nCount].Size = Entry->SizeOfImage;
			
			if (IsUnicodeStringValid(&(Entry->FullDllName)))	
			{
				nLen = GetUnicodeStringSafeLength(&(Entry->FullDllName), SZ_PATH_LEN);
				wcsncpy(pItp->driver.pDriver[pItp->driver.nCount].DriverPath, Entry->FullDllName.Buffer, nLen);
			}
			else if(IsUnicodeStringValid(&(Entry->BaseDllName)))
			{
				nLen = GetUnicodeStringSafeLength(&(Entry->BaseDllName), SZ_PATH_LEN);
				wcsncpy(pItp->driver.pDriver[pItp->driver.nCount].DriverPath, Entry->BaseDllName.Buffer, nLen);
			}
		}

		pItp->driver.nCount ++;
		Entry = (PLDR_DATA_TABLE_ENTRY)Entry->InLoadOrderLinks.Flink;
	}
}

VOID EnumDriver_DeviceObjectType(
	__inout	PITP	pItp)
{
	POBJECT_TYPE	DeviceObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDeviceObjectType];
	PLIST_ENTRY		Entry = *(PLIST_ENTRY*)((SIZE_T)DeviceObjectType + GetOffset(_OBJECT_TYPE_TypeList));
	PDEVICE_OBJECT	Device = NULL;
	PDEVICE_OBJECT	DeviceObject = NULL;
	PLIST_ENTRY		EntryTmp = Entry;

	if (pItp == NULL || 
		DeviceObjectType == NULL || 
		HgMmIsAddressValid(DeviceObjectType) == FALSE	||
		HgMmIsAddressValid(Entry) == FALSE				||
		HgMmIsAddressValid(Entry->Flink) == FALSE)
	{
		return;
	}

	for (; HgMmIsAddressValid(Entry->Flink) && Entry->Flink != EntryTmp; Entry = Entry->Flink)
	{
		Device = (PDEVICE_OBJECT)((SIZE_T)Entry + sizeof(OBJECT_HEADER_CREATOR_INFO) + GetOffset(_OBJECT_HEADER_sizeof));
		if (HgMmIsAddressValid((PVOID)Device) == FALSE){
			continue;
		}
		//DrvLog(("==0x%08X", Device->DeviceType));
		if (IsDriverExist(Device->DriverObject, pItp) == FALSE)
		{
			AddDriverToItp(Device->DriverObject, pItp);
		}
		for (DeviceObject = Device->AttachedDevice;
			DeviceObject && HgMmIsAddressValid(DeviceObject);
			DeviceObject = DeviceObject->AttachedDevice)
		{
			if (IsDriverExist(Device->DriverObject, pItp) == FALSE)
			{
				AddDriverToItp(Device->DriverObject, pItp);
			}
		}
	}
}

VOID AddDriverToItp(
	__in	PDRIVER_OBJECT	DrvObj,
	__inout	PITP			pItp)
{
	PLDR_DATA_TABLE_ENTRY Entry = NULL;
	SIZE_T nLen = 0;

	if (DrvObj == NULL || pItp == NULL){
		return;
	}

	Entry = (PLDR_DATA_TABLE_ENTRY)DrvObj->DriverSection;
	if (Entry == NULL ||
		HgMmIsAddressValid(Entry) == FALSE ||
		HgMmIsAddressValid(Entry->DllBase) == FALSE ||
		(SIZE_T)Entry->DllBase < (SIZE_T)MM_SYSTEM_RANGE_START)
	{
		return;
	}

	if (pItp->driver.nMaxCount > pItp->driver.nCount){
		pItp->driver.pDriver[pItp->driver.nCount].DriverType = eDT_Hide;
		pItp->driver.pDriver[pItp->driver.nCount].LoadOrder = UINT_MAX;
		pItp->driver.pDriver[pItp->driver.nCount].Base = (SIZE_T)Entry->DllBase;
		pItp->driver.pDriver[pItp->driver.nCount].Size = Entry->SizeOfImage;
		pItp->driver.pDriver[pItp->driver.nCount].DriverObject = (SIZE_T)DrvObj;

		if (IsUnicodeStringValid(&(Entry->FullDllName)))	
		{
			nLen = GetUnicodeStringSafeLength(&(Entry->FullDllName), SZ_PATH_LEN);
			wcsncpy(pItp->driver.pDriver[pItp->driver.nCount].DriverPath, Entry->FullDllName.Buffer, nLen);
		}
		else if(IsUnicodeStringValid(&(Entry->BaseDllName)))
		{
			nLen = GetUnicodeStringSafeLength(&(Entry->BaseDllName), SZ_PATH_LEN);
			wcsncpy(pItp->driver.pDriver[pItp->driver.nCount].DriverPath, Entry->BaseDllName.Buffer, nLen);
		}
	}
	pItp->driver.nCount ++;
}

BOOL IsDriverExist(
	__in	PDRIVER_OBJECT	DrvObj,
	__inout	PITP			pItp)
{
	BOOL bExist = TRUE;
	BOOL bFound = FALSE;
	PLDR_DATA_TABLE_ENTRY Entry = NULL;
	SIZE_T i = 0; 
	SIZE_T nCount = 0;

	if (DrvObj == NULL || pItp == NULL || HgMmIsAddressValid((PVOID)DrvObj) == FALSE){
		return TRUE;
	}

	Entry = (PLDR_DATA_TABLE_ENTRY)DrvObj->DriverSection;

	if (Entry == NULL ||
		HgMmIsAddressValid(Entry) == FALSE ||
		HgMmIsAddressValid(Entry->DllBase) == FALSE)
	{
		return TRUE;
	}

	nCount = pItp->driver.nMaxCount > pItp->driver.nCount ? pItp->driver.nCount : pItp->driver.nMaxCount;

	for (i = 0; i < nCount; i++){
		if (pItp->driver.pDriver[i].Base == (SIZE_T)Entry->DllBase){
			if (pItp->driver.pDriver[i].DriverObject == 0){
				pItp->driver.pDriver[i].DriverObject = (SIZE_T)DrvObj;
			}
			bFound = TRUE;
			break;
		}
	}

	if (bFound == FALSE){
		bExist = FALSE;
	}

	return bExist;
}

// 
// reference: http://hi.baidu.com/_achillis/item/268969d87e717d1dd68ed01c
// Before you use typelist field, you need set NtGlobalFlag to MaintainTypeList(Global Flags).
// 
// RegQueryValue	HKLM\System\CurrentControlSet\Control\SESSION MANAGER\GlobalFlag
// Use ProcessMonitor to monitor it.
// 
VOID EnumDriver_DriverObjectType(
	__inout	PITP	pItp)
{
	POBJECT_TYPE	DriverObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDriverObjectType];
	PLIST_ENTRY		Entry = (PLIST_ENTRY)((SIZE_T)DriverObjectType + GetOffset(_OBJECT_TYPE_TypeList));
	PDRIVER_OBJECT	Driver = NULL;
	PDEVICE_OBJECT	DeviceObject = NULL;
	PLIST_ENTRY		EntryTmp = Entry;

	if (pItp == NULL || 
		DriverObjectType == NULL || 
		HgMmIsAddressValid(DriverObjectType) == FALSE	||
		HgMmIsAddressValid(Entry) == FALSE				||
		HgMmIsAddressValid(Entry->Flink) == FALSE)
	{
		return;
	}

	for (; HgMmIsAddressValid(Entry->Flink) && Entry->Flink != EntryTmp; Entry = Entry->Flink)
	{
		Driver = (PDRIVER_OBJECT)((SIZE_T)Entry + sizeof(OBJECT_HEADER_CREATOR_INFO) + GetOffset(_OBJECT_HEADER_sizeof));
		if (IsDriverExist(Driver, pItp) == FALSE)
		{
			AddDriverToItp(Driver, pItp);
		}
		for (DeviceObject = Driver->DeviceObject;
			DeviceObject && HgMmIsAddressValid(DeviceObject);
			DeviceObject = DeviceObject->AttachedDevice)
		{
			if (IsDriverExist(Driver, pItp) == FALSE)
			{
				AddDriverToItp(Driver, pItp);
			}
		}
	}
}

VOID EnumDriver_ObjectDirectory(
	__inout	PITP	pItp)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	OBJECT_ATTRIBUTES ObjectAttributes;
	UNICODE_STRING usDirectory;
	HANDLE hDirectory = NULL;
	PVOID pDirectoryObject = NULL;
	WCHAR pwszDirectory[] = {L"\\\0"};	// root directory(\)
	PETHREAD pThread = NULL;
	CHAR PreMode = 0;

	if (pItp == NULL){
		return;
	}

	SzhInitUnicodeString(&usDirectory, pwszDirectory);
	InitializeObjectAttributes(&ObjectAttributes, &usDirectory, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	pThread = HgPsGetCurrentThread();
	PreMode = ChangePreMode(pThread);

	status = HgZwOpenDirectoryObject(&hDirectory, 0, &ObjectAttributes);

	if (NT_SUCCESS(status)){
		status = HgObReferenceObjectByHandle(hDirectory, 0x10000000, 0, 0, &pDirectoryObject, 0);
		if (NT_SUCCESS(status)){
			if (g_hg_map[eHG_DirectoryObjectType] == 0)
			{
				POBJECT_TYPE DirectoryObjectType = HgObGetObjectType(pDirectoryObject);
				if (DirectoryObjectType != NULL){
					g_hg_map[eHG_DirectoryObjectType] = (SIZE_T)DirectoryObjectType;
				}
			}
			TraverseObjectDirectory(pDirectoryObject, pItp);
			HgObfDereferenceObject(pDirectoryObject);
		}
		status = HgNtClose(hDirectory);
	}
	RecoverPreMode(pThread, PreMode);
}

VOID TraverseObjectDirectory(
	__in	PVOID	pDirectoryObject,
	__inout	PITP	pItp)
{
	POBJECT_TYPE DriverObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDriverObjectType];
	POBJECT_TYPE DeviceObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDeviceObjectType];
	POBJECT_TYPE DirectoryObjectType = (POBJECT_TYPE)g_hg_map[eHG_DirectoryObjectType];
	SIZE_T i = 0;
	POBJECT_DIRECTORY pObjectDirectory = (POBJECT_DIRECTORY)pDirectoryObject;
	POBJECT_DIRECTORY_ENTRY pEntry = NULL;
	PDEVICE_OBJECT DeviceObject = NULL;

	if (DirectoryObjectType == NULL				||
		DeviceObjectType == NULL				||
		DriverObjectType == NULL				||
		HgMmIsAddressValid((PVOID)pObjectDirectory) == FALSE)
	{
		return;
	}
	// field HashBuckets in _OBJECT_DIRECTORY is an array(37).
	for (i = 0; i < 37; i ++){
		pEntry = pObjectDirectory->HashBuckets[i];
		for (; (SIZE_T)pEntry > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid(pEntry); pEntry = pEntry->ChainLink)
		{
			if (HgMmIsAddressValid(pEntry->Object)){
				POBJECT_TYPE pType = HgObGetObjectType(pEntry->Object);
				if (pType == DirectoryObjectType){
					TraverseObjectDirectory(pEntry->Object, pItp);
				}
				else if (pType == DriverObjectType){
					if (IsDriverExist((PDRIVER_OBJECT)pEntry->Object, pItp) == FALSE)
					{
						AddDriverToItp((PDRIVER_OBJECT)pEntry->Object, pItp);
					}

					for (DeviceObject = ((PDRIVER_OBJECT)pEntry->Object)->DeviceObject;
						DeviceObject && HgMmIsAddressValid(DeviceObject);
						DeviceObject = DeviceObject->AttachedDevice)
					{
						if (IsDriverExist(DeviceObject->DriverObject, pItp) == FALSE)
						{
							AddDriverToItp(DeviceObject->DriverObject, pItp);
						}
					}
				}
				else if (pType == DeviceObjectType){
					if (IsDriverExist(((PDEVICE_OBJECT)pEntry->Object)->DriverObject, pItp) == FALSE)
					{
						AddDriverToItp(((PDEVICE_OBJECT)pEntry->Object)->DriverObject, pItp);
					}

					for (DeviceObject = ((PDEVICE_OBJECT)pEntry->Object)->AttachedDevice;
						DeviceObject && HgMmIsAddressValid(DeviceObject);
						DeviceObject = DeviceObject->AttachedDevice)
					{
						if (IsDriverExist(DeviceObject->DriverObject, pItp) == FALSE)
						{
							AddDriverToItp(DeviceObject->DriverObject, pItp);
						}
					}
				}
			}
		}
	}
}

VOID EnumDriver_Memory(
	__inout	PITP	pItp)
{
	SIZE_T KernelBase = g_hg_map[eHG_CurrentKernelBase];
	SIZE_T KernelSize = g_hg_map[eHG_CurrentKernelSize];

	POBJECT_TYPE DriverObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDriverObjectType];
	SYSINF_PAGE_COUNT NumberOfPhysicalPages = 0;
	SYSINF_PAGE_COUNT HighestPhysicalPageNumber = 0;
	SYSINF_PAGE_COUNT nMaxPhysicalPageSize = 0;
	SIZE_T Address = 0;
	PETHREAD pThread = NULL;
	CHAR PreMode = 0;
	SYSTEM_BASIC_INFORMATION sbi;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T AddressMask = SIZE_MAX & (~0xFFF);

	if (pItp == NULL || KernelSize == 0 || KernelBase == 0){
		return;
	}

	pThread = HgPsGetCurrentThread();
	PreMode = ChangePreMode(pThread);
	memset(&sbi, 0, sizeof(SYSTEM_BASIC_INFORMATION));
	status = HgNtQuerySystemInformation(SystemBasicInformation, &sbi, sizeof(SYSTEM_BASIC_INFORMATION), NULL);
	if (NT_SUCCESS(status))
	{
		SIZE_T nObjectHeaderSize = GetOffset(_OBJECT_HEADER_sizeof);
		SIZE_T nSizeOfDriverObject = sizeof(DRIVER_OBJECT) + nObjectHeaderSize;

		NumberOfPhysicalPages = sbi.NumberOfPhysicalPages;
		HighestPhysicalPageNumber = sbi.HighestPhysicalPageNumber;

		if (NumberOfPhysicalPages > HighestPhysicalPageNumber)
		{
			NumberOfPhysicalPages = HighestPhysicalPageNumber;
		}
		nMaxPhysicalPageSize = NumberOfPhysicalPages * PAGE_SIZE;
		Address = (KernelBase + KernelSize) & AddressMask;

		__try{
			SIZE_T i = 0;
			PHYSICAL_ADDRESS PhysicalAddress;
			while (Address < AddressMask)
			{
				if (HgMmIsAddressValid((PVOID)Address) == TRUE ||
					HgMmIsNonPagedSystemAddressValid((PVOID)Address) == TRUE)		// MmIsNonPagedSystemAddressValid: Obsolete. This routine is exported to support existing drivers only.
				{
					PhysicalAddress = HgMmGetPhysicalAddress((PVOID)Address);
					if ((PhysicalAddress.HighPart || PhysicalAddress.LowPart) &&
						PhysicalAddress.QuadPart < nMaxPhysicalPageSize)
					{
						if (HgMmGetVirtualForPhysical(PhysicalAddress) == (PVOID)Address)	// MmGetVirtualForPhysical is reserved for system use. 
						{
							PMDL pMdl = HgIoAllocateMdl((PVOID)Address, PAGE_SIZE, FALSE, FALSE, NULL);
							if (pMdl){
								PVOID pMdlAddress = NULL;
								SIZE_T nRemainSize = 0;
								SIZE_T pDriverObject = 0;
								SIZE_T nPageSize = 0xFE8 - sizeof(DRIVER_OBJECT);
								HgMmBuildMdlForNonPagedPool(pMdl);
								pMdlAddress = HgMmMapLockedPagesSpecifyCache(pMdl, KernelMode, MmCached, NULL, 0, NormalPagePriority);
								if (pMdlAddress)
								{
									nRemainSize = 0;
									pDriverObject = (SIZE_T)pMdlAddress + nObjectHeaderSize;
									do {
										if (HgObGetObjectType((PVOID)pDriverObject) == DriverObjectType &&
											IsDriverObject((PDRIVER_OBJECT)pDriverObject))
										{
											if (IsDriverExist((PDRIVER_OBJECT)pDriverObject, pItp) == FALSE)
											{
												AddDriverToItp((PDRIVER_OBJECT)pDriverObject, pItp);
											}
										}
										nRemainSize += 8;
										pDriverObject += 8;
									} while (nRemainSize <= nPageSize);
								}
							}
						}
					}
				}
				Address += PAGE_SIZE;
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			DrvLog(("[drvsys.c].[EnumDriver_Memory]: Catch exception!\n"));
		}
	}
	RecoverPreMode(pThread, PreMode);
}

BOOL
IsDriverObject(
	__in	PDRIVER_OBJECT DriverObject
	)
{
	BOOL bRtn = FALSE;
	POBJECT_TYPE DriverObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDriverObjectType];
	POBJECT_TYPE DeviceObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDeviceObjectType];

	if (DriverObjectType == NULL ||
		DeviceObjectType == NULL ||
		DriverObjectType == NULL)
	{
		return FALSE;
	}

	if (DriverObject->Type == 4												&&
		DriverObject->Size == sizeof(DRIVER_OBJECT)							&&
		HgObGetObjectType(DriverObject) == DriverObjectType					&&
		HgMmIsAddressValid(DriverObject->DriverSection) == TRUE				&&
		(SIZE_T)DriverObject->DriverSection > (SIZE_T)MM_SYSTEM_RANGE_START	&&
		!(DriverObject->DriverSize & 0x1F)									&&
		(SIZE_T)DriverObject->DriverSize < (SIZE_T)MM_SYSTEM_RANGE_START		&&
		!((SIZE_T)(DriverObject->DriverStart) & 0xFFF)						&&
		(SIZE_T)DriverObject->DriverStart > (SIZE_T)MM_SYSTEM_RANGE_START)
	{
		PDEVICE_OBJECT DeviceObject = DriverObject->DeviceObject;
		if (DeviceObject)
		{
			if (HgMmIsAddressValid(DeviceObject)					&&
				HgObGetObjectType(DeviceObject) == DeviceObjectType	&&
				DeviceObject->Type == 3								&&
				DeviceObject->Size >= sizeof(DEVICE_OBJECT))
			{
				bRtn = TRUE;
			}
		}
		else{
			bRtn = TRUE;
		}
	}

	return bRtn;
}

VOID
RepairDriverPath(
	__inout	PITP	pItp
	)
{
	WCHAR	pwszWindowsDirectory[SZ_PATH_LEN] = {0};
	WCHAR	pwszDriverDirectory[SZ_PATH_LEN] = {0};

	if (pItp == NULL){
		return;
	}

	if (HgGetWindowsDirectory(pwszWindowsDirectory) && HgGetDriverDirectory(pwszDriverDirectory))
	{
		SIZE_T i = 0;
		SIZE_T nCount = pItp->driver.nCount;

		__try{
			for (i = 0; i < nCount; i++){
				if (wcslen(pItp->driver.pDriver[i].DriverPath)){
					WCHAR* pwszTmp = NULL;
					WCHAR* pwszOriginalPath = pItp->driver.pDriver[i].DriverPath;
					WCHAR pwszPath[SZ_PATH_LEN] = {0};
					pwszTmp = wcschr(pwszOriginalPath, L'\\');
					// only name
					if (!pwszTmp)
					{
						wcscpy(pwszPath, pwszDriverDirectory);
						wcscat(pwszPath, pwszOriginalPath);
						wcscpy(pwszOriginalPath, pwszPath);
						pwszOriginalPath[wcslen(pwszPath)] = L'\0';	// ?
					}
					else
					{
						WCHAR pwszQuestionMark[] = L"\\??\\";
						WCHAR pwszSystemRoot[] = L"\\SystemRoot";
						WCHAR pwszWindows[] = L"\\Windows";
						WCHAR pwszWinnt[] = L"\\Winnt";

						if (wcslen(pwszOriginalPath) >= wcslen(pwszQuestionMark) && 
							!_wcsnicmp(pwszOriginalPath, pwszQuestionMark, wcslen(pwszQuestionMark)))
						{
							wcscpy(pwszPath, pwszOriginalPath + wcslen(pwszQuestionMark));
							wcscpy(pwszOriginalPath, pwszPath);
							pwszOriginalPath[wcslen(pwszPath)] = 0;
						}
						else if (wcslen(pwszOriginalPath) >= wcslen(pwszSystemRoot) && 
							!_wcsnicmp(pwszOriginalPath, pwszSystemRoot, wcslen(pwszSystemRoot)))
						{
							wcscpy(pwszPath, pwszWindowsDirectory);
							wcscat(pwszPath, pwszOriginalPath + wcslen(pwszSystemRoot));
							wcscpy(pwszOriginalPath, pwszPath);
							pwszOriginalPath[wcslen(pwszPath)] = 0;
						}
						else if (wcslen(pwszOriginalPath) >= wcslen(pwszWindows) && 
							!_wcsnicmp(pwszOriginalPath, pwszWindows, wcslen(pwszWindows)))
						{
							wcscpy(pwszPath, pwszWindowsDirectory);
							wcscat(pwszPath, pwszOriginalPath + wcslen(pwszWindows));
							wcscpy(pwszOriginalPath, pwszPath);
							pwszOriginalPath[wcslen(pwszPath)] = 0;
						}
						else if (wcslen(pwszOriginalPath) >= wcslen(pwszWinnt) && 
							!_wcsnicmp(pwszOriginalPath, pwszWinnt, wcslen(pwszWinnt)))
						{
							wcscpy(pwszPath, pwszWindowsDirectory);
							wcscat(pwszPath, pwszOriginalPath + wcslen(pwszWinnt));
							wcscpy(pwszOriginalPath, pwszPath);
							pwszOriginalPath[wcslen(pwszPath)] = 0;
						}
					}
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			DrvLog(("[drvsys.c].[RepairDriverPath]: Catch exception!\n"));
		}
	}
}