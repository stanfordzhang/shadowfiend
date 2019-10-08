#ifndef _SZ_DEF_H
#define _SZ_DEF_H

#include <stdio.h>
#include <tchar.h>
#ifdef __cplusplus
#include <string>
#endif

#define PTODO_LINENUMBER_TO_STRING(x) #x
#define PTODO_LINENUMBER(x) PTODO_LINENUMBER_TO_STRING(x)
#define SZ_WARNING(string) __FILE__"(" PTODO_LINENUMBER(__LINE__) "): "string

#define  _OUTPUT_STRHEADER _T(">> SzLog <<: ")

#define SZ_PATH_LENGTH	MAX_PATH * 4
//
// File path length
// 
#define		SZ_PATH_LEN					2 * MAX_PATH
#define		SZ_PATH_LEN_WCHAR			SZ_PATH_LEN * sizeof(WCHAR)

#ifdef __cplusplus
#ifndef mystring
#ifdef _UNICODE
#define mystring wstring
#else
#define mystring string
#endif
#endif
#endif

#ifdef _DEBUG
#define _TRACK
#endif

#ifdef _TRACK
__inline void SzLog(const TCHAR* ptzFormat, ...)
{
#pragma warning(disable:4996)
	va_list vlArgs;	
	TCHAR tzText[1024];
	TCHAR tzText2[1152]; // 1024 + 128
	if(!ptzFormat)
		return;
	va_start(vlArgs, ptzFormat);
	if(_vsctprintf(ptzFormat,vlArgs) < 1024)
	{
#ifdef _UNICODE
	vswprintf_s(tzText, 1024, ptzFormat, vlArgs);
#else
	_vstprintf_s(tzText, 1024, ptzFormat, vlArgs);
#endif
	}else
	{
		_tcscpy(tzText, _T("Input va_list too long, won't show the values!!!!!!!!"));
	}

	_stprintf_s(tzText2, 1152, _T("%s%s\n"), _OUTPUT_STRHEADER, tzText);
	OutputDebugString(tzText2);
#ifdef _Print_To_Console
	_tprintf(tzText2);
#endif
	va_end(vlArgs);
#pragma warning(default:4996)
}

#else
#define SzLog(x,...)
//__inline void SzLog(const TCHAR* ptzFormat, ...){}
#endif

#ifdef _TRACK
__inline void SzLogA(const char* ptzFormat, ...)
{
#pragma warning(disable:4996)
	va_list vlArgs;	
	char tzText[1024];
	char tzText2[1152]; // 1024 + 128
	if(!ptzFormat)
		return;
	va_start(vlArgs, ptzFormat);
	if(_vscprintf(ptzFormat,vlArgs) < 1024)
	{
		vsprintf_s(tzText, 1024, ptzFormat, vlArgs);
	}
	else
	{
		strcpy_s(tzText, 1024, "Input va_list too long, won't show the values!!!!!!!!");
	}

	sprintf_s(tzText2, 1152, "%s%s\n", _OUTPUT_STRHEADER, tzText);
	OutputDebugStringA(tzText2);
#ifdef _Print_To_Console
	_tprintf(tzText2);
#endif
	va_end(vlArgs);
#pragma warning(default:4996)
}

#else
#define SzLogA(x,...)
//__inline void SzLog(const TCHAR* ptzFormat, ...){}
#endif

__forceinline void OutputErrorMessage(LPCTSTR lpctszInfo, const DWORD dwErrorCode)
{
	HRESULT hr = S_OK;

	LPVOID lpMsgBuf = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dwErrorCode,
		0, // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL);
	SzLogA("%s", SZ_WARNING(""));
	SzLog(TEXT(">>> %s, Error Code: %d, Message: %s"), lpctszInfo, dwErrorCode, lpMsgBuf);

	LocalFree(lpMsgBuf);
}

enum SYSTYPE
{
	WIN2K = 0,
	WINXP,
	WINVISTA,
	WIN7,
	INVALID_VERSION
};

#ifndef SafeFree
#define SafeFree(p)\
	if(p){free(p);p=NULL;}
#endif

#ifndef SafeDelete
#define SafeDelete(p)\
	if(p){delete (p);p=NULL;}
#endif

#ifndef SafeDeletes
#define SafeDeletes(p)\
if(p){delete [] (p);p=NULL;}
#endif

#ifndef SafeCloseHandle
#define SafeCloseHandle(h)\
	if(h){CloseHandle(h); h = NULL;}
#endif

#ifndef SafeCloseFile
#define SafeCloseFile(pf)\
	if(pf){fclose(pf); pf = NULL;}
#endif

#ifndef WaitCloseThread
#define WaitCloseThread(hhh, ttt)\
	if(hhh && ttt != 0){WaitForSingleObject(hhh, ttt);}\
	SafeCloseHandle(hhh);
#endif

#ifndef SafeFreeLibrary
#define SafeFreeLibrary(h)\
if(h){FreeLibrary(h); h = NULL;}
#endif

#ifndef SafeReleaseP
#define SafeReleaseP(p)\
if(p){p->Release(); p = NULL;}
#endif


#ifndef SafeAddRef
#define SafeAddRef(p)\
if(p){p->AddRef(); }
#endif

#define CheckNull(p)\
	if(!p){return E_ParameterNull;}

#ifndef CheckFailP
#define CheckFailP(hr, p)\
if (FAILED(hr) || !p){return hr;}
#endif


#ifndef MAILSETBSTR
#define MAILSETBSTR(pIn, p)\
	if(pIn){p = SysAllocString(pIn);}
#endif

#ifndef SAFEFRREBSTR
#define SAFEFRREBSTR(p)\
	if(p){SysFreeString(p);p = NULL;}
#endif

#ifndef CheckPointer
#define CheckPointer(p,ret) { _ASSERTE((p) != NULL);if((p)==NULL) return (ret);}
#endif

#define MyClearPath(szPath)\
{\
	int nCl1 = (int)_tcslen(szPath) - 1;\
	while (nCl1 >= 0)\
	{\
		if(szPath[nCl1] == '\\')\
			nCl1 --;\
		else\
			break;\
	}\
	szPath[nCl1 + 1] = '\0';\
}

#define MyParentPath(szFullPath)\
{\
	int nnnlen = (int)_tcslen(szFullPath) - 1;\
	while (nnnlen >= 0)\
	{\
		if(szFullPath[nnnlen] == '\\')\
		{\
		szFullPath[nnnlen] = '\0';\
		break;\
		}\
		nnnlen --;\
	}\
}

#define _MyLLPrintf(szStr, llData)\
if(szStr)\
{\
	DWORD high = llData >> 32;\
	DWORD low = llData & 0xFFFFFFFF;\
	_stprintf(szStr, _T("%08X%08X"), high, low);\
}


#define _AddXGanString(szXString)\
	if(szXString && (szXString[_tcslen(szXString) - 1] != '\\'))\
	{\
	_tcscat(szXString, _T("\\"));\
	}

#define SafeMemcpy(pcpdst, pcpsrc, cpsize)\
if(pcpdst && pcpsrc)\
{\
	memcpy(pcpdst, pcpsrc, cpsize);\
}

__forceinline BOOL iMyIs64BitCPU()
{
	SYSTEM_INFO si;
	GetNativeSystemInfo(&si);

	if (si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64 ||    
		si.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64 )
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	} 
}

typedef BOOL(WINAPI* NEP_MyIsWow64ProcessFunc)( __in  HANDLE hProcess, __out PBOOL Wow64Process);

//判断是不是低于2K系统，类似2000，98或者更低返回FALSE
//XP,2003及以上返回TRUE
__forceinline BOOL iIsHighOS()
{
	OSVERSIONINFO svex;
	svex.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&svex);
	if(svex.dwMajorVersion >= 6)
		return TRUE;
	else if(svex.dwMajorVersion <= 4)
		return FALSE;
	else//dwMajorVersion==5
	{
		if(svex.dwMinorVersion == 1 || svex.dwMinorVersion == 2)
		{
			//1 is XP; 2 is 2003
			return TRUE;
		}else
			return FALSE;
	}
}

__forceinline BOOL iMyIs64BitOS()
{
	BOOL bIs64 = FALSE;
	if(iIsHighOS())
	{
		NEP_MyIsWow64ProcessFunc pIsWow64Process = (NEP_MyIsWow64ProcessFunc)GetProcAddress(GetModuleHandle(_T("Kernel32.dll")), "IsWow64Process");
		if(pIsWow64Process)
		{
			pIsWow64Process(GetCurrentProcess(), &bIs64);
		}
	}
	return bIs64;
}

__forceinline BOOL MyIs64bitSystem()
{

	if (iMyIs64BitOS())
	{
		//SzLog(_T("n1 当前是64系统!"));
		return TRUE;
	}
	else
	{
		//SzLog(_T("n1 当前是32系统!"));
		return FALSE;
	} 
}
		//LPVOID lpMsgBuf;
	//FormatMessage( 
	//	FORMAT_MESSAGE_ALLOCATE_BUFFER | 
	//	FORMAT_MESSAGE_FROM_SYSTEM | 
	//	FORMAT_MESSAGE_IGNORE_INSERTS,
	//	NULL,
	//	GetLastError(),
	//	0, // Default language
	//	(LPTSTR) &lpMsgBuf,
	//	0,
	//	NULL 
	//	);
	//AfxMessageBox((LPTSTR)lpMsgBuf);

#endif/*_NE_COMMON_MACRO_H*/

//////////////////////////////////////////////////////////////////////
// List Entry
// typedef struct _SzListEntry{
// 	PVOID					pElement;
// 	struct _SzListEntry*	pNext;
// }SzListEntry, *PSzListEntry;
// 
// __forceinline void InsertListEntry(PSzListEntry* pList, PVOID pElement)
// {
// 	PSzListEntry pTmp = *pList;
// 	PSzListEntry pTail = NULL;
// 	PSzListEntry e = (PSzListEntry)malloc(sizeof(SzListEntry));
// 	e->pElement = pElement;
// 	e->pNext = NULL;
// 	while(pTmp != NULL)
// 	{
// 		pTail = pTmp;
// 		pTmp = (PSzListEntry)pTmp->pNext;
// 	}
// 	if (*pList == NULL)
// 	{
// 		*pList = e;
// 	}
// 	else
// 	{
// 		pTail->pNext = e;
// 	}
// }
// 
// __forceinline void ReleaseListEntry(PSzListEntry pList)
// {
// 	PSzListEntry pTmp = NULL;
// 	while(pList != NULL)
// 	{
// 		pTmp = pList;
// 		pList = (PSzListEntry)pList->pNext;
// 		if (pTmp->pElement != NULL)
// 		{
// 			free((void*)(pTmp->pElement));
// 			pTmp->pElement = NULL;
// 		}
// 		if (pTmp != NULL)
// 		{
// 			free((void*)pTmp);
// 			pTmp = NULL;
// 		}
// 	}
// }
///////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////// 
