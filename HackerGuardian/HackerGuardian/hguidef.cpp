#include "stdafx.h"
#include "hguidef.h"

#include <strsafe.h>

CString DosDevicePathToLogicalPath(LPCTSTR lpszDosPath,int nMaxCchBuff)
{
	CString strResult;

	// Translate path with device name to drive letters.
	TCHAR szTemp[MAX_PATH];
	szTemp[0] = '\0';

	if ( lpszDosPath==NULL || !GetLogicalDriveStrings(_countof(szTemp)-1, szTemp) ){
		return lpszDosPath;
	}

	TCHAR szName[MAX_PATH];
	TCHAR szDrive[3] = TEXT(" :");
	BOOL bFound = FALSE;
	TCHAR* p = szTemp;

	do{
		// Copy the drive letter to the template string
		*szDrive = *p;

		// Look up each device name
		if ( QueryDosDevice(szDrive, szName, _countof(szName)) ){
			UINT uNameLen = (UINT)_tcslen(szName);

			if (uNameLen < MAX_PATH) 
			{
				bFound = _tcsnicmp(lpszDosPath, szName, uNameLen) == 0;

				if ( bFound ){
					// Reconstruct pszFilename using szTemp
					// Replace device path with DOS path
					strResult = szDrive;
					strResult += lpszDosPath+uNameLen;
					strResult.MakeLower();
				}
			}
		}

		// Go to the next NULL character.
		while (*p++);
	} while (!bFound && *p); // end of string

	//safe path
	if ( strResult.GetLength()>nMaxCchBuff ){
		strResult = strResult.Left(nMaxCchBuff);
	}

	return strResult;
}

CString FormatDosPath(LPCTSTR lpszDosPath)
{
	CString strTmp;
	if (!lpszDosPath)
	{
		return strTmp;
	}
	strTmp = lpszDosPath;
	WCHAR WindowsDirectory[MAX_PATH] = {0};
	GetWindowsDirectoryW(WindowsDirectory, MAX_PATH);
	// handle macro
	if (strTmp.Find(TEXT("\\SystemRoot")) >= 0)
	{
		strTmp.Replace(TEXT("\\SystemRoot"), WindowsDirectory);
	}
	if (strTmp.Find(TEXT("%SystemRoot%")) >= 0)
	{
		strTmp.Replace(TEXT("%SystemRoot%"), WindowsDirectory);
	}
	if (strTmp.Find(TEXT("%systemroot%")) >= 0)
	{
		strTmp.Replace(TEXT("%systemroot%"), WindowsDirectory);
	}
	if (wcslen(lpszDosPath) > wcslen(L"system32\\") && 
		!_wcsnicmp(lpszDosPath, L"system32\\", wcslen(L"system32\\")))
	{
		strTmp.Format(L"%s\\%s", WindowsDirectory, lpszDosPath/* + wcslen(L"system32\\")*/);
	}
	// handle NT path
	if (strTmp.Find(TEXT("\\Device\\HarddiskVolume")) >= 0)
	{
		strTmp = DosDevicePathToLogicalPath(strTmp.GetBuffer(strTmp.GetLength()), SZ_PATH_LEN);
	}

	if (strTmp.Find(TEXT("\\??\\")) >= 0){
		strTmp = strTmp.Right(strTmp.GetLength() - (int)_tcslen(TEXT("\\??\\")));
	}

	return strTmp;
}

CString GetFileCompany(CString strPath)
{
	CString strCompany;
	
	if (strPath.IsEmpty() || !PathFileExists(strPath)){
		return strCompany;
	}

	LPWSTR pwszFileName = (LPWSTR)(LPCWSTR)strPath;
	DWORD dwHandle = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSizeW(pwszFileName, &dwHandle);

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	if (dwVerInfoSize)
	{
		HLOCAL pBuffer = LocalAlloc(LMEM_ZEROINIT, dwVerInfoSize);
		if (pBuffer)
		{
			if (GetFileVersionInfoW(pwszFileName, dwHandle, dwVerInfoSize, pBuffer))
			{
				UINT nLen = 0;
				if (VerQueryValueW(pBuffer, L"\\VarFileInfo\\Translation", (LPVOID*)&lpTranslate, &nLen))
				{
					LPCWSTR pwszBlock = 0;
					UINT cbSizeBuff = 0;
					WCHAR pwszSubBlock[SZ_PATH_LEN] = {0};

					if ((nLen / sizeof(struct LANGANDCODEPAGE)) > 0)
					{
						StringCchPrintf(pwszSubBlock, 
										sizeof(pwszSubBlock) / sizeof(WCHAR),
										L"\\StringFileInfo\\%04X%04X\\CompanyName",
										lpTranslate[0].wLanguage,
										lpTranslate[0].wCodePage);
					}

					if (VerQueryValueW(pBuffer, pwszSubBlock, (LPVOID*)&pwszBlock, &cbSizeBuff))
					{
						WCHAR pwszCompayName[SZ_PATH_LEN] = {0};
						StringCchCopy(pwszCompayName, SZ_PATH_LEN / sizeof(WCHAR), (LPCWSTR)pwszBlock);
						strCompany = pwszCompayName;
					}
				}
			}
			LocalFree(pBuffer);
		}
	}

	return strCompany;
}

HRESULT CreateProcessDump(DWORD dwProcessId, PWCHAR pwszProcessName,BOOL bMiniDump)
{
	HRESULT hr = S_FALSE;
	HANDLE hProcess = NULL;
	HANDLE hFile = NULL;

	WCHAR pwszCurPath[SZ_PATH_LEN] = {0};
	WCHAR pwszTimes[MAX_PATH] = {0};
	WCHAR pwszDate[MAX_PATH] = {0};	

	if (pwszProcessName == NULL){
		return S_FALSE;
	}

	GetDateFormatW(NULL, 0, NULL, _T("yyyyMMdd"), pwszDate, MAX_PATH);
	//GetTimeFormatW(NULL, TIME_FORCE24HOURFORMAT, NULL, _T("hh'-'mm'-'ss"), pwszTimes, MAX_PATH);
	GetTimeFormatW(NULL, TIME_FORCE24HOURFORMAT, NULL, _T("hhmmss"), pwszTimes, MAX_PATH);
 	GetModuleFileNameW(NULL, pwszCurPath, SZ_PATH_LEN);
	wcsrchr(pwszCurPath, L'\\')[1] = '\0';
	wcscat_s(pwszCurPath, pwszProcessName);
	wcscat_s(pwszCurPath, L"_Dump_");
	wcscat_s(pwszCurPath, pwszDate);
	wcscat_s(pwszCurPath, pwszTimes);
	wcscat_s(pwszCurPath, L".dmp");

	//
	// Open process by process id and get process handle.
	// 
	hProcess = OpenProcess(GENERIC_ALL, FALSE, dwProcessId);
	if (!hProcess){
		AtlMessageBox(NULL, L"Can't open this process!", L"TIPS", MB_OK);
		return S_FALSE;
	}

	//
	// Create dump file.
	// 
	hFile = CreateFileW(pwszCurPath, 
		GENERIC_WRITE, 
		FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE, 
		NULL, 
		CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL, 
		NULL);
	if(INVALID_HANDLE_VALUE == hFile)
	{
		SzLog(_T("DumpExcepFile CreateFile failed, err : %d"), GetLastError());
		return HRESULT_FROM_WIN32(GetLastError());
	}
	// steps (references by ProcessExplorer):
	// 1. LoadLibraryW(L"Dbghelp.dll");
	// 2. GetProcAddress: EnumDirTreeW;
	// 3. if EnumDirTreeW exists, Dbghelp.dll version > 6.1, then goto step 4, otherwise goto step 5;
	// 4. if Dbghelp.dll version < 6.1
	//	  Mini DumpType - MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs
	//	  Full DumpType - MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithFullMemory
	// 5. if Dbghelp.dll version > 6.1
	//	  Mini DumpType - MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithThreadInfo
	//	  Full DumpType - MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithThreadInfo | MiniDumpWithFullMemory
	//	  
	DWORD dwDumpType = MiniDumpNormal;
	HMODULE hDbghelp = LoadLibraryW(L"Dbghelp.dll");
	FARPROC pfnEnumDirTreeW = NULL;
	if (hDbghelp != NULL){
		pfnEnumDirTreeW = GetProcAddress(hDbghelp, "EnumDirTreeW");
	}

	// Dbghelp.dll version < 6.1
	if (pfnEnumDirTreeW == NULL){
		if (bMiniDump == TRUE){
			dwDumpType = MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs;
		}else{
			dwDumpType = MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithFullMemory;
		}
	}else{	// Dbghelp.dll version > 6.1
		if (bMiniDump == TRUE){
			dwDumpType = MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithThreadInfo;
		}else{
			dwDumpType = MiniDumpWithProcessThreadData | MiniDumpWithHandleData | MiniDumpWithDataSegs | MiniDumpWithThreadInfo | MiniDumpWithFullMemory;
		}
	}

	//
	//
	// 
	BOOL bMiniDumpRet = MiniDumpWriteDump(hProcess, dwProcessId, hFile, (MINIDUMP_TYPE)dwDumpType , NULL, NULL, NULL);

	if(bMiniDumpRet){
		AtlMessageBox(NULL, pwszCurPath, L"Dump File Path", MB_OK);
	}else{
		SzLog(_T("MiniDumpWriteDump failed, err : %d"), GetLastError());
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	if (hFile != NULL){
		CloseHandle(hFile);
	}

	if (hProcess != NULL){
		CloseHandle(hProcess);
	}

	return hr;
}

CString GetLongPath(CString szPath)
{
	CString strPath;

	if (szPath.Find(L'~') != -1)
	{
		WCHAR szLongPath[MAX_PATH] = {0};
		GetLongPathName(szPath, szLongPath, MAX_PATH);
		strPath = szLongPath;
	}
	else
	{
		strPath = szPath;
	}

	return strPath;
}

std::vector<std::wstring> ParseStringSequenceW(PWCHAR s, SIZE_T nTotalLen)
{
	PWCHAR current_wchar = s;
	SIZE_T nAccumulation = 0;
	std::vector<std::wstring> vStrRtn;
	while (*current_wchar)
	{
		vStrRtn.push_back(current_wchar);
		current_wchar += wcslen(current_wchar) + 1;
		nAccumulation += wcslen(current_wchar) + 1;
	}

	if (nAccumulation > nTotalLen)
	{
		vStrRtn.clear();
	}

	return vStrRtn;
}

unsigned long StrHexToULongA(char* StrHexA, int* errorno)
{
	unsigned long lRtn = 0;
	char BufferA[MAX_PATH] = {0};
	char *p = BufferA;
	char *_EndPtr = NULL;

	if (StrHexA[0] != '0' && StrHexA[1] != 'x')
	{
		strcpy_s(BufferA, MAX_PATH, "0x");
	}
	strcat_s(BufferA, MAX_PATH, StrHexA);
	p = BufferA;

	lRtn = strtoul(p, &_EndPtr, 16);
	if (_EndPtr != (p + strlen(BufferA)))
	{
		*errorno = -1;
	}
	*errorno = 0;
	return lRtn;
}

CString GetCurrentDatetimeString()
{
	struct tm *local = (struct tm*)malloc(sizeof(struct tm));
	time_t t;
	t = time(NULL);
	localtime_s(local, &t);

	CString strRtn;
	strRtn.Format(L"%04d%02d%02d%02d%02d%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, 
		local->tm_hour, local->tm_min, local->tm_sec);

	if (local){
		RtlZeroMemory(local, sizeof(struct tm));
		free(local);
		local = NULL;
	}

	return strRtn;
}