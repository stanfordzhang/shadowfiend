/************************************************************************/
/*                                                                      */
/************************************************************************/
#include "hgprocess.h"
#include "hgheader.h"

NTSTATUS
EnumProcess(
	__in	PVOID	pInBuffer,
	__in	ULONG	uInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	uOutSize,
	__out	ULONG*	pReturnLength
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pOutItp = (PITP)pOutBuffer;
	size_t		i = 0;
	LookupIdleProcess(pOutItp);
	LookupSystemProcess(pOutItp);
	status = EnumProcess_NtQuerySystemInformation(pOutItp);
	EnumProcess_PspCidTable(pOutItp);
	EnumProcessByThreadList(pOutItp);

	return status;
}

NTSTATUS
EnumProcess_NtQuerySystemInformation(
	__in	PITP	pItp
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PETHREAD	pEThread = HgKeGetCurrentThread();
	CHAR		PreMode;
	SIZE_T		ReturnLength = 0;
	PVOID		SystemInformation = NULL;
	SIZE_T		nRet = 0;
	PSYSTEM_PROCESS_INFORMATION pSpi = NULL;
	PEPROCESS	pEProcess = NULL;

	if (pItp == NULL){
		return status;
	}

	ProcessLog(("Enter EnumProcess!\n"));
	PreMode = ChangePreMode(pEThread);

	status = HgNtQuerySystemInformation(SystemProcessInformation,
										NULL,
										0,
										&ReturnLength);

	ProcessLog(("HgNtQuerySystemInformation return status = 0x%p; \
				ReturnLength = %d.\n", status, ReturnLength));

	if (status == STATUS_INFO_LENGTH_MISMATCH){
		nRet = ReturnLength;
		SystemInformation = HgExAllocatePoolWithTag(NonPagedPool,
													nRet,
													HG_MEM_TAG);
		if (SystemInformation != NULL){
			status = HgNtQuerySystemInformation(SystemProcessInformation,
												SystemInformation,
												nRet,
												&ReturnLength);
			if (NT_SUCCESS(status)){
				size_t nnn = 0;
				ProcessLog(("HgNtQuerySystemInformation successful!\n"));
				pSpi = (PSYSTEM_PROCESS_INFORMATION)SystemInformation;
				while (TRUE){
					nnn++;
//					ProcessLog(("***********%04d - PID: %d\n", nnn, (ULONG_PTR)pSpi->UniqueProcessId));
					if ((ULONG_PTR)pSpi->UniqueProcessId == 0){
						// refer to LookupIdleProcess method.
						goto HG_LOOKUP_PROCESS_NEXT;
					}else if ((ULONG_PTR)pSpi->UniqueProcessId == 4){
						// refer to LookupSystemProcess method.
						goto HG_LOOKUP_PROCESS_NEXT;
					}else{
						// if assigning 0(Idle.exe process) to 
						// PsLookupProcessByProcessId's parameter
						// ProcessId, this function will be
						// return STATUS_INVALID_PARAMETER
						// NOTE(MSDN): If the call to PsLookupProcessByProcessId
						// is successful, PsLookupProcessByProcessId increases
						// the reference count on the object returned in the
						// Process parameter. Consequently, when a driver has 
						// completed using the Process parameter, the driver must
						// call ObDereferenceObject to dereference the Process
						// parameter received from the PsLookupProcessByProcessId routine.
						status = HgPsLookupProcessByProcessId(
												(HANDLE)pSpi->UniqueProcessId, 
												&pEProcess);
//						ProcessLog(("HgPsLookupProcessByProcessId(Idle)- pEProcess 0x%p\n", pEProcess));
						if (NT_SUCCESS(status))
						{
							InsertProcessToBuffer(pEProcess, pItp);
							// invoke ObfDereferenceObject for PsLookupProcessByProcessId
							HgObfDereferenceObject((PVOID)pEProcess);
						}
					}
HG_LOOKUP_PROCESS_NEXT:
					// NOTE: If you use do...while() to do this,
					// you must be careful about the while condition.
					// When I use do...while(), had made a mistake,
					// what a shame!
					if (pSpi->NextEntryOffset == 0)
					{
						break;
					}
					pSpi = (PSYSTEM_PROCESS_INFORMATION)((ULONG_PTR)pSpi + pSpi->NextEntryOffset);
				}
			}
		}

		HgExFreePoolWithTag(SystemInformation, HG_MEM_TAG);
	}

	RecoverPreMode(pEThread, PreMode);
	ProcessLog(("Leave EnumProcess!\n"));

	return status;
}

VOID
LookupIdleProcess(
	__inout	PITP	pItp
	)
{
	ULONG_PTR ulIdleProcessAddr = 0;
	ULONG_PTR PsInitialSystemProcessAddr = 0;
#if defined(_M_AMD64)
	ulIdleProcessAddr = __readgsqword(0x20);
	ulIdleProcessAddr += GetOffset(_KPRCB_IdleThread);
	ulIdleProcessAddr = *(PULONG_PTR)ulIdleProcessAddr;
	ulIdleProcessAddr += g_hg_map[eHG_KTHREAD_Process]; // 0x210 is the offset of Process field in _KTHREAD structure.
	ulIdleProcessAddr = *(PULONG_PTR)ulIdleProcessAddr;
	ProcessLog(("In Win64, the Idle Process's EPROCESS is: 0x%p\n", ulIdleProcessAddr));
#else
	if (g_WindowsBuildNumber >= 6001){
		PsInitialSystemProcessAddr =
			g_hg_map[eHG_PsInitialSystemProcessAddress];

		if ((PsInitialSystemProcessAddr != 0)				&&
			(HgMmIsAddressValid((PVOID)(PsInitialSystemProcessAddr
			+ sizeof(ULONG))) == TRUE))
		{
			ulIdleProcessAddr = *(PULONG_PTR)(PsInitialSystemProcessAddr
				+ sizeof(ULONG));
		}
	}else{
		__asm{
			push eax
			mov eax, FS:[0x20]
			mov ulIdleProcessAddr, eax
			pop eax
		}

		if ((ulIdleProcessAddr != 0)				&&
			(HgMmIsAddressValid((PVOID)ulIdleProcessAddr) == TRUE))
		{
			ulIdleProcessAddr += GetOffset(_KPRCB_IdleThread);
			ulIdleProcessAddr = *(PULONG_PTR)ulIdleProcessAddr;
			ulIdleProcessAddr += g_hg_map[eHG_Offset_KTHREAD_ApcState];
			ulIdleProcessAddr += g_hg_map[eHG_Offset_KAPC_STATE_Process];
			ulIdleProcessAddr = *(PULONG_PTR)ulIdleProcessAddr;
		}
	}
#endif
	if ((ulIdleProcessAddr != 0)		&&
		(HgMmIsAddressValid((PVOID)ulIdleProcessAddr) == TRUE))
	{
		pItp->process.pProcess[pItp->process.nRealCount].nPid = 0;
		pItp->process.pProcess[pItp->process.nRealCount].nParentPid = 0;
		pItp->process.pProcess[pItp->process.nRealCount].pEProcess = ulIdleProcessAddr;
		wcscpy(pItp->process.pProcess[pItp->process.nRealCount].pwszPath, L"System Idle Process");
		pItp->process.nRealCount ++;
	}
}

VOID 
LookupSystemProcess(
	__inout	PITP	pItp
	)
{
	PEPROCESS pSystemProcess = (PEPROCESS)g_hg_map[eHG_SystemProcess];
	ULONG_PTR ulPid = 0;

	ulPid = *(PULONG_PTR)((*(PULONG_PTR)((ULONG_PTR)pSystemProcess 
		+ g_hg_map[eHG_ObjectTableOffset_EPROCESS]))
		+ g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]);

	if (ulPid != 0){
		pItp->process.pProcess[pItp->process.nRealCount].nPid = ulPid;
	}else{
		if(HgPsGetProcessId(pSystemProcess) != 0){
			pItp->process.pProcess[pItp->process.nRealCount].nPid = (ULONG_PTR)HgPsGetProcessId(pSystemProcess);
		}else{
			pItp->process.pProcess[pItp->process.nRealCount].nPid = *(PULONG_PTR)((ULONG_PTR)pSystemProcess
				+ g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]);
		}
	}
	pItp->process.pProcess[pItp->process.nRealCount].nParentPid = 0;
	pItp->process.pProcess[pItp->process.nRealCount].pEProcess = (ULONG_PTR)pSystemProcess;
	wcscpy(pItp->process.pProcess[pItp->process.nRealCount].pwszPath, L"System");
	pItp->process.nRealCount ++;
}

BOOL
GetProcessImagePathByEProcess(
	__in	PEPROCESS	pEProcess,
	__out	PWCHAR		pwszPath
	)
{
	BOOL bRtn = FALSE;
	PSECTION_OBJECT pSectionObject = NULL;

	if ((pEProcess == NULL)	||
		(pwszPath == NULL))
	{
		return FALSE;
	}

#if defined(_WIN64)
	bRtn = GetProcessImagePathBy_SE_AUDIT_PROCESS_CREATION_INFO_64(pEProcess, pwszPath);
#else
	if (g_WindowsVersion == eWin_2000){
	}else{
		pSectionObject = *(PSECTION_OBJECT*)((ULONG_PTR)pEProcess
			+ g_hg_map[eHG_SectionObjectOffset_EPROCESS]);

		ProcessLog(("1 - $ pSectionObject 0x%p!\n", pSectionObject));
	}

	if ((pSectionObject != NULL)							&&
		(HgMmIsAddressValid((PVOID)pSectionObject) == TRUE))
	{
		bRtn = GetProcessImagePathBySectionObject(
			pSectionObject, pwszPath);
	}
#endif

	if ((bRtn == FALSE)										&&
		(HgMmIsAddressValid((PVOID)((ULONG_PTR)pEProcess + GetOffset(_EPROCESS_Peb)))))
	{
		ProcessLog(("pEProcess: 0x%p\n", pEProcess));

#if defined(_WIN64)
		bRtn = GetProcessImagePathByPeb64(pEProcess, pwszPath);
#else
		bRtn = GetProcessImagePathByPeb32(pEProcess, pwszPath);
#endif
	}

	if (bRtn == FALSE){
		wcscpy(pwszPath, L"N/A");
	}

	return bRtn;
}

BOOL
GetProcessImagePathBySectionObject(
	__in	PSECTION_OBJECT	pSectionObject,
	__out	PWCHAR			pwszPath
	)
{
	BOOL bRtn = FALSE;
	PSEGMENT_OBJECT pSegment = NULL;
	PCONTROL_AREA	pControlArea = NULL;
	PFILE_OBJECT	pFileObject = NULL;

	if ((pSectionObject == NULL)						||
		(pwszPath == NULL)								||
		(HgMmIsAddressValid(pSectionObject) == FALSE)	||
		(KeGetCurrentIrql()	> APC_LEVEL))	// ObQueryNameString
	{
		return FALSE;
	}

	__try{
		pSegment = (PSEGMENT_OBJECT)pSectionObject->Segment;
		if ((pSegment == NULL)							||
			(HgMmIsAddressValid(pSegment) == FALSE))
		{
			return FALSE;
		}
		ProcessLog(("2 - $ pSegment 0x%p!\n", pSegment));
#if defined(_WIN64)
		pControlArea = (PCONTROL_AREA)(*(PULONG_PTR)((ULONG_PTR)pSegment + 0x20));
		ProcessLog(("pControlArea: 0x%p\n", pControlArea));
#else
		pControlArea = (PCONTROL_AREA)pSegment->ControlArea;
#endif
		if ((pControlArea == NULL)						||
			(HgMmIsAddressValid(pControlArea) == FALSE))
		{
			return FALSE;
		}
		ProcessLog(("3 - $ pControlArea 0x%p!\n", pControlArea));

		pFileObject = pControlArea->FilePointer;
		if ((pFileObject == NULL)						||
			(HgMmIsAddressValid(pFileObject) == FALSE))
		{
			return FALSE;
		}
		ProcessLog(("4 - $ pFileObject 0x%p!\n", pFileObject));

		if ((g_WindowsVersion == eWin_7)				||
			(g_WindowsVersion == eWin_Vista)			||
			(g_WindowsVersion == eWin_Vista_SP1_SP2)	||
			(g_WindowsVersion == eWin_8))
		{
			pFileObject = (PFILE_OBJECT)((ULONG_PTR)pFileObject & 0xFFFFFFF8);
		}

		bRtn = GetProcessImagePathByFileObject(pFileObject, pwszPath);
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("failed to get process image path by SectionObject\n"));
		wcscpy(pwszPath, L"N/A");
	}
	return bRtn;
}

BOOL
GetProcessImagePathByFileObject(
	__in	PFILE_OBJECT	pFileObject,
	__out	PWCHAR			pwszPath
	)
{
	BOOL bRtn = FALSE;
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	POBJECT_NAME_INFORMATION uFilePath = NULL;
	
	if ((pFileObject == NULL)	||
		(pwszPath == NULL))
	{
		return FALSE;
	}

	// retrieve an MS-DOS device name for a file.
	// such as: C:\WINDOWS\system32\wscntfy.exe
	// NOTE: IoQueryFileDosDeviceName will allocate a new
	// OBJECT_NAME_INFORMATION structure,
	// so you need to free it eventually by calling ExFreePool.
	if (NT_SUCCESS(HgIoQueryFileDosDeviceName(pFileObject, &uFilePath)))
	{
//		ProcessLog(("MS-DOS device name: %S\n", uFilePath->Name.Buffer));
		if (IsUnicodeStringValid(&uFilePath->Name) == TRUE)
		{
			size_t nLen = GetUnicodeStringSafeLength(&uFilePath->Name, SZ_PATH_LEN);
			wcsncpy(pwszPath, uFilePath->Name.Buffer, nLen);
			bRtn = TRUE;
		}
		HgExFreePoolWithTag(uFilePath, 0);
	}

	// if IoQueryFileDosDeviceName failed to retrieve name.
	// NOTE: ObQueryNameString is dangerous method, 
	// refer to the following steps.
	// In MSDN, the parameter {ObjectNameInfo [out, optional]} means:
	// A pointer to a CALLER-allocated buffer, 
	// of the OBJECT_NAME_INFORMATION structure, 
	// that receives the object name information.
	if (bRtn == FALSE){
		ULONG ulRtn = 0;
		PVOID pBuffer = HgExAllocatePoolWithTag(PagedPool, 
												0x1000, 
												HG_MEM_TAG);

		if (pBuffer != NULL){
			RtlZeroMemory(pBuffer, 0x1000);
			status = HgObQueryNameString(	pFileObject,
											(POBJECT_NAME_INFORMATION)pBuffer,
											0x1000,
											&ulRtn);
			if (NT_SUCCESS(status)){
				POBJECT_NAME_INFORMATION pTmp =	(POBJECT_NAME_INFORMATION)pBuffer;

				WCHAR wszHarddiskName[SZ_PATH_LEN] = L"\\Device\\HarddiskVolume";

				if (IsUnicodeStringValid(&pTmp->Name))
				{
					if (pTmp->Name.Length / sizeof(WCHAR) > wcslen(wszHarddiskName)		&&
						!_wcsnicmp(pTmp->Name.Buffer, wszHarddiskName, wcslen(wszHarddiskName)))
					{
						UNICODE_STRING pDosName;
						// return the MS-DOS path for a specified device 
						// object that represents a file system volume.
						if (NT_SUCCESS(HgIoVolumeDeviceToDosName(pFileObject->DeviceObject, &pDosName)))
						{
							if (IsUnicodeStringValid(&pDosName)){
								ULONG_PTR nlen = GetUnicodeStringSafeLength(&pDosName, SZ_PATH_LEN);
								wcsncpy(pwszPath, pDosName.Buffer, nlen);
								nlen = SZ_PATH_LEN - nlen > 	pTmp->Name.Length / sizeof(WCHAR) - 	wcslen(wszHarddiskName) - 1 ? 
									pTmp->Name.Length / sizeof(WCHAR) - 	wcslen(wszHarddiskName) - 1 : SZ_PATH_LEN - nlen - 1; 
								wcsncat(pwszPath, pTmp->Name.Buffer + wcslen(wszHarddiskName) + 1, nlen);
//								ProcessLog(("MS-DOS device name: %ws\n", pwszPath));
								bRtn = TRUE;
							}
							HgExFreePoolWithTag(pDosName.Buffer, 0);
						}
					}else{
						ULONG_PTR nLen = GetUnicodeStringSafeLength(&pTmp->Name, SZ_PATH_LEN);
						wcsncpy(pwszPath, pTmp->Name.Buffer, nLen);
						bRtn = TRUE;
					}
				}
			}
			HgExFreePoolWithTag(pBuffer, HG_MEM_TAG);
		}
	}

	return bRtn;
}

BOOL
GetProcessImagePathByPeb32(
	__in	PEPROCESS		pEProcess,
	__out	PWCHAR			pwszPath
	)
{
	BOOL bRtn = FALSE;
	PHG_PEB peb = NULL;
	KAPC_STATE ks;
	PPEB_LDR_DATA pLdr = NULL;
	BOOL bAttach = FALSE;
	PVOID pBuffer = NULL;

	if ((pEProcess == NULL)					||
		(pwszPath == NULL)					||
		(KeGetCurrentIrql() >= DISPATCH_LEVEL))
	{
		return FALSE;
	}

	if ((HgMmIsAddressValid((PVOID)((ULONG_PTR)pEProcess + GetOffset(_EPROCESS_Peb))) == FALSE))
	{
		return FALSE;
	}
	ProcessLog(("GetOffset(_EPROCESS_Peb): 0x%p\n", GetOffset(_EPROCESS_Peb)));
	peb = *(PHG_PEB*)((ULONG_PTR)pEProcess + GetOffset(_EPROCESS_Peb));
	ProcessLog(("peb: 0x%p\n", peb));
	if (((ULONG_PTR)peb > 0)					&&
		(ULONG_PTR)peb < (SIZE_T)MM_SYSTEM_RANGE_START	&&
		(HgMmIsAddressValid((PVOID)peb) == TRUE))
	{
		pBuffer = HgExAllocatePoolWithTag(	PagedPool,
											SZ_PATH_LEN_WCHAR,
											HG_MEM_TAG);
		if (pBuffer != NULL){
			RtlZeroMemory(pBuffer, SZ_PATH_LEN_WCHAR);

			// does not attach self process.
			if (HgIoGetCurrentProcess() != pEProcess){
				HgKeStackAttachProcess(pEProcess, &ks);
				bAttach = TRUE;
			}

			__try{
				HgProbeForRead(peb, sizeof(HG_PEB), 1);
				ProcessLog(("peb address: 0x%p\n", peb));

				pLdr = (PPEB_LDR_DATA)(peb->Ldr);
				if ((ULONG_PTR)pLdr > 0 && (ULONG_PTR)pLdr < (SIZE_T)MM_SYSTEM_RANGE_START){
					PLIST_ENTRY entry = NULL;
					PLDR_DATA_TABLE_ENTRY pLdrEntry = NULL;

					HgProbeForRead(pLdr, sizeof(PEB_LDR_DATA), 1);
					ProcessLog(("pLdr address: 0x%p\n", pLdr));
					entry = pLdr->InLoadOrderModuleList.Flink;
					ProcessLog(("entry address: 0x%p\n", entry));
					pLdrEntry = CONTAINING_RECORD(	entry, 
													LDR_DATA_TABLE_ENTRY,
													InLoadOrderLinks);
//((LDR_DATA_TABLE_ENTRY *)((PCHAR)(entry) - (ULONG_PTR)(&((LDR_DATA_TABLE_ENTRY *)0)->InLoadOrderLinks)));
					ProcessLog(("pLdrEntry address: 0x%p\n", pLdrEntry));
					// User process space
					if ((ULONG_PTR)pLdrEntry > 0					&&
						(ULONG_PTR)pLdrEntry < (SIZE_T)MM_SYSTEM_RANGE_START)
					{
						HgProbeForRead(	pLdrEntry, 
										sizeof(LDR_DATA_TABLE_ENTRY), 
										1);
						if (pLdrEntry->FullDllName.Buffer		&&
							pLdrEntry->FullDllName.Length)
						{
							ULONG_PTR nLen = GetUnicodeStringSafeLength(
								&pLdrEntry->FullDllName, SZ_PATH_LEN);
							HgProbeForRead(pLdrEntry->FullDllName.Buffer,
								nLen * sizeof(WCHAR), sizeof(WCHAR));
							wcsncpy((WCHAR*)pBuffer,
									pLdrEntry->FullDllName.Buffer, 
									nLen);
							ProcessLog(("pBuffer address: %S\n", (WCHAR*)pBuffer));
							bRtn = TRUE;
						}
					}
				}
			}__except(EXCEPTION_EXECUTE_HANDLER){
				wcscpy(pwszPath, L"N/A");
			}

			if (bAttach == TRUE){
				HgKeUnstackDetachProcess(&ks);
				bAttach = FALSE;
			}

			if (bRtn == TRUE){
				wcscpy(pwszPath, (WCHAR*)pBuffer);
			}

			HgExFreePoolWithTag(pBuffer, HG_MEM_TAG);
		}
	}

	return bRtn;
}

BOOL
GetProcessImagePathByPeb64(
	__in	PEPROCESS		pEProcess,
	__out	PWCHAR			pwszPath
	)
{
	UNICODE_STRING imagName = {0};
	ULONG_PTR uProcessPtr;
	ULONG_PTR uImagNameAddr = 0;
	ULONG_PTR uPebOffset = GetOffset(_EPROCESS_Peb);
	ULONG_PTR uParamOffset = g_hg_map[eHG_PEB_ProcessParameters];
	ULONG_PTR uImagNameOffset = g_hg_map[eHG_RTL_USER_PROCESS_PARAMETERS_ImagePathName] + 0x008;
	BOOL bRtn = FALSE;
	KAPC_STATE ks;
	PPEB_LDR_DATA pLdr = NULL;
	BOOL bAttach = FALSE;
	PVOID pBuffer = NULL;

	if ((pEProcess == NULL)								||
		(HgMmIsAddressValid((PVOID)pEProcess) == FALSE)	||
		(pwszPath == NULL))
	{
		return FALSE;
	}

	if(KeGetCurrentIrql() != PASSIVE_LEVEL)
		return FALSE;

	if(uPebOffset==0)
	{
		ProcessLog (("this function do not supported current os.\r\n"));
		return FALSE ;
	}
	
	pBuffer = HgExAllocatePoolWithTag(	PagedPool,
		SZ_PATH_LEN_WCHAR,
		HG_MEM_TAG);
	if (pBuffer != NULL){
		RtlZeroMemory(pBuffer, SZ_PATH_LEN_WCHAR);

		uProcessPtr = (ULONG_PTR)pEProcess;
		uImagNameAddr = (uProcessPtr + uPebOffset);
		uImagNameAddr = *(PULONG_PTR)uImagNameAddr;
		ProcessLog(("Peb address: 0x%p\n", uImagNameAddr));

		if (((ULONG_PTR)uImagNameAddr > 0)						//&&
	//		((ULONG_PTR)uImagNameAddr < (SIZE_T)MM_SYSTEM_RANGE_START)	&&
			/*(HgMmIsAddressValid((PVOID)uImagNameAddr) == TRUE)*/)
		{
			if (HgIoGetCurrentProcess() != pEProcess){
				HgKeStackAttachProcess(pEProcess, &ks);
				bAttach = TRUE;
			}

			__try{
				HgProbeForRead((PVOID)uImagNameAddr, sizeof(ULONG_PTR), 1);
				uImagNameAddr += uParamOffset;
				uImagNameAddr = *(PULONG_PTR)uImagNameAddr;
				ProcessLog(("uImagNameAddr: 0x%p\n", uImagNameAddr));
				if (uImagNameAddr != 0)
				{
					uImagNameAddr += uImagNameOffset;
					uImagNameAddr = *(PULONG_PTR)uImagNameAddr;
					ProcessLog(("uImagNameAddr: 0x%p\n", uImagNameAddr));
					ProcessLog (("Process full path name: %ws\r\n",(PCWSTR)uImagNameAddr ));
					if (uImagNameAddr != 0)
					{
						wcscpy((WCHAR*)pBuffer,	(PCWSTR)uImagNameAddr);
						ProcessLog(("pBuffer address: %S\n", (WCHAR*)pBuffer));
					}
				}

				bRtn = TRUE;
			}__except(EXCEPTION_EXECUTE_HANDLER){
				wcscpy(pwszPath, L"N/A");
			}

			if (bAttach == TRUE){
				HgKeUnstackDetachProcess(&ks);
				bAttach = FALSE;
			}

			if (bRtn == TRUE){
				wcscpy(pwszPath, (WCHAR*)pBuffer);
			}
			HgExFreePoolWithTag(pBuffer, HG_MEM_TAG);
		}
	}
	bRtn = TRUE;
	return bRtn;
}

VOID
EnumProcess_PspCidTable(
	__in	PITP	pItp
	)
{
	ULONG_PTR PspCidTable = g_hg_map[eHG_PspCidTable];
	if (!pItp){
		return;
	}

	if (PspCidTable == 0){
		GetPspCidTable();
		PspCidTable = g_hg_map[eHG_PspCidTable];
	}

	if (PspCidTable != 0					&&
		HgMmIsAddressValid((PVOID)PspCidTable))
	{
		if (g_WindowsVersion == eWin_2000){
		}else{
			ScanCidTable(PspCidTable, pItp);
		}
	}
}

VOID
GetPspCidTable(
	VOID
	)
{
#if defined(_WIN64)
	GetPspCidTableByLookupFunction64();
#else
	if (g_WindowsVersion == eWin_8){
		GetPspCidTableByLookupFunction_Win8();
	}else{
		if (!GetPspCidTableByLookupFunction() &&
			(eWin_2000 != g_WindowsVersion))
		{
			GetPspCidTableByKdVersionBlock();
		}
	}
#endif
}

VOID
GetPspCidTableByLookupFunction_Win8(
	VOID
	)
{
	ULONG_PTR i = 0;
	size_t sign = 0;
	size_t uCodeSize = 0;
	ULONG_PTR PspReferenceCidTableEntry = 0;
	ULONG_PTR PspCidTable = 0;
	ULONG_PTR pFunAddr = GetPsLookupProcessByProcessIdAddress();

	if (pFunAddr != 0){
		for (i = (ULONG_PTR)pFunAddr; 
			i < (ULONG_PTR)pFunAddr + 100; 
			i += uCodeSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE){
				//PUCHAR pCode;
				//uCodeSize = SizeOfCode((PVOID)i, &pCode);
				//////////////////////////////////////////////////////////////////////////
				DISASM MyDisasm;
				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
				uCodeSize = Disasm(&MyDisasm);
				//////////////////////////////////////////////////////////////////////////
				if (uCodeSize == 0){
					break;
				}

				if (*(PBYTE)i == 0xE8		&&
					*(PBYTE)(i - 2) == 0x6A	&&
					*(PBYTE)(i + 5) == 0x8B	&&
					*(PBYTE)(i + 7) == 0x85)
				{
					PspReferenceCidTableEntry = 
						i + *(PULONG_PTR)(i + 1) + 5;
					break;
				}
			}
		}

		if (PspReferenceCidTableEntry != 0								&&
			HgMmIsAddressValid((PVOID)PspReferenceCidTableEntry) == TRUE)
		{
			uCodeSize = 0;
			for (i = (ULONG_PTR)PspReferenceCidTableEntry;
				i < (ULONG_PTR)PspReferenceCidTableEntry + 100;
				i += uCodeSize)
			{
				if(HgMmIsAddressValid((PVOID)i) == TRUE){
					//PUCHAR pCode;
					//uCodeSize = SizeOfCode((PVOID)i, &pCode);
					//////////////////////////////////////////////////////////////////////////
					DISASM MyDisasm;
					(void) memset (&MyDisasm, 0, sizeof(DISASM));
					MyDisasm.EIP = (UIntPtr)i;
					uCodeSize = Disasm(&MyDisasm);
					//////////////////////////////////////////////////////////////////////////
					if (uCodeSize == 0){
						break;
					}

					if (*(PUSHORT)i == 0x158B		&&
						*(PUSHORT)(i - 2) == 0xEC8B	&&
						*(PBYTE)(i + 6) == 0x83)
					{
						PspCidTable = *(PULONG_PTR)(i + 2);
						break;
					}
				}
			}
		}
	}
	g_hg_map[eHG_PspCidTable] = PspCidTable;
}

BOOL
GetPspCidTableByLookupFunction(
	VOID
	)
{
	ULONG i = 0;
	USHORT sign = 0;
	ULONG uCodeSize = 0;
	ULONG PspCidTable = 0;
	BOOL bRtn = FALSE;
	ULONG_PTR pFuncAddr = GetPsLookupProcessByProcessIdAddress();

	if (pFuncAddr == 0){
		return FALSE;
	}
	switch(g_WindowsVersion){
	case eWin_2000:
	case eWin_XP:
	case eWin_2003:
	case eWin_2003_SP1_SP2:
		{
			sign = 0x35FF;
		}
		break;
	case eWin_Vista:
	case eWin_Vista_SP1_SP2:
	case eWin_7:
		{
			sign = 0x3d8b;
		}
		break;
	}

	if (sign == 0){
		return FALSE;
	}

	for (i = (ULONG)pFuncAddr; 
		i < (ULONG)pFuncAddr + PAGE_SIZE; 
		i += uCodeSize)
	{
		if (HgMmIsAddressValid((PVOID)i) == TRUE){
			//PUCHAR pCode;
			DISASM MyDisasm;
			//uCodeSize = SizeOfCode((PVOID)i, &pCode);
			//////////////////////////////////////////////////////////////////////////
			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (UIntPtr)i;
			uCodeSize = Disasm(&MyDisasm);
			//////////////////////////////////////////////////////////////////////////
			if (uCodeSize == 0 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC3 || (BYTE)(MyDisasm.Instruction.Opcode) == 0xC2){
//			if (uCodeSize == 0 || *pCode == 0xC3 || *pCode == 0xC2){
				break;
			}
			if (sign == *(PUSHORT)i && 0xE8 == *(PBYTE)(i + 6)){
				PspCidTable = *(PULONG)(i + 2);
				bRtn = TRUE;
				break;
			}
		}
	}
	g_hg_map[eHG_PspCidTable] = PspCidTable;
	return bRtn;
}

BOOL
GetPspCidTableByLookupFunction64(
	VOID
	)
{
	BOOL bRtn = FALSE;
	ULONG_PTR PspCidTable = 0;
	ULONG_PTR pFunAddr = GetPsLookupProcessByProcessIdAddress();
	ULONG_PTR CurrentAddress = 0;
	ULONG_PTR Offset = 0;
	ULONG_PTR PspReferenceCidTableEntryAddress = 0;

	if (pFunAddr != 0 && HgMmIsAddressValid((PVOID)pFunAddr))
	{
		ProcessLog(("pFunAddr: 0x%p\n", pFunAddr));
		switch(g_WindowsVersion){
		case eWin_7:
			{
				CurrentAddress = (ULONG_PTR)pFunAddr + 0x034;
				if (HgMmIsAddressValid((PVOID)CurrentAddress))
				{
					Offset = *(PULONG_PTR)((ULONG_PTR)pFunAddr + 0x034 + 0x003) | (SIZE_T)0xFFFFFFFF00000000;
					ProcessLog(("QWORD: 0x%p\n", Offset));
					PspCidTable = CurrentAddress + Offset + 0x007;	// 0x007 is the length of the current instruction.
					if (HgMmIsAddressValid((PVOID)PspCidTable))
					{
						ProcessLog(("PspCidTable: 0x%p\n", PspCidTable));
						g_hg_map[eHG_PspCidTable] = PspCidTable;
						return TRUE;
					}
				}
			}
			break;
		case eWin_8:
			{
				CurrentAddress = (ULONG_PTR)pFunAddr + 0x02E;
				if (HgMmIsAddressValid((PVOID)CurrentAddress))
				{
					Offset = *(PULONG_PTR)(CurrentAddress + 0x001) | (SIZE_T)0xFFFFFFFF00000000;
					PspReferenceCidTableEntryAddress = CurrentAddress + Offset + 0x005;
					if (HgMmIsAddressValid((PVOID)PspReferenceCidTableEntryAddress))
					{
						ProcessLog(("PspReferenceCidTableEntryAddress: 0x%p\n", PspReferenceCidTableEntryAddress));
						CurrentAddress = PspReferenceCidTableEntryAddress + 0x00A;
						if (HgMmIsAddressValid((PVOID)CurrentAddress))
						{
							Offset = *(PULONG_PTR)(CurrentAddress + 0x003) | (SIZE_T)0xFFFFFFFF00000000;
							PspCidTable = CurrentAddress + Offset + 0x007;
							ProcessLog(("PspCidTable: 0x%p\n", PspCidTable));
							g_hg_map[eHG_PspCidTable] = PspCidTable;
							return TRUE;
						}
					}
				}
			}
			break;
		default:
			bRtn = FALSE;
		}
	}
	
	return bRtn;
}

VOID
GetPspCidTableByKdVersionBlock(
	VOID
	)
{
	size_t	PspCidTable = 0;
	size_t	KdVersionBlock = 0;
	
	InsertDpcToFindKdVersionBlock();

	KdVersionBlock = g_hg_map[eHG_KdVersionBlock];

	if ((KdVersionBlock != 0)								&&
		(HgMmIsAddressValid((PVOID)KdVersionBlock) == TRUE))
	{
		PspCidTable = *(PULONG)(KdVersionBlock + 0x80);
	}

	g_hg_map[eHG_PspCidTable] = PspCidTable;
}

VOID
InsertDpcToFindKdVersionBlock(
	VOID
	)
{
	KAFFINITY CpuAffinity;
	size_t nCpuCount = 0;
	size_t i = 0;

	CpuAffinity = HgKeQueryActiveProcessors();

	for(i = 0; i < sizeof(ULONG_PTR); i ++){
		if ((CpuAffinity >> i) & 1){
			nCpuCount ++;
		}
	}

	if (nCpuCount == 1){
		KIRQL OldIrql = KeRaiseIrqlToDpcLevel();
		GetKdVersionBlock();
		KeLowerIrql(OldIrql);
	}else{
		for(i = 0; i < sizeof(ULONG_PTR); i ++){
			if ((CpuAffinity >> i) & 1){
				KEVENT Event;
				KDPC Dpc;

				HgKeInitializeEvent(&Event, NotificationEvent, FALSE);
				HgKeInitializeDpc(&Dpc, DpcRoutine, &Event);
				HgKeSetTargetProcessorDpc(&Dpc, (CCHAR)i);
				HgKeSetImportanceDpc(&Dpc, HighImportance);
				HgKeInsertQueueDpc(&Dpc, NULL, NULL);

				if (HgKeWaitForSingleObject(
					&Event, (KWAIT_REASON)0, 0, 0, 0) == STATUS_SUCCESS)
				{
					size_t KdVersionBlock = g_hg_map[eHG_KdVersionBlock];
					if (KdVersionBlock != 0){
						break;
					}
				}
			}
		}
	}
}

VOID
GetKdVersionBlock(
	VOID
	)
{
	size_t KdVersionBlock = 0;

#ifdef _WIN64 //_M_AMD64//_AMD64_
#else
	__asm{
		mov eax, FS:[0x1C]	// offset 0x1C is _KPCR.SelfPcr
		test eax, eax
		jz _exit_
		mov eax, [eax + 0x34]	// offset 0x34 _KPCR.KdVersionBlock
		test eax, eax
		jz _exit_
		mov KdVersionBlock, eax
	_exit_:
	}
#endif
	
	g_hg_map[eHG_KdVersionBlock] = KdVersionBlock;
}

VOID
DpcRoutine(
	__in		struct _KDPC	*dpc,
	__in_opt	PVOID			DeferredContext,
	__in_opt	PVOID			SystemArgument1,
	__in_opt	PVOID			SystemArgument2
	)
{
	PKEVENT	pEvent = (PKEVENT)DeferredContext;
	
	GetKdVersionBlock();

	if (pEvent){
		HgKeSetEvent(pEvent, IO_NO_INCREMENT, FALSE);
	}
}

VOID
ScanCidTable(
	__in	ULONG_PTR	uHandleTable,
	__in	PITP		pItp
	)
{
	ULONG_PTR HandleTable;
	ULONG_PTR TableCode = 0;
	ULONG_PTR flag = 0;

	if (uHandleTable != 0							&&
		(HgMmIsAddressValid((PVOID)uHandleTable) == TRUE))
	{
		HandleTable = *(PULONG_PTR)uHandleTable;
		if (HandleTable != 0					&&
			HgMmIsAddressValid((PVOID)HandleTable))
		{
			if (g_WindowsVersion == eWin_8){
				TableCode = *(PULONG_PTR)(HandleTable + 8);
			}else{
				TableCode = *(PULONG_PTR)HandleTable;
			}

			flag = TableCode & 3;
			TableCode &= (~3);	// 0xFFFFFFFC

			if ((TableCode == 0)							||
				(HgMmIsAddressValid((PVOID)TableCode) == FALSE))
			{
				return;
			}
//			ProcessLog(("TableCode: 0x%p, flag: %d\n", TableCode, flag));
			switch(flag){
			case 0:
				ScanCidTable_Level_0(TableCode, pItp);
				break;
			case 1:
				ScanCidTable_Level_1(TableCode, pItp);
				break;
			case 2:
				ScanCidTable_Level_2(TableCode, pItp);
				break;
			default:
				ProcessLog(("Failed to scan CidTable!\n"));
			}
		}
	}
}

VOID
ScanCidTable_Level_0(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	)
{
	ULONG_PTR	Object = 0;
	ULONG_PTR	ItemCount = 511;

	do {
		// sizeof(_HANDLE_TABLE_ENTRY)
		TableAddr += 2 * sizeof(ULONG_PTR);
		if (HgMmIsAddressValid((PVOID)TableAddr) == TRUE)
		{
			Object = *(PULONG_PTR)TableAddr;
#if defined(_WIN64)
			if (g_WindowsVersion == eWin_8){
				Object = Object >> 19;
				Object &= (~0xF);
				Object |= 0xfffff00000000000;
			}else{
				Object &= (~7);	// 0xfffffff8;
			}
#else
			Object &= (~7);	// 0xfffffff8;
#endif
			//
			ThreadToProcess((PETHREAD)Object, pItp);
		}
	} while (--ItemCount > 0);
}

VOID
ScanCidTable_Level_1(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	)
{
	do {
		ScanCidTable_Level_0(*(PULONG_PTR)TableAddr, pItp);
		TableAddr += sizeof(ULONG_PTR);
	} while (HgMmIsAddressValid((PVOID)TableAddr) && *(PULONG_PTR)TableAddr != 0);
}

VOID
ScanCidTable_Level_2(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	)
{
	do {
		ScanCidTable_Level_1(*(PULONG_PTR)TableAddr, pItp);
		TableAddr += sizeof(ULONG_PTR);
	} while (HgMmIsAddressValid((PVOID)TableAddr) && *(PULONG_PTR)TableAddr != 0);
}

VOID
ThreadToProcess(
	__in	PETHREAD	pThread,
	__inout	PITP		pItp
	)
{
	if ((pThread != NULL)								&&
		(HgMmIsAddressValid((PVOID)pThread)) == TRUE	&&
		(PVOID)pThread > MM_SYSTEM_RANGE_START)
	{
		POBJECT_TYPE pObjectType = HgObGetObjectType((PVOID)pThread);

		if (pObjectType != NULL && pObjectType == (POBJECT_TYPE)g_hg_map[eHG_PsThreadType])
		{
			PEPROCESS pEProcess1 = NULL;
			PEPROCESS pEProcess2 = NULL;

			ULONG_PTR ApcState = GetOffset(_KTHREAD_ApcState);
			ULONG_PTR Process = GetOffset(_KAPC_STATE_Process);
			//ProcessLog(("pObjectType: 0x%p, pThread: 0x%p\n", pObjectType, pThread));

			pEProcess1 = HgIoThreadToProcess(pThread);
			if (pEProcess1 != NULL)
			{
				if (IsRealProcess(pEProcess1))
				{
					InsertProcessToBuffer(pEProcess1, pItp);
				}
			}

			pEProcess2 = *(PEPROCESS*)((ULONG_PTR)pThread + ApcState + Process);
			if (pEProcess2 != NULL)
			{
				if (IsRealProcess(pEProcess2))
				{
					InsertProcessToBuffer(pEProcess2, pItp);
				}
			}
		}
	}
}

VOID
InsertProcessToBuffer(
	__in	PEPROCESS	pEProcess,
	__in	PITP		pItp
	)
{
	if ((pEProcess == NULL)	||
		(pItp == NULL))
	{
		return;
	}
	if ((HgMmIsAddressValid(pEProcess) == TRUE)	&&
		(IsRealProcess(pEProcess) == TRUE))
	{
		if (IsProcessExist(pEProcess, pItp) == FALSE)
		{
			if (pItp->process.nCount > pItp->process.nRealCount)
			{
				size_t InheritedFromUniqueProcessIdOffset = 
					(size_t)g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS];
				size_t pid = *(size_t*)((size_t)pEProcess + g_hg_map[eHG_ObjectTableOffset_EPROCESS]);
				pid = *(size_t*)(pid + g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]);
				if (pid	!= 0)
				{
					pItp->process.pProcess[pItp->process.nRealCount].nPid = pid;
				}
				else
				{
					if (HgPsGetProcessId(pEProcess) != NULL)
					{
						pItp->process.pProcess[pItp->process.nRealCount].nPid = (size_t)HgPsGetProcessId(pEProcess);
					}else{
						pItp->process.pProcess[pItp->process.nRealCount].nPid =
							*(size_t*)((size_t)pEProcess + g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]);
					}
				}
				pItp->process.pProcess[pItp->process.nRealCount].nParentPid =
					*(size_t*)((size_t)pEProcess + InheritedFromUniqueProcessIdOffset);
				pItp->process.pProcess[pItp->process.nRealCount].pEProcess = (ULONG_PTR)pEProcess;
				GetProcessImagePathByEProcess(pEProcess, pItp->process.pProcess[pItp->process.nRealCount].pwszPath);
			}
			pItp->process.nRealCount ++;
		}
	}
	else{
		ProcessLog(("[Log]^InsertProcessToBuffer: pEProcess is invalid or pEProcess is dead.\n"));
	}
}

BOOL
IsRealProcess(
	__in	PEPROCESS pEProcess
	)
{
	POBJECT_TYPE	pObjectType = NULL;
	ULONG_PTR		ObjectTypeAddress = 0;
	BOOL			bRtn = FALSE;
	POBJECT_TYPE	ProcessType = (POBJECT_TYPE)g_hg_map[eHG_PsProcessType];
	if ((ProcessType != NULL)						&&
		(pEProcess != NULL)							&&
		(HgMmIsAddressValid((PVOID)pEProcess) == TRUE))
	{
		pObjectType = HgObGetObjectType((PVOID)pEProcess);
//		KdPrint(("pEProcess = 0x%p, ProcessType = 0x%p, pObjectType = 0x%p\n", pEProcess, ProcessType, pObjectType));

		if ((pObjectType != NULL)				&&
			(ProcessType == pObjectType)		&&
			(IsProcessDead(pEProcess) == FALSE))
		{
			bRtn = TRUE;
		}
	}
	return bRtn;
}

BOOL
IsProcessDead(
	__in	PEPROCESS	pEProcess
	)
{
	BOOL bRtn = FALSE;
	size_t ObjectTableOffset = 0;
	ObjectTableOffset = (size_t)g_hg_map[eHG_ObjectTableOffset_EPROCESS];

	if ((pEProcess != NULL)							&&
		(HgMmIsAddressValid(pEProcess) == TRUE)		&&
		(HgMmIsAddressValid((PVOID)((size_t)pEProcess + ObjectTableOffset)) == TRUE))
	{
		PVOID pObjectTable = *(PVOID*)((size_t)pEProcess + ObjectTableOffset);
		if ((pObjectTable == NULL)					||
			(HgMmIsAddressValid(pObjectTable) == FALSE))
		{
			ProcessLog(("Process(EPROCESS Address: 0x%p) is dead!\n", pEProcess));
			bRtn = TRUE;
		}
	}else{
		bRtn = TRUE;
	}

	return bRtn;
}

BOOL
IsProcessExist(
	__in	PEPROCESS	pEProcess,
	__in	PITP		pItp
	)
{
	BOOL bRtn = FALSE;
	size_t i = 0;

	if ((pEProcess == NULL)	||
		(pItp == NULL))
	{
		return TRUE;
	}

	for (i = 0; i < pItp->process.nCount; i++)
	{
		if (pItp->process.pProcess[i].pEProcess == (ULONG_PTR)pEProcess)
		{
			bRtn = TRUE;
			break;
		}
	}

	return bRtn;
}

BOOL
GetProcessImagePathBy_SE_AUDIT_PROCESS_CREATION_INFO_64(
	__in	PEPROCESS	pEProcess,
	__out	PWCHAR		pwszPath
	)
{
	BOOL bRtn = FALSE;
	ULONG_PTR nLen = 0;
	PUNICODE_STRING name = (PUNICODE_STRING)*(PULONG_PTR)((ULONG_PTR)pEProcess + GetOffset(_EPROCESS_SeAuditProcessCreationInfo));
	//ProcessLog(("GetProcessImagePathBy_SE_AUDIT_PROCESS_CREATION_INFO_64, pEProcess: 0x%p, name: 0x%p\n", pEProcess, name));
	if (IsUnicodeStringValid(name) == TRUE){
		nLen = GetUnicodeStringSafeLength(name, SZ_PATH_LEN);
		wcsncpy(pwszPath, name->Buffer, nLen);
		//ProcessLog(("-----------------------------%S\n", pwszPath));
		bRtn = TRUE;
	}

	return bRtn;
}

VOID EnumProcessByThreadList(PITP pItp)
{
	SIZE_T WaitListHead = g_hg_map[eHG_WaitListHead];
	SIZE_T DispatcherReadyListHead = g_hg_map[eHG_DispatcherReadyListHead];

	if (g_hg_map[eHG_WaitListHead] == 0)
	{
		GetWaitListHead_DispatcherReadyListHead();
		WaitListHead = g_hg_map[eHG_WaitListHead];
		DispatcherReadyListHead = g_hg_map[eHG_DispatcherReadyListHead];
		ProcessLog(("WaitListHead = 0x%p, DispatcherReadyListHead = 0x%p\n", WaitListHead, DispatcherReadyListHead));
	}

	if (WaitListHead != 0)
	{
		TraverseThreadList((PLIST_ENTRY)WaitListHead, pItp);
	}

	if (DispatcherReadyListHead != 0)
	{
		if (g_WindowsVersion == eWin_2000)
		{
		}else{
			int i = 0;
			for (i = 0; i < 32; i++)
			{
				TraverseThreadList((PLIST_ENTRY)DispatcherReadyListHead, pItp);
				DispatcherReadyListHead = (SIZE_T)DispatcherReadyListHead + sizeof(LIST_ENTRY);
			}
		}
	}
}

VOID GetWaitListHead_DispatcherReadyListHead()
{
	switch (g_WindowsVersion)
	{
	case eWin_2000:
		break;
	case eWin_XP:
		{
			SIZE_T OldBase = g_hg_map[eHG_CurrentKernelBase];
			SIZE_T NewBase = g_hg_map[eHG_NewKernelBase];
			SIZE_T Base = g_hg_map[eHG_ReloadNtosKrnlSuccess] ? NewBase : OldBase;
			PLIST_ENTRY WaitListHead = NULL;

			PVOID pProc = HgMiFindExportedRoutineByName((PVOID)Base, "KeDelayExecutionThread");

			SIZE_T nBegin = (SIZE_T)pProc;
			SIZE_T nEnd = (SIZE_T)pProc + PAGE_SIZE;

//			ProcessLog(("----------------Reload = %d, Base = 0x%p, pProc = 0x%p\n", g_hg_map[eHG_ReloadNtosKrnlSuccess], Base, pProc));

			if (pProc)
			{
				for (; nBegin < nEnd; nBegin ++)
				{
					if (HgMmIsAddressValid((PVOID)nBegin))
					{
						if (*(PUSHORT)nBegin = 0x03C7 && *(PUSHORT)(nBegin + 6) == 0x4389)
						{
							WaitListHead = *(PLIST_ENTRY*)(nBegin + 2);
							break;
						}
					}
				}
			}
			g_hg_map[eHG_WaitListHead] = (SIZE_T)WaitListHead;
		}
		break;
	case eWin_2003:
	case eWin_2003_SP1_SP2:
	case eWin_Vista:
	case eWin_Vista_SP1_SP2:
	case eWin_7:
	case eWin_8:
		{
			PLIST_ENTRY WaitListHead = NULL;
			PLIST_ENTRY DispatcherReadyListHead = NULL;
			SIZE_T Kprcb = 0;
#if defined(_WIN64)
			Kprcb = __readgsqword(0x20);
#else
			__asm{
				push eax
				mov eax, fs:[0x20]
				mov Kprcb, eax
				pop eax
			}
#endif
			if (Kprcb)
			{
				WaitListHead = (PLIST_ENTRY)(Kprcb + GetOffset(_KPRCB_WaitListHead));
				DispatcherReadyListHead = (PLIST_ENTRY)(Kprcb + GetOffset(_KPRCB_DispatcherReadyListHead));
			}

			g_hg_map[eHG_WaitListHead] = (SIZE_T)WaitListHead;
			g_hg_map[eHG_DispatcherReadyListHead] = (SIZE_T)DispatcherReadyListHead;
		}
		break;
	}
}

VOID TraverseThreadList(PLIST_ENTRY	pList, PITP pItp)
{
	SIZE_T WaitListEntryOffset = GetOffset(_KTHREAD_WaitListEntry);

	if (pList && HgMmIsAddressValid((PVOID)pList))
	{
		PLIST_ENTRY pEntry = pList->Flink;
		SIZE_T nMax = PAGE_SIZE;

		while (HgMmIsAddressValid(pEntry) && pEntry != pList && --nMax)
		{
			SIZE_T pThread = (SIZE_T)pEntry - WaitListEntryOffset;
			ThreadToProcess((PETHREAD)pThread, pItp);
		}
	}
}

/************************************************************************/
/* Enumerate modules of the target process                              */
/************************************************************************/
NTSTATUS EnumProcessModule(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pRtnSize
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	PITP		pOutItp = (PITP)pOutBuffer;

	SIZE_T		nProcessId = pInItp->in_process_module.nProcessId;
	PEPROCESS	pEProcess = (PEPROCESS)pInItp->in_process_module.pEProcess;
	PEPROCESS	pTmpEProcess = NULL;

	ProcessLog(("[hgprocess.c].[EnumProcessModule]: nProcessId = %d, pEProcess = 0x%p, pOutBuffer = 0x%p, nOutSize = 0x%p, nCount = %d, nMaxCount = %d\n",
		nProcessId, pEProcess, pOutBuffer, nOutSize, pOutItp->out_process_module.nCount, pOutItp->out_process_module.nMaxCount));

	if (pInBuffer == NULL || pOutBuffer == NULL){
		return status;
	}

	status = HgPsLookupProcessByProcessId((HANDLE)nProcessId, &pTmpEProcess);

	if (IsRealProcess(pEProcess)){
		// Because attach process in EnumProcessModule_PEB, ProbeForRead must read current process space.
		PITP pGlobalItp = (PITP)HgExAllocatePoolWithTag(PagedPool, nOutSize, HG_MEM_TAG);
		ProcessLog(("[hgprocess.c].[EnumProcessModule]: Enumerate Module by peb!\n"));
		if (pGlobalItp == NULL){
			return STATUS_UNSUCCESSFUL;
		}
		RtlZeroMemory(pGlobalItp, nOutSize);
		pGlobalItp->out_process_module.nMaxCount = pOutItp->out_process_module.nMaxCount;
		EnumProcessModule_PEB(pEProcess, pGlobalItp);

		if (pGlobalItp->out_process_module.nCount > pGlobalItp->out_process_module.nMaxCount){
			pOutItp->out_process_module.nCount = pGlobalItp->out_process_module.nCount;
		}else{
			memcpy(pOutItp, pGlobalItp, nOutSize);
		}
		HgExFreePoolWithTag(pGlobalItp, HG_MEM_TAG);
		
		EnumProcessModule_VadRoot(pEProcess, pOutItp);
	}

	if (NT_SUCCESS(status)){
		HgObfDereferenceObject(pTmpEProcess);
	}

	return status;
}

VOID EnumProcessModule_PEB(PEPROCESS pEProcess, PITP pItp)
{
	BOOL bAttach = FALSE;
	KAPC_STATE ApcState;

	if (pEProcess == NULL || pItp == NULL || KeGetCurrentIrql() >= DISPATCH_LEVEL){
		ProcessLog(("[hgprocess.c].[EnumProcessModule_PEB]: Can not enter EnumProcessModule_PEB!\n"));
		return;
	}

	//
	// need < DISPATCH_LEVEL
	// 
	if (HgMmIsAddressValid((PVOID)((SIZE_T)pEProcess + GetOffset(_EPROCESS_Peb))) == FALSE)
	{
		return;
	}

	if (HgIoGetCurrentProcess() != pEProcess)
	{
		HgKeStackAttachProcess(pEProcess, &ApcState);
		bAttach = TRUE;
	}

	__try{
		PHG_PEB peb = *(PHG_PEB*)((SIZE_T)pEProcess + GetOffset(_EPROCESS_Peb));
		if (peb != NULL && (SIZE_T)peb < (SIZE_T)MM_HIGHEST_USER_ADDRESS)
		{
			PPEB_LDR_DATA pLdr = NULL;
			HgProbeForRead(peb, sizeof(HG_PEB), 1);
			HgProbeForRead(peb->Ldr, sizeof(PPEB_LDR_DATA), 1);

			pLdr = (PPEB_LDR_DATA)(peb->Ldr);
			if (pLdr != NULL && (SIZE_T)pLdr < (SIZE_T)MM_HIGHEST_USER_ADDRESS)
			{
				TraverseModuleList(&pLdr->InLoadOrderModuleList, LoadOrderLinks, pItp);
				TraverseModuleList(&pLdr->InMemoryOrderModuleList, MemoryOrderLinks, pItp);
				TraverseModuleList(&pLdr->InInitializationOrderModuleList, InitializationOrderLinks, pItp);
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[EnumProcessModule_PEB]: Catch an exception!\n"));
	}

	if (bAttach){
		HgKeUnstackDetachProcess(&ApcState);
		bAttach = FALSE;
	}
}

VOID TraverseModuleList(PLIST_ENTRY pList, SIZE_T nType, PITP pItp)
{
	PLIST_ENTRY entry = pList->Flink;

	if (pList == NULL || pItp == NULL){
		return;
	}

	while (entry != NULL && (SIZE_T)entry < (SIZE_T)MM_HIGHEST_USER_ADDRESS && entry != pList)
	{
		PLDR_DATA_TABLE_ENTRY pLdrEntry = NULL;

		switch (nType){
		case 0:
			pLdrEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);
			break;
		case 1:
			pLdrEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InMemoryOrderLinks);
			break;
		case 2:
			pLdrEntry = CONTAINING_RECORD(entry, LDR_DATA_TABLE_ENTRY, InInitializationOrderLinks);
			break;
		}

		if (pLdrEntry != NULL && (SIZE_T)pLdrEntry < (SIZE_T)MM_HIGHEST_USER_ADDRESS)
		{
			__try{
				HgProbeForRead(pLdrEntry, sizeof(LDR_DATA_TABLE_ENTRY), 1);

				if (IsModuleExist((SIZE_T)pLdrEntry->DllBase, (SIZE_T)pLdrEntry->SizeOfImage, pItp) == FALSE)
 				{
					if (pItp->out_process_module.nCount < pItp->out_process_module.nMaxCount)
					{
						SIZE_T nLen = GetUnicodeStringSafeLength(&pLdrEntry->FullDllName, SZ_PATH_LEN);
						pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = (SIZE_T)pLdrEntry->DllBase;
						pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = (SIZE_T)pLdrEntry->SizeOfImage;

						HgProbeForRead(pLdrEntry->FullDllName.Buffer, nLen * sizeof(WCHAR), sizeof(WCHAR));
						wcsncpy(pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path, pLdrEntry->FullDllName.Buffer, nLen);
					}
					pItp->out_process_module.nCount ++;
 				}
			}__except(EXCEPTION_EXECUTE_HANDLER){
				ProcessLog(("[hgprocess.c].[TraverseModuleList]: Catch an exception!\n"));
			}
		}
		entry = entry->Flink;
	}
}

BOOL IsModuleExist(SIZE_T Base, SIZE_T Size, PITP pItp)
{
	BOOL bRtn = FALSE;
	SIZE_T i = 0;
	SIZE_T nCount = pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount ? 
		pItp->out_process_module.nCount : pItp->out_process_module.nMaxCount;

	if (Base == 0 || Size == 0 || pItp == NULL){
		ProcessLog(("[hgprocess.c].[IsModuleExist]: Base == 0 || Size == 0 || pItp == NULL\n"));
		return TRUE;
	}
	
 	for (i = 0; i < nCount; i++)
 	{
 		if (pItp->out_process_module.pModule[i].Base == Base && pItp->out_process_module.pModule[i].Size == Size)
 		{
 			bRtn = TRUE;
 			break;
 		}
 	}

	return bRtn;
}

VOID EnumProcessModule_VadRoot(PEPROCESS pEProcess, PITP pItp)
{
	SIZE_T Vad = 0;
	if (pEProcess == NULL || pItp == NULL){
		return;
	}
	//
	// Get VAD
	// 
	switch (g_WindowsVersion)
	{
	case eWin_2000:
	case eWin_XP:
		{
			Vad = *(PSIZE_T)((SIZE_T)pEProcess + GetOffset(_EPROCESS_VadRoot));
		}
		break;
	case eWin_2003:
	case eWin_2003_SP1_SP2:
	case eWin_Vista:
	case eWin_Vista_SP1_SP2:
	case eWin_7:
	case eWin_8:
		{
			Vad = (SIZE_T)pEProcess + GetOffset(_EPROCESS_VadRoot);
		}
		break;
	}
	//
	// Parse VAD
	// 
	switch (g_WindowsVersion)
	{
	case eWin_2000:
		break;
	case eWin_XP:
		{
			PMMVAD_XP pVad_XP = (PMMVAD_XP)Vad;
			if (pVad_XP->NumberGenericTableElements != 0){
				TraverseVadTree_XP(pVad_XP, pItp);
			}
		}
		break;
	case eWin_2003:
	case eWin_2003_SP1_SP2:
		break;
	case eWin_Vista:
	case eWin_Vista_SP1_SP2:
	case eWin_7:
		{
#if defined(_WIN64)
			PMMVAD64_WIN7 pVad64_Win7 = (PMMVAD64_WIN7)Vad;
			TraverseVadTree64_Win7(pVad64_Win7, pItp);
#else
			PMMVAD32_WIN7 pVad_Win7 = (PMMVAD32_WIN7)Vad;
			if (pVad_Win7->NumberGenericTableElements != 0){
				TraverseVadTree32_Win7(pVad_Win7, pItp);
			}
#endif
		}
		break;
	case eWin_8:
		{
#if defined(_WIN64)
			PMMVAD64_WIN8 pVad64_WIN8 = (PMMVAD64_WIN8)Vad;
			TraverseVadTree64_Win8(pVad64_WIN8, pItp);
#else
			PMMVAD_WIN8 pVad_Win8 = (PMMVAD_WIN8)Vad;
			TraverseVadTree32_Win8(pVad_Win8, pItp);
#endif
		}
		break;
	}
}

VOID TraverseVadTree_XP(PMMVAD_XP root, PITP pItp)
{
	if ((SIZE_T)root < (SIZE_T)MM_SYSTEM_RANGE_START || (HgMmIsAddressValid((PVOID)root)) == FALSE){
		ProcessLog(("[hgprocess.c].[TraverseVadTree]: Error 1!\n"));
		return;
	}

	if (root->RightChild){
		TraverseVadTree_XP(root->RightChild, pItp);
	}

	__try{
		if ((SIZE_T)root->ControlArea > (SIZE_T)MM_SYSTEM_RANGE_START	&&
			HgMmIsAddressValid((PVOID)root->ControlArea)					&&
			(SIZE_T)(root->ControlArea->FilePointer) > (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			PFILE_OBJECT pFileObject = (PFILE_OBJECT)(root->ControlArea->FilePointer);
			// FILE_OBJECT.Type
			// A read-only member used by the system to indicate that the object is a file object. 
			// If the object is a file object, the value of this member is 5.
			if (HgMmIsAddressValid(pFileObject) && pFileObject->Type == 5)
			{
				if (NT_SUCCESS(HgObReferenceObjectByPointer(pFileObject, 0, NULL, KernelMode)))
				{
					SIZE_T Base = (SIZE_T)(root->ControlArea->Segment->BasedAddress);
					if (Base > 0 && IsModuleExist(root->StartingVpn << 12, root->ControlArea->Segment->SizeOfSegment.LowPart, pItp) == FALSE)
					{
						if (pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount)
						{
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = root->StartingVpn << 12;
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = root->ControlArea->Segment->SizeOfSegment.LowPart;
							GetProcessImagePathByFileObject(pFileObject, pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path);
						}
						HgObfDereferenceObject(pFileObject);
						pItp->out_process_module.nCount ++;
					}
				}
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[TraverseVadTree]: Catch exception!\n"));
	}

	if (root->LeftChild){
		TraverseVadTree_XP(root->LeftChild, pItp);
	}
}

VOID TraverseVadTree32_Win7(PMMVAD32_WIN7 root, PITP pItp)
{
	if ((SIZE_T)root < (SIZE_T)MM_SYSTEM_RANGE_START || (HgMmIsAddressValid((PVOID)root)) == FALSE){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Error 1!\n"));
		return;
	}

	if (root->RightChild){
		TraverseVadTree32_Win7(root->RightChild, pItp);
	}

	__try{
		if ((SIZE_T)root->Subsection > (SIZE_T)MM_SYSTEM_RANGE_START					&&
			HgMmIsAddressValid((PVOID)root->Subsection)								&&
			(SIZE_T)(root->Subsection->ControlArea) > (SIZE_T)MM_SYSTEM_RANGE_START	&&
			HgMmIsAddressValid(root->Subsection->ControlArea)						&&
			(SIZE_T)(root->Subsection->ControlArea->FilePointer) > (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			PFILE_OBJECT pFileObject = (PFILE_OBJECT)((SIZE_T)(root->Subsection->ControlArea->FilePointer) & (SIZE_T)0xFFFFFFFFFFFFFFF8);
			// FILE_OBJECT.Type
			// A read-only member used by the system to indicate that the object is a file object. 
			// If the object is a file object, the value of this member is 5.
			if ((SIZE_T)pFileObject > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid(pFileObject) && pFileObject->Type == 5)
			{
				if (NT_SUCCESS(HgObReferenceObjectByPointer(pFileObject, 0, NULL, KernelMode)))
				{
					SIZE_T Base = (SIZE_T)(root->Subsection->ControlArea->Segment->BasedAddress);
					if (Base > 0 && IsModuleExist(root->StartingVpn << 12, root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart, pItp) == FALSE){
						if (pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount)
						{
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = root->StartingVpn << 12;
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart;
							GetProcessImagePathByFileObject(pFileObject,
								pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path);
						}
						HgObfDereferenceObject(pFileObject);
						pItp->out_process_module.nCount ++;
					}
				}
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Catch exception!\n"));
	}

	if (root->LeftChild){
		TraverseVadTree32_Win7(root->LeftChild, pItp);
	}
}

VOID TraverseVadTree32_Win8(PMMVAD_WIN8 root, PITP pItp)
{
	if ((SIZE_T)root < (SIZE_T)MM_SYSTEM_RANGE_START || (HgMmIsAddressValid((PVOID)root)) == FALSE){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Error 1!\n"));
		return;
	}

	if (root->RightChild){
		TraverseVadTree32_Win8(root->RightChild, pItp);
	}

	__try{
		if ((SIZE_T)root->Subsection > (SIZE_T)MM_SYSTEM_RANGE_START					&&
			HgMmIsAddressValid((PVOID)root->Subsection)								&&
			(SIZE_T)(root->Subsection->ControlArea) > (SIZE_T)MM_SYSTEM_RANGE_START	&&
			HgMmIsAddressValid(root->Subsection->ControlArea)						&&
			(SIZE_T)(root->Subsection->ControlArea->FilePointer) > (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			PFILE_OBJECT pFileObject = (PFILE_OBJECT)((SIZE_T)(root->Subsection->ControlArea->FilePointer) & (SIZE_T)0xFFFFFFFFFFFFFFF8);
			// FILE_OBJECT.Type
			// A read-only member used by the system to indicate that the object is a file object. 
			// If the object is a file object, the value of this member is 5.
			if ((SIZE_T)pFileObject > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid(pFileObject) && pFileObject->Type == 5)
			{
				if (NT_SUCCESS(HgObReferenceObjectByPointer(pFileObject, 0, NULL, KernelMode)))
				{
					SIZE_T Base = (SIZE_T)(root->Subsection->ControlArea->Segment->BasedAddress);
					if (Base > 0 && IsModuleExist(root->StartingVpn << 12, root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart, pItp) == FALSE){
						if (pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount)
						{
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = root->StartingVpn << 12;
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart;
							GetProcessImagePathByFileObject(pFileObject,
								pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path);
						}
						HgObfDereferenceObject(pFileObject);
						pItp->out_process_module.nCount ++;
					}
				}
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Catch exception!\n"));
	}

	if (root->LeftChild){
		TraverseVadTree32_Win8(root->LeftChild, pItp);
	}
}

VOID TraverseVadTree64_Win7(PMMVAD64_WIN7 root, PITP pItp)
{
	if ((SIZE_T)root < (SIZE_T)MM_SYSTEM_RANGE_START || (HgMmIsAddressValid((PVOID)root)) == FALSE){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Error 1!\n"));
		return;
	}

	if (root->RightChild){
		TraverseVadTree64_Win7(root->RightChild, pItp);
	}

	__try{
		if ((SIZE_T)root->Subsection > (SIZE_T)MM_SYSTEM_RANGE_START					&&
			HgMmIsAddressValid((PVOID)root->Subsection)								&&
			(SIZE_T)(root->Subsection->ControlArea) > (SIZE_T)MM_SYSTEM_RANGE_START	&&
			HgMmIsAddressValid(root->Subsection->ControlArea)						&&
			(SIZE_T)(root->Subsection->ControlArea->FilePointer.Object) > (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			PFILE_OBJECT pFileObject = (PFILE_OBJECT)((SIZE_T)(root->Subsection->ControlArea->FilePointer.Object) & (SIZE_T)0xFFFFFFFFFFFFFFF0);
			// FILE_OBJECT.Type
			// A read-only member used by the system to indicate that the object is a file object. 
			// If the object is a file object, the value of this member is 5.
			if ((SIZE_T)pFileObject > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid(pFileObject) && pFileObject->Type == 5)
			{
				if (NT_SUCCESS(HgObReferenceObjectByPointer(pFileObject, 0, NULL, KernelMode)))
				{
					SIZE_T Base = (SIZE_T)(root->Subsection->ControlArea->Segment->BasedAddress);
					if (Base > 0 && IsModuleExist(((SIZE_T)(root->StartingVpn)) << 12, root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart, pItp) == FALSE){
						if (pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount)
						{
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = ((SIZE_T)(root->StartingVpn)) << 12;
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = (SIZE_T)(root->Subsection->ControlArea->Segment->SizeOfSegment.QuadPart);
							GetProcessImagePathByFileObject(pFileObject, pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path);
						}
						HgObfDereferenceObject(pFileObject);
						pItp->out_process_module.nCount ++;
					}
				}
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Catch exception!\n"));
	}

	if (root->LeftChild){
		TraverseVadTree64_Win7(root->LeftChild, pItp);
	}
}

VOID TraverseVadTree64_Win8(PMMVAD64_WIN8 root, PITP pItp)
{
	if ((SIZE_T)root < (SIZE_T)MM_SYSTEM_RANGE_START || (HgMmIsAddressValid((PVOID)root)) == FALSE){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Error 1!\n"));
		return;
	}

	if (root->Core.VadNode.RightChild){
		TraverseVadTree64_Win8((PMMVAD64_WIN8)root->Core.VadNode.RightChild, pItp);
	}

	__try{
		if ((SIZE_T)root->Subsection > (SIZE_T)MM_SYSTEM_RANGE_START					&&
			HgMmIsAddressValid((PVOID)root->Subsection)								&&
			(SIZE_T)(root->Subsection->ControlArea) > (SIZE_T)MM_SYSTEM_RANGE_START	&&
			HgMmIsAddressValid(root->Subsection->ControlArea)						&&
			(SIZE_T)(root->Subsection->ControlArea->FilePointer.Object) > (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			PFILE_OBJECT pFileObject = (PFILE_OBJECT)((SIZE_T)(root->Subsection->ControlArea->FilePointer.Object) & (SIZE_T)0xFFFFFFFFFFFFFFF0);
			// FILE_OBJECT.Type
			// A read-only member used by the system to indicate that the object is a file object. 
			// If the object is a file object, the value of this member is 5.
			if ((SIZE_T)pFileObject > (SIZE_T)MM_SYSTEM_RANGE_START && HgMmIsAddressValid(pFileObject) && pFileObject->Type == 5)
			{
				if (NT_SUCCESS(HgObReferenceObjectByPointer(pFileObject, 0, NULL, KernelMode)))
				{
					SIZE_T Base = (SIZE_T)(root->Subsection->ControlArea->Segment->BasedAddress);
					if (Base > 0 && IsModuleExist(((SIZE_T)(root->Core.StartingVpn)) << 12, root->Subsection->ControlArea->Segment->SizeOfSegment.LowPart, pItp) == FALSE)
					{
						if (pItp->out_process_module.nMaxCount > pItp->out_process_module.nCount)
						{
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Base = ((SIZE_T)(root->Core.StartingVpn)) << 12;
							pItp->out_process_module.pModule[pItp->out_process_module.nCount].Size = (SIZE_T)(root->Subsection->ControlArea->Segment->SizeOfSegment.QuadPart);
							GetProcessImagePathByFileObject(pFileObject, pItp->out_process_module.pModule[pItp->out_process_module.nCount].Path);
						}
						HgObfDereferenceObject(pFileObject);
						pItp->out_process_module.nCount ++;
					}					
				}
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		ProcessLog(("[hgprocess.c].[TraverseVadTree_Win7]: Catch exception!\n"));
	}

	if (root->Core.VadNode.LeftChild){
		TraverseVadTree64_Win8((PMMVAD64_WIN8)root->Core.VadNode.LeftChild, pItp);
	}
}

NTSTATUS EnumProcessThread(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pRtnSize)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	NTSTATUS	TmpStatus = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	PITP		pOutItp = (PITP)pOutBuffer;

	SIZE_T		nProcessId = pInItp->in_process_module.nProcessId;
	PEPROCESS	pEProcess = (PEPROCESS)pInItp->in_process_module.pEProcess;
	PEPROCESS	pTmpEProcess = NULL;

	ProcessLog(("[hgprocess.c].[EnumProcessThread]: Enter!\n"));
	if (pInBuffer == NULL || pOutBuffer == NULL || pRtnSize == NULL){
		ProcessLog(("[hgprocess.c].[EnumProcessThread]: Incorrect parameters!\n"));
		return STATUS_UNSUCCESSFUL;
	}

	TmpStatus = HgPsLookupProcessByProcessId((HANDLE)nProcessId, &pTmpEProcess);

	if (IsRealProcess(pEProcess)){
		EnumProcessThread_ThreadList(pEProcess, pOutItp);
		EnumProcessThread_PspCidTable(pEProcess, pOutItp);
//		EnumProcessThread_WaitThreadList(pEProcess, pOutItp);
		status = STATUS_SUCCESS;
		*pRtnSize = nOutSize;
	}

	if (NT_SUCCESS(TmpStatus)){
		HgObfDereferenceObject(pTmpEProcess);
	}

	return status;
}

VOID EnumProcessThread_ThreadList(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	PLIST_ENTRY pList = NULL;
	PLIST_ENTRY pEntry = NULL;
	PETHREAD pEThread = NULL;

	ProcessLog(("[hgprocess.c].[EnumProcessThread_ThreadList]: Enter!\n"));
	if (pEProcess == NULL || pItp == NULL){
		ProcessLog(("[hgprocess.c].[EnumProcessThread_ThreadList]: Incorrect parameters!\n"));
		return;
	}

	pList = (PLIST_ENTRY)((SIZE_T)pEProcess + GetOffset(_KPROCESS_ThreadListHead));
	if (pList && HgMmIsAddressValid(pList) && HgMmIsAddressValid(pList->Flink))
	{
		pEntry = pList->Flink;
		while (HgMmIsAddressValid(pEntry) && pEntry != pList)
		{
			pEThread = (PETHREAD)((SIZE_T)pEntry - GetOffset(_KTHREAD_ThreadListEntry));
			AddThreadDataToItp(pEThread, pEProcess, pItp);
			pEntry = pEntry->Flink;
		}
	}

	pList = (PLIST_ENTRY)((SIZE_T)pEProcess + GetOffset(_EPROCESS_ThreadListHead));
	if (pList && HgMmIsAddressValid(pList) && HgMmIsAddressValid(pList->Flink))
	{
		pEntry = pList->Flink;
		while (HgMmIsAddressValid(pEntry) && pEntry != pList)
		{
			pEThread = (PETHREAD)((SIZE_T)pEntry - GetOffset(_ETHREAD_ThreadListEntry));
			AddThreadDataToItp(pEThread, pEProcess, pItp);
			pEntry = pEntry->Flink;
		}
	}
}

VOID EnumProcessThread_PspCidTable(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	SIZE_T PspCidTable = g_hg_map[eHG_PspCidTable];
	if (pEProcess == NULL || pItp == NULL)
	{
		ProcessLog(("[hgprocess.c].[EnumProcessThread_PspCidTable]: Incorrect parameters!\n"));
		return;
	}

	if (PspCidTable == 0){
		GetPspCidTable();
		PspCidTable = g_hg_map[eHG_PspCidTable];
	}

	if (PspCidTable == 0 || HgMmIsAddressValid((PVOID)PspCidTable) == FALSE)
	{
		return;
	}

	if (g_WindowsVersion == eWin_2000){
	}else{
		ScanCidTableToFindThread(PspCidTable, pEProcess, pItp);
	}
}

VOID EnumProcessThread_WaitThreadList(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	SIZE_T WaitInListHead = g_hg_map[eHG_WaitListHead];
	SIZE_T DispatcherReadyListHead = g_hg_map[eHG_DispatcherReadyListHead];

	if (WaitInListHead == 0){
		GetWaitListHead_DispatcherReadyListHead();
		WaitInListHead = g_hg_map[eHG_WaitListHead];
		DispatcherReadyListHead = g_hg_map[eHG_DispatcherReadyListHead];
	}

	ProcessLog(("WaitInListHead = 0x%p, DispatcherReadyListHead = 0x%p\n", WaitInListHead, DispatcherReadyListHead));
	if (WaitInListHead != 0){
		TraverseProcessThreadList((PLIST_ENTRY)WaitInListHead, pEProcess, pItp);
	}

	if (DispatcherReadyListHead != 0){
		if (g_WindowsVersion == eWin_2000){
		}else{
			SIZE_T i = 0;
			for (i = 0; i < 32; i++){
				ProcessLog(("========================================3\n"));
				TraverseProcessThreadList((PLIST_ENTRY)DispatcherReadyListHead, pEProcess, pItp);
				DispatcherReadyListHead = (SIZE_T)DispatcherReadyListHead + sizeof(LIST_ENTRY);
			}
		}
	}
}

VOID AddThreadDataToItp(
	__in	PETHREAD	pEThread,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	POBJECT_TYPE pObjectType = NULL;
	POBJECT_TYPE pThreadType = (POBJECT_TYPE)g_hg_map[eHG_PsThreadType];
	PEPROCESS pInnerEProcess = NULL;

	if (pEThread == NULL						||
		pEProcess == NULL						||
		pItp == NULL							||
		HgMmIsAddressValid(pEThread) == FALSE	||
		HgMmIsAddressValid(pEProcess) == FALSE)
	{
		ProcessLog(("[hgprocess.c].[AddThreadDataToItp]: Incorrect parameters!\n"));
		return;
	}

	pObjectType = HgObGetObjectType((PVOID)pEThread);

	if (pObjectType == pThreadType)
	{
		pInnerEProcess = HgIoThreadToProcess(pEThread);
		if (HgMmIsAddressValid((PVOID)pInnerEProcess) == FALSE)
		{
			return;
		}
		if (pInnerEProcess == NULL){
			// 6b-Bit _ETHREAD does not have ThreadsProcess
#if defined(_WIN64)
#else
			pInnerEProcess = (PEPROCESS)(*(PSIZE_T)(SIZE_T)pEThread + GetOffset(_ETHREAD_ThreadsProcess));
#endif
		}

		if (pInnerEProcess == pEProcess											&& 
			ContainThreadData(pEThread, pItp) == FALSE							&& 
			NT_SUCCESS(HgObReferenceObjectByPointer(pEThread, 0, NULL, KernelMode)))
		{
			if (pItp->out_process_thread.nMaxCount > pItp->out_process_thread.nCount)
			{
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Tid = (SIZE_T)HgPsGetThreadId(pEThread);
				if (pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Tid == 0)
				{
					pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Tid = *(PSIZE_T)((SIZE_T)pEThread + GetOffset(_ETHREAD_Cid) + sizeof(VOID*));
				}

				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].pEThread = (SIZE_T)pEThread;
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Win32StartAddress = *(PSIZE_T)((SIZE_T)pEThread + GetOffset(_ETHREAD_Win32StartAddress));
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].StartAddress = *(PSIZE_T)((SIZE_T)pEThread + GetOffset(_ETHREAD_StartAddress));
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Teb = *(PSIZE_T)((SIZE_T)pEThread + GetOffset(_KTHREAD_Teb));
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].Priority = *(PUCHAR)((SIZE_T)pEThread + GetOffset(_KTHREAD_Priority));
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].SwitchCount = *(PSIZE_T)((SIZE_T)pEThread + GetOffset(_KTHREAD_ContextSwitches));
				pItp->out_process_thread.pThread[pItp->out_process_thread.nCount].State = *(PUCHAR)((SIZE_T)pEThread + GetOffset(_KTHREAD_State));
			}

			pItp->out_process_thread.nCount ++;
			HgObfDereferenceObject(pEThread);
		}
	}
}

BOOL ContainThreadData(
	__in	PETHREAD pEThread, 
	__inout	PITP pItp)
{
	BOOL bRtn = FALSE;
	SIZE_T i = 0;
	SIZE_T nCount = pItp->out_process_thread.nMaxCount > pItp->out_process_thread.nCount ? pItp->out_process_thread.nCount : pItp->out_process_thread.nMaxCount;

	if (pEThread == NULL || pItp == NULL){
		return FALSE;
	}

	for (i = 0; i < nCount; i++)
	{
		if (pItp->out_process_thread.pThread[i].pEThread == (SIZE_T)pEThread)
		{
			bRtn = TRUE;
			break;
		}
	}

	return bRtn;
}

VOID ScanCidTableToFindThread(
	__in	SIZE_T		PspCidTable, 
	__in	PEPROCESS	pEProcess, 
	__inout	PITP		pItp)
{
	SIZE_T HandleTable = 0;
	SIZE_T TableCode = 0;
	SIZE_T Flag = 0;

	if (PspCidTable == 0 || pEProcess == NULL || pItp == NULL || HgMmIsAddressValid((PVOID)PspCidTable) == FALSE)
	{
		return;
	}

	HandleTable = *(PSIZE_T)PspCidTable;
	if (HandleTable == 0 || HgMmIsAddressValid((PVOID)HandleTable) == FALSE)
	{
		return;
	}

	if (g_WindowsVersion == eWin_8){
		TableCode = *(PSIZE_T)(HandleTable + 8);
	}else{
		TableCode = *(PSIZE_T)(HandleTable);
	}

	Flag = TableCode & 3;
	TableCode &= (~3);

	if (TableCode == 0 || HgMmIsAddressValid((PVOID)TableCode) == FALSE)
	{
		return;
	}
	switch (Flag)
	{
	case 0:
		ScanCidTableToFindThread_Level_0(TableCode, pEProcess, pItp);
		break;
	case 1:
		__try{
			ScanCidTableToFindThread_Level_1(TableCode, pEProcess, pItp);
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("[hgprocess.c].[ScanCidTableToFindThread]: Catch ScanCidTableToFindThread_Level_1 exception!\n"));
		}
		break;
	case 2:
		__try{
			ScanCidTableToFindThread_Level_2(TableCode, pEProcess, pItp);
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("[hgprocess.c].[ScanCidTableToFindThread]: Catch ScanCidTableToFindThread_Level_2 exception!\n"));
		}
		break;
	default:
		ProcessLog(("Scan PspCidTable to find thread infomation unsuccessfully!\n"));
	}
}

VOID ScanCidTableToFindThread_Level_0(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	SIZE_T	Object = 0;
	SIZE_T	ItemCount = 511;

	if (HgMmIsAddressValid(pEProcess) == FALSE){
		return;
	}
	do {
		TableCode += 2 * sizeof(SIZE_T);
		if (HgMmIsAddressValid((PVOID)TableCode) == TRUE)
		{
			Object = *(PSIZE_T)TableCode;
			if (HgMmIsAddressValid((PVOID)Object) == FALSE){
				continue;
			}
#if defined(_WIN64)
			if (g_WindowsVersion == eWin_8){
				Object = Object >> 19;
				Object &= (~0xF);
				Object |= 0xfffff00000000000;
			}else{
				Object &= (~7);
			}
#else
			Object &= (~7);
#endif
			AddThreadDataToItp((PETHREAD)Object, pEProcess, pItp);
		}
	} while (--ItemCount > 0);
}

VOID ScanCidTableToFindThread_Level_1(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	if (HgMmIsAddressValid((PVOID)TableCode) == FALSE	||
		HgMmIsAddressValid((PVOID)pEProcess) == FALSE)
	{
		return;
	}
	if (TableCode != 0 && HgMmIsAddressValid((PVOID)TableCode))
	{
		do {
			ScanCidTableToFindThread_Level_0(*(PSIZE_T)TableCode, pEProcess, pItp);
			TableCode += sizeof(SIZE_T);
		} while (HgMmIsAddressValid((PVOID)TableCode) && *(PSIZE_T)TableCode != 0);
	}
}

VOID ScanCidTableToFindThread_Level_2(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	if (TableCode != 0 && HgMmIsAddressValid((PVOID)TableCode))
	{
		do {
			ScanCidTableToFindThread_Level_1(*(PSIZE_T)TableCode, pEProcess, pItp);
			TableCode += sizeof(SIZE_T);
		} while (HgMmIsAddressValid((PVOID)TableCode) && *(PSIZE_T)TableCode != 0);
	}
}

VOID TraverseProcessThreadList(
	__in	PLIST_ENTRY	pList,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp)
{
	if (pList == NULL || pEProcess == NULL || pItp == NULL)
	{
		return;
	}

	if (HgMmIsAddressValid(pList) && HgMmIsAddressValid(pList->Flink))
	{
		PLIST_ENTRY pEntry = pList->Flink;
		while (HgMmIsAddressValid(pEntry) && pEntry != pList){
			SIZE_T pThread = (SIZE_T)pEntry - g_hg_map[eHG_WaitListHead];
			if (HgMmIsAddressValid((PVOID)pThread)){
				AddThreadDataToItp((PETHREAD)pThread, pEProcess, pItp);
			}
			pEntry = pEntry->Flink;
		}
	}
}

NTSTATUS EnumerateProcessMemoryBlocks(PEPROCESS pEProcess, PITP pItp)
{
	NTSTATUS status = 0;
	HANDLE hProcess = NULL;
	POBJECT_TYPE ProcessType = (POBJECT_TYPE)g_hg_map[eHG_PsProcessType];

	ProcessLog(("%s(%d): Enter EnumerateProcessMemoryBlocks!\n", __FILE__, __LINE__));
	if (!pEProcess || !pItp)
	{
		return STATUS_UNSUCCESSFUL;
	}

	status = HgObOpenObjectByPointer(pEProcess, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL, ProcessType, KernelMode, &hProcess);
	if (NT_SUCCESS(status))
	{
		SIZE_T Base = 0;

		while (Base < (SIZE_T)MM_HIGHEST_USER_ADDRESS)
		{
			MEMORY_BASIC_INFORMATION mbi = {0};
			SIZE_T ReturnLength = 0;
			status = HgNtQueryVirtualMemory(hProcess, (PVOID)Base, MemoryBasicInformation, &mbi, sizeof(MEMORY_BASIC_INFORMATION), &ReturnLength);
			if (NT_SUCCESS(status))
			{
				if (pItp->out_process_memory.nMaxCount > pItp->out_process_memory.nCount)
				{
					pItp->out_process_memory.MemoryBlocks[pItp->out_process_memory.nCount].Base = Base;
					pItp->out_process_memory.MemoryBlocks[pItp->out_process_memory.nCount].Size = mbi.RegionSize;
					pItp->out_process_memory.MemoryBlocks[pItp->out_process_memory.nCount].Protect = mbi.Protect;
					pItp->out_process_memory.MemoryBlocks[pItp->out_process_memory.nCount].State = mbi.State;
					pItp->out_process_memory.MemoryBlocks[pItp->out_process_memory.nCount].Type = mbi.Type;
				}
				pItp->out_process_memory.nCount ++;
				Base += mbi.RegionSize;
			}
			else
			{
				ProcessLog(("Error code = %08X\n", status));
				Base += PAGE_SIZE;
			}
		}
		HgNtClose(hProcess);
	}

	return STATUS_SUCCESS;
}

NTSTATUS EnumProcessMemory(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize,
	__out	ULONG*	pReturnLength
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	PITP		pOutItp = (PITP)pOutBuffer;

	PEPROCESS	pEProcess = NULL;
	PEPROCESS	pTempEProcess = NULL;
	NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;
	ProcessLog(("%s(%d): Enter EnumProcessMemory!\n", __FILE__, __LINE__));
	if (pInItp && pOutItp)
	{
		pEProcess = (PEPROCESS)pInItp->in_process_memory.pEProcess;
		TempStatus = HgPsLookupProcessByProcessId((HANDLE)pInItp->in_process_memory.nPId, &pTempEProcess);
		if (IsRealProcess(pEProcess))
		{
			status = EnumerateProcessMemoryBlocks(pEProcess, pOutItp);
			if (pReturnLength)
			{
				*pReturnLength = nOutSize;
			}
		}

		if (NT_SUCCESS(TempStatus))
		{
			HgObfDereferenceObject(pTempEProcess);
		}
	}

	status = STATUS_SUCCESS;
	return status;
}

BOOL ZeroMemoryAux(PVOID Base, SIZE_T Size, KPROCESSOR_MODE AccessMode)
{
	BOOL bRtn = FALSE;

	SIZE_T nLast = 0;
	PMDL pMdl = NULL;
	PVOID pMappedAddress = NULL;
	KIRQL OldIrql;
	
	nLast = (SIZE_T)Base & 0xFFF;
	pMdl = HgIoAllocateMdl((PVOID)((SIZE_T)Base & (~0xFFF)), (ULONG)(nLast + Size + 0xFFF), FALSE, FALSE, NULL);
	if (pMdl)
	{
		HgMmBuildMdlForNonPagedPool(pMdl);	// Update memory
		pMdl->MdlFlags |= MDL_MAPPED_TO_SYSTEM_VA;
		__try{
			pMappedAddress = HgMmMapLockedPagesSpecifyCache(pMdl, AccessMode, MmCached, NULL, 0, NormalPagePriority);
			if (pMappedAddress)
			{
				OldIrql = KeRaiseIrqlToDpcLevel();
				RtlZeroMemory((PVOID)((SIZE_T)pMappedAddress + nLast), Size);
				KeLowerIrql(OldIrql);
				HgMmUnmapLockedPages(pMappedAddress, pMdl);
				bRtn = TRUE;
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("%s(%d): Catch an exception!\n"));
			bRtn = FALSE;
		}
		HgIoFreeMdl(pMdl);
	}

	return bRtn;
}

NTSTATUS ProcessMemoryZeroBlock(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	BOOL		bAttach = FALSE;
	KAPC_STATE	as;
	PEPROCESS	pEProcess = NULL;
	PEPROCESS	pTempEProcess = NULL;
	NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;
	SIZE_T		Base = 0;
	SIZE_T		Size = 0;

	if (!pInItp){
		return status;
	}
	Base = pInItp->in_process_memory.Base;
	Size = pInItp->in_process_memory.Size;
	pEProcess = (PEPROCESS)pInItp->in_process_memory.pEProcess;
	TempStatus = PsLookupProcessByProcessId((HANDLE)pInItp->in_process_memory.nPId, &pTempEProcess);

	if (IsRealProcess(pEProcess)){
		if (HgIoGetCurrentProcess() != pEProcess)
		{
			HgKeStackAttachProcess(pEProcess, &as);
			bAttach = TRUE;
		}
		__try{
			HgProbeForWrite((PVOID)Base, Size, 1);
			if (ZeroMemoryAux((PVOID)Base, Size, UserMode))
			{
				status = STATUS_SUCCESS;
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("%s(%d): Catch an exception!\n"));
		}

		if (bAttach){
			HgKeUnstackDetachProcess(&as);
			bAttach = FALSE;
		}
	}

	if (NT_SUCCESS(TempStatus)){
		HgObfDereferenceObject(pTempEProcess);
	}

	return status;
}

NTSTATUS MmMapMemoryExcute(PVOID MemAddress,SIZE_T nSize,PVOID *ExecuteMap)
	/*
	输入参数：
	MemAddress  reload后的base地址
	nSize 使用真实的ImageSize
	ExecuteMap 用于存放map成功后可执行的地址的地址
	返回：
	只有STATUS_SUCCESS才是成功，其他均为有错误。
	*/
{
	NTSTATUS ns = STATUS_UNSUCCESSFUL;
	PMDL                    Mdl;
	if (!MemAddress||!ExecuteMap)
	{
		ns= STATUS_INSUFFICIENT_RESOURCES;
		goto end;
	}
	if (ExGetPreviousMode()!=KernelMode)
	{
		ns = STATUS_ACCESS_DENIED;
		goto end;
	}
	Mdl = IoAllocateMdl(
		MemAddress,
		(ULONG)nSize,
		FALSE,
		TRUE,
		NULL
		);

	if (!Mdl)
	{
		ns = STATUS_INSUFFICIENT_RESOURCES;
		goto end;
	}
	*ExecuteMap = MmMapLockedPagesSpecifyCache(
		Mdl,
		KernelMode,
		MmCached,
		NULL,
		FALSE,
		LowPagePriority
		);

	if (!(*ExecuteMap))
	{
		IoFreeMdl( Mdl );
		ns = STATUS_INSUFFICIENT_RESOURCES;
		goto end;
	}
	ns = MmProtectMdlSystemAddress(
		Mdl,
		PAGE_EXECUTE_READWRITE
		);
	if (!NT_SUCCESS( ns ))
	{
		IoFreeMdl( Mdl );
	}
end:
	return ns;
}

NTSTATUS ProcessMemoryChangeProtection(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;

	PEPROCESS	pEProcess = NULL;
	PEPROCESS	pTempEProcess = NULL;
	NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;
	POBJECT_TYPE ProcessType = (POBJECT_TYPE)g_hg_map[eHG_PsProcessType];
	HANDLE		hProcess = NULL;
	PVOID		Base = (PVOID)(pInItp->in_process_memory.Base);
	SIZE_T		Size = pInItp->in_process_memory.Size;

	pEProcess = (PEPROCESS)pInItp->in_process_memory.pEProcess;
	TempStatus = HgPsLookupProcessByProcessId((HANDLE)pInItp->in_process_memory.nPId, &pTempEProcess);

	ProcessLog(("***************%p, %p, %p, %p\n", pInItp->in_process_memory.nPId,
		pInItp->in_process_memory.pEProcess,
		pInItp->in_process_memory.Base,
		pInItp->in_process_memory.Size));
	if (IsRealProcess(pEProcess))
	{
		status = HgObOpenObjectByPointer(pEProcess, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL, ProcessType, KernelMode, &hProcess);
		if (NT_SUCCESS(status))
		{
			ULONG OldProtection = 0;
			//////////////////////////////////////////////////////////////////////////
			//PMDL pMdl = NULL;
			//PVOID pMappedAddress = NULL;
			////status = MmMapMemoryExcute(Base, Size, &pMappedAddress);
			//ProcessLog(("-----------------Previous Mode = %02X\n", ExGetPreviousMode()));

			//pMdl = HgIoAllocateMdl((PVOID)Base, (ULONG)Size, FALSE, TRUE, NULL);
			//if (pMdl)
			//{
 		//		MmProbeAndLockPages(pMdl, KernelMode, IoModifyAccess);
 		//		//pMdl->MdlFlags |= MDL_MAPPED_TO_SYSTEM_VA;
			//	__try{
			//		pMappedAddress = HgMmMapLockedPagesSpecifyCache(pMdl, KernelMode, MmCached, NULL, FALSE, NormalPagePriority);
			//		if (pMappedAddress)
			//		{
			//			status = MmProtectMdlSystemAddress(pMdl, (ULONG)(pInItp->in_process_memory.ModifyType));
			//			HgMmUnmapLockedPages(pMappedAddress, pMdl);
			//			MmUnlockPages(pMdl);
			//		}
			//	}__except(EXCEPTION_EXECUTE_HANDLER){
			//		ProcessLog(("%s(%d): Catch an exception!\n"));
			//	}
			//	HgIoFreeMdl(pMdl);
			//}
			//////////////////////////////////////////////////////////////////////////
			status = HgNtProtectVirtualMemory(hProcess, &Base, &pInItp->in_process_memory.Size, (ULONG)(pInItp->in_process_memory.ModifyType), &OldProtection);
			ProcessLog(("%s(%d): ChangeProtection Status: %p\n", __FILE__, __LINE__, status));
			HgNtClose(hProcess);
		}
	}

	if (NT_SUCCESS(TempStatus))
	{
		HgObfDereferenceObject(pTempEProcess);
	}

	return status;
}

NTSTATUS ProcessMemoryFree(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;

	PEPROCESS	pEProcess = NULL;
	PEPROCESS	pTempEProcess = NULL;
	NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;
	POBJECT_TYPE ProcessType = (POBJECT_TYPE)g_hg_map[eHG_PsProcessType];
	HANDLE		hProcess = NULL;
	PVOID		Base = (PVOID)(pInItp->in_process_memory.Base);

	pEProcess = (PEPROCESS)pInItp->in_process_memory.pEProcess;
	TempStatus = HgPsLookupProcessByProcessId((HANDLE)pInItp->in_process_memory.nPId, &pTempEProcess);

	if (IsRealProcess(pEProcess))
	{
		status = HgObOpenObjectByPointer(pEProcess, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL, ProcessType, KernelMode, &hProcess);
		if (NT_SUCCESS(status))
		{
			PETHREAD pThread = HgPsGetCurrentThread();
			CHAR PreMode = ChangePreMode(pThread);
			status = HgNtFreeVirtualMemory(hProcess, &Base, &pInItp->in_process_memory.Size, MEM_RELEASE);
			ProcessLog(("%s(%d): Free Status: %p\n", __FILE__, __LINE__, status));
			HgNtClose(hProcess);
			RecoverPreMode(pThread, PreMode);
		}
	}

	if (NT_SUCCESS(TempStatus))
	{
		HgObfDereferenceObject(pTempEProcess);
	}

	return status;
}

NTSTATUS ReadProcessMemoryAux(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pOutItp = (PITP)pOutBuffer;

	KAPC_STATE ks;
	BOOL bAttach = FALSE;
	PVOID pBuffer = NULL;
	SIZE_T BaseAddress = pOutItp->mem.Base;
	SIZE_T Length = pOutItp->mem.nLength;
	PEPROCESS pEProcess = (PEPROCESS)pOutItp->mem.pEProcess;

	if ((pEProcess == NULL)					||
		(KeGetCurrentIrql() >= DISPATCH_LEVEL))
	{
		return FALSE;
	}
	
	pBuffer = HgExAllocatePoolWithTag(PagedPool, pOutItp->mem.nLength, HG_MEM_TAG);
	if (pBuffer != NULL){
		RtlZeroMemory(pBuffer, pOutItp->mem.nLength);

		// does not attach self process.
		if (HgIoGetCurrentProcess() != pEProcess){
			HgKeStackAttachProcess(pEProcess, &ks);
			bAttach = TRUE;
		}

		if (BaseAddress < (SIZE_T)MM_HIGHEST_USER_ADDRESS	&&
			(HgMmIsAddressValid((PVOID)BaseAddress) == TRUE)){
				__try{
					HgProbeForRead((PVOID)BaseAddress, Length, 1);
					RtlCopyMemory(pBuffer, (PVOID)BaseAddress, Length);
				}__except(EXCEPTION_EXECUTE_HANDLER){
					ProcessLog(("%s(%d): Catch an exception!\n", __FILE__, __LINE__));
				}
		}

		if (bAttach == TRUE){
			HgKeUnstackDetachProcess(&ks);
			bAttach = FALSE;
		}

		RtlCopyMemory(pOutItp->mem.MemoryBlock, pBuffer, pOutItp->mem.nLength);

		HgExFreePoolWithTag(pBuffer, HG_MEM_TAG);
	}
	

	return STATUS_SUCCESS;
}

/************************************************************************/
/* Process Handle                                                       */
/************************************************************************/
VOID GetHandleObjectName(
	__in	HANDLE	Handle,
	__in	SIZE_T	Object,
	__in	PWCHAR	ObjectName)
{
	PVOID	HandleName = NULL;

	if (!Handle || !Object || !ObjectName){
		return;
	}

	HandleName = HgExAllocatePoolWithTag(PagedPool, 0x1000, HG_MEM_TAG);
	if (HandleName){
		ULONG nRet = 0;
		PETHREAD pThread = NULL;
		CHAR PreMode =0;

		pThread = HgPsGetCurrentThread();
		PreMode = ChangePreMode(pThread);
		RtlZeroMemory(HandleName, 0x1000);

		__try{
			if (NT_SUCCESS(HgNtQueryObject(Handle, HgObjectNameInformation, HandleName, 0x1000, &nRet))){
				POBJECT_NAME_INFORMATION pObjectNameInfo = (POBJECT_NAME_INFORMATION)HandleName;
				if (IsUnicodeStringValid(&pObjectNameInfo->Name)){
					size_t nLen = GetUnicodeStringSafeLength(&pObjectNameInfo->Name, SZ_PATH_LEN);
					wcsncpy(ObjectName, pObjectNameInfo->Name.Buffer, nLen);
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("%s(%d): Catch an exception!\n", __FILE__, __LINE__));
		}

		RecoverPreMode(pThread, PreMode);
		HgExFreePoolWithTag(HandleName, HG_MEM_TAG);
	}
}

VOID GetHandleTypeName(
	__in	HANDLE	Handle,
	__in	SIZE_T	Object,
	__in	PWCHAR	TypeName)
{
	PVOID	HandleName = NULL;

	if (!Handle || !Object || !TypeName){
		return;
	}

	HandleName = HgExAllocatePoolWithTag(PagedPool, 0x1000, HG_MEM_TAG);
	if (HandleName){
		ULONG nRet = 0;
		PETHREAD pThread = NULL;
		CHAR PreMode =0;

		pThread = HgPsGetCurrentThread();
		PreMode = ChangePreMode(pThread);
		RtlZeroMemory(HandleName, 0x1000);
		__try{
			if (NT_SUCCESS(HgNtQueryObject(Handle, HgObjectTypeInformation, HandleName, 0x1000, &nRet))){
				POBJECT_TYPE_INFORMATION pObjectNameInfo = (POBJECT_TYPE_INFORMATION)HandleName;
				if (IsUnicodeStringValid(&pObjectNameInfo->TypeName)){
					size_t nLen = GetUnicodeStringSafeLength(&pObjectNameInfo->TypeName, SZ_PATH_LEN);
					wcsncpy(TypeName, pObjectNameInfo->TypeName.Buffer, nLen);
				}
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){
			ProcessLog(("%s(%d): Catch an exception!\n", __FILE__, __LINE__));
		}

		RecoverPreMode(pThread, PreMode);
		HgExFreePoolWithTag(HandleName, HG_MEM_TAG);
	}
}

VOID InsertHandleToBuffer(
	__in	PEPROCESS	pEProcess,
	__in	HANDLE		Handle,
	__in	SIZE_T		Object,
	__inout PITP		pItp
	)
{
	BOOL			bAttach = FALSE;
	KAPC_STATE		as;
	PHANDLE_DATA	pHandleData = NULL;
	SIZE_T			nHandle = (SIZE_T)Handle;

	if (!pEProcess || !Handle || !pItp || !Object || !HgMmIsAddressValid((PVOID)Object) ||
		!(pHandleData = (PHANDLE_DATA)HgExAllocatePoolWithTag(PagedPool, sizeof(HANDLE_DATA), HG_MEM_TAG))){
			ProcessLog(("%s(%d): Invalid arguments!\n", __FILE__, __LINE__));
			return;
	}

	if (HgIoGetCurrentProcess() != pEProcess){
		HgKeStackAttachProcess(pEProcess, &as);
		bAttach = TRUE;
	}
	RtlZeroMemory((PVOID)pHandleData, sizeof(HANDLE_DATA));

	pHandleData->Handle = (SIZE_T)Handle;
	pHandleData->Object = Object;
	if (HgMmIsAddressValid((PVOID)(Object - GetOffset(_OBJECT_HEADER_sizeof)))){
		pHandleData->ReferenceCount = *(PSIZE_T)(Object - GetOffset(_OBJECT_HEADER_sizeof));
	} else {
		pHandleData->ReferenceCount = 0;
	}
	//switch (g_WindowsVersion)
	//{
	//case eWin_7:
	//case eWin_8:
	//	{
			//
			// System process use ObpKernelHandleTable, so we need to handle Handles especially.
			// In GetHandleObjectName and GetHandleTypeName will invoke NtQueryObject, 
			// and NtQueryObject must invoke ObReferenceObjectByHandle method.
			// ObReferenceObjectByHandle source code is in obref.c in WRK-1.2.
			// When Handle is Kernel Handle, ObReferenceObjectByHandle will do something as follow.
			//
			//  Handle = DecodeKernelHandle( Handle );
			//  HandleTable = ObpKernelHandleTable;
			// If you read DecodeKernelHandle, you will find a ^ and are you clear?
			//
			if (pEProcess == (PEPROCESS)g_hg_map[eHG_SystemProcess]){
				nHandle = (SIZE_T)EncodeKernelHandle(nHandle);
			}
	//	}
	//	break;
	//default:
	//	break;
	//}
	
	GetHandleObjectName((HANDLE)nHandle, Object, pHandleData->HandleName);
	GetHandleTypeName((HANDLE)nHandle, Object, pHandleData->ObjectName);
	
	if (bAttach){
		HgKeUnstackDetachProcess(&as);
		bAttach = FALSE;
	}
	//ProcessLog(("Handle = %p, Object = %p, Reference = %d, HandleName = %S, ObjectName = %S\n", 
		//pHandleData->Handle, pHandleData->Object, pHandleData->ReferenceCount, pHandleData->HandleName, pHandleData->ObjectName));
	memcpy(&pItp->get_process_handles.HandleData[pItp->get_process_handles.nCount], pHandleData, sizeof(HANDLE_DATA));
	HgExFreePoolWithTag(pHandleData, HG_MEM_TAG);
	pHandleData = NULL;
}

NTSTATUS EnumerateProcessHandles(
	SIZE_T	Pid,
	SIZE_T	EProcess,
	PITP	pItp
	)
{
	NTSTATUS NtStatus = STATUS_UNSUCCESSFUL;
	PETHREAD CurrentThread = NULL;
	CHAR	PreMode = 0;
	SIZE_T	nRet = 0x10000;

	CurrentThread = HgPsGetCurrentThread();
	PreMode = ChangePreMode(CurrentThread);

	do {
		PVOID Buffer = HgExAllocatePoolWithTag(PagedPool, nRet, HG_MEM_TAG);
		if (Buffer){
			RtlZeroMemory(Buffer, nRet);
			NtStatus = HgNtQuerySystemInformation(SystemHandleInformation, Buffer, nRet, &nRet);
			if (NT_SUCCESS(NtStatus)){
				PSYSTEM_HANDLE_INFORMATION Handles = (PSYSTEM_HANDLE_INFORMATION)Buffer;
				ULONG i = 0;
				for (i = 0; i < Handles->NumberOfHandles; i++){
					if (Pid == Handles->Handles[i].UniqueProcessId){
						if (pItp->get_process_handles.nCount < pItp->get_process_handles.nMaxCount)
						{
							// Insert
							InsertHandleToBuffer(
								(PEPROCESS)EProcess, 
								(HANDLE)Handles->Handles[i].HandleValue,
								(SIZE_T)Handles->Handles[i].Object,
								pItp);
						}
						pItp->get_process_handles.nCount ++;
					}
				}
			}
			HgExFreePoolWithTag(Buffer, 0);
			nRet *= 2;
		}
	} while (NtStatus == STATUS_INFO_LENGTH_MISMATCH);

	RecoverPreMode(CurrentThread, PreMode);
	return NtStatus;
}

NTSTATUS EnumProcessHandle(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pReturnLength
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	PITP		pOutItp = (PITP)pOutBuffer;

	SIZE_T nPid = 0;
	PEPROCESS	pEProcess = NULL;

	//PEPROCESS	pTempEProcess = NULL;
	//NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;

	ProcessLog(("%s(%d): Enter EnumProcessHandle!\n", __FILE__, __LINE__));
	if (pInItp && pOutItp){
		nPid = pInItp->get_process_handles_params.Pid;
		pEProcess = (PEPROCESS)pInItp->get_process_handles_params.EProcess;
		//TempStatus = HgPsLookupProcessByProcessId((HANDLE)pInItp->get_process_handles_params.Pid, &pTempEProcess);
		//ProcessLog(("EPROCESS 1: %p, EPROCESS 2: %p\n", pInItp->get_process_handles_params.EProcess, pTempEProcess));
		if (IsRealProcess(pEProcess)){
			status = EnumerateProcessHandles(nPid, (SIZE_T)pEProcess, pOutItp);
			if (pReturnLength){
				*pReturnLength = nOutSize;
			}
		}

		//if (NT_SUCCESS(TempStatus)){
		//	HgObfDereferenceObject(pTempEProcess);
		//}
	}

	status = STATUS_SUCCESS;
	return status;
}

NTSTATUS HazeCloseHandle(
	__in	SIZE_T	Handle,
	__in	SIZE_T	HandleObject,
	__in	SIZE_T	bForceKill,
	__in	PEPROCESS	pEProcess)
{
	NTSTATUS NtStatus = STATUS_UNSUCCESSFUL;
	BOOL bAttach = FALSE;
	KAPC_STATE as;
	PHANDLE_DATA pHandleData = NULL;
	SIZE_T nHandle = Handle;
	PVOID pObject = NULL;

	if (!Handle || !HandleObject || !pEProcess){
		return STATUS_UNSUCCESSFUL;
	}

	//switch (g_WindowsVersion)
	//{
	//case eWin_7:
	//case eWin_8:
	//	{
			if (pEProcess == (PEPROCESS)g_hg_map[eHG_SystemProcess]){
				nHandle = (SIZE_T)EncodeKernelHandle(nHandle);
			}
	//	}
	//	break;
	//default:
	//	break;
	//}

	if (HgIoGetCurrentProcess() != pEProcess){
		HgKeStackAttachProcess(pEProcess, &as);
		bAttach = TRUE;
	}

	NtStatus = HgObReferenceObjectByHandle((HANDLE)nHandle, 0x80000000, NULL, KernelMode, &pObject, NULL);
	if (NT_SUCCESS(NtStatus)){
		PETHREAD CurrentThread = NULL;
		CHAR	PreMode = 0;
		OBJECT_HANDLE_FLAG_INFORMATION ObjectHandleFlagInfo;
		ULONG ReturnLength = 0;

		CurrentThread = HgPsGetCurrentThread();
		PreMode = ChangePreMode(CurrentThread);

		if (pObject == (PVOID)HandleObject &&
			NT_SUCCESS(HgNtQueryObject(
				(HANDLE)nHandle, 
				HgObjectHandleFlagInformation,
				&ObjectHandleFlagInfo, 
				sizeof(OBJECT_HANDLE_FLAG_INFORMATION), 
				&ReturnLength)))
		{
			if (bForceKill){
				if (ObjectHandleFlagInfo.ProtectFromClose){
					ObjectHandleFlagInfo.ProtectFromClose = FALSE;
					NtStatus = HgNtSetInformationObject((HANDLE)nHandle, HgObjectHandleFlagInformation, &ObjectHandleFlagInfo, sizeof(OBJECT_HANDLE_FLAG_INFORMATION));
					if (NT_SUCCESS(NtStatus)){
						ProcessLog(("%s(%d): Force close process handle!\n", __FILE__, __LINE__));
						NtStatus = HgNtClose((HANDLE)nHandle);
					}
				} else {
					NtStatus = HgNtClose((HANDLE)nHandle);
				}
			} else {
				if (!ObjectHandleFlagInfo.ProtectFromClose){
					NtStatus = HgNtClose((HANDLE)nHandle);
				}
			}
		}

		RecoverPreMode(CurrentThread, PreMode);
		HgObfDereferenceObject(pObject);
	}

	if (bAttach){
		HgKeUnstackDetachProcess(&as);
		bAttach = FALSE;
	}

	return NtStatus;
}

NTSTATUS CloseProcessHandle(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pReturnLength
	)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pInItp = (PITP)pInBuffer;
	PITP		pOutItp = (PITP)pOutBuffer;

	PEPROCESS	pEProcess = NULL;
	PEPROCESS	pTempEProcess = NULL;
	NTSTATUS	TempStatus = STATUS_UNSUCCESSFUL;

	ProcessLog(("%s(%d): Enter CloseProcessHandle!\n", __FILE__, __LINE__));
	ProcessLog(("PEPROCESS = %p\n", (PEPROCESS)pInItp->close_process_handle_params.EProcess));
	if (pInItp && pOutItp){
		pEProcess = (PEPROCESS)pInItp->close_process_handle_params.EProcess;
		TempStatus = HgPsLookupProcessByProcessId((HANDLE)pInItp->close_process_handle_params.Pid, &pTempEProcess);
		if (IsRealProcess(pEProcess)){
			status = HazeCloseHandle(
				pInItp->close_process_handle_params.Handle,
				pInItp->close_process_handle_params.HandleObject,
				pInItp->close_process_handle_params.IsForceKill,
				(PEPROCESS)pInItp->close_process_handle_params.EProcess);

			if (pReturnLength){
				*pReturnLength = nOutSize;
			}
		}

		if (NT_SUCCESS(TempStatus)){
			HgObfDereferenceObject(pTempEProcess);
		}
	}

	status = STATUS_SUCCESS;
	return status;
}