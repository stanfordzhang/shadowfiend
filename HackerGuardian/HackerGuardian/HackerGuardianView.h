// HackerGuardianView.h : interface of the CHackerGuardianView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once
#include "hguidef.h"
#include "resource.h"

#define VIEW_STYLES (LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SHAREIMAGELISTS|LVS_AUTOARRANGE)
#define VIEW_EX_STYLES (WS_EX_CLIENTEDGE)

#define	Ascending	TRUE
#define Descending	FALSE

template <typename T>
class CHackerGuardianView : 
	public CWindowImpl<CHackerGuardianView<T>, CListViewCtrl, CWinTraitsOR<VIEW_STYLES,VIEW_EX_STYLES> >,
	public CCustomDraw<CHackerGuardianView<T> >
{
private:
	BOOL		m_bSortord;
	int			m_nColumn;
	TABID		m_eTabId;
	CTabView*	m_top_tab;
	CTabView*	m_bottom_tab;
	CSplitterWindow*				m_vertical_splitter;
	CHorSplitterWindow*				m_horizontal_splitter;

public:
	DECLARE_WND_SUPERCLASS(NULL, CListViewCtrl::GetWndClassName())

	CHackerGuardianView() 
		: m_bSortord(FALSE)
		, m_nColumn(-1)
		, m_eTabId(eProcess)
		, m_top_tab(NULL)
		, m_bottom_tab(NULL)
		, m_vertical_splitter(NULL)
		, m_horizontal_splitter(NULL)
	{
	}

	BOOL PreTranslateMessage(MSG* pMsg)
	{
		pMsg;
		return FALSE;
	}

	BEGIN_MSG_MAP(CHackerGuardianView)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		//MSG_WM_RBUTTONDOWN(OnRButtonDown)
		REFLECTED_NOTIFY_CODE_HANDLER(LVN_DELETEITEM, OnDeleteItem)
		REFLECTED_NOTIFY_CODE_HANDLER/*_EX*/(LVN_COLUMNCLICK , OnColumnClick)
		REFLECTED_NOTIFY_CODE_HANDLER(LVN_KEYDOWN, OnLvnKeyDown)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(NM_RCLICK, OnRClick)
		//REFLECTED_NOTIFY_CODE_HANDLER_EX(NM_CLICK, OnLClick)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(NM_DBLCLK, OnLDBClk)
		REFLECTED_NOTIFY_CODE_HANDLER_EX(NM_CUSTOMDRAW, OnCustomDraw)
		// Process
		COMMAND_ID_HANDLER(ID_RM_PROCESS_REFRESH, OnRM_ProcessRefresh)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MODULE, OnRM_ProcessModule)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_PROPERTIES, OnRM_ProcessProperties)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_FULLDUMP, OnRM_ProcessFullDump)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MINIDUMP, OnRM_ProcessMiniDump)
		// Module
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MODULE_REFRESH, OnRM_ProcessModuleRefresh)
		// Memory
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MEMORY_EDIT, OnRM_ProcessMemoryEdit)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MEMORY_DISASSEMBLE, OnRM_ProcessMemoryDisassemble)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MEMORY_ZERO, OnRM_ProcessMemoryZeroMemory)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MEMORY_PROTECT, OnRM_ProcessMemoryProtect)
		MESSAGE_HANDLER(WM_MEM_CHANGE_PROTECTION, OnMemChangeProtection)
		COMMAND_ID_HANDLER(ID_RM_PROCESS_MEMORY_FREE, OnRM_ProcessMemoryFree)
		// Process Handle
		COMMAND_ID_HANDLER(IDRM_PROCESS_HANDLE_REFRESH, OnRM_ProcessHandle_Refresh)
		COMMAND_ID_HANDLER(IDRM_PROCESS_HANDLE_CLOSE, OnRM_ProcessHandle_Close)
		COMMAND_ID_HANDLER(IDRM_PROCESS_HANDLE_FORCECLOSE, OnRM_ProcessHandle_ForceClose)		
		// Driver
		COMMAND_ID_HANDLER(ID_RM_DRIVER_REFRESH, OnRM_DriverRefresh)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
//	
	LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled){
		return S_OK;
	}

	void OnRButtonDown(UINT nFlags, CPoint point){
		::MessageBoxA(NULL, "a", "b", MB_OK);
		int a = GetSelectedIndex();
		int b = a;
	}

	LRESULT OnDeleteItem(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
 		LPNMLISTVIEW pnmv = (LPNMLISTVIEW)pnmh;
 		T* pItem = (T*)GetItemData(pnmv->iItem);
 		ATLASSERT(pItem);
 		if (pItem)	// Delete attached structure
 			delete pItem;
		return 0;
	}

	LRESULT OnLvnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		::MessageBoxA(NULL, "a", "b", MB_OK);
		return 0;
	}

	LRESULT OnLvnKeyDown(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	{
		LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pnmh;
		CHackerGuardianView *pListCtrl = this;
		//::MessageBoxA(NULL, "a", "b", MB_OK);
		
		//POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
//		this->GetFirstSelectedItemPosition();
// 		if(pos!=NULL)
// 		{
// 			int iCurSel = pListCtrl->GetNextSelectedItem(pos);
// 			if(pLVKeyDow->wVKey==VK_UP)
// 			{ //按一次UP键选中项会上移一位，但是这个操作是在按下UP键时触发的
// 				//此时还并未移动，所以手动调整选中项的位置
// 				if(iCurSel>=1)
// 					iCurSel--; 
// 				if(iCurSel>=0)
// 					pListCtrl->SetItemState(iCurSel,LVIS_SELECTED,LVIS_SELECTED);//在此处进行需要的操作  
// 			}
// 			else if(pLVKeyDow->wVKey==VK_DOWN)
// 			{ //按一次UP键选中项会下移一位，但是这个操作是在按下DOWN键时触发的
// 				//此时还并未移动，所以手动调整选中项的位置
// 				if(iCurSel<pListCtrl->GetItemCount())
// 					iCurSel++;
// 				if(iCurSel>=0)
// 					pListCtrl->SetItemState(iCurSel,LVIS_SELECTED,LVIS_SELECTED);//在此处进行需要的操作
// 			}
// 		}
		return 0;
	}

	LRESULT OnColumnClick(int /*idCtrl*/, LPNMHDR pnmh, BOOL& /*bHandled*/)
	//LRESULT OnColumnClick(NMHDR* phdr)
	{
		LPNMLISTVIEW pnmv=(LPNMLISTVIEW)pnmh;
		BOOL bSortord = 
			(m_nColumn == pnmv->iSubItem ? !m_bSortord : Ascending);
		m_nColumn = pnmv->iSubItem;
		m_bSortord = bSortord;
		this->SortItems(CompareFunc, MAKELPARAM(m_nColumn, m_bSortord));
		return 0;
	}

	void SetTabId(TABID t){
		m_eTabId = t;
	}

	void SetTab(CTabView* top_tab, CTabView* bottom_tab, CSplitterWindow* vertical_splitter, CHorSplitterWindow* horizontal_splitter){
		m_top_tab = top_tab;
		m_bottom_tab = bottom_tab;
		m_vertical_splitter = vertical_splitter;
		m_horizontal_splitter = horizontal_splitter;
	}

	LRESULT OnRClick(NMHDR* phdr)
	{
		LPNMLISTVIEW pNMListView = (LPNMLISTVIEW)phdr;
		if(pNMListView->iItem != -1)
		{
			// RClick menu for process
			if (m_eTabId == eProcess){
				DWORD dwPos = GetMessagePos();
				CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

				CMenuHandle menuRClick;
				menuRClick.LoadMenu(IDR_RCLICK_PROCESS);
				CMenuHandle menuPopup = menuRClick.GetSubMenu(0);
				menuPopup.TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
											point.x, 
											point.y, 
											this->m_hWnd);
			}else if (m_eTabId == eProcessModule){
				DWORD dwPos = GetMessagePos();
				CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

				CMenuHandle menuRClick;
				menuRClick.LoadMenu(IDR_RCLICK_PROCESS_MODULE);
				CMenuHandle menuPopup = menuRClick.GetSubMenu(0);
				menuPopup.TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
											point.x, 
											point.y, 
											this->m_hWnd);
			}else if (m_eTabId == eProcessMemory){
				DWORD dwPos = GetMessagePos();
				CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

				CMenuHandle menuRClick;
				menuRClick.LoadMenu(IDR_RCLICK_PROCESS_MEMORY);
				CMenuHandle menuPopup = menuRClick.GetSubMenu(0);

// 				CMenu MemMenu;
// 				MemMenu.CreatePopupMenu();
// 				MemMenu.AppendMenu(MF_STRING, ID_RM_PROCESS_MEMORY_EDIT, L"Edit");
// 				MemMenu.AppendMenu(MF_STRING, ID_RM_PROCESS_MEMORY_DISASSEMBLE, L"Disassemble");
// 				MemMenu.AppendMenu(MF_STRING, ID_RM_PROCESS_MEMORY_ZERO, L"Zero Memory");
// 				MemMenu.AppendMenu(MF_STRING, ID_RM_PROCESS_MEMORY_PROTECT, L"Change Protection");
// 				MemMenu.AppendMenu(MF_STRING, ID_RM_PROCESS_MEMORY_FREE, L"Free Memory");
// 				MemMenu.AppendMenu(MF_SEPARATOR);
// 				MemMenu.AppendMenu(MF_POPUP, (UINT)MemMenu.m_hMenu, L"");
// 				MemMenu.TrackPopupMenu(TPM_RIGHTBUTTON, point.x, point.y, this->m_hWnd);

				for(int i=0; i<this->GetItemCount(); i++){
					if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
						PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
						WCHAR szProtectW[MAX_PATH] = {0};
						WCHAR szStateW[MAX_PATH] = {0};
						CString strProtect;
						this->GetItemText(i, 2, szProtectW, MAX_PATH);
						CString strState;
						this->GetItemText(i, 3, szStateW, MAX_PATH);
						strProtect = szProtectW;
						strState = szStateW;
						
						if (strProtect.Find(L"Write") == -1)	{
							menuPopup.EnableMenuItem(ID_RM_PROCESS_MEMORY_ZERO, MF_GRAYED | MF_DISABLED);
						}
						if (strProtect.Find(L"Execute") == -1){
							menuPopup.EnableMenuItem(ID_RM_PROCESS_MEMORY_DISASSEMBLE, MF_GRAYED | MF_DISABLED);
						}
						if (!strState.CompareNoCase(L"Free") && strProtect.Find(L"No") != -1){
							menuPopup.EnableMenuItem(ID_RM_PROCESS_MEMORY_FREE, MF_GRAYED | MF_DISABLED);
							menuPopup.EnableMenuItem(ID_RM_PROCESS_MEMORY_PROTECT, MF_GRAYED | MF_DISABLED);
						}
						if (!strState.CompareNoCase(L"Reserve")){
							menuPopup.EnableMenuItem(ID_RM_PROCESS_MEMORY_PROTECT, MF_GRAYED | MF_DISABLED);
						}
						break;
					}
				}
				menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this->m_hWnd);
			}	// ====== Process Handle =====
			else if (m_eTabId == eProcessHandle){
				DWORD dwPos = GetMessagePos();
				CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
				CMenuHandle menuRClick;
				menuRClick.LoadMenu(IDR_RCLICK_PROCESS_HANDLE);
				CMenuHandle menuPopup = menuRClick.GetSubMenu(0);
				menuPopup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this->m_hWnd);
			}	// ====== Driver =====
			else if (m_eTabId == eDriver){
				DWORD dwPos = GetMessagePos();
				CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

				CMenuHandle menuRClick;
				menuRClick.LoadMenu(IDR_RCLICK_DRIVER);
				CMenuHandle menuPopup = menuRClick.GetSubMenu(0);
				menuPopup.TrackPopupMenu(	TPM_LEFTALIGN | TPM_RIGHTBUTTON,
											point.x,
											point.y, 
											this->m_hWnd);
			}
		}
		return 0;
	}

	LRESULT OnLDBClk(NMHDR* phdr)
	{
		LPNMLISTVIEW pNMListView = (LPNMLISTVIEW)phdr;
		if(pNMListView->iItem >= 0)
		{
			// RClick menu for process
			if (m_eTabId == eProcess){
				SzLogA("[HackerGuardianView.h].[OnLClick]: Enter eProcess!");
				PSZ_PROCESSINFO pi = (PSZ_PROCESSINFO)GetItemData(pNMListView->iItem);
				CLVProcess::GetInstance()->BottomTabRemoveAllPage();
				CLVProcess::GetInstance()->DisplayProcessModule(pi->m_nPid, pi->m_nEProcessAddress);
				CLVProcess::GetInstance()->DisplayProcessThread(pi->m_nPid, pi->m_nEProcessAddress);
				CLVProcess::GetInstance()->DisplayProcessMemory(pi->m_nPid, pi->m_nEProcessAddress);
				CLVProcess::GetInstance()->DisplayProcessHandle(pi->m_nPid, pi->m_nEProcessAddress);
			}
		}

		return 0;
	}

	LRESULT OnRM_ProcessRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CLVProcess::GetInstance()->RefreshProcess(/*this*/);
		return 0;
	}

	LRESULT OnRM_ProcessFullDump(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{	
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PSZ_PROCESSINFO pi = (PSZ_PROCESSINFO)GetItemData(i);
				CreateProcessDump((DWORD)pi->m_nPid, pi->m_ptszName, FALSE);
				break;
			}
		}
		return 0;
	}

	LRESULT OnRM_ProcessMiniDump(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{	
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PSZ_PROCESSINFO pi = (PSZ_PROCESSINFO)GetItemData(i);
				CreateProcessDump((DWORD)pi->m_nPid, pi->m_ptszName, TRUE);
				break;
			}
		}
		return 0;
	}

	LRESULT OnRM_ProcessModule(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtrl*/, BOOL& /*bHandled*/)
	{
		//CLVProcess::GetInstance()->DisplayProcessModule((size_t)GetSelectedIndex());

		return 0;
	}

	LRESULT OnRM_ProcessProperties(WORD wNotifyCode, WORD wID, HWND /*hWndCtrl*/, BOOL& /*bHandled*/)
	{
		PSZ_PROCESSINFO pi = NULL;
		for(int i=0; i<this->GetItemCount(); i++)
		{
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED )
			{
				pi = (PSZ_PROCESSINFO)GetItemData(i);
				break;
			}
		}
		CUIBase::PropertiesDialog(pi->m_ptszPath);
		return 0;
	}

	LRESULT OnRM_ProcessModuleRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::MessageBoxA(NULL, "OnRM_ProcessModuleRefresh", "OnRM_ProcessModuleRefresh", MB_OK);
		// 		if (gQueryInterface != NULL){
		// 			PSzhProcessRetrieve SzhProcessRetrieve = (PSzhProcessRetrieve)gQueryInterface(eProcessRetrieve);			 
		// 			if (SzhProcessRetrieve != NULL){
		// 				SzhProcessRetrieve();
		// 			}
		// 		}
		return 0;
	}

	LRESULT OnRM_ProcessMemoryEdit(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CHexDlg dlg;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
				dlg.InitValue(MemInfo->m_nBase, (DWORD)MemInfo->m_nSize, MemInfo->m_pEProcess, MemInfo->m_szModuleNameW);
				break;
			}
		}
		dlg.DoModal();
		return 0;
	};

	LRESULT OnRM_ProcessMemoryDisassemble(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CDisassemblerDlg dlg;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
				if (MemInfo->m_nSize < 0x1000)
				{
					dlg.InitValue(MemInfo->m_nBase, (DWORD)MemInfo->m_nSize, MemInfo->m_pEProcess);
				}
				else
				{
					dlg.InitValue(MemInfo->m_nBase, 0x1000, MemInfo->m_pEProcess);
				}
				break;
			}
		}
		dlg.DoModal();
		return 0;
	};

	LRESULT OnRM_ProcessMemoryZeroMemory(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (AtlMessageBox(m_hWnd, L"Warning! This operation is danger. Are you sure to continue?", L"Warning", MB_OKCANCEL | MB_ICONWARNING) == IDCANCEL){
			return 0;
		}
		BOOL bRet = FALSE;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
				bRet = CHGDriver::GetInstance()->ProcessMemoryZero(
					MemInfo->m_Pid,
					MemInfo->m_pEProcess, 
					MemInfo->m_nBase, 
					MemInfo->m_nSize);
				break;
			}
		}
		if (bRet){
			AtlMessageBox(m_hWnd, L"Zero memory successfully!", L"Info", MB_OK);
		}else{
			AtlMessageBox(m_hWnd, L"Sorry! This operation failed!", L"Error", MB_OK);
		}
		return 0;
	};

	LRESULT OnRM_ProcessMemoryProtect(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		CMemoryChangeProtectionDlg dlg;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
				dlg.Initializate(MemInfo->m_Pid, MemInfo->m_pEProcess, MemInfo->m_nBase, (DWORD)MemInfo->m_nSize, this->m_hWnd);
				break;
			}
		}
		dlg.DoModal();
		return 0;
	}

	LRESULT OnMemChangeProtection(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				this->SetItemText(i, 2, CLVProcess::GetInstance()->GetMemoryProtectString((SIZE_T)wParam));
				break;
			}
		}
		
		return 0;
	}

	LRESULT OnRM_ProcessMemoryFree(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (AtlMessageBox(m_hWnd, L"Warning! This operation is danger. Are you sure to continue?", L"Warning", MB_OKCANCEL | MB_ICONASTERISK) == IDCANCEL){
			return 0;
		}
		BOOL bRet = FALSE;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				PHG_MEMORY_INFO MemInfo = (PHG_MEMORY_INFO)GetItemData(i);
				bRet = CHGDriver::GetInstance()->ProcessMemoryFree(
					MemInfo->m_Pid,
					MemInfo->m_pEProcess, 
					MemInfo->m_nBase, 
					MemInfo->m_nSize);
				if (bRet){
					AtlMessageBox(m_hWnd, L"Free memory successfully!", L"Info", MB_OK);
					this->DeleteItem(i);
				}else{
					AtlMessageBox(m_hWnd, L"Sorry! This operation failed!", L"Error", MB_OK);
				}
				break;
			}
		}
		return 0;
	}

	LRESULT OnRM_ProcessHandle_Close(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bRet = FALSE;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				HG_HANDLE_INFO info = *(PHG_HANDLE_INFO)GetItemData(i);
				bRet = CHGDriver::GetInstance()->CloseProcessHandle(info.m_Pid, info.m_pEProcess, info.m_nHandle, info.m_nHandleObject, FALSE);
				if (bRet == FALSE){
					AtlMessageBox(m_hWnd, L"Sorry! Failed to close the handle!", L"Error", MB_OK);
				} else {
					DeleteItem(i);
					//CLVProcess::GetInstance()->RefreshProcessHandles(info.m_Pid, info.m_pEProcess);
				}
				break;
			}
		}
		return S_OK;
	}

	LRESULT OnRM_ProcessHandle_ForceClose(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bRet = FALSE;
		for(int i=0; i<this->GetItemCount(); i++){
			if(this->GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ){
				HG_HANDLE_INFO info = *(PHG_HANDLE_INFO)GetItemData(i);
				bRet = CHGDriver::GetInstance()->CloseProcessHandle(info.m_Pid, info.m_pEProcess, info.m_nHandle, info.m_nHandleObject, TRUE);
				if (bRet == FALSE){
					AtlMessageBox(m_hWnd, L"Sorry! Failed to close the handle!", L"Error", MB_OK);
				} else {
					DeleteItem(i);
					//CLVProcess::GetInstance()->RefreshProcessHandles(info.m_Pid, info.m_pEProcess);
				}
				break;
			}
		}
		return S_OK;
	}

	LRESULT OnRM_ProcessHandle_Refresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		for(int i=0; i<this->GetItemCount(); i++){
			HG_HANDLE_INFO info = *(PHG_HANDLE_INFO)GetItemData(i);
			CLVProcess::GetInstance()->RefreshProcessHandles(info.m_Pid, info.m_pEProcess);
			break;
		}
		return S_OK;
	}

	LRESULT OnRM_DriverRefresh(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		::MessageBoxA(NULL, "OnRM_DriverRefresh", "OnRM_DriverRefresh", MB_OK);
		// 		if (gQueryInterface != NULL){
		// 			PSzhProcessRetrieve SzhProcessRetrieve = (PSzhProcessRetrieve)gQueryInterface(eProcessRetrieve);			 
		// 			if (SzhProcessRetrieve != NULL){
		// 				SzhProcessRetrieve();
		// 			}
		// 		}
		return 0;
	}

	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2,LPARAM lParamSort)
	{
		T* pItem1=(T*)lParam1;
		ATLASSERT(pItem1);
		T* pItem2=(T*)lParam2;
		ATLASSERT(pItem2);
		switch (HIWORD(lParamSort))
		{
		case (WORD)Ascending:
			return pItem1->Compare(pItem2,LOWORD(lParamSort));
		case (WORD)Descending:
			return pItem2->Compare(pItem1,LOWORD(lParamSort));
		}
		return 0;
	}

	LRESULT OnCustomDraw(NMHDR* pNMHDR)
	{
		NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
		// First thing - check the draw stage. If it's the control's prepaint
		// stage, then tell Windows we want messages for every item.
		if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
		{
			return CDRF_NOTIFYITEMDRAW;
		}
		else if ( CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage )
		{
			// This is the prepaint stage for an item. Here's where we set the
			// item's text color. Our return value will tell Windows to draw the
			// item itself, but it will use the new color we set here.
			// We'll cycle the colors through red, green, and light blue.
			COLORREF crText = RGB(0, 0, 0);

// 			if ( (pLVCD->nmcd.dwItemSpec % 3) == 0 )
// 				crText = RGB(255,0,0);
// 			else if ( (pLVCD->nmcd.dwItemSpec % 3) == 1 )
// 				crText = RGB(0,255,0);
// 			else
// 				crText = RGB(128,128,255);
// 				
			switch (m_eTabId)
			{
			case eProcess:
				{
					PSZ_PROCESSINFO pi = (PSZ_PROCESSINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = pi->m_RGB;
				}
				break;
			case eProcessModule:
				{
					PSZ_MODULEINFO mi = (PSZ_MODULEINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = mi->m_RGB;
				}
				break;
			case eDriver:
				{
					PHG_DRIVERINFO di = (PHG_DRIVERINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = di->m_RGB;
				}
				break;
			case eKernelNotify:
				{
					PHG_NOTIFYINFO di = (PHG_NOTIFYINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = di->m_RGB;
				}
				break;
			case eHookSSDT:
				{
					PHG_SDTINFO arg0 = (PHG_SDTINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = arg0->m_RGB;
				}
				break;
			case eHookShadowSSDT:
				{
					PHG_SDTINFO arg0 = (PHG_SDTINFO)GetItemData((int)(pLVCD->nmcd.dwItemSpec));
					crText = arg0->m_RGB;
				}
				break;
			}
			
			// Store the color back in the NMLVCUSTOMDRAW struct.
			pLVCD->clrText = crText;
			// Tell Windows to paint the control itself.
			return CDRF_DODEFAULT;
		}
		return CDRF_DODEFAULT;
	}
};
