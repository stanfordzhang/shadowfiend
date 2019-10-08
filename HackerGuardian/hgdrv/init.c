
#include "init.h"

#if defined (_WIN64)
#pragma  message("_x64  macro  activated!")  
#include "reload64.h"
#else
#pragma  message("_x86  macro  activated!")  
#include "reload32.h"
#endif

extern ULONG_PTR g_hg_map[HG_MAP_LENGTH];
extern PDRIVER_OBJECT	g_pHGDrvObj;

BOOL
InitializeEnvironment(
	PDRIVER_OBJECT pDrvObj
	)
{
	BOOL bRet = FALSE;
	WIN_VERSION WinVersion = eWin_Unknow;
	//////////////////////////////////////////////////////////////////////////
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PETHREAD	pEThread = HgKeGetCurrentThread();
	CHAR		PreMode;
	ULONG		ReturnLength = 0;
	PVOID		SystemInformation = NULL;
	SIZE_T		nRet = 0;
	PSYSTEM_PROCESS_INFORMATION pSpi = NULL;
	PEPROCESS	pEProcess = NULL;

	InitLog(("Entry InitializeEnvironment!\n"));
	memset((void*)g_hg_map, 0, HG_MAP_LENGTH * sizeof(ULONG));
	WinVersion = GetWindowsVersion();

	if (WinVersion != eWin_Unknow){
		PEPROCESS pEProcess = PsGetCurrentProcess();

		PVOID pKeServiceDescriptionTable = NULL;
		CHAR pszKeServiceDescriptionTable[] = "KeServiceDescriptorTable";
	
		g_pHGDrvObj = pDrvObj;
		g_hg_map[eHG_SystemProcess] = (ULONG_PTR)pEProcess;
		g_WindowsVersion = WinVersion;

		InitMapTable();

#if defined(_WIN64)		// (_WIN64) || (_M_IX86) || (_M_AMD64)
		if (ReloadKernel64()){
//			g_hg_map[eHG_ReloadNtosKrnlSuccess] = TRUE;
 		}
#else
		if (ReloadNtoskrnl32()){
			g_hg_map[eHG_ReloadNtosKrnlSuccess] = TRUE;
			g_bReloadKernelModule = TRUE;
		}else{
			InitLog(("Reload ntoskrnl.exe unsuccessfully!\n"));
		};
#endif
		GetNtoskrnlLdrDataTableEntry();
		InitWindowsApi();
		InitSSDT();

#if defined(_WIN64)
#else
		if (ReloadHalDll())	{
			InitLog(("Reload hal.dll successfully!\n"));
		}else{
			InitLog(("Reload hal.dll unsuccessfully!\n"));
		}
		
		if (ReloadWin32k())	{
			InitLog(("Reload win32k.dll successfully!\n"));
		}else{
			InitLog(("Reload win32k.dll unsuccessfully!\n"));
		}
		//InitShadowSSDT();
		//GetCommonDriverSizeAndBase();

		//InitNtfs();
		//InitFastfat()
#endif

		// Windows 7/8 64-Bit does not export KeServiceDescriptorTable symbol in export directory.
#if defined (_WIN64)
		InitLog(("Enter _Win64\n"));
		LookupSsdtAddress64((PVOID)g_hg_map[eHG_CurrentKernelBase]);
		InitLog(("%s(%d): Windows 64-Bit SSDT = 0x%p\n", __FILE__, __LINE__, g_hg_map[eHG_OriginalSSDT64]));
		InitLog(("%s(%d): Windows 64-Bit Shadow SSDT = 0x%p\n", __FILE__, __LINE__, g_hg_map[eHG_OriginalShadowSSDT64]));
		pKeServiceDescriptionTable = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginalSSDT64];
		InitLog(("Shadow SSDT Count = %d\n", ((PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginalShadowSSDT64])->NumberOfServices));
// 		if (g_hg_map[eHG_ReloadNtosKrnlSuccess] == TRUE){
// 			LookupSsdtAddress64((PVOID)g_hg_map[eHG_ReloadedKernelBase64]);
// 			InitLog(("Reloaded SSDT = 0x%p\n", g_hg_map[eHG_ReloadedSSDT64]));
// 		}
//		pKeServiceDescriptionTable = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_ReloadedSSDT64];
#else
		pKeServiceDescriptionTable = HgMmGetSystemRoutineAddress(pszKeServiceDescriptionTable);
		InitLog((" KeServiceDescriptorTable Address = 0x%p\n", &KeServiceDescriptorTable));
#endif
		g_hg_map[eHG_CurrentSSDT] = (ULONG_PTR)pKeServiceDescriptionTable;
		InitLog(("pKeServiceDescriptorTable Address = 0x%p\n", pKeServiceDescriptionTable));
		if (pKeServiceDescriptionTable != NULL){
			g_hg_map[eHG_SSDTCount] = ((PKSERVICE_TABLE_DESCRIPTOR)pKeServiceDescriptionTable)->NumberOfServices;
			InitLog(("SSDT Count = %d\n", g_hg_map[eHG_SSDTCount]));
		}else{
			InitLog(("pKeServiceDescriptorTable == NULL\n"));
		}

		bRet = TRUE;
	}

	return bRet;
}

WIN_VERSION
	GetWindowsVersion(
	)
{
	WIN_VERSION WinVersion = eWin_Unknow;
	RTL_OSVERSIONINFOEXW OSVerInfo;
	pfnRtlGetVersion pRtlGetVersion = NULL;
	WCHAR pwszRtlGetVersion[] = L"RtlGetVersion";

	pRtlGetVersion = (pfnRtlGetVersion)GetFunctionAddressByName(pwszRtlGetVersion);
	if (pRtlGetVersion != NULL){
		pRtlGetVersion((PRTL_OSVERSIONINFOW)&OSVerInfo);
	} else {
		PsGetVersion(&OSVerInfo.dwMajorVersion, 
			&OSVerInfo.dwMinorVersion, &OSVerInfo.dwBuildNumber, NULL);
	}

	InitLog(("Windows Build Number: %d\n", OSVerInfo.dwBuildNumber));
	g_WindowsBuildNumber = OSVerInfo.dwBuildNumber;

	if (OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 0){
		InitLog(("Windows 2000\n"));
		WinVersion = eWin_2000;
	}else if (OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 1){
		InitLog(("Windows XP\n"));
		WinVersion = eWin_XP;
	}else if (OSVerInfo.dwMajorVersion == 5 && OSVerInfo.dwMinorVersion == 2)
	{
		if (OSVerInfo.wServicePackMajor == 0){
			InitLog(("Windows 2003\n"));
			WinVersion = eWin_2003;
		}else{
			InitLog(("Windows 2003 SP1 SP2\n"));
			WinVersion = eWin_2003_SP1_SP2;
		}
	}else if (OSVerInfo.dwMajorVersion == 6 && OSVerInfo.dwMinorVersion == 0){
		if (OSVerInfo.dwBuildNumber == 6000){
			InitLog(("Windows Vista\n"));
			WinVersion = eWin_Vista;
		}else if (OSVerInfo.dwBuildNumber == 6001 || OSVerInfo.dwBuildNumber == 6002){
			InitLog(("Windows Vista SP1 SP2\n"));
			WinVersion = eWin_Vista_SP1_SP2;
		}
	}else if (OSVerInfo.dwMajorVersion == 6 && OSVerInfo.dwMinorVersion == 1){
		InitLog(("Windows 7\n"));
		WinVersion = eWin_7;
	}else if (OSVerInfo.dwMajorVersion == 6 && OSVerInfo.dwMinorVersion == 2){
		InitLog(("Windows 8"));
		if (OSVerInfo.dwBuildNumber == 8250){
		}else if (OSVerInfo.dwBuildNumber == 8400){
		}else if (OSVerInfo.dwBuildNumber == 9200){
			WinVersion = eWin_8;
		}
	}

	return WinVersion;
}

VOID
InitMapTable(
	)
{
	WIN_VERSION WinVersion		= (WIN_VERSION)g_WindowsVersion;
	PLIST_ENTRY pThreadHead		= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ProcessThreadHead]));
	PLIST_ENTRY pProcessHead	= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ProcessHead]));
	PLIST_ENTRY pHandleHead		= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ProcessHandleHead]));
	PLIST_ENTRY pObjectTypeHead	= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ObjectTypeHead]));
	PLIST_ENTRY pMemoryHead		= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ProcessMemoryHead]));
	PLIST_ENTRY pModuleHead		= (PLIST_ENTRY)(&(g_hg_map_list[eHG_ProcessModuleHead]));

	InitializeListHead(pThreadHead);
	InitializeListHead(pProcessHead);
	InitializeListHead(pHandleHead);
	InitializeListHead(pObjectTypeHead);
	InitializeListHead(pMemoryHead);
	InitializeListHead(pModuleHead);

	InitializeListHead(&g_tmp_process_list);
	KeInitializeSpinLock(&g_tmp_process_spin_lock);

	SetOffset(_KPRCB_IdleThread, 0x00C);
	g_hg_map[eHG_Offset_KAPC_STATE_Process] = 0x10;
	SetOffset(_OBJECT_HEADER_sizeof,					0x0018);
	g_hg_map[eHG_ObjectTypeOffset_OBJECT_HEADER] = 0x8;
	g_hg_map[eHG_IoDriverObjectType] = (ULONG_PTR)*IoDriverObjectType;
	g_hg_map[eHG_IoDeviceObjectType] = (ULONG_PTR)*IoDeviceObjectType;
	g_hg_map[eHG_IoFileObjectType] = (ULONG_PTR)*IoFileObjectType;
	g_hg_map[eHG_PsInitialSystemProcessAddress] = (ULONG_PTR)&PsInitialSystemProcess;
	g_hg_map[eHG_PsProcessType] = (ULONG_PTR)*PsProcessType;
	g_hg_map[eHG_PsThreadType] = (ULONG_PTR)*PsThreadType;

	switch (WinVersion){
	case eWin_2000:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0x128;
			SetOffset(_EPROCESS_Peb,							0x01B0);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x1c8;
			SetOffset(_KTHREAD_WaitListEntry,					0x005C);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x34;
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x9c;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x10;
			g_hg_map[eHG_DebugPortOffset]								= 0x120;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x01E0);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0234);
			SetOffset(_ETHREAD_StartAddress,					0x0230);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01A4);
			SetOffset(_KTHREAD_Teb,								0x0020);
			SetOffset(_KTHREAD_Priority,						0x0033);
			SetOffset(_KTHREAD_ContextSwitches,					0x004C);
			SetOffset(_KTHREAD_State,							0x002D);
			SetOffset(_KTHREAD_PreviousMode,					0x0134);
			SetOffset(_ETHREAD_ThreadsProcess,					0x022C);
			SetOffset(_EPROCESS_ThreadListHead,					0x0270);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0240);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x18;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x1c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x28;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0xd0;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x1ad;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x1ac;

			SetOffset(_EPROCESS_VadRoot,						0x0194);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0038);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x80;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x110;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0x9c;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0x97;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0x6a;
			g_hg_map[eHG_NtCloseIndex]									= 0x18;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0x61;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0x8e;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0xe7;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0x6b;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0x88;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0xe1;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0xdd;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0xb5;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0xc5;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x47;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0x77;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xa;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0xe0;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0x0;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x0;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 110;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 149;
			g_hg_map[eHG_NtOpenKeyIndex]								= 103;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 60;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 61;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 53;
			g_hg_map[eHG_NtCreateKeyIndex]								= 35;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 215;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 55;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 302;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 466;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 437;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 639;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 470;
		}
		break;
	case eWin_XP:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xc4;
			SetOffset(_EPROCESS_Peb,							0x01B0);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x14c;
			SetOffset(_KTHREAD_WaitListEntry,					0x0060);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x34;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x138;
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x84;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_SectionBaseAddressOffset_EPROCESS]				= 0x13C;
			g_hg_map[eHG_DebugPortOffset]								= 0xbc;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x01EC);
			SetOffset(_ETHREAD_StartAddress,					0x0224);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0228);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01B0);
			SetOffset(_KTHREAD_Teb,								0x0020);
			SetOffset(_KTHREAD_Priority,						0x0033);
			SetOffset(_KTHREAD_ContextSwitches,					0x004C);
			SetOffset(_KTHREAD_State,							0x002D);
			SetOffset(_KTHREAD_PreviousMode,					0x0140);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0220);
			SetOffset(_EPROCESS_ThreadListHead,					0x0190);
			SetOffset(_ETHREAD_ThreadListEntry,					0x022C);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x18;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x1c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x28;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0xd4;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x248;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x1b9;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x1b8;

			SetOffset(_EPROCESS_VadRoot,						0x011C);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0038);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x10;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x118;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0xb2;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0xad;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0x7a;
			g_hg_map[eHG_NtCloseIndex]									= 0x19;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0x71;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xa3;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 267;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 123;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 156;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x102;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0xfe;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0xce;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0xe3;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x53;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0x89;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xb;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x101;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0xcd;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0xfd;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0x7f;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0xaa;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0x77;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x47;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x49;
// 			g_hg_map[eHG_NtOpenFileIndex]								= 0x74;
// 			g_hg_map[eHG_NtCreateSectionIndex]							= 0x32;
// 			g_hg_map[eHG_NtMapViewOfSectionIndex]						= 0x6c;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 63;
			g_hg_map[eHG_NtCreateKeyIndex]								= 41;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 247;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 65;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 312;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 483;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 453;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 586;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 490;

			SetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,		0x000);
			SetOffset(_LDR_DATA_TABLE_ENTRY_DllBase,				0x018);
			SetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName,			0x024);
		}
		break;
	case eWin_2003:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xc4;
			SetOffset(_EPROCESS_Peb,							0x0190);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x128;
			SetOffset(_KTHREAD_WaitListEntry,					0x0060);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x34;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x114;
			SetOffset(_KPRCB_WaitListHead,						0x920);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x930);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x84;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_SectionBaseAddressOffset_EPROCESS]				= 0x118;
			g_hg_map[eHG_DebugPortOffset]								= 0xbc;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x01F4);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0230);
			SetOffset(_ETHREAD_StartAddress,					0x022C);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01AC);
			SetOffset(_KTHREAD_Teb,								0x0030);
			SetOffset(_KTHREAD_Priority,						0x005B);
			SetOffset(_KTHREAD_ContextSwitches,					0x0028);
			SetOffset(_KTHREAD_State,							0x002C);
			SetOffset(_KTHREAD_PreviousMode,					0x0115);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0228);
			SetOffset(_EPROCESS_ThreadListHead,					0x0170);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0234);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x18;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x1c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x20;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x70;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x250;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x1bb;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x1ba;

			SetOffset(_EPROCESS_VadRoot,						0x0258);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0038);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x100;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x100;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0xba;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0xb5;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0x80;
			g_hg_map[eHG_NtCloseIndex]									= 0x1b;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0x77;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xaa;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0x115;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0x81;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0xa3;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x10b;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0x107;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0xd6;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0xec;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x57;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0x8f;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xc;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x10a;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0xd5;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x106;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0x85;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0xb2;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0x7d;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x4b;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x4d;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 66;
			g_hg_map[eHG_NtCreateKeyIndex]								= 43;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 256;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 68;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 311;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 481;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 452;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 582;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 488;
		}
		break;
	case eWin_2003_SP1_SP2:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xd4;
			SetOffset(_EPROCESS_Peb,							0x01A0);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x138;
			SetOffset(_KTHREAD_WaitListEntry,					0x0060);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x28;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x124;
			SetOffset(_KPRCB_WaitListHead,						0x9F0);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x930);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x94;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_DebugPortOffset]								= 0xcc;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x01E4);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0220);
			SetOffset(_ETHREAD_StartAddress,					0x021C);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01A8);
			SetOffset(_KTHREAD_Teb,								0x0074);
			SetOffset(_KTHREAD_Priority,						0x005B);
			SetOffset(_KTHREAD_ContextSwitches,					0x0048);
			SetOffset(_KTHREAD_State,							0x004C);
			SetOffset(_KTHREAD_PreviousMode,					0x00D7);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0218);
			SetOffset(_EPROCESS_ThreadListHead,					0x0180);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0224);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x18;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x1c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x20;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x70;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x240;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x150;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x14f;

			SetOffset(_EPROCESS_VadRoot,						0x0250);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0038);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x200;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x108;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0xba;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0xb5;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0x80;
			g_hg_map[eHG_NtCloseIndex]									= 0x1b;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0x77;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xaa;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0x115;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0x81;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0xa3;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x10b;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0x107;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0xd6;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0xec;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x57;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0x8f;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xc;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x10a;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0xd5;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x106;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0x85;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0xb2;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0x7d;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x4b;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x4d;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 0x42;
			g_hg_map[eHG_NtCreateKeyIndex]								= 0x2b;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 0x100;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 0x44;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 311;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 481;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 452;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 582;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 488;
		}
		break;
	case eWin_Vista:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xdc;
			SetOffset(_EPROCESS_Peb,							0x0118);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x124;
			SetOffset(_KTHREAD_WaitListEntry,					0x0070);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x38;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x110;
			SetOffset(_KPRCB_WaitListHead,						0x1a20);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x1a60);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x9c;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_DebugPortOffset]								= 0xd4;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x020C);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0240);
			SetOffset(_ETHREAD_StartAddress,					0x01F8);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01C4);
			SetOffset(_KTHREAD_Teb,								0x0084);
			SetOffset(_KTHREAD_Priority,						0x004F);
			SetOffset(_KTHREAD_ContextSwitches,					0x0058);
			SetOffset(_KTHREAD_State,							0x005C);
			SetOffset(_KTHREAD_PreviousMode,					0x00E7);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0144);
			SetOffset(_EPROCESS_ThreadListHead,					0x0168);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0248);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x28;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x2c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x30;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x80;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x260;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x16c;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x16b;

			SetOffset(_EPROCESS_VadRoot,						0x0238);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0038);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x200;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x118;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0xfd;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0xf8;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0xc2;
			g_hg_map[eHG_NtCloseIndex]									= 0x30;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0xb7;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xed;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0x160;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0xc3;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0xe6;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x153;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0x14f;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0x119;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0x134;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x93;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0xd2;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xc;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x152;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0x118;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x14e;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0xc8;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0xf5;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0xbd;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x85;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x88;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 0x7b;
			g_hg_map[eHG_NtCreateKeyIndex]								= 0x40;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 0x148;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 0x7e;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 322;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 504;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 471;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 610;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 512;
		}
		break;
	case eWin_Vista_SP1_SP2:
		{
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xdc;
			SetOffset(_EPROCESS_Peb,							0x0188);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x124;
			SetOffset(_KTHREAD_WaitListEntry,					0x0070);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x38;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x110;
			SetOffset(_KPRCB_WaitListHead,						0x1aa0);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x1ae0);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x9c;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_DebugPortOffset]								= 0xd4;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x0050);
			SetOffset(_ETHREAD_Cid,								0x020C);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0240);
			SetOffset(_ETHREAD_StartAddress,					0x01F8);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01C4);
			SetOffset(_KTHREAD_Teb,								0x0084);
			SetOffset(_KTHREAD_Priority,						0x004F);
			SetOffset(_KTHREAD_ContextSwitches,					0x0058);
			SetOffset(_KTHREAD_State,							0x005C);
			SetOffset(_KTHREAD_PreviousMode,					0x00E7);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0144);
			SetOffset(_EPROCESS_ThreadListHead,					0x0168);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0248);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x28;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x2c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x30;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x80;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x260;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x16c;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x13c;

			SetOffset(_EPROCESS_VadRoot,						0x0238);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0000);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x200;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x118;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x32;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x30;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0xfd;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0xf8;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0xc2;
			g_hg_map[eHG_NtCloseIndex]									= 0x30;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0xb7;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xed;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0x15c;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0xc3;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0xe6;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x14f;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0x14b;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0x11a;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0x130;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x93;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0xd2;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xc;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x14e;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0x119;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x14a;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0xc8;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0xf5;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0xbd;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x85;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x88;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 0x7b;
			g_hg_map[eHG_NtCreateKeyIndex]								= 0x40;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 0x144;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 0x7e;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 322;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 504;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 471;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 610;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 512;
		}
		break;
	case eWin_7:
		{
#if defined(_WIN64)
			//==================== struct offset ==========================
			// _EPROCESS
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x290;
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0x200;
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x180;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x268;
			SetOffset(_EPROCESS_Peb,								0x0338);
			SetOffset(_EPROCESS_SeAuditProcessCreationInfo,			0x390);
			SetOffset(_EPROCESS_ThreadListHead,						0x0308);
			SetOffset(_EPROCESS_VadRoot,							0x0448);
			// _KPROCESS
			SetOffset(_KPROCESS_ThreadListHead,						0x0030);
			// _ETHREAD
			SetOffset(_ETHREAD_ThreadListEntry,						0x0420);
			SetOffset(_ETHREAD_Cid,									0x03B0);
			SetOffset(_ETHREAD_Win32StartAddress,					0x0410);
			SetOffset(_ETHREAD_StartAddress,						0x0388);
			// _KTHREAD
			SetOffset(_KTHREAD_ThreadListEntry,						0x02F8);
			SetOffset(_KTHREAD_Teb,									0x00B8);
			SetOffset(_KTHREAD_Priority,							0x007B);
			SetOffset(_KTHREAD_ContextSwitches,						0x0134);
			SetOffset(_KTHREAD_State,								0x0164);
			SetOffset(_KTHREAD_WaitListEntry,						0x00A0);
			g_hg_map[eHG_KTHREAD_Process]								= 0x210;
			SetOffset(_KTHREAD_PreviousMode,						0x01F6);
			// _OBJECT_TYPE
			SetOffset(_OBJECT_TYPE_TypeList,					0x0000);
			// _OBJECT_HEADER
			SetOffset(_OBJECT_HEADER_sizeof,					0x0028);
			// other
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x010;
			g_hg_map[eHG_PEB_ProcessParameters]							= 0x20;
			g_hg_map[eHG_RTL_USER_PROCESS_PARAMETERS_ImagePathName]		= 0x60;
			SetOffset(_KPRCB_IdleThread,							0x0018);
			SetOffset(_KPRCB_WaitListHead,							0x4480);
			SetOffset(_KPRCB_DispatcherReadyListHead,				0x4500);
			SetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,		0x0000);
			SetOffset(_LDR_DATA_TABLE_ENTRY_DllBase,				0x0030);
			SetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName,			0x0048);
			g_hg_map[eHG_NtQueryVirtualMemoryIndex]					= 32;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]				= 77;
#else
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0xf4;
			SetOffset(_EPROCESS_Peb,							0x01A8);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x140;
			SetOffset(_KTHREAD_WaitListEntry,					0x0074);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x40;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x128;
			SetOffset(_KPRCB_WaitListHead,						0x31E0);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x3220);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0xb4;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x8;
			g_hg_map[eHG_SectionBaseAddressOffset_EPROCESS]				= 0x12c;
			g_hg_map[eHG_DebugPortOffset]								= 0xec;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x002C);
			SetOffset(_ETHREAD_Cid,								0x022C);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0260);
			SetOffset(_ETHREAD_StartAddress,					0x0218);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01E0);
			SetOffset(_KTHREAD_Teb,								0x0088);
			SetOffset(_KTHREAD_Priority,						0x0057);
			SetOffset(_KTHREAD_ContextSwitches,					0x0064);
			SetOffset(_KTHREAD_State,							0x0068);
			SetOffset(_KTHREAD_PreviousMode,					0x013A);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0150);
			SetOffset(_EPROCESS_ThreadListHead,					0x0188);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0268);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x28;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x2c;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x30;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x84;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x280;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x188;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x140;

			SetOffset(_EPROCESS_VadRoot,						0x0278);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0000);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x100;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x120;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x6d;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x6e;

			g_hg_map[eHG_NtQueryVirtualMemoryIndex]						= 0x10b;
			g_hg_map[eHG_NtQuerySystemInformationIndex]					= 0x105;
			g_hg_map[eHG_NtOpenProcessIndex]							= 0xbe;
			g_hg_map[eHG_NtCloseIndex]									= 0x32;
			g_hg_map[eHG_NtOpenDirectoryObjectIndex]					= 0xaf;
			g_hg_map[eHG_NtQueryObjectIndex]							= 0xf8;
			g_hg_map[eHG_NtUnmapViewOfSectionIndex]						= 0x181;
			g_hg_map[eHG_NtOpenProcessTokenIndex]						= 0xbf;
			g_hg_map[eHG_NtQueryInformationTokenIndex]					= 0xed;
			g_hg_map[eHG_NtTerminateThreadIndex]						= 0x173;
			g_hg_map[eHG_NtSuspendThreadIndex]							= 0x16f;
			g_hg_map[eHG_NtResumeThreadIndex]							= 0x130;
			g_hg_map[eHG_NtSetInformationObjectIndex]					= 0x14c;
			g_hg_map[eHG_NtFreeVirtualMemoryIndex]						= 0x83;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]					= 0xd7;
			g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]					= 0xc;
			g_hg_map[eHG_NtTerminateProcessIndex]						= 0x172;
			g_hg_map[eHG_NtResumeProcessIndex]							= 0x12f;
			g_hg_map[eHG_NtSuspendProcessIndex]							= 0x16e;
			g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]					= 0xc5;
			g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]				= 0x102;
			g_hg_map[eHG_NtOpenKeyIndex]								= 0xb6;
			g_hg_map[eHG_NtEnumerateKeyIndex]							= 0x74;
			g_hg_map[eHG_NtEnumerateValueKeyIndex]						= 0x77;
			g_hg_map[eHG_NtDeleteKeyIndex]								= 103;
			g_hg_map[eHG_NtCreateKeyIndex]								= 70;
			g_hg_map[eHG_NtSetValueKeyIndex]							= 358;
			g_hg_map[eHG_NtDeleteValueKeyIndex]							= 106;

			g_hg_map[eHG_NtUserBuildHwndListIndex]						= 323;
			g_hg_map[eHG_NtUserQueryWindowIndex]						= 515;
			g_hg_map[eHG_NtUserKillTimerIndex]							= 482;
			g_hg_map[eHG_NtUserValidateTimerCallbackIndex]				= 623;
			g_hg_map[eHG_NtUserRegisterHotkeyIndex]						= 523;

			SetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,		0x000);
			SetOffset(_LDR_DATA_TABLE_ENTRY_DllBase,				0x018);
			SetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName,			0x024);
#endif
		}
		break;
	case eWin_8:
		{
#if defined(_WIN64)
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x3D0;
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0x408;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x028;
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0x2E0;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x3A8;
			SetOffset(_EPROCESS_Peb,							0x03E8);
			g_hg_map[eHG_PEB_ProcessParameters]							= 0x20;
			g_hg_map[eHG_RTL_USER_PROCESS_PARAMETERS_ImagePathName]		= 0x60;
			SetOffset(_KPRCB_IdleThread,							0x018);
			SetOffset(_EPROCESS_SeAuditProcessCreationInfo,			0x450);
			g_hg_map[eHG_KTHREAD_Process]						= 0x220;
			SetOffset(_KPRCB_WaitListHead,						0x5080);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x5100);
			SetOffset(_KTHREAD_WaitListEntry,					0x00D8);

			SetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,	0x000);
			SetOffset(_LDR_DATA_TABLE_ENTRY_DllBase,			0x030);
			SetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName,		0x048);
			SetOffset(_EPROCESS_VadRoot,						0x0590);

			SetOffset(_KPROCESS_ThreadListHead,					0x0030);
			SetOffset(_KTHREAD_ThreadListEntry,					0x02F8);
			SetOffset(_EPROCESS_ThreadListHead,					0x0470);
			SetOffset(_ETHREAD_ThreadListEntry,					0x0400);
			SetOffset(_ETHREAD_Cid,								0x0398);
			SetOffset(_ETHREAD_Win32StartAddress,				0x03F0);
			SetOffset(_ETHREAD_StartAddress,					0x0370);
			SetOffset(_KTHREAD_Teb,								0x00F0);
			SetOffset(_KTHREAD_Priority,						0x00C3);
			SetOffset(_KTHREAD_ContextSwitches,					0x0154);
			SetOffset(_KTHREAD_State,							0x0184);
			SetOffset(_KTHREAD_PreviousMode,					0x0232);
			// _OBJECT_TYPE
			SetOffset(_OBJECT_TYPE_TypeList,					0x0000);
			// _OBJECT_HEADER
			SetOffset(_OBJECT_HEADER_sizeof,					0x0028);
			g_hg_map[eHG_NtQueryVirtualMemoryIndex]					= 33;
			g_hg_map[eHG_NtProtectVirtualMemoryIndex]				= 78;
#else
			// process
			g_hg_map[eHG_ObjectTableOffset_EPROCESS]					= 0x150;
			SetOffset(_EPROCESS_Peb,							0x0140);
			g_hg_map[eHG_InheritedFromUniqueProcessIdOffset_EPROCESS]	= 0x134;
			SetOffset(_KTHREAD_WaitListEntry,					0x009C);
			g_hg_map[eHG_Offset_KTHREAD_ApcState]						= 0x70;
			g_hg_map[eHG_SectionObjectOffset_EPROCESS]					= 0x11c;
			SetOffset(_KPRCB_WaitListHead,						0x3ae0);
			SetOffset(_KPRCB_DispatcherReadyListHead,			0x3b20);
			g_hg_map[eHG_UniqueProcessIdOffset_EPROCESS]				= 0xb4;
			g_hg_map[eHG_UniqueProcessIdOffset_HANDLE_TABLE]			= 0x18;
			g_hg_map[eHG_DebugPortOffset]								= 0x154;
			// thread
			SetOffset(_KPROCESS_ThreadListHead,					0x002C);
			SetOffset(_ETHREAD_Cid,								0x0214);
			SetOffset(_ETHREAD_Win32StartAddress,				0x0244);
			SetOffset(_ETHREAD_StartAddress,					0x0200);
			SetOffset(_KTHREAD_ThreadListEntry,					0x01D4);
			SetOffset(_KTHREAD_Teb,								0x00A8);
			SetOffset(_KTHREAD_Priority,						0x0087);
			SetOffset(_KTHREAD_ContextSwitches,					0x008C);
			SetOffset(_KTHREAD_State,							0x0090);
			SetOffset(_KTHREAD_PreviousMode,					0x015A);
			SetOffset(_ETHREAD_ThreadsProcess,					0x0150);
			SetOffset(_EPROCESS_ThreadListHead,					0x0194);
			SetOffset(_ETHREAD_ThreadListEntry,					0x024C);
			g_hg_map[eHG_InitialStackOffset_ETHREAD]					= 0x20;
			g_hg_map[eHG_StackLimitOffset_KTHREAD]						= 0x24;
			g_hg_map[eHG_KernelStackOffset_KTHREAD]						= 0x48;
			g_hg_map[eHG_KernelApcDisableOffset_KTHREAD]				= 0x13c;
			g_hg_map[eHG_CrossThreadFlagsOffset_ETHREAD]				= 0x268;
			g_hg_map[eHG_SuspendCountOffset_KTHREAD]					= 0x18c;
			g_hg_map[eHG_FreezeCountOffset_KTHREAD]						= 0x5c;

			SetOffset(_EPROCESS_VadRoot,						0x0274);
			SetOffset(_OBJECT_TYPE_TypeList,					0x0000);
			g_hg_map[eHG_TIMER_TABLE_SIZE]								= 0x100;
			g_hg_map[eHG_QueueListEntryOffset_KTHREAD]					= 0x140;
			g_hg_map[eHG_IoplOffset_KPROCESS]							= 0x0;
			g_hg_map[eHG_IopmOffsetOffset_KPROCESS]						= 0x6e;

			SetOffset(_LDR_DATA_TABLE_ENTRY_InLoadOrderLinks,		0x000);
			SetOffset(_LDR_DATA_TABLE_ENTRY_DllBase,				0x018);
			SetOffset(_LDR_DATA_TABLE_ENTRY_FullDllName,			0x024);

			if (g_WindowsBuildNumber == 9200){			
				g_hg_map[eHG_NtQueryVirtualMemoryIndex]					= 0x8f;
				g_hg_map[eHG_NtQuerySystemInformationIndex]				= 0x95;
				g_hg_map[eHG_NtOpenProcessIndex]						= 0xdd;
				g_hg_map[eHG_NtCloseIndex]								= 0x174;
				g_hg_map[eHG_NtOpenDirectoryObjectIndex]				= 0xec;
				g_hg_map[eHG_NtQueryObjectIndex]						= 0xa2;
				g_hg_map[eHG_NtUnmapViewOfSectionIndex]					= 0x13;
				g_hg_map[eHG_NtOpenProcessTokenIndex]					= 0xdc;
				g_hg_map[eHG_NtQueryInformationTokenIndex]				= 0xad;
				g_hg_map[eHG_NtTerminateThreadIndex]					= 0x22;
				g_hg_map[eHG_NtSuspendThreadIndex]						= 0x26;
				g_hg_map[eHG_NtResumeThreadIndex]						= 0x68;
				g_hg_map[eHG_NtSetInformationObjectIndex]				= 0x4b;
				g_hg_map[eHG_NtFreeVirtualMemoryIndex]					= 0x118;
				g_hg_map[eHG_NtProtectVirtualMemoryIndex]				= 0xc3;
				g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]				= 0x19e;
				g_hg_map[eHG_NtTerminateProcessIndex]					= 0x23;
				g_hg_map[eHG_NtResumeProcessIndex]						= 0x69;
				g_hg_map[eHG_NtSuspendProcessIndex]						= 0x27;
				g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]				= 0xd6;
				g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]			= 0x98;
				g_hg_map[eHG_NtOpenKeyIndex]							= 0xe5;
				g_hg_map[eHG_NtEnumerateKeyIndex]						= 0x12a;
				g_hg_map[eHG_NtEnumerateValueKeyIndex]					= 0x127;
				g_hg_map[eHG_NtDeleteKeyIndex]							= 0x139;
				g_hg_map[eHG_NtCreateKeyIndex]							= 0x15e;
				g_hg_map[eHG_NtSetValueKeyIndex]						= 0x30;
				g_hg_map[eHG_NtDeleteValueKeyIndex]						= 0x136;

				g_hg_map[eHG_NtUserBuildHwndListIndex]					= 0x168;
				g_hg_map[eHG_NtUserQueryWindowIndex]					= 0x1e2;
				g_hg_map[eHG_NtUserKillTimerIndex]						= 0x205;
				g_hg_map[eHG_NtUserValidateTimerCallbackIndex]			= 0x28f;
				g_hg_map[eHG_NtUserRegisterHotkeyIndex]					= 0x273;
			}else{
				g_hg_map[eHG_NtQueryVirtualMemoryIndex]					= 0x90;
				g_hg_map[eHG_NtQuerySystemInformationIndex]				= 0x96;
				g_hg_map[eHG_NtOpenProcessIndex]						= 0xde;
				g_hg_map[eHG_NtCloseIndex]								= 0x175;
				g_hg_map[eHG_NtOpenDirectoryObjectIndex]				= 0xed;
				g_hg_map[eHG_NtQueryObjectIndex]						= 0xa3;
				g_hg_map[eHG_NtUnmapViewOfSectionIndex]					= 0x13;
				g_hg_map[eHG_NtOpenProcessTokenIndex]					= 0xdd;
				g_hg_map[eHG_NtQueryInformationTokenIndex]				= 0xae;
				g_hg_map[eHG_NtTerminateThreadIndex]					= 0x22;
				g_hg_map[eHG_NtSuspendThreadIndex]						= 0x26;
				g_hg_map[eHG_NtResumeThreadIndex]						= 0x69;
				g_hg_map[eHG_NtSetInformationObjectIndex]				= 0x4c;
				g_hg_map[eHG_NtFreeVirtualMemoryIndex]					= 0x119;
				g_hg_map[eHG_NtProtectVirtualMemoryIndex]				= 0xc4;
				g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]				= 0x19f;
				g_hg_map[eHG_NtTerminateProcessIndex]					= 0x23;
				g_hg_map[eHG_NtResumeProcessIndex]						= 0x6a;
				g_hg_map[eHG_NtSuspendProcessIndex]						= 0x27;
				g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]				= 0xd7;
				g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]			= 0x99;
				g_hg_map[eHG_NtOpenKeyIndex]							= 0xe6;
				g_hg_map[eHG_NtEnumerateKeyIndex]						= 0x12b;
				g_hg_map[eHG_NtEnumerateValueKeyIndex]					= 0x128;

				g_hg_map[eHG_NtUserBuildHwndListIndex]					= 0x168;
				g_hg_map[eHG_NtUserQueryWindowIndex]					= 0x1e1;
				g_hg_map[eHG_NtUserKillTimerIndex]						= 0x203;
				g_hg_map[eHG_NtUserValidateTimerCallbackIndex]			= 0x28c;
				g_hg_map[eHG_NtUserRegisterHotkeyIndex]					= 0x270;
			}
#endif
		}
		break;
	}
}

VOID
GetNtoskrnlLdrDataTableEntry(
	)
{
	PDRIVER_OBJECT pDrvObj = (PDRIVER_OBJECT)g_pHGDrvObj;
	ULONG_PTR nKernelBase = g_hg_map[eHG_CurrentKernelBase];
	PLDR_DATA_TABLE_ENTRY Entry = NULL;
	PLDR_DATA_TABLE_ENTRY FirstEntry = NULL;

	FirstEntry = Entry = (PLDR_DATA_TABLE_ENTRY)pDrvObj->DriverSection;

	if (pDrvObj && nKernelBase)
	{
		while((PLDR_DATA_TABLE_ENTRY)Entry->InLoadOrderLinks.Flink != FirstEntry)
		{
			if(nKernelBase == (SIZE_T)Entry->DllBase)
			{
				g_hg_map[eHG_Ntoskrnl_LDR_DATA_TABLE_ENTRY] = (SIZE_T)Entry;
				InitLog(("ntoskrnl LdrDataTableEntry = 0x%p\n", Entry));
				break;
			}
			Entry = (PLDR_DATA_TABLE_ENTRY)Entry->InLoadOrderLinks.Flink;
		}
	}
}

VOID InitWindowsApi()
{
	ULONG_PTR NewBase = g_hg_map[eHG_NewKernelBase];
	ULONG_PTR OldBase = g_hg_map[eHG_CurrentKernelBase];
	ULONG_PTR bReloadNtoskrnlSuccess = g_hg_map[eHG_ReloadNtosKrnlSuccess];
	SIZE_T Base = bReloadNtoskrnlSuccess ? NewBase : OldBase;

	PVOID pMmIsAddressValid = NULL;
	PVOID pMmGetVirtualForPhysical = NULL;
	PVOID pMmGetPhysicalAddress = NULL;
	PVOID pObQueryNameString = NULL;
	PVOID pObfDereferenceObject = NULL;
	PVOID pIoVolumeDeviceToDosName = NULL;
	PVOID pIoGetCurrentProcess = NULL;
	PVOID pObOpenObjectByPointer = NULL;

	PVOID pObReferenceObjectByPointer = NULL;
	PVOID pPsLookupProcessByProcessId = NULL;
	PVOID pIoThreadToProcess = NULL;
	PVOID pExAllocatePoolWithTag = NULL;
	PVOID pExFreePoolWithTag = NULL;

	PVOID pKeSetEvent = NULL;
	PVOID pKeQueryActiveProcessors = NULL;
	PVOID pKeInitializeEvent = NULL;
	PVOID pKeInitializeDpc = NULL;
	PVOID pKeSetTargetProcessorDpc = NULL;
	PVOID pKeSetImportanceDpc = NULL;
	PVOID pKeInsertQueueDpc = NULL;
	PVOID pKeWaitForSingleObject = NULL;

	PVOID pKeStackAttachProcess = NULL;
	PVOID pKeUnstackDetachProcess = NULL;
	PVOID pProbeForRead = NULL;
	PVOID pObReferenceObjectByHandle = NULL;
	PVOID pObGetObjectType = NULL;
	PVOID pKeGetCurrentThread = NULL;
	PVOID pPsGetProcessId = NULL;
	PVOID pPsGetThreadId = NULL;

	PVOID pIoQueryFileDosDeviceName = NULL;
	PVOID pMmIsNonPagedSystemAddressValid = NULL;
	PVOID pIoAllocateMdl = NULL;
	PVOID pMmBuildMdlForNonPagedPool = NULL;
	PVOID pMmMapLockedPagesSpecifyCache = NULL;
	PVOID pMmUnmapLockedPages = NULL;
	PVOID pIoFreeMdl = NULL;
	PVOID pProbeForWrite = NULL;

	PVOID pIoCreateFile = NULL;
	PVOID pObMakeTemporaryObject = NULL;
	PVOID pPsTerminateSystemThread = NULL;
	PVOID pPsCreateSystemThread = NULL;
	PVOID pIoDeleteDevice = NULL;
	PVOID pObOpenObjectByName = NULL;
	PVOID pPsSetLoadImageNotifyRoutine = NULL;
	PVOID pPsSetCreateProcessNotifyRoutine = NULL;

	PVOID pPsSetCreateThreadNotifyRoutine = NULL;
	PVOID pPsRemoveCreateThreadNotifyRoutine = NULL;
	PVOID pPsRemoveLoadImageNotifyRoutine = NULL;
	PVOID pIoRegisterShutdownNotification = NULL;
	PVOID pCmUnRegisterCallback = NULL;
	PVOID pIoUnregisterShutdownNotification = NULL;
	PVOID pKeUpdateSystemTime = NULL;
	PVOID pKeCancelTimer = NULL;

	PVOID pIoInitializeTimer = NULL;
	PVOID pIoStartTimer = NULL;
	PVOID pIoStopTimer = NULL;
	PVOID pExQueueWorkItem = NULL;
	PVOID pKe386IoSetAccessProcess = NULL;
	PVOID pIoGetDeviceObjectPointer = NULL;
	PVOID pObReferenceObjectByName = NULL;
	PVOID pRtlInitAnsiString = NULL;

	PVOID pRtlAnsiStringToUnicodeString = NULL;
	PVOID pRtlFreeUnicodeString = NULL;
	PVOID pPsGetCurrentProcessId = NULL;
	PVOID pIoFreeIrp = NULL;
	PVOID pMmFlushImageSection = NULL;
	PVOID pIoCreateDriver = NULL;
	PVOID pObCreateObjectType = NULL;
	PVOID pObGetObjectSecurity = NULL;

	PVOID pObCreateObjectTypeEx = NULL;
	PVOID pIofCompleteRequest = NULL;

	InitLog(("Base = 0x%p, Is reload kernel successful? %s\n", Base, bReloadNtoskrnlSuccess ? "true" : "false"));

	pMmIsAddressValid = HgMiFindExportedRoutineByName((PVOID)Base, "MmIsAddressValid");
	if (pMmIsAddressValid == NULL){
		InitLog(("### [Error] ###: Get pMmIsAddressValid address unsuccessfully!\n"));
	}else{
		InitLog(("### [Success] ### pMmIsAddressValid = 0x%p\n", pMmIsAddressValid));
	}
	g_hg_map[eHG_MmIsAddressValid] = (SIZE_T)pMmIsAddressValid;

	pMmGetVirtualForPhysical = HgMiFindExportedRoutineByName((PVOID)Base, "MmGetVirtualForPhysical");
	g_hg_map[eHG_MmGetVirtualForPhysical] = (SIZE_T)pMmGetVirtualForPhysical;

	pMmGetPhysicalAddress = HgMiFindExportedRoutineByName((PVOID)Base, "MmGetPhysicalAddress");
	g_hg_map[eHG_MmGetPhysicalAddress] = (SIZE_T)pMmGetPhysicalAddress;

	pObQueryNameString = HgMiFindExportedRoutineByName((PVOID)Base, "ObQueryNameString");
	g_hg_map[eHG_ObQueryNameString] = (SIZE_T)pObQueryNameString;

	pObfDereferenceObject = HgMiFindExportedRoutineByName((PVOID)Base, "ObfDereferenceObject");
	g_hg_map[eHG_ObfDereferenceObject] = (SIZE_T)pObfDereferenceObject;

	pIoVolumeDeviceToDosName = HgMiFindExportedRoutineByName((PVOID)Base, "IoVolumeDeviceToDosName");
	if (pIoVolumeDeviceToDosName == NULL){
		pIoVolumeDeviceToDosName = HgMiFindExportedRoutineByName((PVOID)Base, "RtlVolumeDeviceToDosName");
	}
	g_hg_map[eHG_IoVolumeDeviceToDosName] = (SIZE_T)pIoVolumeDeviceToDosName;

	pIoGetCurrentProcess = HgMiFindExportedRoutineByName((PVOID)Base, "IoGetCurrentProcess");
	g_hg_map[eHG_IoGetCurrentProcess] = (SIZE_T)pIoGetCurrentProcess;

	pObOpenObjectByPointer = HgMiFindExportedRoutineByName((PVOID)Base, "ObOpenObjectByPointer");
	g_hg_map[eHG_ObOpenObjectByPointer] = (SIZE_T)pObOpenObjectByPointer;

	pObReferenceObjectByPointer = HgMiFindExportedRoutineByName((PVOID)Base, "ObReferenceObjectByPointer");
	g_hg_map[eHG_ObReferenceObjectByPointer] = (SIZE_T)pObReferenceObjectByPointer;

	pPsLookupProcessByProcessId = HgMiFindExportedRoutineByName((PVOID)Base, "PsLookupProcessByProcessId");
	g_hg_map[eHG_PsLookupProcessByProcessId] = (SIZE_T)pPsLookupProcessByProcessId;

	pIoThreadToProcess = HgMiFindExportedRoutineByName((PVOID)Base, "IoThreadToProcess");
	g_hg_map[eHG_IoThreadToProcess] = (SIZE_T)pIoThreadToProcess;

	pExAllocatePoolWithTag = HgMiFindExportedRoutineByName((PVOID)Base, "ExAllocatePoolWithTag");
	g_hg_map[eHG_ExAllocatePoolWithTag] = (SIZE_T)pExAllocatePoolWithTag;

	pExFreePoolWithTag = HgMiFindExportedRoutineByName((PVOID)Base, "ExFreePoolWithTag");
	g_hg_map[eHG_ExFreePoolWithTag] = (SIZE_T)pExFreePoolWithTag;

	pKeSetEvent = HgMiFindExportedRoutineByName((PVOID)Base, "KeSetEvent");
	g_hg_map[eHG_KeSetEvent] = (SIZE_T)pKeSetEvent;

	pKeQueryActiveProcessors = HgMiFindExportedRoutineByName((PVOID)Base, "KeQueryActiveProcessors");
	g_hg_map[eHG_KeQueryActiveProcessors] = (SIZE_T)pKeQueryActiveProcessors;

	pKeInitializeEvent = HgMiFindExportedRoutineByName((PVOID)Base, "KeInitializeEvent");
	g_hg_map[eHG_KeInitializeEvent] = (SIZE_T)pKeInitializeEvent;

	pKeInitializeDpc = HgMiFindExportedRoutineByName((PVOID)Base, "KeInitializeDpc");
	g_hg_map[eHG_KeInitializeDpc] = (SIZE_T)pKeInitializeDpc;

	pKeSetTargetProcessorDpc = HgMiFindExportedRoutineByName((PVOID)Base, "KeSetTargetProcessorDpc");
	g_hg_map[eHG_KeSetTargetProcessorDpc] = (SIZE_T)pKeSetTargetProcessorDpc;

	pKeSetImportanceDpc = HgMiFindExportedRoutineByName((PVOID)Base, "KeSetImportanceDpc");
	g_hg_map[eHG_KeSetImportanceDpc] = (SIZE_T)pKeSetImportanceDpc;

	pKeInsertQueueDpc = HgMiFindExportedRoutineByName((PVOID)Base, "KeInsertQueueDpc");
	g_hg_map[eHG_KeInsertQueueDpc] = (SIZE_T)pKeInsertQueueDpc;

	pKeWaitForSingleObject = HgMiFindExportedRoutineByName((PVOID)Base, "KeWaitForSingleObject");
	g_hg_map[eHG_KeWaitForSingleObject] = (SIZE_T)pKeWaitForSingleObject;

	pKeStackAttachProcess = HgMiFindExportedRoutineByName((PVOID)Base, "KeStackAttachProcess");
	g_hg_map[eHG_KeStackAttachProcess] = (SIZE_T)pKeStackAttachProcess;

	pKeUnstackDetachProcess = HgMiFindExportedRoutineByName((PVOID)Base, "KeUnstackDetachProcess");
	g_hg_map[eHG_KeUnstackDetachProcess] = (SIZE_T)pKeUnstackDetachProcess;

	pProbeForRead = HgMiFindExportedRoutineByName((PVOID)Base, "ProbeForRead");
	g_hg_map[eHG_ProbeForRead] = (SIZE_T)pProbeForRead;

	pObReferenceObjectByHandle = HgMiFindExportedRoutineByName((PVOID)Base, "ObReferenceObjectByHandle");
	g_hg_map[eHG_ObReferenceObjectByHandle] = (SIZE_T)pObReferenceObjectByHandle;

	pObGetObjectType = HgMiFindExportedRoutineByName((PVOID)Base, "ObGetObjectType");
	if (pObGetObjectType == NULL){
		KdPrint(("### [Error] ###: Get pObGetObjectType address unsuccessfully!\n"));
	}else{
		KdPrint(("### [Success] ### pObGetObjectType = 0x%p\n", pObGetObjectType));
	}
	g_hg_map[eHG_ObGetObjectType] = (SIZE_T)pObGetObjectType;

	pKeGetCurrentThread = HgMiFindExportedRoutineByName((PVOID)Base, "KeGetCurrentThread");
	g_hg_map[eHG_KeGetCurrentThread] = (SIZE_T)pKeGetCurrentThread;

	pPsGetProcessId = HgMiFindExportedRoutineByName((PVOID)Base, "PsGetProcessId");
	g_hg_map[eHG_PsGetProcessId] = (SIZE_T)pPsGetProcessId;

	pPsGetThreadId = HgMiFindExportedRoutineByName((PVOID)Base, "PsGetThreadId");
	g_hg_map[eHG_PsGetThreadId] = (SIZE_T)pPsGetThreadId;

	pIoQueryFileDosDeviceName = HgMiFindExportedRoutineByName((PVOID)Base, "IoQueryFileDisDeviceName");
	g_hg_map[eHG_IoQueryFileDosDeviceName] = (SIZE_T)pIoQueryFileDosDeviceName;

	pMmIsNonPagedSystemAddressValid = HgMiFindExportedRoutineByName((PVOID)Base, "MmIsNonPagedSystemAddressValid");
	g_hg_map[eHG_MmIsNonPagedSystemAddressValid] = (SIZE_T)pMmIsNonPagedSystemAddressValid;

	pIoAllocateMdl = HgMiFindExportedRoutineByName((PVOID)Base, "IoAllocateMdl");
	g_hg_map[eHG_IoAllocateMdl] = (SIZE_T)pIoAllocateMdl;

	pMmBuildMdlForNonPagedPool = HgMiFindExportedRoutineByName((PVOID)Base, "MmBuildMdlForNonPagedPool");
	g_hg_map[eHG_MmBuildMdlForNonPagedPool] = (SIZE_T)pMmBuildMdlForNonPagedPool;

	pMmMapLockedPagesSpecifyCache = HgMiFindExportedRoutineByName((PVOID)Base, "MmMapLockedPagesSpecifyCache");
	g_hg_map[eHG_MmMapLockedPagesSpecifyCache] = (SIZE_T)pMmMapLockedPagesSpecifyCache;

	pMmUnmapLockedPages = HgMiFindExportedRoutineByName((PVOID)Base, "MmUnmapLockedPages");
	g_hg_map[eHG_MmUnmapLockedPages] = (SIZE_T)pMmUnmapLockedPages;

	pIoFreeMdl = HgMiFindExportedRoutineByName((PVOID)Base, "IoFreeMdl");
	g_hg_map[eHG_IoFreeMdl] = (SIZE_T)pIoFreeMdl;

	pProbeForWrite = HgMiFindExportedRoutineByName((PVOID)Base, "ProbeForWrite");
	g_hg_map[eHG_ProbeForWrite] = (SIZE_T)pProbeForWrite;

	pIoCreateFile = HgMiFindExportedRoutineByName((PVOID)Base, "IoCreateFile");
	g_hg_map[eHG_IoCreateFile] = (SIZE_T)pIoCreateFile;

	pObMakeTemporaryObject = HgMiFindExportedRoutineByName((PVOID)Base, "ObMakeTemporaryObject");
	g_hg_map[eHG_ObMakeTemporaryObject] = (SIZE_T)pObMakeTemporaryObject;

	pPsTerminateSystemThread = HgMiFindExportedRoutineByName((PVOID)Base, "PsTerminateSystemThread");
	g_hg_map[eHG_PsTerminateSystemThread] = (SIZE_T)pPsTerminateSystemThread;

	pPsCreateSystemThread = HgMiFindExportedRoutineByName((PVOID)Base, "PsCreateSystemThread");
	g_hg_map[eHG_PsCreateSystemThread] = (SIZE_T)pPsCreateSystemThread;

	pIoDeleteDevice = HgMiFindExportedRoutineByName((PVOID)Base, "IoDeleteDevice");
	g_hg_map[eHG_IoDeleteDevice] = (SIZE_T)pIoDeleteDevice;

	pObOpenObjectByName = HgMiFindExportedRoutineByName((PVOID)Base, "ObOpenObjectName");
	g_hg_map[eHG_ObOpenObjectByName] = (SIZE_T)pObOpenObjectByName;

	pPsSetCreateProcessNotifyRoutine = HgMiFindExportedRoutineByName((PVOID)Base, "PsSetCreateProcessNotifyRoutine");
	g_hg_map[eHG_PsSetCreateProcessNotifyRoutine] = (SIZE_T)pPsSetCreateProcessNotifyRoutine;

	pPsSetLoadImageNotifyRoutine = HgMiFindExportedRoutineByName((PVOID)Base, "PsSetLoadImageNotifyRoutine");
	g_hg_map[eHG_PsSetLoadImageNotifyRoutine] = (SIZE_T)pPsSetLoadImageNotifyRoutine;

	pPsSetCreateThreadNotifyRoutine = HgMiFindExportedRoutineByName((PVOID)Base, "PsSetCreateThreadNotifyRoutine");
	g_hg_map[eHG_PsSetCreateThreadNotifyRoutine] = (SIZE_T)pPsSetCreateThreadNotifyRoutine;

	pPsRemoveCreateThreadNotifyRoutine = HgMiFindExportedRoutineByName((PVOID)Base, "PsRemoveCreateThreadNotifyRoutine");
	g_hg_map[eHG_PsRemoveCreateThreadNotifyRoutine] = (SIZE_T)pPsRemoveCreateThreadNotifyRoutine;

	pPsRemoveLoadImageNotifyRoutine = HgMiFindExportedRoutineByName((PVOID)Base, "PsRemoveLoadImageNotifyRoutine");
	g_hg_map[eHG_PsRemoveLoadImageNotifyRoutine] = (SIZE_T)pPsRemoveLoadImageNotifyRoutine;

	pIoRegisterShutdownNotification = HgMiFindExportedRoutineByName((PVOID)Base, "IoRegisterShutdownNotification");
	g_hg_map[eHG_IoRegisterShutdownNotification] = (SIZE_T)pIoRegisterShutdownNotification;

	pCmUnRegisterCallback = HgMiFindExportedRoutineByName((PVOID)Base, "CmUnRegisterCallback");
	g_hg_map[eHG_CmUnRegisterCallback] = (SIZE_T)pCmUnRegisterCallback;

	pIoUnregisterShutdownNotification = HgMiFindExportedRoutineByName((PVOID)Base, "IoUnregisterShutdownNotification");
	g_hg_map[eHG_IoUnregisterShutdownNotification] = (SIZE_T)pIoUnregisterShutdownNotification;

	pKeUpdateSystemTime = HgMiFindExportedRoutineByName((PVOID)Base, "KeUpdateSystemTime");
	g_hg_map[eHG_KeUpdateSystemTime] = (SIZE_T)pKeUpdateSystemTime;

	pKeCancelTimer = HgMiFindExportedRoutineByName((PVOID)Base, "KeCancelTimer");
	g_hg_map[eHG_KeCancelTimer] = (SIZE_T)pKeCancelTimer;

	pIoInitializeTimer = HgMiFindExportedRoutineByName((PVOID)Base, "IoInitializeTimer");
	g_hg_map[eHG_IoInitializeTimer] = (SIZE_T)pIoInitializeTimer;

	pIoStartTimer = HgMiFindExportedRoutineByName((PVOID)Base, "IoStartTimer");
	g_hg_map[eHG_IoStartTimer] = (SIZE_T)pIoStartTimer;

	pIoStopTimer = HgMiFindExportedRoutineByName((PVOID)Base, "IoStopTimer");
	g_hg_map[eHG_IoStopTimer] = (SIZE_T)pIoStopTimer;

	pExQueueWorkItem = HgMiFindExportedRoutineByName((PVOID)Base, "ExQueueWorkItem");
	g_hg_map[eHG_ExQueueWorkItem] = (SIZE_T)pExQueueWorkItem;

	pKe386IoSetAccessProcess = HgMiFindExportedRoutineByName((PVOID)Base, "Ke386IoSetAccessProcess");
	g_hg_map[eHG_Ke386IoSetAccessProcess] = (SIZE_T)pKe386IoSetAccessProcess;

	pIoGetDeviceObjectPointer = HgMiFindExportedRoutineByName((PVOID)Base, "IoGetDeviceObjectPointer");
	g_hg_map[eHG_IoGetDeviceObjectPointer] = (SIZE_T)pIoGetDeviceObjectPointer;

	pObReferenceObjectByName = HgMiFindExportedRoutineByName((PVOID)Base, "ObReferenceObjectByName");
	g_hg_map[eHG_ObReferenceObjectByName] = (SIZE_T)pObReferenceObjectByName;

	pRtlInitAnsiString = HgMiFindExportedRoutineByName((PVOID)Base, "RtlInitAnsiString");
	g_hg_map[eHG_RtlInitAnsiString] = (SIZE_T)pRtlInitAnsiString;

	pRtlAnsiStringToUnicodeString = HgMiFindExportedRoutineByName((PVOID)Base, "RtlAnsiStringToUnicodeString");
	g_hg_map[eHG_RtlAnsiStringToUnicodeString] = (SIZE_T)pRtlAnsiStringToUnicodeString;

	pRtlFreeUnicodeString = HgMiFindExportedRoutineByName((PVOID)Base, "RtlFreeUnicodeString");
	g_hg_map[eHG_RtlFreeUnicodeString] = (SIZE_T)pRtlFreeUnicodeString;

	pPsGetCurrentProcessId = HgMiFindExportedRoutineByName((PVOID)Base, "PsGetCurrentProcessId");
	g_hg_map[eHG_PsGetCurrentProcess] = (SIZE_T)pPsGetCurrentProcessId;

	pIoFreeIrp = HgMiFindExportedRoutineByName((PVOID)Base, "IoFreeIrp");
	g_hg_map[eHG_IoFreeIrp] = (SIZE_T)pIoFreeIrp;

	pMmFlushImageSection = HgMiFindExportedRoutineByName((PVOID)Base, "MmFlushImageSection");
	g_hg_map[eHG_MmFlushImageSection] = (SIZE_T)pMmFlushImageSection;

	pIoCreateDriver = HgMiFindExportedRoutineByName((PVOID)Base, "IoCreateDriver");
	g_hg_map[eHG_IoCreateDriver] = (SIZE_T)pIoCreateDriver;

	pObGetObjectSecurity = HgMiFindExportedRoutineByName((PVOID)Base, "ObGetObjectSecurity");
	g_hg_map[eHG_ObGetObjectSecurity] = (SIZE_T)pObGetObjectSecurity;

	pObCreateObjectType = HgMiFindExportedRoutineByName((PVOID)Base, "ObCreateObjectType");
	g_hg_map[eHG_ObCreateObjectType] = (SIZE_T)pObCreateObjectType;

	pIofCompleteRequest = HgMiFindExportedRoutineByName((PVOID)Base, "IofCompleteRequest");
	g_hg_map[eHG_IofCompleteRequest] = (SIZE_T)pIofCompleteRequest;

	pObCreateObjectTypeEx = HgMiFindExportedRoutineByName((PVOID)Base, "ObCreateObjectTypeEx");
	g_hg_map[eHG_ObCreateObjectTypeEx] = (SIZE_T)pObCreateObjectTypeEx;
}

VOID
InitSSDT(
	)
{
	PKSERVICE_TABLE_DESCRIPTOR pOriginSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_OriginSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pReloadSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_ReloadSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pNowSSDT = (PKSERVICE_TABLE_DESCRIPTOR)g_hg_map[eHG_CurrentSSDT];
	PKSERVICE_TABLE_DESCRIPTOR pSSDT = NULL;

	if (pReloadSSDT && pReloadSSDT->ServiceTableBase){
		pSSDT = pReloadSSDT;
	}else if(pOriginSSDT && pOriginSSDT->ServiceTableBase){
		pSSDT = pOriginSSDT;
	}else if(pNowSSDT && pNowSSDT->ServiceTableBase){
		pSSDT = pNowSSDT;
	}

	if (pSSDT == NULL){
		return;
	}

	g_hg_map[eHG_NtQuerySystemInformation] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtQuerySystemInformationIndex]];
	g_hg_map[eHG_NtOpenProcess] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenProcessIndex]];
	g_hg_map[eHG_NtQueryVirtualMemory] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtQueryVirtualMemoryIndex]];
	g_hg_map[eHG_NtClose] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtCloseIndex]];
	g_hg_map[eHG_NtOpenDirectoryObject] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenDirectoryObjectIndex]];
	g_hg_map[eHG_NtQueryObject] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtQueryObjectIndex]];
	g_hg_map[eHG_NtOpenProcessToken] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenProcessTokenIndex]];
	g_hg_map[eHG_NtQueryInformationToken] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtQueryInformationTokenIndex]];
	g_hg_map[eHG_NtUnmapViewOfSection] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtUnmapViewOfSectionIndex]];
	g_hg_map[eHG_NtTerminateThread] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtTerminateThreadIndex]];
	g_hg_map[eHG_NtResumeThread] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtResumeThreadIndex]];
	g_hg_map[eHG_NtSuspendThread] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtSuspendThreadIndex]];
	g_hg_map[eHG_NtSetInformationObject] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtSetInformationObjectIndex]];
	g_hg_map[eHG_NtFreeVirtualMemory] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtFreeVirtualMemoryIndex]];
	g_hg_map[eHG_NtProtectVirtualMemory] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtProtectVirtualMemoryIndex]];
	g_hg_map[eHG_NtAdjustPrivilegesToken] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtAdjustPrivilegesTokenIndex]];
	g_hg_map[eHG_NtTerminateProcess] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtTerminateProcessIndex]];
	g_hg_map[eHG_NtResumeProcess] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtResumeProcessIndex]];
	g_hg_map[eHG_NtSuspendProcess] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtSuspendProcessIndex]];
	g_hg_map[eHG_NtOpenSymbolicLinkObject] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenSymbolicLinkObjectIndex]];
	g_hg_map[eHG_NtQuerySymbolicLinkObject] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtQuerySymbolicLinkObjectIndex]];
	g_hg_map[eHG_NtOpenKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenKeyIndex]];
	g_hg_map[eHG_NtEnumerateKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtEnumerateKeyIndex]];
	g_hg_map[eHG_NtEnumerateValueKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtEnumerateValueKeyIndex]];
	g_hg_map[eHG_NtOpenFile] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtOpenFileIndex]];
	g_hg_map[eHG_NtCreateSection] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtCreateSectionIndex]];
	g_hg_map[eHG_NtMapViewOfSection] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtMapViewOfSectionIndex]];
	g_hg_map[eHG_NtDeleteKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtDeleteKeyIndex]];
	g_hg_map[eHG_NtCreateKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtCreateKeyIndex]];
	g_hg_map[eHG_NtSetValueKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtSetValueKeyIndex]];
	g_hg_map[eHG_NtDeleteValueKey] = pSSDT->ServiceTableBase[g_hg_map[eHG_NtDeleteValueKeyIndex]];
}

VOID
GetCommonDriverSizeAndBase(
	)
{
	PLDR_DATA_TABLE_ENTRY entry = NULL;
	PLDR_DATA_TABLE_ENTRY firstEntry = NULL;
	WCHAR pwszNtfs[] = L"ntfs.sys";
	WCHAR pwszFastfat[] = L"Fastfat.sys";
	WCHAR pwszKbdClass[] = L"Kbdclass.sys";
	WCHAR pwszMouClass[] = L"Mouclass.sys";
	WCHAR pwszClasspnp[] = L"Classpnp.sys";
	WCHAR pwszAtapi[] = L"Atapi.sys";
	WCHAR pwszAcpi[] = L"Acpi.sys";
	WCHAR pwszTcpip[] = L"Tcpip.sys";
	WCHAR pwszAtaPort[] = L"AtaPort.sys";

	firstEntry = entry = (PLDR_DATA_TABLE_ENTRY)g_hg_map[eHG_Ntoskrnl_LDR_DATA_TABLE_ENTRY];
	if (entry){
		__try{
			while ((PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink != firstEntry){
				if (entry->BaseDllName.Length && entry->BaseDllName.Buffer){
					ULONG nLen = entry->BaseDllName.Length;
					ULONG nCharCnt = nLen / sizeof(WCHAR);

					//NTFS
					if (wcslen(pwszNtfs) == nCharCnt		&&
						!_wcsnicmp(pwszNtfs, entry->BaseDllName.Buffer, nCharCnt))
					{
						g_hg_map[eHG_NtfsBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_NtfsSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszAcpi) == nCharCnt	&&
						!_wcsnicmp(pwszAcpi, entry->BaseDllName.Buffer, nCharCnt))
					{	// acpi
						g_hg_map[eHG_AcpiBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_AcpiSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszAtapi) == nCharCnt	&&
						!_wcsnicmp(pwszAtapi, entry->BaseDllName.Buffer, nCharCnt))
					{	// atapi
						g_hg_map[eHG_AtapiBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_AtapiSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszFastfat) == nCharCnt	&&
						!_wcsnicmp(pwszFastfat, entry->BaseDllName.Buffer, nCharCnt))
					{	// fastfat
						g_hg_map[eHG_FastfatBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_FastfatSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszKbdClass) == nCharCnt	&&
						!_wcsnicmp(pwszKbdClass, entry->BaseDllName.Buffer, nCharCnt))
					{	// KbdClass
						g_hg_map[eHG_KbdclassBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_KbdclassSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszMouClass) == nCharCnt	&&
						!_wcsnicmp(pwszMouClass, entry->BaseDllName.Buffer, nCharCnt))
					{	// Mouclass
						g_hg_map[eHG_MouclassBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_MouclassSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszClasspnp) == nCharCnt	&&
						!_wcsnicmp(pwszClasspnp, entry->BaseDllName.Buffer, nCharCnt))
					{	// Classpnp
						g_hg_map[eHG_ClasspnpBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_ClasspnpSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszTcpip) == nCharCnt	&&
						!_wcsnicmp(pwszTcpip, entry->BaseDllName.Buffer, nCharCnt))
					{	// tcpip
						g_hg_map[eHG_TcpipBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_TcpipSize] = (ULONG)entry->SizeOfImage;
					}else if (wcslen(pwszAtaPort) == nCharCnt	&&
						!_wcsnicmp(pwszAtaPort, entry->BaseDllName.Buffer, nCharCnt))
					{	// Ataport
						g_hg_map[eHG_AtaPortBase] = (ULONG)entry->DllBase;
						g_hg_map[eHG_AtaPortSize] = (ULONG)entry->SizeOfImage;
					}
				}
				entry = (PLDR_DATA_TABLE_ENTRY)entry->InLoadOrderLinks.Flink;
			}
		}__except(EXCEPTION_EXECUTE_HANDLER){

		}
	}
}