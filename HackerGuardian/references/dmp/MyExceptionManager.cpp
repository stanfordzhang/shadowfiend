#include "stdafx.h"
#include "MyExceptionManager.h"
#include <Dbghelp.h>
#include <crtdbg.h>
#include <new.h>
#include <string>
#include <iostream>
#include <exception>
#include <Windows.h>

using namespace std;

#pragma comment(lib, "Dbghelp")
#pragma comment(lib, "Version.lib")

CMyExceptionManager *gpExcepMg = NULL;

LONG WINAPI MyExcepFunc(__in struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	
	if(gpExcepMg != NULL)
	{
		gpExcepMg->DumpExcepFile(ExceptionInfo);
	}
	return EXCEPTION_EXECUTE_HANDLER;
}

CMyExceptionManager::CMyExceptionManager(void)
: m_dwDMPType(MiniDumpNormal)
, m_hWndMain(NULL)
{
	gpExcepMg = this;
}

CMyExceptionManager::~CMyExceptionManager(void)
{

}

DWORD CMyExceptionManager::GetCurrentPath(IN HMODULE hModule, OUT TCHAR *szPath, IN DWORD dwSize)
{
	if(NULL == szPath || 0 == dwSize)
		return 0;
	TCHAR szTemp[MAX_PATH] = {0};
	DWORD dwGet = GetModuleFileName(hModule, szTemp, MAX_PATH);
	if(0 == dwGet)
		return 0;
	TCHAR szDriver[10] = {0};
	TCHAR szDirectory[MAX_PATH] = {0};

	_tsplitpath_s(szTemp, szDriver, 10, szDirectory, MAX_PATH, NULL, 0, NULL, 0);
	ZeroMemory(szTemp, MAX_PATH * sizeof(TCHAR));

	_tmakepath_s(szTemp, MAX_PATH, szDriver, szDirectory, NULL, NULL);

	DWORD dwLen = (DWORD)_tcslen(szTemp);
	if(dwLen < dwSize)
	{
		_tcscpy_s(szPath, dwSize, szTemp);
		return dwLen;
	}else
	{
		_tcsncpy_s(szPath, dwSize, szTemp, dwSize - 1);
		szPath[dwSize - 1] = '\0';
		return (dwSize - 1);
	}
}


void set_unexpected_function() 
{
	SzLog(_T("set_unexpected"));
	 throw "unexpected function";
	//terminate();
}

void trans_func( unsigned int u, EXCEPTION_POINTERS* pExp )
{
	SzLog(_T("trans_func"));
	//throw SE_Exception();
}

void termfunction( ) 
{
	SzLog(_T("termfunction"));
	abort( );
}

int handle_program_memory_depletion( size_t )
{
	SzLog(_T("handle_program_memory_depletion"));
	return 0;
}

void myPurecallHandler(void)
{
	SzLog(_T("myPurecallHandler"));
	exit(0);
}

void __cdecl newhandler( )
{
	SzLog(_T("newhandler"));
	throw bad_alloc( );
	return;
}

void myInvalidParameterHandler(const wchar_t* expression,
	const wchar_t* function, 
	const wchar_t* file, 
	unsigned int line, 
	uintptr_t pReserved)
{
	SzLog(_T("Invalid parameter detected in function %s. File: %s Line: %d\n"), function, file, line);
	SzLog(_T("Expression: %s\n"), expression);
	//抛出异常方便捕捉然后生成dmp
	 throw "Invalid Parameter";
	//abort();
}

void DisableSetUnhandledExceptionFilter()
{
	void *addr = (void*)GetProcAddress(LoadLibrary(_T("kernel32.dll")),	"SetUnhandledExceptionFilter");
	if (addr)
	{

		unsigned char code[16];
		int size = 0;
		code[size++] = 0x33;
		code[size++] = 0xC0;
		code[size++] = 0xC2;
		code[size++] = 0x04;
		code[size++] = 0x00;
		//改成
		//77C2F4FB 33 C0                xor         eax,eax  
		//77C2F4FD C2 04 00             ret         4  
		DWORD dwOldFlag = 0, dwTempFlag = 0;
		VirtualProtect(addr, size, PAGE_EXECUTE_READWRITE, &dwOldFlag);
		WriteProcessMemory(GetCurrentProcess(), addr, code, size, NULL);
		VirtualProtect(addr, size, dwOldFlag, &dwTempFlag);
	}
}

HRESULT CMyExceptionManager::SetUnHandleException(DWORD dwDMPType /* = 0 */)
{
#ifdef DEBUG
	m_dwDMPType = MiniDumpWithDataSegs;
#else
	m_dwDMPType = dwDMPType;
#endif
	
	_set_invalid_parameter_handler(myInvalidParameterHandler);
	_CrtSetReportMode(_CRT_ASSERT, 0);

	//_set_new_mode(1);

	//_set_se_translator(trans_func);
	set_unexpected(set_unexpected_function);
	//set_terminate(termfunction);
	//_set_new_handler(handle_program_memory_depletion);
	//_set_invalid_parameter_handler(myInvalidParameterHandler);
	//_set_purecall_handler(myPurecallHandler);
	//set_new_handler(newhandler);

	SzLog(_T("MyExcep : %p"), MyExcepFunc);
	LPTOP_LEVEL_EXCEPTION_FILTER  pLast = SetUnhandledExceptionFilter(MyExcepFunc);
	SzLog(_T("pLast : %p"), pLast);
	DisableSetUnhandledExceptionFilter();
	return S_OK;
}

HRESULT CMyExceptionManager::DumpExcepFile(IN struct _EXCEPTION_POINTERS *ExceptionInfo)
{
	HRESULT hr = S_FALSE;
	TCHAR szDumpFile[MAX_PATH] = {0};
	vGenDumpFile(szDumpFile, MAX_PATH);
	HANDLE hFile = CreateFile(szDumpFile, GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		SzLog(_T("DumpExcepFile CreateFile failed, err : %d"), GetLastError());
		return HRESULT_FROM_WIN32(GetLastError());
	}
	MINIDUMP_EXCEPTION_INFORMATION m1;
	m1.ThreadId = GetCurrentThreadId();
	m1.ExceptionPointers = ExceptionInfo;
	m1.ClientPointers = TRUE;
	BOOL bMiniDumpRet = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, (MINIDUMP_TYPE)m_dwDMPType , &m1, NULL, NULL);//MiniDumpNormal
	SzLog(_T("dumpFile : %s, dumpRet : %d, err : %08X\n"), szDumpFile, bMiniDumpRet, GetLastError());
	SzLog(_T("ExceptionCode:%08X, ExceptionFlags:%d, ExceptionAddress:%08X\n"), ExceptionInfo->ExceptionRecord->ExceptionCode, 
		ExceptionInfo->ExceptionRecord->ExceptionFlags, ExceptionInfo->ExceptionRecord->ExceptionAddress);
	
	CloseHandle(hFile);
	SzLog(_T("MiniDumpWriteDump ret : %d"), bMiniDumpRet);
	if(bMiniDumpRet)
	{
		//先关闭主窗口
		if(m_hWndMain != NULL)
		{
			::ShowWindow(m_hWndMain, SW_HIDE);
		}

		hr = vUploadDumpFile(ExceptionInfo, szDumpFile);

#ifdef DEBUG
		vShowErrDlg(ExceptionInfo, szDumpFile);
#else
		vShowErrDlg(NULL, NULL);
#endif
	}else
	{
		SzLog(_T("MiniDumpWriteDump failed, err : %d"), GetLastError());
		hr = HRESULT_FROM_WIN32(GetLastError());
	}
	
	return hr;
}

HRESULT CMyExceptionManager::vGenDumpFile(OUT TCHAR *szFilePath, IN DWORD dwStrLen)
{
	//生成dump文件路径，不需要创建文件
	if(szFilePath != NULL)
	{
		//只是测试
		TCHAR szCurPath[MAX_PATH] = {0};
		TCHAR szTimes[MAX_PATH] = {0};
		TCHAR szDate[MAX_PATH] = {0};	
		GetDateFormat(NULL, 0, NULL, _T("yyyyMMdd"), szDate, MAX_PATH);
		GetTimeFormat(NULL, TIME_FORCE24HOURFORMAT, NULL, _T("hh'-'mm'-'ss"), szTimes, MAX_PATH);
		GetCurrentPath(NULL, szCurPath, MAX_PATH);
		vAddLogPath(szCurPath);
		_stprintf_s(szFilePath, dwStrLen, _T("%s%s_%s.dmp"), szCurPath, szDate, szTimes);
		SzLog(_T("vGenDumpFile file : %s"), szFilePath);

		return S_OK;
	}else
		return E_INVALIDARG;

}

BOOL	CMyExceptionManager::vAddLogPath(IN TCHAR *szCurPath)
{
	TCHAR szLogPath[MAX_PATH] = {0};
	_stprintf_s(szLogPath, MAX_PATH, _T("%slog\\"), szCurPath);
	if(_taccess(szLogPath, 0x00) == -1)
	{
		//没目录需要创建
		::CreateDirectory(szLogPath, NULL);
	}
	BOOL bCreate = FALSE;
	_stprintf_s(szLogPath, MAX_PATH, _T("%slog\\dmp\\"), szCurPath);
	if(_taccess(szLogPath, 0x00) == -1)
	{
		//没目录需要创建
		bCreate = ::CreateDirectory(szLogPath, NULL);
	}else
	{
		bCreate = TRUE;
	}
	if(bCreate)
	{
		_tcscpy_s(szCurPath, MAX_PATH, szLogPath);
	}
	return bCreate;
}

void CMyExceptionManager::vShowErrDlg(IN struct _EXCEPTION_POINTERS *ExceptionInfo, IN TCHAR *szFilePath)
{
	
	mystring strText = _T("Sorry! Some bugs lead to the program crash!");
	if(szFilePath != NULL)
	{
		strText += _T("DMP File:");
		strText += szFilePath;
	}
	SzLog(_T("%s"), strText.c_str());
	AtlMessageBox(NULL, strText.c_str());
	
	//如果主窗口存在，关闭主窗口
	//
}


BOOL CMyExceptionManager::MyGetPEFileVersion(DWORD &dwFileVersionMS, DWORD &dwFileVersionLS, TCHAR *szPEFileName/* = NULL*/)
{	
	//szPEFileName如果为NULL则获取当前exe版本信息
	TCHAR szFileName[MAX_PATH + 32] = {0};
	if(NULL == szPEFileName)
	{
		if(!GetModuleFileName(NULL, szFileName, MAX_PATH))
		{
			SzLog(_T("MyGetPEFileVersion: GetModuleFileName failed err : %d"), GetLastError());
			return FALSE;
		}
	}else
	{
		_tcscpy_s(szFileName, MAX_PATH, szPEFileName);
	}

	TCHAR szVersionBuffer[8192] = _T("");   
	DWORD dwVerSize = 0;   
	DWORD dwHandle = 0;   
	
	BOOL bRet = FALSE;
	dwVerSize = GetFileVersionInfoSize(szFileName, &dwHandle);   

	if (dwVerSize)
	{
		if (GetFileVersionInfo(szFileName, dwHandle, dwVerSize, szVersionBuffer))   
		{   
			VS_FIXEDFILEINFO * pInfo;   
			unsigned int nInfoLen;   

			if (VerQueryValue(szVersionBuffer, _T("\\"), (void**)&pInfo, &nInfoLen))
			{	
				dwFileVersionMS = pInfo->dwFileVersionMS;
				dwFileVersionLS = pInfo->dwFileVersionLS;
				bRet = TRUE;
			}   
		}  
	} 	
	SzLog(_T("MyGetPEFileVersion:%s, result : %d, err : %d"), szFileName, bRet, GetLastError());
	return bRet;
}

HRESULT CMyExceptionManager::vUploadDumpFile(IN struct _EXCEPTION_POINTERS *ExceptionInfo, IN TCHAR *szFilePath)
{
	HRESULT hr = E_FAIL;
	if(m_dwDMPType == MiniDumpWithDataSegs)
	{
		mystring strText = szFilePath;
		strText  +=  _T(",此dmp文件太大，不上传!");
		return S_FALSE;
	}
	if(NULL == m_hWndMain || NULL == ExceptionInfo || NULL == szFilePath)
		return E_NOTIMPL;

	const int nBufflen = 1024;
	TCHAR szExceptionInfo[nBufflen] = {0};
	DWORD dwFileVersionMS = 0;
	DWORD dwFileVersionLS = 0;
	MyGetPEFileVersion(dwFileVersionMS, dwFileVersionLS);
	_stprintf_s(szExceptionInfo, nBufflen, 
		_T("ExceptionCode:%08X, ExceptionFlags:%d, ExceptionAddress:%08X, Version:%d.%d.%d.%d"), 
		ExceptionInfo->ExceptionRecord->ExceptionCode, 
		ExceptionInfo->ExceptionRecord->ExceptionFlags, 
		ExceptionInfo->ExceptionRecord->ExceptionAddress,
		HIWORD(dwFileVersionMS), LOWORD(dwFileVersionMS),
		HIWORD(dwFileVersionLS), LOWORD(dwFileVersionLS)
		);

	return hr;
}
