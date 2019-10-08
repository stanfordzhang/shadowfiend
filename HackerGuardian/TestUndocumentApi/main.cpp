#include <stdio.h>
#include <tchar.h>
#include <windows.h>  
#include "winnt.h"
#include <iostream>
#include <locale>
#include <WinBase.h>
#pragma comment(lib, "Advapi32.lib")

//-----------------------------定义--------------------------------

typedef ULONG KPRIORITY;
typedef LONG NTSTATUS;

typedef enum _THREAD_INFORMATION_CLASS {
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

typedef struct _CLIENT_ID {  
	DWORD   UniqueProcess;  
	DWORD   UniqueThread;  
} CLIENT_ID; 
typedef   CLIENT_ID   *PCLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION {
	NTSTATUS                ExitStatus;
	PVOID                   TebBaseAddress;
	CLIENT_ID               ClientId;
	KAFFINITY               AffinityMask;
	KPRIORITY               Priority;
	KPRIORITY               BasePriority;

} THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;



typedef struct _THREAD_TIMES_INFORMATION {
	LARGE_INTEGER           CreationTime;
	LARGE_INTEGER           ExitTime;
	LARGE_INTEGER           KernelTime;
	LARGE_INTEGER           UserTime;
} THREAD_TIMES_INFORMATION, *PTHREAD_TIMES_INFORMATION;

typedef LONG (__stdcall *PFN_NtQueryInformationThread)(  
	IN  HANDLE               ThreadHandle,
	IN  THREAD_INFORMATION_CLASS ThreadInformationClass,
	OUT PVOID               ThreadInformation,
	IN  ULONG                ThreadInformationLength,
	OUT PULONG              ReturnLength OPTIONAL 
	);


EXTERN_C PFN_NtQueryInformationThread pfn_NtQueryInformationThread = NULL;



//-----------------------函数--------------------------------------

void EmptySystemTime(SYSTEMTIME& sysTime)
{
	sysTime.wDay = 0;
	sysTime.wDayOfWeek = 0;
	sysTime.wHour = 0;
	sysTime.wMilliseconds = 0;
	sysTime.wMinute = 0;
	sysTime.wMonth = 0;
	sysTime.wSecond = 0;
	sysTime.wYear = 0;

}

BOOL LargeIntegerToLocalSystemTime(const LARGE_INTEGER& largeInteger, SYSTEMTIME& sysTime)
{
	EmptySystemTime(sysTime);

	if (largeInteger.QuadPart == 0)
	{
		return TRUE;
	}
	FILETIME* fileTime = (FILETIME*)&largeInteger;

	// first convert file time (UTC time) to local time
	FILETIME localTime;
	if (!FileTimeToLocalFileTime(fileTime, &localTime))
	{
		return FALSE;
	}

	// then convert that time to system time
	if (!FileTimeToSystemTime(&localTime, &sysTime))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL LargeIntegerToTimeSpan(const LARGE_INTEGER& largeInteger, SYSTEMTIME& sysTime)
{
	LargeIntegerToLocalSystemTime(largeInteger, sysTime);
	if (largeInteger.QuadPart == 0)
	{
		return TRUE;
	}

	sysTime.wYear -= 1601;
	sysTime.wDayOfWeek = 0;
	sysTime.wMonth -= 1;
	sysTime.wDay -= 1;
	sysTime.wHour -= 8;
	return TRUE;
}

BOOL QueryThreadTimes(HANDLE hThread)
{
	THREAD_TIMES_INFORMATION  threadTimesInfo;
	LONG status = pfn_NtQueryInformationThread(hThread, ThreadTimes, &threadTimesInfo,sizeof(threadTimesInfo), NULL);

	SYSTEMTIME sysTime;
	LargeIntegerToLocalSystemTime(threadTimesInfo.CreationTime, sysTime);
	_tprintf(_T("CreationTime is %d-%d-%d %d:%d:%d.%d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour,sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	LargeIntegerToLocalSystemTime(threadTimesInfo.ExitTime, sysTime);
	_tprintf(_T("ExitTime     is %d-%d-%d %d:%d:%d.%d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour,sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	LargeIntegerToTimeSpan(threadTimesInfo.KernelTime, sysTime);
	_tprintf(_T("KernelTime   is %d-%d-%d %d:%d:%d.%d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour,sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	LargeIntegerToTimeSpan(threadTimesInfo.UserTime, sysTime);
	_tprintf(_T("UserTime     is %d-%d-%d %d:%d:%d.%d\n"), sysTime.wYear, sysTime.wMonth, sysTime.wDay,
		sysTime.wHour,sysTime.wMinute, sysTime.wSecond, sysTime.wMilliseconds);
	return TRUE;
}

BOOL QueryThreadBasicInformation(HANDLE hThread)
{
	THREAD_BASIC_INFORMATION threadBasicInfo;
	LONG status = pfn_NtQueryInformationThread(hThread, ThreadBasicInformation, &threadBasicInfo,sizeof(threadBasicInfo), NULL);

	_tprintf   (TEXT("process ID is %u\n"), threadBasicInfo.ClientId.UniqueProcess);  
	_tprintf   (TEXT("Thread ID is %u\n"),  threadBasicInfo.ClientId.UniqueThread);  
	_tprintf   (TEXT("TEB is 0x%p\n"),threadBasicInfo.TebBaseAddress);

	return TRUE;
}

SIZE_T QueryThreadStartAddress(HANDLE hThread)
{
	SIZE_T StartAddress = 0;
	LONG status = pfn_NtQueryInformationThread(hThread, ThreadQuerySetWin32StartAddress, &StartAddress,sizeof(StartAddress), NULL);

	_tprintf(TEXT("StartAddress = 0x%p\n"), StartAddress);

	return StartAddress;
}

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

//-------------------------主函数----------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	// 模拟命令提示符
// 	FILE* pFile = _popen("route", "r");
// 	int ch;
// 	
// 	int times = 0;
// 	while ((ch = fgetc(pFile)) != EOF){
// 		if (times == 128){
// 			break;
// 		}
// 		putchar(ch);
// 		times ++;
// 	}
// 	_pclose(pFile);
// 	system("pause");
// 	return 0;
	//////////////////////////////////////////////////////////////////////////
#if defined(_WIN64)
	printf("_WIN64\n");
	printf("%d\n", sizeof(LONG) * 8);
	printf("%p\n", -1);
	printf("%p\n", ((SSIZE_T)(-1)));
	printf("%p\n", ((ULONG_PTR)((SSIZE_T)0xFFFFFFFF80000000)));
#else
	printf("_WIN32\n");
	printf("%d\n", sizeof(LONG) * 8);
	printf("%p\n", -1);
	printf("%p\n", ((SSIZE_T)(-1)));
	printf("%p\n", ((ULONG_PTR)((SSIZE_T)0x80000000)));
#endif
	EnableDebugPrivilege(TRUE);
	HINSTANCE hNTDLL = GetModuleHandle(_T("ntdll"));  
	pfn_NtQueryInformationThread = (PFN_NtQueryInformationThread)GetProcAddress(hNTDLL, "NtQueryInformationThread");

	setlocale(LC_ALL, "CHS");

	DWORD dwThreadId = 0 ;
	while ( _tprintf ( _T("请输入目标线程ID：") ) && std::cin >> dwThreadId && dwThreadId > 0 ) 
	{
		HANDLE hThread = OpenThread(THREAD_ALL_ACCESS,FALSE, dwThreadId);
		if (NULL == hThread)
		{
			_tprintf(_T("OpenThread : %u Error! ErrorCode:%u\n"), dwThreadId, GetLastError());
			continue;
		}
		QueryThreadTimes(hThread);
		QueryThreadBasicInformation(hThread);
		QueryThreadStartAddress(hThread);

		CloseHandle ( hThread ) ;
	}


	return 0;
}