#pragma once

class CMyExceptionManager
{
public:
	CMyExceptionManager(void);
	~CMyExceptionManager(void);

public:
	
	HRESULT SetUnHandleException(DWORD dwDMPType = 0);
	void SetWndMain(HWND hWnd){
		m_hWndMain = hWnd;
	}
	HRESULT DumpExcepFile(IN struct _EXCEPTION_POINTERS *ExceptionInfo);
	

	DWORD GetCurrentPath(IN HMODULE hModule, OUT TCHAR *szPath, IN DWORD dwSize);


	static BOOL  MyGetPEFileVersion(DWORD &dwFileVersionMS, DWORD &dwFileVersionLS, TCHAR *szPEFileName = NULL);
protected:
	HRESULT vGenDumpFile(OUT TCHAR *szFilePath, IN DWORD dwStrLen);
	HRESULT vUploadDumpFile(IN struct _EXCEPTION_POINTERS *ExceptionInfo, IN TCHAR *szFilePath);
	BOOL	vAddLogPath(IN TCHAR *szCurPath);

	void	vShowErrDlg(IN struct _EXCEPTION_POINTERS *ExceptionInfo, IN TCHAR *szFilePath);
protected:
	DWORD	m_dwDMPType;
	HWND	m_hWndMain;
};
