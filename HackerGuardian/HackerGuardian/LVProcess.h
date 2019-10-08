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

#ifndef __LVPROCESS_H__
#define __LVPROCESS_H__

#include "stdafx.h"

#include "HGDriver.h"

#include <TlHelp32.h>

/************************************************************************/
/*                                                                      */
/************************************************************************/

typedef enum _KTHREAD_STATE {
	Initialized,
	Ready,
	Running,
	Standby,
	Terminated,
	Waiting,
	Transition,
	DeferredReady,
	GateWait
} KTHREAD_STATE;

/************************************************************************/
/* CLVProcess                                                           */
/************************************************************************/
struct CLVProcess
{
private:
	// from MainFrm
	static CTabView*						m_top_tab;
	static CTabView*						m_bottom_tab;
	static CSplitterWindow*					m_vertical_splitter;
	static CHorSplitterWindow*				m_horizontal_splitter;
	static CStatusBarCtrl*					m_pStatus;

	CHackerGuardianView<SZ_PROCESSINFO> m_LVProcess;
	CImageList								m_ProcessIcon;
	HICON									m_hProcessDefaultIcon;
	CHackerGuardianView<SZ_MODULEINFO> m_LVModule;
	CHackerGuardianView<SZ_THREADINFO> m_LVThread;
	CHackerGuardianView<HG_MEMORY_INFO> m_LVMemory;
	CHackerGuardianView<HG_HANDLE_INFO> m_LVHandle;

	std::vector<SZ_PROCESSINFO>				m_vProcessList;
	std::vector<SZ_PROCESSINFO>	m_vProcessList_ZwQuerySystemInformation;
	std::vector<SZ_PROCESSINFO> m_vProcessList_PspCidTable;
	std::vector<SZ_PROCESSINFO> m_vProcessList_WalkerThreadList;

private:
	CLVProcess(){}
	//
	// To forbid copy and assignment operation, because of Singleton Pattern.
	//
	CLVProcess(const CLVProcess& RightSides){

	}

	CLVProcess& operator=(const CLVProcess& RightSides){
		if (this == &RightSides){
			return *this;
		}
	}

private:
	BOOL IsAccessProcess(DWORD dwPid)
	{
		BOOL bRtn = TRUE;

		HANDLE hProcess = OpenProcess(PROCESS_TERMINATE | PROCESS_VM_OPERATION, TRUE, dwPid);
		if (hProcess){
			CloseHandle(hProcess);
		}else{
			bRtn = FALSE;
		}

		return bRtn;
	}

	CString GetStateString(SIZE_T State)
	{
		switch (State){
		case Initialized:
			return TEXT("Initialized");
		case Ready:
			return TEXT("Ready");
		case Running:
			return TEXT("Running");
		case Standby:
			return TEXT("Standby");
		case Terminated:
			return TEXT("Terminated");
		case Waiting:
			return TEXT("Waiting");
		case Transition:
			return TEXT("Transition");
		case DeferredReady:
			return TEXT("DeferredReady");
		case GateWait:
			return TEXT("GateWait");
		}

		return TEXT("");
	}

	CString GetMemoryStateString(SIZE_T State)
	{
		switch (State){
		case MEM_COMMIT:
			return TEXT("Commit");
		case MEM_FREE:
			return TEXT("Free");
		case MEM_RESERVE:
			return TEXT("Reserve");
		case MEM_DECOMMIT:
			return TEXT("Decommit");
		case MEM_RELEASE:
			return TEXT("Release");
		}

		return TEXT("");
	}

	CString GetMemoryTypeString(SIZE_T State)
	{
		switch (State){
		case MEM_PRIVATE:
			return TEXT("Private");
		case MEM_MAPPED:
			return TEXT("Map");
		case MEM_IMAGE:
			return TEXT("Image");
		}

		return TEXT("");
	}

public:
	CString GetMemoryProtectString(SIZE_T Protect)
	{
		std::map<SIZE_T, CString> vMemoryProtect;
		vMemoryProtect[PAGE_NOACCESS] = L"No Access";
		vMemoryProtect[PAGE_READONLY] = L"Read";
		vMemoryProtect[PAGE_READWRITE] = L"ReadWrite";
		vMemoryProtect[PAGE_WRITECOPY] = L"WriteCopy";
		vMemoryProtect[PAGE_EXECUTE] = L"Execute";
		vMemoryProtect[PAGE_EXECUTE_READ] = L"ReadExecute";
		vMemoryProtect[PAGE_EXECUTE_READWRITE] = L"ReadWriteExecute";
		vMemoryProtect[PAGE_EXECUTE_WRITECOPY] = L"WriteCopyExecute";
		vMemoryProtect[PAGE_GUARD] = L"Guard";
		vMemoryProtect[PAGE_NOCACHE] = L"No Cache";
		vMemoryProtect[PAGE_WRITECOMBINE] = L"WriteCombine";

		CString strRtn;
		BOOL bFirst = TRUE;

		for (std::map<SIZE_T, CString>::iterator iter = vMemoryProtect.begin();
			iter != vMemoryProtect.end();
			iter ++)
		{
			if (iter->first & Protect)
			{
				if (bFirst == TRUE)
				{
					strRtn += iter->second;
					bFirst = FALSE;
				}
				else
				{
					strRtn += L" & ";
					strRtn += iter->second;
				}
			}
		}

		return strRtn;
	}

public:
	static void Init(
		CTabView* top_tab, 
		CTabView* bottom_tab,
		CSplitterWindow* vertical_splitter,
		CHorSplitterWindow*	horizontal_splitter,
		CStatusBarCtrl* status)
	{
		ATLASSERT(top_tab != NULL);
		ATLASSERT(bottom_tab != NULL);
		ATLASSERT(vertical_splitter != NULL);
		ATLASSERT(horizontal_splitter != NULL);
		ATLASSERT(status != NULL);

		m_top_tab = top_tab;
		m_bottom_tab = bottom_tab;
		m_vertical_splitter = vertical_splitter;
		m_horizontal_splitter = horizontal_splitter;
		m_pStatus = status;
	}

	static CLVProcess* GetInstance(){
		static CLVProcess inst;
		return &inst;
	}

	HRESULT ListProcess(){
		HRESULT hr = S_OK;

		// Check m_top_tab
		if (m_top_tab == NULL || m_bottom_tab == NULL){
			return S_FALSE;
		}

		// 
// 		for (int i = 0; i < m_top_tab->GetPageCount(); i++){
// 			if (_tcscmp(ptszTabProcess[eEnglish], m_top_tab->GetPageTitle(i)) == 0){
// 				m_top_tab->RemovePage(i);
// 				break;
// 			}
// 		}
		
		// Create list view
		//m_LVProcess = new CHackerGuardianView<SZ_PROCESSINFO>;
		m_LVProcess.SetTabId(eProcess);

		m_LVProcess.Create(m_top_tab->m_hWnd, CWindow::rcDefault, NULL, 
			//WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
			WS_CHILD | LVS_REPORT | LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP,
			0);

		//
		m_ProcessIcon.Create(16, 16, ILC_COLOR16 | ILC_MASK, 2, 2);
		m_ProcessIcon.SetBkColor(RGB(255, 255, 255));
		m_LVProcess.SetImageList(m_ProcessIcon.m_hImageList, LVSIL_SMALL);
		//
		m_LVProcess.SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

//		LONG lStyle;
//		lStyle = GetWindowLong(m_pView->m_hWnd, GWL_STYLE);
//		lStyle &= ~LVS_TYPEMASK;
//		lStyle |= LVS_REPORT; 
//		::SetWindowLong(m_pView->m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_LVProcess.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
//		dwStyle |= LVS_EX_CHECKBOXES;
		m_LVProcess.SetExtendedListViewStyle(dwStyle);

		//
		m_hProcessDefaultIcon = reinterpret_cast<HICON>(::LoadImage(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_PROCESS_DEFAULT), IMAGE_ICON, 48, 48, LR_DEFAULTCOLOR));

		//
		m_top_tab->AddPage(m_LVProcess.m_hWnd, ptszTabProcess[eEnglish]);

		// Clear
		m_LVProcess.DeleteAllItems();

		m_LVProcess.InsertColumn(0, ptszTabProcessColumn[eEnglish][0], LVCFMT_LEFT, 150);//插入列
		m_LVProcess.InsertColumn(1, ptszTabProcessColumn[eEnglish][1], LVCFMT_LEFT, 50);
		m_LVProcess.InsertColumn(2, ptszTabProcessColumn[eEnglish][2], LVCFMT_LEFT, 50);
		m_LVProcess.InsertColumn(3, ptszTabProcessColumn[eEnglish][3], LVCFMT_LEFT, 260);
#if defined(_WIN64)
		m_LVProcess.InsertColumn(4, ptszTabProcessColumn[eEnglish][4], LVCFMT_LEFT, 150);
#else
		m_LVProcess.InsertColumn(4, ptszTabProcessColumn[eEnglish][4], LVCFMT_LEFT, 88);
#endif
		m_LVProcess.InsertColumn(5, ptszTabProcessColumn[eEnglish][5], LVCFMT_CENTER, 60);
		m_LVProcess.InsertColumn(6, ptszTabProcessColumn[eEnglish][6], LVCFMT_LEFT, 200);

		//
		// Traverse all processes.
		//
 		EnumProcess0();

		return hr;
	}

	HRESULT InsertProcessItem(PSZ_PROCESSINFO pi, int nIcon){
		HRESULT hr = S_OK;
		int nRow = 0;
		if (pi == NULL){
			return S_FALSE;
		}
		
		nRow = m_LVProcess.InsertItem(m_LVProcess.GetItemCount(), pi->m_ptszName, nIcon);
		m_LVProcess.SetItemText(nRow, 1, size_tToCString(pi->m_nPid));
		if (pi->m_nParentPid == 0){
			m_LVProcess.SetItemText(nRow, 2, L"");
		}else{
			m_LVProcess.SetItemText(nRow, 2, 
				size_tToCString(pi->m_nParentPid));
		}
		m_LVProcess.SetItemText(nRow, 3, pi->m_ptszPath);
		m_LVProcess.SetItemText(nRow, 4, size_tToHexCString(pi->m_nEProcessAddress));
		m_LVProcess.SetItemText(nRow, 5, pi->m_pwszAccess);
		m_LVProcess.SetItemText(nRow, 6, pi->m_pwszCompany);
		m_LVProcess.SetItemData(nRow, (DWORD_PTR)pi);

		return hr;
	}

	void EnumProcess0()
	{
		PITP pItp = NULL;
		PSZ_PROCESSINFO pPITmp = NULL;
		CString strPath;
		WCHAR pwszAccess[8] = {0};
		CString strCompany;
		COLORREF rgb = RGB(255, 0, 0);
		std::vector<DWORD> vPid = EnumProcess3();
		CHGDriver::GetInstance()->EnumerateProcess(&pItp);
		m_ProcessIcon.RemoveAll();
		
		if (pItp != NULL){
			SzLog(TEXT("pItp->data.process.nRealCount: %d"), pItp->process.nRealCount);
			CString strStatus;
			strStatus.Format(TEXT("Process count: %d, "), pItp->process.nRealCount);
			m_pStatus->SetText(0, strStatus);
			for (ULONG i = 0; i < pItp->process.nRealCount; i++){
				CString strName = pItp->process.pProcess[i].pwszPath;
				strName = strName.Right(strName.GetLength() - strName.ReverseFind(TEXT('\\')) - 1);
				strPath = FormatDosPath(pItp->process.pProcess[i].pwszPath).GetBuffer(SZ_PATH_LEN);
				//
				// extract process icon
				// 
				SHFILEINFO si = {0};
				DWORD_PTR nRtn = 0;
				nRtn = SHGetFileInfoW(strPath.GetBuffer(strPath.GetLength()), 0, &si, sizeof(SHFILEINFO), SHGFI_ICON);
				nRtn ? m_ProcessIcon.AddIcon(si.hIcon) : m_ProcessIcon.AddIcon(m_hProcessDefaultIcon);
				//
				// Can the process be accessed?
				// 
				if (IsAccessProcess((DWORD)pItp->process.pProcess[i].nPid)){
					wcscpy_s(pwszAccess, L"-");
				}else{
					wcscpy_s(pwszAccess, L"Denial");
				}
				//
				// company
				// 
				strCompany = GetFileCompany(strPath.GetBuffer(SZ_PATH_LEN));
				//
				// if the process is hidden, display by red.
				// 
				if (vPid.end() != find(vPid.begin(), vPid.end(), (DWORD)pItp->process.pProcess[i].nPid)){
					if (strCompany.Find(TEXT("Microsoft")) >= 0){
						rgb = RGB(0, 0, 0);
					}else{
						rgb = RGB(0, 0, 255);
					}
				}else{
					rgb = RGB(255, 0, 0);
				}
				pPITmp = new SZ_PROCESSINFO(strName.GetBuffer(strName.GetLength()),
											pItp->process.pProcess[i].nPid,
											pItp->process.pProcess[i].nParentPid,
											strPath.GetBuffer(SZ_PATH_LEN),
											pItp->process.pProcess[i].pEProcess,
											pwszAccess, 
											strCompany.GetBuffer(SZ_PATH_LEN), 
											rgb);
				m_vProcessList.push_back(*pPITmp);
				InsertProcessItem(pPITmp, i);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("ERROR: pItp == NULL");
		}
	}

	void RefreshProcess(/*CHackerGuardianView<SZ_PROCESSINFO>* pView*/)
	{
		//ATLASSERT(pView != NULL);
		//m_pView = pView;
		m_LVProcess.DeleteAllItems();
		EnumProcess0();
	}

	void BottomTabRemoveAllPage()
	{
		m_bottom_tab->RemoveAllPages();
	}

	void DisplayProcessModule(SIZE_T nProcessId, SIZE_T pEProcess){
		m_horizontal_splitter->SetSinglePaneMode(SPLIT_PANE_NONE);

		//m_LVProcessModule = new CHackerGuardianView<SZ_MODULEINFO>;
		m_LVModule.SetTabId(eProcessModule);

		m_LVModule.Create(	m_bottom_tab->m_hWnd, 
								CWindow::rcDefault, 
								NULL, 
								WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
								0);

		m_LVModule.SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

		LONG lStyle;
		lStyle = GetWindowLong(m_LVModule.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT; 
		::SetWindowLong(m_LVModule.m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_LVModule.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
//		dwStyle |= LVS_EX_CHECKBOXES;
		m_LVModule.SetExtendedListViewStyle(dwStyle);
		m_bottom_tab->AddPage(m_LVModule.m_hWnd, ptszTabProcessModule[eEnglish]);

		// Clear
		m_LVModule.DeleteAllItems();

		m_LVModule.InsertColumn(0, ptszTabProcessModuleColumn[eEnglish][0], LVCFMT_LEFT, 150);//插入列
		m_LVModule.InsertColumn(1, ptszTabProcessModuleColumn[eEnglish][1], LVCFMT_LEFT, 260);
#if defined(_WIN64)
		m_LVModule.InsertColumn(2, ptszTabProcessModuleColumn[eEnglish][2], LVCFMT_LEFT, 150);
		m_LVModule.InsertColumn(3, ptszTabProcessModuleColumn[eEnglish][3], LVCFMT_LEFT, 150);
#else
		m_LVModule.InsertColumn(2, ptszTabProcessModuleColumn[eEnglish][2], LVCFMT_LEFT, 75);
		m_LVModule.InsertColumn(3, ptszTabProcessModuleColumn[eEnglish][3], LVCFMT_LEFT, 75);
#endif
		m_LVModule.InsertColumn(4, ptszTabProcessModuleColumn[eEnglish][4], LVCFMT_LEFT, 200);

		EnumProcessModule0(nProcessId, pEProcess);

		m_top_tab->SetActivePage(m_top_tab->GetActivePage());
	}

	void EnumProcessModule0(SIZE_T nProcessId, SIZE_T pEProcess)
	{
		SIZE_T i = 0;
		PITP pItp = NULL;
		PSZ_MODULEINFO mi = NULL;
		CString strPath;
		CString strCompany;
		COLORREF rgb = RGB(0, 0, 0);
		CHGDriver::GetInstance()->EnumerateProcessModule(nProcessId, pEProcess, &pItp);

		WCHAR NewTitle[64] = {0};
		wcscpy_s(NewTitle, ptszTabProcessThread[eEnglish]);
		wsprintf(NewTitle, L"%s [%d]", ptszTabProcessModule[eEnglish], pItp->out_process_module.nCount);
		m_bottom_tab->SetPageTitle(m_bottom_tab->GetActivePage(), NewTitle);

		if (pItp != NULL){
			for (i = 0; i < pItp->out_process_module.nCount; i++){
				CString strName = pItp->out_process_module.pModule[i].Path;
				strName = strName.Right(strName.GetLength() - strName.ReverseFind(TEXT('\\')) - 1);
				strPath = FormatDosPath(pItp->out_process_module.pModule[i].Path).GetBuffer(SZ_PATH_LEN);
				//
				// company
				// 
				strCompany = GetFileCompany(strPath.GetBuffer(SZ_PATH_LEN));
				//
				// if the process is hidden, display by red.
				// 
// 				if (vPid.end() != find(vPid.begin(), vPid.end(), (DWORD)pItp->process.pProcess[i].nPid)){
				if (strCompany.Find(TEXT("Microsoft")) >= 0){
					rgb = RGB(0, 0, 0);
				}else{
					rgb = RGB(0, 0, 255);
				}
// 				}else{
// 					rgb = RGB(255, 0, 0);
// 				}
				mi = new SZ_MODULEINFO(strName.GetBuffer(strName.GetLength()),
										strPath.GetBuffer(strPath.GetLength()),
										pItp->out_process_module.pModule[i].Base,
										pItp->out_process_module.pModule[i].Size,
										strCompany.GetBuffer(SZ_PATH_LEN), 
										rgb);
				InsertModuleItem(mi);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVProcess.h].[EnumProcessModule0]: pItp == NULL");
		}
	}

	void InsertModuleItem(PSZ_MODULEINFO mi)
	{
		int nRow = 0;
		if (mi == NULL){
			return;
		}

		nRow = m_LVModule.InsertItem(m_LVModule.GetItemCount(), mi->m_pwszName);
		m_LVModule.SetItemText(nRow, 1, mi->m_pwszPath);
		m_LVModule.SetItemText(nRow, 2, size_tToHexCString(mi->m_nImageBase));
		m_LVModule.SetItemText(nRow, 3, size_tToHexCString(mi->m_nImageSize));
		m_LVModule.SetItemText(nRow, 4,	mi->m_pwszCompany);
		m_LVModule.SetItemData(nRow, (DWORD_PTR)mi);
	}

	void DisplayProcessThread(SIZE_T nProcessId, SIZE_T pEProcess){
		//m_LVThread = new CHackerGuardianView<SZ_THREADINFO>;
		m_LVThread.SetTabId(eProcessThread);

		m_LVThread.Create(	m_bottom_tab->m_hWnd, 
								CWindow::rcDefault, 
								NULL, 
								WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 
								0);

		m_LVThread.SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

		LONG lStyle;
		lStyle = GetWindowLong(m_LVThread.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT; 
		::SetWindowLong(m_LVThread.m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_LVThread.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
		//		dwStyle |= LVS_EX_CHECKBOXES;
		m_LVThread.SetExtendedListViewStyle(dwStyle);
		m_bottom_tab->AddPage(m_LVThread.m_hWnd, ptszTabProcessThread[eEnglish]);

		// Clear
		m_LVThread.DeleteAllItems();

		m_LVThread.InsertColumn(0, ptszTabProcessThreadColumn[eEnglish][0], LVCFMT_LEFT, 50);//插入列
#if defined(_WIN64)
		m_LVThread.InsertColumn(1, ptszTabProcessThreadColumn[eEnglish][1], LVCFMT_LEFT, 150);
		m_LVThread.InsertColumn(2, ptszTabProcessThreadColumn[eEnglish][2], LVCFMT_LEFT, 150);
#else
		m_LVThread.InsertColumn(1, ptszTabProcessThreadColumn[eEnglish][1], LVCFMT_LEFT, 100);
		m_LVThread.InsertColumn(2, ptszTabProcessThreadColumn[eEnglish][2], LVCFMT_LEFT, 100);
#endif
		m_LVThread.InsertColumn(3, ptszTabProcessThreadColumn[eEnglish][3], LVCFMT_LEFT, 70);
#if defined(_WIN64)
		m_LVThread.InsertColumn(4, ptszTabProcessThreadColumn[eEnglish][4], LVCFMT_LEFT, 150);
		m_LVThread.InsertColumn(5, ptszTabProcessThreadColumn[eEnglish][5], LVCFMT_LEFT, 150);
#else
		m_LVThread.InsertColumn(4, ptszTabProcessThreadColumn[eEnglish][4], LVCFMT_LEFT, 120);
		m_LVThread.InsertColumn(5, ptszTabProcessThreadColumn[eEnglish][5], LVCFMT_LEFT, 100);
#endif
		m_LVThread.InsertColumn(6, ptszTabProcessThreadColumn[eEnglish][6], LVCFMT_LEFT, 150);
		m_LVThread.InsertColumn(7, ptszTabProcessThreadColumn[eEnglish][7], LVCFMT_LEFT, 90);
		m_LVThread.InsertColumn(8, ptszTabProcessThreadColumn[eEnglish][8], LVCFMT_LEFT, 100);

		EnumProcessThread0(nProcessId, pEProcess);

		m_top_tab->SetActivePage(m_top_tab->GetActivePage());
	}

	void EnumProcessThread0(SIZE_T nProcessId, SIZE_T pEProcess)
	{
		SzLogA("[LVProcess.h].[EnumProcessThread0]: Enter! nProcessId = 0x%p, pEProcess = 0x%p", nProcessId, pEProcess);
		SIZE_T i = 0;
		PITP pItp = NULL;
		PSZ_THREADINFO ti = NULL;
		CString strPath;
		CString strCompany;
		COLORREF rgb = RGB(0, 0, 0);
		CHGDriver::GetInstance()->EnumerateProcessThread(nProcessId, pEProcess, &pItp);

		WCHAR NewTitle[64] = {0};
		wcscpy_s(NewTitle, ptszTabProcessThread[eEnglish]);
		wsprintf(NewTitle, L"%s [%d]", ptszTabProcessThread[eEnglish], pItp->out_process_thread.nCount);
		m_bottom_tab->SetPageTitle(m_bottom_tab->GetActivePage(), NewTitle);

		if (pItp != NULL){
			SzLogA("[LVProcess.h].[EnumProcessThread0]: pItp->out_process_thread.nCount = %d", pItp->out_process_thread.nCount);
			for (i = 0; i < pItp->out_process_thread.nCount; i++){
				ti = new SZ_THREADINFO(	pItp->out_process_thread.pThread[i].Tid,
										pItp->out_process_thread.pThread[i].pEThread,
										pItp->out_process_thread.pThread[i].Teb,
										pItp->out_process_thread.pThread[i].Priority,
										pItp->out_process_thread.pThread[i].Win32StartAddress, 
										pItp->out_process_thread.pThread[i].StartAddress,
										GetThreadModule(pItp->out_process_thread.pThread[i].Win32StartAddress, pItp->out_process_thread.pThread[i].StartAddress).GetBuffer(MAX_PATH),
										pItp->out_process_thread.pThread[i].SwitchCount,
										pItp->out_process_thread.pThread[i].State);
				InsertThreadItem(ti);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVProcess.h].[EnumProcessThread0]: pItp == NULL");
		}
	}

	void DisplayProcessMemory(SIZE_T nProcessId, SIZE_T pEProcess){
		//m_LVMemory = new CHackerGuardianView<HG_MEMORY_INFO>;
		m_LVMemory.SetTabId(eProcessMemory);

		m_LVMemory.Create(m_bottom_tab->m_hWnd, 	CWindow::rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

		m_LVMemory.SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

		LONG lStyle;
		lStyle = GetWindowLong(m_LVMemory.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT; 
		::SetWindowLong(m_LVMemory.m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_LVMemory.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
		//		dwStyle |= LVS_EX_CHECKBOXES;
		m_LVMemory.SetExtendedListViewStyle(dwStyle);
		m_bottom_tab->AddPage(m_LVMemory.m_hWnd, ptszTabProcessMemory[eEnglish]);

		// Clear
		m_LVMemory.DeleteAllItems();

#if defined(_WIN64)
		m_LVMemory.InsertColumn(0, ptszTabProcessMemoryColumn[eEnglish][0], LVCFMT_LEFT, 150);
		m_LVMemory.InsertColumn(1, ptszTabProcessMemoryColumn[eEnglish][1], LVCFMT_LEFT, 150);
#else
		m_LVMemory.InsertColumn(0, ptszTabProcessMemoryColumn[eEnglish][0], LVCFMT_LEFT, 100);
		m_LVMemory.InsertColumn(1, ptszTabProcessMemoryColumn[eEnglish][1], LVCFMT_LEFT, 100);
#endif
		m_LVMemory.InsertColumn(2, ptszTabProcessMemoryColumn[eEnglish][2], LVCFMT_LEFT, 160);
		m_LVMemory.InsertColumn(3, ptszTabProcessMemoryColumn[eEnglish][3], LVCFMT_LEFT, 60);
		m_LVMemory.InsertColumn(4, ptszTabProcessMemoryColumn[eEnglish][4], LVCFMT_LEFT, 60);
		m_LVMemory.InsertColumn(5, ptszTabProcessMemoryColumn[eEnglish][5], LVCFMT_LEFT, 150);

		EnumProcessMemory(nProcessId, pEProcess);

		m_top_tab->SetActivePage(m_top_tab->GetActivePage());
	}

	void InsertMemoryItem(PHG_MEMORY_INFO arg0)
	{
		int nRow = 0;
		if (arg0 == NULL){
			return;
		}

		nRow = m_LVMemory.InsertItem(m_LVMemory.GetItemCount(), size_tToHexCString(arg0->m_nBase));
		m_LVMemory.SetItemText(nRow, 1, size_tToHexCString(arg0->m_nSize));
		m_LVMemory.SetItemText(nRow, 2, GetMemoryProtectString(arg0->m_nProtect));
		m_LVMemory.SetItemText(nRow, 3, GetMemoryStateString(arg0->m_nState));
		m_LVMemory.SetItemText(nRow, 4,	GetMemoryTypeString(arg0->m_nType));
		m_LVMemory.SetItemText(nRow, 5,	arg0->m_szModuleNameW);
		m_LVMemory.SetItemData(nRow, (DWORD_PTR)arg0);
	}

	CString GetMemoryModule(SIZE_T Base)
	{
		int i = 0;
		CString strRtn;
		for (i = 0; i < m_LVModule.GetItemCount(); i++)
		{
			PSZ_MODULEINFO mi = (PSZ_MODULEINFO)m_LVModule.GetItemData((int)(i));
			if (Base >= mi->m_nImageBase && Base <= mi->m_nImageBase + mi->m_nImageSize)
			{
				strRtn = mi->m_pwszName;
				return strRtn;
			}
		}
		return strRtn;
	}

	void EnumProcessMemory(SIZE_T nProcessId, SIZE_T pEProcess)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SIZE_T len = 0;
		len = VirtualQueryEx(GetCurrentProcess(), (LPCVOID)0, &mbi, sizeof(mbi));
		if (len == 0)
		{
			SzLogA("Error code = %p", GetLastError());
		}
		else
		{
			SzLogA("----%p, %p", mbi.Protect, mbi.RegionSize);
		}
		SzLogA("%s(%d): Enter! nProcessId = 0x%p, pEProcess = 0x%p", __FILE__, __LINE__, nProcessId, pEProcess);
		SIZE_T i = 0;
		PITP pItp = NULL;
		PHG_MEMORY_INFO arg0 = NULL;
		CString strPath;
		CString strCompany;
		COLORREF rgb = RGB(0, 0, 0);
		CHGDriver::GetInstance()->EnumerateProcessMemory(nProcessId, pEProcess, &pItp);

		WCHAR NewTitle[64] = {0};
		wcscpy_s(NewTitle, ptszTabProcessMemory[eEnglish]);
		wsprintf(NewTitle, L"%s [%d]", ptszTabProcessMemory[eEnglish], pItp->out_process_memory.nCount);
		m_bottom_tab->SetPageTitle(m_bottom_tab->GetActivePage(), NewTitle);

		if (pItp != NULL){
			SzLogA("%s(%d): pItp->out_process_memory.nCount = %d", __FILE__, __LINE__, pItp->out_process_memory.nCount);
			for (i = 0; i < pItp->out_process_memory.nCount; i++){
				arg0 = new HG_MEMORY_INFO(
					pItp->out_process_memory.MemoryBlocks[i].Base,
					pItp->out_process_memory.MemoryBlocks[i].Size,
					pItp->out_process_memory.MemoryBlocks[i].Protect,
					pItp->out_process_memory.MemoryBlocks[i].State,
					pItp->out_process_memory.MemoryBlocks[i].Type,
					pEProcess,
					nProcessId,
					GetMemoryModule(pItp->out_process_memory.MemoryBlocks[i].Base).GetBuffer(SZ_PATH_LEN * sizeof(WCHAR)));
				InsertMemoryItem(arg0);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVProcess.h].[EnumProcessThread0]: pItp == NULL");
		}
	}

	void InsertThreadItem(PSZ_THREADINFO ti)
	{
		int nRow = 0;
		if (ti == NULL){
			return;
		}

		nRow = m_LVThread.InsertItem(m_LVThread.GetItemCount(), size_tToCString(ti->m_nTid));
		m_LVThread.SetItemText(nRow, 1, size_tToHexCString(ti->m_nEThread));
		m_LVThread.SetItemText(nRow, 2, size_tToHexCString(ti->m_nTeb));
		m_LVThread.SetItemText(nRow, 3, size_tToCString(ti->m_nPriority));
		m_LVThread.SetItemText(nRow, 4,	size_tToHexCString(ti->m_nWin32StartAddress));
		m_LVThread.SetItemText(nRow, 5,	size_tToHexCString(ti->m_nStartAddress));
		m_LVThread.SetItemText(nRow, 6,	ti->m_pwszModuleName);
		m_LVThread.SetItemText(nRow, 7,	size_tToCString(ti->m_nSwitchCount));
		m_LVThread.SetItemText(nRow, 8,	GetStateString(ti->m_nState));
		m_LVThread.SetItemData(nRow, (DWORD_PTR)ti);
	}

	CString GetThreadModule(SIZE_T Win32StartAddress, SIZE_T StartAddress)
	{
		int i = 0;
		CString strRtn;
		for (i = 0; i < m_LVModule.GetItemCount(); i++)
		{
			PSZ_MODULEINFO mi = (PSZ_MODULEINFO)m_LVModule.GetItemData((int)(i));
			if (Win32StartAddress >= mi->m_nImageBase && Win32StartAddress <= mi->m_nImageBase + mi->m_nImageSize)
			{
				strRtn = mi->m_pwszName;
				return strRtn;
			}
		}

		for (i = 0; i < m_LVModule.GetItemCount(); i++)
		{
			PSZ_MODULEINFO mi = (PSZ_MODULEINFO)m_LVModule.GetItemData((int)(i));
			if (StartAddress >= mi->m_nImageBase && StartAddress <= mi->m_nImageBase + mi->m_nImageSize)
			{
				strRtn = mi->m_pwszName;
				return strRtn;
			}
		}
		return strRtn;
	}

	/*
	 * Description:
	 *	Traverse process list.
	 *	
	 * NOTE:
	 *	suffix 0 stands for kernel mode.
	 *	suffix 3 stands for user mode.
	 *	
	 */
	std::vector<DWORD> EnumProcess3()
	{
		std::vector<DWORD> vPid;
		PROCESSENTRY32 pe32;
		HANDLE hProcessSnap = NULL;
		BOOL bNextProcess = FALSE;

		pe32.dwSize = sizeof(PROCESSENTRY32);
		hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

		if (hProcessSnap == INVALID_HANDLE_VALUE)
		{
			SzLogA("[LVProcess.h].[EnumProcess3]: Invoke CreateToolhelp32Snapshot unsuccessfully!");
			return vPid;
		}

		bNextProcess = ::Process32First(hProcessSnap, &pe32);
		for (;bNextProcess;)
		{
			vPid.push_back(pe32.th32ProcessID);
			bNextProcess = ::Process32Next(hProcessSnap, &pe32);
		}

		::CloseHandle(hProcessSnap);
		return vPid;
	}

	void DisplayProcessHandle(SIZE_T nProcessId, SIZE_T pEProcess){
		//m_LVHandle = new CHackerGuardianView<HG_HANDLE_INFO>;
		m_LVHandle.SetTabId(eProcessHandle);

		m_LVHandle.Create(m_bottom_tab->m_hWnd, CWindow::rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, 0);

		m_LVHandle.SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

		LONG lStyle;
		lStyle = GetWindowLong(m_LVHandle.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT; 
		::SetWindowLong(m_LVHandle.m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_LVHandle.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
		//		dwStyle |= LVS_EX_CHECKBOXES;
		m_LVHandle.SetExtendedListViewStyle(dwStyle);
		m_bottom_tab->AddPage(m_LVHandle.m_hWnd, ptszTabProcessHandle[eEnglish]);

		// Clear
		m_LVHandle.DeleteAllItems();

		m_LVHandle.InsertColumn(0, ptszTabProcessHandleColumn[eEnglish][0], LVCFMT_LEFT, 100);
		m_LVHandle.InsertColumn(1, ptszTabProcessHandleColumn[eEnglish][1], LVCFMT_LEFT, 420);
		m_LVHandle.InsertColumn(2, ptszTabProcessHandleColumn[eEnglish][2], LVCFMT_LEFT, 100);
#if defined(_WIN64)
		m_LVHandle.InsertColumn(3, ptszTabProcessHandleColumn[eEnglish][3], LVCFMT_LEFT, 150);
#else
		m_LVHandle.InsertColumn(3, ptszTabProcessHandleColumn[eEnglish][3], LVCFMT_LEFT, 100);
#endif
		m_LVHandle.InsertColumn(4, ptszTabProcessHandleColumn[eEnglish][4], LVCFMT_LEFT, 0);	// not use
		m_LVHandle.InsertColumn(5, ptszTabProcessHandleColumn[eEnglish][5], LVCFMT_LEFT, 80);	//

		EnumProcessHandle(nProcessId, pEProcess);

		m_top_tab->SetActivePage(m_top_tab->GetActivePage());
	}

	void EnumProcessHandle(SIZE_T nProcessId, SIZE_T pEProcess)
	{
		MEMORY_BASIC_INFORMATION mbi = {0};
		SzLogA("%s(%d): Enter! nProcessId = 0x%p, pEProcess = 0x%p", __FILE__, __LINE__, nProcessId, pEProcess);
		SIZE_T i = 0;
		PITP pItp = NULL;
		PHG_HANDLE_INFO arg0 = NULL;
		CString strPath;
		CString strCompany;
		COLORREF rgb = RGB(0, 0, 0);
		CHGDriver::GetInstance()->EnumerateProcessHandle(nProcessId, pEProcess, &pItp);

		WCHAR NewTitle[64] = {0};
		wcscpy_s(NewTitle, ptszTabProcessHandle[eEnglish]);
		wsprintf(NewTitle, L"%s [%d]", ptszTabProcessHandle[eEnglish], pItp->get_process_handles.nCount);
		m_bottom_tab->SetPageTitle(m_bottom_tab->GetActivePage(), NewTitle);

		if (pItp != NULL){
			SzLogA("%s(%d): pItp->get_process_handles.nCount = %d", __FILE__, __LINE__, pItp->get_process_handles.nCount);
			for (i = 0; i < pItp->get_process_handles.nCount; i++){
				arg0 = new HG_HANDLE_INFO(
					pItp->get_process_handles.HandleData[i].ObjectName,
					pItp->get_process_handles.HandleData[i].HandleName,
					pItp->get_process_handles.HandleData[i].Handle,
					pItp->get_process_handles.HandleData[i].Object,
					0,
					pItp->get_process_handles.HandleData[i].ReferenceCount,
					pEProcess,
					nProcessId);
				InsertHandleItem(arg0);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVProcess.h].[EnumProcessThread0]: pItp == NULL");
		}
	}

	void InsertHandleItem(PHG_HANDLE_INFO arg0)
	{
		int nRow = 0;
		if (arg0 == NULL){
			return;
		}

		nRow = m_LVHandle.InsertItem(m_LVHandle.GetItemCount(), arg0->m_strHandleType);
		m_LVHandle.SetItemText(nRow, 1, arg0->m_strHandleName);
		CString strTmp;
		strTmp.Format(TEXT("0x%08X"), arg0->m_nHandle);
		m_LVHandle.SetItemText(nRow, 2, strTmp);
		m_LVHandle.SetItemText(nRow, 3, size_tToHexCString(arg0->m_nHandleObject));
		m_LVHandle.SetItemText(nRow, 4,	L"");
		m_LVHandle.SetItemText(nRow, 5,	size_tToCString(arg0->m_nReferenceCount));
		m_LVHandle.SetItemData(nRow, (DWORD_PTR)arg0);
	}

	void RefreshProcessHandles(SIZE_T nProcessId, SIZE_T pEProcess)
	{
		m_LVHandle.DeleteAllItems();
		EnumProcessHandle(nProcessId, pEProcess);
	}

};

#endif
