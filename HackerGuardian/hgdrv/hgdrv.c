/*************************************************************************
* Copyright (c) 2012 - Stanford Studio
* 
* Original filename	: hgdrv.c
* Project			: Hacker Guardian
* Date of creation	: 2013-03-06
* Author(s)			: Stanford Zhang
* Mail				: zhangstanford@gmail.com
* 
* Purpose			: <description>
* 
* Revisions			:
*	0000[2012-03-06] Initial revision.
*	
*************************************************************************/
#include <ntifs.h>
#include "hgexport.h"
#include "hgioctl.h"
#include "init.h"
#include "hgprocess.h"
#include "drvsys.h"
#include "registry.h"
#include "hgnotify.h"
#include "hgheader.h"

extern PSHORT	NtBuildNumber;
extern BOOL		g_bReloadKernelModule;

WCHAR pwszDevName[] = L"\\Device\\HGDevice";
WCHAR pwszDosName[] = L"\\??\\HackerGuardian";

NTSTATUS 
DispatchClose(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;
	DeLog(("Enter DispatchClose!\n"));

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

	return status;
}

NTSTATUS 
DispatchCreate(
	IN PDEVICE_OBJECT		DeviceObject,
	IN PIRP					Irp
	)
{
	NTSTATUS status = STATUS_SUCCESS;

	DeLog(("Enter hgdrv.sys DispatchCreate\n"));
// 	PEPROCESS pCurrentEprocess = PsGetCurrentProcess();
// 	ULONG nDebugportOffset = GetGlobalVeriable(enumDebugportOffset);
// 	if (pCurrentEprocess && nDebugportOffset)
// 	{
// 
// 		ULONG DebugObject = *(PULONG)((ULONG)pCurrentEprocess + nDebugportOffset);
// 		if (DebugObject)
// 		{
// 			status = STATUS_INVALID_PARAMETER;
// 		}
// 	}
// 
	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	DeLog(("Leave hgdrv.sys DispatchCreate\n"));
	return status;
}

NTSTATUS
DirectHandler(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize,
	__out	ULONG*	nRet
	)
{

	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pItp	= (PITP)pInBuffer;

	__try
	{
		if (nInSize >= sizeof(ULONG)){
			switch (pItp->op_type){
			case OT_PROCESS_ENUMERATE:
				DeLog(("[hgdrv.c].[DirectHandler]: Enumerating processes...\n"));
				DeLog(("pInBuffer: 0x%p, pOutBuffer: 0x%p\n", pInBuffer, pOutBuffer));
				EnumProcess(pInBuffer, nInSize, pOutBuffer, nOutSize, nRet);
				break;
			}
		}

		status = STATUS_SUCCESS;
	}__except(EXCEPTION_EXECUTE_HANDLER){
		DeLog(("[hgdrv.c].[DirectHandler]: EXCEPTION_EXECUTE_HANDLER :(\n"));
		status = STATUS_UNSUCCESSFUL;
	}
	return status;
}

/************************************************************************/
/* Communication between ring0 and ring3                                */
/************************************************************************/
NTSTATUS
NeitherHandler(
	__in	PVOID	pInBuffer,
	__in	ULONG	ulInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	ulOutSize,
	__out	ULONG*	dwRet
	)
{
	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pItp	= (PITP)pInBuffer;
	ULONG		i		= 0;
	__try{
		HgProbeForRead(pInBuffer, ulInSize, 1);
		HgProbeForWrite(pOutBuffer, ulOutSize, 1);
		
// 		DeLog(("HgProbeForRead Address = 0x%p,\
// 			HgProbeForWrite Address = 0x%p\n",
// 			&HgProbeForRead, &HgProbeForWrite));

		if (ulInSize >= sizeof(ULONG)){
			switch (pItp->op_type)
			{
			case OT_PROCESS_ENUMERATE:
				DeLog(("pInBuffer: 0x%p, pOutBuffer: 0x%p\n", pInBuffer, pOutBuffer));
				EnumProcess(pInBuffer, 
							ulInSize, 
							pOutBuffer, 
							ulOutSize, 
							dwRet);
				status = STATUS_SUCCESS;
				return status;
			case OT_PROCESS_MODULE:
				DeLog(("[hgdrv.c].[NeitherHandler]: enumerate process module!\n"));
				EnumProcessModule(	pInBuffer, 
									(SIZE_T)ulInSize, 
									pOutBuffer, 
									(SIZE_T)ulOutSize, 
									(SIZE_T*)dwRet);
				status = STATUS_SUCCESS;
				return status;
			case OT_PROCESS_THREAD:
				EnumProcessThread(	pInBuffer,
									(SIZE_T)ulInSize,
									pOutBuffer,
									(SIZE_T)ulOutSize,
									(PSIZE_T)dwRet);
				status = STATUS_SUCCESS;
				return status;
				//
				// ===== Process memory =====
				// 
			case OT_PROCESS_MEMORY_ENUM:
				return EnumProcessMemory(pInBuffer, ulInSize, pOutBuffer, ulOutSize, dwRet);
			case OT_PROCESS_MEMORY_ZERO:
				return ProcessMemoryZeroBlock(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
			case OT_PROCESS_MEMORY_CHANGE_PROTECTION:
				return ProcessMemoryChangeProtection(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
			case OT_PROCESS_MEMORY_FREE:
				return ProcessMemoryFree(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
				//
				// ===== Process handle =====
				// 
			case OT_PROCESS_HANDLE_ENUM:
				return EnumProcessHandle(pInBuffer, ulInSize, pOutBuffer, ulOutSize, (PSIZE_T)dwRet);
			case OT_PROCESS_HANDLE_CLOSE:
				return CloseProcessHandle(pInBuffer, ulInSize, pOutBuffer, ulOutSize, (PSIZE_T)dwRet);
				//
				//========== Driver ==========
				//
			case OT_DRIVER_ENUMERATE:
				EnumDriver(	pInBuffer,
							(SIZE_T)ulInSize,
							pOutBuffer,
							(SIZE_T)ulOutSize,
							(PSIZE_T)dwRet);
				status = STATUS_SUCCESS;
				return status;
				//
				//========== Registry ==========
			case OT_REGISTRY_ENUMERATE_KEY:
				status = RegistryEnumerateKey(	pInBuffer,
												(SIZE_T)ulInSize,
												pOutBuffer,
												(SIZE_T)ulOutSize,
												(PSIZE_T)dwRet);
				return status;
			case OT_REGISTRY_ENUMERATE_VALUE_KEY:
				status = RegistryEnumerateValueKey(	pInBuffer,
													(SIZE_T)ulInSize,
													pOutBuffer,
													(SIZE_T)ulOutSize,
													(PSIZE_T)dwRet);
				return status;
			case OT_KERNEL_ENUMERATE_NOTIFY:
				status = EnumNotify(pInBuffer, ulInSize, pOutBuffer, ulOutSize, dwRet);
				return status;
			case OT_HOOK_SSDT:
				status = EnumSSDT(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
				return status;
			case OT_HOOK_SHADOW_SSDT:
				status = EnumShadowSSDT(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
				return status;
			case OT_MEMORY_READ:
				return ReadProcessMemoryAux(pInBuffer, ulInSize, pOutBuffer, ulOutSize);
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		DeLog(("[hgdrv.c].[NeitherHandler]: Catch the exception\n"));
		status = STATUS_UNSUCCESSFUL;
	}
	return status;
}

NTSTATUS 
DispatchDeviceControl(
	IN PDEVICE_OBJECT		pDevObj,
	IN PIRP					pIrp
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	// Get Current Stack
	PIO_STACK_LOCATION pStack = IoGetCurrentIrpStackLocation(pIrp);
	// Get IO Control Code
	ULONG uCtlCode = pStack->Parameters.DeviceIoControl.IoControlCode;
	// Get input buffer size
	ULONG uInSize = pStack->Parameters.DeviceIoControl.InputBufferLength;
	// Get output buffer size
	ULONG uOutSize = pStack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG uInfo = 0;
	UCHAR* pInputBuffer = NULL;
	UCHAR* pOutputBuffer = NULL;
	UCHAR* pUserInputBuffer = NULL;
	UCHAR* pUserOutputBuffer = NULL;
	ULONG i = 0;
	DWORD dwRet = 0;

	DeLog(("Entry DispatchDeviceControl!\n"));
//	DeLog(("uInSize = %d\n", uInSize));
//	DeLog(("uOutSize = %d\n", uOutSize));
	switch (uCtlCode){
	case SZH_IOCTL_BUFFERED:
		// input data
		pInputBuffer = (UCHAR*)pIrp->AssociatedIrp.SystemBuffer;
		DeLog(("pInputBuffer = 0x%p\n", pInputBuffer));
		for(i = 0; i < uInSize; i ++){
			DeLog(("%02X\n", pInputBuffer[i]));
		}
		// output data
		pOutputBuffer = (UCHAR*)pIrp->AssociatedIrp.SystemBuffer;
		DeLog(("pOutputBuffer = 0x%p\n", pOutputBuffer));
		memset(pOutputBuffer, 0xAA, uOutSize);
		uInfo = uOutSize;
		status = STATUS_SUCCESS;
		break;
	case SZH_IOCTL_IN_DIRECT:
		{
			DeLog(("SZH_IOCTL_IN_DIRECT\n"));
			pInputBuffer = (UCHAR*)pIrp->AssociatedIrp.SystemBuffer;
	// 		for(i = 0; i < uInSize; i++){
	// 			DeLog(("%X\n", pInputBuffer[i]));
	// 		}
			pOutputBuffer = (UCHAR*)MmGetSystemAddressForMdlSafe(pIrp->MdlAddress, NormalPagePriority);
			//memset(pOutputBuffer, 0xAA, uOutSize);
			//uInfo = uOutSize;
			status = DirectHandler(pInputBuffer, uInSize, pOutputBuffer, uOutSize, &dwRet);
		}
		break;
	case SZH_IOCTL_OUT_DIRECT:
		status = STATUS_SUCCESS;
		break;
	case SZH_IOCTL_NEITHER:
		{
			pUserInputBuffer = (UCHAR*)pStack->Parameters.DeviceIoControl.Type3InputBuffer;
			pUserOutputBuffer = (UCHAR*)pIrp->UserBuffer;
			if (pUserInputBuffer == NULL || pUserOutputBuffer == NULL){
				return STATUS_UNSUCCESSFUL;
			}
			status = NeitherHandler(pUserInputBuffer,
									uInSize, 
									pUserOutputBuffer, 
									uOutSize, 
									&dwRet);	
		}
		break;
	default:
		status = STATUS_INVALID_VARIANT;
	}

	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = dwRet;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return status;
}

VOID 
DriverUnload(
	IN PDRIVER_OBJECT	DriverObject
	)
{
	PDEVICE_OBJECT	pdoNextDevObj = DriverObject->DeviceObject;
	UNICODE_STRING	usSymName;
	PDEVICE_OBJECT	pdoCurDevObj = NULL;
	DeLog(("Enter DriverUnload!\n"));

	//////////////////////////////////////////////////////////////////////////
// 	if (g_hg_map[eHG_NewKernelBase]){
// 		ExFreePoolWithTag((PVOID)g_hg_map[eHG_NewKernelBase], HG_MEM_TAG);
// 		g_hg_map[eHG_NewKernelBase] = 0;
// 	}
// 
// 	if (g_hg_map[eHG_NewKernelBaseUnrepair]){
// 		ExFreePoolWithTag((PVOID)g_hg_map[eHG_NewKernelBaseUnrepair], HG_MEM_TAG);
// 		g_hg_map[eHG_NewKernelBaseUnrepair] = 0;
// 	}
//
// 	if (g_hg_map[eHG_OriginKiServiceTable]){
// 		ExFreePoolWithTag((PVOID)g_hg_map[eHG_OriginKiServiceTable], HG_MEM_TAG);
// 		g_hg_map[eHG_OriginKiServiceTable] = 0;
// 	}
//
// 	if (g_hg_map[eHG_OriginSSDT]){
// 		ExFreePool((PVOID)g_hg_map[eHG_OriginSSDT]);
// 	}
//
// 	if (g_hg_map[eHG_ReloadKiServiceTable]){
// 		ExFreePool((PVOID)g_hg_map[eHG_ReloadKiServiceTable]);
// 	}
// 
// 	if (g_hg_map[eHG_ReloadSSDT]){
// 		ExFreePool((PVOID)g_hg_map[eHG_ReloadSSDT]);
// 	}
	//////////////////////////////////////////////////////////////////////////

	RtlInitUnicodeString(&usSymName, pwszDosName);
	IoDeleteSymbolicLink(&usSymName);
	while(pdoNextDevObj){
		pdoCurDevObj = pdoNextDevObj;
		pdoNextDevObj = pdoCurDevObj->NextDevice;
		IoDeleteDevice(pdoCurDevObj);
	}
}

NTSTATUS 
DriverEntry(
	IN OUT PDRIVER_OBJECT   pDriverObject,
	IN PUNICODE_STRING      pRegistryPath
	)
{
	NTSTATUS		status				= STATUS_UNSUCCESSFUL;
	PDEVICE_OBJECT	pdoDevObj			= NULL;
	UNICODE_STRING	usDevName;
	UNICODE_STRING	usSymName;
// 	WCHAR			pwszRegPath[1024]	= {0};
// 	WCHAR*			pwszRegName			= NULL;

	DISASM MyDisasm;
	int len, i = 0;
	int Error = 0;

	DeLog(("Version : %s\n", BeaEngineVersion()));
	DeLog(("Revision : %s\n", BeaEngineRevision()));

// 	(void) memset (&MyDisasm, 0, sizeof(DISASM));
// 	MyDisasm.EIP = (UIntPtr)&tes;
// 	DeLog(("MyDisasm.EIP = 0x%p\n", MyDisasm.EIP));
// 	while ((!Error) && (i < 20))
// 	{
// #if defined(_WIN64)
// 		/* =================== Define 64 bits architecture =================== */
// 		MyDisasm.Archi = 64;
// #else
// 		MyDisasm.Archi = 0;
// #endif
// 		len = Disasm(&MyDisasm);
// 		if (len != UNKNOWN_OPCODE)
// 		{
// 			DeLog(("%s\n", MyDisasm.CompleteInstr));
// 			DeLog(("%p\t%016X\t%08X\t%08X\t%016X\n", MyDisasm.EIP, MyDisasm.VirtualAddr, MyDisasm.SecurityBlock, MyDisasm.Archi, MyDisasm.Options));
// 			DeLog(("%08X\t%08X\t%s\t%08X\t%016X\t%016X\t%08X\n",
// 				MyDisasm.Instruction.Category, MyDisasm.Instruction.Opcode, MyDisasm.Instruction.Mnemonic,
// 				MyDisasm.Instruction.BranchType, MyDisasm.Instruction.Flags, MyDisasm.Instruction.AddrValue,
// 				MyDisasm.Instruction.Immediat, MyDisasm.Instruction.ImplicitModifiedRegs));
// // 			DeLog(("******************0x%p\n", MyDisasm.Instruction.Opcode));
// // 			DeLog(("Category = 0x%p\n", MyDisasm.Instruction.Category));
// // 			DeLog(("ArgMnemonic = 0x%p\n", MyDisasm.Argument1.ArgType));
// 			MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
// 			i++;
// 		}else{
// 			Error = 1;
// 		}
// 	}

	DeLog(("Enter hgdrv.sys DriverEntry!"));
	DeLog(("NtBuildNumber = %d\n", *NtBuildNumber));
	DeLog(("MM_HIGHEST_USER_ADDRESS = 0x%p\n", MM_HIGHEST_USER_ADDRESS));	//32-Bit: 0x7FFEFFFF
	DeLog(("MM_SYSTEM_RANGE_START = 0x%p\n", MM_SYSTEM_RANGE_START));		// 32-Bit: 0x80000000

	if (!(*NtBuildNumber == 2195								||	// 2000
		*NtBuildNumber == 2600									||	// XP
		*NtBuildNumber == 3790									||	// Win2003
		*NtBuildNumber >= 0x1770 && *NtBuildNumber <= 0x1772	||	// Vista RTM/SP1/SP2, 2008 RTM/SP2
		*NtBuildNumber == 7600									||	// Win7
		*NtBuildNumber == 7601									||	
		*NtBuildNumber == 9200)){									// Win8
		DeLog(("Sorry! hgdrv.sys doesn't support this system!"));
		return status;
	}
// 	DeLog(("Print registry information!"));
// 	DeLog(("%wZ"), pRegistryPath);
// 	wcsncpy(pwszRegPath, 
// 		pRegistryPath->Buffer, 
// 		pRegistryPath->Length / sizeof(WCHAR));
// 	DeLog(("Registry Path = %S"), pwszRegPath);

	//
	// Initialize
	// 
	g_bReloadKernelModule = FALSE;
	InitializeEnvironment(pDriverObject);

//	EnumIdtHook(NULL, 0, pDriverObject, 0);

	RtlInitUnicodeString(&usDevName, pwszDevName);
	status = IoCreateDevice(
		pDriverObject,
		0, &usDevName,
		FILE_DEVICE_UNKNOWN,
		FILE_DEVICE_SECURE_OPEN,
		FALSE, &pdoDevObj);
	if (!NT_SUCCESS(status)){
		DeLog(("IoCreateDevice Error!\n"));
		return status;
	}
	//pdoDevObj->Flags |= DO_DIRECT_IO;

	RtlInitUnicodeString(&usSymName, pwszDosName);
	status = IoCreateSymbolicLink(&usSymName, &usDevName);
	if (!NT_SUCCESS(status)){
		DeLog(("IoCreateSymbolicLink Error!\n"));
		IoDeleteDevice(pdoDevObj);
		return status;
	}

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = DispatchClose;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchDeviceControl;
	pDriverObject->DriverUnload = DriverUnload;

	status = STATUS_SUCCESS;
	DeLog(("Leave hgdrv.sys DriverEntry!\n"));
	return status;
}