// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"

#include "HGDriver.h"

class CHgListCtrl : 
	public CWindowImpl<CHgListCtrl, CListViewCtrl>
{   
	BEGIN_MSG_MAP_EX(CHgListCtrl)        
		/*		MSG_WM_CONTEXTMENU(OnContextMenu) */   
		END_MSG_MAP() 
};

class CDisassemblerDlg : 
	public CDialogImpl<CDisassemblerDlg>,
	public CWinDataExchange<CDisassemblerDlg>

{
	BEGIN_DDX_MAP(CDisassemblerDlg)
		DDX_CONTROL(IDC_EDIT_BASE, m_wndBase)
		DDX_CONTROL(IDC_EDIT_SIZE, m_wndSize)
		DDX_CONTROL(IDC_LIST_DISASSEMBLER, m_wndListCtrl)
	END_DDX_MAP()
private:
	SIZE_T m_TargetBase;
	PBYTE m_Buffer;
	SIZE_T m_BufferLength;
	SIZE_T m_CurrentEProcess;
	CHgListCtrl m_wndListCtrl;

protected:
	CHgHexEdit	m_wndBase;
	CHgHexEdit	m_wndSize;
public:
	enum { IDD = IDD_DISASSEMBLER };

	CDisassemblerDlg()
		: m_Buffer(NULL)
		, m_BufferLength(0)
		, m_TargetBase(0)
		, m_CurrentEProcess(0)
	{

	}

	void InitValue(SIZE_T Base, SIZE_T Size, SIZE_T pEProcess){
		SIZE_T NumberOfBytesRead = 0;

		m_TargetBase = Base;
		m_CurrentEProcess = pEProcess;

		m_BufferLength = Size;
		//m_Buffer = (PBYTE)malloc(m_BufferLength);
	}

	BEGIN_MSG_MAP(CHexDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());
		DoDataExchange(FALSE);

		//if (m_Buffer)
		{
			CString strTmp;
			strTmp.Format(L"%p", m_TargetBase);
			m_wndBase.SetWindowTextW(strTmp);
			strTmp.Format(L"%08X", m_BufferLength);
			m_wndSize.SetWindowTextW(strTmp);
		}
// 		else
// 		{
// #if defined(_WIN64)
// 			m_wndBase.SetWindowTextW(L"0000000000400000");
// #else
// 			m_wndBase.SetWindowTextW(L"00400000");
// #endif
// 			m_wndSize.SetWindowTextW(L"1000");
// 		}

		LONG lStyle;
		lStyle = ::GetWindowLong(m_wndListCtrl.m_hWnd, GWL_STYLE);
		lStyle &= ~LVS_TYPEMASK;
		lStyle |= LVS_REPORT; 
		::SetWindowLong(m_wndListCtrl.m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_wndListCtrl.GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
		//dwStyle |= LVS_EX_CHECKBOXES;
		m_wndListCtrl.SetExtendedListViewStyle(dwStyle);

		m_wndListCtrl.InsertColumn(0, L"Ordinal", LVCFMT_CENTER, 64);
#if defined(_WIN64)
		m_wndListCtrl.InsertColumn(1, L"Address", LVCFMT_LEFT, 150);
#else
		m_wndListCtrl.InsertColumn(1, L"Address", LVCFMT_LEFT, 100);
#endif
		m_wndListCtrl.InsertColumn(2, L"Hex", LVCFMT_LEFT, 150);
		m_wndListCtrl.InsertColumn(3, L"Assembly", LVCFMT_LEFT, 360);
		
		PITP pMemItp = NULL;
		CHGDriver::GetInstance()->ReadProcessMemoryAux(m_TargetBase, m_BufferLength, m_CurrentEProcess, &pMemItp);
		if (pMemItp){
			m_Buffer = pMemItp->mem.MemoryBlock;
			Disassemble();
		}
		CHGDriver::GetInstance()->FreeItp(pMemItp);
		m_Buffer = NULL;

		return TRUE;
	}

	LRESULT OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		EndDialog(wID);
		return 0;
	}

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		WCHAR BaseBuffer[MAX_PATH] = {0};
		WCHAR SizeBuffer[MAX_PATH] = {0};
		m_wndBase.GetWindowTextW(BaseBuffer, MAX_PATH);
		m_wndSize.GetWindowTextW(SizeBuffer, MAX_PATH); 

		// Transfer data from the controls to member variables.
		if ( !DoDataExchange(true) )        
			return 0;//...

		int errorno = 0;
		SIZE_T Base = 0;
		DWORD Size = 0;
		Base = StrHexToULongA(CW2A(BaseBuffer), &errorno);
		if (errorno == -1)
		{
			::MessageBoxW(NULL, L"Please type hex characters, don't use other characters like space key and so on!", L"Error", MB_OK);
			return 0;
		}
		Size = StrHexToULongA(CW2A(SizeBuffer), &errorno);
		if (errorno == -1)
		{
			::MessageBoxW(NULL, L"Please type hex characters, don't use other characters like space key and so on!", L"Error", MB_OK);
			return 0;
		}

		if (m_Buffer && m_BufferLength){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}

		SIZE_T NumberOfBytesRead = 0;
		m_TargetBase = Base;
		m_BufferLength = Size;
		//m_Buffer = (PBYTE)malloc(m_BufferLength);

		PITP pMemItp = NULL;
		CHGDriver::GetInstance()->ReadProcessMemoryAux(m_TargetBase, m_BufferLength, m_CurrentEProcess, &pMemItp);
		if (pMemItp){
			//RtlCopyMemory(m_Buffer, pMemItp->mem.MemoryBlock, Size);
			m_Buffer = pMemItp->mem.MemoryBlock;
			Disassemble();
		}
		CHGDriver::GetInstance()->FreeItp(pMemItp);
		m_Buffer = NULL;

		return 0;
	}

	void Disassemble()
	{
		m_wndListCtrl.DeleteAllItems();
		DISASM MyDisasm;
		int i = 0;

		UIntPtr InstBase = 0;
		int InstLength = 0;
		CHAR HexBuffer[MAX_PATH] = {0};
		CHAR SingleHex[4] = {0};

		for (InstBase = (UIntPtr)m_Buffer; InstBase < (UIntPtr)(m_Buffer + m_BufferLength); InstBase += InstLength)
		{
			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = InstBase;
#if defined(_WIN64)
			/* =================== Define 64 bits architecture =================== */
			MyDisasm.Archi = 64;
#else
			MyDisasm.Archi = 0;
#endif
			InstLength = Disasm(&MyDisasm);
			if (InstLength == UNKNOWN_OPCODE)
			{
				InstLength = 1;
			}
			RtlZeroMemory(HexBuffer, MAX_PATH);
			for (i = 0; i < InstLength; i ++)
			{
				_itoa_s(*(PBYTE)(InstBase + i), SingleHex, 4, 16);
				_strupr_s(SingleHex, 4);
				if (strlen(SingleHex) == 1)
				{
					SingleHex[3] = 0;
					SingleHex[2] = 0;
					SingleHex[1] = SingleHex[0];
					SingleHex[0] = '0';
				}
				strcat_s(HexBuffer, MAX_PATH, SingleHex);
			}
			if (InstLength != UNKNOWN_OPCODE){
				InsertRecord(InstBase - (SIZE_T)m_Buffer + m_TargetBase, HexBuffer, MyDisasm.CompleteInstr);
			}else{
				InsertRecord(InstBase - (SIZE_T)m_Buffer + m_TargetBase, HexBuffer, "??");
			}
		}
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (m_Buffer && m_BufferLength){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (m_Buffer && m_BufferLength){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}
		int nRetCode = 0;
		EndDialog(nRetCode);
		return 0;
	}

	void InsertRecord(SIZE_T Address, PCHAR Hex, PCHAR Assembly)
	{
		int nRow = 0;
		if (Hex == NULL || Address == 0 || Assembly == NULL){
			return;
		}

		nRow = m_wndListCtrl.InsertItem(m_wndListCtrl.GetItemCount(), size_tToCString(m_wndListCtrl.GetItemCount()));
		CString strAddress;
		strAddress.Format(L"%p", Address);
		m_wndListCtrl.SetItemText(nRow, 1, strAddress);
		m_wndListCtrl.SetItemText(nRow, 2, CA2W(Hex));
		m_wndListCtrl.SetItemText(nRow, 3, CA2W(Assembly));
	}
};
