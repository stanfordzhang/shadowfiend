// aboutdlg.h : interface of the CAboutDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "stdafx.h"

#include "HGDriver.h"

#define IS_HEXCHAR(ch)  ((ch >= L'0' && ch <= L'9') || (ch >= L'a' && ch <= L'f') || (ch >= L'A' && ch <= L'F'))

class CHgHexEdit : 
	public CWindowImpl<CHgHexEdit, CEdit>
{    
	BEGIN_MSG_MAP_EX(CHgHexEdit)
		MESSAGE_HANDLER(WM_CHAR, OnChar)
		REFLECTED_COMMAND_CODE_HANDLER(EN_CHANGE, OnChange)
		DEFAULT_REFLECTION_HANDLER()
	END_MSG_MAP()  

	LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		WCHAR ch = (WCHAR)wParam;

		if (IS_HEXCHAR(ch) || ch == VK_BACK || ch == VK_DELETE)
		{
			bHandled = FALSE;
		}

		return 0;
	}

	LRESULT OnChange(UINT code, UINT id, HWND hCtrl, BOOL &bHandled)
	{
		int nLen = ::GetWindowTextLengthW(hCtrl);
		if (nLen <= 0) return 0;
		WCHAR *pBuffer = new WCHAR[++nLen];
		ZeroMemory(pBuffer, nLen*sizeof(WCHAR));
		::GetWindowTextW(hCtrl, pBuffer, nLen);
		BOOL bIsNotHexChar = FALSE;

		for (int i = 0; i < (nLen - 1); i++)
		{
			if (IS_HEXCHAR(pBuffer[i]))
				continue;

			bIsNotHexChar = TRUE;
			pBuffer[i] = L'\0';
			break;
		}

		if (bIsNotHexChar)
			::SetWindowTextW(hCtrl, pBuffer);

		delete pBuffer;

		return 0;
	}
};

class CHgListBox : 
	public CWindowImpl<CHgListBox, CListBox>
{   
	BEGIN_MSG_MAP_EX(CHgListBox)        
		/*		MSG_WM_CONTEXTMENU(OnContextMenu) */   
	END_MSG_MAP() 
};

class CHexDlg : 
	public CDialogImpl<CHexDlg>,
	public CWinDataExchange<CHexDlg>

{
	BEGIN_DDX_MAP(CHexDlg)
		DDX_CONTROL(IDC_EDIT_BASE, m_wndBase)
		DDX_CONTROL(IDC_EDIT_SIZE, m_wndSize)
		DDX_CHECK(IDC_CHK_UNICODE, m_nUnicode)
		DDX_CONTROL(IDC_EDIT_MASK, m_mask)
		DDX_CONTROL(IDC_LIST_ADDRESS, m_AddressList)
	END_DDX_MAP()
private:
	SIZE_T m_TargetBase;
	PBYTE m_Buffer;
	SIZE_T m_BufferLength;
	BOOL m_bIsFile;
	SIZE_T m_CurrentEProcess;
	CString m_HostName;
protected:
	CHgHexEdit	m_wndBase;
	CHgHexEdit	m_wndSize;
	int m_nUnicode;
	CHgHexEdit m_mask;
	CHgListBox m_AddressList;
public:
	enum { IDD = IDD_HEX_EDITOR };
	CHexEditorCtrl m_HexEditor;

	CHexDlg()
		: m_nUnicode(0)
		, m_Buffer(NULL)
		, m_BufferLength(0)
		, m_TargetBase(0)
		, m_bIsFile(FALSE)
		, m_CurrentEProcess(0)
	{

	}

	void InitValue(SIZE_T Base, SIZE_T Size, SIZE_T pEProcess, PWCHAR HostName){
		SIZE_T NumberOfBytesRead = 0;

		m_TargetBase = Base;
		m_bIsFile = FALSE;
		m_CurrentEProcess = pEProcess;

		m_BufferLength = Size;
		m_Buffer = (PBYTE)malloc(m_BufferLength);
		m_HostName = HostName;

		PITP pMemItp = NULL;
		CHGDriver::GetInstance()->ReadProcessMemoryAux(m_TargetBase, Size, pEProcess, &pMemItp);

		if (pMemItp){
			RtlCopyMemory(m_Buffer, pMemItp->mem.MemoryBlock, Size);
		}

		CHGDriver::GetInstance()->FreeItp(pMemItp);
	}

	BEGIN_MSG_MAP(CHexDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_BTN_SEARCH, OnSearch)
		COMMAND_ID_HANDLER(IDC_BTN_FILE, OnFile)
		COMMAND_ID_HANDLER(IDC_BTN_SAVE, OnSave)
		REFLECT_NOTIFICATIONS()
	END_MSG_MAP()


	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CenterWindow(GetParent());

		m_HexEditor.Create(m_hWnd, rcDefault, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, WS_EX_CLIENTEDGE);

		//m_HexEditor.SetDisplayOptions(TRUE, TRUE, FALSE);
		//SIZE sz = { 20, 20 }; m_view.SetMargins(sz); sz = m_view.GetMargins();
		//m_HexEditor.SetReadOnly();
		m_HexEditor.SetDataSize(1);
		m_HexEditor.MoveWindow(0, 45, 720, 480);
		DoDataExchange(FALSE);

		if (m_Buffer)
		{
			CString strTmp;
			strTmp.Format(L"%p", m_TargetBase);
			m_wndBase.SetWindowTextW(strTmp);
			strTmp.Format(L"%08X", m_BufferLength);
			m_wndSize.SetWindowTextW(strTmp);
		}
		else
		{
#if defined(_WIN64)
			m_wndBase.SetWindowTextW(L"0000000000400000");
#else
			m_wndBase.SetWindowTextW(L"00400000");
#endif
			m_wndSize.SetWindowTextW(L"1000");
		}

		m_HexEditor.SetBuffer((PBYTE)m_Buffer, m_BufferLength, m_TargetBase);

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
		if ( m_nUnicode ){
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, FALSE);
		}else{
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, TRUE);
		}

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

		if (m_Buffer && m_BufferLength && (m_bIsFile == FALSE)){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}

		if (m_bIsFile == FALSE)
		{
			SIZE_T NumberOfBytesRead = 0;
			m_TargetBase = Base;
			m_BufferLength = Size;
			m_Buffer = (PBYTE)malloc(m_BufferLength);
			PITP pMemItp = NULL;
			CHGDriver::GetInstance()->ReadProcessMemoryAux(m_TargetBase, Size, m_CurrentEProcess, &pMemItp);

			if (pMemItp){
				RtlCopyMemory(m_Buffer, pMemItp->mem.MemoryBlock, Size);
			}

			CHGDriver::GetInstance()->FreeItp(pMemItp);
			m_HexEditor.SetBuffer((PBYTE)m_Buffer, m_BufferLength, m_TargetBase);
		}

		return 0;
	}

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (m_Buffer && m_BufferLength && (m_bIsFile == FALSE)){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}
		EndDialog(wID);
		return 0;
	}

	LRESULT OnSearch(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		WCHAR MaskBuffer[MAX_PATH] = {0};
		int errorno = 0;
		m_mask.GetWindowTextW(MaskBuffer, MAX_PATH);
		if ((wcslen(MaskBuffer) < 2) || (wcslen(MaskBuffer) % 2 == 1))
		{
			::MessageBoxW(NULL, L"Please type hex characters!", L"Error", MB_OK);
			return 0;
		}
		BYTE ByteList[MAX_PATH] = {0};
		SIZE_T ByteCount = wcslen(MaskBuffer) / 2;
		for (SIZE_T i = 0; i < ByteCount * 2; i += 2)
		{
			WCHAR wByte[3] = {0};
			BYTE bByte = 0;
			wByte[0] = MaskBuffer[i];
			wByte[1] = MaskBuffer[i + 1];
			bByte = (BYTE)StrHexToULongA(CW2A(wByte), &errorno);
			ByteList[i / 2] = bByte;
		}
		
		PBYTE ByteAddress = m_Buffer;
		SzLogA("ByteCount = %d", ByteCount);
		for (SIZE_T j = 0; j < m_BufferLength - ByteCount; j ++)
		{
			if (memcmp(ByteAddress, ByteList, ByteCount) == 0)
			{
				CString strAddress;
				strAddress.Format(L"%p", ((SIZE_T)ByteAddress - (SIZE_T)m_Buffer + m_TargetBase));
				SzLogA("%p: %04X", ((SIZE_T)ByteAddress - (SIZE_T)m_Buffer + m_TargetBase), *(PDWORD)ByteAddress);
				m_AddressList.AddString(strAddress);
				ByteAddress += ByteCount;
			}
			else
				ByteAddress ++;
		}
		// First DDX call, hooks up variables to controls.
		DoDataExchange(false);

		return 0;
	}

	LRESULT OnFile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		if (m_Buffer && m_BufferLength && (m_bIsFile == FALSE)){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}

		m_bIsFile = TRUE;

		//TCHAR szPath[MAX_PATH];
		//::GetTempPath(MAX_PATH, szPath);
		LPCTSTR lpcstrFilter = _T("All Files (*.*)\0*.*\0")
			_T("");
		// Open file
		CFileDialog dlg(TRUE, _T("dat"), _T(""), OFN_EXPLORER | OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, lpcstrFilter);
		//dlg.m_ofn.lpstrInitialDir = szPath;
		if( dlg.DoModal() != IDOK ) 
			return 0;

		// Transfer data from the controls to member variables.
		if ( !DoDataExchange(true) )        
			return 0;//...
		if ( m_nUnicode ){
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, FALSE);
		}else{
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, TRUE);
		}
		m_HexEditor.m_TargetBase = 0;

		m_HexEditor.SetFilename(dlg.m_ofn.lpstrFile);

#if defined(_WIN64)
		m_wndBase.SetWindowTextW(L"0000000000000000");
#else
		m_wndBase.SetWindowTextW(L"00000000");
#endif
		CString strTmp;
		strTmp.Format(L"%p", m_HexEditor.m_Size);
		m_wndSize.SetWindowTextW(strTmp);

		m_BufferLength = m_HexEditor.m_Size;
		m_Buffer = m_HexEditor.m_Base;

		return 0;
	}

	LRESULT OnSave(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	{
		LPCTSTR lpcstrFilter = _T("All Files (*.*)\0*.*\0");
		WCHAR FileName[MAX_PATH] = {0};
		m_HostName.Replace(L".", L"_");
		wcscpy_s(FileName, m_HostName);
		wcscat_s(FileName, L"_");
		CString tmpBase;
		tmpBase.Format(L"%p", m_TargetBase);
		wcscat_s(FileName, tmpBase);
		wcscat_s(FileName, L"_");
		wcscat_s(FileName, GetCurrentDatetimeString());
		// Open file
		CFileDialog dlg(FALSE, L"mem", FileName, 0, lpcstrFilter);
		//dlg.m_ofn.lpstrInitialDir = szPath;
		if( dlg.DoModal() != IDOK ){
			return 0;
		}

		WCHAR SizeBuffer[MAX_PATH] = {0};
		m_wndSize.GetWindowTextW(SizeBuffer, MAX_PATH); 

		// Transfer data from the controls to member variables.
		if ( !DoDataExchange(true) )        
			return 0;//...
		if ( m_nUnicode ){
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, FALSE);
		}else{
			m_HexEditor.SetDisplayOptions(TRUE, TRUE, TRUE);
		}

		int errorno = 0;
		DWORD Size = 0;
		Size = StrHexToULongA(CW2A(SizeBuffer), &errorno);

		FILE* fp;
		_wfopen_s(&fp, dlg.m_ofn.lpstrFile, L"wb");
		if(fp == NULL){
			AtlMessageBox(m_hWnd, L"_wfopen", L"Info", MB_ICONWARNING);
			return 0;
		}
		fwrite(m_Buffer, 1, Size, fp);
		fclose(fp);
		return 0;
	}

	LRESULT OnClose(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if (m_Buffer && m_BufferLength && (m_bIsFile == FALSE)){
			ZeroMemory(m_Buffer, m_BufferLength);
			free(m_Buffer);
			m_Buffer = NULL;
			m_BufferLength = 0;
		}
		int nRetCode = 0;
		EndDialog(nRetCode);
		return 0;
	}
};
