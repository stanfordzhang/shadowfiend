
#include "reload32.h"

extern PDRIVER_OBJECT	g_pHGDrvObj;

BOOL
ReloadNtoskrnl32(
	)
{
	BOOL bRet = FALSE;
	SIZE_T KernelModuleBase = 0;
	SIZE_T KernelModuleSize = 0;
	SIZE_T NewNtoskrnlBaseRepaired = 0;
	SIZE_T NewNtoskrnlBaseUnrepaired = 0;

	PVOID OriginKiServiceTable = NULL;
	PVOID ReloadKiServiceTable = NULL;
	PKSERVICE_TABLE_DESCRIPTOR OriginSSDT = NULL;
	PKSERVICE_TABLE_DESCRIPTOR ReloadSSDT = NULL;

	WCHAR pwszNtoskrnlPath[SZ_PATH_LEN] = {0};

	if (GetNtoskrnlInfo(pwszNtoskrnlPath, &KernelModuleBase, &KernelModuleSize) == FALSE){
		Reload32Log(("Get ntoskrnl.exe info failed\n"));
		bRet = FALSE;
		return bRet;
	}

	g_hg_map[eHG_CurrentKernelBase] = KernelModuleBase;
	g_hg_map[eHG_CurrentKernelSize] = KernelModuleSize;

	Reload32Log(("ntoskrnl.exe = path: %S, Base: 0x%p, Size: 0x%p\n",	pwszNtoskrnlPath, KernelModuleBase, KernelModuleSize));

	// 
	if (PeLoad_Unrepair(pwszNtoskrnlPath, &NewNtoskrnlBaseUnrepaired)	&& 
		PeLoad_Repair(pwszNtoskrnlPath, &NewNtoskrnlBaseRepaired, KernelModuleBase))
	{
		Reload32Log(("NewImageBase_Unrepair = 0x%p\n", NewNtoskrnlBaseUnrepaired));
		Reload32Log(("NewImageBase = 0x%p\n", NewNtoskrnlBaseRepaired));

		g_hg_map[eHG_NewKernelBase] = NewNtoskrnlBaseRepaired;
		g_hg_map[eHG_NewKernelBaseUnrepair] = NewNtoskrnlBaseUnrepaired;

		OriginKiServiceTable = ExAllocatePoolWithTag(NonPagedPool, KeServiceDescriptorTable.NumberOfServices * sizeof(DWORD), HG_MEM_TAG);
		OriginSSDT = (PKSERVICE_TABLE_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool,sizeof(KSERVICE_TABLE_DESCRIPTOR), HG_MEM_TAG);
		ReloadKiServiceTable = ExAllocatePoolWithTag(NonPagedPool, KeServiceDescriptorTable.NumberOfServices * sizeof(DWORD), HG_MEM_TAG);
		ReloadSSDT = (PKSERVICE_TABLE_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool, sizeof(KSERVICE_TABLE_DESCRIPTOR), HG_MEM_TAG);

		if ((OriginKiServiceTable != NULL)	&&
			(OriginSSDT != NULL)			&&
			(ReloadKiServiceTable != NULL)	&&
			(ReloadSSDT != NULL))
		{
			RtlZeroMemory(OriginKiServiceTable, KeServiceDescriptorTable.NumberOfServices * sizeof(SIZE_T));
			RtlZeroMemory(OriginSSDT, sizeof(KSERVICE_TABLE_DESCRIPTOR));
			RtlZeroMemory(ReloadKiServiceTable,	KeServiceDescriptorTable.NumberOfServices * sizeof(SIZE_T));
			RtlZeroMemory(ReloadSSDT, sizeof(KSERVICE_TABLE_DESCRIPTOR));

			if (GetOriginalKiServiceTable(NewNtoskrnlBaseRepaired, KernelModuleBase, (PSIZE_T)&OriginKiServiceTable))
			{
				OriginSSDT->NumberOfServices = KeServiceDescriptorTable.NumberOfServices;
				OriginSSDT->ParamTableBase = KeServiceDescriptorTable.ParamTableBase;
				OriginSSDT->ServiceCounterTableBase = KeServiceDescriptorTable.ServiceCounterTableBase;
				OriginSSDT->ServiceTableBase = (PULONG_PTR)OriginKiServiceTable;

				g_hg_map[eHG_OriginKiServiceTable] = (SIZE_T)OriginKiServiceTable;
				g_hg_map[eHG_OriginSSDT] = (SIZE_T)OriginSSDT;

				if (FixOriginalKiServiceTable(	(PSIZE_T)OriginKiServiceTable,
												(PSIZE_T)ReloadKiServiceTable, 
												NewNtoskrnlBaseRepaired,
												KernelModuleBase))
				{
					ReloadSSDT->NumberOfServices = KeServiceDescriptorTable.NumberOfServices;
					ReloadSSDT->ParamTableBase = KeServiceDescriptorTable.ParamTableBase;
					ReloadSSDT->ServiceCounterTableBase = KeServiceDescriptorTable.ServiceCounterTableBase;
					ReloadSSDT->ServiceTableBase = (PULONG_PTR)ReloadKiServiceTable;

					g_hg_map[eHG_ReloadKiServiceTable] = (SIZE_T)ReloadKiServiceTable;
					g_hg_map[eHG_ReloadSSDT] = (SIZE_T)ReloadSSDT;

					// ’‚¿Ôƒ⁄¥Ê–π¬©
					bRet = TRUE;
					return bRet;
				}
			}
		}
	}
	if (NewNtoskrnlBaseRepaired){
		ExFreePool((PVOID)NewNtoskrnlBaseRepaired);
		NewNtoskrnlBaseRepaired = 0;
	}

	if (OriginKiServiceTable){
		ExFreePool((PVOID)OriginKiServiceTable);
		OriginKiServiceTable = NULL;
	}

	if (OriginSSDT){
		ExFreePool((PVOID)OriginSSDT);
		OriginSSDT = NULL;
	}

	if (ReloadKiServiceTable){
		ExFreePool((PVOID)ReloadKiServiceTable);
		ReloadKiServiceTable = NULL;
	}

	if (ReloadSSDT){
		ExFreePool((PVOID)ReloadSSDT);
		ReloadSSDT = NULL;
	}

	if (NewNtoskrnlBaseUnrepaired){
		ExFreePool((PVOID)NewNtoskrnlBaseUnrepaired);
		NewNtoskrnlBaseUnrepaired = 0;
	}

	return bRet;
}

BOOL 
PeLoad_Unrepair(
	__in	PWCHAR	pwszPath,
	__out	PSIZE_T	pNewBase
	)
{
	BOOL bRet = FALSE;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	HANDLE hFile;
	LARGE_INTEGER FileSize;
	DWORD Length = 0;
	PBYTE FileBuffer = NULL;
	PBYTE ImageBase = NULL;
	IO_STATUS_BLOCK IoStatus;
	ULONG i = 0 ;
	ULONG uLen = 0;

	if ((pwszPath == NULL) || (pNewBase == NULL)){
		return FALSE;
	}

	// Open target file
	status = KernelOpenFile(	pwszPath,
								&hFile,
								SYNCHRONIZE | FILE_EXECUTE,
								FILE_ATTRIBUTE_NORMAL,
								FILE_SHARE_READ,
								FILE_OPEN,
								FILE_SYNCHRONOUS_IO_NONALERT);
	if (!NT_SUCCESS(status)){
		return bRet;
	}
	
	// Get target file size
	status = KernelGetFileSize(hFile, &FileSize);
	if (!NT_SUCCESS(status)){
		ZwClose(hFile);
		return bRet;
	}

	Length = FileSize.LowPart;	// 
	FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, Length, HG_MEM_TAG);
	if (FileBuffer == NULL){
		do {
			i = 1;
			uLen = Length + 2045 * i;
			FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, uLen, 0);
		} while (FileBuffer == NULL && ++i < 10);
	}

	if (FileBuffer == NULL){
		ZwClose(hFile);
		return bRet;
	}

	// Read file to memory
	status = KernelReadFile(hFile, NULL, Length, FileBuffer, &IoStatus);
	if (!NT_SUCCESS(status)){
		ZwClose(hFile);
		ExFreePool(FileBuffer);
		return bRet;
	}

	ZwClose(hFile);

	// memory image
	if (ImagePEFile(FileBuffer, (PVOID*)&ImageBase) == FALSE){
		Reload32Log(("Image file failed!\n"));
		ExFreePool(FileBuffer);
		return bRet;
	}

	ExFreePool(FileBuffer);

	*pNewBase = (ULONG)ImageBase;
	bRet = TRUE;

	return bRet;
}

NTSTATUS
KernelOpenFile(
	__in	PWCHAR		pwszFilePath,
	__out	PHANDLE		FileHandle,
	__in	ACCESS_MASK	DesiredAccess,
	__in	ULONG		FileAttributes,
	__in	ULONG		ShareAccess,
	__in	ULONG		CreateDisposition,
	__in	ULONG		CreateOptions
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	WCHAR SystemRootName[32] = L"\\SystemRoot";
	WCHAR FileNodeName[SZ_PATH_LEN] = {0};
	UNICODE_STRING FilePath;
	PDEVICE_OBJECT RealDevice = NULL;
	PDEVICE_OBJECT DeviceObject = NULL;
	PFILE_OBJECT FileObject = NULL;
	ULONG uLen = 0;

	if (pwszFilePath == NULL){
		return status;
	}

	Reload32Log(("pwszFilePath = %S\n", pwszFilePath));
	if (_wcsnicmp(pwszFilePath, SystemRootName, wcslen(SystemRootName)) == 0){
		if (GetWindowsRootName(FileNodeName) == FALSE){
			return status;
		}
		uLen = wcslen(SystemRootName);
		wcscat(FileNodeName, &pwszFilePath[uLen]);
	}else{
		if (pwszFilePath[1] != L':' || pwszFilePath[2] != L'\\'){
			return status;
		}
		wcscpy(FileNodeName, &pwszFilePath[2]);
	}
	Reload32Log(("FileNodeName = %S\n", FileNodeName));
	Reload32Log(("pwszFilePath = %S\n", pwszFilePath));
	if (GetDeviceObjectFromFileFullName(pwszFilePath,
		&RealDevice, &DeviceObject) == FALSE)
	{
		Reload32Log(("Get Device object and real device object failed!\n"));
		return status;
	}

	RtlInitUnicodeString(&FilePath, FileNodeName);

	status = IrpCreateFile(	&FilePath,
							DesiredAccess,
							FileAttributes,
							ShareAccess,
							CreateDisposition,
							CreateOptions,
							DeviceObject,
							RealDevice,
							&FileObject);

	if (!NT_SUCCESS(status)){
		Reload32Log(("Irp Create File Failed!\n"));
		return status;
	}
	status = ObOpenObjectByPointer(	FileObject,
									OBJ_KERNEL_HANDLE,
									0,
									DesiredAccess | 0x100000,
									*IoFileObjectType,
									0,
									FileHandle);
	ObDereferenceObject(FileObject);

	return status;
}

BOOL
GetWindowsRootName(
	__in	WCHAR* pwszRootName
	)
{
	BOOL bRet = FALSE;

	UNICODE_STRING RootName;
	UNICODE_STRING ObjectName;
	OBJECT_ATTRIBUTES oa;
	HANDLE hLink;
	NTSTATUS status;
	PWCHAR ObjectNameBuffer;
	ULONG uLen = 0;
	ULONG i = 0;
	PWCHAR SystemRootName = (PWCHAR)0x7FFE0030;

	ObjectNameBuffer = (PWCHAR)ExAllocatePoolWithTag(	NonPagedPool, 
														SZ_PATH_LEN_WCHAR,
														HG_MEM_TAG);

	if (pwszRootName == NULL){
		return bRet;
	}

	if (ObjectNameBuffer != NULL){
		RtlZeroMemory(ObjectNameBuffer, SZ_PATH_LEN_WCHAR);
		RtlInitUnicodeString(&RootName, L"\\SystemRoot");
		InitializeObjectAttributes(	&oa, 
									&RootName, 
									OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
									NULL, 
									NULL);

		status = ZwOpenSymbolicLinkObject(&hLink, GENERIC_READ, &oa);
		if (NT_SUCCESS(status)){
			ObjectName.Buffer = ObjectNameBuffer;
			ObjectName.Length = 0;
			ObjectName.MaximumLength = SZ_PATH_LEN;

			status = ZwQuerySymbolicLinkObject(hLink, &ObjectName, NULL);
			if (NT_SUCCESS(status)){
				Reload32Log(("ObjectName.Buffer = %S\n", ObjectName.Buffer));
				uLen = ObjectName.Length / sizeof(WCHAR);
				for (i = uLen - 1; i > 0; i--){
					if (ObjectNameBuffer[i] == L'\\'){
						RtlCopyMemory(pwszRootName, &ObjectNameBuffer[i], (uLen - i) * sizeof(WCHAR));
						bRet = TRUE;
						break;
					}
				}
			}
			ZwClose(hLink);
		}
		ExFreePool(ObjectNameBuffer);
	}
	if (bRet == FALSE){
		__try{
			ProbeForRead(SystemRootName, wcslen(L"X:\\WINDOWS") * sizeof(WCHAR), sizeof(WCHAR));
			if (SystemRootName[1] == L':' && SystemRootName[2] == L'\\'){
				wcscpy(pwszRootName, &SystemRootName[2]);
				bRet = TRUE;
			}
		}__except(1){

		}
	}

	return bRet;
}

BOOL
GetDeviceObjectFromFileFullName(
	__in	PWCHAR			FileFullName,
	__out	PDEVICE_OBJECT*	RealDevice,
	__out	PDEVICE_OBJECT*	DeviceObject
	)
{
	BOOL bRet = FALSE;

	WCHAR pwszRootName[32] = {0};
	UNICODE_STRING uRootName;
	OBJECT_ATTRIBUTES oa;
	NTSTATUS status;
	HANDLE hFile;
	IO_STATUS_BLOCK IoStatus;
	PFILE_OBJECT FileObject = NULL;

	if (FileFullName[0] == L'\\'){
		wcscpy(pwszRootName, L"\\SystemRoot");
	}else{
		wcscpy(pwszRootName, L"\\DosDevices\\*:\\");
		pwszRootName[12] = FileFullName[0];
	}

	RtlInitUnicodeString(&uRootName, pwszRootName);
	InitializeObjectAttributes(&oa, &uRootName, 
		OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, NULL, NULL);

	status = IoCreateFile(	&hFile,
							SYNCHRONIZE,
							&oa,
							&IoStatus,
							0, 
							FILE_ATTRIBUTE_NORMAL,
							FILE_SHARE_READ | FILE_SHARE_WRITE,
							FILE_OPEN,
							FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT,
							NULL, 
							0,
							CreateFileTypeNone,
							NULL,
							IO_NO_PARAMETER_CHECKING);

	if (!NT_SUCCESS(status)){
		return bRet;
	}

	status = ObReferenceObjectByHandle(hFile, 1, *IoFileObjectType, 
		KernelMode, (PVOID*)&FileObject, NULL);

	if (!NT_SUCCESS(status)){
		ZwClose(hFile);
		return bRet;
	}
	if (IoGetFileSystemVpbInfo(FileObject, DeviceObject, RealDevice) == FALSE){
		ObDereferenceObject(FileObject);
		ZwClose(hFile);
		return bRet;
	}

	ObDereferenceObject(FileObject);
	ZwClose(hFile);
	bRet = TRUE;

	return bRet;
}

BOOL
IoGetFileSystemVpbInfo(
	__in	PFILE_OBJECT	FileObject,
	__out	PDEVICE_OBJECT*	DeviceObject,
	__out	PDEVICE_OBJECT* RealDevice
	)
{
	BOOL bRet = FALSE;

	if (FileObject->Vpb != NULL && FileObject->Vpb->DeviceObject != NULL){
		if (DeviceObject != NULL){
			*DeviceObject = FileObject->Vpb->DeviceObject;
		}
		if (RealDevice != NULL){
			*RealDevice = FileObject->Vpb->RealDevice;
		}
		bRet = TRUE;
	}else if(!(FileObject->Flags & FO_DIRECT_DEVICE_OPEN)	&&
				FileObject->DeviceObject->Vpb != NULL		&&
				FileObject->DeviceObject->Vpb->DeviceObject != NULL)
	{
		if (DeviceObject != NULL){
			*DeviceObject = FileObject->DeviceObject->Vpb->DeviceObject;
		}
		if (RealDevice != NULL){
			*RealDevice = FileObject->DeviceObject->Vpb->RealDevice;
		}
		bRet = TRUE;
	}else{
		if (DeviceObject != NULL){
			*DeviceObject = FileObject->DeviceObject;
		}
		if (RealDevice != NULL){
			*RealDevice = NULL;
		}
		bRet = TRUE;
	}

	return bRet;
}

NTSTATUS
KernelGetFileSize(
	__in	HANDLE			hFile,
	__out	PLARGE_INTEGER	FileSize
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;

	PFILE_OBJECT FileObject = NULL;
	PDEVICE_OBJECT DeviceObject = NULL;
	PDEVICE_OBJECT RealDevice = NULL;
	FILE_STANDARD_INFORMATION FileInformation;

	status = ObReferenceObjectByHandle(	hFile,
										0,
										*IoFileObjectType,
										KernelMode,
										(PVOID*)&FileObject,
										0);

	if (!NT_SUCCESS(status)){
		return status;
	}

	if (IoGetFileSystemVpbInfo(FileObject, &DeviceObject, &RealDevice) == FALSE){
		ObDereferenceObject(FileObject);
		return status;
	}

	status = IrpQueryInformationFile(	FileObject, 
										DeviceObject, 
										&FileInformation, 
										sizeof(FILE_STANDARD_INFORMATION),
										FileStandardInformation);

	if (!NT_SUCCESS(status)){
		ObDereferenceObject(FileObject);
		return status;
	}

	FileSize->HighPart = FileInformation.EndOfFile.HighPart;
	FileSize->LowPart = FileInformation.EndOfFile.LowPart;

	ObDereferenceObject(FileObject);

	return status;
}

NTSTATUS
KernelReadFile(
	__in	HANDLE				hFile,
	__out	PLARGE_INTEGER		ByteOffset,
	__out	ULONG				Length,
	__out	PVOID				FileBuffer,
	__out	PIO_STATUS_BLOCK	IoStatusBlock
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PFILE_OBJECT FileObject = NULL;
	PDEVICE_OBJECT DeviceObject = NULL;
	PDEVICE_OBJECT RealDevice = NULL;
	FILE_STANDARD_INFORMATION FileInformation;

	status = ObReferenceObjectByHandle(hFile, 0, *IoFileObjectType,
		KernelMode, (PVOID*)&FileObject, 0);

	if (!NT_SUCCESS(status)){
		return status;
	}

	if (IoGetFileSystemVpbInfo(FileObject, &DeviceObject, &RealDevice) == FALSE){
		ObDereferenceObject(FileObject);
		return status;
	}

	status = IrpReadFile(FileObject, DeviceObject, 
		IoStatusBlock, FileBuffer, Length, ByteOffset);

	ObDereferenceObject(FileObject);

	return status;
}

BOOL
PeLoad_Repair(
	__in	PWCHAR	pwszFilePath,
	__out	PSIZE_T	NewBase,
	__in	SIZE_T	OriginBase
	)
{
	BOOL bRet = FALSE;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	HANDLE hFile;
	LARGE_INTEGER FileSize;
	ULONG Length = 0;
	PBYTE FileBuffer = NULL;
	PBYTE ImageBase = NULL;
	IO_STATUS_BLOCK IoStatus;
	//////////////////////////////////////////////////////////////////////////
// 	OBJECT_ATTRIBUTES oa;
// 	UNICODE_STRING uFile;

	ULONG i = 0;
	ULONG uLen = 0;

	if ((pwszFilePath == NULL)	||
		(NewBase == NULL)		||
		(OriginBase == 0)){
		return bRet;
	}

	*NewBase = 0;

	status = KernelOpenFile(pwszFilePath, 
							&hFile, 
							SYNCHRONIZE | FILE_EXECUTE,
							FILE_ATTRIBUTE_NORMAL,
							FILE_SHARE_READ,
							FILE_OPEN,
							FILE_SYNCHRONOUS_IO_NONALERT);
	if (!NT_SUCCESS(status)){
		return bRet;
	}
	
	status = KernelGetFileSize(hFile, &FileSize);
	if (!NT_SUCCESS(status)){
		ZwClose(hFile);
		return bRet;
	}

	Length = FileSize.LowPart;
	FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, Length, HG_MEM_TAG);
	if (FileBuffer == NULL){
		i = 1;
		do {
			uLen = Length + 2048 * i;
			FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, uLen, 0);
		} while (FileBuffer == NULL && ++i < 10);
	}

	if (FileBuffer == NULL){
		ZwClose(hFile);
		return bRet;
	}
	
	status = KernelReadFile(hFile, NULL, Length, FileBuffer, &IoStatus);
	if (!NT_SUCCESS(status)){
		ZwClose(hFile);
		ExFreePool(FileBuffer);
		return bRet;
	}

	ZwClose(hFile);
	//////////////////////////////////////////////////////////////////////////
// 	RtlInitUnicodeString(&uFile, pwszFilePath);
// 	InitializeObjectAttributes(&oa, &uFile, OBJ_CASE_INSENSITIVE, NULL, NULL);
// 	status = ZwOpenFile(&hFile,
// 						GENERIC_ALL,
// 						&oa,
// 						&IoStatus,
// 						FILE_SHARE_READ	| FILE_SHARE_WRITE,
// 						FILE_SYNCHRONOUS_IO_NONALERT);
// 
// 	if (NT_SUCCESS(status)){
// 		Reload32Log(("Open file successfully!\n"));
// 		status = ZwReadFile(	hFile, 
// 					NULL,
// 					NULL,
// 					NULL,
// 					&IoStatus,
// 					FileBuffer,
// 					Length,
// 					NULL,
// 					NULL);
// 		if (NT_SUCCESS(status)){
// 			Reload32Log(("Read file successfully!\n"));
// 		}
// 		ZwClose(hFile);
// 	}else{
// 		Reload32Log(("Open file unsuccessfully!\n"));
// 	}

	if (ImagePEFile(FileBuffer, (PVOID*)&ImageBase) == FALSE){
		Reload32Log(("Image file failed!\n"));
		ExFreePool(FileBuffer);
		return bRet;
	}

	Reload32Log(("Image base address: 0x%p\n", ImageBase));
	ExFreePool(FileBuffer);

	if (FixImportTable(ImageBase, OriginBase) == FALSE){
		Reload32Log(("Fix import table failed!\n"));
		ExFreePool(ImageBase);
		return bRet;
	}
	
	if (FixBaseRelocTable(ImageBase, OriginBase) == FALSE){
		Reload32Log(("Fix base relocation table failed!\n"));
		ExFreePool(ImageBase);
		return bRet;
	}
	*NewBase = (ULONG)ImageBase;
	bRet = TRUE;

	return bRet;
}

BOOL
GetOriginalKiServiceTable(
	__in	SIZE_T	NewImageBase,
	__in	SIZE_T	ExistImageBase,
	__out	PSIZE_T	NewKiServiceTable
	)
{
	BOOL bRet = FALSE;
	PIMAGE_DOS_HEADER ImageDosHeader = NULL;
	PIMAGE_NT_HEADERS ImageNtHeader = NULL;
	SIZE_T KeServiceDescriptorTableRva = 0;
	PIMAGE_BASE_RELOCATION ImageBaseReloc = NULL;
	ULONG RelocSize = 0;
	int ItemCount = 0;
	int Index = 0;
	int Type = 0;
	PSIZE_T ReloadAddress = NULL;
	SIZE_T RvaData = 0;
	DWORD count = 0;
	PWORD TypeOffset = NULL;

	if ((NewImageBase == 0)		||
		(ExistImageBase == 0)	||
		(NewKiServiceTable == NULL))
	{
		return bRet;
	}

	ImageDosHeader = (PIMAGE_DOS_HEADER)NewImageBase;
	if (ImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE){
		return bRet;
	}

	ImageNtHeader = (PIMAGE_NT_HEADERS)(NewImageBase + ImageDosHeader->e_lfanew);
	if (ImageNtHeader->Signature != IMAGE_NT_SIGNATURE){
		return bRet;
	}
	KeServiceDescriptorTableRva = (SIZE_T)MiFindExportedRoutine((PVOID)NewImageBase, TRUE, "KeServiceDescriptorTable", 0);
	if (KeServiceDescriptorTableRva == 0){
		return bRet;
	}

	KeServiceDescriptorTableRva = KeServiceDescriptorTableRva - (SIZE_T)NewImageBase;
	Reload32Log(("KeServiceDescriptorTableRva = 0x%p\n", KeServiceDescriptorTableRva));
	ImageBaseReloc = (PIMAGE_BASE_RELOCATION)HgRtlImageDirectoryEntryToData((PVOID)NewImageBase,
																			TRUE, 
																			IMAGE_DIRECTORY_ENTRY_BASERELOC,	
																			&RelocSize);
	if (ImageBaseReloc == NULL){
		return bRet;
	}
	Reload32Log(("ImageBaseReloc = 0x%p, SizeOfBlock = 0x%p\n", ImageBaseReloc, RelocSize));
	
	while (ImageBaseReloc->SizeOfBlock)
	{
		count++;
		ItemCount = (ImageBaseReloc->SizeOfBlock - 	sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		TypeOffset = (PWORD)((PCHAR)ImageBaseReloc + sizeof(IMAGE_BASE_RELOCATION));
		for (Index = 0; Index < ItemCount; Index++)
		{
			Type = TypeOffset[Index] >> 12;
			if(Type == IMAGE_REL_BASED_HIGHLOW)
			{
				ReloadAddress = (PSIZE_T)((SIZE_T)(TypeOffset[Index] & 0x0fff) + ImageBaseReloc->VirtualAddress + (SIZE_T)NewImageBase);
				
				RvaData = *ReloadAddress - ExistImageBase;
				if (RvaData == KeServiceDescriptorTableRva)
				{
					// find it from a special function, as follows:
					// c705a03f55808c2b5080 mov dword ptr [nt!KeServiceDescriptorTable (80553fa0)],offset nt!KiServiceTable (80502b8c)
					if(*(USHORT*)((SIZE_T)ReloadAddress - 2) == 0x05c7)
					{
						*NewKiServiceTable = *(SIZE_T*)((SIZE_T)ReloadAddress + 4) - ExistImageBase + (SIZE_T)NewImageBase;
						Reload32Log(("ReloadAddress = 0x%p\n", ReloadAddress));
						Reload32Log(("Find Original Image SSDT: 0x%p\n", *NewKiServiceTable));
						bRet = TRUE;
						return bRet;
					}
				}
			}
		}
		// Next
		ImageBaseReloc = (PIMAGE_BASE_RELOCATION)((SIZE_T)ImageBaseReloc + ImageBaseReloc->SizeOfBlock);
	}

	return bRet;
}

BOOL
FixOriginalKiServiceTable(
	__in	PSIZE_T	OriginKiServiceTable,
	__out	PSIZE_T	ReloadKiServiceTable,
	__in	SIZE_T	NewBase,
	__in	SIZE_T	OldBase
	)
{
	BOOL bRet = FALSE;
	SIZE_T i = 0;
	SIZE_T dwCount = KeServiceDescriptorTable.NumberOfServices;
	if ((OriginKiServiceTable == NULL)	||
		(ReloadKiServiceTable == NULL)	||
		(NewBase == 0)					||
		(OldBase == 0))
	{
		return bRet;
	}
	
	for (i = 0; i < dwCount; i++){
		ReloadKiServiceTable[i] = OriginKiServiceTable[i] - OldBase + NewBase;
	}

	bRet = TRUE;
	return bRet;
}

PVOID
LookupKernelModuleByNameW(
	__in	PWCHAR	pwszModuleName,
	__out	PDWORD	nModuleSize
	)
{
	PVOID pRet = NULL;
	PDRIVER_OBJECT pDrvObj = g_pHGDrvObj;

	if ((pwszModuleName == NULL)	&&
		(nModuleSize == NULL))
	{
		return pRet;
	}

	__try{
		PWCHAR Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, SZ_PATH_LEN_WCHAR, HG_MEM_TAG);
		PLDR_DATA_TABLE_ENTRY DriverSection = NULL;
		PLDR_DATA_TABLE_ENTRY LdrEntry = NULL;
		UNICODE_STRING uKernelModuleName;
		UNICODE_STRING uModuleName;
		int length = 0;
		int Index = 0;

		if (Buffer != NULL){
			RtlZeroMemory(Buffer, SZ_PATH_LEN_WCHAR);
			RtlInitUnicodeString(&uKernelModuleName, pwszModuleName);

			DriverSection = (PLDR_DATA_TABLE_ENTRY)pDrvObj->DriverSection;
			LdrEntry = (PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;

			while(LdrEntry && DriverSection != LdrEntry){
				if ((ULONG)LdrEntry->DllBase > (SIZE_T)MM_SYSTEM_RANGE_START &&
					LdrEntry->FullDllName.Length > 0				&&
					LdrEntry->FullDllName.Buffer != NULL			&&
					MmIsAddressValid(&LdrEntry->FullDllName.Buffer[LdrEntry->FullDllName.Length / sizeof(WCHAR) - 1]))
				{
					length = LdrEntry->FullDllName.Length / sizeof(WCHAR);

					for (Index = length - 1; Index > 0; Index--){
						if (LdrEntry->FullDllName.Buffer[Index] == L'\\'){
							break;
						}
					}
					if (LdrEntry->FullDllName.Buffer[Index] == L'\\'){
						RtlCopyMemory(Buffer, &(LdrEntry->FullDllName.Buffer[Index + 1]), (length - Index - 1) * sizeof(WCHAR));
						uModuleName.Buffer = Buffer;
						uModuleName.Length = (length - Index - 1) * sizeof(WCHAR);
						uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
					}else{
						RtlCopyMemory(Buffer, LdrEntry->FullDllName.Buffer, length * sizeof(WCHAR));
						uModuleName.Buffer = Buffer;
						uModuleName.Length = length * sizeof(WCHAR);
						uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
					}
					if (RtlEqualUnicodeString(&uModuleName, &uKernelModuleName, TRUE)){
						ExFreePool(Buffer);
						if (nModuleSize){
							*nModuleSize = LdrEntry->SizeOfImage;
						}
						return LdrEntry->DllBase;
					}
				}
				LdrEntry = (PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
			}
			ExFreePool(Buffer);
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
	}

	return pRet;
}

BOOL
ReloadHalDll(
	)
{
	BOOL bRet = FALSE;
	ULONG HalDllBase = 0;
	ULONG HalDllSize = 0;

	pfnMmIsAddressValid	pMmIsAddressValid = (pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];
	WCHAR pwszHalDll[] = L"Hal.dll";
	WCHAR pwszHalmacpiDll[] = L"halmacpi.dll";
	WCHAR* pwszModuleName = NULL;

	HalDllBase = (ULONG)LookupKernelModuleByNameW(pwszHalDll, & HalDllSize);

	if ((HalDllBase == 0)	||
		(HalDllSize == 0))
	{
		HalDllBase = (ULONG)LookupKernelModuleByNameW(pwszHalmacpiDll, &HalDllSize);
		if ((HalDllBase != 0)	&&
			(HalDllSize != 0))
		{
			pwszModuleName = pwszHalmacpiDll;
		}
	}else{
		pwszModuleName = pwszHalDll;
	}
	Reload32Log(("HalDllBase = 0x%p\n", HalDllBase));
	if (HalDllBase && HalDllSize && pwszModuleName){
		SIZE_T NewBase = 0;
		WCHAR pwszSystem32[] = L"\\SystemRoot\\System32\\";
		WCHAR pwszPath[SZ_PATH_LEN] = {0};

		g_hg_map[eHG_OriginHalDllBase] = HalDllBase;
		g_hg_map[eHG_HalDllSize] = HalDllSize;

		wcscpy(pwszPath, pwszSystem32);
		wcscat(pwszPath, pwszModuleName);

		if (PeLoad_Repair(pwszPath, &NewBase, HalDllBase)	&&
			(NewBase > (SIZE_T)MM_SYSTEM_RANGE_START))
		{
			SIZE_T NewBase_Fixed = 0;
			g_hg_map[eHG_NewHalDllBase] = NewBase;
			if (PeLoad_Unrepair(pwszPath, &NewBase_Fixed)){
				g_hg_map[eHG_NewHalDllBase_Unfix] = NewBase;
			}
		}
	}

	bRet = TRUE;
	return bRet;
}

PEPROCESS
LookupProcessByName(
	__in	PWCHAR	ProcessName
	)
{
	UNICODE_STRING uProcessName;
	PEPROCESS pProcess = NULL;
	SIZE_T uSize = 0x10000;
	SIZE_T uRet = 0;
	ULONG i = 0;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PVOID pBuffer = NULL;
	PSYSTEM_PROCESS_INFORMATION  pSpiNext = NULL;
	PETHREAD pThread = NULL;
	//CHAR PreMode = 0;

	pfnExAllocatePoolWithTag pExAllocatePoolWithTag = (pfnExAllocatePoolWithTag)g_hg_map[eHG_ExAllocatePoolWithTag];
	pfnNtQuerySystemInformation pNtQuerySystemInformation = (pfnNtQuerySystemInformation)g_hg_map[eHG_NtQuerySystemInformation];
	pfnExFreePoolWithTag pExFreePoolWithTag = (pfnExFreePoolWithTag)g_hg_map[eHG_ExFreePoolWithTag];
	pfnPsLookupProcessByProcessId pPsLookupProcessByProcessId = (pfnPsLookupProcessByProcessId)g_hg_map[eHG_PsLookupProcessByProcessId];
	pfnObfDereferenceObject pObfDereferenceObjcet = (pfnObfDereferenceObject)g_hg_map[eHG_ObfDereferenceObject];
	pfnPsGetCurrentThread pPsGetCurrentThread = (pfnPsGetCurrentThread)g_hg_map[eHG_PsGetCurrentProcess];

	if (pExAllocatePoolWithTag == NULL){
		Reload32Log(("%s(%d): pExAllocatePoolWithTag = 0x%p\n", __FILE__, __LINE__, pExAllocatePoolWithTag));
		return NULL;
	}
	if (pNtQuerySystemInformation == NULL){
		Reload32Log(("{init.c}{pNtQuerySystemInformation} Error!\n"));
		return NULL;
	}
	if (pExFreePoolWithTag == NULL){
		Reload32Log(("{init.c}{pExFreePoolWithTag} Error!\n"));
		return NULL;
	}
	if (pPsLookupProcessByProcessId == NULL){
		Reload32Log(("{init.c}{pPsLookupProcessByProcessId} Error!\n"));
		return NULL;
	}
	if (pObfDereferenceObjcet == NULL){
		Reload32Log(("{init.c}{pObfDereferenceObjcet} Error!\n"));
		return NULL;
	}
	if (pPsGetCurrentThread == NULL){
		Reload32Log(("{init.c}{pPsGetCurrentThread} Error!\n"));
		return NULL;
	}
	RtlInitUnicodeString(&uProcessName, ProcessName);
	pThread = pPsGetCurrentThread();
	//PreMode = ChangePreMode(pThread);

	do {
		pBuffer = pExAllocatePoolWithTag(NonPagedPool, uSize, HG_MEM_TAG);
		if (pBuffer)
		{
			status = HgNtQuerySystemInformation(SystemProcessInformation, pBuffer, uSize, &uRet);
			if (NT_SUCCESS(status)){
				pSpiNext = (PSYSTEM_PROCESS_INFORMATION)pBuffer;
				do {
					if (RtlCompareUnicodeString(&pSpiNext->ImageName, &uProcessName, TRUE) == 0)
					{
						PEPROCESS pCurrentProcess;
						status = pPsLookupProcessByProcessId((HANDLE)pSpiNext->UniqueProcessId, &pCurrentProcess);
						if (NT_SUCCESS(status)){
							pProcess = pCurrentProcess;
						}
					}
					pSpiNext = (PSYSTEM_PROCESS_INFORMATION)((ULONG)pSpiNext + pSpiNext->NextEntryOffset);
				} while (pSpiNext->NextEntryOffset && !pProcess);
			}else{
				uSize *= 2;
			}
			pExFreePoolWithTag(pBuffer, 0);
			pBuffer = NULL;
		}else{
			break;
		}
	} while (status = STATUS_INFO_LENGTH_MISMATCH && ++i < 10);

	//RecoverPreMode(pThread, PreMode);
	return pProcess;
}

PKSERVICE_TABLE_DESCRIPTOR
GetAddrOfShadowTable(
	)
{
	PKSERVICE_TABLE_DESCRIPTOR pShadowTableRet = NULL;
	PCHAR ptr = NULL;
	size_t i = 0;
	ULONG_PTR bReloadNtosSuccess = g_hg_map[eHG_ReloadNtosKrnlSuccess];
	ULONG_PTR Base = bReloadNtosSuccess ? g_hg_map[eHG_NewKernelBase] : g_hg_map[eHG_CurrentKernelBase];
	pfnMmIsAddressValid pMmIsAddressValid = (pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if (pMmIsAddressValid == NULL){
		return NULL;
	}

	ptr = (PCHAR)HgMiFindExportedRoutineByName((PVOID)Base, "KeAddSystemServiceTable");
	if (ptr	== NULL){
		return NULL;
	}

	// scan KeAddSystemServiceTable function body for pointers to shadow table
	for (i = 0; i < 0x1000; i++, ptr++){
		if (pMmIsAddressValid(ptr)){
			PVOID ProbeShadowTablePtr = NULL;
			ProbeShadowTablePtr = (PVOID)(*(PULONG)ptr);
			// preliminary address check /bypass main SDT ptr
			if (IsSafeAddress(ProbeShadowTablePtr, 2 * sizeof(KSERVICE_TABLE_DESCRIPTOR))	&&
				(&KeServiceDescriptorTable) != ProbeShadowTablePtr)
			{
				// can't do MmGetSystemAddressForMdl safe here, it returns invalid(unmapped) VA
				// shadow table first item equals to SDT
				if (0 == memcmp(ProbeShadowTablePtr, &KeServiceDescriptorTable, sizeof(KSERVICE_TABLE_DESCRIPTOR)))
				{
					// address found
					pShadowTableRet = (PKSERVICE_TABLE_DESCRIPTOR)ProbeShadowTablePtr;
				}
			}
		}
		if (pShadowTableRet != NULL){
			break;
		}
	}

	return pShadowTableRet;
}

BOOL
InitReloadWin32K(
	__in	PKSERVICE_TABLE_DESCRIPTOR	ShadowSSDT,
	__in	SIZE_T						OldWin32kBase
	)
{
	BOOL bRet = FALSE;
	SIZE_T NewWin32kBase = 0;
	PSIZE_T OriginShadowServiceTable = NULL;
	PSIZE_T ReloadShadowServiceTable = NULL;
	PSIZE_T* pServiceTable = NULL;
	PKSERVICE_TABLE_DESCRIPTOR OriginShadowSSDT = NULL;
	PKSERVICE_TABLE_DESCRIPTOR ReloadShadowSSDT = NULL;

	OriginShadowServiceTable = (PSIZE_T)ExAllocatePoolWithTag(NonPagedPool, ShadowSSDT->NumberOfServices * sizeof(DWORD), HG_MEM_TAG);
	ReloadShadowServiceTable = (PSIZE_T)ExAllocatePoolWithTag(NonPagedPool, ShadowSSDT->NumberOfServices * sizeof(DWORD), HG_MEM_TAG); 
	OriginShadowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool, sizeof(KSERVICE_TABLE_DESCRIPTOR), HG_MEM_TAG);
	ReloadShadowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)ExAllocatePoolWithTag(NonPagedPool, sizeof(KSERVICE_TABLE_DESCRIPTOR), HG_MEM_TAG);

	if (ShadowSSDT->NumberOfServices > 0		&&
		OriginShadowServiceTable			&&
		ReloadShadowServiceTable			&&
		OriginShadowSSDT					&&
		ReloadShadowSSDT)
	{
		memset(OriginShadowServiceTable, 0, ShadowSSDT->NumberOfServices * sizeof(DWORD));
		memset(ReloadShadowServiceTable, 0, ShadowSSDT->NumberOfServices * sizeof(DWORD));
		memset(OriginShadowSSDT, 0, sizeof(KSERVICE_TABLE_DESCRIPTOR));
		memset(ReloadShadowSSDT, 0, sizeof(KSERVICE_TABLE_DESCRIPTOR));

		if (PeLoad_Repair(L"\\SystemRoot\\System32\\win32k.sys", &NewWin32kBase, OldWin32kBase))
		{
			PDWORD pOriginKiServiceTable = NULL;
			g_hg_map[eHG_ReloadWin32kSuccess] = 1;
			g_hg_map[eHG_NewWin32kBase] = NewWin32kBase;

			//Reload32Log(("NewWin32kBase = 0x%p, OldWin32kBase = 0x%p\n", NewWin32kBase, OldWin32kBase));

			if (GetOriginalW32pTable((PVOID)NewWin32kBase, (DWORD)OldWin32kBase, &pOriginKiServiceTable) && pOriginKiServiceTable)
			{
				ULONG i = 0;
				ULONG nCnt = 0;
				nCnt = ShadowSSDT->NumberOfServices;

				for (i = 0; i < nCnt; i++){
					OriginShadowServiceTable[i] = (ULONG)pOriginKiServiceTable[i];
					ReloadShadowServiceTable[i] = (ULONG)pOriginKiServiceTable[i] - 	(ULONG)OldWin32kBase + (ULONG)NewWin32kBase;
				}
				OriginShadowSSDT->NumberOfServices = ShadowSSDT->NumberOfServices;
				OriginShadowSSDT->ParamTableBase = ShadowSSDT->ParamTableBase;
				OriginShadowSSDT->ServiceCounterTableBase = ShadowSSDT->ServiceCounterTableBase;
				OriginShadowSSDT->ServiceTableBase = OriginShadowServiceTable;
				g_hg_map[eHG_OriginShadowSSDT] = (ULONG)OriginShadowSSDT;
				g_hg_map[eHG_OriginShadowKiServiceTable] = (ULONG)OriginShadowServiceTable;

				ReloadShadowSSDT->NumberOfServices = ShadowSSDT->NumberOfServices;
				ReloadShadowSSDT->ParamTableBase = ShadowSSDT->ParamTableBase;
				ReloadShadowSSDT->ServiceCounterTableBase = ShadowSSDT->ServiceCounterTableBase;
				ReloadShadowSSDT->ServiceTableBase = ReloadShadowServiceTable;
				g_hg_map[eHG_ReloadShadowSSDT] = (ULONG)ReloadShadowSSDT;
				g_hg_map[eHG_ReloadShadowKiServiceTable] = (ULONG)ReloadShadowServiceTable;

				return TRUE;
			}
		}

		if (NT_SUCCESS(GetRawShadowAddr(ShadowSSDT, (PVOID)OldWin32kBase, (PVOID*)&pServiceTable)) && pServiceTable)
		{
			ULONG i = 0;
			ULONG nCnt = ShadowSSDT->NumberOfServices;

			memset(OriginShadowServiceTable, 0,	ShadowSSDT->NumberOfServices * sizeof(DWORD));
			memset(ReloadShadowServiceTable, 0,	ShadowSSDT->NumberOfServices * sizeof(DWORD));
			memset(OriginShadowSSDT, 0, sizeof(KSERVICE_TABLE_DESCRIPTOR));
			memset(ReloadShadowSSDT, 0, sizeof(KSERVICE_TABLE_DESCRIPTOR));

			if (NewWin32kBase){
				for (i = 0; i < nCnt; i++){
					OriginShadowServiceTable[i] = (ULONG)pServiceTable[i];
					ReloadShadowServiceTable[i] = (ULONG)OriginShadowServiceTable[i] - (ULONG)OldWin32kBase + (ULONG)NewWin32kBase;
				}

				OriginShadowSSDT->NumberOfServices = ShadowSSDT->NumberOfServices;
				OriginShadowSSDT->ParamTableBase = ShadowSSDT->ParamTableBase;
				OriginShadowSSDT->ServiceCounterTableBase = ShadowSSDT->ServiceCounterTableBase;
				OriginShadowSSDT->ServiceTableBase = ShadowSSDT->ServiceTableBase;
				g_hg_map[eHG_OriginShadowSSDT] = (ULONG)OriginShadowSSDT;
				g_hg_map[eHG_OriginShadowKiServiceTable] = (ULONG)OriginShadowServiceTable;

				ReloadShadowSSDT->NumberOfServices = ShadowSSDT->NumberOfServices;
				ReloadShadowSSDT->ParamTableBase = ShadowSSDT->ParamTableBase;
				ReloadShadowSSDT->ServiceCounterTableBase = ShadowSSDT->ServiceCounterTableBase;
				ReloadShadowSSDT->ServiceTableBase = ShadowSSDT->ServiceTableBase;
				g_hg_map[eHG_ReloadShadowSSDT] = (ULONG)ReloadShadowSSDT;
				KdPrint(("**********************************2\n"));
				g_hg_map[eHG_ReloadShadowKiServiceTable] = (ULONG)ReloadShadowServiceTable;
			}else{
				for (i = 0; i < nCnt; i++){
					OriginShadowServiceTable[i] = (ULONG)pServiceTable[i];
				}

				OriginShadowSSDT->NumberOfServices = ShadowSSDT->NumberOfServices;
				OriginShadowSSDT->ParamTableBase = ShadowSSDT->ParamTableBase;
				OriginShadowSSDT->ServiceCounterTableBase = ShadowSSDT->ServiceCounterTableBase;
				OriginShadowSSDT->ServiceTableBase = ShadowSSDT->ServiceTableBase;
				g_hg_map[eHG_OriginShadowSSDT] = (ULONG)OriginShadowSSDT;
				g_hg_map[eHG_OriginShadowKiServiceTable] = (ULONG)OriginShadowServiceTable;
			}
			ExFreePool(pServiceTable);
			return TRUE;
		}
	}

	if (OriginShadowServiceTable){
		ExFreePool(OriginShadowServiceTable);
	}

	if (ReloadShadowServiceTable){
		ExFreePool(ReloadShadowServiceTable);
	}

	if (OriginShadowSSDT){
		ExFreePool(OriginShadowSSDT);
	}

	if (ReloadShadowSSDT){
		ExFreePool(ReloadShadowSSDT);
	}

	if (NewWin32kBase){
		ExFreePool((PVOID)NewWin32kBase);
	}

	return bRet;
}

BOOL
GetOriginalW32pTable(
	__in	PVOID	NewWin32kBase,
	__in	DWORD	OldWin32kBase,
	__out	PDWORD*	W32pTable
	)
{
	BOOL bRet = FALSE;
	PIMAGE_NT_HEADERS NtHeader = NULL;
	ULONG_PTR EntryPoint;
	DWORD dwEntryPoint;
	DWORD dwCurAddress;
	DWORD Length = 0;
	//PUCHAR pOpcode = 0;
	ULONG ulCount = 0;
	PVOID pKeAddSystemServiceTable = NULL;

	if ((NewWin32kBase == 0)	||
		(W32pTable == NULL)		||
		(OldWin32kBase == 0))
	{
		return FALSE;
	}

	pKeAddSystemServiceTable = (PVOID)GetProcFromIAT(NewWin32kBase, "KeAddSystemServiceTable");

	if (pKeAddSystemServiceTable == NULL){
		Reload32Log(("Get KeAddSystemServiceTable failed!\n"));
	}

	NtHeader = HgRtlImageNtHeader(NewWin32kBase);
	if (NtHeader != NULL){
		EntryPoint = NtHeader->OptionalHeader.AddressOfEntryPoint;
		EntryPoint += (ULONG_PTR)NewWin32kBase;
		dwEntryPoint = (DWORD)EntryPoint;

		//
		// IDA
		// 68 80 A2 99 BF		push	offset off_BF99A280
		// FF 15 58 D4 98 BF	call	ds:KeAddSystemServiceTable
		// 
		// use call instruction to locate ds:KeAddSystemServiceTable
		// 
		__try{
			for (dwCurAddress = dwEntryPoint; 
				dwCurAddress < dwEntryPoint + 0x1000; 
				dwCurAddress += Length)
			{
				//Length = SizeOfCode((PUCHAR)dwCurAddress, &pOpcode);
				//////////////////////////////////////////////////////////////////////////
				DISASM MyDisasm;
				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)dwCurAddress;
				Length = Disasm(&MyDisasm);
				//////////////////////////////////////////////////////////////////////////
				if (*(WORD*)dwCurAddress == 0x15FF){
					if (ulCount++ >= 5){
						break;
					}

					if (MmIsAddressValid((PVOID)(dwCurAddress + 2))	&&
						*(PULONG)(dwCurAddress + 2) > 0x80000000	&&
						MmIsAddressValid((PVOID)(dwCurAddress - 4))	&&
						*(PULONG)(dwCurAddress - 4) > 0x80000000		&&
						MmIsAddressValid((PVOID)(*(PDWORD)(dwCurAddress + 2) - (DWORD)OldWin32kBase + (DWORD)NewWin32kBase)) &&
						*(PDWORD)(*(PDWORD)(dwCurAddress + 2) - (DWORD)OldWin32kBase + (DWORD)NewWin32kBase) == (DWORD)pKeAddSystemServiceTable)
					{
						*W32pTable = (PDWORD)(*(PDWORD)(dwCurAddress - 4) - (DWORD)OldWin32kBase + (DWORD)NewWin32kBase);
						bRet = TRUE;
						break;
					}
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){

		}
	}

	return bRet;
}

size_t
	GetProcFromIAT(
	__in	PVOID	ImageBase,
	__in	PCHAR	pszProcName
	)
{
	PIMAGE_IMPORT_DESCRIPTOR ImageImportDescriptor = NULL;
	PIMAGE_THUNK_DATA ImageThunkData = NULL;
	PIMAGE_THUNK_DATA FirstThunk = NULL;
	PIMAGE_IMPORT_BY_NAME ImportByName = NULL;
	DWORD ImportSize = 0;
	PVOID ModuleBase = NULL;
	DWORD FunctionAddress = 0;
	TCHAR pszDllName[20];
	PIMAGE_THUNK_DATA pOrgFirstThunk;
	PIMAGE_THUNK_DATA pFirstThunk;
	DWORD index = 0;

	ImageImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)HgRtlImageDirectoryEntryToData(	ImageBase, 
		TRUE, 
		IMAGE_DIRECTORY_ENTRY_IMPORT, 
		&ImportSize);

	if (ImageImportDescriptor == NULL){
		return 0;
	}
	//Reload32Log(("ImageImportDescriptor = 0x%p, ImportSize = 0x%p\n", ImageImportDescriptor, ImportSize));


	while (ImageImportDescriptor->Name			||
		ImageImportDescriptor->FirstThunk		||
		ImageImportDescriptor->OriginalFirstThunk)
	{
		CHAR ModuleName[SZ_PATH_LEN] = {0};
		strcpy(ModuleName, (PCHAR)((ULONG)ImageBase + ImageImportDescriptor->Name));
		//Reload32Log(("Module Name = %s\n", ModuleName));
		if (_stricmp(ModuleName, "ntkrnlpa.exe") == 0	||
			_stricmp(ModuleName, "ntoskrnl.exe") == 0	||
			_stricmp(ModuleName, "ntkrnlmp.exe") == 0	||
			_stricmp(ModuleName, "ntkrpamp.exe") == 0)
		{
			break;
		}
		ImageImportDescriptor++;
	}

	pOrgFirstThunk = (PIMAGE_THUNK_DATA)(ImageImportDescriptor->OriginalFirstThunk + (DWORD)ImageBase);
	pFirstThunk = (PIMAGE_THUNK_DATA)(ImageImportDescriptor->FirstThunk + (DWORD)ImageBase);
	//Reload32Log(("ImageBase = 0x%p, pOrgFirstThunk = 0x%p, pFirstThunk = 0x%p\n", ImageBase, pOrgFirstThunk, pFirstThunk));

	for (index = 0; &(pOrgFirstThunk[index]); index++)
	{
		if (!(pOrgFirstThunk[index].u1.Ordinal & IMAGE_ORDINAL_FLAG32))
		{
			CHAR szFunName[100] = {0};
			PIMAGE_IMPORT_BY_NAME pFunName = (PIMAGE_IMPORT_BY_NAME)(pOrgFirstThunk[index].u1.ForwarderString + (DWORD)ImageBase);
			strcpy(szFunName, (CHAR*)(pFunName->Name));
			if (_stricmp(szFunName, pszProcName) == 0){
				return pFirstThunk[index].u1.Function;
			}
		}
	}
	return 0;
}

NTSTATUS
	GetRawShadowAddr(
	__in	PKSERVICE_TABLE_DESCRIPTOR	ShadowSSDTAddr,
	__in	PVOID						BaseOfwin32k_sys,
	__out	PVOID*						pOutShadowServiceTable
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	HANDLE hFile = NULL;
	HANDLE hSection = NULL;
	IO_STATUS_BLOCK IoStatusBlock;
	OBJECT_ATTRIBUTES ObjectAttribute;
	PVOID pSectionObject;
	PVOID BaseAddress = NULL;
	SIZE_T nLen = 0;
	ULONG ulSizeShadowTable;
	PVOID pRawShadowServiceTable;
	UNICODE_STRING usWin32K;

	RtlInitUnicodeString(&usWin32K, L"\\SystemRoot\\System32\\win32k.sys");
	InitializeObjectAttributes(	&ObjectAttribute,
		&usWin32K,
		OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
		NULL,
		NULL);
	status = ZwOpenFile(&hFile, 
		SYNCHRONIZE | FILE_EXECUTE, 
		&ObjectAttribute,
		&IoStatusBlock,
		FILE_SHARE_READ,
		FILE_SYNCHRONOUS_IO_NONALERT);

	if(NT_SUCCESS(status)){
		ObjectAttribute.ObjectName = 0;
		status = ZwCreateSection(	&hSection,
			SECTION_MAP_EXECUTE | SECTION_MAP_READ | SECTION_QUERY,
			&ObjectAttribute,
			0,
			PAGE_EXECUTE,
			SEC_IMAGE,
			hFile);
		if (NT_SUCCESS(status)){
			status = ObReferenceObjectByHandle(	hSection,
				SECTION_MAP_EXECUTE | SECTION_MAP_READ | SECTION_QUERY,
				*MmSectionObjectType,
				KernelMode,
				(PVOID*)&pSectionObject,
				NULL);
			if(NT_SUCCESS(status)){
				status = MmMapViewInSystemSpace(pSectionObject, &BaseAddress, &nLen);
				if (NT_SUCCESS(status)){
					PIMAGE_NT_HEADERS32 pImageNtHeader;
					DWORD ulRVA;
					PIMAGE_SECTION_HEADER pImageSectionHeader;
					LARGE_INTEGER ByteOffset;
					int nSections = 0;

					status = STATUS_UNSUCCESSFUL;

					pImageNtHeader = (PIMAGE_NT_HEADERS32)((PBYTE)BaseAddress + ((PIMAGE_OPTIONAL_HEADER32)BaseAddress)->SizeOfHeaders);
					ulRVA = (PBYTE)(ShadowSSDTAddr->ServiceTableBase) - (PBYTE)BaseOfwin32k_sys;
					pImageSectionHeader = (PIMAGE_SECTION_HEADER)((PBYTE)pImageNtHeader + sizeof(IMAGE_NT_HEADERS32));
					if (pImageNtHeader->FileHeader.NumberOfSections > 0){
						for (nSections = 0; nSections < pImageNtHeader->FileHeader.NumberOfSections; nSections++){
							if (ulRVA >= pImageSectionHeader->VirtualAddress && ulRVA < pImageSectionHeader->VirtualAddress + pImageSectionHeader->Misc.VirtualSize){
								break;
							}
							pImageSectionHeader++;
						}
						ByteOffset.LowPart = pImageSectionHeader->PointerToRawData + ulRVA - pImageSectionHeader->VirtualAddress;

						ByteOffset.HighPart = 0;
						ulSizeShadowTable = ShadowSSDTAddr->NumberOfServices;
						if (ulSizeShadowTable){
							pRawShadowServiceTable = ExAllocatePoolWithTag(NonPagedPool, ulSizeShadowTable * 4, HG_MEM_TAG);
							if (pRawShadowServiceTable){
								RtlZeroMemory(pRawShadowServiceTable, ulSizeShadowTable * 4);
								status = ZwReadFile(hFile,
									NULL,
									NULL,
									NULL,
									&IoStatusBlock,
									pRawShadowServiceTable, 
									ulSizeShadowTable * 4,
									&ByteOffset,
									NULL);
								if (NT_SUCCESS(status)){
									SIZE_T dwAlignOffset = (PBYTE)BaseOfwin32k_sys - (PBYTE)pImageNtHeader->OptionalHeader.ImageBase;
									ULONG ulNum = 0;
									for (ulNum = 0; ulNum < ulSizeShadowTable; ulNum++){
										((SIZE_T*)pRawShadowServiceTable)[ulNum] += dwAlignOffset;
										// encrypt
									}
									status = STATUS_SUCCESS;
								}
								*pOutShadowServiceTable = pRawShadowServiceTable;
							}else{
								status = STATUS_INSUFFICIENT_RESOURCES;
							}
						}
					}
					MmUnmapViewInSystemSpace(BaseAddress);
				}
				ObDereferenceObject(pSectionObject);
			}
			ZwClose(hSection);
		}
		ZwClose(hFile);
	}

	return status;
}

BOOL 
ReloadWin32k(
	)
{
	BOOL bRet = FALSE;
	PEPROCESS pCsrss = LookupProcessByName(L"csrss.exe");
	PKSERVICE_TABLE_DESCRIPTOR pShadowSSDT = NULL;
	KAPC_STATE as;
	ULONG uWin32kBase = 0;
	ULONG uWin32kSize = 0;
	pfnKeStackAttachProcess pKeStackAttachProcess =	(pfnKeStackAttachProcess)g_hg_map[eHG_KeStackAttachProcess];
	pfnKeUnstackDetachProcess pKeUnstackDetachProcess = (pfnKeUnstackDetachProcess)g_hg_map[eHG_KeUnstackDetachProcess];
	pfnMmIsAddressValid pMmIsAddressValid = (pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if (pCsrss == NULL){
		Reload32Log(("pCsrss == NULL\n"));
		return bRet;
	}

	g_hg_map[eHG_CsrssEprocess] = (ULONG)pCsrss;
	pKeStackAttachProcess(pCsrss, &as);

	pShadowSSDT = GetAddrOfShadowTable();
	Reload32Log(("Shadow SSDT Address: 0x%p\n", pShadowSSDT));

	if (pShadowSSDT						&&
		pMmIsAddressValid(pShadowSSDT)	&&
		&pShadowSSDT[1]					&&
		pMmIsAddressValid(&pShadowSSDT[1]))
	{
		uWin32kBase = (ULONG)LookupKernelModuleByNameA("win32k.sys", &uWin32kSize);

		if (uWin32kBase && uWin32kSize){
			g_hg_map[eHG_OriginWin32kBase] = uWin32kBase;
			g_hg_map[eHG_Win32kSize] = uWin32kSize;
			g_hg_map[eHG_NowShadowSSDT] = (ULONG)&pShadowSSDT[1];
			g_hg_map[eHG_ShadowSSDTFunctionsCount] = pShadowSSDT[1].NumberOfServices;
			if (InitReloadWin32K(&pShadowSSDT[1], uWin32kBase)){
				Reload32Log(("Reload Win32k sucessully!\n"));
			}
		}
	}
	InitShadowSSDT();
	pKeUnstackDetachProcess(&as);

	return TRUE;
}

VOID
InitShadowSSDT(
	)
{
	PKSERVICE_TABLE_DESCRIPTOR pOriginShadowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginShadowSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pReloadShadowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_ReloadShadowSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pNowShadowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_NowShadowSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pTable = NULL;
	Reload32Log(("pOriginShadowSSDT = 0x%p, pReloadShadowSSDT = 0x%p, pNowShadowSSDT = 0x%p\n", pOriginShadowSSDT, pReloadShadowSSDT, pNowShadowSSDT));
	if (pReloadShadowSSDT && pReloadShadowSSDT->ServiceTableBase){
		pTable = pReloadShadowSSDT;
	}else if(pOriginShadowSSDT && pOriginShadowSSDT->ServiceTableBase){
		pTable = pOriginShadowSSDT;
	}else if (pNowShadowSSDT && pNowShadowSSDT->ServiceTableBase){
		pTable = pNowShadowSSDT;
	}

	if (pTable && HgMmIsAddressValid((PVOID)pTable->ServiceTableBase)){
		g_hg_map[eHG_NtUserBuildHwndList] = pTable->ServiceTableBase[g_hg_map[eHG_NtUserBuildHwndListIndex]];
		g_hg_map[eHG_NtUserQueryWindow] = pTable->ServiceTableBase[g_hg_map[eHG_NtUserQueryWindowIndex]];
		g_hg_map[eHG_NtUserKillTimer] =	pTable->ServiceTableBase[g_hg_map[eHG_NtUserKillTimerIndex]];
		g_hg_map[eHG_NtUserValidateTimerCallback] =	pTable->ServiceTableBase[g_hg_map[eHG_NtUserValidateTimerCallbackIndex]];
		g_hg_map[eHG_NtUserRegisterHotkey] = pTable->ServiceTableBase[g_hg_map[eHG_NtUserRegisterHotkeyIndex]];
	}else{
		Reload32Log(("Initialize SSDT Shadow unsuccessfully!\n"));
	}
}

BOOLEAN
IsSafeAddress(
	__in	PVOID	Address,
	__in	ULONG	Size
	)
{
	BOOLEAN bResult = FALSE;
	ULONG_PTR ulpAddress;
	pfnMmIsAddressValid pMmisAddressValid = (pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];
	pMmisAddressValid = HgMmIsAddressValid;
	if (pMmisAddressValid == NULL){
		Reload32Log(("----------------------pMmisAddressValid == NULL\n"));
		return FALSE;
	}
	ulpAddress = (ULONG_PTR)Address;
	// non-NULL ptr
	if(ulpAddress){
		// no wrapping
		if(ulpAddress + Size >= ulpAddress){
			bResult = TRUE;
		}
	}

	// valid ptr
	bResult &= pMmisAddressValid(Address) && pMmisAddressValid((PCHAR)Address + Size - 1);

	return bResult;
}

VOID
InitNtfs(
	)
{
	CHAR pszModuleName[] = "ntoskrnl.exe";
	CHAR pszFunctionName[] = "MmFlushImageSection";
	WCHAR pwszFileSystem[] = L"\\FileSystem\\";
	WCHAR pwszDriverDir[] = L"\\SystemRoot\\System32\\drivers\\";
	SIZE_T NtfsBase = g_hg_map[eHG_NtfsBase];
	SIZE_T NtfsSize = g_hg_map[eHG_NtfsSize];
	pfnExAllocatePoolWithTag pExAllocatePooWithTag =
		(pfnExAllocatePoolWithTag)g_hg_map[eHG_ExAllocatePoolWithTag];
	pfnExFreePoolWithTag pExFreePoolWithTag =
		(pfnExFreePoolWithTag)g_hg_map[eHG_ExFreePoolWithTag];
	pfnMmIsAddressValid pMmIsAddressValid = 
		(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];
	WCHAR pwszNtfsPath[SZ_PATH_LEN] = {0};
	WCHAR pwszNtfsSys[] = L"ntfs.sys";
	WCHAR pwszNtfs[] = L"ntfs";
	SIZE_T NewBase = 0;
	SIZE_T NtfsImpMmFlushImageSection = 0;
	WCHAR pwszFileSystemNtfs[SZ_PATH_LEN] = {0};
	PDRIVER_OBJECT pDriverObject = NULL;

	if ((NtfsBase == 0)				||
		(NtfsSize == 0)				||
		(pExAllocatePooWithTag == NULL) || 
		(pExFreePoolWithTag == NULL)	||
		(pMmIsAddressValid == NULL))
	{
		return;
	}

	wcscpy(pwszFileSystemNtfs, pwszFileSystem);
	wcscat(pwszFileSystemNtfs, pwszNtfs);

	if (NT_SUCCESS(GetDriverObject(pwszFileSystemNtfs, &pDriverObject))){
		g_hg_map[eHG_NtfsDriverObject] = (ULONG)pDriverObject;
	}

	wcscpy(pwszNtfsPath, pwszDriverDir);
	wcscat(pwszNtfsPath, pwszNtfsSys);

	if (PeLoad_Repair(pwszNtfsPath, &NewBase, NtfsBase) == TRUE){
		ULONG_PTR Offset = 0;
		PULONG_PTR pOriginDispatchs = NULL;
		PULONG_PTR pReloadDispatchs = NULL;

		if (FindIatFunction((PVOID)NewBase, NtfsSize, 
			pszModuleName, pszFunctionName, &Offset) && Offset)
		{
			NtfsImpMmFlushImageSection = NtfsBase + Offset;
			g_hg_map[eHG_NtfsImpMmFlushImageSection] = NtfsImpMmFlushImageSection;
		}

		pOriginDispatchs = (PSIZE_T)pExAllocatePooWithTag(NonPagedPool, sizeof(ULONG) * 28, HG_MEM_TAG);
		pReloadDispatchs = (PSIZE_T)pExAllocatePooWithTag(NonPagedPool, sizeof(ULONG) * 28, HG_MEM_TAG);

		if (pOriginDispatchs && pReloadDispatchs){
			SIZE_T pDriverEntry = 0;
			ULONG nCodeLen = 0;
			SIZE_T Address = 0;
			ULONG MovTemp = 0;
			ENTRY_POINT_TYPE EntryType = EPT_EntryNone;
			WIN_VERSION Version = (WIN_VERSION)g_WindowsVersion;

			memset(pOriginDispatchs, 0, sizeof(ULONG) * 28);
			memset(pReloadDispatchs, 0, sizeof(ULONG) * 28);

			if (Version == eWin_2003	||
				Version == eWin_8)
			{
				EntryType = EPT_EntryPointJmp;
			}else{
				EntryType = EPT_EntryPointHead;
			}

			if(GetDriverEntryPoint((PVOID)NewBase, &pDriverEntry, EntryType) == FALSE){
				pExFreePoolWithTag(pOriginDispatchs, 0);
				pExFreePoolWithTag(pReloadDispatchs, 0);
				pExFreePoolWithTag((PVOID)NewBase, 0);
				return ;
			}

			for (Address = pDriverEntry;
				Address < pDriverEntry + 1000;
				Address += nCodeLen)
			{
				INSTRUCTION Inst;

				if (pMmIsAddressValid((PVOID)Address) == FALSE){
					break;
				}
				memset(&Inst, 0, sizeof(INSTRUCTION));
				get_instruction(&Inst, (PBYTE)Address, MODE_32);

				if (Inst.type = INSTRUCTION_TYPE_MOV)
				{
					if (Inst.op1.type == OPERAND_TYPE_MEMORY	&&
						Inst.op2.type == OPERAND_TYPE_IMMEDIATE	&&
						Inst.op1.displacement >= 0x38			&&
						Inst.op1.displacement <= 0xA4)
					{
						ULONG nIndex = (Inst.op1.displacement - 0x38) / sizeof(ULONG);
						pOriginDispatchs[nIndex] = Inst.op2.immediate;
						pReloadDispatchs[nIndex] = Inst.op2.immediate - NtfsBase + NewBase;
					}else if (Inst.op1.type == OPERAND_TYPE_REGISTER	&&
						Inst.op2.type == OPERAND_TYPE_IMMEDIATE			&&
						pMmIsAddressValid((PVOID)Inst.op2.immediate)	&&
						Inst.opcode < 0xBF)
					{
						MovTemp = Inst.op2.immediate;
					}else if (Inst.op1.type == OPERAND_TYPE_MEMORY	&&
						Inst.op2.type == OPERAND_TYPE_REGISTER		&&
						Inst.op1.displacement >= 0x38				&&
						Inst.op1.displacement <= 0xA4				&&
						MovTemp										&&
						pMmIsAddressValid((PVOID)MovTemp))
					{
						ULONG nIndex = (Inst.op1.displacement - 0x38) / sizeof(ULONG);
						pOriginDispatchs[nIndex] = MovTemp;
						pReloadDispatchs[nIndex] = MovTemp - NtfsBase + NewBase;
					}
				}
				if (Inst.length == 0){
					break;
				}else if (Inst.type == INSTRUCTION_TYPE_RET){
					break;
				}
				nCodeLen = Inst.length;
			}

			FixOriginDispatch(pOriginDispatchs);
			FixReloadDispatch(pReloadDispatchs);

			g_hg_map[eHG_NtfsOriginalDispatchs] = (ULONG)pOriginDispatchs;
			g_hg_map[eHG_NtfsReloadDispatchs] = (ULONG)pReloadDispatchs;
		}

		g_hg_map[eHG_ReloadNtfsBase] = NewBase;
	}
}

VOID
InitFastfat(
	)
{
	CHAR pszModuleName[] = "ntoskrnl.exe";
	CHAR pszFunctionName[] = "MmFlushImageSection";
	WCHAR pwszFileSystem[] = L"\\FileSystem\\";
	WCHAR pwszDriverDir[] = L"\\SystemRoot\\System32\\drivers\\";
	ULONG_PTR FastfatBase = g_hg_map[eHG_FastfatBase];
	ULONG_PTR FastfatSize = g_hg_map[eHG_FastfatSize];

	pfnExAllocatePoolWithTag pExAllocatePoolWithTag =
		(pfnExAllocatePoolWithTag)g_hg_map[eHG_ExAllocatePoolWithTag];
	pfnExFreePoolWithTag pExFreePoolWithTag =
		(pfnExFreePoolWithTag)g_hg_map[eHG_ExFreePoolWithTag];
	pfnMmIsAddressValid pMmIsAddressValid =
		(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	WCHAR pwszFastfatPath[SZ_PATH_LEN] = {0};
	WCHAR pwszFastfatSys[] = L"fastfat.sys";
	WCHAR pwszFastfat[] = L"fastfat";
	ULONG_PTR NewBase = 0;
	ULONG_PTR FastfatImpMmFlushImageSection = 0;
	WCHAR pwszFileSystemFastfat[SZ_PATH_LEN] = {0};
	PDRIVER_OBJECT pDriverObject = NULL;

	if ((FastfatBase == 0)			||
		(FastfatSize == 0)				||
		(pExAllocatePoolWithTag == NULL)||
		(pExFreePoolWithTag == NULL)	||
		(pMmIsAddressValid == NULL))
	{
		return;
	}

	wcscpy(pwszFileSystemFastfat, pwszFileSystem);
	wcscat(pwszFileSystemFastfat, pwszFastfat);

	if (NT_SUCCESS(GetDriverObject(pwszFileSystemFastfat, &pDriverObject))){
		g_hg_map[eHG_FastfatDriverObject] = (ULONG_PTR)pDriverObject;
	}

	wcscpy(pwszFastfatPath, pwszDriverDir);
	wcscat(pwszFastfatPath, pwszFastfatSys);

	if (PeLoad_Repair(pwszFastfatPath, &NewBase, FastfatBase)){
		ULONG_PTR Offset = 0;
		PULONG_PTR pOriginDispatchs = NULL;
		PULONG_PTR pReloadDispatchs = NULL;

		if (FindIatFunction((PVOID)NewBase, FastfatSize, 
			pszModuleName, pszFunctionName, &Offset) && Offset)
		{
			FastfatImpMmFlushImageSection = FastfatBase + Offset;
			g_hg_map[eHG_FastfatImpMmFlushImageSection] = FastfatImpMmFlushImageSection;
		}

		pOriginDispatchs = (PULONG_PTR)pExAllocatePoolWithTag(NonPagedPool, 
			sizeof(ULONG_PTR) * 28, HG_MEM_TAG);
		pReloadDispatchs = (PULONG_PTR)pExAllocatePoolWithTag(NonPagedPool,
			sizeof(ULONG_PTR) * 28, HG_MEM_TAG);
		if ((pOriginDispatchs != NULL)	&&
			(pReloadDispatchs != NULL))
		{
			ULONG_PTR pDriverEntry = 0;
			ULONG_PTR nCodeLen = 0;
			ULONG_PTR Address = 0;
			ULONG_PTR MovTemp = 0;

			memset(pOriginDispatchs, 0, sizeof(ULONG_PTR) * 28);
			memset(pReloadDispatchs, 0, sizeof(ULONG_PTR) * 28);

			if (GetDriverEntryPoint((PVOID)NewBase, &pDriverEntry, EPT_EntryPointHead))
			{
				pExFreePoolWithTag(pOriginDispatchs, 0);
				pExFreePoolWithTag(pReloadDispatchs, 0);
				pExFreePoolWithTag((PVOID)NewBase, 0);
				return;
			}

			for (Address = pDriverEntry;
				Address < pDriverEntry + PAGE_SIZE;
				Address += nCodeLen)
			{
				INSTRUCTION Inst;
				if (pMmIsAddressValid((PVOID)Address)){
					break;
				}
				memset(&Inst, 0, sizeof(INSTRUCTION));
				get_instruction(&Inst, (PBYTE)Address, MODE_32);
				if (Inst.type = INSTRUCTION_TYPE_MOV)
				{
					if (Inst.op1.type == OPERAND_TYPE_MEMORY	&&
						Inst.op2.type == OPERAND_TYPE_IMMEDIATE	&&
						Inst.op1.displacement >= 0x38			&&
						Inst.op1.displacement <= 0xA4)
					{
						ULONG_PTR nIndex = (Inst.op1.displacement - 0x38) / sizeof(ULONG_PTR);
						pOriginDispatchs[nIndex] = Inst.op2.immediate;
						pReloadDispatchs[nIndex] = Inst.op2.immediate - FastfatBase + NewBase;
					}
				}

				if (Inst.length == 0){
					break;
				}else if (Inst.type == INSTRUCTION_TYPE_RET){
					break;
				}
				nCodeLen = Inst.length;
			}
			FixOriginDispatch(pOriginDispatchs);
			FixReloadDispatch(pReloadDispatchs);

			g_hg_map[eHG_FastfatOriginalDispatchs] = (ULONG_PTR)pOriginDispatchs;
			g_hg_map[eHG_FastfatReloadDispatchs] = (ULONG_PTR)pReloadDispatchs;
		}
		g_hg_map[eHG_ReloadFastfatBase] = NewBase;
	}
}