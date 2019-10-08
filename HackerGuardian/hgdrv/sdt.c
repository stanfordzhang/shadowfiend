
#include "hgheader.h"

#if defined(_WIN64)
#else
#include "reload32.h"
#endif

//
// private method
// 
#if defined(_WIN64)
SIZE_T CalculateFunctionAddressFromSDT(SIZE_T sdt, SIZE_T Ordinal)
{
	PKSERVICE_TABLE_DESCRIPTOR pSdt = (PKSERVICE_TABLE_DESCRIPTOR)sdt;
	PULONG32 ServiceTableBase = (PULONG32)(pSdt->ServiceTableBase);
	SIZE_T Mask = 0xFFFFFFFFEFFFFFFF;
	// 	SdtLog(("--------------%d, 0x%p, 0x%08X, 0x%p\n", Ordinal,
// 		ServiceTableBase, ((SIZE_T)(ServiceTableBase[Ordinal & 0xFFF]) >> 4),
// 		(PVOID)(((SIZE_T)(pSdt->ServiceTableBase) + ((SIZE_T)(ServiceTableBase[Ordinal & 0xFFF]) >> 4) & Mask))));
	if (!pSdt || !HgMmIsAddressValid((PVOID)pSdt) || 
		!ServiceTableBase || !HgMmIsAddressValid((PVOID)ServiceTableBase) || 
//		!ServiceTableBase[Ordinal & 0xFFF] || 
//		!HgMmIsAddressValid((PVOID)(((SIZE_T)(pSdt->ServiceTableBase) + ((SIZE_T)(ServiceTableBase[Ordinal & 0xFFF]) >> 4) & Mask))) ||
		Ordinal > pSdt->NumberOfServices)
	{
		return 0;
	}
	return (SIZE_T)(((SIZE_T)(pSdt->ServiceTableBase) + ((SIZE_T)(ServiceTableBase[Ordinal & 0xFFF]) >> 4) & Mask));
}

PVOID ImageNtdll64()
{
	PVOID ntdllBase = NULL;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	HANDLE hFile;
	FILE_STANDARD_INFORMATION fsi;
	SIZE_T Length = 0;
	PBYTE FileBuffer = NULL;
	PBYTE ImageBase = NULL;
	IO_STATUS_BLOCK IoStatus;
	SIZE_T i = 0;
	SIZE_T nLen = 0;

	OBJECT_ATTRIBUTES oa;
	UNICODE_STRING uNtdll;
	WCHAR pwszNtdll[] = L"\\SystemRoot\\System32\\ntdll.dll";
	RtlInitUnicodeString(&uNtdll, pwszNtdll);
	InitializeObjectAttributes(&oa, &uNtdll, OBJ_CASE_INSENSITIVE, NULL, NULL);

	status = ZwOpenFile(&hFile,
						SYNCHRONIZE | FILE_EXECUTE,
						&oa,
						&IoStatus,
						FILE_SHARE_READ,
						FILE_SYNCHRONOUS_IO_NONALERT);
	if (!NT_SUCCESS(status))
	{
		SdtLog(("0x%p, Open ntdll.dll unsuccessfully\n", status));
		return NULL;
	}

	status = ZwQueryInformationFile(hFile,
									&IoStatus,
									&fsi,
									sizeof(FILE_STANDARD_INFORMATION),
									FileStandardInformation);
	if (!NT_SUCCESS(status))
	{
		SdtLog(("Get ntdll size unsuccessfully!\n"));
		ZwClose(hFile);
		return NULL;
	}

	Length = fsi.EndOfFile.QuadPart;
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
		return NULL;
	}

	SdtLog(("Open file successfully!\n"));
	status = ZwReadFile(hFile, 
						NULL,
						NULL,
						NULL,
						&IoStatus,
						FileBuffer,
						(ULONG)Length,
						NULL,
						NULL);
	if (!NT_SUCCESS(status)){
		SdtLog(("Read file unsuccessfully!\n"));
		HgExFreePoolWithTag(FileBuffer, 0);
		ZwClose(hFile);
		return NULL;
	}
	ZwClose(hFile);

	if (!ImagePEFile(FileBuffer, &ntdllBase)){
		HgExFreePoolWithTag(FileBuffer, 0);
		return NULL;
	}

	HgExFreePoolWithTag(FileBuffer, 0);

	return ntdllBase;
}

BOOL DetermineKernelRange64(SIZE_T Address)
{
	BOOL bRtn = FALSE;

	SIZE_T KernelBase = g_hg_map[eHG_CurrentKernelBase];
	SIZE_T KernelSize = g_hg_map[eHG_CurrentKernelSize];
	SIZE_T Win32kBase = g_hg_map[eHG_OriginWin32kBase];
	SIZE_T Win32kSize = g_hg_map[eHG_Win32kSize];

	if (!Address)
	{
		return FALSE;
	}

	if (((SIZE_T)Address >= KernelBase) && ((SIZE_T)Address <= KernelBase + KernelSize) ||
		((SIZE_T)Address >= Win32kBase) && ((SIZE_T)Address <= Win32kBase + Win32kSize))
	{
		return TRUE;
	}

	return bRtn;
}

PVOID CheckNtoskrnlWin32kInlineHook64(PVOID Address, OUT PSIZE_T pOffsetOfHook)
{
	PVOID RtnAddress = NULL;

	DISASM MyDisasm;
	int InstLen = 0;
	int Error = 0;
	UIntPtr eip = 0;

	if (Address == NULL || !HgMmIsAddressValid((PVOID)Address)){
		return NULL;
	}

	for (eip = (UIntPtr)Address; (eip < ((SIZE_T)Address & (~0x007)) + PAGE_SIZE) && (!Error) && (!RtnAddress); eip += InstLen)
	{
		if (*(PBYTE)eip == 0x90 || *(PBYTE)eip == 0xcc)
		{
			goto HG_EXIT;
		}
		RtnAddress = NULL;
		(void) memset (&MyDisasm, 0, sizeof(DISASM));
		MyDisasm.EIP = eip;
#if defined(_WIN64)
		// =================== Define 64 bits architecture ================
		MyDisasm.Archi = 64;
#else
		MyDisasm.Archi = 0;
#endif
		InstLen = Disasm(&MyDisasm);
		if (InstLen != UNKNOWN_OPCODE)
		{
			//SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
			//SdtLog(("%s, %d, %04x\n", MyDisasm.Instruction.Mnemonic, MyDisasm.Instruction.BranchType, LOWORD (MyDisasm.Instruction.Category)));
			if (_strnicmp(MyDisasm.Instruction.Mnemonic, "jmp", 3) == 0 && MyDisasm.Instruction.BranchType == JmpType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				// eb 1d	jmp xxx	// near jmp
				// e9 xxx	jmp xxx	// far jmp
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (PVOID)(MyDisasm.Instruction.AddrValue);
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
				// jmp dword ptr [xxx]
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (MEMORY_TYPE | RELATIVE_)	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.AddrValue))
				{
					RtnAddress = (PVOID)*(PSIZE_T)MyDisasm.Instruction.AddrValue;
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "call", 4) == 0 && MyDisasm.Instruction.BranchType == CallType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				// FF15 12510026	CALL NEAR [26005112]
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (MEMORY_TYPE | RELATIVE_)	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.AddrValue))
				{
					RtnAddress = (PVOID)*(PSIZE_T)MyDisasm.Instruction.AddrValue;
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
				// E8 0502FCFF     call XXX
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (PVOID)(MyDisasm.Instruction.AddrValue);
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
			}
			else if (MyDisasm.Instruction.BranchType == RetType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				goto HG_EXIT;
			}
			// mov XXX
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "mov", 3) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					//SdtLog(("Enter MOV: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
					RtnAddress = (PVOID)(MyDisasm.Instruction.Immediat);
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}

					if (RtnAddress)
					{
						DISASM disasm2;
						int InstLen2 = 0;
						RtlZeroMemory(&disasm2, sizeof(DISASM));
						disasm2.EIP = MyDisasm.EIP + InstLen;
#if defined(_WIN64)
						// =================== Define 64 bits architecture =================== 
						disasm2.Archi = 64;
#else
						disasm2.Archi = 0;
#endif
						InstLen2 = Disasm(&disasm2);
						//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
						if ((LOWORD(MyDisasm.Argument1.ArgType) == disasm2.Argument1.Memory.BaseRegister) && (disasm2.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm2.Instruction.BranchType == CallType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER){	

						}else if ((LOWORD(MyDisasm.Argument1.ArgType) == disasm2.Argument1.Memory.BaseRegister) && (disasm2.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm2.Instruction.BranchType == JmpType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER){	

						}else{
							RtnAddress = 0;
						}
					}
				}
			}
			// push xxx
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "push", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				// push XXX
				// ret
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					//SdtLog(("Enter PUSH: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
					RtnAddress = (PVOID)(MyDisasm.Instruction.Immediat);
					if (DetermineKernelRange64((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}

					if (RtnAddress)
					{
						DISASM disasm2;
						int InstLen2 = 0;
						RtlZeroMemory(&disasm2, sizeof(DISASM));
						disasm2.EIP = MyDisasm.EIP + InstLen;
#if defined(_WIN64)
						// =================== Define 64 bits architecture =================== 
						disasm2.Archi = 64;
#else
						disasm2.Archi = 0;
#endif
						InstLen2 = Disasm(&disasm2);
						//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
						if (disasm2.Instruction.BranchType == RetType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER)
						{							
						}
						else if (_strnicmp(disasm2.Instruction.Mnemonic, "pop", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
						{
							DISASM disasm3;
							int InstLen3 = 0;
							RtlZeroMemory(&disasm3, sizeof(DISASM));
							disasm3.EIP = disasm2.EIP + InstLen2;
#if defined(_WIN64)
							// =================== Define 64 bits architecture =================== 
							disasm3.Archi = 64;
#else
							disasm3.Archi = 0;
#endif
							InstLen3 = Disasm(&disasm3);

							if ((LOWORD(disasm2.Argument1.ArgType) == disasm3.Argument1.Memory.BaseRegister) && (disasm3.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm3.Instruction.BranchType == CallType && LOWORD (disasm3.Instruction.Category) == CONTROL_TRANSFER){	

							}else if ((LOWORD(disasm2.Argument1.ArgType) == disasm3.Argument1.Memory.BaseRegister) && (disasm3.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm3.Instruction.BranchType == JmpType && LOWORD (disasm3.Instruction.Category) == CONTROL_TRANSFER){	

							}else{
								RtnAddress = 0;
							}
						}else{
							RtnAddress = 0;
						}
					}
				}
			}
		}else{
			Error = 1;
		}

		if (RtnAddress)
		{
			*pOffsetOfHook = (SIZE_T)eip - (SIZE_T)Address;
		}
	}

HG_EXIT:
	// 	if (RtnAddress)
	// 	{
	// 		SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
	// 		SdtLog(("RtnAddress = 0x%p\n", RtnAddress));
	// 	}
	// 	
	return RtnAddress;
}

SIZE_T RetrieveInlineHookAddress64(SIZE_T Address, SIZE_T nLen, OUT PSIZE_T pOffset)
{
	SIZE_T RtnAddress = 0;
	DISASM MyDisasm;
	int InstLen = 0;
	int Error = 0;
	SIZE_T nOffset = 0;
	SIZE_T pMove = 0;

	if (Address == 0 || !HgMmIsAddressValid((PVOID)Address) || nLen == 0){
		return 0;
	}

	do {
		RtnAddress = 0;
		(void) memset (&MyDisasm, 0, sizeof(DISASM));
		MyDisasm.EIP = (UIntPtr)Address + nOffset;
		MyDisasm.Archi = 64;
		InstLen = Disasm(&MyDisasm);
		if (InstLen != UNKNOWN_OPCODE)
		{
			//SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
			//SdtLog(("%s, %d, %04x\n", MyDisasm.Instruction.Mnemonic, MyDisasm.Instruction.BranchType, LOWORD (MyDisasm.Instruction.Category)));
			if (_strnicmp(MyDisasm.Instruction.Mnemonic, "jmp", 3) == 0 && MyDisasm.Instruction.BranchType == JmpType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				//SdtLog(("Enter JMP\n"));
				//SdtLog(("(MyDisasm.Argument1.ArgType & 0xFFFF0000) = 0x%p, InstLen = %d, MyDisasm.Instruction.AddrValue = 0x%p, MyDisasm.EIP + InstLen + MyDisasm.Instruction.AddrValue = 0x%p\n", (MyDisasm.Argument1.ArgType & 0xFFFF0000), InstLen, MyDisasm.Instruction.AddrValue, (MyDisasm.EIP + InstLen + MyDisasm.Instruction.AddrValue)));
				// jmp eax
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG)	&&
					(InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)pMove))
				{
					//SdtLog(("Enter JMP 1\n"));
					RtnAddress = pMove;
				}
				// jmp dword ptr [xxx]
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == MEMORY_TYPE	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Argument1.Memory.Displacement))
				{
					//SdtLog(("Enter JMP 2\n"));
					RtnAddress = (SIZE_T)MyDisasm.Argument1.Memory.Displacement;
					SdtLog(("RtnAddress = 0x%p\n", RtnAddress));
				}//
				// eb 1d	jmp xxx	// near jmp
				// e9 xxx	jmp xxx	// far jmp
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(MyDisasm.Instruction.AddrValue)))
				{
					//SdtLog(("Enter JMP 3\n"));
					RtnAddress = (SIZE_T)(MyDisasm.Instruction.AddrValue);
				}
				// mov eax, XXX
				// jmp dword ptr [eax]
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "mov", 3) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				//SdtLog(("Enter MOV: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					pMove = (SIZE_T)MyDisasm.Instruction.Immediat;
				}
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "call", 4) == 0 && MyDisasm.Instruction.BranchType == CallType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				//SdtLog(("Enter CALL\n"));
				// FF15 12510026	CALL NEAR [26005112]
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == MEMORY_TYPE	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Argument1.Memory.Displacement))
				{
					RtnAddress = (SIZE_T)MyDisasm.Argument1.Memory.Displacement;
				}
				// E8 0502FCFF     call XXX
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5)	&&
					HgMmIsAddressValid((PVOID)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (SIZE_T)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue);
				}
				//			mov eax, offset SbieDrv+0x1374e (b0b7e74e)
				// ffd0		call eax 
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG)	&&
					(InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)pMove))
				{
					RtnAddress = pMove;
				}
				// mov eax, XXX
				// call dword ptr [eax]
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "push", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				//SdtLog(("Enter PUSH\n"));
				// push XXX
				// ret
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					DISASM disasm2;
					int InstLen2 = 0;
					RtlZeroMemory(&disasm2, sizeof(DISASM));
					disasm2.EIP = MyDisasm.EIP + InstLen;
					disasm2.Archi = 64;
					InstLen2 = Disasm(&disasm2);
					//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
					if (disasm2.Instruction.BranchType == RetType)
					{
						RtnAddress = (SIZE_T)MyDisasm.Instruction.Immediat;
					}
				}
			}

			MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)InstLen;
		}else{
			Error = 1;
		}
		*pOffset = nOffset;
		nOffset += InstLen;
	} while ((!RtnAddress) && (!Error) && (nOffset < nLen));

	if (DetermineKernelRange64((SIZE_T)RtnAddress))
	{
		RtnAddress = 0;
	}

	return RtnAddress;
}

SIZE_T CheckSdtInlineHook64(SIZE_T Ordinal, OUT PSIZE_T pOffsetOfHook)
{
	SIZE_T Address = 0;

	Address = RetrieveInlineHookAddress64(CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalSSDT64], Ordinal), 0x10, pOffsetOfHook);

	if (Address == 0 || Address < (SIZE_T)MM_SYSTEM_RANGE_START || !HgMmIsAddressValid((PVOID)Address))
	{
		Address = 0;
	}

	if (Address == 0)
	{
		Address = (SIZE_T)CheckNtoskrnlWin32kInlineHook64((PVOID)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalSSDT64], Ordinal), pOffsetOfHook);
		if (Address == 0 || Address < (SIZE_T)MM_SYSTEM_RANGE_START || !HgMmIsAddressValid((PVOID)Address))
		{
			Address = 0;
		}
	}

	return Address;
}

SIZE_T CheckShadowInlineHook64(SIZE_T Ordinal, OUT PSIZE_T pOffsetOfHook)
{
	SIZE_T Address = 0;

	PKSERVICE_TABLE_DESCRIPTOR pCurrentShadowSdt = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginalShadowSSDT64];
	SIZE_T nShadowSdtCount = pCurrentShadowSdt->NumberOfServices;

	if (pCurrentShadowSdt && HgMmIsAddressValid((PVOID)pCurrentShadowSdt) && HgMmIsAddressValid((PVOID)pCurrentShadowSdt))
	{		
		Address = RetrieveInlineHookAddress64(CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalShadowSSDT64], Ordinal), 0x10, pOffsetOfHook);
		if (Address == 0 || Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
			!HgMmIsAddressValid((PVOID)Address))
		{
			Address = 0;
		}

		if (Address == 0)
		{
			Address = (SIZE_T)CheckNtoskrnlWin32kInlineHook64((PVOID)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalShadowSSDT64], Ordinal), pOffsetOfHook);
			if (Address == 0	||
				Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
				!HgMmIsAddressValid((PVOID)Address))
			{
				Address = 0;
			}
		}
	}

	return Address;
}
#else
PVOID ImageNtdll32()
{
	PVOID ntdllBase = NULL;

	HANDLE hNtdll = NULL;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG Length = 0;
	LARGE_INTEGER FileSize;
	PVOID FileBuffer = NULL;
	IO_STATUS_BLOCK IoStatus;
	WCHAR pwszNtdll[] = L"\\SystemRoot\\System32\\ntdll.dll";

	status = KernelOpenFile(pwszNtdll,
							&hNtdll,
							SYNCHRONIZE | FILE_EXECUTE,
							FILE_ATTRIBUTE_NORMAL,
							FILE_SHARE_READ,
							FILE_OPEN,
							FILE_SYNCHRONOUS_IO_NONALERT);
	if (!NT_SUCCESS(status)){
		return NULL;
	}

	status = KernelGetFileSize(hNtdll, &FileSize);
	if (!NT_SUCCESS(status)){
		HgNtClose(hNtdll);
		return NULL;
	}

	Length = FileSize.LowPart;
	FileBuffer = HgExAllocatePoolWithTag(PagedPool, Length, HG_MEM_TAG);
	if (!FileBuffer){
		ULONG i = 1;
		do {
			ULONG nLen = Length + 2048 * i;
			FileBuffer = HgExAllocatePoolWithTag(PagedPool, nLen, HG_MEM_TAG);
		} while (FileBuffer == NULL && ++i < 10);
	}

	if (FileBuffer == NULL){
		HgNtClose(hNtdll);
		return NULL;
	}

	status = KernelReadFile(hNtdll, NULL, Length, FileBuffer, &IoStatus);
	if (!NT_SUCCESS(status)){
		HgNtClose(hNtdll);
		HgExFreePoolWithTag(FileBuffer, 0);
		return NULL;
	}

	HgNtClose(hNtdll);

	if (!ImagePEFile(FileBuffer, &ntdllBase)){
		HgExFreePoolWithTag(FileBuffer, 0);
		return NULL;
	}

	HgExFreePoolWithTag(FileBuffer, 0);

	return ntdllBase;
}

BOOL DetermineKernelRange32(SIZE_T Address)
{
	BOOL bRtn = FALSE;

	ULONG KernelBase = g_hg_map[eHG_CurrentKernelBase];
	ULONG KernelSize = g_hg_map[eHG_CurrentKernelSize];
	ULONG Win32kBase = g_hg_map[eHG_OriginWin32kBase];
	ULONG Win32kSize = g_hg_map[eHG_Win32kSize];
	ULONG HalBase = g_hg_map[eHG_OriginHalDllBase];
	ULONG HalSize = g_hg_map[eHG_HalDllSize];

	if (!Address)
	{
		return FALSE;
	}

	if (((SIZE_T)Address >= KernelBase) && ((SIZE_T)Address <= KernelBase + KernelSize) ||
		((SIZE_T)Address >= Win32kBase) && ((SIZE_T)Address <= Win32kBase + Win32kSize) ||
		((SIZE_T)Address >= HalBase) && ((SIZE_T)Address <= HalBase + HalSize))
	{
		return TRUE;
	}

	return bRtn;
}

PVOID CheckNtoskrnlWin32kInlineHook32(PVOID Address, OUT PSIZE_T pOffsetOfHook)
{
	PVOID RtnAddress = NULL;

	DISASM MyDisasm;
	int InstLen = 0;
	int Error = 0;
	UIntPtr eip = 0;

	if (Address == NULL || !HgMmIsAddressValid(Address)){
		return NULL;
	}

	for (eip = (UIntPtr)Address; (eip < ((SIZE_T)Address & (~0x007)) + PAGE_SIZE) && (!Error) && (!RtnAddress); eip += InstLen)
	{
		if (*(PBYTE)eip == 0x90 || *(PBYTE)eip == 0xcc)
		{
			goto HG_EXIT;
		}
		RtnAddress = NULL;
		(void) memset (&MyDisasm, 0, sizeof(DISASM));
		MyDisasm.EIP = eip;
#if defined(_WIN64)
		// =================== Define 64 bits architecture ================
		MyDisasm.Archi = 64;
#else
		MyDisasm.Archi = 0;
#endif
		InstLen = Disasm(&MyDisasm);
		if (InstLen != UNKNOWN_OPCODE)
		{
// 			SIZE_T AAAA = 0x83ff9A3D;
// 			if (MyDisasm.EIP > AAAA + 0xA8 && MyDisasm.EIP < AAAA + 0xF0)
// 			{
// 				int aa = 0;
// 				SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
// 
// 				for (aa = 0; aa < InstLen; aa++)
// 				{
// 					SdtLog(("%02X", *(PBYTE)(MyDisasm.EIP + aa)));
// 				}
// 				SdtLog(("\n"));
// 			}
			//SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
			//SdtLog(("%s, %d, %04x\n", MyDisasm.Instruction.Mnemonic, MyDisasm.Instruction.BranchType, LOWORD (MyDisasm.Instruction.Category)));
			if (_strnicmp(MyDisasm.Instruction.Mnemonic, "jmp", 3) == 0 && MyDisasm.Instruction.BranchType == JmpType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				// eb 1d	jmp xxx	// near jmp
				// e9 xxx	jmp xxx	// far jmp
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (PVOID)(MyDisasm.Instruction.AddrValue);
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
				// jmp dword ptr [xxx]
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (MEMORY_TYPE | RELATIVE_)	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.AddrValue))
				{
					RtnAddress = (PVOID)*(PSIZE_T)MyDisasm.Instruction.AddrValue;
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "call", 4) == 0 && MyDisasm.Instruction.BranchType == CallType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				// FF15 12510026	CALL NEAR [26005112]
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (MEMORY_TYPE | RELATIVE_)	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.AddrValue))
				{
					RtnAddress = (PVOID)*(PSIZE_T)MyDisasm.Instruction.AddrValue;
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
				// E8 0502FCFF     call XXX
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (PVOID)(MyDisasm.Instruction.AddrValue);
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}
				}
			}
			else if (MyDisasm.Instruction.BranchType == RetType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				goto HG_EXIT;
			}
			// mov XXX
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "mov", 3) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				if (MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					//SdtLog(("Enter MOV: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
					RtnAddress = (PVOID)(MyDisasm.Instruction.Immediat);
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}

					if (RtnAddress)
					{
						DISASM disasm2;
						int InstLen2 = 0;
						RtlZeroMemory(&disasm2, sizeof(DISASM));
						disasm2.EIP = MyDisasm.EIP + InstLen;
#if defined(_WIN64)
						// =================== Define 64 bits architecture =================== 
						disasm2.Archi = 64;
#else
						disasm2.Archi = 0;
#endif
						InstLen2 = Disasm(&disasm2);
						//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
						if ((LOWORD(MyDisasm.Argument1.ArgType) == disasm2.Argument1.Memory.BaseRegister) && (disasm2.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm2.Instruction.BranchType == CallType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER){	

						}else if ((LOWORD(MyDisasm.Argument1.ArgType) == disasm2.Argument1.Memory.BaseRegister) && (disasm2.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm2.Instruction.BranchType == JmpType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER){	

						}else{
							RtnAddress = 0;
						}
					}
				}
			}
			// push xxx
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "push", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				// push XXX
				// ret
				if (MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					//SdtLog(("Enter PUSH: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
					RtnAddress = (PVOID)(MyDisasm.Instruction.Immediat);
					if (DetermineKernelRange32((SIZE_T)RtnAddress))
					{
						RtnAddress = NULL;
					}
					if (RtnAddress < MM_SYSTEM_RANGE_START || !HgMmIsAddressValid(RtnAddress))
					{
						RtnAddress = NULL;
					}

					if (RtnAddress)
					{
						DISASM disasm2;
						int InstLen2 = 0;
						RtlZeroMemory(&disasm2, sizeof(DISASM));
						disasm2.EIP = MyDisasm.EIP + InstLen;
#if defined(_WIN64)
						// =================== Define 64 bits architecture =================== 
						disasm2.Archi = 64;
#else
						disasm2.Archi = 0;
#endif
						InstLen2 = Disasm(&disasm2);
						//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
						if (disasm2.Instruction.BranchType == RetType && LOWORD (disasm2.Instruction.Category) == CONTROL_TRANSFER)
						{							
						}
						else if (_strnicmp(disasm2.Instruction.Mnemonic, "pop", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
						{
							DISASM disasm3;
							int InstLen3 = 0;
							RtlZeroMemory(&disasm3, sizeof(DISASM));
							disasm3.EIP = disasm2.EIP + InstLen2;
#if defined(_WIN64)
							// =================== Define 64 bits architecture =================== 
							disasm3.Archi = 64;
#else
							disasm3.Archi = 0;
#endif
							InstLen3 = Disasm(&disasm3);

							if ((LOWORD(disasm2.Argument1.ArgType) == disasm3.Argument1.Memory.BaseRegister) && (disasm3.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm3.Instruction.BranchType == CallType && LOWORD (disasm3.Instruction.Category) == CONTROL_TRANSFER){	

							}else if ((LOWORD(disasm2.Argument1.ArgType) == disasm3.Argument1.Memory.BaseRegister) && (disasm3.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG) && disasm3.Instruction.BranchType == JmpType && LOWORD (disasm3.Instruction.Category) == CONTROL_TRANSFER){	

							}else{
								RtnAddress = 0;
							}
						}else{
							RtnAddress = 0;
						}
					}
				}
			}
		}else{
			Error = 1;
		}

		if (RtnAddress)
		{
			*pOffsetOfHook = (SIZE_T)eip - (SIZE_T)Address;
		}
	}

HG_EXIT:
// 	if (RtnAddress)
// 	{
// 		SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
// 		SdtLog(("RtnAddress = 0x%p\n", RtnAddress));
// 	}
// 	
	return RtnAddress;
}

SIZE_T RetrieveInlineHookAddress32(SIZE_T Address, SIZE_T nLen, OUT PSIZE_T pOffset)
{
	SIZE_T RtnAddress = 0;
	DISASM MyDisasm;
	int InstLen = 0;
	int Error = 0;
	SIZE_T nOffset = 0;
	SIZE_T pMove = 0;

	if (Address == 0 || !HgMmIsAddressValid((PVOID)Address) || nLen == 0){
		return 0;
	}

	do {
		RtnAddress = 0;
		(void) memset (&MyDisasm, 0, sizeof(DISASM));
		MyDisasm.EIP = (UIntPtr)Address + nOffset;
#if defined(_WIN64)
		// =================== Define 64 bits architecture ================
		MyDisasm.Archi = 64;
#else
		MyDisasm.Archi = 0;
#endif
		InstLen = Disasm(&MyDisasm);
		if (InstLen != UNKNOWN_OPCODE)
		{
			//SdtLog(("0x%p: MyDisasm.CompleteInstr = %s\n", MyDisasm.EIP, MyDisasm.CompleteInstr));
			//SdtLog(("%s, %d, %04x\n", MyDisasm.Instruction.Mnemonic, MyDisasm.Instruction.BranchType, LOWORD (MyDisasm.Instruction.Category)));
			if (_strnicmp(MyDisasm.Instruction.Mnemonic, "jmp", 3) == 0 && MyDisasm.Instruction.BranchType == JmpType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				//SdtLog(("Enter JMP\n"));
				//SdtLog(("(MyDisasm.Argument1.ArgType & 0xFFFF0000) = 0x%p, InstLen = %d, MyDisasm.Instruction.AddrValue = 0x%p, MyDisasm.EIP + InstLen + MyDisasm.Instruction.AddrValue = 0x%p\n", (MyDisasm.Argument1.ArgType & 0xFFFF0000), InstLen, MyDisasm.Instruction.AddrValue, (MyDisasm.EIP + InstLen + MyDisasm.Instruction.AddrValue)));
				// jmp eax
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG)	&&
					(InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)pMove))
				{
					//SdtLog(("Enter JMP 1\n"));
					RtnAddress = pMove;
				}
				// jmp dword ptr [xxx]
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == MEMORY_TYPE	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Argument1.Memory.Displacement))
				{
					//SdtLog(("Enter JMP 2\n"));
					RtnAddress = (SIZE_T)MyDisasm.Argument1.Memory.Displacement;
					SdtLog(("RtnAddress = 0x%p\n", RtnAddress));
				}//
				// eb 1d	jmp xxx	// near jmp
				// e9 xxx	jmp xxx	// far jmp
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5 || InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue)))
				{
					//SdtLog(("Enter JMP 3\n"));
					RtnAddress = (SIZE_T)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue);
				}
				// mov eax, XXX
				// jmp dword ptr [eax]
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "mov", 3) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				//SdtLog(("Enter MOV: Immediate = 0x%p\n", MyDisasm.Instruction.Immediat));
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					pMove = (SIZE_T)MyDisasm.Instruction.Immediat;
// 					if (nOffset <= 5){
// 						RtnAddress = (SIZE_T)MyDisasm.Instruction.Immediat;
// 					}
					//SdtLog(("MOV: pMove = 0x%p\n", pMove));
				}
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "call", 4) == 0 && MyDisasm.Instruction.BranchType == CallType && LOWORD (MyDisasm.Instruction.Category) == CONTROL_TRANSFER)
			{
				//SdtLog(("Enter CALL\n"));
				// FF15 12510026	CALL NEAR [26005112]
				if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == MEMORY_TYPE	&&
					(InstLen == 6) &&
					HgMmIsAddressValid((PVOID)MyDisasm.Argument1.Memory.Displacement))
				{
					RtnAddress = (SIZE_T)MyDisasm.Argument1.Memory.Displacement;
				}
				// E8 0502FCFF     call XXX
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == CONSTANT_TYPE + RELATIVE_	&&
					(InstLen == 5)	&&
					HgMmIsAddressValid((PVOID)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue)))
				{
					RtnAddress = (SIZE_T)(/*MyDisasm.EIP + InstLen + */MyDisasm.Instruction.AddrValue);
				}
				//			mov eax, offset SbieDrv+0x1374e (b0b7e74e)
				// ffd0		call eax 
				else if ((MyDisasm.Argument1.ArgType & 0xFFFF0000) == (REGISTER_TYPE + GENERAL_REG)	&&
					(InstLen == 2)	&&
					HgMmIsAddressValid((PVOID)pMove))
				{
					RtnAddress = pMove;
				}
				// mov eax, XXX
				// call dword ptr [eax]
			}
			else if (_strnicmp(MyDisasm.Instruction.Mnemonic, "push", 4) == 0 && MyDisasm.Instruction.BranchType == 0 && LOWORD (MyDisasm.Instruction.Category) == DATA_TRANSFER)
			{
				//SdtLog(("Enter PUSH\n"));
				// push XXX
				// ret
				if ((SIZE_T)MyDisasm.Instruction.Immediat > (SIZE_T)MM_SYSTEM_RANGE_START &&
					HgMmIsAddressValid((PVOID)MyDisasm.Instruction.Immediat))
				{
					DISASM disasm2;
					int InstLen2 = 0;
					RtlZeroMemory(&disasm2, sizeof(DISASM));
					disasm2.EIP = MyDisasm.EIP + InstLen;
#if defined(_WIN64)
					// =================== Define 64 bits architecture =================== 
					disasm2.Archi = 64;
#else
					disasm2.Archi = 0;
#endif
					InstLen2 = Disasm(&disasm2);
					//SdtLog(("0x%p: %s\n", disasm2.EIP, disasm2.CompleteInstr));
					if (disasm2.Instruction.BranchType == RetType)
					{
						RtnAddress = (SIZE_T)MyDisasm.Instruction.Immediat;
					}
				}
			}

			MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)InstLen;
		}else{
			Error = 1;
		}
		*pOffset = nOffset;
		nOffset += InstLen;
	} while ((!RtnAddress) && (!Error) && (nOffset < nLen));
	return RtnAddress;
}

SIZE_T CheckSdtInlineHook32(SIZE_T Ordinal, OUT PSIZE_T pOffsetOfHook)
{
	SIZE_T Address = 0;
	PSIZE_T ReloadedServiceTable = (PSIZE_T)g_hg_map[eHG_ReloadKiServiceTable];
	PSIZE_T OriginalServiceTable = (PSIZE_T)g_hg_map[eHG_OriginKiServiceTable];

	if (ReloadedServiceTable && OriginalServiceTable && 
		HgMmIsAddressValid((PVOID)ReloadedServiceTable) && 
		HgMmIsAddressValid((PVOID)OriginalServiceTable))
	{
		if (HgMmIsAddressValid((PVOID)ReloadedServiceTable[Ordinal]) &&
			HgMmIsAddressValid((PVOID)OriginalServiceTable[Ordinal]))
		{
			if (memcmp((PBYTE)(ReloadedServiceTable[Ordinal]), (PBYTE)(OriginalServiceTable[Ordinal]), 0x20))
			{
				Address = RetrieveInlineHookAddress32(OriginalServiceTable[Ordinal], 0x10, pOffsetOfHook);

				if (Address == 0	||
					Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
					!HgMmIsAddressValid((PVOID)Address))
				{
					Address = 0;
				}
			}

			if (Address == 0)
			{
				Address = (SIZE_T)CheckNtoskrnlWin32kInlineHook32((PVOID)OriginalServiceTable[Ordinal], pOffsetOfHook);
				if (Address == 0	||
					Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
					!HgMmIsAddressValid((PVOID)Address))
				{
					Address = 0;
				}
			}
		}
	}

	return Address;
}

SIZE_T CheckShadowInlineHook32(SIZE_T Ordinal, OUT PSIZE_T pOffsetOfHook)
{
	SIZE_T Address = 0;

	PSIZE_T pReloadShadowSdt = (PSIZE_T)g_hg_map[eHG_ReloadShadowKiServiceTable];
	PSIZE_T pOriginalShadowSdt = (PSIZE_T)g_hg_map[eHG_OriginShadowKiServiceTable];
	if (pOriginalShadowSdt && pReloadShadowSdt	&&
		HgMmIsAddressValid((PVOID)pOriginalShadowSdt) && HgMmIsAddressValid((PVOID)pReloadShadowSdt))
	{		
		SIZE_T OriginalTmp = pOriginalShadowSdt[Ordinal];
		SIZE_T ReloadTmp = pReloadShadowSdt[Ordinal];

		if (HgMmIsAddressValid((PVOID)OriginalTmp) && HgMmIsAddressValid((PVOID)ReloadTmp))
		{
			if (memcmp((PBYTE)OriginalTmp, (PBYTE)ReloadTmp, 0x20))
			{
#if defined(_WIN64)
#else
				Address = RetrieveInlineHookAddress32(OriginalTmp, 0x10, pOffsetOfHook);
#endif
				if (Address == 0	||
					Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
					!HgMmIsAddressValid((PVOID)Address))
				{
					Address = 0;
				}
			}

			if (Address == 0)
			{
#if defined(_WIN64)
#else
				Address = (SIZE_T)CheckNtoskrnlWin32kInlineHook32((PVOID)OriginalTmp, pOffsetOfHook);
#endif
				if (Address == 0	||
					Address < (SIZE_T)MM_SYSTEM_RANGE_START ||
					!HgMmIsAddressValid((PVOID)Address))
				{
					Address = 0;
				}
			}
		}
	}

	return Address;
}
#endif

BOOL HgEnumSsdtHookInfo(PITP pItp)
{
	BOOL bRtn = FALSE;
	PVOID ntdllBase = NULL;
	PIMAGE_DOS_HEADER DosHeader = NULL;
	PIMAGE_NT_HEADERS NtHeader = NULL;
	IMAGE_OPTIONAL_HEADER OptionalHeader;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory;
	PDWORD arrayOfFunctionAddress;
	PDWORD arrayOfFunctionNames;
	PWORD arrayofFunctionOrdinals;
	DWORD BaseOrdinal = 1;
	DWORD i =0;
	CHAR pszNT[] = "nt";
	PCHAR FunctionName = NULL;
	DWORD FunctionOrdinal = 0;
	SIZE_T FunctionAddress = 0;
	DWORD Hint = 0;
	DWORD dwEdi = 0;
	DWORD nCount = 0;
	ANSI_STRING aNameTmp;
	UNICODE_STRING uNameTmp;
	SIZE_T OffsetOfHook = 0;

	PKSERVICE_TABLE_DESCRIPTOR CurrentSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginSSDT];
	PKSERVICE_TABLE_DESCRIPTOR OriginalSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_ReloadSSDT];
	PSIZE_T CurrentServiceTable = (PSIZE_T)(((PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_CurrentSSDT])->ServiceTableBase);
	PSIZE_T OriginalServiceTable = (PSIZE_T)g_hg_map[eHG_OriginKiServiceTable];
	SIZE_T NumberOfServices = g_hg_map[eHG_SSDTCount];
	SIZE_T nOffsetOfHint = 0;

#if defined(_WIN64)
	PKSERVICE_TABLE_DESCRIPTOR CurrentSSDT64 = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginalSSDT64];
	PULONG32 CurrentServiceTable64 = (PULONG32)(CurrentSSDT64->ServiceTableBase);
	ntdllBase = ImageNtdll64();
#else
	ntdllBase = ImageNtdll32();
#endif

	if (ntdllBase == NULL){
		return FALSE;
	}

	__try{
		DosHeader = (PIMAGE_DOS_HEADER)ntdllBase;
		if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE){
			return FALSE;
		}
		NtHeader = (PIMAGE_NT_HEADERS)((SIZE_T)DosHeader + DosHeader->e_lfanew);
		if (NtHeader->Signature != IMAGE_NT_SIGNATURE){
			return FALSE;
		}

		OptionalHeader = NtHeader->OptionalHeader;
		ExportDirectory = (IMAGE_EXPORT_DIRECTORY*)((PBYTE)ntdllBase + OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
		arrayOfFunctionAddress = (PDWORD)((PBYTE)ntdllBase + ExportDirectory->AddressOfFunctions);
		arrayOfFunctionNames = (PDWORD)((PBYTE)ntdllBase + ExportDirectory->AddressOfNames);
		arrayofFunctionOrdinals = (PWORD)((PBYTE)ntdllBase + ExportDirectory->AddressOfNameOrdinals);
		BaseOrdinal = ExportDirectory->Base;

		for (i = 0; i < ExportDirectory->NumberOfFunctions; i++)	{
			FunctionName = (PCHAR)((PBYTE)ntdllBase + arrayOfFunctionNames[i]);
			if (FunctionName && HgMmIsAddressValid(FunctionName) && strlen(FunctionName) > strlen(pszNT) && !_strnicmp(FunctionName, pszNT, strlen(pszNT)))
			{
				SIZE_T nFunLen = strlen(FunctionName);
				if (nFunLen >= 64){
					nFunLen = 63;
				}
				FunctionOrdinal = arrayofFunctionOrdinals[i] + BaseOrdinal - 1;
				FunctionAddress = (SIZE_T)((PBYTE)ntdllBase + arrayOfFunctionAddress[FunctionOrdinal]);

#if defined(_WIN64)
				nOffsetOfHint = 4;
#else
				nOffsetOfHint = 1;
#endif
				if (HgMmIsAddressValid((PVOID)(FunctionAddress + nOffsetOfHint))){
					Hint = *((PWORD)(FunctionAddress + nOffsetOfHint));
					if (Hint > (DWORD)g_hg_map[eHG_SSDTCount])
					{
					}
					else
					{
						//SdtLog(("[%d][0x%p]function name: %s\n", Hint, CurrentServiceTable64[Hint], FunctionName));
						if (pItp->sdt.nMaxCount > pItp->sdt.nCount)
						{
							pItp->sdt.sdt[pItp->sdt.nCount].Hint = Hint;
							RtlInitAnsiString(&aNameTmp, FunctionName);
							if (NT_SUCCESS(RtlAnsiStringToUnicodeString(&uNameTmp, &aNameTmp, TRUE)))
							{
								wcsncpy(pItp->sdt.sdt[pItp->sdt.nCount].Name, uNameTmp.Buffer, uNameTmp.Length / sizeof(WCHAR));
								RtlFreeUnicodeString(&uNameTmp);
							}
#if defined(_WIN64)							
							pItp->sdt.sdt[pItp->sdt.nCount].CurrentAddress = (PVOID)((SIZE_T)(SIZE_T)(CurrentSSDT64->ServiceTableBase) + ((SIZE_T)(CurrentServiceTable64[Hint & 0xFFF]) >> 4));
							pItp->sdt.sdt[pItp->sdt.nCount].OriginalAddress = (PVOID)((SIZE_T)(SIZE_T)(CurrentSSDT64->ServiceTableBase) + ((SIZE_T)(CurrentServiceTable64[Hint & 0xFFF]) >> 4));
							pItp->sdt.sdt[pItp->sdt.nCount].AddressOfHook = (PVOID)CheckSdtInlineHook64(Hint, &OffsetOfHook);
							pItp->sdt.sdt[pItp->sdt.nCount].OffsetOfHook = OffsetOfHook;
#else
							pItp->sdt.sdt[pItp->sdt.nCount].CurrentAddress = (PVOID)CurrentServiceTable[Hint];
							pItp->sdt.sdt[pItp->sdt.nCount].OriginalAddress = (PVOID)OriginalServiceTable[Hint];
							pItp->sdt.sdt[pItp->sdt.nCount].AddressOfHook = (PVOID)CheckSdtInlineHook32(Hint, &OffsetOfHook);
							pItp->sdt.sdt[pItp->sdt.nCount].OffsetOfHook = OffsetOfHook;
#endif
						}
						pItp->sdt.nCount ++;
					}
				}
			}
		}

		//
		// Allocate at ImageNtdll32/64
		// 
		if (ntdllBase){
			HgExFreePoolWithTag(ntdllBase, 0);
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		SdtLog(("%s(%d): Catch exception!\n", __FILE__, __LINE__));
	}

	return bRtn;
}

BOOL HgEnumShadowSsdtHookInfo(PITP pItp)
{
	BOOL bRtn = FALSE;

	SIZE_T i = 0;
	SIZE_T OffsetOfHook = 0;
#if defined(_WIN64)
	PKSERVICE_TABLE_DESCRIPTOR pCurrentShadowSdt = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginalShadowSSDT64];
	SIZE_T nShadowSdtCount = pCurrentShadowSdt->NumberOfServices;
	if (pItp && pCurrentShadowSdt && HgMmIsAddressValid((PVOID)pCurrentShadowSdt))
	{
		for (i = 0; i < nShadowSdtCount; i++)
		{
			OffsetOfHook = 0;
			if (pItp->shadow_sdt.nMaxCount > pItp->sdt.nCount)
			{
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].Hint = i;
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].CurrentAddress = (PVOID)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalShadowSSDT64], i);
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].OriginalAddress = (PVOID)CalculateFunctionAddressFromSDT(g_hg_map[eHG_OriginalShadowSSDT64], i);
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].AddressOfHook = (PVOID)CheckShadowInlineHook64(i, &OffsetOfHook);
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].OffsetOfHook = OffsetOfHook;
			}
			pItp->shadow_sdt.nCount ++;
		}
	}
#else
	PSIZE_T pOriginalShadowSdt = (PSIZE_T)g_hg_map[eHG_OriginShadowKiServiceTable];
	PKSERVICE_TABLE_DESCRIPTOR pCurrentShadowSdtEntry = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_NowShadowSSDT];
	SIZE_T nShadowSdtCount = g_hg_map[eHG_ShadowSSDTFunctionsCount];
	//SdtLog(("%s(%d): nShadowSdtCount = %d\n", __FILE__, __LINE__, nShadowSdtCount));
	//SdtLog(("---------------0x%p, 0x%p\n", pOriginalShadowSdt, pCurrentShadowSdtEntry));
	if (pItp && pOriginalShadowSdt && pCurrentShadowSdtEntry && nShadowSdtCount)
	{
		for (i = 0; i < nShadowSdtCount; i++)
		{
			OffsetOfHook = 0;
			if (pItp->shadow_sdt.nMaxCount > pItp->sdt.nCount)
			{
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].Hint = i;
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].CurrentAddress = (PVOID)pCurrentShadowSdtEntry->ServiceTableBase[i];
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].OriginalAddress = (PVOID)pOriginalShadowSdt[i];
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].AddressOfHook = (PVOID)CheckShadowInlineHook32(i, &OffsetOfHook);
				pItp->shadow_sdt.shadow_sdt[pItp->shadow_sdt.nCount].OffsetOfHook = OffsetOfHook;
			}
			pItp->shadow_sdt.nCount ++;
		}
	}
#endif
	
	return bRtn;
}

//
// Export
// 
NTSTATUS EnumSSDT(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize)
{
	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pSdtItp = (PITP)pOutBuffer;
	//
	// Get all sdt function.
	// 
	HgEnumSsdtHookInfo(pSdtItp);

	status = STATUS_SUCCESS;
	return status;
}

NTSTATUS EnumShadowSSDT(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize)
{
	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pSdtItp = (PITP)pOutBuffer;
	//
	// Get all sdt function.
	//
	HgEnumShadowSsdtHookInfo(pSdtItp);

	status = STATUS_SUCCESS;
	return status;
}
