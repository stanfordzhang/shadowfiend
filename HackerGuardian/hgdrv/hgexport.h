
#ifndef __HG_EXPORT_H__
#define __HG_EXPORT_H__

#include "stdhg.h"
#include <ntifs.h>
#include <windef.h>
#include <ntiologc.h>
#include <ntimage.h>
#include "hgdef.h"

/************************************************************************/
/*                                                                      */
/************************************************************************/
//
// global variable
// 
extern POBJECT_TYPE*	IoDriverObjectType;
extern POBJECT_TYPE*	IoDeviceObjectType;
extern POBJECT_TYPE*	LpcPortObjectType;
extern POBJECT_TYPE*	PsProcessType;
extern POBJECT_TYPE*	CmpKeyObjectType;
extern POBJECT_TYPE*	IoFileObjectType;
extern POBJECT_TYPE*	ExEventObjectType;
extern POBJECT_TYPE*	ExSemaphoreObjectType;
extern POBJECT_TYPE*	TmTransactionManagerObjectType;
extern POBJECT_TYPE*	TmResourceManagerObjectType;
extern POBJECT_TYPE*	TmEnlistmentObjectType;
extern POBJECT_TYPE*	TmTransactionObjectType;
extern POBJECT_TYPE*	PsThreadType;
extern POBJECT_TYPE*	SeTokenObjectType;
extern POBJECT_TYPE*	MmSectionObjectType;

extern ULONG			g_WindowsBuildNumber;
extern ULONG			g_WindowsVersion;			// Windows version
extern PDRIVER_OBJECT	g_pHGDrvObj;				// Current Driver Object
extern BOOL				g_bReloadKernelModule;
extern ULONG_PTR		g_hg_map[HG_MAP_LENGTH];
extern LIST_ENTRY		g_hg_map_list[100];
extern LIST_ENTRY		g_tmp_process_list;
extern KSPIN_LOCK		g_tmp_process_spin_lock;

// the offset of field in the structures.
VOID SetOffset(ULONG_PTR id, ULONG_PTR v);
ULONG_PTR GetOffset(ULONG_PTR id);

/************************************************************************/
/* Windows export function/variable declaration                         */
/************************************************************************/
//
// Declare ZwQuerySystemInformation
//
NTSTATUS
ZwQuerySystemInformation(
	__in		SYSTEM_INFORMATION_CLASS	SystemInformationClass,
	__out		PVOID						SytemInformation,
	__in		SIZE_T						SystemInformationLength,
	__out_opt	PSIZE_T						ReturnLength
	);

//
// Declare NtQuerySystemInformation
//
NTSTATUS
NtQuerySystemInformation(
	__in		SYSTEM_INFORMATION_CLASS	SystemInformationClass,
	__out		PVOID						SytemInformation,
	__in		SIZE_T						SystemInformationLength,
	__out_opt	PSIZE_T						ReturnLength
	);

//
// Declare ObCreateObject
//
NTKERNELAPI
NTSTATUS
ObCreateObject(
	__in		KPROCESSOR_MODE		ProbeMode,
	__in		POBJECT_TYPE		ObjectType,
	__in_opt	POBJECT_ATTRIBUTES	ObjectAttributes,
	__in		KPROCESSOR_MODE		OwnershipMode,
	__inout_opt	PVOID				ParseContext,
	__in		ULONG				ObjectBodySize,
	__in		ULONG				PagePoolCharge,
	__in		ULONG				NonPagePoolCharge,
	__out		PVOID*				Object
	);

//
// Declare SeCreateAccessState
//
NTKERNELAPI
NTSTATUS
SeCreateAccessState(
	__out	PACCESS_STATE		AccessState,
	__out	PAUX_ACCESS_DATA	AuxData,
	__in	ACCESS_MASK			DesiredAccess,
	__in	PGENERIC_MAPPING	GenericMapping
	);

/************************************************************************/
/* global function                                                       */
/************************************************************************/
FORCEINLINE
BOOLEAN
HgMmIsAddressValid(
	IN	PVOID	VirtualAddress
	)
{
	pfnMmIsAddressValid pMmIsAddressValid = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: MmIsAddressValid>>\n"));
		pMmIsAddressValid = 
			(pfnMmIsAddressValid)g_hg_map[eHG_MmIsAddressValid];
	}else{
		pMmIsAddressValid = &MmIsAddressValid;
	}
	if (pMmIsAddressValid != NULL){
		return pMmIsAddressValid(VirtualAddress);
	}else{
		return FALSE;
	}
}

FORCEINLINE
PVOID
HgExAllocatePoolWithTag(
	IN	POOL_TYPE	PoolType,
	IN	SIZE_T		NumberOfBytes,
	IN	ULONG		Tag
	)
{
	pfnExAllocatePoolWithTag pExAllocatePoolWithTag = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ExAllocatePoolWithTag>>\n"));
		pExAllocatePoolWithTag =
			(pfnExAllocatePoolWithTag)g_hg_map[eHG_ExAllocatePoolWithTag];
	}else{
		pExAllocatePoolWithTag = &ExAllocatePoolWithTag;
	}
		
	if (pExAllocatePoolWithTag != NULL){
		return pExAllocatePoolWithTag(PoolType, NumberOfBytes, Tag);
	}else{
		return NULL;
	}
}

FORCEINLINE
NTSTATUS
HgNtQuerySystemInformation(
	IN				SYSTEM_INFORMATION_CLASS	SystemInformationClass,
	IN	OUT			PVOID						SystemInformation,
	IN				SIZE_T						SystemInformationLength,
	OUT OPTIONAL	PSIZE_T						ReturnLength
	)
{
	pfnNtQuerySystemInformation pNtQuerySystemInformation = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: NtQuerySystemInformation>>\n"));
		pNtQuerySystemInformation =
			(pfnNtQuerySystemInformation)g_hg_map[eHG_NtQuerySystemInformation];
	}else{
		pNtQuerySystemInformation = &ZwQuerySystemInformation;
	}
		
	if (pNtQuerySystemInformation != NULL){
		return pNtQuerySystemInformation(	SystemInformationClass,
											SystemInformation,
											SystemInformationLength,
											ReturnLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
VOID
HgExFreePoolWithTag(
	IN	PVOID	P,
	IN	ULONG	Tag
	)
{
	pfnExFreePoolWithTag pExFreePoolWithTag = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ExFreePoolWithTag>>\n"));
		pExFreePoolWithTag =
			(pfnExFreePoolWithTag)g_hg_map[eHG_ExFreePoolWithTag];
	}else{
		pExFreePoolWithTag = &ExFreePoolWithTag;
	}
		
	if (pExFreePoolWithTag != NULL){
		pExFreePoolWithTag(P, Tag);
	}
}

FORCEINLINE
NTSTATUS
HgPsLookupProcessByProcessId(
	IN	HANDLE		ProcessId,
	OUT	PEPROCESS	*Process
	)
{
	pfnPsLookupProcessByProcessId pPsLookupProcessByProcessId = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsLookupProcessByProcessId>>\n"));
		pPsLookupProcessByProcessId =
			(pfnPsLookupProcessByProcessId)
			g_hg_map[eHG_PsLookupProcessByProcessId];
	}else{
		pPsLookupProcessByProcessId = &PsLookupProcessByProcessId;
	}
		
	if (pPsLookupProcessByProcessId != NULL){
		return pPsLookupProcessByProcessId(ProcessId, Process);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
ULONG_PTR
GetPsLookupProcessByProcessIdAddress(
	VOID
	)
{
	ULONG_PTR nRtn = 0;
	if (g_bReloadKernelModule == TRUE){
		nRtn = g_hg_map[eHG_PsLookupProcessByProcessId];
	}else{
		nRtn = (ULONG_PTR)&PsLookupProcessByProcessId;
	}

	return nRtn;
}

FORCEINLINE
LONG_PTR
FASTCALL
HgObfDereferenceObject(
	IN	PVOID	Object
	)
{
	pfnObfDereferenceObject pObfDereferenceObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ObfDereferenceObject>>\n"));
		pObfDereferenceObject =
			(pfnObfDereferenceObject)g_hg_map[eHG_ObfDereferenceObject];
	}else{
		pObfDereferenceObject = &ObfDereferenceObject;
	}
		
	if (pObfDereferenceObject != NULL){
		return pObfDereferenceObject(Object);
	}else{
		return 0;
	}
}

FORCEINLINE
PETHREAD
HgPsGetCurrentThread(
	VOID
	)
{
	pfnPsGetCurrentThread pPsGetCurrentThread = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsGetCurrentThread>>\n"));
		pPsGetCurrentThread =
			(pfnPsGetCurrentThread)g_hg_map[eHG_KeGetCurrentThread];
	}else{
		pPsGetCurrentThread = &PsGetCurrentThread;
	}
		
	if (pPsGetCurrentThread != NULL){
		return pPsGetCurrentThread();
	}else{
		return NULL;
	}
}

FORCEINLINE
VOID
HgProbeForRead(
	__in	PVOID	Address,
	__in	SIZE_T	Length,
	__in	ULONG	Alignment
	)
{
	pfnProbeForRead pProbeForRead = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ProbeForRead>>\n"));
		pProbeForRead =
			(pfnProbeForRead)g_hg_map[eHG_ProbeForRead];
	}else{
		pProbeForRead = &ProbeForRead;
	}

	if (pProbeForRead != NULL){
		pProbeForRead(Address, Length, Alignment);
	}
}

FORCEINLINE
VOID
HgProbeForWrite(
	__in	PVOID	Address,
	__in	SIZE_T	Length,
	__in	ULONG	Alignment
	)
{
	pfnProbeForWrite pProbeForWrite = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ProbeForWrite>>\n"));
		pProbeForWrite =
			(pfnProbeForWrite)g_hg_map[eHG_ProbeForWrite];
	}else{
		pProbeForWrite = &ProbeForWrite;
	}

	if (pProbeForWrite != NULL){
		pProbeForWrite(Address, Length, Alignment);
	}
}

FORCEINLINE
PRKTHREAD
HgKeGetCurrentThread(
	VOID
	)
{
	return HgPsGetCurrentThread();
}

FORCEINLINE
HANDLE
HgPsGetProcessId(
	__in	PEPROCESS	Process
	)
{
	pfnPsGetProcessId pPsGetProcessId = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: PsGetProcessId>>\n"));
		pPsGetProcessId =
			(pfnPsGetProcessId)g_hg_map[eHG_PsGetProcessId];
	}else{
		pPsGetProcessId = &PsGetProcessId;
	}

	if (pPsGetProcessId != NULL){
		return pPsGetProcessId(Process);
	}else{
		return NULL;
	}
}

FORCEINLINE
NTSTATUS
HgObQueryNameString(
	__in		PVOID						Object,
	__out_opt	POBJECT_NAME_INFORMATION	ObjectNameInfo,
	__in		ULONG						Length,
	__out		PULONG						ReturnLength
	)
{
	pfnObQueryNameString pObQueryNameString = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: ObQueryNameString>>\n"));
		pObQueryNameString =
			(pfnObQueryNameString)g_hg_map[eHG_ObQueryNameString];
	}else{
		pObQueryNameString = &ObQueryNameString;
	}

	if (pObQueryNameString != NULL){
		return pObQueryNameString(	Object,
									ObjectNameInfo,
									Length,
									ReturnLength);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
NTSTATUS
HgIoQueryFileDosDeviceName(
	__in	PFILE_OBJECT				FileObject,
	__out	POBJECT_NAME_INFORMATION	*ObjectNameInformation
	)
{
	pfnIoQueryFileDosDeviceName pIoQueryFileDosDeviceName = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoQueryFileDosDeviceName>>\n"));
		pIoQueryFileDosDeviceName =
			(pfnIoQueryFileDosDeviceName)
				g_hg_map[eHG_IoQueryFileDosDeviceName];
	}else{
		pIoQueryFileDosDeviceName = &IoQueryFileDosDeviceName;
	}

	if (pIoQueryFileDosDeviceName != NULL){
		return pIoQueryFileDosDeviceName(	FileObject,
											ObjectNameInformation);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
NTSTATUS
HgIoVolumeDeviceToDosName(
	__in	PVOID			VolumeDeviceObject,
	__out	PUNICODE_STRING	DosName
	)
{
	pfnIoVolumeDeviceToDosName pIoVolumeDeviceToDosName = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoVolumeDeviceToDosName>>\n"));
		pIoVolumeDeviceToDosName =
			(pfnIoVolumeDeviceToDosName)
			g_hg_map[eHG_IoVolumeDeviceToDosName];
	}else{
		pIoVolumeDeviceToDosName = &IoVolumeDeviceToDosName;
	}

	if (pIoVolumeDeviceToDosName != NULL){
		return pIoVolumeDeviceToDosName(VolumeDeviceObject, DosName);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
PEPROCESS
HgIoGetCurrentProcess(
	VOID
	)
{
	pfnIoGetCurrentProcess pIoGetCurrentProcess = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoGetCurrentProcess>>\n"));
		pIoGetCurrentProcess =
			(pfnIoGetCurrentProcess)
				g_hg_map[eHG_IoGetCurrentProcess];
	}else{
		pIoGetCurrentProcess = &IoGetCurrentProcess;
	}

	if (pIoGetCurrentProcess != NULL){
		return pIoGetCurrentProcess();
	}else{
		return NULL;
	}
}

FORCEINLINE
VOID
HgKeStackAttachProcess(
	__in	PKPROCESS		Process,
	__out	PRKAPC_STATE	ApcState
	)
{
	pfnKeStackAttachProcess pKeStackAttachProcess = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeStackAttachProcess>>\n"));
		pKeStackAttachProcess =
			(pfnKeStackAttachProcess)
				g_hg_map[eHG_KeStackAttachProcess];
	}else{
		pKeStackAttachProcess = &KeStackAttachProcess;
	}

	if (pKeStackAttachProcess != NULL){
		pKeStackAttachProcess(Process, ApcState);
	}
}

FORCEINLINE
VOID
HgKeUnstackDetachProcess(
	__in	PRKAPC_STATE	ApcState
	)
{
	pfnKeUnstackDetachProcess pKeUnstackDetachProcess = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeUnstackDetachProcess>>\n"));
		pKeUnstackDetachProcess =
			(pfnKeUnstackDetachProcess)
				g_hg_map[eHG_KeUnstackDetachProcess];
	}else{
		pKeUnstackDetachProcess = &KeUnstackDetachProcess;
	}

	if (pKeUnstackDetachProcess != NULL){
		pKeUnstackDetachProcess(ApcState);
	}
}

FORCEINLINE
KAFFINITY
HgKeQueryActiveProcessors(
	VOID
	)
{
	pfnKeQueryActiveProcessors pKeQueryActiveProcessors = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeQueryActiveProcessors>>\n"));
		pKeQueryActiveProcessors =
			(pfnKeQueryActiveProcessors)
				g_hg_map[eHG_KeQueryActiveProcessors];
	}else{
		pKeQueryActiveProcessors = &KeQueryActiveProcessors;
	}

	if (pKeQueryActiveProcessors != NULL){
		return pKeQueryActiveProcessors();
	}else{
		return 0;
	}
}

FORCEINLINE
VOID
HgKeInitializeEvent(
	__in	PRKEVENT	Event,
	__in	EVENT_TYPE	Type, 
	__in	BOOLEAN		State
	)
{
	pfnKeInitializeEvent pKeInitializeEvent = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeInitializeEvent>>\n"));
		pKeInitializeEvent =
			(pfnKeInitializeEvent)
			g_hg_map[eHG_KeInitializeEvent];
	}else{
		pKeInitializeEvent = &KeInitializeEvent;
	}

	if (pKeInitializeEvent != NULL){
		pKeInitializeEvent(Event, Type, State);
	}
}

FORCEINLINE
VOID
HgKeInitializeDpc(
	__in	PRKDPC				Dpc,
	__in	PKDEFERRED_ROUTINE	DeferredRoutine,
	__in	PVOID				DeferredContext
	)
{
	pfnKeInitializeDpc pKeInitializeDpc = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeInitializeDpc>>\n"));
		pKeInitializeDpc =
			(pfnKeInitializeDpc)
			g_hg_map[eHG_KeInitializeDpc];
	}else{
		pKeInitializeDpc = &KeInitializeDpc;
	}

	if (pKeInitializeDpc != NULL){
		pKeInitializeDpc(Dpc, DeferredRoutine, DeferredContext);
	}
}

FORCEINLINE
VOID
HgKeSetTargetProcessorDpc(
	__in	PRKDPC	Dpc,
	__in	CCHAR	Number
	)
{
	pfnKeSetTargetProcessorDpc pKeSetTargetProcessorDpc = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeSetTargetProcessorDpc>>\n"));
		pKeSetTargetProcessorDpc =
			(pfnKeSetTargetProcessorDpc)
			g_hg_map[eHG_KeSetTargetProcessorDpc];
	}else{
		pKeSetTargetProcessorDpc = &KeSetTargetProcessorDpc;
	}

	if (pKeSetTargetProcessorDpc != NULL){
		pKeSetTargetProcessorDpc(Dpc, Number);
	}
}

FORCEINLINE
VOID
HgKeSetImportanceDpc(
	__in	PRKDPC			Dpc,
	__in	KDPC_IMPORTANCE	Importance
	)
{
	pfnKeSetImportanceDpc pKeSetImportanceDpc = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeSetImportanceDpc>>\n"));
		pKeSetImportanceDpc = (pfnKeSetImportanceDpc)g_hg_map[eHG_KeSetImportanceDpc];
	}else{
		pKeSetImportanceDpc = &KeSetImportanceDpc;
	}

	if (pKeSetImportanceDpc != NULL){
		pKeSetImportanceDpc(Dpc, Importance);
	}
}

FORCEINLINE
BOOLEAN
HgKeInsertQueueDpc(
	__in	PRKDPC	Dpc,
	__in	PVOID	SystemArgument1,
	__in	PVOID	SystemArgument2
	)
{
	pfnKeInsertQueueDpc pKeInsertQueueDpc = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeInsertQueueDpc>>\n"));
		pKeInsertQueueDpc =
			(pfnKeInsertQueueDpc)
			g_hg_map[eHG_KeInsertQueueDpc];
	}else{
		pKeInsertQueueDpc = &KeInsertQueueDpc;
	}

	if (pKeInsertQueueDpc != NULL){
		return pKeInsertQueueDpc(Dpc, SystemArgument1, SystemArgument2);
	}else{
		return FALSE;
	}
}

FORCEINLINE
NTSTATUS
HgKeWaitForSingleObject(
	__in		PVOID			Object,
	__in		KWAIT_REASON	WaitReason,
	__in		KPROCESSOR_MODE	WaitMode,
	__in		BOOLEAN			Alertable,
	__in_opt	PLARGE_INTEGER	Timeout
	)
{
	pfnKeWaitForSingleObject pKeWaitForSingleObject = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeWaitForSingleObject>>\n"));
		pKeWaitForSingleObject =
			(pfnKeWaitForSingleObject)
			g_hg_map[eHG_KeWaitForSingleObject];
	}else{
		pKeWaitForSingleObject = &KeWaitForSingleObject;
	}

	if (pKeWaitForSingleObject != NULL){
		return pKeWaitForSingleObject(	Object,
										WaitReason,
										WaitMode,
										Alertable,
										Timeout);
	}else{
		return STATUS_UNSUCCESSFUL;
	}
}

FORCEINLINE
LONG
HgKeSetEvent(
	__in	PRKEVENT	Event,
	__in	KPRIORITY	Increment,
	__in	BOOLEAN		Wait
	)
{
	pfnKeSetEvent pKeSetEvent = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: KeSetEvent>>\n"));
		pKeSetEvent =
			(pfnKeSetEvent)
			g_hg_map[eHG_KeSetEvent];
	}else{
		pKeSetEvent = &KeSetEvent;
	}

	if (pKeSetEvent != NULL){
		return pKeSetEvent(Event, Increment, Wait);
	}else{
		return 0;
	}
}

POBJECT_TYPE
HgObGetObjectType(
	__in	PVOID	pObject
	);

FORCEINLINE
PEPROCESS
HgIoThreadToProcess(
	__in	PETHREAD	Thread
	)
{
	pfnIoThreadToProcess pIoThreadToProcess = NULL;
	if (g_bReloadKernelModule == TRUE){
		ExportLog(("<<Using Reloaded Function: IoThreadToProcess>>\n"));
		pIoThreadToProcess =
			(pfnIoThreadToProcess)g_hg_map[eHG_IoThreadToProcess];
	}else{
		pIoThreadToProcess = &IoThreadToProcess;
	}

	if (pIoThreadToProcess != NULL){
		return pIoThreadToProcess(Thread);
	}else{
		return NULL;
	}
}

NTSTATUS HgObReferenceObjectByPointer(
	__in	PVOID				Object,
	__in	ACCESS_MASK			DesireAccess,
	__in	POBJECT_TYPE		ObjectType,
	__in	KPROCESSOR_MODE		AccessMode);

HANDLE HgPsGetThreadId(
	__in	PETHREAD	Thread);

NTSTATUS HgZwOpenDirectoryObject(
	__out	PHANDLE				DirectoryHandle,
	__in	ACCESS_MASK			DesiredAccess,
	__in	POBJECT_ATTRIBUTES	ObjectAttributes);

NTSTATUS
HgObReferenceObjectByHandle(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation
	);

BOOLEAN
HgMmIsNonPagedSystemAddressValid(
	__in PVOID VirtualAddress
	);

PHYSICAL_ADDRESS
HgMmGetPhysicalAddress (
	__in PVOID BaseAddress
	);

PVOID
HgMmGetVirtualForPhysical (
	__in PHYSICAL_ADDRESS PhysicalAddress
	);

PMDL
HgIoAllocateMdl(
	__in_opt __drv_aliasesMem PVOID VirtualAddress,
	__in ULONG Length,
	__in BOOLEAN SecondaryBuffer,
	__in BOOLEAN ChargeQuota,
	__inout_opt PIRP Irp
	);

VOID
HgMmBuildMdlForNonPagedPool (
	__inout PMDLX MemoryDescriptorList
	);

PVOID
HgMmMapLockedPagesSpecifyCache (
	__in PMDLX MemoryDescriptorList,
	__in __drv_strictType(KPROCESSOR_MODE/enum _MODE,__drv_typeConst) 
	KPROCESSOR_MODE AccessMode,
	__in __drv_strictTypeMatch(__drv_typeCond) MEMORY_CACHING_TYPE CacheType,
	__in_opt PVOID BaseAddress,
	__in ULONG BugCheckOnFailure,
	__in __drv_strictTypeMatch(__drv_typeCond) MM_PAGE_PRIORITY Priority
	);

VOID
HgMmUnmapLockedPages (
	__in PVOID BaseAddress,
	__in PMDL MemoryDescriptorList
	);

VOID
HgIoFreeMdl(
	PMDL Mdl
	);

BOOL
HgGetWindowsDirectory(
	__inout	PWCHAR	pwszRootName
	);

BOOL
HgGetDriverDirectory(
	__inout PWCHAR	pwszDir
	);

NTSTATUS
NTAPI
HgNtOpenSymbolicLinkObject(
	__out PHANDLE LinkHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	);

NTSTATUS
NTAPI
HgNtQuerySymbolicLinkObject(
	__in HANDLE LinkHandle,
	__inout PUNICODE_STRING LinkTarget,
	__out_opt PULONG ReturnedLength
	);

NTSTATUS
NTAPI
HgNtClose (
	__in HANDLE Handle
	);

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
	);

NTSTATUS
NTAPI
HgZwOpenKey(
	__out PHANDLE KeyHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	);

NTSTATUS
NTAPI
HgZwEnumerateKey(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_INFORMATION_CLASS KeyInformationClass,
	__out_bcount_opt(Length) PVOID KeyInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	);

NTSTATUS
NTAPI
HgZwEnumerateValueKey(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	__out_bcount_opt(Length) PVOID KeyValueInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	);

NTSTATUS
HgPsSetCreateProcessNotifyRoutine(
	__in PCREATE_PROCESS_NOTIFY_ROUTINE NotifyRoutine,
	__in BOOLEAN Remove
	);

NTSTATUS
HgPsSetCreateThreadNotifyRoutine(
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	);

NTSTATUS
HgPsSetLoadImageNotifyRoutine(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	);

NTSTATUS
HgPsRemoveCreateThreadNotifyRoutine(
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	);

NTSTATUS
HgPsRemoveLoadImageNotifyRoutine(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	);

NTSTATUS
HgIoRegisterShutdownNotification(
	__in PDEVICE_OBJECT DeviceObject
	);

NTSTATUS
HgCmUnRegisterCallback(
	__in LARGE_INTEGER    Cookie
	);

VOID
HgIoUnregisterShutdownNotification(
	__in PDEVICE_OBJECT DeviceObject
	);

NTSTATUS HgNtQueryVirtualMemory(
	__in HANDLE ProcessHandle,
	__in PVOID BaseAddress,
	__in MEMORY_INFORMATION_CLASS MemoryInformationClass,
	__out_bcount(MemoryInformationLength) PVOID MemoryInformation,
	__in SIZE_T MemoryInformationLength,
	__out_opt PSIZE_T ReturnLength
	);

NTSTATUS HgObOpenObjectByPointer(
	__in PVOID  Object,
	__in ULONG  HandleAttributes,
	__in_opt PACCESS_STATE  PassedAccessState,
	__in ACCESS_MASK  DesiredAccess,
	__in POBJECT_TYPE  ObjectType,
	__in KPROCESSOR_MODE  AccessMode,
	__out	PHANDLE  Handle
	);

NTSTATUS __stdcall HgNtProtectVirtualMemory(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	UnsafeBaseAddress,
	__inout	PSIZE_T	UnsafeNumberOfBytesToProtect,
	__in	ULONG	NewAccessProtection,
	__out	PULONG	UnsafeOldAccessProtection);

NTSTATUS __stdcall HgNtFreeVirtualMemory(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	BaseAddress,
	__inout	PSIZE_T	RegionSize,
	__in	ULONG	FreeType);

NTSTATUS NTAPI	HgNtQueryObject (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength,
	__out_opt PULONG ReturnLength);

NTSTATUS NTAPI HgNtSetInformationObject (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__in_bcount(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength);

#endif