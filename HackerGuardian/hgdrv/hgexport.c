
#include "hgexport.h"
#include "hglib.h"
#include "hgheader.h"

ULONG			g_WindowsBuildNumber	= 0;
ULONG			g_WindowsVersion		= 0;
PDRIVER_OBJECT	g_pHGDrvObj				= NULL;
BOOL			g_bReloadKernelModule	= FALSE;
ULONG_PTR		g_hg_map[HG_MAP_LENGTH] = {0};
LIST_ENTRY		g_hg_map_list[100]		= {0};
LIST_ENTRY		g_tmp_process_list;
KSPIN_LOCK		g_tmp_process_spin_lock;


//
// static variable
// 
static ULONG_PTR	g_field_offset[1024]	= {0};
static ULONG_PTR	g_Miscellaneous[128]	= {0};
static ULONG_PTR	g_SSDT[1024]			= {0};
static ULONG_PTR	g_ShadowSSDT[1024]		= {0};

VOID SetOffset(SIZE_T id, ULONG_PTR v){
	g_field_offset[id] = v;
}

ULONG_PTR GetOffset(SIZE_T id){
	return g_field_offset[id];
}

VOID SetMiscellaneous(SIZE_T id, ULONG_PTR v){
	g_Miscellaneous[id] = v;
}

ULONG_PTR GetMiscellaneous(SIZE_T id){
	return g_Miscellaneous[id];
}

VOID SetSSDT(SIZE_T id, ULONG_PTR v){
	g_SSDT[id] = v;
}

ULONG_PTR GetSSDT(SIZE_T id){
	return g_SSDT[id];
}

VOID SetShadowSSDT(SIZE_T id, ULONG_PTR v){
	g_ShadowSSDT[id] = v;
}

ULONG_PTR GetShadowSSDT(SIZE_T id){
	return g_ShadowSSDT[id];
}


//////////////////////////////////////////////////////////////////////////
POBJECT_TYPE HgObGetObjectType(PVOID pObject)
{
	POBJECT_TYPE pObjectType = NULL;
	SIZE_T _OBJECT_HEADER_TypeIndex_ReverseOffset = 0;
	if ((pObject == NULL)					||
		(HgMmIsAddressValid(pObject) == FALSE))
	{
		return NULL;
	}

	switch (g_WindowsVersion)
	{
	case eWin_2000:
	case eWin_XP:
	case eWin_2003:
	case eWin_2003_SP1_SP2:
	case eWin_Vista:
	case eWin_Vista_SP1_SP2:		
		{
			size_t SizeOfObjectHeader = 0;
			size_t ObjectTypeOffset = 0;
			size_t ObjectTypeAddress = 0;
			SizeOfObjectHeader = GetOffset(_OBJECT_HEADER_sizeof);
			ObjectTypeOffset = 
				(size_t)g_hg_map[eHG_ObjectTypeOffset_OBJECT_HEADER];
			ObjectTypeAddress = 
				(size_t)pObject - SizeOfObjectHeader + ObjectTypeOffset;

			if (HgMmIsAddressValid((PVOID)ObjectTypeAddress) == TRUE)
			{
				pObjectType = *(POBJECT_TYPE*)ObjectTypeAddress;
			}
		}
		break;
	case eWin_7:
	case eWin_8:
		{
			pfnObGetObjectType ObGetObjectType = NULL;
			if (g_bReloadKernelModule == TRUE){
				ExportLog(("<<Using Reloaded Function: ObGetObjectType>>\n"));
				ObGetObjectType = (pfnObGetObjectType)g_hg_map[eHG_ObGetObjectType];
			}else{
				if (g_hg_map[eHG_ObGetObjectType] != 0){
//					KdPrint(("1-------------------------ObGetObjectType != NULL\n"));
					ObGetObjectType = (pfnObGetObjectType)g_hg_map[eHG_ObGetObjectType];
				}else{
					WCHAR pwszName[] = L"ObGetObjectType";
					UNICODE_STRING uName;
					KdPrint(("2-------------------------ObGetObjectType == NULL\n"));
					RtlInitUnicodeString(&uName, pwszName);	
					ObGetObjectType = (pfnObGetObjectType)MmGetSystemRoutineAddress(&uName);
					if (ObGetObjectType != NULL){
						g_hg_map[eHG_ObGetObjectType] = (SIZE_T)ObGetObjectType;
						KdPrint(("-----------ObGetObjectType = 0x%p\n", ObGetObjectType));
					}else{
						KdPrint(("-----------Get ObGetObjectType Error-------------\n"));
					}
				}
			}
			if (ObGetObjectType != NULL && HgMmIsAddressValid(ObGetObjectType))
			{
//				KdPrint(("ObGetObjectType = 0x%p, pObject = 0x%p\n", ObGetObjectType, pObject));
#if defined(_WIN64)
				// In windows 64-Bit, _OBJECT_HEADER is before object, and 0x18 is the sizeof(_OBJECT_HEADER) minus TypeIndex offset.
				// Disassembly ObGetObjectType
				_OBJECT_HEADER_TypeIndex_ReverseOffset = 0x18;
#else
				_OBJECT_HEADER_TypeIndex_ReverseOffset = 0x0C;
#endif
				if (HgMmIsAddressValid((PVOID)((SIZE_T)pObject - _OBJECT_HEADER_TypeIndex_ReverseOffset)) == FALSE)	{
					return NULL;
				}
				pObjectType = ObGetObjectType(pObject);
			}
		}
		break;
	}
	return pObjectType;
}

NTSTATUS HgObReferenceObjectByPointer(
	__in	PVOID				Object,
	__in	ACCESS_MASK			DesireAccess,
	__in	POBJECT_TYPE		ObjectType,
	__in	KPROCESSOR_MODE		AccessMode)
{
	pfnObReferenceObjectByPointer pObReferenceObjectByPointer = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ObReferenceObjectByPointer>>\n"));
		pObReferenceObjectByPointer = (pfnObReferenceObjectByPointer)g_hg_map[eHG_ObReferenceObjectByPointer];
	}else{
		pObReferenceObjectByPointer = &ObReferenceObjectByPointer;
	}

	if (pObReferenceObjectByPointer != NULL){
		return pObReferenceObjectByPointer(Object, DesireAccess, ObjectType, AccessMode);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

HANDLE HgPsGetThreadId(
	__in	PETHREAD	Thread)
{
	pfnPsGetThreadId pPsGetThreadId = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsGetThreadId>>\n"));
		pPsGetThreadId = (pfnPsGetThreadId)g_hg_map[eHG_PsGetThreadId];
	}else{
		pPsGetThreadId = &PsGetThreadId;
	}

	if (pPsGetThreadId != NULL){
		return pPsGetThreadId(Thread);
	}else{
		return NULL;
	}
}

NTSTATUS HgZwOpenDirectoryObject(
	__out	PHANDLE				DirectoryHandle,
	__in	ACCESS_MASK			DesiredAccess,
	__in	POBJECT_ATTRIBUTES	ObjectAttributes)
{
	pfnZwOpenDirectoryObject pZwOpenDirectoryObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ZwOpenDirectoryObject>>\n"));
		pZwOpenDirectoryObject = (pfnZwOpenDirectoryObject)g_hg_map[eHG_NtOpenDirectoryObject];
	}else{
		pZwOpenDirectoryObject = &ZwOpenDirectoryObject;
	}

	if (pZwOpenDirectoryObject != NULL){
		return pZwOpenDirectoryObject(DirectoryHandle, DesiredAccess, ObjectAttributes);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
HgObReferenceObjectByHandle(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation)
{
	pfnObReferenceObjectByHandle pObReferenceObjectByHandle = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ObReferenceObjectByHandle>>\n"));
		pObReferenceObjectByHandle = (pfnObReferenceObjectByHandle)g_hg_map[eHG_ObReferenceObjectByHandle];
	}else{
		pObReferenceObjectByHandle = &ObReferenceObjectByHandle;
	}

	if (pObReferenceObjectByHandle != NULL){
		return pObReferenceObjectByHandle(Handle, DesiredAccess, ObjectType, AccessMode, Object, HandleInformation);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

#pragma warning(disable:4996)
BOOLEAN
HgMmIsNonPagedSystemAddressValid(
	__in PVOID VirtualAddress
	)
{
	pfnMmIsNonPagedSystemAddressValid pMmIsNonPagedSystemAddressValid = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmIsNonPagedSystemAddressValid>>\n"));
		pMmIsNonPagedSystemAddressValid = (pfnMmIsNonPagedSystemAddressValid)g_hg_map[eHG_MmIsNonPagedSystemAddressValid];
	}else{
		pMmIsNonPagedSystemAddressValid = &MmIsNonPagedSystemAddressValid;	// warning 4996
	}
	if (pMmIsNonPagedSystemAddressValid != NULL){
		return pMmIsNonPagedSystemAddressValid(VirtualAddress);
	}else{
		return FALSE;
	}
}

PHYSICAL_ADDRESS HgMmGetPhysicalAddress(
	__in PVOID BaseAddress)
{
	pfnMmGetPhysicalAddress pMmGetPhysicalAddress = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmGetPhysicalAddress>>\n"));
		pMmGetPhysicalAddress = (pfnMmGetPhysicalAddress)g_hg_map[eHG_MmGetPhysicalAddress];
	}else{
		pMmGetPhysicalAddress = &MmGetPhysicalAddress;
	}

	return pMmGetPhysicalAddress(BaseAddress);
}

PVOID
HgMmGetVirtualForPhysical (
	__in PHYSICAL_ADDRESS PhysicalAddress
	)
{
	pfnMmGetVirtualForPhysical pMmGetVirtualForPhysical = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmGetVirtualForPhysical>>\n"));
		pMmGetVirtualForPhysical = (pfnMmGetVirtualForPhysical)g_hg_map[eHG_MmGetVirtualForPhysical];
	}else{
		pMmGetVirtualForPhysical = &MmGetVirtualForPhysical;
	}
	if (pMmGetVirtualForPhysical != NULL){
		return pMmGetVirtualForPhysical(PhysicalAddress);
	}else{
		return NULL;
	}
}

PMDL HgIoAllocateMdl(
	__in_opt __drv_aliasesMem PVOID VirtualAddress,
	__in ULONG Length,
	__in BOOLEAN SecondaryBuffer,
	__in BOOLEAN ChargeQuota,
	__inout_opt PIRP Irp)
{
	pfnIoAllocateMdl pIoAllocateMdl = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoAllocateMdl>>\n"));
		pIoAllocateMdl = (pfnIoAllocateMdl)g_hg_map[eHG_IoAllocateMdl];
	}else{
		pIoAllocateMdl = &IoAllocateMdl;
	}
	if (pIoAllocateMdl != NULL){
		return pIoAllocateMdl(VirtualAddress, Length, SecondaryBuffer, ChargeQuota, Irp);
	}else{
		return NULL;
	}
}

VOID HgMmBuildMdlForNonPagedPool(
	__inout PMDLX MemoryDescriptorList)
{
	pfnMmBuildMdlForNonPagedPool pMmBuildMdlForNonPagedPool = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmBuildMdlForNonPagedPool>>\n"));
		pMmBuildMdlForNonPagedPool = (pfnMmBuildMdlForNonPagedPool)g_hg_map[eHG_MmBuildMdlForNonPagedPool];
	}else{
		pMmBuildMdlForNonPagedPool = &MmBuildMdlForNonPagedPool;
	}

	if (pMmBuildMdlForNonPagedPool != NULL){
		pMmBuildMdlForNonPagedPool(MemoryDescriptorList);
	}
}

PVOID HgMmMapLockedPagesSpecifyCache (
	__in PMDLX MemoryDescriptorList,
	__in __drv_strictType(KPROCESSOR_MODE/enum _MODE,__drv_typeConst) KPROCESSOR_MODE AccessMode,
	__in __drv_strictTypeMatch(__drv_typeCond) MEMORY_CACHING_TYPE CacheType,
	__in_opt PVOID BaseAddress,
	__in ULONG BugCheckOnFailure,
	__in __drv_strictTypeMatch(__drv_typeCond) MM_PAGE_PRIORITY Priority
	)
{
	pfnMmMapLockedPagesSpecifyCache pMmMapLockedPagesSpecifyCache = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmMapLockedPagesSpecifyCache>>\n"));
		pMmMapLockedPagesSpecifyCache = (pfnMmMapLockedPagesSpecifyCache)g_hg_map[eHG_MmMapLockedPagesSpecifyCache];
	}else{
		pMmMapLockedPagesSpecifyCache = &MmMapLockedPagesSpecifyCache;
	}
	if (pMmMapLockedPagesSpecifyCache != NULL){
		return pMmMapLockedPagesSpecifyCache(MemoryDescriptorList, AccessMode, CacheType, BaseAddress, BugCheckOnFailure, Priority);
	}else{
		return NULL;
	}
}

VOID HgMmUnmapLockedPages (
	__in PVOID BaseAddress,
	__in PMDL MemoryDescriptorList)
{
	pfnMmUnmapLockedPages pMmUnmapLockedPages = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmUnmapLockedPages>>\n"));
		pMmUnmapLockedPages = (pfnMmUnmapLockedPages)g_hg_map[eHG_MmUnmapLockedPages];
	}else{
		pMmUnmapLockedPages = &MmUnmapLockedPages;
	}

	if (pMmUnmapLockedPages != NULL){
		pMmUnmapLockedPages(BaseAddress, MemoryDescriptorList);
	}
}

VOID
HgIoFreeMdl(
	PMDL Mdl
	)
{
	pfnIoFreeMdl pIoFreeMdl = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoFreeMdl>>\n"));
		pIoFreeMdl = (pfnIoFreeMdl)g_hg_map[eHG_IoFreeMdl];
	}else{
		pIoFreeMdl = &IoFreeMdl;
	}

	if (pIoFreeMdl != NULL){
		pIoFreeMdl(Mdl);
	}
}

BOOL
HgGetWindowsDirectory(
	__inout	PWCHAR	pwszRootName
	)
{
	UNICODE_STRING RootName;
	UNICODE_STRING ObjectName;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE hLink = NULL;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	BOOL bRtn = FALSE;
	HANDLE hFile = NULL;
	WCHAR pwszSystemRoot[] = L"\\SystemRoot";
	IO_STATUS_BLOCK IoStatusBlock;
	POBJECT_TYPE FileObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoFileObjectType];
	PFILE_OBJECT pFileObject = NULL;
	UNICODE_STRING pDosName;
	WCHAR pwszWindowsDir[SZ_PATH_LEN] = {0};

	if (pwszRootName == NULL){
		return FALSE;
	}
	DrvLog(("[hgexport.c].[HgGetWindowsDirectory]: Enter!\n"));
	SzhInitUnicodeString(&RootName, pwszSystemRoot);
	InitializeObjectAttributes(&ObjectAttributes, &RootName, OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = HgIoCreateFile(&hFile,
							SYNCHRONIZE,
							&ObjectAttributes,
							&IoStatusBlock,
							NULL,
							0,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							FILE_OPEN,
							FILE_DIRECTORY_FILE,
							0,
							0,
							CreateFileTypeNone,
							0,
							IO_NO_PARAMETER_CHECKING);
	
	if (NT_SUCCESS(status)	&&
		NT_SUCCESS(HgObReferenceObjectByHandle(hFile, 0, FileObjectType, KernelMode, &pFileObject, NULL)) &&
		pFileObject->DeviceObject	&&
		NT_SUCCESS(HgIoVolumeDeviceToDosName(pFileObject->DeviceObject, &pDosName)))
	{
		WCHAR* ObjectNameBuffer = NULL;
		PETHREAD pThread = HgPsGetCurrentThread();
		CHAR PreMode = ChangePreMode(pThread);
		wcsncpy(pwszWindowsDir, pDosName.Buffer, pDosName.Length);
		ObjectNameBuffer = (PWCHAR)HgExAllocatePoolWithTag(PagedPool, SZ_PATH_LEN_WCHAR, HG_MEM_TAG);
		if (ObjectNameBuffer)
		{
			memset(ObjectNameBuffer, 0, SZ_PATH_LEN_WCHAR);
			status = HgNtOpenSymbolicLinkObject(&hLink, GENERIC_READ, &ObjectAttributes);
			if (NT_SUCCESS(status)){
				ObjectName.Buffer = ObjectNameBuffer;
				ObjectName.Length = 0;
				ObjectName.MaximumLength = SZ_PATH_LEN;
				status = HgNtQuerySymbolicLinkObject(hLink, &ObjectName, NULL);
				if (NT_SUCCESS(status)){
					SIZE_T nLen = ObjectName.Length / sizeof(WCHAR);
					SIZE_T i = 0;
					for (i = nLen - 1; i > 0; i--)
					{
						if (ObjectNameBuffer[i] == L'\\')
						{
							wcsncat(pwszWindowsDir, &ObjectNameBuffer[i], nLen - i);
							bRtn = TRUE;
							break;
						}
					}
				}
			}
			HgExFreePoolWithTag(ObjectNameBuffer, 0);
		}
		if (hFile){
			HgNtClose(hFile);
		}
		if (hLink){
			HgNtClose(hLink);
		}
		if (pFileObject){
			HgObfDereferenceObject(pFileObject);
		}
		HgExFreePoolWithTag(pDosName.Buffer, 0);
		RecoverPreMode(pThread, PreMode);
	}

	if (bRtn == FALSE){
		__try{
			WCHAR* SystemRootName = (WCHAR*)0x7FFE0030;
			WCHAR pwszWindows[] = L"X:\\WINDOWS";
			if (HgMmIsAddressValid(SystemRootName)){
				HgProbeForRead(SystemRootName, wcslen(pwszWindows) * sizeof(WCHAR), sizeof(WCHAR));

				if (SystemRootName[1] == L':' && SystemRootName[2] == L'\\')
				{
					wcsncat(pwszWindowsDir, SystemRootName, wcslen(pwszWindows));
					bRtn = TRUE;
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ExportLog(("[hgexprot.c].[HgGetWindowsDirectory]: Catch exception!\n"));
		}
	}

	if (bRtn == TRUE){
		wcscpy(pwszRootName, pwszWindowsDir);
	}

	return bRtn;
}

BOOL
HgGetDriverDirectory(
	__inout PWCHAR	pwszDir
	)
{
	BOOL bRtn = FALSE;
	WCHAR pwszWindowsDirectory[SZ_PATH_LEN] = {0};

	if (HgGetWindowsDirectory(pwszWindowsDirectory))
	{
		WCHAR pwszDriver[] = L"\\system32\\drivers\\";
		wcscat(pwszWindowsDirectory, pwszDriver);
		wcscpy(pwszDir, pwszWindowsDirectory);
		bRtn = TRUE;
	}

	return bRtn;
}

NTSTATUS
NTAPI
HgNtOpenSymbolicLinkObject(
	__out PHANDLE LinkHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	)
{
	pfnNtOpenSymbolicLinkObject pNtOpenSymbolicLinkObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtOpenSymbolicLinkObject>>\n"));
		pNtOpenSymbolicLinkObject = (pfnNtOpenSymbolicLinkObject)g_hg_map[eHG_NtOpenSymbolicLinkObject];
	}else{
		pNtOpenSymbolicLinkObject = &ZwOpenSymbolicLinkObject;
	}

	if (pNtOpenSymbolicLinkObject != NULL){
		return pNtOpenSymbolicLinkObject(LinkHandle, DesiredAccess, ObjectAttributes);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
NTAPI
HgNtQuerySymbolicLinkObject(
	__in HANDLE LinkHandle,
	__inout PUNICODE_STRING LinkTarget,
	__out_opt PULONG ReturnedLength
	)
{
	pfnNtQuerySymbolicLinkObject pNtQuerySymbolicLinkObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtQuerySymbolicLinkObject>>\n"));
		pNtQuerySymbolicLinkObject = (pfnNtQuerySymbolicLinkObject)g_hg_map[eHG_NtQuerySymbolicLinkObject];
	}else{
		pNtQuerySymbolicLinkObject = &ZwQuerySymbolicLinkObject;
	}

	if (pNtQuerySymbolicLinkObject != NULL){
		return pNtQuerySymbolicLinkObject(LinkHandle, LinkTarget, ReturnedLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
NTAPI
HgNtClose (
	__in HANDLE Handle
	)
{
	pfnNtClose pNtClose = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtClose>>\n"));
		pNtClose = (pfnNtClose)g_hg_map[eHG_NtClose];
	}else{
		pNtClose = &NtClose;
	}

	if (pNtClose != NULL){
		return pNtClose(Handle);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
HgIoCreateFile(
	__out PHANDLE FileHandle,
	__in  ACCESS_MASK DesiredAccess,
	__in  POBJECT_ATTRIBUTES ObjectAttributes,
	__out PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt PLARGE_INTEGER AllocationSize,
	__in  ULONG FileAttributes,
	__in  ULONG ShareAccess,
	__in  ULONG Disposition,
	__in  ULONG CreateOptions,
	__in_opt PVOID EaBuffer,
	__in  ULONG EaLength,
	__in  CREATE_FILE_TYPE CreateFileType,
	__in_opt PVOID InternalParameters,
	__in  ULONG Options
	)
{
	pfnIoCreateFile pIoCreateFile = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoCreateFile>>\n"));
		pIoCreateFile = (pfnIoCreateFile)g_hg_map[eHG_IoCreateFile];
	}else{
		pIoCreateFile = &IoCreateFile;
	}

	if (pIoCreateFile != NULL){
		return pIoCreateFile(	FileHandle,
								DesiredAccess,
								ObjectAttributes,
								IoStatusBlock,
								AllocationSize,
								FileAttributes,
								ShareAccess,
								Disposition,
								CreateOptions,
								EaBuffer,
								EaLength,
								CreateFileType,
								InternalParameters,
								Options);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
NTAPI
HgZwOpenKey(
	__out PHANDLE KeyHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	)
{
	pfnZwOpenKey pZwOpenKey = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ZwOpenKey>>\n"));
		pZwOpenKey = (pfnZwOpenKey)g_hg_map[eHG_NtOpenKey];
	}else{
		pZwOpenKey = &ZwOpenKey;
	}

	if (pZwOpenKey != NULL){
		return pZwOpenKey(KeyHandle, DesiredAccess, ObjectAttributes);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
NTAPI
HgZwEnumerateKey(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_INFORMATION_CLASS KeyInformationClass,
	__out_bcount_opt(Length) PVOID KeyInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	)
{
	pfnZwEnumerateKey pZwEnumerateKey = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ZwEnumerateKey>>\n"));
		pZwEnumerateKey = (pfnZwEnumerateKey)g_hg_map[eHG_NtEnumerateKey];
	}else{
		pZwEnumerateKey = &ZwEnumerateKey;
	}

	if (pZwEnumerateKey != NULL){
		return pZwEnumerateKey(KeyHandle, Index, KeyInformationClass, KeyInformation, Length, ResultLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS
NTAPI
HgZwEnumerateValueKey(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	__out_bcount_opt(Length) PVOID KeyValueInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	)
{
	pfnZwEnumerateValueKey pZwEnumerateValueKey = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ZwEnumerateValueKey>>\n"));
		pZwEnumerateValueKey = (pfnZwEnumerateValueKey)g_hg_map[eHG_NtEnumerateValueKey];
	}else{
		pZwEnumerateValueKey = &ZwEnumerateValueKey;
	}

	if (pZwEnumerateValueKey != NULL){
		return pZwEnumerateValueKey(KeyHandle, Index, KeyValueInformationClass, KeyValueInformation, Length, ResultLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgPsSetCreateProcessNotifyRoutine(
	__in PCREATE_PROCESS_NOTIFY_ROUTINE NotifyRoutine,
	__in BOOLEAN Remove)
{
	pfnPsSetCreateProcessNotifyRoutine pPsSetCreateProcessNotifyRoutine = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsSetCreateProcessNotifyRoutine>>\n"));
		pPsSetCreateProcessNotifyRoutine = (pfnPsSetCreateProcessNotifyRoutine)g_hg_map[eHG_PsSetCreateProcessNotifyRoutine];
	}else{
		pPsSetCreateProcessNotifyRoutine = &PsSetCreateProcessNotifyRoutine;
	}

	if (pPsSetCreateProcessNotifyRoutine != NULL){
		return pPsSetCreateProcessNotifyRoutine(NotifyRoutine, Remove);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgPsSetCreateThreadNotifyRoutine(
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	)
{
	pfnPsSetCreateThreadNotifyRoutine pPsSetCreateThreadNotifyRoutine = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsSetCreateThreadNotifyRoutine>>\n"));
		pPsSetCreateThreadNotifyRoutine = (pfnPsSetCreateThreadNotifyRoutine)g_hg_map[eHG_PsSetCreateThreadNotifyRoutine];
	}else{
		pPsSetCreateThreadNotifyRoutine = &PsSetCreateThreadNotifyRoutine;
	}

	if (pPsSetCreateThreadNotifyRoutine != NULL){
		return pPsSetCreateThreadNotifyRoutine(NotifyRoutine);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgPsSetLoadImageNotifyRoutine(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	)
{
	pfnPsSetLoadImageNotifyRoutine pPsSetLoadImageNotifyRoutine = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsSetLoadImageNotifyRoutine>>\n"));
		pPsSetLoadImageNotifyRoutine = (pfnPsSetLoadImageNotifyRoutine)g_hg_map[eHG_PsSetLoadImageNotifyRoutine];
	}else{
		pPsSetLoadImageNotifyRoutine = &PsSetLoadImageNotifyRoutine;
	}

	if (pPsSetLoadImageNotifyRoutine != NULL){
		return pPsSetLoadImageNotifyRoutine(NotifyRoutine);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgPsRemoveCreateThreadNotifyRoutine(
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	)
{
	pfnPsRemoveCreateThreadNotifyRoutine pPsRemoveCreateThreadNotifyRoutine = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsRemoveCreateThreadNotifyRoutine>>\n"));
		pPsRemoveCreateThreadNotifyRoutine = (pfnPsRemoveCreateThreadNotifyRoutine)g_hg_map[eHG_PsRemoveCreateThreadNotifyRoutine];
	}else{
		pPsRemoveCreateThreadNotifyRoutine = &PsRemoveCreateThreadNotifyRoutine;
	}

	if (pPsRemoveCreateThreadNotifyRoutine != NULL){
		return pPsRemoveCreateThreadNotifyRoutine(NotifyRoutine);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgPsRemoveLoadImageNotifyRoutine(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	)
{
	pfnPsRemoveLoadImageNotifyRoutine pPsRemoveLoadImageNotifyRoutine = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsRemoveLoadImageNotifyRoutine>>\n"));
		pPsRemoveLoadImageNotifyRoutine = (pfnPsRemoveLoadImageNotifyRoutine)g_hg_map[eHG_PsRemoveLoadImageNotifyRoutine];
	}else{
		pPsRemoveLoadImageNotifyRoutine = &PsRemoveLoadImageNotifyRoutine;
	}

	if (pPsRemoveLoadImageNotifyRoutine != NULL){
		return pPsRemoveLoadImageNotifyRoutine(NotifyRoutine);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgIoRegisterShutdownNotification(
	__in PDEVICE_OBJECT DeviceObject
	)
{
	pfnIoRegisterShutdownNotification pIoRegisterShutdownNotification = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoRegisterShutdownNotification>>\n"));
		pIoRegisterShutdownNotification = (pfnIoRegisterShutdownNotification)g_hg_map[eHG_IoRegisterShutdownNotification];
	}else{
		pIoRegisterShutdownNotification = &IoRegisterShutdownNotification;
	}

	if (pIoRegisterShutdownNotification != NULL){
		return pIoRegisterShutdownNotification(DeviceObject);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgCmUnRegisterCallback(
	__in LARGE_INTEGER    Cookie
	)
{
	pfnCmUnRegisterCallback pCmUnRegisterCallback = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: CmUnRegisterCallback>>\n"));
		pCmUnRegisterCallback = (pfnCmUnRegisterCallback)g_hg_map[eHG_CmUnRegisterCallback];
	}else{
		pCmUnRegisterCallback = &CmUnRegisterCallback;
	}

	if (pCmUnRegisterCallback != NULL){
		return pCmUnRegisterCallback(Cookie);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

VOID HgIoUnregisterShutdownNotification(
	__in PDEVICE_OBJECT DeviceObject
	)
{
	pfnIoUnregisterShutdownNotification pIoUnregisterShutdownNotification = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoUnregisterShutdownNotification>>\n"));
		pIoUnregisterShutdownNotification = (pfnIoUnregisterShutdownNotification)g_hg_map[eHG_IoUnregisterShutdownNotification];
	}else{
		pIoUnregisterShutdownNotification = &IoUnregisterShutdownNotification;
	}

	if (pIoUnregisterShutdownNotification != NULL){
		pIoUnregisterShutdownNotification(DeviceObject);
	}
}

NTSTATUS HgNtQueryVirtualMemory(
	__in HANDLE ProcessHandle,
	__in PVOID BaseAddress,
	__in MEMORY_INFORMATION_CLASS MemoryInformationClass,
	__out_bcount(MemoryInformationLength) PVOID MemoryInformation,
	__in SIZE_T MemoryInformationLength,
	__out_opt PSIZE_T ReturnLength
	)
{
	pfnNtQueryVirtualMemory pNtQueryVirtualMemory = NULL;
#if defined(_WIN64)
	pNtQueryVirtualMemory = (pfnNtQueryVirtualMemory)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalSSDT64], g_hg_map[eHG_NtQueryVirtualMemoryIndex]);
#else
	pNtQueryVirtualMemory = (pfnNtQueryVirtualMemory)g_hg_map[eHG_NtQueryVirtualMemory];
#endif
	if (pNtQueryVirtualMemory != NULL){
		PETHREAD pThread = HgPsGetCurrentThread();
		CHAR PreMode = ChangePreMode(pThread);
		NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
		__try{
			ntStatus = pNtQueryVirtualMemory(ProcessHandle, BaseAddress, MemoryInformationClass, MemoryInformation, MemoryInformationLength, ReturnLength);
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("%s(%d): Catch Exception!\n", __FILE__, __LINE__));
			ntStatus = STATUS_UNSUCCESSFUL;
		}
		RecoverPreMode(pThread, PreMode);
		return ntStatus;
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS __stdcall HgNtProtectVirtualMemory(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	UnsafeBaseAddress,
	__inout	PSIZE_T	UnsafeNumberOfBytesToProtect,
	__in	ULONG	NewAccessProtection,
	__out	PULONG	UnsafeOldAccessProtection)
{
	pfnNtProtectVirtualMemory pNtProtectVirtualMemory = NULL;
#if defined(_WIN64)
	pNtProtectVirtualMemory = (pfnNtProtectVirtualMemory)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalSSDT64], g_hg_map[eHG_NtProtectVirtualMemoryIndex]);
#else
	pNtProtectVirtualMemory = (pfnNtProtectVirtualMemory)g_hg_map[eHG_NtProtectVirtualMemory];
#endif
	if (pNtProtectVirtualMemory != NULL){
		PETHREAD pThread = HgPsGetCurrentThread();
		CHAR PreMode = ChangePreMode(pThread);
		NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
		ntStatus = pNtProtectVirtualMemory(ProcessHandle, UnsafeBaseAddress, UnsafeNumberOfBytesToProtect, NewAccessProtection, UnsafeOldAccessProtection);
		RecoverPreMode(pThread, PreMode);
		return ntStatus;
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS __stdcall HgNtFreeVirtualMemory(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	BaseAddress,
	__inout	PSIZE_T	RegionSize,
	__in	ULONG	FreeType)
{
	pfnNtFreeVirtualMemory pNtFreeVirtualMemory = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtFreeVirtualMemory>>\n"));
#if defined(_WIN64)
		pNtFreeVirtualMemory = (pfnNtFreeVirtualMemory)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalSSDT64], g_hg_map[eHG_NtFreeVirtualMemoryIndex]);
#else
		pNtFreeVirtualMemory = (pfnNtFreeVirtualMemory)g_hg_map[eHG_NtFreeVirtualMemory];
#endif
	}else{
		pNtFreeVirtualMemory = &NtFreeVirtualMemory;
	}

	if (pNtFreeVirtualMemory != NULL){
		return pNtFreeVirtualMemory(ProcessHandle, BaseAddress, RegionSize, FreeType);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS HgObOpenObjectByPointer(
	__in PVOID  Object,
	__in ULONG  HandleAttributes,
	__in_opt PACCESS_STATE  PassedAccessState,
	__in ACCESS_MASK  DesiredAccess,
	__in POBJECT_TYPE  ObjectType,
	__in KPROCESSOR_MODE  AccessMode,
	__out	PHANDLE  Handle
	)
{
	pfnObOpenObjectByPointer pObOpenObjectByPointer = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ObOpenObjectByPointer>>\n"));
		pObOpenObjectByPointer = (pfnObOpenObjectByPointer)g_hg_map[eHG_ObOpenObjectByPointer];
	}else{
		pObOpenObjectByPointer = &ObOpenObjectByPointer;
	}

	if (pObOpenObjectByPointer != NULL){
		return pObOpenObjectByPointer(Object, HandleAttributes, PassedAccessState, DesiredAccess, ObjectType, AccessMode, Handle);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS NTAPI	HgNtQueryObject (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength,
	__out_opt PULONG ReturnLength)
{
	pfnNtQueryObject pNtQueryObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtQueryObject>>\n"));
		pNtQueryObject = (pfnNtQueryObject)g_hg_map[eHG_NtQueryObject];
	}else{
		pNtQueryObject = &ZwQueryObject;
	}

	if (pNtQueryObject != NULL){
		return pNtQueryObject(
			Handle, 
			ObjectInformationClass, 
			ObjectInformation, 
			ObjectInformationLength, 
			ReturnLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

NTSTATUS NTAPI HgNtSetInformationObject (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__in_bcount(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength)
{
	pfnNtSetInformationObject pNtSetInformationObject = NULL;
	pNtSetInformationObject = (pfnNtSetInformationObject)g_hg_map[eHG_NtSetInformationObject];

	if (pNtSetInformationObject != NULL){
		return pNtSetInformationObject(
			Handle, 
			ObjectInformationClass, 
			ObjectInformation, 
			ObjectInformationLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}