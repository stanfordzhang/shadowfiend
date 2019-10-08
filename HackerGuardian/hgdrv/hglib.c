
#include "hglib.h"
#include "hgexport.h"

extern ULONG g_WindowsVersion;
extern PDRIVER_OBJECT	g_pHGDrvObj;

PVOID
GetFunctionAddressByName(
		WCHAR* pwszName
	)
{
	UNICODE_STRING usName;
	PVOID pFunAddr = NULL;
	if (pwszName != NULL){
		RtlInitUnicodeString(&usName, pwszName);
		pFunAddr = MmGetSystemRoutineAddress(&usName);
	}
	return pFunAddr;
}

PVOID
HgMmGetSystemRoutineAddress(
	IN CHAR* SystemRoutineName
	)

/*++

Routine Description:

    This function returns the address of the argument function pointer if
    it is in the kernel or HAL, NULL if it is not.

Arguments:

    SystemRoutineName - Supplies the name of the desired routine.

Return Value:

    Non-NULL function pointer if successful.  NULL if not.

Environment:

    Kernel mode, PASSIVE_LEVEL, arbitrary process context.

--*/

{
	PVOID FunctionAddress = NULL;
	BOOL Found = FALSE;
	PDRIVER_OBJECT DriverObject = g_pHGDrvObj;
	PLDR_DATA_TABLE_ENTRY DriverSection = NULL, LdrEntry = NULL;
	WCHAR szNtoskrnl[20] = {L'n', L't', L'o', L's', L'k', L'r', L'n', L'l', L'.', L'e', L'x', L'e'};
	WCHAR szHal[10] = {L'h', L'a', L'l', L'.', L'd', L'l', L'l'};

	if (!DriverObject || !SystemRoutineName)
	{
		KdPrint(("if (!DriverObject || !SystemRoutineName) error\n"));
		return NULL;
	}

	ASSERT (KeGetCurrentIrql() == PASSIVE_LEVEL);

	DriverSection = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
	LdrEntry = (PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;
	while (LdrEntry && DriverSection != LdrEntry)
	{
		Found = FALSE;

		if ((SIZE_T)LdrEntry->DllBase >= (SIZE_T)MM_SYSTEM_RANGE_START &&
			LdrEntry->BaseDllName.Length > 0	   &&
			LdrEntry->BaseDllName.Buffer != NULL)
		{
			ULONG nChars = LdrEntry->BaseDllName.Length / sizeof(WCHAR);
			ULONG nLen = nChars >= wcslen(szNtoskrnl) ? wcslen(szNtoskrnl) : nChars;

			if (!_wcsnicmp(LdrEntry->BaseDllName.Buffer, szNtoskrnl, nLen)) 
			{
				Found = TRUE;
			}

			if (!Found)
			{
				ULONG nLen = nChars >= wcslen(szHal) ? wcslen(szHal) : nChars;
				if (!_wcsnicmp(LdrEntry->BaseDllName.Buffer, szHal, nLen))
				{
					Found = TRUE;
				}
			}

			if (Found == TRUE) {

				FunctionAddress = HgMiFindExportedRoutineByName(LdrEntry->DllBase, SystemRoutineName);

				if (FunctionAddress != NULL) 
				{
					break;
				}
			}
		}

		LdrEntry = (PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
	}

	return FunctionAddress;
}

PVOID
HgMiFindExportedRoutineByName(
	IN	PVOID DllBase, 
	IN	CHAR* RoutineName
	)

/*++

Routine Description:

    This function searches the argument module looking for the requested
    exported function name.

Arguments:

    DllBase - Supplies the base address of the requested module.

    AnsiImageRoutineName - Supplies the ANSI routine name being searched for.

Return Value:

    The virtual address of the requested routine or NULL if not found.

--*/

{
	USHORT OrdinalNumber;
	PULONG NameTableBase;
	PUSHORT NameOrdinalTableBase;
	PULONG Addr;
	LONG High;
	LONG Low;
	LONG Middle;
	LONG Result;
	ULONG ExportSize;
	PVOID FunctionAddress;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;

	PAGED_CODE();

	if (!DllBase || !RoutineName)
	{
		return NULL;
	}

//	KdPrint(("FindExportedRoutineByName-> DllBase: 0x%p, RoutineName: %s\n", DllBase, RoutineName));

	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY) HgRtlImageDirectoryEntryToData (DllBase,
																				TRUE,
																				IMAGE_DIRECTORY_ENTRY_EXPORT,
																				&ExportSize);

	if (ExportDirectory == NULL) {
		return NULL;
	}

	//
	// Initialize the pointer to the array of RVA-based ansi export strings.
	//

	NameTableBase = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNames);

	//
	// Initialize the pointer to the array of USHORT ordinal numbers.
	//

	NameOrdinalTableBase = (PUSHORT)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNameOrdinals);

	//
	// Lookup the desired name in the name table using a binary search.
	//

	Low = 0;
	Middle = 0;
	High = ExportDirectory->NumberOfNames - 1;

	while (High >= Low) {

		//
		// Compute the next probe index and compare the import name
		// with the export name entry.
		//

		Middle = (Low + High) >> 1;
		
		Result = strcmp (RoutineName,
			(PCHAR)DllBase + NameTableBase[Middle]);

		if (Result < 0) {
			High = Middle - 1;
		}
		else if (Result > 0) {
			Low = Middle + 1;
		}
		else {
			break;
		}
	}

	//
	// If the high index is less than the low index, then a matching
	// table entry was not found. Otherwise, get the ordinal number
	// from the ordinal table.
	//

	if (High < Low) {
		return NULL;
	}

	OrdinalNumber = NameOrdinalTableBase[Middle];

	//
	// If the OrdinalNumber is not within the Export Address Table,
	// then this image does not implement the function.  Return not found.
	//

	if ((ULONG)OrdinalNumber >= ExportDirectory->NumberOfFunctions) {
		return NULL;
	}

	//
	// Index into the array of RVA export addresses by ordinal number.
	//

	Addr = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfFunctions);

	FunctionAddress = (PVOID)((PCHAR)DllBase + Addr[OrdinalNumber]);

	//
	// Forwarders are not used by the kernel and HAL to each other.
	//

	//     ASSERT ((FunctionAddress <= (PVOID)ExportDirectory) ||
	//             (FunctionAddress >= (PVOID)((PCHAR)ExportDirectory + ExportSize)));

	return FunctionAddress;
}

PVOID
HgRtlImageDirectoryEntryToData(
	IN PVOID Base, 
	IN BOOLEAN MappedAsImage, 
	IN USHORT DirectoryEntry, 
	OUT PULONG Size 
	)

/*++

Routine Description:

    This function locates a Directory Entry within the image header
    and returns either the virtual address or seek address of the
    data the Directory describes.

Arguments:

    Base - Supplies the base of the image or data file.

    MappedAsImage - FALSE if the file is mapped as a data file.
                  - TRUE if the file is mapped as an image.

    DirectoryEntry - Supplies the directory entry to locate.

    Size - Return the size of the directory.

Return Value:

    NULL - The file does not contain data for the specified directory entry.

    NON-NULL - Returns the address of the raw data the directory describes.

--*/

{
	PIMAGE_NT_HEADERS NtHeaders;

	if (LDR_IS_DATAFILE(Base)) {
		Base = LDR_DATAFILE_TO_VIEW(Base);
		MappedAsImage = FALSE;
	}

	NtHeaders = HgRtlImageNtHeader(Base);

	if (!NtHeaders)
		return NULL;

	if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		return (HgRtlpImageDirectoryEntryToData32(	Base,
													MappedAsImage,
													DirectoryEntry,
													Size,
													(PIMAGE_NT_HEADERS32)NtHeaders));
	} else if (NtHeaders->OptionalHeader.Magic == IMAGE_NT_OPTIONAL_HDR64_MAGIC) {
		return (HgRtlpImageDirectoryEntryToData64(	Base,
													MappedAsImage,
													DirectoryEntry,
													Size,
													(PIMAGE_NT_HEADERS64)NtHeaders));
	} else {
		return (NULL);
	}
}

PIMAGE_NT_HEADERS
NTAPI
HgRtlImageNtHeader(
	IN	PVOID	Base
	)
{
	PIMAGE_NT_HEADERS NtHeaders = NULL;
	(VOID)HgRtlImageNtHeaderEx(RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK, Base, 0, &NtHeaders);
	return NtHeaders;
}

NTSTATUS
NTAPI
HgRtlImageNtHeaderEx(
	IN	ULONG	Flags,
	IN	PVOID	Base,
	IN	ULONG64	Size,
	OUT	PIMAGE_NT_HEADERS * OutHeaders
	)
{
	PIMAGE_NT_HEADERS NtHeaders = 0;
	ULONG e_lfanew = 0;
	BOOLEAN RangeCheck = 0;
	NTSTATUS Status = 0;
	const ULONG ValidFlags = 
		RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK;

	if (OutHeaders != NULL) {
		*OutHeaders = NULL;
	}
	if (OutHeaders == NULL) {
		Status = STATUS_INVALID_PARAMETER;
		goto Exit;
	}
	if ((Flags & ~ValidFlags) != 0) {
		Status = STATUS_INVALID_PARAMETER;
		goto Exit;
	}
	if (Base == NULL || Base == (PVOID)(LONG_PTR)-1) {
		Status = STATUS_INVALID_PARAMETER;
		goto Exit;
	}

	RangeCheck = ((Flags & RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK) == 0);
	if (RangeCheck) {
		if (Size < sizeof(IMAGE_DOS_HEADER)) {
			Status = STATUS_INVALID_IMAGE_FORMAT;
			goto Exit;
		}
	}

	//
	// Exception handling is not available in the boot loader, and exceptions
	// were not historically caught here in kernel mode. Drivers are considered
	// trusted, so we can't get an exception here due to a bad file, but we
	// could take an inpage error.
	//
#define EXIT goto Exit
	if (((PIMAGE_DOS_HEADER)Base)->e_magic != IMAGE_DOS_SIGNATURE) {
		Status = STATUS_INVALID_IMAGE_FORMAT;
		EXIT;
	}
	e_lfanew = ((PIMAGE_DOS_HEADER)Base)->e_lfanew;
	if (RangeCheck) {
		if (e_lfanew >= Size
#define SIZEOF_PE_SIGNATURE 4
			|| e_lfanew >= (MAXULONG - SIZEOF_PE_SIGNATURE - sizeof(IMAGE_FILE_HEADER))
			|| (e_lfanew + SIZEOF_PE_SIGNATURE + sizeof(IMAGE_FILE_HEADER)) >= Size
			) {
				Status = STATUS_INVALID_IMAGE_FORMAT;
				EXIT;
		}
	}

	NtHeaders = (PIMAGE_NT_HEADERS)((PCHAR)Base + e_lfanew);

	//
	// In kernelmode, do not cross from usermode address to kernelmode address.
	//
	if (Base < MM_HIGHEST_USER_ADDRESS) {
		if ((PVOID)NtHeaders >= MM_HIGHEST_USER_ADDRESS) {
			Status = STATUS_INVALID_IMAGE_FORMAT;
			EXIT;
		}
		//
		// Note that this check is slightly overeager since IMAGE_NT_HEADERS has
		// a builtin array of data_directories that may be larger than the image
		// actually has. A better check would be to add FileHeader.SizeOfOptionalHeader,
		// after ensuring that the FileHeader does not cross the u/k boundary.
		//
		if ((PVOID)((PCHAR)NtHeaders + sizeof (IMAGE_NT_HEADERS)) >= MM_HIGHEST_USER_ADDRESS) {
			Status = STATUS_INVALID_IMAGE_FORMAT;
			EXIT;
		}
	}

	if (NtHeaders->Signature != IMAGE_NT_SIGNATURE) {
		Status = STATUS_INVALID_IMAGE_FORMAT;
		EXIT;
	}
	Status = STATUS_SUCCESS;

Exit:
	if (NT_SUCCESS(Status)) {
		*OutHeaders = NtHeaders;
	}
	return Status;
}

PVOID
HgRtlpImageDirectoryEntryToData32(
	IN	PVOID	Base,
	IN	BOOLEAN	MappedAsImage,
	IN	USHORT	DirectoryEntry,
	OUT	PULONG	Size,
	PIMAGE_NT_HEADERS32 NtHeaders
	)
{
	ULONG DirectoryAddress;

	if (DirectoryEntry >= NtHeaders->OptionalHeader.NumberOfRvaAndSizes) {
		return( NULL );
	}

	if (!(DirectoryAddress = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].VirtualAddress)) {
		return( NULL );
	}

	if (Base < MM_HIGHEST_USER_ADDRESS) {
		if ((PVOID)((PCHAR)Base + DirectoryAddress) >= MM_HIGHEST_USER_ADDRESS) {
			return( NULL );
		}
	}

	*Size = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].Size;
	if (MappedAsImage || DirectoryAddress < NtHeaders->OptionalHeader.SizeOfHeaders) {
		return( (PVOID)((PCHAR)Base + DirectoryAddress) );
	}

	return( HgRtlAddressInSectionTable((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}

PVOID
HgRtlpImageDirectoryEntryToData64(
	IN	PVOID	Base,
	IN	BOOLEAN	MappedAsImage,
	IN	USHORT	DirectoryEntry,
	OUT	PULONG	Size,
	PIMAGE_NT_HEADERS64 NtHeaders
	)
{
	ULONG DirectoryAddress;

	if (DirectoryEntry >= NtHeaders->OptionalHeader.NumberOfRvaAndSizes) {
		return( NULL );
	}

	if (!(DirectoryAddress = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].VirtualAddress)) {
		return( NULL );
	}

	if (Base < MM_HIGHEST_USER_ADDRESS) {
		if ((PVOID)((PCHAR)Base + DirectoryAddress) >= MM_HIGHEST_USER_ADDRESS) {
			return( NULL );
		}
	}

	*Size = NtHeaders->OptionalHeader.DataDirectory[ DirectoryEntry ].Size;
	if (MappedAsImage || DirectoryAddress < NtHeaders->OptionalHeader.SizeOfHeaders) {
		return( (PVOID)((PCHAR)Base + DirectoryAddress) );
	}

	return( HgRtlAddressInSectionTable((PIMAGE_NT_HEADERS)NtHeaders, Base, DirectoryAddress ));
}

PVOID
HgRtlAddressInSectionTable(
	IN	PIMAGE_NT_HEADERS NtHeaders,
	IN	PVOID	Base,
	IN	ULONG	Address
	)
{
	PIMAGE_SECTION_HEADER NtSection;

	NtSection = SzhRtlSectionTableFromVirtualAddress(	NtHeaders,
														Base,
														Address);
	if (NtSection != NULL) {
		return( ((PCHAR)Base + ((ULONG_PTR)Address - NtSection->VirtualAddress) + NtSection->PointerToRawData) );
	}
	else {
		return( NULL );
	}
}

PIMAGE_SECTION_HEADER
	SzhRtlSectionTableFromVirtualAddress(
	IN	PIMAGE_NT_HEADERS NtHeaders,
	IN	PVOID	Base,
	IN	ULONG	Address
	)
{
	ULONG i;
	PIMAGE_SECTION_HEADER NtSection;

	NtSection = IMAGE_FIRST_SECTION( NtHeaders );
	for (i = 0; i < NtHeaders->FileHeader.NumberOfSections; i++) 
	{
		if ((ULONG)Address >= NtSection->VirtualAddress &&
			(ULONG)Address < NtSection->VirtualAddress + NtSection->SizeOfRawData) 
		{
			return NtSection;
		}

		++NtSection;
	}

	return NULL;
}

NTSTATUS
	GetDriverObject(
	__in	PWCHAR			pwszDriverName,
	__in	PDRIVER_OBJECT*	pDriverObject
	)
{
	PDRIVER_OBJECT DriverObject;
	UNICODE_STRING ObjectName;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	pfnObReferenceObjectByName pObReferenceObjectByName = 
		(pfnObReferenceObjectByName)g_hg_map[eHG_ObReferenceObjectByName];
	POBJECT_TYPE DriverObjectType = (POBJECT_TYPE)g_hg_map[eHG_IoDriverObjectType];

	if ((pObReferenceObjectByName == NULL)	||
		(pwszDriverName == NULL)			||
		(pDriverObject == NULL)				||
		(DriverObjectType == NULL))
	{
		return status;
	}

	SzhInitUnicodeString(&ObjectName, pwszDriverName);

	status = pObReferenceObjectByName(	&ObjectName,
										OBJ_CASE_INSENSITIVE,
										NULL,
										0,
										DriverObjectType,
										KernelMode,
										NULL,
										(PVOID*)&DriverObject);
	if (NT_SUCCESS(status)){
		*pDriverObject = DriverObject;
	}else{
		*pDriverObject = NULL;
	}
	return status;
}

VOID
SzhInitUnicodeString(
	__inout	PUNICODE_STRING DestinationString,
	__in	PCWSTR			SourceString
	)
{
	ULONG DestSize;
	if (SourceString != NULL){
		DestSize = wcslen(SourceString) * sizeof(WCHAR);
		DestinationString->Length = (USHORT)DestSize;
		DestinationString->MaximumLength = (USHORT)DestSize + sizeof(WCHAR);
	}else{
		DestinationString->Length = 0;
		DestinationString->MaximumLength = 0;
	}
	DestinationString->Buffer = (PWCHAR)SourceString;
}

BOOL
FindIatFunction(
	__in	PVOID	pBase,
	__in	ULONG_PTR	ulModuleSize,
	__in	PCHAR	ImportDllName,
	__in	PCHAR	ImportApiName,
	__out	PULONG_PTR	ImportFunOffset
	)
{
	BOOL bRet = FALSE;
	ULONG ulRet = 0;
	PIMAGE_IMPORT_DESCRIPTOR pImportModuleDirectory = NULL;
	ULONG *OriginalFirstThunk = NULL;
	ULONG *FirstThunk = NULL;

	if ((pBase != NULL)						&&
		(ImportDllName != NULL)				&&
		(ImportApiName != NULL)				&&
		(ImportFunOffset != NULL)			&&
		(KeGetCurrentIrql() == PASSIVE_LEVEL))
	{
		__try{
			pImportModuleDirectory = (PIMAGE_IMPORT_DESCRIPTOR)
				HgRtlImageDirectoryEntryToData(pBase,
												TRUE,
												IMAGE_DIRECTORY_ENTRY_IMPORT,
												&ulRet);

			if (pImportModuleDirectory){
				size_t nDllNameLen = strlen(ImportDllName);
				size_t nApiNameLen = strlen(ImportApiName);

				while (pImportModuleDirectory && pImportModuleDirectory->Name){
					PCHAR strModuleName = (PCHAR)((PBYTE)pBase + pImportModuleDirectory->Name);

					if (!_strnicmp(strModuleName, ImportDllName, nDllNameLen)){
						PIMAGE_IMPORT_BY_NAME pImageImportByName = NULL;
						ULONG i = 0;

						OriginalFirstThunk = (ULONG*)((CHAR*)pBase + pImportModuleDirectory->OriginalFirstThunk);
						FirstThunk = (ULONG *)((CHAR*)pBase + pImportModuleDirectory->FirstThunk);

						for (i = 0; FirstThunk[i] && OriginalFirstThunk[i]; i++){
							if ((PVOID)OriginalFirstThunk[i] < (PVOID)pBase){
								pImageImportByName = (PIMAGE_IMPORT_BY_NAME)(OriginalFirstThunk[i] + (PBYTE)pBase);
							}else{
								pImageImportByName = (PIMAGE_IMPORT_BY_NAME)OriginalFirstThunk[i];
							}

							if (pImageImportByName							&&
								(PBYTE)pImageImportByName > (PBYTE)pBase	&&
								(PBYTE)pImageImportByName < (PBYTE)((PBYTE)pBase + ulModuleSize))
							{
								if (!strncmp((PCHAR)&pImageImportByName->Name, ImportApiName, nApiNameLen))
								{
									*ImportFunOffset = (ULONG)&FirstThunk[i] - (ULONG)pBase;
									bRet = TRUE;
									break;
								}
							}
						}
						break;
					}
					pImportModuleDirectory ++;
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			bRet = FALSE;
		}
	}

	return bRet;
}

BOOL 
GetDriverEntryPoint(
	__in	PVOID				ImageBase,
	__out	PULONG_PTR			pOutDriverEntry,
	__in	ENTRY_POINT_TYPE	EntryType
	)
{
	BOOL bRet = FALSE;
	PIMAGE_NT_HEADERS NtHeader = NULL;
	ULONG_PTR EntryPoint = 0;

	pfnMmIsAddressValid pMmIsAddressValid = 
		(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if ((ImageBase == NULL)				||
		(pOutDriverEntry == NULL)		||
		(pMmIsAddressValid == NULL)		||
		(EntryType == EPT_EntryNone))
	{
		return FALSE;
	}

	NtHeader = HgRtlImageNtHeader(ImageBase);
	if (NtHeader != NULL){
		DWORD dwEntryPoint = 0;
		DWORD dwCurAddress = 0;
		DWORD Length = 0;
		//PUCHAR pOpcode;

		EntryPoint = NtHeader->OptionalHeader.AddressOfEntryPoint;
		EntryPoint += (ULONG_PTR)ImageBase;
		dwEntryPoint = (DWORD)EntryPoint;

		if (EntryType = EPT_EntryPointHead){
			*pOutDriverEntry = dwEntryPoint;
			bRet = TRUE;
		}else if (EntryType == EPT_EntryPointJmp){
			for (dwCurAddress = dwEntryPoint; 
				dwCurAddress < dwEntryPoint + PAGE_SIZE; 
				dwCurAddress += Length)
			{
				//Length = SizeOfCode((PUCHAR)dwCurAddress, &pOpcode);
				//////////////////////////////////////////////////////////////////////////
				DISASM MyDisasm;
				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)dwCurAddress;
				Length = Disasm(&MyDisasm);
				//////////////////////////////////////////////////////////////////////////
				if (Length == 5 && *(PBYTE)dwCurAddress == 0xE9){
					ULONG pJmpAddress = dwCurAddress + *(PULONG)(dwCurAddress + 1) + 5;
					if (pJmpAddress > (SIZE_T)MM_SYSTEM_RANGE_START && pMmIsAddressValid((PVOID)pJmpAddress))
					{
						*pOutDriverEntry = pJmpAddress;
						bRet = TRUE;
						break;
					}
				}
			}
		}
	}
	return bRet;
}

VOID
FixOriginDispatch(
	__in	PULONG_PTR	pOriginDispatchs
	)
{
	ULONG_PTR IopInvalidDeviceRequest = g_hg_map[eHG_IopInvalidDeviceRequest];
	ULONG_PTR i = 0;
	pfnMmIsAddressValid pMmIsAddressValid = (pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if (IopInvalidDeviceRequest == 0){
		GetIopInvalidDeviceRequest();
		IopInvalidDeviceRequest = g_hg_map[eHG_IopInvalidDeviceRequest];
	}

	for (i = 0; i < 28; i++){
		if (!pOriginDispatchs[i]){
			pOriginDispatchs[i] = IopInvalidDeviceRequest;
		}
	}
}

VOID
	GetIopInvalidDeviceRequest(
	)
{
	ULONG_PTR pIoCreateDriver = g_hg_map[eHG_IoCreateDriver];
	WIN_VERSION Version = (WIN_VERSION)g_WindowsVersion;

	pfnMmIsAddressValid pMmIsAddressValid = 
		(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if (pIoCreateDriver && pMmIsAddressValid){
		ULONG_PTR Address = 0;
		ULONG_PTR nCodeLen = 0;

		for (Address = pIoCreateDriver; 
			Address < pIoCreateDriver + PAGE_SIZE; 
			Address += nCodeLen)
		{
			//PCHAR pCode;
			if (pMmIsAddressValid((PVOID)Address) == FALSE){
				break;
			}

			//nCodeLen = SizeOfCode((PVOID)Address, (unsigned char**)&pCode);
			//////////////////////////////////////////////////////////////////////////
			{
				DISASM MyDisasm;
				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)Address;
				nCodeLen = Disasm(&MyDisasm);
			}
			//////////////////////////////////////////////////////////////////////////
			if (nCodeLen == 0){
				break;
			}

			if (Version == eWin_2000)
			{
				if (nCodeLen == 7			&& 
					*(PBYTE)Address == 0xC7 &&
					pMmIsAddressValid((PVOID)(*(PULONG)(Address + 3))))
				{
					g_hg_map[eHG_IopInvalidDeviceRequest] = *(PULONG)(Address + 3);
				}
			}else{
				if (nCodeLen == 5			&& 
					*(PBYTE)Address == 0xB8	&&
					pMmIsAddressValid((PVOID)(*(PULONG)(Address + 1))))
				{
					g_hg_map[eHG_IopInvalidDeviceRequest] = *(PULONG)(Address + 1);
					break;
				}
			}
		}
	}
}

VOID
FixReloadDispatch(
	__in	PULONG_PTR	pReloadDispatchs
	)
{
	ULONG_PTR IopInvalidDeviceRequest = g_hg_map[eHG_IopInvalidDeviceRequest];
	ULONG_PTR OriginBase = g_hg_map[eHG_CurrentKernelBase];
	ULONG_PTR ReloadBase = g_hg_map[eHG_NewKernelBase];	
	ULONG_PTR i = 0;
	pfnMmIsAddressValid pMmisAddressValid = 
		(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];

	if (IopInvalidDeviceRequest == 0){
		GetIopInvalidDeviceRequest();
		IopInvalidDeviceRequest = g_hg_map[eHG_IopInvalidDeviceRequest];
	}

	IopInvalidDeviceRequest = IopInvalidDeviceRequest - OriginBase + ReloadBase;

	for (i = 0; i < 28; i++){
		if (!pReloadDispatchs[i]){
			pReloadDispatchs[i] = IopInvalidDeviceRequest;
		}
	}
}

/*
** UNICODE_STRING Members
** Length
**	The length, in bytes, of the string stored in Buffer.
** MaximumLength
**	The length, in bytes, of Buffer.
** Buffer
**	Pointer to a buffer used to contain a string of wide characters.
*/
BOOL
IsUnicodeStringValid(
	__in	PUNICODE_STRING	uString
	)
{
	BOOL bRtn = FALSE;

	if (uString == NULL){
		return FALSE;
	}

	if ((uString->Length > 0)							&&
		(uString->Buffer != NULL)						&&
		(HgMmIsAddressValid(uString->Buffer) == TRUE)	&&
		(HgMmIsAddressValid(&uString->Buffer[uString->Length / sizeof(WCHAR) - 1]) == TRUE))
	{
		return TRUE;
	}

	return bRtn;
}

size_t
GetUnicodeStringSafeLength(
	__in	PUNICODE_STRING	uString,
	__in	USHORT			nLen
	)
{
	return nLen > uString->Length / sizeof(WCHAR) ?
		uString->Length / sizeof(WCHAR) : nLen - 1;
}

POBJECT_TYPE
KeGetObjectType(
	__in	PVOID	pObject
	)
{
	POBJECT_TYPE pObjectType = NULL;
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
			pfnObGetObjectType ObGetObjectType = 
				(pfnObGetObjectType)g_hg_map[eHG_ObGetObjectType];
			if (ObGetObjectType != NULL){
				KdPrint(("<Using reloaded ObGetObjectType>\n"));
				pObjectType = ObGetObjectType(pObject);
			}else{
				WCHAR pwszName[] = L"ObGetObjectType";
				UNICODE_STRING uName;
				RtlInitUnicodeString(&uName, pwszName);	
				ObGetObjectType = (pfnObGetObjectType)MmGetSystemRoutineAddress(&uName);
				if (ObGetObjectType != NULL){
					pObjectType = ObGetObjectType(pObject);
				}
			}
		}
		break;
	}
	return pObjectType;
}

BOOL
GetNtoskrnlInfo(
	__out	WCHAR*	pwszNtoskrnlPath,
	__out	PSIZE_T	SystemKernelModuleBase,
	__out	PSIZE_T	SystemKernelModuleSize
	)
{
	BOOL					bRet = TRUE;

	SIZE_T					nSize = 0;
	SIZE_T					i = 0;
	PRTL_PROCESS_MODUELS	pModules = NULL;
	PCHAR					pszKernelName = NULL;
	ANSI_STRING				aKernelName;
	UNICODE_STRING			uKernelName;

	if ((pwszNtoskrnlPath == NULL) 
		|| (SystemKernelModuleBase == NULL) 
		|| (SystemKernelModuleSize == NULL))
	{
		return FALSE;
	}

	if (ZwQuerySystemInformation(SystemModuleInformation, NULL, 0, &nSize) != STATUS_INFO_LENGTH_MISMATCH)
	{
		return FALSE;
	}

	pModules = (PRTL_PROCESS_MODUELS)ExAllocatePoolWithTag(	NonPagedPool,
		nSize,
		HG_MEM_TAG);
	if (pModules != NULL)
	{
		memset(pModules, 0, nSize);
		if (!NT_SUCCESS(ZwQuerySystemInformation(SystemModuleInformation, pModules, nSize, &nSize)))
		{
			ExFreePool(pModules);
			PELog(("Failed to query module information!\n"));
			return FALSE;
		}
	}
	// ntoskrnl.exe is the first module in the module list.
	pszKernelName = (PCHAR)(pModules->Modules[0].OffsetToFileName + pModules->Modules[0].FullPathName);
	PELog(("pszKernelName = %s\n", pszKernelName));
	RtlInitAnsiString(&aKernelName, pszKernelName);
	RtlAnsiStringToUnicodeString(&uKernelName, &aKernelName, TRUE);

	wcscat(pwszNtoskrnlPath, L"\\SystemRoot\\system32\\");
	memcpy(pwszNtoskrnlPath + wcslen(L"\\SystemRoot\\system32\\"), uKernelName.Buffer, uKernelName.Length);
	*SystemKernelModuleBase = (SIZE_T)pModules->Modules[0].ImageBase;
	*SystemKernelModuleSize = (SIZE_T)pModules->Modules[0].ImageSize;

	ExFreePool(pModules);
	RtlFreeUnicodeString(&uKernelName);

	return bRet;
}

BOOL ImagePEFile(
	__in	PVOID	FileBuffer,
	__inout PVOID*	ImageModuleBase
	)
{
	BOOL bRet = FALSE;
	PIMAGE_DOS_HEADER ImageDosHeader = NULL;
	PIMAGE_NT_HEADERS ImageNtHeader = NULL;
	PIMAGE_SECTION_HEADER ImageSectionHeader = NULL;
	DWORD FileAlignment = 0;
	DWORD SectionAlignment = 0;
	DWORD NumberOfSections = 0;
	DWORD SizeOfImage = 0;
	DWORD SizeOfHeaders = 0;
	DWORD Index = 0;
	PVOID ImageBase = NULL;
	DWORD SizeOfNtHeaders = 0;
	ULONG i = 0;
	ULONG uLen = 0;

	DWORD FileOffset = 0;
	DWORD Length = 0;
	DWORD ImageOffset = 0;

	if ((FileBuffer == NULL) || (ImageModuleBase == NULL)){
		return bRet;
	}
	ImageDosHeader = (PIMAGE_DOS_HEADER)FileBuffer;
	if (ImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE){
		return bRet;
	}
	ImageNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)FileBuffer + ImageDosHeader->e_lfanew);
	if(ImageNtHeader->Signature != IMAGE_NT_SIGNATURE){
		return bRet;
	}

	FileAlignment = ImageNtHeader->OptionalHeader.FileAlignment;
	SectionAlignment = ImageNtHeader->OptionalHeader.SectionAlignment;
	NumberOfSections = ImageNtHeader->FileHeader.NumberOfSections;
	SizeOfImage = ImageNtHeader->OptionalHeader.SizeOfImage;
	SizeOfHeaders = ImageNtHeader->OptionalHeader.SizeOfHeaders;
	SizeOfImage = AlignSize(SizeOfImage, SectionAlignment);

	//	PELog(("FileAlignment = 0x%p, SectionAlignment = 0x%p\n", FileAlignment, SectionAlignment));
	//	PELog(("SizeOfImage = %d, SizeOfHeaders = 0x%p, SizeOfImage = 0x%p\n", SizeOfImage, SizeOfHeaders, SizeOfImage));
	ImageBase = ExAllocatePoolWithTag(NonPagedPool, SizeOfImage, HG_MEM_TAG);
	if(ImageBase == NULL){
		do {
			i = 1;
			uLen = SizeOfImage + 2048 * i;
			ImageBase = ExAllocatePoolWithTag(NonPagedPool, uLen, 0);
		} while (ImageBase == NULL && ++i < 10);
	}

	if (ImageBase == NULL){
		return bRet;
	}

	RtlZeroMemory(ImageBase, SizeOfImage);

	SizeOfNtHeaders = sizeof(ImageNtHeader->FileHeader) + sizeof(ImageNtHeader->Signature) + ImageNtHeader->FileHeader.SizeOfOptionalHeader;
	ImageSectionHeader = (PIMAGE_SECTION_HEADER)((SIZE_T)ImageNtHeader + SizeOfNtHeaders);

	for(Index = 0; Index < NumberOfSections; Index++){
		ImageSectionHeader[Index].SizeOfRawData = AlignSize(ImageSectionHeader[Index].SizeOfRawData, FileAlignment);
		ImageSectionHeader[Index].Misc.VirtualSize = AlignSize(ImageSectionHeader[Index].Misc.VirtualSize, SectionAlignment);
	}

	if (ImageSectionHeader[NumberOfSections - 1].VirtualAddress +
		ImageSectionHeader[NumberOfSections - 1].SizeOfRawData > SizeOfImage)
	{
		ImageSectionHeader[NumberOfSections - 1].SizeOfRawData = SizeOfImage - ImageSectionHeader[NumberOfSections - 1].VirtualAddress;
	}

	RtlCopyMemory(ImageBase, FileBuffer, SizeOfHeaders);
	//	PELog(("NumberOfSections = 0x%p", NumberOfSections));
	for(Index = 0; Index < NumberOfSections; Index++){
//		UCHAR SectionName[9] = {0};
		FileOffset = ImageSectionHeader[Index].PointerToRawData;
		Length = ImageSectionHeader[Index].SizeOfRawData;
		ImageOffset = ImageSectionHeader[Index].VirtualAddress;
//		RtlCopyMemory(SectionName, ImageSectionHeader[Index].Name, 8);
//		LibLog(("Name = %s, FileOffset = 0x%08X, Length = 0x%08X, ImageOffset = 0x%08X\n", SectionName, FileOffset, Length, ImageOffset));
		RtlCopyMemory(&((PBYTE)ImageBase)[ImageOffset], &((PBYTE)FileBuffer)[FileOffset], Length);
	}

	*ImageModuleBase = ImageBase;
	bRet = TRUE;

	return bRet;
}

BOOL FixImportTable(
	__in	PBYTE	ImageBase,
	__in	SIZE_T	ExistImageBase
	)
{
	BOOL bRet = FALSE;
	PIMAGE_IMPORT_DESCRIPTOR ImageImportDescriptor = NULL;
	PIMAGE_THUNK_DATA ImageThunkData = NULL;
	PIMAGE_THUNK_DATA FirstThunk = NULL;
	PIMAGE_IMPORT_BY_NAME ImportByName = NULL;
	DWORD ImportSize = 0;
	PVOID ModuleBase = NULL;
	PVOID FunctionAddress = 0;
	PIMAGE_DOS_HEADER ImageDosHeader = NULL;
	PIMAGE_NT_HEADERS ImageNtHeader = NULL;

	ImageDosHeader = (PIMAGE_DOS_HEADER)ImageBase;
	if (ImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE){
		return bRet;
	}

	ImageNtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)ImageBase + ImageDosHeader->e_lfanew);
	if (ImageNtHeader->Signature != IMAGE_NT_SIGNATURE){
		return bRet;
	}

	ImageImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)HgRtlImageDirectoryEntryToData(ImageBase, TRUE, IMAGE_DIRECTORY_ENTRY_IMPORT, &ImportSize);
	//LibLog(("<hglib.c>.<FixImportTable>: ImageImportDescriptor = 0x%p, ImportSize = 0x%p\n", ImageImportDescriptor, ImportSize));

	if (ImageImportDescriptor == NULL){
		return bRet;
	}

	while (ImageImportDescriptor->OriginalFirstThunk && ImageImportDescriptor->Name)
	{
		CHAR ModuleName[SZ_PATH_LEN] = {0};
		strcpy(ModuleName, (CHAR*)(ImageBase + ImageImportDescriptor->Name));

		if (_stricmp(ModuleName, "ntkrnlpa.exe") == 0	||
			_stricmp(ModuleName, "ntoskrnl.exe") == 0	||
			_stricmp(ModuleName, "ntkrnlmp.exe") == 0	||
			_stricmp(ModuleName, "ntkrpamp.exe") == 0)
		{
			ModuleBase = GetKernelModuleBase("ntkrnlpa.exe");
			if (ModuleBase == NULL){
				ModuleBase = GetKernelModuleBase("ntoskrnl.exe");
				if (ModuleBase == NULL){
					ModuleBase = GetKernelModuleBase("ntkrnlmp.exe");
					if (ModuleBase == NULL){
						ModuleBase = GetKernelModuleBase("ntkrpamp.exe");
					}
				}
			}
		}else if (!_stricmp(ModuleName, "HAL.dll")){
			ModuleBase = GetKernelModuleBase(ModuleName);
			if(ModuleBase == NULL){
				ModuleBase = GetKernelModuleBase("halmacpi.dll");
			}
		}else{
			ModuleBase= GetKernelModuleBase(ModuleName);
		}
		//LibLog(("<hglib.c>.<FixImportTable>: Name = %s, Base = 0x%p\n", ModuleName, ModuleBase));

		if (ModuleBase == NULL){
			//LibLog(("<hglib.c>.<FixImportTable>: Can't find module: %s\n", ModuleName));
			FirstThunk = (PIMAGE_THUNK_DATA)(ImageBase + ImageImportDescriptor->FirstThunk);
			InsertOriginalFirstThunk((SIZE_T)ImageBase, ExistImageBase, FirstThunk);
			ImageImportDescriptor++;
			continue;
		}
		ImageThunkData = (PIMAGE_THUNK_DATA)(ImageBase + ImageImportDescriptor->OriginalFirstThunk);
		FirstThunk = (PIMAGE_THUNK_DATA)(ImageBase + ImageImportDescriptor->FirstThunk);

		while (ImageThunkData->u1.Ordinal)
		{
			if (IMAGE_SNAP_BY_ORDINAL32(ImageThunkData->u1.Ordinal))
			{
				//LibLog(("Ordinal = %d\n", ImageThunkData->u1.Ordinal));
				FunctionAddress = (PVOID)MiFindExportedRoutine(ModuleBase, FALSE, NULL, ImageThunkData->u1.Ordinal & ~IMAGE_ORDINAL_FLAG32);
				if (FunctionAddress == 0){
					//LibLog(("Can't find function ordinal: %d\n", ImageThunkData->u1.Ordinal & ~IMAGE_ORDINAL_FLAG32));
					return bRet;
				}
				FirstThunk->u1.Function = (SIZE_T)FunctionAddress;
			}else{
				ImportByName = (PIMAGE_IMPORT_BY_NAME)(ImageBase + ImageThunkData->u1.AddressOfData);
				//LibLog(("Name = %s\n", ImportByName->Name));
				FunctionAddress = (PVOID)MiFindExportedRoutine(ModuleBase, TRUE, (PCHAR)(ImportByName->Name), 0);
				if (FunctionAddress == 0){
					//LibLog(("Can't find function name: %s\n", ImportByName->Name));
					return bRet;
				}
				FirstThunk->u1.Function = (SIZE_T)FunctionAddress;
			}
			FirstThunk++;
			ImageThunkData++;
		}
		ImageImportDescriptor++;
	}

	return TRUE;
}

PVOID GetKernelModuleBase(
	__in	PCHAR	KernelModuleName
	)
{
	PVOID pRtn = NULL;
	ANSI_STRING aKernelModuleName;
	UNICODE_STRING uKernelModuleName;
	UNICODE_STRING uModuleName;
	ULONG Length = 0;
	ULONG Index = 0;
	PDRIVER_OBJECT DriverObject = g_pHGDrvObj;
	SIZE_T nDriverSection = 0;
	PUNICODE_STRING FullDllName;
	SIZE_T DllBase = 0;
	SIZE_T LdrTable = 0;

	if ((DriverObject == NULL) || (KernelModuleName == NULL)){
		return pRtn;
	}

	RtlInitAnsiString(&aKernelModuleName, KernelModuleName);
	RtlAnsiStringToUnicodeString(&uKernelModuleName, &aKernelModuleName, TRUE);

	nDriverSection = (SIZE_T)(DriverObject->DriverSection);
	LdrTable = (SIZE_T)((PLIST_ENTRY)LOCATE_FIELD(nDriverSection, GetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks)))->Flink;
	FullDllName = (PUNICODE_STRING)LOCATE_FIELD(LdrTable, GetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName));
	DllBase = *(PSIZE_T)LOCATE_FIELD(LdrTable, GetOffset(_LDR_DATA_TABLE_ENTRY_DllBase));

//	LibLog(("[hglib.c].<GetKernelModuleBase>: Name = %S, DriverSection = 0x%p, LdrEntry = 0x%p\n", uKernelModuleName.Buffer, nDriverSection, LdrTable));

	while (LdrTable && nDriverSection != LdrTable)
	{
		if ((SIZE_T)DllBase >= (SIZE_T)MM_SYSTEM_RANGE_START	&&
			FullDllName->Length > 0								&&
			FullDllName->Buffer != NULL)
		{
 			if (MmIsAddressValid(&FullDllName->	Buffer[FullDllName->Length / sizeof(WCHAR) - 1]))
			{
				WCHAR Buffer[SZ_PATH_LEN] = {0};
 
 				Length = FullDllName->Length / sizeof(WCHAR);
				for (Index = Length - 1; Index > 0; Index--)
				{
					if (FullDllName->Buffer[Index] == L'\\'){
						break;
					}
				}
				//LibLog(("[hglib.c].<GetKernelModuleBase64>: Length = %d, Index = %d\n", Length, Index));
 
				if (FullDllName->Buffer[Index] == L'\\'){
					RtlCopyMemory(Buffer, &(FullDllName->Buffer[Index + 1]), (Length - Index - 1) * sizeof(WCHAR));
					uModuleName.Buffer = Buffer;
					uModuleName.Length = (USHORT)((Length - Index - 1) * sizeof(WCHAR));
					uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
				}else{
					RtlCopyMemory(Buffer, FullDllName->Buffer, Length * sizeof(WCHAR));
					uModuleName.Buffer = Buffer;
					uModuleName.Length = (USHORT)(Length * sizeof(WCHAR));
					uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
 				}
//				LibLog(("[hglib.c].<GetKernelModuleBase64>: Module Name: %S, Base: 0x%p\n", uModuleName.Buffer, DllBase));
				if (RtlEqualUnicodeString(&uModuleName, &uKernelModuleName, TRUE)){
					return (PVOID)DllBase;
				}
 			}
		}
		LdrTable = (SIZE_T)((PLIST_ENTRY)LOCATE_FIELD(LdrTable, GetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks)))->Flink;
		FullDllName = (PUNICODE_STRING)LOCATE_FIELD(LdrTable, GetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName));
		DllBase = *(PSIZE_T)LOCATE_FIELD(LdrTable, GetOffset(_LDR_DATA_TABLE_ENTRY_DllBase));
	}

	return pRtn;
}

BOOL InsertOriginalFirstThunk(
	__in	SIZE_T				ImageBase,
	__in	SIZE_T				ExistImageBase,
	__out	PIMAGE_THUNK_DATA	FirstThunk
	)
{
	BOOL bRet = FALSE;
	SIZE_T Offset = 0;
	PIMAGE_THUNK_DATA OriginalFirstThunk = NULL;

	Offset = (SIZE_T)FirstThunk - ImageBase;
	OriginalFirstThunk = (PIMAGE_THUNK_DATA)(ExistImageBase + Offset);

	while (OriginalFirstThunk->u1.Function){
//		LibLog(("Function Address: 0x%p\n", OriginalFirstThunk->u1.Function));
		FirstThunk->u1.Function = OriginalFirstThunk->u1.Function;
		OriginalFirstThunk++;
		FirstThunk++;
	}
	bRet = TRUE;

	return bRet;
}

PVOID MiFindExportedRoutine(
	__in	PVOID	DllBase,
	__in	BOOL	ByName,
	__in	PCHAR	RoutineName,
	__in	DWORD	Ordinal
	)
{
	USHORT OrdinalNumber;
	PULONG NameTableBase;
	PUSHORT NameOrdinalTableBase;
	PULONG AddressTableBase;
	PULONG Addr;
	LONG High;
	LONG Low;
	LONG Middle;
	LONG Result;
	ULONG ExportSize;
	PVOID FunctionAddress;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;

	PAGED_CODE();

	ExportDirectory = (PIMAGE_EXPORT_DIRECTORY) HgRtlImageDirectoryEntryToData (DllBase,
		TRUE,
		IMAGE_DIRECTORY_ENTRY_EXPORT,
		&ExportSize);

	if (ExportDirectory == NULL) {
		return NULL;
	}

	//
	// Initialize the pointer to the array of RVA-based ansi export strings.
	//

	NameTableBase = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNames);

	//
	// Initialize the pointer to the array of USHORT ordinal numbers.
	//

	NameOrdinalTableBase = (PUSHORT)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfNameOrdinals);

	AddressTableBase = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfFunctions);

	if (!ByName)
	{
		KdPrint(("Import by orgin\n"));
		return (PVOID)AddressTableBase[Ordinal];
	}

	//
	// Lookup the desired name in the name table using a binary search.
	//

	Low = 0;
	Middle = 0;
	High = ExportDirectory->NumberOfNames - 1;

	while (High >= Low) {

		//
		// Compute the next probe index and compare the import name
		// with the export name entry.
		//

		Middle = (Low + High) >> 1;

		Result = strcmp (RoutineName,
			(PCHAR)DllBase + NameTableBase[Middle]);

		if (Result < 0) {
			High = Middle - 1;
		}
		else if (Result > 0) {
			Low = Middle + 1;
		}
		else {
			break;
		}
	}
	//
	// If the high index is less than the low index, then a matching
	// table entry was not found. Otherwise, get the ordinal number
	// from the ordinal table.
	//

	if (High < Low) {
		return NULL;
	}

	OrdinalNumber = NameOrdinalTableBase[Middle];

	//
	// If the OrdinalNumber is not within the Export Address Table,
	// then this image does not implement the function.  Return not found.
	//

	if ((ULONG)OrdinalNumber >= ExportDirectory->NumberOfFunctions) {
		return NULL;
	}

	//
	// Index into the array of RVA export addresses by ordinal number.
	//

	Addr = (PULONG)((PCHAR)DllBase + (ULONG)ExportDirectory->AddressOfFunctions);

	FunctionAddress = (PVOID)((PCHAR)DllBase + Addr[OrdinalNumber]);

	//
	// Forwarders are not used by the kernel and HAL to each other.
	//

	// 	ASSERT ((FunctionAddress <= (PVOID)ExportDirectory) ||
	// 		(FunctionAddress >= (PVOID)((PCHAR)ExportDirectory + ExportSize)));

	return FunctionAddress;
}

BOOL FixBaseRelocTable(
	__in	PVOID	NewImageBase,
	__out	SIZE_T	ExistImageBase
	)
/*++

Routine Description:

    This routine relocates an image file that was not loaded into memory
    at the preferred address.

Arguments:

    NewBase - Supplies a pointer to the image base.

    AdditionalBias - An additional quantity to add to all fixups.  The
                     32-bit X86 loader uses this when loading 64-bit images
                     to specify a NewBase that is actually a 64-bit value.

    LoaderName - Indicates which loader routine is being called from.

    Success - Value to return if relocation successful.

    Conflict - Value to return if can't relocate.

    Invalid - Value to return if relocations are invalid.

Return Value:

    Success if image is relocated.
    Conflict if image can't be relocated.
    Invalid if image contains invalid fixups.

--*/

{
    LONGLONG Diff;
    ULONG TotalCountBytes = 0;
    ULONG_PTR VA;
    ULONGLONG OriginalImageBase;
    ULONG SizeOfBlock;
    PUCHAR FixupVA;
    USHORT Offset;
    PUSHORT NextOffset = NULL;
    PIMAGE_NT_HEADERS NtHeaders;
    PIMAGE_BASE_RELOCATION NextBlock;
	PIMAGE_DOS_HEADER ImageDosHeader = NULL;
	PIMAGE_NT_HEADERS ImageNtHeaders = NULL;

	ImageDosHeader = (PIMAGE_DOS_HEADER)NewImageBase;
	if (ImageDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}

	ImageNtHeaders = (PIMAGE_NT_HEADERS)((SIZE_T)NewImageBase + ImageDosHeader->e_lfanew);
	if (ImageNtHeaders->Signature != IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}

    NtHeaders = HgRtlImageNtHeader( NewImageBase );
    if (NtHeaders == NULL) 
	{
		return FALSE;
    }

    switch (NtHeaders->OptionalHeader.Magic) {
       
        case IMAGE_NT_OPTIONAL_HDR32_MAGIC:

            OriginalImageBase = ((PIMAGE_NT_HEADERS32)NtHeaders)->OptionalHeader.ImageBase;
            break;

        case IMAGE_NT_OPTIONAL_HDR64_MAGIC:

            OriginalImageBase = ((PIMAGE_NT_HEADERS64)NtHeaders)->OptionalHeader.ImageBase;
            break;

        default:
			return FALSE;
    }

    //
    // Locate the relocation section.
    //

    NextBlock = (PIMAGE_BASE_RELOCATION)HgRtlImageDirectoryEntryToData(
            NewImageBase, TRUE, IMAGE_DIRECTORY_ENTRY_BASERELOC, &TotalCountBytes);

    //
    // It is possible for a file to have no relocations, but the relocations
    // must not have been stripped.
    //

    if (!NextBlock || !TotalCountBytes) 
	{
    
        if (NtHeaders->FileHeader.Characteristics & IMAGE_FILE_RELOCS_STRIPPED) 
		{
            KdPrint(("Image can't be relocated, no fixup information.\n"));
			return FALSE;
        }
		else 
		{
            return TRUE;
        }
    }

    //
    // If the image has a relocation table, then apply the specified fixup
    // information to the image.
    //
    Diff = (ULONG_PTR)ExistImageBase - OriginalImageBase;
    while (TotalCountBytes)
	{
        SizeOfBlock = NextBlock->SizeOfBlock;
        TotalCountBytes -= SizeOfBlock;
        SizeOfBlock -= sizeof(IMAGE_BASE_RELOCATION);
        SizeOfBlock /= sizeof(USHORT);
        NextOffset = (PUSHORT)((PCHAR)NextBlock + sizeof(IMAGE_BASE_RELOCATION));

        VA = (ULONG_PTR)NewImageBase + NextBlock->VirtualAddress;

        if ( !(NextBlock = LdrProcessRelocationBlockLongLong( VA,
                                                              SizeOfBlock,
                                                              NextOffset,
                                                              Diff)) ) 
		{
            KdPrint(("%s: Unknown base relocation type\n"));
			return FALSE;
        }
    }

    return TRUE;
}

PIMAGE_BASE_RELOCATION LdrProcessRelocationBlockLongLong(
	__in	ULONG_PTR	VA,
	__in	ULONG		SizeOfBlock,
	__in	PUSHORT		NextOffset,
	__in	LONGLONG	Diff
	)
{
	PUCHAR FixupVA;
	USHORT Offset;
	LONG Temp;
	ULONG Temp32;
	ULONGLONG Value64;
	LONGLONG Temp64;

	while (SizeOfBlock--) {

		Offset = *NextOffset & (USHORT)0xfff;
		FixupVA = (PUCHAR)(VA + Offset);

		//
		// Apply the fixups.
		//

		switch ((*NextOffset) >> 12) {

		case IMAGE_REL_BASED_HIGHLOW :
			//
			// HighLow - (32-bits) relocate the high and low half
			//      of an address.
			//
			*(LONG UNALIGNED *)FixupVA += (ULONG) Diff;
			break;

		case IMAGE_REL_BASED_HIGH :
			//
			// High - (16-bits) relocate the high half of an address.
			//
			Temp = *(PUSHORT)FixupVA << 16;
			Temp += (ULONG) Diff;
			*(PUSHORT)FixupVA = (USHORT)(Temp >> 16);
			break;

		case IMAGE_REL_BASED_HIGHADJ :
			//
			// Adjust high - (16-bits) relocate the high half of an
			//      address and adjust for sign extension of low half.
			//

			//
			// If the address has already been relocated then don't
			// process it again now or information will be lost.
			//
			if (Offset & LDRP_RELOCATION_FINAL) {
				++NextOffset;
				--SizeOfBlock;
				break;
			}

			Temp = *(PUSHORT)FixupVA << 16;
			++NextOffset;
			--SizeOfBlock;
			Temp += (LONG)(*(PSHORT)NextOffset);
			Temp += (ULONG) Diff;
			Temp += 0x8000;
			*(PUSHORT)FixupVA = (USHORT)(Temp >> 16);

			break;

		case IMAGE_REL_BASED_LOW :
			//
			// Low - (16-bit) relocate the low half of an address.
			//
			Temp = *(PSHORT)FixupVA;
			Temp += (ULONG) Diff;
			*(PUSHORT)FixupVA = (USHORT)Temp;
			break;

		case IMAGE_REL_BASED_IA64_IMM64:

			//
			// Align it to bundle address before fixing up the
			// 64-bit immediate value of the movl instruction.
			//

			FixupVA = (PUCHAR)((ULONG_PTR)FixupVA & ~(15));
			Value64 = (ULONGLONG)0;

			//
			// Extract the lower 32 bits of IMM64 from bundle
			//


			EXT_IMM64(Value64,
				(PULONG)FixupVA + EMARCH_ENC_I17_IMM7B_INST_WORD_X,
				EMARCH_ENC_I17_IMM7B_SIZE_X,
				EMARCH_ENC_I17_IMM7B_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM7B_VAL_POS_X);
			EXT_IMM64(Value64,
				(PULONG)FixupVA + EMARCH_ENC_I17_IMM9D_INST_WORD_X,
				EMARCH_ENC_I17_IMM9D_SIZE_X,
				EMARCH_ENC_I17_IMM9D_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM9D_VAL_POS_X);
			EXT_IMM64(Value64,
				(PULONG)FixupVA + EMARCH_ENC_I17_IMM5C_INST_WORD_X,
				EMARCH_ENC_I17_IMM5C_SIZE_X,
				EMARCH_ENC_I17_IMM5C_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM5C_VAL_POS_X);
			EXT_IMM64(Value64,
				(PULONG)FixupVA + EMARCH_ENC_I17_IC_INST_WORD_X,
				EMARCH_ENC_I17_IC_SIZE_X,
				EMARCH_ENC_I17_IC_INST_WORD_POS_X,
				EMARCH_ENC_I17_IC_VAL_POS_X);
			EXT_IMM64(Value64,
				(PULONG)FixupVA + EMARCH_ENC_I17_IMM41a_INST_WORD_X,
				EMARCH_ENC_I17_IMM41a_SIZE_X,
				EMARCH_ENC_I17_IMM41a_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41a_VAL_POS_X);

			EXT_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM41b_INST_WORD_X),
				EMARCH_ENC_I17_IMM41b_SIZE_X,
				EMARCH_ENC_I17_IMM41b_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41b_VAL_POS_X);
			EXT_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM41c_INST_WORD_X),
				EMARCH_ENC_I17_IMM41c_SIZE_X,
				EMARCH_ENC_I17_IMM41c_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41c_VAL_POS_X);
			EXT_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_SIGN_INST_WORD_X),
				EMARCH_ENC_I17_SIGN_SIZE_X,
				EMARCH_ENC_I17_SIGN_INST_WORD_POS_X,
				EMARCH_ENC_I17_SIGN_VAL_POS_X);
			//
			// Update 64-bit address
			//

			Value64+=Diff;

			//
			// Insert IMM64 into bundle
			//

			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM7B_INST_WORD_X),
				EMARCH_ENC_I17_IMM7B_SIZE_X,
				EMARCH_ENC_I17_IMM7B_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM7B_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM9D_INST_WORD_X),
				EMARCH_ENC_I17_IMM9D_SIZE_X,
				EMARCH_ENC_I17_IMM9D_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM9D_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM5C_INST_WORD_X),
				EMARCH_ENC_I17_IMM5C_SIZE_X,
				EMARCH_ENC_I17_IMM5C_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM5C_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IC_INST_WORD_X),
				EMARCH_ENC_I17_IC_SIZE_X,
				EMARCH_ENC_I17_IC_INST_WORD_POS_X,
				EMARCH_ENC_I17_IC_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM41a_INST_WORD_X),
				EMARCH_ENC_I17_IMM41a_SIZE_X,
				EMARCH_ENC_I17_IMM41a_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41a_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM41b_INST_WORD_X),
				EMARCH_ENC_I17_IMM41b_SIZE_X,
				EMARCH_ENC_I17_IMM41b_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41b_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_IMM41c_INST_WORD_X),
				EMARCH_ENC_I17_IMM41c_SIZE_X,
				EMARCH_ENC_I17_IMM41c_INST_WORD_POS_X,
				EMARCH_ENC_I17_IMM41c_VAL_POS_X);
			INS_IMM64(Value64,
				((PULONG)FixupVA + EMARCH_ENC_I17_SIGN_INST_WORD_X),
				EMARCH_ENC_I17_SIGN_SIZE_X,
				EMARCH_ENC_I17_SIGN_INST_WORD_POS_X,
				EMARCH_ENC_I17_SIGN_VAL_POS_X);
			break;

		case IMAGE_REL_BASED_DIR64:

			*(ULONGLONG UNALIGNED *)FixupVA += Diff;

			break;

		case IMAGE_REL_BASED_MIPS_JMPADDR :
			//
			// JumpAddress - (32-bits) relocate a MIPS jump address.
			//
			Temp = (*(PULONG)FixupVA & 0x3ffffff) << 2;
			Temp += (ULONG) Diff;
			*(PULONG)FixupVA = (*(PULONG)FixupVA & ~0x3ffffff) |
				((Temp >> 2) & 0x3ffffff);

			break;

		case IMAGE_REL_BASED_ABSOLUTE :
			//
			// Absolute - no fixup required.
			//
			break;

		case IMAGE_REL_BASED_SECTION :
			//
			// Section Relative reloc.  Ignore for now.
			//
			break;

		case IMAGE_REL_BASED_REL32 :
			//
			// Relative intrasection. Ignore for now.
			//
			break;

		default :
			//
			// Illegal - illegal relocation type.
			//

			return (PIMAGE_BASE_RELOCATION)NULL;
		}
		++NextOffset;
	}
	return (PIMAGE_BASE_RELOCATION)NextOffset;
}

SIZE_T RelativeAddressToAbsoluteAddress(
	__in	SIZE_T	CurrentAddress,
	__in	SSIZE_T	Offset,
	__in	ULONG32	CurrentInstructionSize
	)
{
	SIZE_T DestinationAddress = 0;

	if (HgMmIsAddressValid((PVOID)CurrentAddress)){
		DestinationAddress = CurrentAddress + Offset + CurrentInstructionSize;
	}

	return DestinationAddress;
}

PVOID LookupKernelModuleByNameA(
	__in	PCHAR	pszModuleName,
	__in	PULONG32	nModuleSize
	)
{
	PLDR_DATA_TABLE_ENTRY DriverSection;
	PLDR_DATA_TABLE_ENTRY LdrEntry;
	ANSI_STRING aKernelModuleName;
	UNICODE_STRING uKernelModuleName;
	UNICODE_STRING uModuleName;
	PWCHAR Buffer;
	int Length = 0;
	int Index = 0;
	PDRIVER_OBJECT DriverObject = g_pHGDrvObj;

	if (DriverObject == NULL){
		return NULL;
	}

	__try{
		RtlInitAnsiString(&aKernelModuleName, pszModuleName);
		RtlAnsiStringToUnicodeString(&uKernelModuleName, &aKernelModuleName, TRUE);
		Buffer = (PWCHAR)ExAllocatePoolWithTag(NonPagedPool, SZ_PATH_LEN_WCHAR, HG_MEM_TAG);
		if (Buffer == NULL){
			RtlFreeUnicodeString(&uKernelModuleName);
			return NULL;
		}

		RtlZeroMemory(Buffer, SZ_PATH_LEN_WCHAR);
		DriverSection = (PLDR_DATA_TABLE_ENTRY)DriverObject->DriverSection;
		LdrEntry = (PLDR_DATA_TABLE_ENTRY)DriverSection->InLoadOrderLinks.Flink;

		while (LdrEntry && DriverSection != LdrEntry){
			if (LdrEntry->DllBase >= MmSystemRangeStart	&&
				LdrEntry->FullDllName.Length > 0		&&
				LdrEntry->FullDllName.Buffer != NULL)
			{
				if (MmIsAddressValid(&LdrEntry->FullDllName.Buffer[LdrEntry->FullDllName.Length / 2 -1]))
				{
					Length = LdrEntry->FullDllName.Length / 2;
					for (Index = Length - 1; Index > 0; Index--){
						if (LdrEntry->FullDllName.Buffer[Index] == 0x005C){
							break;
						}
					}
					if (LdrEntry->FullDllName.Buffer[Index] == 0x005c){
						RtlCopyMemory(Buffer, &(LdrEntry->FullDllName.Buffer[Index + 1]), (Length - Index - 1) * 2);
						uModuleName.Buffer = Buffer;
						uModuleName.Length = (Length - Index - 1) * 2;
						uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
					}else{
						RtlCopyMemory(Buffer, LdrEntry->FullDllName.Buffer, Length * 2);
						uModuleName.Buffer = Buffer;
						uModuleName.Length = Length * 2;
						uModuleName.MaximumLength = SZ_PATH_LEN_WCHAR;
					}

					if (RtlEqualUnicodeString(&uModuleName, &uKernelModuleName, TRUE)){
						ExFreePool(Buffer);
						RtlFreeUnicodeString(&uKernelModuleName);
						*nModuleSize = LdrEntry->SizeOfImage;
						return LdrEntry->DllBase;
					}
				}
			}
			LdrEntry = (PLDR_DATA_TABLE_ENTRY)LdrEntry->InLoadOrderLinks.Flink;
		}

		RtlFreeUnicodeString(&uKernelModuleName);
		ExFreePool(Buffer);
	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return NULL;
}