// HackerGuardian.cpp : main source file for HackerGuardian.exe
//

#include "stdafx.h"

#include "resource.h"

#include "aboutdlg.h"
#include "MainFrm.h"
#include "..\references\dmp\MyExceptionManager.h"

CAppModule _Module;

CMyExceptionManager g_Dump;

int Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainFrame wndMain;

	if(wndMain.CreateEx() == NULL)
	{
		ATLTRACE(_T("Main window creation failed!\n"));
		return 0;
	}

	wndMain.ShowWindow(nCmdShow);
	g_Dump.SetWndMain(wndMain.m_hWnd);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	//
	// 
	// 
	if (GetWinVer() == eWin_Other){
		AtlMessageBox(NULL, TEXT("Sorry! This program only supports 5 Windows OS, as follows:\r\n    Windows XP \r\n    Windows 7 32-Bit / 64-Bit \r\n    Windows 8 32-Bit / 64-Bit"), TEXT("Info"), MB_OK);
		return 0;
	}
	//
	// Administrator permissions
	// 
	if (GetWinVer() == eWin_XP){
	}else{
		if (IsAdministrator() == TRUE){
		}else{
			AtlMessageBox(NULL, TEXT("This program need administrator privilege!\nPlease Right-Click this program and click \"run as administrator\"!"), TEXT("Info"), MB_OK);
			return 0;
		}
	}
	//
	// Adjust privilege
	// 
	EnableDebugPrivilege(TRUE);
	EnableLoadDriverPrivilege(TRUE);

	g_Dump.SetUnHandleException();
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
