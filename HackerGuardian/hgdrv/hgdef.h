
#ifndef __HG_DEF_H__
#define __HG_DEF_H__

/************************************************************************/
/* http://en.wikipedia.org/wiki/Windows_NT#Releases
========================================================================================
												Windows NT releases
========================================================================================
 Version	Marketing name						Release date				RTM build	Editions
----------------------------------------------------------------------------------------
 NT 3.1		Windows NT 3.1						27 July 1993				528			Workstation (named just Windows NT), Advanced Server
 NT 3.5		Windows NT 3.5						21 September 1994			807			Workstation, Server	
 NT 3.51	Windows NT 3.51						30 May 1995					1057		Workstation, Server
 NT 4.0		Windows NT 4.0						29 July 1996				1381		Workstation, Server, Server Enterprise Edition, Terminal Server, Embedded
----------------------------------------------------------------------------------------
 NT 5.0		Windows 2000						17 February 2000			2195		Professional, Server, Advanced Server
												26 September 2000						Data center Server
----------------------------------------------------------------------------------------
 NT 5.1		Windows XP							25 October 2001				2600		Home, Professional, Media Center (original, 2003, 2004 & 2005), Tablet PC (original and 2005), Starter, Embedded, Home N, Professional N
			Windows Fundamentals for Legacy PCs	8 July 2006								N/A
----------------------------------------------------------------------------------------
 NT 5.2		Windows XP							28 March 2003				3790		64-bit Edition Version 2003
			Windows Server 2003					24 April 2003							Standard, Enterprise, Data center, Web, Storage, Small Business Server, Compute Cluster	
			Windows XP							25 April 2005							Professional x64 Edition
			Windows Server 2003 R2				6 December 2005							Standard, Enterprise, Data center, Web, Storage, Small Business Server, Compute Cluster	
			Windows Home Server					16 July 2007							N/A	
----------------------------------------------------------------------------------------
 NT 6.0		Windows Vista						Business: 30 November 2006	6000 (RTM)									Starter, Home Basic, Home Premium, Business, Enterprise, Ultimate, Home Basic N, Business N
												Consumer: 30 January 2007	6001 (SP1)
																			6002 (SP2)
----------------------------------------------------------------------------------------
			Windows Server 2008					27 February 2008			6001 (RTM)	Foundation, Standard, Enterprise, Data center, Web Server, HPC Server, Itanium-Based Systems
																			6002 (SP2)
----------------------------------------------------------------------------------------
 NT 6.1		Windows 7							22 October 2009				7600 (RTM)	Starter, Home Basic, Home Premium, Professional, Enterprise, Ultimate
																			7601 (SP1)
----------------------------------------------------------------------------------------
			Windows Server 2008 R2				22 October 2009				7600 (RTM)	Foundation, Standard, Enterprise, Data center, Web Server, HPC Server, Itanium-Based Systems
																			7601 (SP1)
----------------------------------------------------------------------------------------
 NT 6.2		Windows 8							26 October 2012[27]			9200		Windows 8 (x86/64), Windows 8 Pro (x86/64), Windows 8 Enterprise (x86/64), Windows RT (ARM)
			Windows Phone 8						30 October 2012				9900						N/A
			Windows Server 2012					4 September 2012			9200		Foundation, Essentials, Standard, Data center
========================================================================================
 */
/************************************************************************/

/************************************************************************/
/* 
 *	MS VC++ 10.0	_MSC_VER = 1600
 *	MS VC++ 9.0		_MSC_VER = 1500
 *	MS VC++ 8.0		_MSC_VER = 1400
 *	MS VC++ 7.1		_MSC_VER = 1310
 *	MS VC++ 7.0		_MSC_VER = 1300
 *	MS VC++ 6.0		_MSC_VER = 1200
 *	MS VC++ 5.0		_MSC_VER = 1100
 */
/************************************************************************/

#include <ntifs.h>
#include <ntimage.h>
#include <WinDef.h>

/************************************************************************/
/* Define enum                                                          */
/************************************************************************/
//
// Windows Version
// 
typedef enum _WIN_VERSION{
	eWin_Unknow = 0,
	eWin_2000,
	eWin_XP,
	eWin_2003,
	eWin_2003_SP1_SP2,
	eWin_Vista,
	eWin_Vista_SP1_SP2,
	eWin_7,
	eWin_8
}WIN_VERSION;

//
// Global Map Table Key
// 
typedef enum _MAP_TABLE_KEY{
	eHG_SystemProcess = 0,
	eHG_CurrentSSDT,
	eHG_SSDTCount,
	eHG_Offset_KAPC_STATE_Process,
	eHG_ObjectTypeOffset_OBJECT_HEADER,
	eHG_IoDriverObjectType,
	eHG_IoDeviceObjectType,
	eHG_IoFileObjectType,
	eHG_PsInitialSystemProcessAddress,
	eHG_PsProcessType,
	eHG_PsThreadType,
	eHG_ObjectTableOffset_EPROCESS,
	eHG_InheritedFromUniqueProcessIdOffset_EPROCESS,
	eHG_Offset_KTHREAD_ApcState,
	eHG_SectionObjectOffset_EPROCESS,
	eHG_UniqueProcessIdOffset_EPROCESS,
	eHG_UniqueProcessIdOffset_HANDLE_TABLE,
	eHG_SectionBaseAddressOffset_EPROCESS,
	eHG_DebugPortOffset,
	eHG_InitialStackOffset_ETHREAD,
	eHG_StackLimitOffset_KTHREAD,
	eHG_KernelStackOffset_KTHREAD,
	eHG_KernelApcDisableOffset_KTHREAD,
	eHG_CrossThreadFlagsOffset_ETHREAD,
	eHG_SuspendCountOffset_KTHREAD,
	eHG_FreezeCountOffset_KTHREAD,
	eHG_TIMER_TABLE_SIZE,
	eHG_QueueListEntryOffset_KTHREAD,
	eHG_IoplOffset_KPROCESS,
	eHG_IopmOffsetOffset_KPROCESS,
	// SSDT ID
	eHG_NtQueryVirtualMemoryIndex,
	eHG_NtQuerySystemInformationIndex,
	eHG_NtOpenProcessIndex,
	eHG_NtCloseIndex,
	eHG_NtOpenDirectoryObjectIndex,
	eHG_NtQueryObjectIndex,
	eHG_NtUnmapViewOfSectionIndex,
	eHG_NtOpenProcessTokenIndex,
	eHG_NtQueryInformationTokenIndex,
	eHG_NtTerminateThreadIndex,
	eHG_NtSuspendThreadIndex,
	eHG_NtResumeThreadIndex,
	eHG_NtSetInformationObjectIndex,
	eHG_NtFreeVirtualMemoryIndex,
	eHG_NtProtectVirtualMemoryIndex,
	eHG_NtAdjustPrivilegesTokenIndex,
	eHG_NtTerminateProcessIndex,
	eHG_NtResumeProcessIndex,
	eHG_NtSuspendProcessIndex,
	eHG_NtOpenSymbolicLinkObjectIndex,
	eHG_NtQuerySymbolicLinkObjectIndex,
	eHG_NtOpenKeyIndex,
	eHG_NtEnumerateKeyIndex,
	eHG_NtEnumerateValueKeyIndex,
	eHG_NtOpenFileIndex,
	eHG_NtCreateSectionIndex,
	eHG_NtMapViewOfSectionIndex,
	eHG_NtDeleteKeyIndex,
	eHG_NtCreateKeyIndex,
	eHG_NtSetValueKeyIndex,
	eHG_NtDeleteValueKeyIndex,
	// Shadow SSDT ID
	eHG_NtUserBuildHwndListIndex,
	eHG_NtUserQueryWindowIndex,
	eHG_NtUserKillTimerIndex,
	eHG_NtUserValidateTimerCallbackIndex,
	eHG_NtUserRegisterHotkeyIndex,

	eHG_CurrentKernelBase,
	eHG_CurrentKernelSize,

	eHG_NewKernelBase,
	eHG_NewKernelBaseUnrepair,
	eHG_OriginKiServiceTable,
	eHG_OriginSSDT,
	eHG_ReloadKiServiceTable,
	eHG_ReloadSSDT,
	eHG_ReloadNtosKrnlSuccess,
	eHG_Ntoskrnl_LDR_DATA_TABLE_ENTRY,

	// API
	eHG_MmIsAddressValid,
	eHG_MmGetVirtualForPhysical,
	eHG_MmGetPhysicalAddress,
	eHG_ObQueryNameString,
	eHG_ObfDereferenceObject,
	eHG_IoVolumeDeviceToDosName,
	eHG_IoGetCurrentProcess,
	eHG_ObOpenObjectByPointer,
	eHG_ObReferenceObjectByPointer,
	eHG_PsLookupProcessByProcessId,
	eHG_IoThreadToProcess,
	eHG_ExAllocatePoolWithTag,
	eHG_ExFreePoolWithTag,
	eHG_KeSetEvent,
	eHG_KeQueryActiveProcessors,
	eHG_KeInitializeEvent,
	eHG_KeInitializeDpc,
	eHG_KeSetTargetProcessorDpc,
	eHG_KeSetImportanceDpc,
	eHG_KeInsertQueueDpc,
	eHG_KeWaitForSingleObject,
	eHG_KeStackAttachProcess,
	eHG_KeUnstackDetachProcess,
	eHG_ProbeForRead,
	eHG_ObReferenceObjectByHandle,
	eHG_ObGetObjectType,
	eHG_KeGetCurrentThread,
	eHG_PsGetProcessId,
	eHG_PsGetThreadId,
	eHG_IoQueryFileDosDeviceName,
	eHG_MmIsNonPagedSystemAddressValid,
	eHG_IoAllocateMdl,
	eHG_MmBuildMdlForNonPagedPool,
	eHG_MmMapLockedPagesSpecifyCache,
	eHG_MmUnmapLockedPages,
	eHG_IoFreeMdl,
	eHG_ProbeForWrite,
	eHG_IoCreateFile,
	eHG_ObMakeTemporaryObject,
	eHG_PsTerminateSystemThread,
	eHG_PsCreateSystemThread,
	eHG_IoDeleteDevice,
	eHG_ObOpenObjectByName,
	eHG_PsSetCreateProcessNotifyRoutine,
	eHG_PsSetLoadImageNotifyRoutine,
	eHG_PsSetCreateThreadNotifyRoutine,
	eHG_PsRemoveCreateThreadNotifyRoutine,
	eHG_PsRemoveLoadImageNotifyRoutine,
	eHG_IoRegisterShutdownNotification,
	eHG_CmUnRegisterCallback,
	eHG_IoUnregisterShutdownNotification,
	eHG_KeUpdateSystemTime,
	eHG_KeCancelTimer,
	eHG_IoInitializeTimer,
	eHG_IoStartTimer,
	eHG_IoStopTimer,
	eHG_ExQueueWorkItem,
	eHG_Ke386IoSetAccessProcess,
	eHG_IoGetDeviceObjectPointer,
	eHG_ObReferenceObjectByName,
	eHG_RtlInitAnsiString,
	eHG_RtlAnsiStringToUnicodeString,
	eHG_RtlFreeUnicodeString,
	eHG_PsGetCurrentProcess,
	eHG_IoFreeIrp,
	eHG_MmFlushImageSection,
	eHG_IoCreateDriver,
	eHG_ObGetObjectSecurity,
	eHG_ObCreateObjectType,
	eHG_IofCompleteRequest,
	eHG_ObCreateObjectTypeEx,

	eHG_NtQuerySystemInformation,
	eHG_NtOpenProcess,
	eHG_NtQueryVirtualMemory,
	eHG_NtClose,
	eHG_NtOpenDirectoryObject,
	eHG_NtQueryObject,
	eHG_NtOpenProcessToken,
	eHG_NtQueryInformationToken,
	eHG_NtUnmapViewOfSection,
	eHG_NtTerminateThread,
	eHG_NtResumeThread,
	eHG_NtSuspendThread,
	eHG_NtSetInformationObject,
	eHG_NtFreeVirtualMemory,
	eHG_NtProtectVirtualMemory,
	eHG_NtAdjustPrivilegesToken,
	eHG_NtTerminateProcess,
	eHG_NtResumeProcess,
	eHG_NtSuspendProcess,
	eHG_NtOpenSymbolicLinkObject,
	eHG_NtQuerySymbolicLinkObject,
	eHG_NtOpenKey,
	eHG_NtEnumerateKey,
	eHG_NtEnumerateValueKey,
	eHG_NtOpenFile,
	eHG_NtCreateSection,
	eHG_NtMapViewOfSection,
	eHG_NtDeleteKey,
	eHG_NtCreateKey,
	eHG_NtSetValueKey,
	eHG_NtDeleteValueKey,

	eHG_OriginHalDllBase,
	eHG_HalDllSize,
	eHG_NewHalDllBase,
	eHG_NewHalDllBase_Unfix,

	eHG_CsrssEprocess,
	eHG_OriginWin32kBase,
	eHG_Win32kSize,
	eHG_NowShadowSSDT,
	eHG_ShadowSSDTFunctionsCount,

	eHG_ReloadWin32kSuccess,
	eHG_NewWin32kBase,
	eHG_OriginShadowSSDT,
	eHG_OriginShadowKiServiceTable,
	eHG_ReloadShadowSSDT,
	eHG_ReloadShadowKiServiceTable,

	eHG_NtUserBuildHwndList,
	eHG_NtUserQueryWindow,
	eHG_NtUserKillTimer,
	eHG_NtUserValidateTimerCallback,
	eHG_NtUserRegisterHotkey,

	eHG_NtfsBase,
	eHG_NtfsSize,
	eHG_AcpiBase,
	eHG_AcpiSize,
	eHG_AtapiBase,
	eHG_AtapiSize,
	eHG_FastfatBase,
	eHG_FastfatSize,
	eHG_KbdclassBase,
	eHG_KbdclassSize,
	eHG_MouclassBase,
	eHG_MouclassSize,
	eHG_ClasspnpBase,
	eHG_ClasspnpSize,
	eHG_TcpipBase,
	eHG_TcpipSize,
	eHG_AtaPortBase,
	eHG_AtaPortSize,

	eHG_NtfsDriverObject,
	eHG_ReloadNtfsBase,
	eHG_NtfsImpMmFlushImageSection,

	eHG_NtfsOriginalDispatchs,
	eHG_NtfsReloadDispatchs,
	eHG_IopInvalidDeviceRequest,
	eHG_FastfatDriverObject,
	eHG_FastfatImpMmFlushImageSection,
	eHG_FastfatOriginalDispatchs,
	eHG_FastfatReloadDispatchs,
	eHG_ReloadFastfatBase,

	eHG_PspCidTable,
	eHG_KdVersionBlock,
	eHG_PEB_ProcessParameters,
	eHG_RTL_USER_PROCESS_PARAMETERS_ImagePathName,
	eHG_KTHREAD_Process,

	eHG_WaitListHead,
	eHG_DispatcherReadyListHead,

	// reload 64-Bit kernel
	eHG_ReloadedKernelBase64,
	eHG_ReloadedKernelSize64,
	eHG_OriginalSSDT64,
	eHG_OriginalShadowSSDT64,
	eHG_ReloadedSSDT64,
	eHG_ReloadedShadowSSDT64,
	eHG_DirectoryObjectType,

	// kernel notify routine
	eHG_PspCreateProcessNotifyRoutine,
	eHG_PspCreateThreadNotifyRoutine,
	eHG_PspLoadImageNotifyRoutine,
	eHG_CmpCallBackVector,
	eHG_IopNotifyShutdownQueueHead,
}MAP_TABLE_KEY;

typedef enum _FIELD_OFFSET_ID{
	//
	// _KPCR
	//
	_KPCR_Prcb = 0,
	// _KPRCB
	//
	_KPRCB_IdleThread,
	_KPRCB_WaitListHead,
	_KPRCB_DispatcherReadyListHead,
	//
	// _KAPC_STATE
	// 
	_KAPC_STATE_Process,
	//
	// _KTHREAD
	//
	_KTHREAD_ApcState,
	_KTHREAD_PreviousMode,
	_KTHREAD_WaitListEntry,
	_KTHREAD_ThreadListEntry,
	_KTHREAD_Teb,
	_KTHREAD_Priority,
	_KTHREAD_ContextSwitches,
	_KTHREAD_State,
	//
	// _ETHREAD
	//
	_ETHREAD_ThreadListEntry,
	_ETHREAD_Cid,
	_ETHREAD_Win32StartAddress,
	_ETHREAD_StartAddress,
	_ETHREAD_ThreadsProcess,		// 64-bit does not have this field.
	//
	// _KPROCESS
	//
	_KPROCESS_ThreadListHead,
	//
	// _EPROCESS
	// 
	_EPROCESS_UniqueProcessId,
	_EPROCESS_ActiveProcessLinks,
	_EPROCESS_ObjectTable,
	_EPROCESS_InheritedFromUniqueProcessId,
	_EPROCESS_ImageFileName,
	_EPROCESS_Peb,
	_EPROCESS_SeAuditProcessCreationInfo,
	_EPROCESS_VadRoot,
	_EPROCESS_ThreadListHead,
	//
	// _HANDLE_TABLE
	// 
	_HANDLE_TABLE_TableCode,
	_HANDLE_TABLE_UniqueProcessId,
	//
	// _LDR_DATA_TABLE_ENTRY
	//
	_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,
	_LDR_DATA_TABLE_ENTRY_FullDllName,
	_LDR_DATA_TABLE_ENTRY_BaseDllName,
	_LDR_DATA_TABLE_ENTRY_DllBase,
	//
	// _OBJECT_TYPE
	// 
	_OBJECT_TYPE_TypeList,
	//
	// _OBJECT_HEADER
	// 
	_OBJECT_HEADER_sizeof,
}FIELD_OFFSET_ID;

//
// Global Map Table Key
// 
typedef enum _MAP_LIST_KEY{
	eHG_ProcessHead,
	eHG_ProcessThreadHead,
	eHG_ProcessHandleHead,
	eHG_ObjectTypeHead,
	eHG_ProcessMemoryHead,
	eHG_ProcessModuleHead
}MAP_LIST_KEY;

//
// System Information Classes.
//

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS;  

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation,
	MemoryWorkingSetList,
	MemorySectionName
}MEMORY_INFORMATION_CLASS;

//
// System Handle Information
// 
typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFO {
	USHORT UniqueProcessId;
	USHORT CreatorBackTraceIndex;
	UCHAR ObjectTypeIndex;
	UCHAR HandleAttributes;
	USHORT HandleValue;
	PVOID Object;
	ULONG GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFO, *PSYSTEM_HANDLE_TABLE_ENTRY_INFO;

typedef struct _SYSTEM_HANDLE_INFORMATION {
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO Handles[ 1 ];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

//
//
// 
typedef enum _ENTRY_POINT_TYPE{
	EPT_EntryNone,
	EPT_EntryPointHead,
	EPT_EntryPointJmp,
}ENTRY_POINT_TYPE;

/************************************************************************/
/* Define Macro                                                         */
/************************************************************************/
//
// File path length
// 
#define		SZ_PATH_LEN					2 * MAX_PATH
#define		SZ_PATH_LEN_WCHAR			SZ_PATH_LEN * sizeof(WCHAR)
#define		HG_MAP_LENGTH				2048
#define		HG_MEM_TAG					'MTAG'

#define		AlignSize(nSize, nAlign)	((((nSize) + (nAlign) - 1) / (nAlign)) * (nAlign))

#define		LDRP_RELOCATION_INCREMENT		0x1
#define		LDRP_RELOCATION_FINAL			0x2

#define		SEC_IMAGE					0x1000000
#define		MEM_IMAGE					SEC_IMAGE 

#define		UNICODE_STRING_MAX_BYTES   ((USHORT) 65534)

#define		LOCATE_FIELD(base, offset)	(SIZE_T)(((SIZE_T)base)+((SIZE_T)offset))

//
//  The following macros are used to test and manipulate special kernel
//  handles.  A kernel handle is just a regular handle with its sign
//  bit set.  But must exclude -1 and -2 values which are the current
//  process and current thread constants.
//

#if defined(_WIN64)
#define KERNEL_HANDLE_MASK ((ULONG_PTR)((SSIZE_T)0xFFFFFFFF80000000))
#else
#define KERNEL_HANDLE_MASK ((ULONG_PTR)((SSIZE_T)0x80000000))
#endif

#define IsKernelHandle(H,M)                                \
	(((KERNEL_HANDLE_MASK & (ULONG_PTR)(H)) == KERNEL_HANDLE_MASK) && \
	((M) == KernelMode) &&                                \
	((H) != NtCurrentThread()) &&                         \
	((H) != NtCurrentProcess()))

#define EncodeKernelHandle(H) (HANDLE)(KERNEL_HANDLE_MASK | (ULONG_PTR)(H))

#define DecodeKernelHandle(H) (HANDLE)(KERNEL_HANDLE_MASK ^ (ULONG_PTR)(H))

//
// This function is from WRK-v1.2
// File: WRK-v1.2/public/sdk/inc/ntldr.h
// Prototype:	LDR_VIEW_TO_DATAFILE(x)
//				LDR_IS_DATAFILE(x)				
//				LDR_IS_VIEW(x)
//				LDR_DATAFILE_TO_VIEW(x)
// 
#define LDR_VIEW_TO_DATAFILE(x) ((PVOID)(((ULONG_PTR)(x)) |  (ULONG_PTR)1))
#define LDR_IS_DATAFILE(x)              (((ULONG_PTR)(x)) &  (ULONG_PTR)1)
#define LDR_IS_VIEW(x)                  (!LDR_IS_DATAFILE(x))
#define LDR_DATAFILE_TO_VIEW(x) ((PVOID)(((ULONG_PTR)(x)) & ~(ULONG_PTR)1))

//
// This function is from WRK-v1.2
// File: WRK-v1.2/public/sdk/inc/ntrtl.h
// Prototype:	RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK
// 
#define RTL_IMAGE_NT_HEADER_EX_FLAG_NO_RANGE_CHECK (0x00000001)

/************************************************************************/
/* Define Structure                                                     */
/************************************************************************/
//
// LDR_DATA_TABLE_ENTRY
//
typedef struct _LDR_DATA_TABLE_ENTRY {
	LIST_ENTRY		InLoadOrderLinks;
	LIST_ENTRY		InMemoryOrderLinks;
	union{
		LIST_ENTRY	InInitializationOrderLinks;
		LIST_ENTRY	InProgressLinks;
	};
	PVOID			DllBase;
	PVOID			EntryPoint;
	ULONG32			SizeOfImage;
	UNICODE_STRING	FullDllName;
	UNICODE_STRING	BaseDllName;
	ULONG32			Flags;
	UINT16			LoadCount;
	UINT16			TlsIndex;
	union {
		LIST_ENTRY	HashLinks;
		struct {
			PVOID	SectionPointer;
			ULONG32	CheckSum;
		};
	};
	union {
		struct {
			ULONG32 TimeDateStamp;
			PVOID	LoadedImports;
		};
	};
	PVOID			EntryPointActivationContext;
	PVOID			PatchInformation;
} LDR_DATA_TABLE_ENTRY, *PLDR_DATA_TABLE_ENTRY;

//
// Service descriptor table
//
#pragma pack(1)	
typedef struct _KSERVICE_TABLE_DESCRIPTOR {
	//
	// Table containing Services elements of pointers to service handler
	// functions, indexed by service ID.
	//
	PULONG_PTR	ServiceTableBase;
	//
	// Table that counts how many times each service is used. This table
	// is only updated in checked builds.
	//
	PULONG	ServiceCounterTableBase; 
	//
	// Number of services contained in this table.
	//
	ULONG	NumberOfServices;
	//
	// Table containing the number of bytes of parameters the handler
	// function takes.
	//
	PUCHAR	ParamTableBase;
} KSERVICE_TABLE_DESCRIPTOR, *PKSERVICE_TABLE_DESCRIPTOR;
#pragma pack()

#if defined(_M_AMD64)
extern KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
#else
__declspec(dllimport) KSERVICE_TABLE_DESCRIPTOR KeServiceDescriptorTable;
//extern PServiceDescriptorTableEntry KeServiceDescriptorTable;
#endif

//
// SYSTEM_MODULE_LIST
//
typedef struct _RTL_PROCESS_MODULE_INFORMATION{
	HANDLE Section;                 // Not filled in
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;		// e.g., OffsetToFileName = 0x12, FullPathName = \WINDOWS\system32\ntkrnlpa.exe, strlen("\WINDOWS\system32\") = 0x12.
	UCHAR FullPathName[256];
}RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODUELS{
	ULONG	NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[1];
}RTL_PROCESS_MODUELS, *PRTL_PROCESS_MODUELS;

//
// SYSTEM_THREAD
//
typedef struct _SYSTEM_THREAD{
	LARGE_INTEGER	KernelTime;
	LARGE_INTEGER	UserTime;
	LARGE_INTEGER	CreateTime;
	ULONG			WaitTime;
	PVOID			StartAddress;
	CLIENT_ID		ClientId;
	KPRIORITY		Priority;
	LONG			BasePriority;
	ULONG			ContextSwitchCount;
	ULONG			State;
	KWAIT_REASON	WaitReason;
}SYSTEM_THREAD, *PSYSTEM_THREAD;

//
// SYSTEM_PROCESS_INFORMATION
//
typedef struct _SYSTEM_PROCESS_INFORMATION{
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG			Reserved2[2];
	ULONG			PrivatePageCount;
	VM_COUNTERS		VirtualMemoryCounters;
	IO_COUNTERS		IoCounters;
	SYSTEM_THREAD	Threads[0];
}SYSTEM_PROCESS_INFORMATION, *PSYSTEM_PROCESS_INFORMATION;

typedef struct _MEMORY_BASIC_INFORMATION {
	PVOID BaseAddress;
	PVOID AllocationBase;
	ULONG AllocationProtect;
	SIZE_T RegionSize;
	ULONG State;
	ULONG Protect;
	ULONG Type;
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

//
// this structure is used to irpfile.c
//
typedef struct _AUX_ACCESS_DATA {
	PPRIVILEGE_SET PrivilegesUsed;
	GENERIC_MAPPING GenericMapping;
	ACCESS_MASK AccessesToAudit;
	ACCESS_MASK MaximumAuditMask;
	ULONG Unknown[41];
} AUX_ACCESS_DATA, *PAUX_ACCESS_DATA;

//
// 
// 
typedef struct _CONTROL_AREA{
/* +0x000 */	PVOID			Segment;
/* +0x004 */	LIST_ENTRY		DereferenceList;
/* +0x00C */	ULONG			NumberOfSectionReference;
/* +0x010 */	ULONG			NumberOfPfnReference;
/* +0x014 */	ULONG			NumberOfMappedViews;
/* +0x018 */	ULONG			NumberOfSystemCacheViews;
/* +0x01C */	ULONG			NumberOfUserReference;
/* +0x000 */	union{
					ULONG		LongFlags;
					ULONG		Flags;
				}u;
/* +0x000 */	PFILE_OBJECT	FilePointer;
}CONTROL_AREA, *PCONTROL_AREA;
//
//
//
typedef struct _SEGMENT_OBJECT{
	PCONTROL_AREA	ControlArea;
	ULONG			TotalNumberOfPtes;
	ULONG			NonExtendedPtes;
	ULONG			Spare0;
}SEGMENT_OBJECT, *PSEGMENT_OBJECT;

//
//
// 
typedef struct _SECTION_OBJECT{
	PVOID			StartingVa;
	PVOID			EndingVa;
	PVOID			Parent;
	PVOID			LeftChild;
	PVOID			RightChild;
	PSEGMENT_OBJECT	Segment;
}SECTION_OBJECT, *PSECTION_OBJECT;

//
//
// 
typedef struct _HG_PEB{
	BOOLEAN		InheritedAddressSpace;
	BOOLEAN		ReadImageFileExecOptions;
	BOOLEAN		BeingDebugged;
	union{
		BOOLEAN	BitField;
		struct {
			BOOLEAN	ImageUsersLargePages:1;
			BOOLEAN SpareBits:7;
		};
	};
	PVOID		Mutant;
	PVOID		ImageBaseAddress;
	PVOID		Ldr;
	PVOID		ProcessParameters;
}HG_PEB, *PHG_PEB;

//
//
// 
typedef struct _PEB_LDR_DATA{
	ULONG32		Length;
	BOOLEAN		Initialized;
	HANDLE		SsHandle;
	LIST_ENTRY	InLoadOrderModuleList;
	LIST_ENTRY	InMemoryOrderModuleList;
	LIST_ENTRY	InInitializationOrderModuleList;
	PVOID		EntryInProgress;
}PEB_LDR_DATA, *PPEB_LDR_DATA;

/************************************************************************/
/* XP 32-Bit                                                            */
/************************************************************************/
typedef struct _SEGMENT_XP
{
	PVOID ControlArea;					// +0x000
	ULONG TotalNumberOfPtes;			// +0x004
	ULONG NonExtendedPtes;				// +0x008
	ULONG WritableUserReferences;		// +0x00c
	LARGE_INTEGER SizeOfSegment;		// +0x010 
	LARGE_INTEGER SegmentPteTemplate;	// +0x018
	ULONG NumberOfCommittedPages;		// +0x020
	PVOID ExtendInfo;					// +0x024
	PVOID SystemImageBase;				// +0x028
	PVOID BasedAddress;					// +0x02c
}SEGMENT_XP, *PSEGMENT_XP;

typedef struct _CONTROL_AREA_XP
{
	struct _SEGMENT_XP *Segment;
	LIST_ENTRY DereferenceList;
	ULONG NumberOfSectionReferences;
	ULONG NumberOfPfnReferences;
	ULONG NumberOfMappedViews;
	ULONG NumberOfSystemCacheViews;
	ULONG NumberOfUserReferences;
	union
	{
		ULONG LongFlags;
		ULONG Flags;
	} u;
	PFILE_OBJECT FilePointer;
} CONTROL_AREA_XP, *PCONTROL_AREA_XP;

typedef struct _MMVAD_XP
{
	ULONG StartingVpn;
	ULONG EndingVpn;
	struct _MMVAD_XP *Parent;
	struct _MMVAD_XP *LeftChild;
	struct _MMVAD_XP *RightChild;
	ULONG NumberGenericTableElements;
	PCONTROL_AREA_XP ControlArea;
}MMVAD_XP, *PMMVAD_XP;

/************************************************************************/
/* Win7 32-Bit                                                          */
/************************************************************************/
typedef struct _SEGMENT32_WIN7
{
	PVOID ControlArea;					// +0x000
	ULONG TotalNumberOfPtes;			// +0x004
	ULONG NonExtendedPtes;				// +0x008
	ULONG NumberOfCommittedPages;		// +0x00c
	LARGE_INTEGER SizeOfSegment;		// +0x010
	PVOID BasedAddress;					// +0x018
	ULONG SegmentLock;					// +0x01c
	ULONG SegmentFlags;					// +0x020
	ULONG u1;							// +0x024
	ULONG SystemImageBase;				// +0x028
}SEGMENT32_WIN7, *PSEGMENT32_WIN7;

typedef struct _CONTROL_AREA32_WIN7
{
	struct _SEGMENT32_WIN7 *Segment;
	LIST_ENTRY DereferenceList;
	ULONG NumberOfSectionReferences;
	ULONG NumberOfPfnReferences;
	ULONG NumberOfMappedViews;
	ULONG NumberOfSystemCacheViews;
	ULONG NumberOfUserReferences;
	union
	{
		ULONG LongFlags;
		ULONG Flags;
	} u;
	PFILE_OBJECT FilePointer;
} CONTROL_AREA32_WIN7, *PCONTROL_AREA32_WIN7;

typedef struct _SUBSECTION32_WIN7
{
	PCONTROL_AREA32_WIN7 ControlArea;
}SUBSECTION32_WIN7, *PSUBSECTION32_WIN7;

typedef struct _MMVAD32_WIN7
{
	struct _MMVAD32_WIN7 *Parent;
	struct _MMVAD32_WIN7 *LeftChild;
	struct _MMVAD32_WIN7 *RightChild;
	ULONG StartingVpn;
	ULONG EndingVpn;
	ULONG NumberGenericTableElements;
	ULONG PushLock;
	ULONG u5;
	ULONG u2;
	PSUBSECTION32_WIN7 Subsection;
}MMVAD32_WIN7, *PMMVAD32_WIN7;

/************************************************************************/
/* Win7 64-Bit                                                          */
/************************************************************************/
typedef struct _EX_FAST_REF64_WIN7      // 3 elements, 0x8 bytes (sizeof) 
{                                                                  
	union                        // 3 elements, 0x8 bytes (sizeof) 
	{                                                              
		/*0x000*/         VOID*        Object;                                       
		/*0x000*/         UINT64       RefCnt : 4; // 0 BitPosition                  
		/*0x000*/         UINT64       Value;                                        
	};                                                             
}EX_FAST_REF64_WIN7, *PEX_FAST_REF64_WIN7;

typedef struct _SEGMENT64_WIN7                                         // 12 elements, 0x50 bytes (sizeof) 
{                                                                                                   
/*0x000*/	struct _CONTROL_AREA64_WIN7* ControlArea;                                                              
/*0x008*/	ULONG32						TotalNumberOfPtes;                                                                 
/*0x00C*/	ULONG32						SegmentFlags;                         // 12 elements, 0x4 bytes (sizeof)  
/*0x010*/	UINT64						NumberOfCommittedPages;                                                            
/*0x018*/	LARGE_INTEGER				SizeOfSegment;                                                         
/*0x020*/	VOID*						BasedAddress;                                                                  
/*0x028*/	UINT64						SegmentLock;                           // 7 elements, 0x8 bytes (sizeof)                                                                                              
/*0x030*/	UINT64						u1;                                           
/*0x038*/	VOID*						FirstMappedVa;                                                                                           
/*0x040*/	VOID*						PrototypePte;                                                                      
}SEGMENT64_WIN7, *PSEGMENT64_WIN7;

typedef struct _CONTROL_AREA64_WIN7                                      // 16 elements, 0x80 bytes (sizeof) 
{                                                                                                     
/*0x000*/     struct _SEGMENT64_WIN7*	Segment;                                                                         
/*0x008*/     struct _LIST_ENTRY DereferenceList;                           // 2 elements, 0x10 bytes (sizeof)  
/*0x018*/     UINT64       NumberOfSectionReferences;                                                           
/*0x020*/     UINT64       NumberOfPfnReferences;                                                               
/*0x028*/     UINT64       NumberOfMappedViews;                                                                 
/*0x030*/     UINT64       NumberOfUserReferences;                                                              
	union                                                         // 2 elements, 0x4 bytes (sizeof)   
	{                                                                                                 
/*0x038*/	ULONG32						LongFlags;                                                                       
/*0x038*/	UINT32						Flags;                            // 27 elements, 0x4 bytes (sizeof)  
	}u;                                                                                               
/*0x03C*/     ULONG32      FlushInProgressCount;                                                                
/*0x040*/     struct _EX_FAST_REF64_WIN7 FilePointer;                              // 3 elements, 0x8 bytes (sizeof)   
/*0x048*/     LONG32       ControlAreaLock;
}CONTROL_AREA64_WIN7, *PCONTROL_AREA64_WIN7;

typedef struct _SUBSECTION64_WIN7                          // 9 elements, 0x38 bytes (sizeof) 
{                                                                                      
/*0x000*/	struct _CONTROL_AREA64_WIN7* ControlArea;                                    
}SUBSECTION64_WIN7, *PSUBSECTION64_WIN7;                                          

typedef struct _MMVAD64_WIN7                                // 9 elements, 0x80 bytes (sizeof) 
{                                                                                       
/*0x000*/	struct _MMVAD64_WIN7*	Parent;
/*0x008*/	struct _MMVAD64_WIN7*	LeftChild;
/*0x010*/	struct _MMVAD64_WIN7*	RightChild;                                                    
/*0x018*/	UINT64					StartingVpn;
/*0x020*/	UINT64					EndingVpn;
			union                                            // 2 elements, 0x4 bytes (sizeof)  
			{                                                                                   
/*0x028*/         ULONG32			LongFlags2;                                                        
/*0x028*/         ULONG32			VadFlags2;
			}u;                                                                           
/*0x030*/   UINT64					PushLock;
			union                                      // 2 elements, 0x8 bytes (sizeof)   
			{                                                                              
/*0x038*/         UINT64			LongFlags3;                                                   
/*0x038*/         UINT64			VadFlags3;        // 8 elements, 0x8 bytes (sizeof)   
			}u5;                                                                           
			union                                      // 2 elements, 0x4 bytes (sizeof)   
			{                                                                              
/*0x040*/         ULONG32			LongFlags2;                                                   
/*0x040*/         ULONG32			VadFlags2;        // 9 elements, 0x4 bytes (sizeof)   
			}u2;
/*0x048*/	struct _SUBSECTION64_WIN7* Subsection;                                                                                
/*0x050*/	UINT64 Placeholder2; //struct _MMPTE* FirstPrototypePte;                                                   
/*0x058*/	UINT64 Placeholder3; //struct _MMPTE* LastContiguousPte;                                                   
/*0x060*/	struct _LIST_ENTRY ViewLinks;                    // 2 elements, 0x10 bytes (sizeof) 
/*0x070*/	UINT64 Placeholder5; //struct _EPROCESS* VadsProcess;                                                                                
}MMVAD64_WIN7, *PMMVAD64_WIN7;  
/************************************************************************/
/* Win8 32-Bit                                                          */
/************************************************************************/

typedef struct _SEGMENT_WIN8
{
	PVOID ControlArea;				// +0x000 
	ULONG TotalNumberOfPtes;		// +0x004 
	ULONG SegmentFlags;				// +0x008 
	ULONG NumberOfCommittedPages;	// +0x00c 
	LARGE_INTEGER SizeOfSegment;	// +0x010 
	PVOID BasedAddress;				// +0x018 
	ULONG SegmentLock;				// +0x01c 
	ULONG u1;						// +0x020 
	ULONG SystemImageBase;			// +0x024 			
}SEGMENT_WIN8, *PSEGMENT_WIN8;

typedef struct _CONTROL_AREA_WIN8
{
	struct _SEGMENT_WIN8 *Segment;
	LIST_ENTRY DereferenceList;
	ULONG NumberOfSectionReferences;
	ULONG NumberOfPfnReferences;
	ULONG NumberOfMappedViews;
	ULONG NumberOfSystemCacheViews;
	ULONG NumberOfUserReferences;
	union
	{
		ULONG LongFlags;
		ULONG Flags;
	} u;
	PFILE_OBJECT FilePointer;
} CONTROL_AREA_WIN8, *PCONTROL_AREA_WIN8;

typedef struct _SUBSECTION_WIN8
{
	PCONTROL_AREA_WIN8 ControlArea;
}SUBSECTION_WIN8, *PSUBSECTION_WIN8;

typedef struct _MMVAD_WIN8
{
	struct _MMVAD_WIN8 *Parent;
	struct _MMVAD_WIN8 *LeftChild;
	struct _MMVAD_WIN8 *RightChild;
	ULONG StartingVpn;
	ULONG EndingVpn;
	ULONG PushLock;
	ULONG u;
	ULONG u1;
	ULONG EventList;
	ULONG ReferenceCuont;
	ULONG u2;
	PSUBSECTION_WIN8 Subsection;
}MMVAD_WIN8, *PMMVAD_WIN8;

/************************************************************************/
/* Win8 64-Bit                                                          */
/************************************************************************/
typedef struct _EX_FAST_REF64_WIN8      // 3 elements, 0x8 bytes (sizeof) 
{                                                                  
	union                        // 3 elements, 0x8 bytes (sizeof) 
	{                                                              
		/*0x000*/         VOID*        Object;                                       
		/*0x000*/         UINT64       RefCnt : 4; // 0 BitPosition                  
		/*0x000*/         UINT64       Value;                                        
	};                                                             
}EX_FAST_REF64_WIN8, *PEX_FAST_REF64_WIN8;

typedef struct _SEGMENT64_WIN8                                         // 12 elements, 0x50 bytes (sizeof) 
{                                                                                                   
	/*0x000*/     struct _CONTROL_AREA64_WIN8* ControlArea;                                                              
	/*0x008*/     ULONG32      TotalNumberOfPtes;                                                                 
	/*0x00C*/     ULONG32		SegmentFlags;                         // 12 elements, 0x4 bytes (sizeof)  
	/*0x010*/     UINT64       NumberOfCommittedPages;                                                            
	/*0x018*/     LARGE_INTEGER		SizeOfSegment;                                                         
	/*0x020*/     VOID*        BasedAddress;                                                                  
	/*0x028*/     UINT64		SegmentLock;                           // 7 elements, 0x8 bytes (sizeof)                                                                                              
	/*0x030*/     UINT64       u1;                                           
	/*0x038*/     VOID*        FirstMappedVa;                                                                                           
	/*0x040*/     VOID*		PrototypePte;                                                                       
}SEGMENT64_WIN8, *PSEGMENT64_WIN8;

typedef struct _CONTROL_AREA64_WIN8                                      // 16 elements, 0x80 bytes (sizeof) 
{                                                                                                     
/*0x000*/     struct _SEGMENT64_WIN8*	Segment;                                                                         
/*0x008*/     struct _LIST_ENTRY		ListHead;                           // 2 elements, 0x10 bytes (sizeof)  
/*0x018*/     UINT64		NumberOfSectionReferences;                                                           
/*0x020*/     UINT64		NumberOfPfnReferences;                                                               
/*0x028*/     UINT64		NumberOfMappedViews;                                                                 
/*0x030*/     UINT64		NumberOfUserReferences;                                                              
	union                                                         // 2 elements, 0x4 bytes (sizeof)   
	{                                                                                                 
/*0x038*/     ULONG32		LongFlags;                                                                       
/*0x038*/     UINT32		Flags;                            // 27 elements, 0x4 bytes (sizeof)  
	}u;                                                                                               
/*0x03C*/     ULONG32      FlushInProgressCount;                                                                
/*0x040*/     struct _EX_FAST_REF64_WIN8 FilePointer;                              // 3 elements, 0x8 bytes (sizeof)   
/*0x048*/     LONG32       ControlAreaLock;
}CONTROL_AREA64_WIN8, *PCONTROL_AREA64_WIN8;

typedef struct _SUBSECTION64_WIN8                          // 9 elements, 0x38 bytes (sizeof) 
{                                                                                      
/*0x000*/     struct _CONTROL_AREA64_WIN8* ControlArea;                                    
}SUBSECTION64_WIN8, *PSUBSECTION64_WIN8;

typedef struct _MM_AVL_NODE64_WIN8          // 3 elements, 0x18 bytes (sizeof) 
{                                                                       
	union                            // 2 elements, 0x8 bytes (sizeof)  
	{                                                                   
/*0x000*/	INT64        Balance : 2;    // 0 BitPosition                   
/*0x000*/	struct _MM_AVL_NODE64_WIN8* Parent;                                    
	}u1;                                                                
/*0x008*/	struct _MM_AVL_NODE64_WIN8* LeftChild;                                     
/*0x010*/	struct _MM_AVL_NODE64_WIN8* RightChild;                                    
}MM_AVL_NODE64_WIN8, *PMM_AVL_NODE64_WIN8;                                      

typedef struct _MMVAD_SHORT64_WIN8                // 8 elements, 0x40 bytes (sizeof) 
{                                                                             
/*0x000*/     struct _MM_AVL_NODE64_WIN8 VadNode;           // 3 elements, 0x18 bytes (sizeof) 
/*0x018*/     ULONG32      StartingVpn;                                                 
/*0x01C*/     ULONG32      EndingVpn;                                                   
/*0x020*/     UINT64	PushLock;         // 7 elements, 0x8 bytes (sizeof)  
	union                                  // 2 elements, 0x4 bytes (sizeof)  
	{                                                                         
/*0x028*/	ULONG32		LongFlags;                                               
/*0x028*/	ULONG32		VadFlags;      // 9 elements, 0x4 bytes (sizeof)  
	}u;                                                                       
	union                                  // 2 elements, 0x4 bytes (sizeof)  
	{                                                                         
/*0x02C*/         ULONG32      LongFlags1;                                              
/*0x02C*/         ULONG32	VadFlags1;    // 2 elements, 0x4 bytes (sizeof)  
	}u1;                                                                      
/*0x030*/     PVOID EventList;                                    
/*0x038*/     LONG32       ReferenceCount;                                              
}MMVAD_SHORT64_WIN8, *PMMVAD_SHORT64_WIN8;                                             

typedef struct _MMVAD64_WIN8                                // 9 elements, 0x80 bytes (sizeof) 
{                                                                                       
/*0x000*/     struct _MMVAD_SHORT64_WIN8 Core;                        // 8 elements, 0x40 bytes (sizeof) 
	union                                            // 2 elements, 0x4 bytes (sizeof)  
	{                                                                                   
/*0x040*/	ULONG32     LongFlags2;                                                        
/*0x040*/	ULONG32		VadFlags2;              // 7 elements, 0x4 bytes (sizeof)  
	}u2;                                                                                                                                                                  
/*0x048*/     struct _SUBSECTION64_WIN8* Subsection;                                                                                
/*0x050*/     UINT64 Placeholder2; //struct _MMPTE* FirstPrototypePte;                                                   
/*0x058*/     UINT64 Placeholder3; //struct _MMPTE* LastContiguousPte;                                                   
/*0x060*/     struct _LIST_ENTRY ViewLinks;                    // 2 elements, 0x10 bytes (sizeof) 
/*0x070*/     UINT64 Placeholder5; //struct _EPROCESS* VadsProcess;                                                      
	union                                            // 2 elements, 0x8 bytes (sizeof)  
	{
		UINT64	Placeholder6;
/*0x078*/         //struct _MI_VAD_SEQUENTIAL_INFO SequentialVa; // 2 elements, 0x8 bytes (sizeof)  
/*0x078*/         //struct _MMEXTEND_INFO* ExtendedInfo;                                            
	}u4;                                                                                
}MMVAD64_WIN8, *PMMVAD64_WIN8;
//
// _OBJECT_HEADER_CREATOR_INFO
//
typedef struct _OBJECT_HEADER_CREATOR_INFO{
	LIST_ENTRY	TypeList;
	PVOID		CreatorUniqueProcess;
	UINT16		CreatorBackTraceIndex;
	UINT16		Reserved;
}OBJECT_HEADER_CREATOR_INFO, *POBJECT_HEADER_CREATOR_INFO;
//
//
// 
typedef struct _OBJECT_DIRECTORY_ENTRY
{                                                                                
/*0x000*/	struct _OBJECT_DIRECTORY_ENTRY* ChainLink;                                   
/*0x004*/	VOID*        Object;                                                                 
/*0x008*/	ULONG32      HashValue;                                             
}OBJECT_DIRECTORY_ENTRY, *POBJECT_DIRECTORY_ENTRY; 
//
//
// 
typedef struct _OBJECT_DIRECTORY                     // 6 elements, 0xA4 bytes (sizeof) 
{                                                                                       
/*0x000*/	struct _OBJECT_DIRECTORY_ENTRY* HashBuckets[37];                                            
}OBJECT_DIRECTORY, *POBJECT_DIRECTORY; 
//
//
// 
#if defined(_WIN64)
typedef ULONG SYSINF_PAGE_COUNT;
#else
typedef SIZE_T SYSINF_PAGE_COUNT;
#endif

typedef struct _SYSTEM_BASIC_INFORMATION {
	ULONG Reserved;
	ULONG TimerResolution;
	ULONG PageSize;
	SYSINF_PAGE_COUNT NumberOfPhysicalPages;
	SYSINF_PAGE_COUNT LowestPhysicalPageNumber;
	SYSINF_PAGE_COUNT HighestPhysicalPageNumber;
	ULONG AllocationGranularity;
	ULONG_PTR MinimumUserModeAddress;
	ULONG_PTR MaximumUserModeAddress;
	ULONG_PTR ActiveProcessorsAffinityMask;
	CCHAR NumberOfProcessors;
} SYSTEM_BASIC_INFORMATION, *PSYSTEM_BASIC_INFORMATION;

//
// Object Information Classes
//
// /* WDK defined*/
//typedef enum _OBJECT_INFORMATION_CLASS {
//	ObjectBasicInformation=0,
//	ObjectTypeInformation=2,
//} OBJECT_INFORMATION_CLASS;

typedef enum _HG_OBJECT_INFORMATION_CLASS {
 	HgObjectBasicInformation,
 	HgObjectNameInformation,
 	HgObjectTypeInformation,
 	HgObjectTypesInformation,
 	HgObjectHandleFlagInformation,
 	HgObjectSessionInformation,
 	HgMaxObjectInfoClass  // MaxObjectInfoClass should always be the last enum
 } HG_OBJECT_INFORMATION_CLASS;

typedef struct _OBJECT_BASIC_INFORMATION {
	ULONG Attributes;
	ACCESS_MASK GrantedAccess;
	ULONG HandleCount;
	ULONG PointerCount;
	ULONG PagedPoolCharge;
	ULONG NonPagedPoolCharge;
	ULONG Reserved[ 3 ];
	ULONG NameInfoSize;
	ULONG TypeInfoSize;
	ULONG SecurityDescriptorSize;
	LARGE_INTEGER CreationTime;
} OBJECT_BASIC_INFORMATION, *POBJECT_BASIC_INFORMATION;

// typedef struct _OBJECT_NAME_INFORMATION {               // ntddk wdm nthal
// 	UNICODE_STRING Name;                                // ntddk wdm nthal
// } OBJECT_NAME_INFORMATION, *POBJECT_NAME_INFORMATION;   // ntddk wdm nthal

typedef struct _OBJECT_TYPE_INFORMATION {
	UNICODE_STRING TypeName;
	ULONG TotalNumberOfObjects;
	ULONG TotalNumberOfHandles;
	ULONG TotalPagedPoolUsage;
	ULONG TotalNonPagedPoolUsage;
	ULONG TotalNamePoolUsage;
	ULONG TotalHandleTableUsage;
	ULONG HighWaterNumberOfObjects;
	ULONG HighWaterNumberOfHandles;
	ULONG HighWaterPagedPoolUsage;
	ULONG HighWaterNonPagedPoolUsage;
	ULONG HighWaterNamePoolUsage;
	ULONG HighWaterHandleTableUsage;
	ULONG InvalidAttributes;
	GENERIC_MAPPING GenericMapping;
	ULONG ValidAccessMask;
	BOOLEAN SecurityRequired;
	BOOLEAN MaintainHandleCount;
	ULONG PoolType;
	ULONG DefaultPagedPoolCharge;
	ULONG DefaultNonPagedPoolCharge;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

typedef struct _OBJECT_TYPES_INFORMATION {
	ULONG NumberOfTypes;
	// OBJECT_TYPE_INFORMATION TypeInformation;
} OBJECT_TYPES_INFORMATION, *POBJECT_TYPES_INFORMATION;

typedef struct _OBJECT_HANDLE_FLAG_INFORMATION {
	BOOLEAN Inherit;
	BOOLEAN ProtectFromClose;
} OBJECT_HANDLE_FLAG_INFORMATION, *POBJECT_HANDLE_FLAG_INFORMATION;

/************************************************************************/
/* Define Function Pointer                                              */
/************************************************************************/
//
//
//
typedef
NTSTATUS
	(* pfnRtlGetVersion)(
	__out	PRTL_OSVERSIONINFOW lpVersionInfomation
	);

//
//
// 
typedef
NTSTATUS
(*pfnObReferenceObjectByName)(
	__in		PUNICODE_STRING		ObjectName,
	__in		ULONG				Attributes,
	__in_opt	PACCESS_STATE		AccessState,
	__in_opt	PACCESS_MASK		DesiredAccess,
	__in		POBJECT_TYPE		ObjectType,
	__in		KPROCESSOR_MODE		AccessMode,
	__inout_opt	PVOID				ParseContext,
	__out		PVOID				*Object
	);

//
//
// 
typedef
VOID
(*pfnKeStackAttachProcess)(
	__in	PKPROCESS		Process,
	__out	PRKAPC_STATE	ApcState
	);

//
//
// 
typedef
VOID
(*pfnKeUnstackDetachProcess)(
	__in	PRKAPC_STATE	ApcState
	);

//
//
// 
typedef
BOOLEAN
(*pfnMmIsAddressValid)(
	__in	PVOID	VirtualAddress
	);

//
//
// 
typedef
PVOID
(*pfnExAllocatePoolWithTag)(
	__in	__drv_strictTypeMatch(__drv_typeExpr) POOL_TYPE PoolType,
	__in	SIZE_T	NumberOfBytes,
	__in	ULONG	Tag
	);

//
//
// 
typedef
NTSTATUS
(*pfnNtQuerySystemInformation)(
	__in		SYSTEM_INFORMATION_CLASS	SystemInformationClass,
	__out		PVOID	SystemInformation,
	__in		SIZE_T	SystemInformationLength,
	__out_opt	PSIZE_T	ReturnLength
	);

//
//
// 
typedef
VOID
(*pfnExFreePoolWithTag)(
	__in	PVOID	P,
	__in	ULONG	Tag
	);

//
//
// 
typedef
NTSTATUS
(*pfnPsLookupProcessByProcessId)(
	__in	HANDLE		ProcessId,
	__out	PEPROCESS	*Process
	);

//
//
// 
typedef
PETHREAD
(*pfnPsGetCurrentThread)(
	VOID
	);

//
//
// 
typedef
LONG_PTR
(FASTCALL *pfnObfDereferenceObject)(
	__in	PVOID	Object
	);

//
//
// 
typedef
VOID
(__stdcall *pfnProbeForRead)(
	__in	PVOID	Address,
	__in	SIZE_T	Length,
	__in	ULONG	Alignment
	);

//
//
// 
typedef
VOID
(__stdcall *pfnProbeForWrite)(
	__in	PVOID	Address,
	__in	SIZE_T	Length,
	__in	ULONG	Alignment
	);

//
//
// 
typedef
HANDLE
(*pfnPsGetProcessId)(
	__in	PEPROCESS	Process
	);

//
//
// 
typedef
NTSTATUS
(*pfnObQueryNameString)(
	__in		PVOID						Object,
	__out_opt	POBJECT_NAME_INFORMATION	ObjectNameInfo,
	__in		ULONG						Length,
	__out		PULONG						ReturnLength
	);

//
//
// 
typedef
NTSTATUS
(*pfnIoQueryFileDosDeviceName)(
	__in	PFILE_OBJECT				FileObject,
	__out	POBJECT_NAME_INFORMATION	*ObjectNameInformation
	);

//
//
//
typedef
NTSTATUS
(*pfnIoVolumeDeviceToDosName)(
	__in	PVOID			VolumeDeviceObject,
	__out	PUNICODE_STRING	DosName
	);

//
//
// 
typedef
PEPROCESS
(*pfnIoGetCurrentProcess)(
	VOID
	);

//
//
// 
typedef
KAFFINITY
(*pfnKeQueryActiveProcessors)(
	VOID
	);

//
//
//
// 
typedef
VOID
(*pfnKeInitializeEvent)(
	__in	PRKEVENT	Event,
	__in	EVENT_TYPE	Type, 
	__in	BOOLEAN		State 
	);

//
//
// 
typedef
VOID
(*pfnKeInitializeDpc)(
	__in	PRKDPC				Dpc,
	__in	PKDEFERRED_ROUTINE	DeferredRoutine,
	__in	PVOID				DeferredContext
	);

//
//
// 
typedef
VOID
(*pfnKeSetTargetProcessorDpc)(
	__in	PRKDPC	Dpc,
	__in	CCHAR	Number
	);

//
//
// 
typedef
VOID
(*pfnKeSetImportanceDpc)(
	__in	PRKDPC			Dpc,
	__in	KDPC_IMPORTANCE	Importance
	);

//
//
// 
typedef
BOOLEAN
(*pfnKeInsertQueueDpc)(
	__in	PRKDPC	Dpc,
	__in	PVOID	SystemArgument1,
	__in	PVOID	SystemArgument2
	);

//
//
// 
typedef
NTSTATUS
(*pfnKeWaitForSingleObject)(
	__in		PVOID			Object,
	__in		KWAIT_REASON	WaitReason,
	__in		KPROCESSOR_MODE	WaitMode,
	__in		BOOLEAN			Alertable,
	__in_opt	PLARGE_INTEGER	Timeout
	);

//
//
// 
typedef
LONG
(*pfnKeSetEvent)(
	__in	PRKEVENT	Event,
	__in	KPRIORITY	Increment,
	__in	BOOLEAN		Wait
	);

//
//
// 
typedef
POBJECT_TYPE
(*pfnObGetObjectType)(
	__in	PVOID	pObject
	);

//
//
// 
typedef
PEPROCESS
(*pfnIoThreadToProcess)(
	__in	PETHREAD	Thread
	);

//
//
// 
typedef NTSTATUS (*pfnObReferenceObjectByPointer)(
	__in	PVOID				Object,
	__in	ACCESS_MASK			DesireAccess,
	__in	POBJECT_TYPE		ObjectType,
	__in	KPROCESSOR_MODE		AccessMode);

//
//
// 
typedef	HANDLE (*pfnPsGetThreadId)(
	__in	PETHREAD	Thread);

//
//
//
typedef NTSTATUS (__stdcall *pfnZwOpenDirectoryObject)(
	__out	PHANDLE				DirectoryHandle,
	__in	ACCESS_MASK			DesiredAccess,
	__in	POBJECT_ATTRIBUTES	ObjectAttributes);
//
//
//
typedef
NTSTATUS
(*pfnObReferenceObjectByHandle)(
	__in HANDLE Handle,
	__in ACCESS_MASK DesiredAccess,
	__in_opt POBJECT_TYPE ObjectType,
	__in KPROCESSOR_MODE AccessMode,
	__out PVOID *Object,
	__out_opt POBJECT_HANDLE_INFORMATION HandleInformation
	);
//
//
//
typedef
BOOLEAN
(*pfnMmIsNonPagedSystemAddressValid)(
	__in PVOID VirtualAddress
	);
//
//
//
typedef
PHYSICAL_ADDRESS
(*pfnMmGetPhysicalAddress)(
	__in PVOID BaseAddress
	);
//
//
//
typedef
PVOID
	(*pfnMmGetVirtualForPhysical)(
	__in PHYSICAL_ADDRESS PhysicalAddress
	);
//
//
// 
typedef
PMDL
(*pfnIoAllocateMdl)(
	__in_opt __drv_aliasesMem PVOID VirtualAddress,
	__in ULONG Length,
	__in BOOLEAN SecondaryBuffer,
	__in BOOLEAN ChargeQuota,
	__inout_opt PIRP Irp
	);
//
//
//
typedef
VOID
(*pfnMmBuildMdlForNonPagedPool) (
	__inout PMDLX MemoryDescriptorList
	);
//
//
//
typedef
PVOID
(*pfnMmMapLockedPagesSpecifyCache) (
	__in PMDLX MemoryDescriptorList,
	__in __drv_strictType(KPROCESSOR_MODE/enum _MODE,__drv_typeConst) 
	KPROCESSOR_MODE AccessMode,
	__in __drv_strictTypeMatch(__drv_typeCond) MEMORY_CACHING_TYPE CacheType,
	__in_opt PVOID BaseAddress,
	__in ULONG BugCheckOnFailure,
	__in __drv_strictTypeMatch(__drv_typeCond) MM_PAGE_PRIORITY Priority
	);
//
//
// 
typedef
VOID
(*pfnMmUnmapLockedPages) (
	__in PVOID BaseAddress,
	__in PMDL MemoryDescriptorList
	);
//
//
// 
typedef
VOID
(*pfnIoFreeMdl)(
	PMDL Mdl
	);
//
//
//
typedef
NTSTATUS
(__stdcall *pfnNtOpenSymbolicLinkObject)(
	__out PHANDLE LinkHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	);
//
//
//
typedef
NTSTATUS
(__stdcall *pfnNtQuerySymbolicLinkObject)(
	__in HANDLE LinkHandle,
	__inout PUNICODE_STRING LinkTarget,
	__out_opt PULONG ReturnedLength
	);
//
//
// 
typedef
NTSTATUS
(NTAPI *pfnNtClose) (
	__in HANDLE Handle
	);
//
//
// 
typedef
NTSTATUS
(*pfnIoCreateFile)(
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
//
//
//
typedef
NTSTATUS
(NTAPI *pfnZwOpenKey)(
	__out PHANDLE KeyHandle,
	__in ACCESS_MASK DesiredAccess,
	__in POBJECT_ATTRIBUTES ObjectAttributes
	);
//
//
//
typedef
NTSTATUS
(NTAPI *pfnZwEnumerateKey)(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_INFORMATION_CLASS KeyInformationClass,
	__out_bcount_opt(Length) PVOID KeyInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	);
//
//
//
typedef
NTSTATUS
(NTAPI *pfnZwEnumerateValueKey)(
	__in HANDLE KeyHandle,
	__in ULONG Index,
	__in KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	__out_bcount_opt(Length) PVOID KeyValueInformation,
	__in ULONG Length,
	__out PULONG ResultLength
	);
//
//
//
typedef
NTSTATUS
(*pfnPsSetCreateProcessNotifyRoutine)(
	__in PCREATE_PROCESS_NOTIFY_ROUTINE NotifyRoutine,
	__in BOOLEAN Remove
	);
//
//
// 
typedef
NTSTATUS
(*pfnPsSetCreateThreadNotifyRoutine)(
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	);
//
//
//
typedef
NTSTATUS
(*pfnPsSetLoadImageNotifyRoutine)(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	);
//
//
//
typedef
NTSTATUS
(*pfnPsRemoveCreateThreadNotifyRoutine) (
	__in PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	);
//
//
//
typedef
NTSTATUS
(*pfnPsRemoveLoadImageNotifyRoutine)(
	__in PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	);
//
//
//
typedef
NTSTATUS
(*pfnIoRegisterShutdownNotification)(
	__in PDEVICE_OBJECT DeviceObject
	);
//
//
//
typedef
NTSTATUS
(*pfnCmUnRegisterCallback)(
	__in LARGE_INTEGER    Cookie
	);
//
//
// 
typedef
VOID
(*pfnIoUnregisterShutdownNotification)(
	__in PDEVICE_OBJECT DeviceObject
	);
//
//
// 
typedef
NTSTATUS 
(*pfnNtQueryVirtualMemory)(
	__in HANDLE ProcessHandle,
	__in PVOID BaseAddress,
	__in MEMORY_INFORMATION_CLASS MemoryInformationClass,
	__out_bcount(MemoryInformationLength) PVOID MemoryInformation,
	__in SIZE_T MemoryInformationLength,
	__out_opt PSIZE_T ReturnLength
	);
//
//
//
typedef
NTSTATUS
(__stdcall *pfnNtProtectVirtualMemory)(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	UnsafeBaseAddress,
	__inout	PSIZE_T	UnsafeNumberOfBytesToProtect,
	__in	ULONG	NewAccessProtection,
	__out	PULONG	UnsafeOldAccessProtection);
//
// 
// 
typedef
NTSTATUS
(__stdcall *pfnNtFreeVirtualMemory)(
	__in	HANDLE	ProcessHandle,
	__inout	PVOID*	BaseAddress,
	__inout	PSIZE_T	RegionSize,
	__in	ULONG	FreeType);
//
//
// 
typedef
NTSTATUS
	(*pfnObOpenObjectByPointer)(
	__in PVOID  Object,
	__in ULONG  HandleAttributes,
	__in_opt PACCESS_STATE  PassedAccessState,
	__in ACCESS_MASK  DesiredAccess,
	__in POBJECT_TYPE  ObjectType,
	__in KPROCESSOR_MODE  AccessMode,
	__out	PHANDLE  Handle
	);
//
//
//
typedef
NTSTATUS
	(*pfnNtQueryObject) (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__out_bcount_opt(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength,
	__out_opt PULONG ReturnLength
	);

//
//
// 
typedef
NTSTATUS
(*pfnNtSetInformationObject) (
	__in HANDLE Handle,
	__in HG_OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__in_bcount(ObjectInformationLength) PVOID ObjectInformation,
	__in ULONG ObjectInformationLength
	);

#endif