#ifndef __LIST_DICT_H__
#define __LIST_DICT_H__

/************************************************************************/
/* Define Structure                                                     */
/************************************************************************/
template<class T>
struct CSortListViewItem
{
	// Comparison function. You must override it
	int Compare(T* pItem,int nField){	
		ATLASSERT(FALSE);
		return 0;
	}
};

/************************************************************************/
/* Define process info structure                                        */
/************************************************************************/
typedef struct _SZ_PROCESSINFO: public CSortListViewItem<_SZ_PROCESSINFO>{
	TCHAR		m_ptszName[MAX_PATH];
	size_t		m_nPid;
	size_t		m_nParentPid;
	TCHAR		m_ptszPath[SZ_PATH_LEN];
	size_t		m_nEProcessAddress;
	WCHAR		m_pwszAccess[8];
	WCHAR		m_pwszCompany[MAX_PATH];
	COLORREF	m_RGB;

	_SZ_PROCESSINFO()
		: m_nPid(-1)
		, m_nParentPid(0)
		, m_nEProcessAddress(0)
		, m_RGB(RGB(255, 0, 0))
	{
		memset(m_ptszName, 0, sizeof(TCHAR) * MAX_PATH);
		memset(m_ptszPath, 0, sizeof(TCHAR) * SZ_PATH_LEN);
		ZeroMemory(m_pwszAccess, sizeof(WCHAR) * 8);
		ZeroMemory(m_pwszCompany, sizeof(WCHAR) * MAX_PATH);
	}

	_SZ_PROCESSINFO(
		PTCHAR		ptszName, 
		size_t		dwPid, 
		size_t		ulParentPid, 
		PTCHAR		ptszPath,
		size_t		ulEProcessAddress,
		PWCHAR		pwszAccess,
		PWCHAR		pwszCompany,
		COLORREF	rgb = RGB(255, 0, 0))
		: m_nPid(dwPid)
		, m_nParentPid(ulParentPid)
		, m_nEProcessAddress(ulEProcessAddress)
		, m_RGB(rgb)
	{
		_tcscpy_s(m_ptszName, ptszName);
		_tcscpy_s(m_ptszPath, ptszPath);
		wcscpy_s(m_pwszAccess, pwszAccess);
		wcscpy_s(m_pwszCompany, pwszCompany);
	}

	int Compare(_SZ_PROCESSINFO* pi, int nField){
		switch (nField){
		case 0:	// Compare only the filename - regardless of the path
			return _tcsicmp(m_ptszName, pi->m_ptszName);
		case 1:	// Do you know any good comparison function for 64 bits integers???
			return m_nPid > pi->m_nPid ? 
				1 : (m_nPid > pi->m_nPid ? -1 : 0);
		case 2:	// parent PID
			return m_nParentPid > pi->m_nParentPid ? 
				1 : (m_nParentPid > pi->m_nParentPid ? -1 : 0);
		case 3:	// Compare time stamp
			return _tcsicmp(m_ptszPath, pi->m_ptszPath);
		case 4:	// EPROCESS Address
			return m_nEProcessAddress > pi->m_nEProcessAddress
				? 1 : (m_nEProcessAddress > 
				pi->m_nEProcessAddress ? -1 : 0);
		case 5:	// access
			return _tcsicmp(m_pwszAccess, pi->m_pwszAccess);
		case 6:	// access
			return _tcsicmp(m_pwszCompany, pi->m_pwszCompany);
		}
		return 0;
	}
}SZ_PROCESSINFO, *PSZ_PROCESSINFO;

/************************************************************************/
/* Define module info structure                                         */
/************************************************************************/
typedef struct _SZ_MODULEINFO: public CSortListViewItem<_SZ_MODULEINFO>{
	WCHAR		m_pwszName[MAX_PATH];
	WCHAR		m_pwszPath[SZ_PATH_LEN];
	size_t		m_nImageBase;
	size_t		m_nImageSize;
	WCHAR		m_pwszCompany[MAX_PATH];
	COLORREF	m_RGB;

	_SZ_MODULEINFO()
		: m_nImageBase(0)
		, m_nImageSize(0)
		, m_RGB(RGB(255, 0, 0))
	{
		memset(m_pwszName, 0, sizeof(WCHAR) * MAX_PATH);
		memset(m_pwszPath, 0, sizeof(WCHAR) * SZ_PATH_LEN);
		ZeroMemory(m_pwszCompany, sizeof(WCHAR) * MAX_PATH);
	}

	_SZ_MODULEINFO(
		PWCHAR	pwszName, 
		PWCHAR	pwszPath, 
		size_t	nImageBase,
		size_t	nImageSize,
		PWCHAR	pwszCompany,
		COLORREF	rgb = RGB(255, 0, 0))
		: m_nImageBase(nImageBase)
		, m_nImageSize(nImageSize)
		, m_RGB(rgb)
	{
		wcscpy_s(m_pwszName, pwszName);
		wcscpy_s(m_pwszPath, pwszPath);
		wcscpy_s(m_pwszCompany, pwszCompany);
	}

	int Compare(_SZ_MODULEINFO* mi, int nField){
		switch (nField){
		case 0:	// 
			return _wcsicmp(m_pwszName, mi->m_pwszName);
		case 1:	// 
			return _wcsicmp(m_pwszPath, mi->m_pwszPath);
		case 2:	// 
			return m_nImageBase > mi->m_nImageBase ? 1 : (m_nImageBase > mi->m_nImageBase ? -1 : 0);
		case 3:	// 
			return m_nImageSize > mi->m_nImageSize ? 1 : (m_nImageSize > mi->m_nImageSize ? -1 : 0);
		case 4:	// 
			return _wcsicmp(m_pwszCompany, mi->m_pwszCompany);
		}
		return 0;
	}
}SZ_MODULEINFO, *PSZ_MODULEINFO;

/************************************************************************/
/* Define thread info structure                                         */
/************************************************************************/
typedef struct _SZ_THREADINFO: public CSortListViewItem<_SZ_THREADINFO>{
	size_t		m_nTid;
	size_t		m_nEThread;
	size_t		m_nTeb;
	size_t		m_nPriority;
	size_t		m_nWin32StartAddress;
	SIZE_T		m_nStartAddress;
	WCHAR		m_pwszModuleName[MAX_PATH];
	size_t		m_nSwitchCount;
	size_t		m_nState;		// 0 - wait; 1 - wait; 2 - terminated.

	_SZ_THREADINFO()
		: m_nTid(0)
		, m_nEThread(0)
		, m_nTeb(0)
		, m_nPriority(0)
		, m_nWin32StartAddress(0)
		, m_nStartAddress(0)
		, m_nSwitchCount(0)
		, m_nState(0)
	{
		ZeroMemory(m_pwszModuleName, sizeof(WCHAR) * MAX_PATH);
	}

	_SZ_THREADINFO(
		size_t	nTid,
		size_t	nEThread,
		size_t	nTeb,
		size_t	nPriority,
		size_t	nWin32StartAddress,
		SIZE_T	nStartAddress,
		PWCHAR	pwszModuleName,
		size_t	nSwitchCount,
		size_t	nState)
		: m_nTid(nTid)
		, m_nEThread(nEThread)
		, m_nTeb(nTeb)
		, m_nPriority(nPriority)
		, m_nWin32StartAddress(nWin32StartAddress)
		, m_nStartAddress(nStartAddress)
		, m_nSwitchCount(nSwitchCount)
		, m_nState(nState)
	{
		wcscpy_s(m_pwszModuleName, pwszModuleName);
	}

	int Compare(_SZ_THREADINFO* ti, int nField){
		switch (nField){
		case 0:	// 
			return m_nTid > ti->m_nTid ? 1 : (m_nTid > ti->m_nTid ? -1 : 0);
		case 1:	// 
			return m_nEThread > ti->m_nEThread ? 1 : (m_nEThread > ti->m_nEThread ? -1 : 0);
		case 2:	// 
			return m_nTeb > ti->m_nTeb ? 1 : (m_nTeb > ti->m_nTeb ? -1 : 0);
		case 3:	// 
			return m_nPriority > ti->m_nPriority ? 1 : (m_nPriority > ti->m_nPriority ? -1 : 0);
		case 4:	// 
			return m_nWin32StartAddress > ti->m_nWin32StartAddress ? 1 : (m_nWin32StartAddress > ti->m_nWin32StartAddress ? -1 : 0);
		case 5:	// 
			return m_nStartAddress > ti->m_nStartAddress ? 1 : (m_nStartAddress > ti->m_nStartAddress ? -1 : 0);
		case 6:	// 
			return _wcsicmp(m_pwszModuleName, ti->m_pwszModuleName);
		case 7:	// 
			return m_nSwitchCount > ti->m_nSwitchCount ? 1 : (m_nSwitchCount > ti->m_nSwitchCount ? -1 : 0);
		case 8:	// 
			return m_nState > ti->m_nState ? 1 : (m_nState > ti->m_nState ? -1 : 0);
		}
		return 0;
	}
}SZ_THREADINFO, *PSZ_THREADINFO;

/************************************************************************/
/* Define memory info structure                                         */
/************************************************************************/
typedef struct _HG_MEMORY_INFO: public CSortListViewItem<_HG_MEMORY_INFO>{
	size_t		m_nBase;
	size_t		m_nSize;
	size_t		m_nProtect;
	size_t		m_nState;
	size_t		m_nType;
	SIZE_T		m_pEProcess;
	SIZE_T		m_Pid;
	WCHAR		m_szModuleNameW[SZ_PATH_LEN];

	_HG_MEMORY_INFO()
		: m_nBase(0)
		, m_nSize(0)
		, m_nProtect(0)
		, m_nState(0)
		, m_nType(0)
		, m_pEProcess(0)
		, m_Pid(0)
	{
		ZeroMemory(m_szModuleNameW, sizeof(WCHAR) * SZ_PATH_LEN);
	}

	_HG_MEMORY_INFO(
		size_t	nBase,
		size_t	nSize,
		size_t	nProtect,
		size_t	nState,
		size_t	nType,
		SIZE_T	pEProcess,
		SIZE_T	Pid,
		PWCHAR	szModuleNameW)
		: m_nBase(nBase)
		, m_nSize(nSize)
		, m_nProtect(nProtect)
		, m_nState(nState)
		, m_nType(nType)
		, m_pEProcess(pEProcess)
		, m_Pid(Pid)
	{
		wcscpy_s(m_szModuleNameW, szModuleNameW);
	}

	int Compare(_HG_MEMORY_INFO* ti, int nField){
		switch (nField){
		case 0:	// 
			return m_nBase > ti->m_nBase ? 1 : (m_nBase > ti->m_nBase ? -1 : 0);
		case 1:	// 
			return m_nSize > ti->m_nSize ? 1 : (m_nSize > ti->m_nSize ? -1 : 0);
		case 2:	// 
			return m_nProtect > ti->m_nProtect ? 1 : (m_nProtect > ti->m_nProtect ? -1 : 0);
		case 3:	// 
			return m_nState > ti->m_nState ? 1 : (m_nState > ti->m_nState ? -1 : 0);
		case 4:	// 
			return m_nType > ti->m_nType ? 1 : (m_nType > ti->m_nType ? -1 : 0);
		case 5:	// 
			return _wcsicmp(m_szModuleNameW, ti->m_szModuleNameW);
		}
		return 0;
	}
}HG_MEMORY_INFO, *PHG_MEMORY_INFO;

/************************************************************************/
/* Define handle info structure                                         */
/************************************************************************/
typedef struct _HG_HANDLE_INFO: public CSortListViewItem<_HG_HANDLE_INFO>{
	WCHAR		m_strHandleType[64];
	WCHAR		m_strHandleName[SZ_PATH_LEN];
	SIZE_T		m_nHandle;
	SIZE_T		m_nHandleObject;
	SIZE_T		m_nHandleTypeCode;
	SIZE_T		m_nReferenceCount;
	SIZE_T		m_pEProcess;
	SIZE_T		m_Pid;

	_HG_HANDLE_INFO()
		: m_nHandle(0)
		, m_nHandleObject(0)
		, m_nHandleTypeCode(0)
		, m_nReferenceCount(0)
		, m_pEProcess(0)
		, m_Pid(0)
	{
		ZeroMemory(m_strHandleType, sizeof(WCHAR) * 64);
		ZeroMemory(m_strHandleName, sizeof(WCHAR) * SZ_PATH_LEN);
	}

	_HG_HANDLE_INFO(
		PWCHAR	HandleTypeW,
		PWCHAR	HandleNameW,
		SIZE_T	nHandle,
		SIZE_T	nHandleObject,
		SIZE_T	nHandleTypeCode,
		SIZE_T	nReferenceCount,
		SIZE_T	pEProcess,
		SIZE_T	Pid)
		: m_nHandle(nHandle)
		, m_nHandleObject(nHandleObject)
		, m_nHandleTypeCode(nHandleTypeCode)
		, m_nReferenceCount(nReferenceCount)
		, m_pEProcess(pEProcess)
		, m_Pid(Pid)
	{
		wcscpy_s(m_strHandleType, HandleTypeW);
		wcscpy_s(m_strHandleName, HandleNameW);
	}

	int Compare(_HG_HANDLE_INFO* arg0, int nField){
		switch (nField){
		case 0:	// 
			return _wcsicmp(m_strHandleType, arg0->m_strHandleType);
		case 1:	// 
			return _wcsicmp(m_strHandleName, arg0->m_strHandleName);
		case 2:	// 
			return m_nHandle > arg0->m_nHandle ? 1 : (m_nHandle > arg0->m_nHandle ? -1 : 0);
		case 3:	// 
			return m_nHandleObject > arg0->m_nHandleObject ? 1 : (m_nHandleObject > arg0->m_nHandleObject ? -1 : 0);
		case 4:	// 
			return m_nHandleTypeCode > arg0->m_nHandleTypeCode ? 1 : (m_nHandleTypeCode > arg0->m_nHandleTypeCode ? -1 : 0);
		case 5:	// 
			return m_nReferenceCount > arg0->m_nReferenceCount ? 1 : (m_nReferenceCount > arg0->m_nReferenceCount ? -1 : 0);
		}
		return 0;
	}
}HG_HANDLE_INFO, *PHG_HANDLE_INFO;


#endif