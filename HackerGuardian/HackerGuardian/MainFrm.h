// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CMainFrame : 
	public CRibbonFrameWindowImpl<CMainFrame>, 
	public CMessageFilter, public CIdleHandler
{
public:
	DECLARE_FRAME_WND_CLASS(NULL, IDR_MAINFRAME)
	
	CSplitterWindow		m_vertical_splitter;
	CHorSplitterWindow	m_horizontal_splitter;

	CPaneContainer		m_pane;
	CTreeViewCtrl		m_treeview;

	//CHackerGuardianView<SZ_PROCESSINFO> m_TopListView;
	//CHackerGuardianView<SZ_PROCESSINFO>	m_BottomListView;

	CTabView			m_top_tab;
	CTabView			m_bottom_tab;

	CCommandBarCtrl m_CmdBar;
	CStatusBarCtrl	m_status;

	//TODO: Declare ribbon controls

	// Ribbon control map
	BEGIN_RIBBON_CONTROL_MAP(CMainFrame)
	END_RIBBON_CONTROL_MAP()

	virtual BOOL PreTranslateMessage(MSG* pMsg)
	{
		if(CRibbonFrameWindowImpl<CMainFrame>::PreTranslateMessage(pMsg))
			return TRUE;

		return FALSE;
	}

	virtual BOOL OnIdle()
	{
		UIUpdateToolBar();
		return FALSE;
	}

	BEGIN_UPDATE_UI_MAP(CMainFrame)
		UPDATE_ELEMENT(ID_BASIC_RELOAD_KERNEL, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TOOLBAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_STATUS_BAR, UPDUI_MENUPOPUP)
		UPDATE_ELEMENT(ID_VIEW_TREEPANE, UPDUI_MENUPOPUP)
	END_UPDATE_UI_MAP()

	BEGIN_MSG_MAP(CMainFrame)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(ID_BASIC_PROCESS, OnEnumProcess)
		COMMAND_ID_HANDLER(ID_BASIC_DRIVER, OnEnumDriver)
		COMMAND_ID_HANDLER(ID_BASIC_RELOAD_KERNEL, OnBasicReloadKernel)
		COMMAND_ID_HANDLER(ID_KERNEL_NOTIFY_ROUTINE, OnKernelEnumNotify)
		COMMAND_ID_HANDLER(ID_HOOK_SSDT, OnHookSSDT)
		COMMAND_ID_HANDLER(ID_HOOK_SHADOW_SSDT, OnHookShadowSSDT)
		COMMAND_ID_HANDLER(ID_APP_EXIT, OnFileExit)
		COMMAND_ID_HANDLER(ID_FILE_NEW, OnFileNew)
		COMMAND_ID_HANDLER(ID_VIEW_TOOLBAR, OnViewToolBar)
		COMMAND_ID_HANDLER(ID_VIEW_STATUS_BAR, OnViewStatusBar)
		COMMAND_ID_HANDLER(ID_VIEW_RIBBON, OnViewRibbon)
		COMMAND_ID_HANDLER(ID_APP_ABOUT, OnAppAbout)
		COMMAND_ID_HANDLER(ID_VIEW_TREEPANE, OnViewTreePane)
		COMMAND_ID_HANDLER(ID_PANE_CLOSE, OnTreePaneClose)
		COMMAND_ID_HANDLER(ID_TOPTAB_CLOSE, OnTopTabClose)
		COMMAND_ID_HANDLER(ID_TOPTAB_CLOSEALL, OnTopTabCloseAll)
		COMMAND_ID_HANDLER(ID_BOTTOMTAB_CLOSE, OnBottomTabClose)
		COMMAND_ID_HANDLER(ID_BOTTOMTAB_CLOSEALL, OnBottomTabCloseAll)
		COMMAND_RANGE_HANDLER(ID_WINDOW_TABFIRST, ID_WINDOW_TABLAST, OnWindowActivate)
		NOTIFY_CODE_HANDLER(TBVN_CONTEXTMENU, OnTabViewContextMenu)
		//NOTIFY_CODE_HANDLER_EX(NM_RCLICK, OnRClick)
		// RClick Menu
		// process
		//COMMAND_ID_HANDLER(ID_RM_PROCESS_REFRESH, OnRM_ProcessRefresh)
		REFLECT_NOTIFICATIONS()
		CHAIN_MSG_MAP(CRibbonFrameWindowImpl<CMainFrame>)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
//

	LRESULT OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
	{
		bool bRibbonUI = RunTimeHelper::IsRibbonUIAvailable();
		if (bRibbonUI)
			UIAddMenu(GetMenu(), true);
		else
			CMenuHandle(GetMenu()).DeleteMenu(ID_VIEW_RIBBON, MF_BYCOMMAND);

		// create command bar window
		HWND hWndCmdBar = m_CmdBar.Create(m_hWnd, rcDefault, NULL, ATL_SIMPLE_CMDBAR_PANE_STYLE);
		// attach menu
		m_CmdBar.AttachMenu(GetMenu());
		// load command bar images
		m_CmdBar.LoadImages(IDR_MAINFRAME);
		// remove old menu
		SetMenu(NULL);

		HWND hWndToolBar = CreateSimpleToolBarCtrl(m_hWnd, IDR_MAINFRAME, FALSE, ATL_SIMPLE_TOOLBAR_PANE_STYLE);

		CreateSimpleReBar(ATL_SIMPLE_REBAR_NOBORDER_STYLE);
		AddSimpleReBarBand(hWndCmdBar);
		AddSimpleReBarBand(hWndToolBar, NULL, TRUE);

		CreateSimpleStatusBar();

		m_hWndClient = m_vertical_splitter.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_horizontal_splitter.Create(m_vertical_splitter, rcDefault, NULL,
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_pane.SetPaneContainerExtendedStyle(PANECNT_NOBORDER);
		m_pane.Create(m_vertical_splitter, _T("Explorer"), WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		m_treeview.Create(m_pane, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
		m_pane.SetClient(m_treeview);

		m_top_tab.Create(m_horizontal_splitter.m_hWnd, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
			, WS_EX_CLIENTEDGE);
		m_bottom_tab.Create(m_horizontal_splitter.m_hWnd, rcDefault, NULL, 
			WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN
			, WS_EX_CLIENTEDGE);

		//m_TopListView.Create(m_horizontal_splitter, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);
		//m_BottomListView.Create(m_horizontal_splitter, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | LVS_REPORT | LVS_SHOWSELALWAYS, WS_EX_CLIENTEDGE);

		m_vertical_splitter.SetSplitterPanes(m_pane, m_horizontal_splitter);
		m_horizontal_splitter.SetSplitterPanes(m_top_tab, m_bottom_tab/*m_BottomListView*/);

		UpdateLayout();
		m_vertical_splitter.SetSplitterPosPct(25);
		m_horizontal_splitter.SetSplitterPosPct(75);

		UIAddToolBar(hWndToolBar);
		UISetCheck(ID_VIEW_TOOLBAR, 1);
		UISetCheck(ID_VIEW_STATUS_BAR, 1);
		UISetCheck(ID_VIEW_TREEPANE, 1);
		UISetCheck(ID_BASIC_RELOAD_KERNEL, 0);

		// register object for message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->AddMessageFilter(this);
		pLoop->AddIdleHandler(this);

		ShowRibbonUI(bRibbonUI);
		UISetCheck(ID_VIEW_RIBBON, bRibbonUI);

		m_status.Attach(m_hWndStatusBar);

		//CMenuHandle menuMain = m_CmdBar.GetMenu();
		//m_top_tab.SetWindowMenu(menuMain.GetSubMenu(3));
		//m_bottom_tab.SetWindowMenu(menuMain.GetSubMenu(4));
		// 
		// Initialize global instance
		// 
		CLVProcess::Init(&m_top_tab, &m_bottom_tab, &m_vertical_splitter, &m_horizontal_splitter, &m_status);
		CLVDriver::Init(&m_top_tab, &m_bottom_tab, &m_vertical_splitter, &m_horizontal_splitter, &m_status);
		CLVKernel::Init(&m_top_tab, &m_bottom_tab, &m_vertical_splitter, &m_horizontal_splitter, &m_status);
		CLVHook::Init(&m_top_tab, &m_bottom_tab, &m_vertical_splitter, &m_horizontal_splitter, &m_status);

		// Initialize Process List
		CLVProcess::GetInstance()->ListProcess();
		CLVProcess::GetInstance()->EnumProcess3();

		m_vertical_splitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
		m_horizontal_splitter.SetSinglePaneMode(SPLIT_PANE_TOP);

		//
		// adjust window's size
		// 
		RECT rc;
		this->GetClientRect(&rc);
		ClientToScreen(&rc);
		rc.right = rc.left + 960;
		rc.bottom = rc.top + 600;
		this->MoveWindow(&rc);

		return 0;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& bHandled)
	{
		// unregister message filtering and idle updates
		CMessageLoop* pLoop = _Module.GetMessageLoop();
		ATLASSERT(pLoop != NULL);
		pLoop->RemoveMessageFilter(this);
		pLoop->RemoveIdleHandler(this);

		bHandled = FALSE;
		return 1;
	}

	LRESULT OnEnumProcess(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVProcess::GetInstance()->ListProcess();

		return 0;
	}

	LRESULT OnEnumDriver(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVDriver::GetInstance()->DisplayDriver();

		return 0;
	}

	LRESULT OnKernelEnumNotify(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVKernel::GetInstance()->DisplayNotify();

		return 0;
	}

	LRESULT OnHookSSDT(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVHook::GetInstance()->DisplaySDT(R3_HOOK_SDT);

		return 0;
	}

	LRESULT OnHookShadowSSDT(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVHook::GetInstance()->DisplaySDT(R3_HOOK_SHADOW_SDT);

		return 0;
	}

	LRESULT OnFileExit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		PostMessage(WM_CLOSE);
		return 0;
	}

	LRESULT OnFileNew(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_status.SetText(0, TEXT("aaa"));
		return 0;
	}

	LRESULT OnViewToolBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_VIEW_TOOLBAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnBasicReloadKernel(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		static BOOL bVisible = TRUE;	// initially visible
		bVisible = !bVisible;
		CReBarCtrl rebar = m_hWndToolBar;
		int nBandIndex = rebar.IdToIndex(ATL_IDW_BAND_FIRST + 1);	// toolbar is 2nd added band
		rebar.ShowBand(nBandIndex, bVisible);
		UISetCheck(ID_BASIC_RELOAD_KERNEL, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewStatusBar(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bVisible = !::IsWindowVisible(m_hWndStatusBar);
		::ShowWindow(m_hWndStatusBar, bVisible ? SW_SHOWNOACTIVATE : SW_HIDE);
		UISetCheck(ID_VIEW_STATUS_BAR, bVisible);
		UpdateLayout();
		return 0;
	}

	LRESULT OnViewRibbon(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		ShowRibbonUI(!IsRibbonUI());
		UISetCheck(ID_VIEW_RIBBON, IsRibbonUI());
		return 0;
	}

	LRESULT OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return 0;
	}

	LRESULT OnViewTreePane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		bool bShow = (m_vertical_splitter.GetSinglePaneMode() != SPLIT_PANE_NONE);
		m_vertical_splitter.SetSinglePaneMode(bShow ? SPLIT_PANE_NONE : SPLIT_PANE_RIGHT);
		UISetCheck(ID_VIEW_TREEPANE, bShow);

		return 0;
	}

	LRESULT OnTreePaneClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND hWndCtl, BOOL& /*bHandled*/)
	{
		if(hWndCtl == m_pane.m_hWnd)
		{
			m_vertical_splitter.SetSinglePaneMode(SPLIT_PANE_RIGHT);
			UISetCheck(ID_VIEW_TREEPANE, 0);
		}

		return 0;
	}

	LRESULT OnTopTabClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int nActivePage = m_top_tab.GetActivePage();
		if(nActivePage != -1)
			m_top_tab.RemovePage(nActivePage);
		else
			::MessageBeep((UINT)-1);

		return 0;
	}

	LRESULT OnTopTabCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_top_tab.RemoveAllPages();

		return 0;
	}

	LRESULT OnBottomTabClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int nActivePage = m_top_tab.GetActivePage();
		if(nActivePage != -1)
			m_bottom_tab.RemovePage(nActivePage);
		else
			::MessageBeep((UINT)-1);

		return 0;
	}

	LRESULT OnBottomTabCloseAll(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		m_bottom_tab.RemoveAllPages();

		return 0;
	}

	LRESULT OnWindowActivate(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		int nPage = wID - ID_WINDOW_TABFIRST;
		m_top_tab.SetActivePage(nPage);

		return 0;
	}

	LRESULT OnTabViewContextMenu(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LPTBVCONTEXTMENUINFO lpcmi = (LPTBVCONTEXTMENUINFO)pnmh;
		CMenuHandle menuPopup;
		CMenuHandle menuMain = m_CmdBar.GetMenu();
		if (pnmh->hwndFrom == m_top_tab.m_hWnd){
			menuPopup = menuMain.GetSubMenu(4);
			int nRet = (int)m_CmdBar.TrackPopupMenu(menuPopup, 
				TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
				lpcmi->pt.x, lpcmi->pt.y);
			if(nRet == ID_TOPTAB_CLOSE)
				m_top_tab.RemovePage((int)pnmh->idFrom);
			else
				SendMessage(WM_COMMAND, MAKEWPARAM(nRet, 0));
		}else if (pnmh->hwndFrom){
			menuPopup = menuMain.GetSubMenu(5);
			int nRet = (int)m_CmdBar.TrackPopupMenu(menuPopup, 
				TPM_RETURNCMD | TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
				lpcmi->pt.x, lpcmi->pt.y);
			if(nRet == ID_BOTTOMTAB_CLOSE)
				m_bottom_tab.RemovePage((int)pnmh->idFrom);
			else
				SendMessage(WM_COMMAND, MAKEWPARAM(nRet, 0));
		}

		return 0;
	}	

// 	LRESULT OnRClick(NMHDR* phdr)
// 	{
// 		LPNMLISTVIEW pNMListView = (LPNMLISTVIEW)phdr;
// 		if(pNMListView->iItem != -1)
// 		{
// 			DWORD dwPos = GetMessagePos();
// 			CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
// 
// 			CMenuHandle menuRClick;
// 			menuRClick.LoadMenu(IDR_RCLICK_PROCESS);
// 			CMenuHandle menuPopup = menuRClick.GetSubMenu(0);
// 			menuPopup.TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
// 				point.x, 
// 				point.y, 
// 				this->m_hWnd);
// 		} 
// 		return 0;
// 	}
// 
// 	LRESULT OnRM_ProcessRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
// 	{
// 		::MessageBoxA(NULL, "OnRM_ProcessRefresh", "OnRM_ProcessRefresh", MB_OK);
// 		return 0;
// 	}
};
