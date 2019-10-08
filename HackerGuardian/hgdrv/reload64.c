
#include "reload64.h"

BOOL ReloadKernel64(VOID)
{
	BOOL bRtn = FALSE;
	SIZE_T OriginalKernelBase = 0;
	SIZE_T OriginalKernelSize = 0;
	SIZE_T NewNtoskrnlBaseRepaired = 0;
	SIZE_T NewNtoskrnlBaseUnrepaired = 0;

	SIZE_T ReloadedKernelAddress = 0;

	PVOID OriginKiServiceTable = NULL;
	PVOID ReloadKiServiceTable = NULL;
	PKSERVICE_TABLE_DESCRIPTOR OriginSSDT = NULL;
	PKSERVICE_TABLE_DESCRIPTOR ReloadSSDT = NULL;

	SIZE_T Win32kBase = 0;
	ULONG32 Win32kSize = 0;

	WCHAR pwszNtoskrnlPath[SZ_PATH_LEN] = {0};

	if (GetNtoskrnlInfo(pwszNtoskrnlPath, &OriginalKernelBase, &OriginalKernelSize) == FALSE){
		Reload64Log(("[reload64.c].[ReloadKernel64]: Get ntoskrnl.exe info unsuccessfully!\n"));
		return FALSE;
	}

	g_hg_map[eHG_CurrentKernelBase] = OriginalKernelBase;
	g_hg_map[eHG_CurrentKernelSize] = OriginalKernelSize;

	Win32kBase = (SIZE_T)LookupKernelModuleByNameA("win32k.sys", &Win32kSize);
	g_hg_map[eHG_OriginWin32kBase] = Win32kBase;
	g_hg_map[eHG_Win32kSize] = Win32kSize;

	Reload64Log(("[reload64.c].[ReloadKernel64]: ntoskrnl.exe = path: %S, Base: 0x%p, Size: 0x%p\n", pwszNtoskrnlPath, OriginalKernelBase, OriginalKernelSize));

// 	if (ImageKernel64(pwszNtoskrnlPath, &ReloadedKernelAddress, OriginalKernelBase)){
// 		g_hg_map[eHG_ReloadedKernelBase64] = ReloadedKernelAddress;
// 
// 		return TRUE;
// 	}
	/*
	// 
	if (PeLoad_Unrepair(pwszNtoskrnlPath, &NewNtoskrnlBaseUnrepaired)	&& 
		PeLoad_Repair(pwszNtoskrnlPath, &NewNtoskrnlBaseRepaired, KernelModuleBase))
	{
	PELog(("NewImageBase_Unrepair = 0x%p\n", NewNtoskrnlBaseUnrepaired));
		PELog(("NewImageBase = 0x%p\n", NewNtoskrnlBaseRepaired));

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
				OriginSSDT->ServiceTableBase = KeServiceDescriptorTable.ServiceTableBase;

				g_hg_map[eHG_OriginKiServiceTable] = (SIZE_T)OriginKiServiceTable;
				g_hg_map[eHG_OriginSSDT] = (SIZE_T)OriginSSDT;

				if (FixOriginalKiServiceTable(	(PDWORD)OriginKiServiceTable,
					(PDWORD)ReloadKiServiceTable, 
					(DWORD)NewNtoskrnlBaseRepaired,
					KernelModuleBase))
				{
					ReloadSSDT->NumberOfServices = KeServiceDescriptorTable.NumberOfServices;
					ReloadSSDT->ParamTableBase = KeServiceDescriptorTable.ParamTableBase;
					ReloadSSDT->ServiceCounterTableBase = KeServiceDescriptorTable.ServiceCounterTableBase;
					ReloadSSDT->ServiceTableBase = KeServiceDescriptorTable.ServiceTableBase;

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
	}

	if (OriginKiServiceTable){
		ExFreePool((PVOID)OriginKiServiceTable);
	}

	if (OriginSSDT){
		ExFreePool((PVOID)OriginSSDT);
	}

	if (ReloadKiServiceTable){
		ExFreePool((PVOID)ReloadKiServiceTable);
	}

	if (ReloadSSDT){
		ExFreePool((PVOID)ReloadSSDT);
	}

	if (NewNtoskrnlBaseUnrepaired){
		ExFreePool((PVOID)NewNtoskrnlBaseUnrepaired);
	}
	*/
	return bRtn;
}

VOID LookupSsdtAddress64(
	PVOID	Base
	)
{
	// need to reload kernel
	// KeAddSystemServiceTable is not a export function
	PKSERVICE_TABLE_DESCRIPTOR pSsdt = NULL;
	PKSERVICE_TABLE_DESCRIPTOR pShadowSsdt = NULL;
	SIZE_T StartAddr = 0;
	int nInstructionSize = 0;
	LONG32 nOffset = 0;
	PCHAR pZwClose = NULL;
	PCHAR pKiServiceInternal = NULL;
	PCHAR pKiSystemServiceStart = NULL;
	size_t i = 0;
	//SIZE_T bReloadNtosSuccess = g_hg_map[eHG_ReloadNtosKrnlSuccess];
	//SIZE_T Base = bReloadNtosSuccess ? g_hg_map[eHG_ReloadedKernelBase64] : g_hg_map[eHG_OriginalKernelBase64];
	//SIZE_T Base = g_hg_map[eHG_OriginalKernelBase64];
	DISASM MyDisasm;
	int Error = 0;

	// Using MSR register can get SSDT base.
// 	{
// 		SIZE_T msrRet = __readmsr(0xC0000082);
// 		SdtLog(("msrRet = 0x%p\n", msrRet));
// 	}

	Reload64Log(("[reload64.c].[LookupSsdtAddress64]: OriginalBase64 = 0x%p, ReloadBase64 = 0x%p, Base = 0x%p\n", 
		g_hg_map[eHG_CurrentKernelBase],	g_hg_map[eHG_ReloadedKernelBase64], Base));

	// There's no KeAddSystemServiceTable in Win7 x64.
	// KiSystemServiceRepeat
	pZwClose = (PCHAR)HgMiFindExportedRoutineByName((PVOID)Base, "ZwClose");
	if (pZwClose == NULL){
		Reload64Log(("pZwClose == NULL\n"));
		return;
	}
	Reload64Log(("[reload64.c].[LookupSsdtAddress64]: pZwClose = 0x%p\n", pZwClose));
	//////////////////////////////////////////////////////////////////////////
	if (HgMmIsAddressValid((PVOID)pZwClose)){
		for (StartAddr = (SIZE_T)pZwClose; StartAddr < (SIZE_T)pZwClose + PAGE_SIZE; StartAddr += nInstructionSize)
		{
			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (UIntPtr)StartAddr;
//			Reload64Log(("MyDisasm.EIP = 0x%p\n", MyDisasm.EIP));

#if defined(_WIN64)
			/* =================== Define 64 bits architecture =================== */
			MyDisasm.Archi = 64;
#else
			MyDisasm.Archi = 0;
#endif
			nInstructionSize = Disasm(&MyDisasm);

			if (nInstructionSize == 0 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC3 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC2){
				return;
			}
			if (0x50 == *(PBYTE)(StartAddr - 6) && 0xB8 == *(PBYTE)(StartAddr - 5) && 0xE9 == *(PBYTE)StartAddr && 0x9066 == *(PUSHORT)(StartAddr + 5)){
				nOffset = *(PLONG32)(StartAddr + 1);
				Reload64Log(("Current Address = %p\n", StartAddr));
				pKiServiceInternal = (PCHAR)RelativeAddressToAbsoluteAddress(StartAddr, nOffset, nInstructionSize);
				if (HgMmIsAddressValid((PVOID)pKiServiceInternal)){
					Reload64Log(("[reload64.c].[LookupSsdtAddress64]: pKiServiceInternal = 0x%p\n", pKiServiceInternal));
				}
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (HgMmIsAddressValid((PVOID)pKiServiceInternal)){
		for (StartAddr = (SIZE_T)pKiServiceInternal; StartAddr < (SIZE_T)pKiServiceInternal + PAGE_SIZE; StartAddr += nInstructionSize)
		{
			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (UIntPtr)StartAddr;
//			Reload64Log(("MyDisasm.EIP = 0x%p\n", MyDisasm.EIP));

#if defined(_WIN64)
			/* =================== Define 64 bits architecture =================== */
			MyDisasm.Archi = 64;
#else
			MyDisasm.Archi = 0;
#endif
			nInstructionSize = Disasm(&MyDisasm);

			if (nInstructionSize == 0 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC3 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC2){
				return;
			}
			if (0x8B4C == *(PUINT16)(StartAddr - 14) && 0x894C == *(PUINT16)(StartAddr - 7) && 0x8D4C == *(PUINT16)StartAddr && 0xFF41 == *(PUINT16)(StartAddr + 7)){
				nOffset = *(PLONG32)(StartAddr + 3);
				pKiSystemServiceStart = (PCHAR)RelativeAddressToAbsoluteAddress(StartAddr, nOffset, nInstructionSize);
				if (HgMmIsAddressValid((PVOID)pKiSystemServiceStart)){
					Reload64Log(("[reload64.c].[LookupSsdtAddress64]: pKiSystemServiceStart = 0x%p\n", pKiSystemServiceStart));
				}
				break;
			}
		}
	}
	//////////////////////////////////////////////////////////////////////////
	if (HgMmIsAddressValid((PVOID)pKiSystemServiceStart)){
		for (StartAddr = (SIZE_T)pKiSystemServiceStart; StartAddr < (SIZE_T)pKiSystemServiceStart + PAGE_SIZE; StartAddr += nInstructionSize)
		{
			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (UIntPtr)StartAddr;
//			Reload64Log(("MyDisasm.EIP = 0x%p\n", MyDisasm.EIP));

#if defined(_WIN64)
			/* =================== Define 64 bits architecture =================== */
			MyDisasm.Archi = 64;
#else
			MyDisasm.Archi = 0;
#endif
			nInstructionSize = Disasm(&MyDisasm);

			if (nInstructionSize == 0 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC3 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC2){
				return;
			}
			if (0xE783 == *(PUINT16)(StartAddr - 8) && 0x25 == *(PBYTE)(StartAddr - 5) && 0x8D4C == *(PUINT16)StartAddr && 0x8D4C == *(PUINT16)(StartAddr + 7)){
				nOffset = *(PLONG32)(StartAddr + 3);
				pSsdt = (PKSERVICE_TABLE_DESCRIPTOR)RelativeAddressToAbsoluteAddress(StartAddr, nOffset, nInstructionSize);
				if (HgMmIsAddressValid((PVOID)pSsdt)){
					Reload64Log(("[reload64.c].[LookupSsdtAddress64]: pSsdt = 0x%p\n", pSsdt));
				}
				nOffset = *(PLONG32)(StartAddr + 10);
				pShadowSsdt = (PKSERVICE_TABLE_DESCRIPTOR)RelativeAddressToAbsoluteAddress(StartAddr + 7, nOffset, nInstructionSize);
				if (HgMmIsAddressValid((PVOID)pShadowSsdt)){
					Reload64Log(("[reload64.c].[LookupSsdtAddress64]: pShadowSsdt = 0x%p\n", pShadowSsdt));
				}
				break;
			}
		}
	}

	if ((SIZE_T)Base == g_hg_map[eHG_ReloadedKernelBase64]){
		g_hg_map[eHG_ReloadedSSDT64] = (SIZE_T)pSsdt;
		g_hg_map[eHG_ReloadedShadowSSDT64] = (SIZE_T)pShadowSsdt + 0x20;
	}else if ((SIZE_T)Base == g_hg_map[eHG_CurrentKernelBase]){
		g_hg_map[eHG_OriginalSSDT64] = (SIZE_T)pSsdt;
		g_hg_map[eHG_OriginalShadowSSDT64] = (SIZE_T)pShadowSsdt + 0x20;	// 0x20 is sizeof(KSERVICE_TABLE_DESCRIPTOR) that is aligned by 8 bytes.
	}
}

BOOL ImageKernel64(
	__in	PWCHAR	KernelPath,
	__inout	PSIZE_T	ImageAddress,
	__in	SIZE_T	OriginalBase
	)
{
	BOOL						bRtn		= FALSE;
	NTSTATUS					status		= STATUS_UNSUCCESSFUL;
	HANDLE						hFile		= NULL;
	LARGE_INTEGER				FileSize;
	SIZE_T						Length		= 0;
	PBYTE						FileBuffer	= NULL;
	PBYTE						ImageBase	= NULL;
	IO_STATUS_BLOCK				IoStatus;
	OBJECT_ATTRIBUTES			oa;
	UNICODE_STRING				uFile;
	FILE_STANDARD_INFORMATION	fsi;
	SIZE_T i = 0;
	SIZE_T nLen = 0;

	RtlInitUnicodeString(&uFile, KernelPath);
	InitializeObjectAttributes(&oa, &uFile, OBJ_CASE_INSENSITIVE, NULL, NULL);
	status = ZwOpenFile(&hFile,
						GENERIC_ALL,
						&oa,
						&IoStatus,
						FILE_SHARE_READ	| FILE_SHARE_WRITE,
						FILE_SYNCHRONOUS_IO_NONALERT);

	if (!NT_SUCCESS(status)){
		Reload64Log(("[reload64.c].[ImageKernel64]: Open file unsuccessfully!\n"));
	}
	status = ZwQueryInformationFile(hFile, 
									&IoStatus, 
									&fsi, 
									sizeof(FILE_STANDARD_INFORMATION), 
									FileStandardInformation);
	if (!NT_SUCCESS(status)){
		Reload64Log(("[reload64.c].[ImageKernel64]: Query file size unsuccessfully!\n"));
		ZwClose(hFile);
		return FALSE;
	}

	Length = fsi.EndOfFile.QuadPart;
	Reload64Log(("[reload64.c].[ImageKernel64]: File size: 0x%p.\n", Length));
	FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, Length, HG_MEM_TAG);
	if (FileBuffer == NULL){
		i = 1;
		do {
			nLen = Length + 2048 * i;
			FileBuffer = (PBYTE)ExAllocatePoolWithTag(PagedPool, nLen, 0);
		} while (FileBuffer == NULL && ++i < 10);
	}

	if (FileBuffer == NULL){
		ZwClose(hFile);
		return FALSE;
	}
	Reload64Log(("[reload64.c].[ImageKernel64]: FileBuffer: 0x%p.\n", FileBuffer));
	status = ZwReadFile(hFile, 
						NULL,
						NULL,
						NULL,
						&IoStatus,
						FileBuffer,
						(ULONG)Length,
						NULL,
						NULL);

	if (NT_SUCCESS(status)){
		Reload64Log(("[reload64.c].[ImageKernel64]: Read file successfully!\n"));
	}

	if (hFile != NULL){
		ZwClose(hFile);
	}

	if (ImagePEFile(FileBuffer, (PVOID*)&ImageBase) == FALSE){
		Reload64Log(("[reload64.c].[ImageKernel64]: Image file unsuccessfully!\n"));
		ExFreePool(FileBuffer);
		return bRtn;
	}

	Reload64Log(("[reload64.c].[ImageKernel64]: Image base address: 0x%p\n", ImageBase));
	ExFreePool(FileBuffer);

	if (FixImportTable(ImageBase, OriginalBase) == FALSE){
		Reload64Log(("Fix import table unsuccessfully!\n"));
		ExFreePool(ImageBase);
		return bRtn;
	}

	if (FixBaseRelocTable(ImageBase, OriginalBase) == FALSE){
		PELog(("Fix base relocation table failed!\n"));
		ExFreePool(ImageBase);
		return bRtn;
	}
	*ImageAddress = (SIZE_T)ImageBase;

	return TRUE;
}