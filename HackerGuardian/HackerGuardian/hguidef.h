/* 
 * Description:
 * define struct
 * 
 * Author:
 *	Stanford
 *	
 * Date:
 *	2013/03/05
 *	
 * Version:
 *
 * Revision:
 *	
 */

#ifndef __HGUIDEF_H__
#define __HGUIDEF_H__

#include <vector>
#include "..\references\szdef.h"
/*
*
Operating system		Version number	dwMajorVersion	dwMinorVersion	Other
Windows 8				6.2				6				2				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2012		6.2				6				2				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows 7				6.1				6				1				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2008 R2	6.1				6				1				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows Server 2008		6.0				6				0				OSVERSIONINFOEX.wProductType != VER_NT_WORKSTATION
Windows Vista			6.0				6				0				OSVERSIONINFOEX.wProductType == VER_NT_WORKSTATION
Windows Server 2003 R2	5.2				5				2				GetSystemMetrics(SM_SERVERR2) != 0
Windows Server 2003		5.2				5				2				GetSystemMetrics(SM_SERVERR2) == 0
Windows XP				5.1				5				1				Not applicable
Windows 2000			5.0				5				0				Not applicable

 */
typedef enum _WINDOWS_VERSION{
	eWin_XP = 0,
	eWin_7,
	eWin_8,
	eWin_Other
}WINDOWS_VERSION;

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

#define STATUS_SUCCESS                  ((NTSTATUS)0x00000000L)   
#define STATUS_UNSUCCESSFUL             ((NTSTATUS)0xC0000001L)   
#define STATUS_NOT_IMPLEMENTED          ((NTSTATUS)0xC0000002L)   
#define STATUS_INVALID_INFO_CLASS       ((NTSTATUS)0xC0000003L)   
#define STATUS_INFO_LENGTH_MISMATCH     ((NTSTATUS)0xC0000004L)   

//
// 
// 
#define	WM_MEM_CHANGE_PROTECTION	WM_USER + 2
//
// 
// 
typedef struct _RTL_PROCESS_MODULE_INFORMATION {
	HANDLE Section;                 // Not filled in
	PVOID MappedBase;
	PVOID ImageBase;
	ULONG ImageSize;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT OffsetToFileName;
	UCHAR  FullPathName[ 256 ];
} RTL_PROCESS_MODULE_INFORMATION, *PRTL_PROCESS_MODULE_INFORMATION;

typedef struct _RTL_PROCESS_MODULES {
	ULONG NumberOfModules;
	RTL_PROCESS_MODULE_INFORMATION Modules[ 1 ];
} RTL_PROCESS_MODULES, *PRTL_PROCESS_MODULES;

/************************************************************************/
/* Tab ID                                                               */
/************************************************************************/
typedef enum _TABID{
	eProcess = 0,
	eProcessModule,
	eProcessThread,
	eProcessMemory,
	eProcessHandle,
	eDriver,
	eKernelNotify,
	eHookSSDT,
	eHookShadowSSDT,
}TABID;

/************************************************************************/
/* Convert DWORD to CString                                             */
/************************************************************************/
__forceinline CString DWORD2CString(DWORD n){
	CString strBuffer;
	strBuffer.Format(TEXT("%d"), n);
	return strBuffer;
}

/************************************************************************/
/* Convert ULONG to CString                                             */
/************************************************************************/
__forceinline CString ULONG2CString(ULONG n){
	CString strBuffer;
	strBuffer.Format(TEXT("%d"), n);
	return strBuffer;
}

/************************************************************************/
/* Convert ULONG to Hex CString                                         */
/************************************************************************/
__forceinline CString ULONG2HexCString(ULONG n){
	CString strBuffer;
	strBuffer.Format(TEXT("0x%08X\n"), n);
	return strBuffer;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
__forceinline CString size_tToCString(size_t n){
	CString strBuffer;
	strBuffer.Format(TEXT("%d"), n);
	return strBuffer;
}

/************************************************************************/
/* Convert size_t to Hex CString                                         */
/************************************************************************/
__forceinline CString size_tToHexCString(size_t n){
	CString strBuffer;
	strBuffer.Format(TEXT("0x%p"), n);
	return strBuffer;
}

/************************************************************************/
/*                                                                      */
/************************************************************************/
FORCEINLINE
BOOL EnableDebugPrivilege(BOOL bEnable) 
{
	BOOL bOk = FALSE; 
	HANDLE hToken;

	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
		TOKEN_PRIVILEGES tp;

		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}

	return bOk;
}

FORCEINLINE
BOOL EnableLoadDriverPrivilege(BOOL bEnable) 
{
	BOOL bOk = FALSE; 
	HANDLE hToken;

	if (OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken)) 
	{
		TOKEN_PRIVILEGES tp;

		tp.PrivilegeCount = 1;
		LookupPrivilegeValue(NULL, SE_LOAD_DRIVER_NAME, &tp.Privileges[0].Luid);
		tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(tp), NULL, NULL);
		bOk = (GetLastError() == ERROR_SUCCESS);
		::CloseHandle(hToken);
	}

	return bOk;
}

FORCEINLINE
BOOL IsAdministrator() //判断是否管理员模式 
{
		BOOL bIsElevated = FALSE; 
		HANDLE hToken = NULL; 
		UINT16 uWinVer = LOWORD(GetVersion()); 
		uWinVer = MAKEWORD(HIBYTE(uWinVer),LOBYTE(uWinVer)); 

		if (uWinVer < 0x0600)//不是VISTA、Windows7 
			return(FALSE); 

		if (OpenProcessToken(GetCurrentProcess(),TOKEN_QUERY,&hToken)) { 

			struct { 
				DWORD TokenIsElevated; 
			} /*TOKEN_ELEVATION*/te; 
			DWORD dwReturnLength = 0; 

			if (GetTokenInformation(hToken,/*TokenElevation*/(_TOKEN_INFORMATION_CLASS)20,&te,sizeof(te),&dwReturnLength)) { 
				if (dwReturnLength == sizeof(te)) 
					bIsElevated = te.TokenIsElevated; 
			} 
			CloseHandle( hToken ); 
		} 
		return bIsElevated; 
}

__forceinline WINDOWS_VERSION GetWinVer()
{
	OSVERSIONINFOEX svex;
	svex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&svex);
	if ((svex.dwMajorVersion == 5)	&&
		(svex.dwMinorVersion == 1))
	{
		return eWin_XP;
	}else if ((svex.dwMajorVersion == 6)	&&
		(svex.dwMinorVersion == 1)			&&
		(svex.wProductType == VER_NT_WORKSTATION))
	{
		return eWin_7;
	}else if ((svex.dwMajorVersion == 6)	&&
		(svex.dwMinorVersion == 2)			&&
		(svex.wProductType == VER_NT_WORKSTATION))
	{
		return eWin_8;
	}
	return eWin_Other;
}

CString DosDevicePathToLogicalPath(LPCTSTR lpszDosPath,int nMaxCchBuff);
CString FormatDosPath(LPCTSTR lpszDosPath);
CString GetFileCompany(CString strPath);

HRESULT CreateProcessDump(DWORD dwProcessId, PWCHAR pwszProcessName,BOOL bMiniDump = TRUE);

CString GetLongPath(CString szPath);

typedef
NTSTATUS
(WINAPI *pfnZwQuerySystemInformation)(
	_In_       SYSTEM_INFORMATION_CLASS SystemInformationClass,
	_Inout_    PVOID SystemInformation,
	_In_       ULONG SystemInformationLength,
	_Out_opt_  PULONG ReturnLength
	);

std::vector<std::wstring> ParseStringSequenceW(PWCHAR s, SIZE_T nTotalLen);

unsigned long StrHexToULongA(char* StrHexA, int* errorno);

CString GetCurrentDatetimeString();

#endif