// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"

#include "HGDriver.h"

class CHgComboBox : 
	public CWindowImpl<CHgComboBox, CComboBox>
{   
	BEGIN_MSG_MAP_EX(CHgListCtrl)        
		/*		MSG_WM_CONTEXTMENU(OnContextMenu) */   
		END_MSG_MAP() 
};

class CMemoryChangeProtectionDlg : 
	public CDialogImpl<CMemoryChangeProtectionDlg>,
	public CWinDataExchange<CMemoryChangeProtectionDlg>

{
	BEGIN_DDX_MAP(CMemoryChangeProtectionDlg)
		DDX_CONTROL(IDC_CBB_PROTECTION, m_wndProtection)
	END_DDX_MAP()
private:
	CHgComboBox m_wndProtection;
	SIZE_T		m_Pid;
	SIZE_T		m_EProcess;
	SIZE_T		m_Base;
	SIZE_T		m_Size;
	HWND		m_ParentWnd;

protected:
	
public:
	enum { IDD = IDD_MEMORY_CHANGE_PROTECTION };

	BEGIN_MSG_MAP(CHexDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
//		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

	void Initializate(SIZE_T Pid, SIZE_T EProcess, SIZE_T Base, SIZE_T Size, HWND ParentWnd){
		m_Pid = Pid;
		m_EProcess = EProcess;
		m_Base = Base;
		m_Size = Size;
		m_ParentWnd = ParentWnd;
	}

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		DoDataExchange(FALSE);

		m_wndProtection.AddString(L"PAGE_NOACCESS");
		m_wndProtection.AddString(L"PAGE_READONLY");
		m_wndProtection.AddString(L"PAGE_READWRITE");
		m_wndProtection.AddString(L"PAGE_WRITECOPY");
		m_wndProtection.AddString(L"PAGE_EXECUTE");
		m_wndProtection.AddString(L"PAGE_EXECUTE_READ");
		m_wndProtection.AddString(L"PAGE_EXECUTE_READWRITE");
		m_wndProtection.AddString(L"PAGE_EXECUTE_WRITECOPY");

		m_wndProtection.SetItemData(0, PAGE_NOACCESS);
		m_wndProtection.SetItemData(1, PAGE_READONLY);
		m_wndProtection.SetItemData(2, PAGE_READWRITE);
		m_wndProtection.SetItemData(3, PAGE_WRITECOPY);
		m_wndProtection.SetItemData(4, PAGE_EXECUTE);
		m_wndProtection.SetItemData(5, PAGE_EXECUTE_READ);
		m_wndProtection.SetItemData(6, PAGE_EXECUTE_READWRITE);
		m_wndProtection.SetItemData(7, PAGE_EXECUTE_WRITECOPY);

		m_wndProtection.SetCurSel(6);

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		BOOL bRet = CHGDriver::GetInstance()->ProcessMemoryChangeProtection(
			m_Pid,
			m_EProcess,
			m_Base,
			m_Size,
			(SIZE_T)m_wndProtection.GetItemData(m_wndProtection.GetCurSel()));

		if (bRet)
		{
			AtlMessageBox(m_hWnd, L"Change protection successfully!", L"Info", MB_OK);
			::SendMessage(m_ParentWnd, WM_MEM_CHANGE_PROTECTION, (WPARAM)(m_wndProtection.GetItemData(m_wndProtection.GetCurSel())), NULL);
			EndDialog(wID);
		}
		else
		{
			AtlMessageBox(m_hWnd, L"Sorry! Change protection failed!", L"Info", MB_OK);
		}
		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

// 	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
// 	{
// 		int nRetCode = 0;
// 		EndDialog(nRetCode);
// 		return 0;
// 	}
};
