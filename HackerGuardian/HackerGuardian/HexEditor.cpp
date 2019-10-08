
#include "StdAfx.h"
#include "resource.h"

#include "HexEditor.h"

#define ROUND(x) (x / m_iDataSize * m_iDataSize)


//////////////////////////////////////////////////////////////////////////////
// Constructor / destructor

CHexEditorCtrl::CHexEditorCtrl() : 
   m_dwPos(0), 
   m_bShowAddress(true),
   m_bShowData(true),
   m_bShowAscii(true),
   m_bInDataPane(true),
   m_iDataSize(1),
   m_dwDigitOffset(0),
   m_cInvalidAscii('.'),
   m_dwSelStart((SIZE_T)-1),
   m_dwSelEnd((SIZE_T)-1),
   m_bReadOnly(FALSE),
   m_Base(0),
   m_Size(0),
   m_bIsFile(TRUE),
   m_TargetBase(0)
{
   ::ZeroMemory(&m_rcData, sizeof(m_rcData));
   ::ZeroMemory(&m_rcAscii, sizeof(m_rcAscii));
   m_szMargin.cx = 3;
   m_szMargin.cy = 2;
}

CHexEditorCtrl::~CHexEditorCtrl()
{
}


//////////////////////////////////////////////////////////////////////////////
// Operations

BOOL CHexEditorCtrl::PreTranslateMessage(MSG* /*pMsg*/)
{
   return FALSE;
}

BOOL CHexEditorCtrl::SetFilename(LPCTSTR pstrFilename)
{
   ATLASSERT(::IsWindow(m_hWnd));
   m_File.Close();
   if( !m_File.Open(pstrFilename) ) return FALSE;
   m_sFilename = pstrFilename;
   m_aUndostack.RemoveAll();
   m_dwPos = 0;
   m_Size = m_File.GetSize();
   m_Base = m_File.GetData();
   m_bIsFile = TRUE;
   SetScrollPos(SB_VERT, 0);
   PostMessage(WM_SIZE);                    // Resize scrollbars
   PostMessage(WM_SETFOCUS);                // Create caret
   m_dwSelStart = m_dwSelEnd = (SIZE_T) -1;  // See comment in DoPaint() for explanation
   return Invalidate();
}

BOOL CHexEditorCtrl::SetBuffer(PBYTE pBuffer, SIZE_T dwSize, SIZE_T TargetBase)
{
	ATLASSERT(::IsWindow(m_hWnd));
	m_aUndostack.RemoveAll();
	m_dwPos = 0;
	m_Size = dwSize;
	m_Base = pBuffer;
	m_bIsFile = FALSE;
	m_TargetBase = TargetBase;
	SetScrollPos(SB_VERT, 0);
	PostMessage(WM_SIZE);                    // Resize scrollbars
	PostMessage(WM_SETFOCUS);                // Create caret
	m_dwSelStart = m_dwSelEnd = (SIZE_T) -1;  // See comment in DoPaint() for explanation
	return Invalidate();
}

BOOL CHexEditorCtrl::GetModify() const
{
   return CanUndo();
}

BOOL CHexEditorCtrl::Undo()
{
   if( m_aUndostack.GetSize() == 0 ) return FALSE;
   // Undo all changes to this position. This solves problems
   // where only one digit gets undone at a time.
   SIZE_T dwPos = ROUND(m_aUndostack[ m_aUndostack.GetSize() - 1 ].dwPos);
   while( m_aUndostack.GetSize() > 0 
          && ROUND(m_aUndostack[ m_aUndostack.GetSize() - 1 ].dwPos) == dwPos ) 
   {
      UNDOENTRY entry = m_aUndostack[ m_aUndostack.GetSize() - 1 ];
      m_aUndostack.RemoveAt( m_aUndostack.GetSize() - 1 );
      LPBYTE pData = m_Base;
      *(pData + entry.dwPos) = entry.bValue;
   }
   SetSel(dwPos, dwPos);
   return Invalidate();
}

BOOL CHexEditorCtrl::CanUndo() const
{
   return m_aUndostack.GetSize() > 0;
}

BOOL CHexEditorCtrl::CanCopy() const
{
   return m_dwSelStart != m_dwSelEnd;
}

BOOL CHexEditorCtrl::GetReadOnly() const
{
   return m_bReadOnly;
}

void CHexEditorCtrl::SetReadOnly(BOOL bReadOnly)
{
   ATLASSERT(::IsWindow(m_hWnd));
   m_bReadOnly = bReadOnly;
   Invalidate();
}

int CHexEditorCtrl::GetLineHeight() const
{
   ATLASSERT(::IsWindow(m_hWnd));
   return m_tmEditor.tmHeight;
}

int CHexEditorCtrl::GetLinesPrPage() const
{
   ATLASSERT(::IsWindow(m_hWnd));
   RECT rcClient;
   GetClientRect(&rcClient);
   return (rcClient.bottom - rcClient.top) / GetLineHeight();
}

void CHexEditorCtrl::SetDataSize(int iSize)
{
   ATLASSERT(::IsWindow(m_hWnd));
   ATLASSERT(iSize==1 || iSize==2 || iSize==4);   // BYTE / WORD / DWORD
   ATLASSERT((BYTES_PR_LINE % iSize)==0);         // Integral, please
   m_iDataSize = (BYTE) iSize;
   Invalidate();
}

int CHexEditorCtrl::GetDataSize() const
{
   return m_iDataSize;
}

void CHexEditorCtrl::SetInvalidChar(TCHAR ch)
{
   ATLASSERT(::IsWindow(m_hWnd));
   m_cInvalidAscii = ch;
   Invalidate();
}

SIZE CHexEditorCtrl::GetMargins() const
{
   return m_szMargin;
}

void CHexEditorCtrl::SetMargins(SIZE szMargin)
{
   ATLASSERT(::IsWindow(m_hWnd));
   m_szMargin = szMargin;
   Invalidate();
}

void CHexEditorCtrl::SetDisplayOptions(BOOL bShowAddress, BOOL bShowData, BOOL bShowAscii)
{
   ATLASSERT(::IsWindow(m_hWnd));
   m_bShowAddress = bShowAddress == TRUE;
   m_bShowData = bShowData == TRUE;
   m_bShowAscii = bShowAscii == TRUE;
   Invalidate();
}

BOOL CHexEditorCtrl::SetSel(SIZE_T dwStart, SIZE_T dwEnd, BOOL bNoScroll /*= FALSE*/)
{
   ATLASSERT(::IsWindow(m_hWnd));
   if( !m_File.IsOpen() && (m_bIsFile == TRUE)) return FALSE;
   if( dwEnd == (SIZE_T) -1 ) dwEnd = m_Size;
   if( dwStart == dwEnd && dwEnd >= m_Size ) dwEnd = m_Size - 1;
   if( dwStart != dwEnd && dwEnd > m_Size ) dwEnd = m_Size;
   if( dwStart >= m_Size ) dwStart = m_Size - 1;
   dwStart = ROUND(dwStart);
   dwEnd = ROUND(dwEnd);
   if( dwEnd == dwStart && m_dwSelStart != m_dwSelEnd ) ShowCaret();
   if( dwEnd != dwStart && m_dwSelStart == m_dwSelEnd ) HideCaret();
   m_dwSelStart = dwStart;
   m_dwSelEnd = dwEnd;
   m_dwDigitOffset = 0;
   if( !bNoScroll ) RecalcPosition(dwEnd);
   if( dwStart == dwEnd ) RecalcCaret();
   return Invalidate();
}

void CHexEditorCtrl::GetSel(SIZE_T& dwStart, SIZE_T& dwEnd) const
{
   dwStart = m_dwSelStart;
   dwEnd = m_dwSelEnd;
   if( dwStart > dwEnd) {        // Return values in normalized form
      SIZE_T dwTemp = dwStart;
      dwStart = dwEnd;
      dwEnd = dwTemp;
   }
}


//////////////////////////////////////////////////////////////////////////////
// Message handlers

LRESULT CHexEditorCtrl::OnCreate(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   CLogFont lf = AtlGetStockFont(OEM_FIXED_FONT);
   wcscpy_s(lf.lfFaceName, 32, _T("Lucida Console"));     // This is the font we prefer
   lf.lfPitchAndFamily = FIXED_PITCH;                // Make sure Windows finds a fixed-width font
   m_fontEditor.CreateFontIndirect(&lf);

   CClientDC dc = m_hWnd;
   HFONT hOldFont = dc.SelectFont(m_fontEditor);
   dc.GetTextMetrics(&m_tmEditor);
   dc.SelectFont(hOldFont);

   ModifyStyle(0, WS_VSCROLL);
   SetScrollPos(SB_VERT, 0, TRUE);

   return 0;
}

LRESULT CHexEditorCtrl::OnSetFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   if( m_File.IsOpen()  && (m_bIsFile == TRUE)) {
      CreateSolidCaret(2, m_tmEditor.tmHeight - 2);
      ShowCaret();
   }
   return 0;
}

LRESULT CHexEditorCtrl::OnKillFocus(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   HideCaret();
   DestroyCaret();
   return 0;
}

LRESULT CHexEditorCtrl::OnSetCursor(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   if( m_rcAscii.left == 0 && m_rcData.left == 0 ) return 0; // Not ready yet!
   // Determine what area the cursor is over and change
   // cursor shape...
   SIZE_T dwPos = GetMessagePos();
   POINT pt = { GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos) };
   ScreenToClient(&pt);
   SIZE_T dwDummy = 0;
   bool bDummy = false;
   ::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(GetPosFromPoint(pt, dwDummy, bDummy) ? IDC_IBEAM : IDC_ARROW)));
   return TRUE;
}

LRESULT CHexEditorCtrl::OnChar(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   if( m_bReadOnly ) {
      ::MessageBeep((UINT)-1);
      return 0;
   }
   if( wParam < VK_SPACE ) return 0;
   if( m_dwSelStart != m_dwSelEnd ) SetSel(m_dwSelStart, m_dwSelStart);
   if( m_bInDataPane ) {
      if( isdigit(wParam) || (toupper(wParam) >= 'A' && toupper(wParam) <= 'F') ) {
         BYTE b = (BYTE)( isdigit(wParam) ? wParam - '0' : toupper(wParam) - 'A' + 10 );
         AssignDigitValue(m_dwSelStart, m_dwDigitOffset, b);
      }
   }
   else {
      AssignCharValue(m_dwSelStart, m_dwDigitOffset, LOBYTE(wParam));
      if( HIBYTE(wParam) != 0 ) AssignCharValue(m_dwSelStart, m_dwDigitOffset, HIBYTE(wParam));
   }
   return 0;
}

LRESULT CHexEditorCtrl::OnKeyDown(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   if( GetCapture() == m_hWnd ) {
      ReleaseCapture();
      return 0;
   }
   SIZE_T dwPage = BYTES_PR_LINE * (SIZE_T) GetLinesPrPage();
   bool bCtrl = (::GetAsyncKeyState(VK_CONTROL) & 0x8000) != 0;
   bool bShift = (::GetAsyncKeyState(VK_SHIFT) & 0x8000) != 0;
   switch( wParam ) {
   case VK_F6:
      PostMessage(WM_COMMAND, MAKEWPARAM(ID_NEXT_PANE, 0));
      return 0;
   case VK_DELETE:
      if( m_bInDataPane && !m_bReadOnly ) {
         m_dwDigitOffset = 0;
         for( int i = 0; i < m_iDataSize * 2; i++ ) PostMessage(WM_CHAR, '0');
      }
      return 0;
   case VK_LEFT:
      if( m_dwSelEnd < m_iDataSize ) return 0;
      SetSel(bShift ? m_dwSelStart : m_dwSelEnd - m_iDataSize, m_dwSelEnd - m_iDataSize);
      return 0;
   case VK_RIGHT:
      if( m_dwSelStart + m_iDataSize > m_Size ) return 0;
      SetSel(bShift ? m_dwSelStart : m_dwSelEnd + m_iDataSize, m_dwSelEnd + m_iDataSize);
      return 0;
   case VK_UP:
      if( bCtrl ) return SendMessage(WM_VSCROLL, SB_LINEUP);
      if( m_dwSelEnd < BYTES_PR_LINE ) return 0;
      SetSel(bShift ? m_dwSelStart : m_dwSelEnd - BYTES_PR_LINE, m_dwSelEnd - BYTES_PR_LINE);
      return 0;
   case VK_DOWN:
      if( bCtrl ) return SendMessage(WM_VSCROLL, SB_LINEDOWN);
      if( m_dwSelStart + BYTES_PR_LINE > m_Size ) return 0;
      SetSel(bShift ? m_dwSelStart : m_dwSelEnd + BYTES_PR_LINE, m_dwSelEnd + BYTES_PR_LINE);
      return 0;
   case VK_HOME:
      if( bCtrl ) SetSel(bShift ? m_dwSelStart : 0, 0);
      else SetSel(bShift ? m_dwSelStart : m_dwSelEnd - (m_dwSelEnd % BYTES_PR_LINE), m_dwSelEnd - (m_dwSelEnd % BYTES_PR_LINE));
      return 0;
   case VK_END:
      if( bCtrl ) SetSel(bShift ? m_dwSelStart : m_Size - 1, m_Size - (bShift ? 0 : 1));
      else SetSel(bShift ? m_dwSelStart : (m_dwSelEnd | 0xF) + (bShift ? 1 : 0), (m_dwSelEnd | 0xF) + (bShift ? 1 : 0));
      return 0;
   case VK_PRIOR:
      if( bCtrl ) return SendMessage(WM_VSCROLL, SB_PAGEUP);
      if( m_dwSelEnd < dwPage ) SetSel(bShift ? m_dwSelStart : 0, 0);
      else SetSel(bShift ? m_dwSelStart : m_dwSelEnd - dwPage, m_dwSelEnd - dwPage);
      return 0;
   case VK_NEXT:
      if( bCtrl ) return SendMessage(WM_VSCROLL, SB_PAGEDOWN);
      SetSel(bShift ? m_dwSelStart : m_dwSelEnd + dwPage, m_dwSelEnd + dwPage);
      return 0;
   }
   return 0;
}

LRESULT CHexEditorCtrl::OnLButtonDblClk(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
   POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
   bool bInDataPane = false;
   SIZE_T dwPos = 0;
   if( !GetPosFromPoint(pt, dwPos, bInDataPane) ) return 0;
   m_bInDataPane = bInDataPane;
   SetSel(dwPos, dwPos + m_iDataSize);
   return 0;
}

LRESULT CHexEditorCtrl::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
   SetFocus();
   POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
   bool bInDataPane = false;
   SIZE_T dwPos = 0;
   if( !GetPosFromPoint(pt, dwPos, bInDataPane) ) return 0;
   m_bInDataPane = bInDataPane;
   SetSel(dwPos, dwPos);
   // If user is dragging the mouse, we'll initiate a selection...
   ClientToScreen(&pt);
   if( ::DragDetect(m_hWnd, pt) ) SetCapture();
   return 0;
}

LRESULT CHexEditorCtrl::OnLButtonUp(UINT /*uMsg*/, WPARAM wParam, LPARAM lParam, BOOL& /*bHandled*/)
{
   if( GetCapture() != m_hWnd ) return 0;
   SendMessage(WM_MOUSEMOVE, wParam, lParam);
   ReleaseCapture();
   return 0;
}

LRESULT CHexEditorCtrl::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{
   if( GetCapture() != m_hWnd ) return 0;
   POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
   // Mouse outside client rectangle? Let's scroll the view
   RECT rcClient;
   GetClientRect(&rcClient);
   if( pt.y < 0 && m_dwPos > 0 ) SendMessage(WM_VSCROLL, SB_LINEUP);
   if( pt.y > rcClient.bottom - rcClient.top ) SendMessage(WM_VSCROLL, SB_LINEDOWN);
   // Expand the selection if mouse is over a valid position?
   bool bInDataPane = false;
   SIZE_T dwPos = 0;
   if( !GetPosFromPoint(pt, dwPos, bInDataPane) ) return 0;
   if( m_bInDataPane != bInDataPane ) return 0;
   SetSel(m_dwSelStart, dwPos == 0 ? 0 : dwPos + m_iDataSize);
   return 0;
}

LRESULT CHexEditorCtrl::OnSize(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   SetScrollRange(SB_VERT, 0, (int) (m_Size / BYTES_PR_LINE) - GetLinesPrPage() + 1, TRUE);
   return 0;
}

LRESULT CHexEditorCtrl::OnVScroll(UINT /*uMsg*/, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
   SCROLLINFO si = { sizeof(SCROLLINFO), SIF_POS | SIF_RANGE | SIF_TRACKPOS };
   GetScrollInfo(SB_VERT, &si);
   SIZE_T nPos = m_dwPos / BYTES_PR_LINE;
   switch( LOWORD(wParam) ) {
   case SB_TOP:
      nPos = 0;
      break;
   case SB_BOTTOM:
      nPos = si.nMax;
      break;
   case SB_LINEUP:
      if( nPos > 0 ) nPos -= 1;
      break;
   case SB_LINEDOWN:
      nPos += 1;
      break;
   case SB_PAGEUP:
      if( (int)nPos > GetLinesPrPage() ) nPos -= GetLinesPrPage(); else nPos = 0;
      break;
   case SB_PAGEDOWN:
      nPos += GetLinesPrPage();
      break;
   case SB_THUMBTRACK:      
   case SB_THUMBPOSITION:
      nPos = si.nTrackPos;
      break;
   }
   if( (int)nPos < si.nMin ) nPos = si.nMin;
   if( (int)nPos > si.nMax ) nPos = si.nMax;
   if( nPos == si.nPos ) return 0;
   SetScrollPos(SB_VERT, (int)nPos, TRUE);
   m_dwPos = nPos * BYTES_PR_LINE;
   RecalcCaret();
   Invalidate();
   return 0;
}

LRESULT CHexEditorCtrl::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
#if (_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400) || defined(_WIN32_WCE)
   uMsg;
   int zDelta = (int) (short) HIWORD(wParam);
#else
   int zDelta = (uMsg == WM_MOUSEWHEEL) ? (int) (short) HIWORD(wParam) : (int) wParam;
#endif //!((_WIN32_WINNT >= 0x0400) || (_WIN32_WINDOWS > 0x0400) || defined(_WIN32_WCE))
   for( int i = 0; i < abs(zDelta / WHEEL_DELTA * 2); i++ ) SendMessage(WM_VSCROLL, zDelta > 0 ? SB_LINEUP : SB_LINEDOWN);
   return 0;
}

LRESULT CHexEditorCtrl::OnNextPane(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   m_bInDataPane = !m_bInDataPane;
   RecalcCaret();
   Invalidate();
   return 0;
}

LRESULT CHexEditorCtrl::OnEditUndo(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   Undo();
   return 0;
}

LRESULT CHexEditorCtrl::OnEditCopy(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
   SIZE_T dwStart = 0;
   SIZE_T dwEnd = 0;
   GetSel(dwStart, dwEnd);
   if( dwStart >= dwEnd ) return 0;
   if( dwEnd - dwStart > 0x10000 ) {
      if( IDNO == AtlMessageBox(m_hWnd, IDS_ERR_LARGECLIPBOARD, IDS_CAPTION_ERROR, MB_ICONQUESTION | MB_YESNO) ) return 0;
   }
   CWaitCursor cursor;
   if( !::OpenClipboard(m_hWnd) ) return 0;
   // Rescale string buffer
   CString sText;
   sText.GetBuffer((int)((dwEnd - dwStart) * 4));
   sText.ReleaseBuffer(0);
   // Generate text for clipboard
   LPBYTE pData = m_Base;
   if( m_bInDataPane ) {
      TCHAR szBuffer[32];
      SIZE_T nCount = 0;
      for( SIZE_T i = dwStart; i < dwEnd; i++ ) {
         ::wsprintf(szBuffer, _T("%02X "), (long) *(pData + i));
         sText += szBuffer;
         if( (++nCount % BYTES_PR_LINE) == 0 ) sText += _T("\r\n");
      }
   }
   else {
      for( SIZE_T i = dwStart; i < dwEnd; i++ ) {
         TCHAR ch = *(pData + i);
         ch = isprint(ch) ? ch : m_cInvalidAscii;
         sText += ch;
      }
   }
#ifdef _UNICODE
   LPSTR p = (LPSTR) malloc( (sText.GetLength() + 1) * 2);
   AtlW2AHelper(p, sText, sText.GetLength() + 1);
#else
   LPCSTR p = sText;
#endif
   ::EmptyClipboard();
   HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, strlen(p) + 1);
   LPSTR lptstrCopy = (LPSTR) GlobalLock(hglbCopy);
   strcpy_s(lptstrCopy,strlen(p) + 1, p); 
   GlobalUnlock(hglbCopy);
   ::SetClipboardData(CF_TEXT, lptstrCopy); 
   ::CloseClipboard(); 
#ifdef _UNICODE
   free(p);
#endif
   return 0;
}


//////////////////////////////////////////////////////////////////////////////
// Implementation

BOOL CHexEditorCtrl::GetPosFromPoint(POINT pt, SIZE_T& dwPos, bool& bInDataPane)
{
   ATLASSERT(::IsWindow(m_hWnd));
   ATLASSERT(m_rcData.left>0);   // Oops! Not initialized! Call UpdateWindow() or delay the call!!!
   // Get rectangles for columns. Expand them a bit so it's easier
   // to hit with the mouse...
   RECT rcData = m_rcData;
   RECT rcAscii = m_rcAscii;
   ::InflateRect(&rcData, 4, 0);
   ::InflateRect(&rcAscii, 4, 0);
   if( ::PtInRect(&rcData, pt) ) {
      ::OffsetRect(&rcData, -2, -2);
      int xpos = (pt.x - rcData.left) / (((m_iDataSize * 2) + 1) * m_tmEditor.tmAveCharWidth) * m_iDataSize;
      if( xpos < 0 ) xpos = 0;
      if( xpos > BYTES_PR_LINE - 1 ) xpos = BYTES_PR_LINE - 1;
      int ypos = (pt.y - rcData.top) / GetLineHeight();
      dwPos = m_dwPos + (SIZE_T) xpos + ((SIZE_T) ypos * BYTES_PR_LINE);
      bInDataPane = true;
      return TRUE;
   }
   if( ::PtInRect(&rcAscii, pt) ) {
      ::OffsetRect(&rcAscii, 4, 0);
      int xpos = (pt.x - rcAscii.left) / m_tmEditor.tmAveCharWidth;
      if( xpos < 0 ) xpos = 0;
      if( xpos > BYTES_PR_LINE - 1 ) xpos = BYTES_PR_LINE - 1;
      int ypos = (pt.y - rcAscii.top) / GetLineHeight();
      dwPos = m_dwPos + (SIZE_T) xpos + ((SIZE_T) ypos * BYTES_PR_LINE);
      bInDataPane = false;
      return TRUE;
   }
   return FALSE;
}

void CHexEditorCtrl::RecalcCaret()
{
   ATLASSERT(::IsWindow(m_hWnd));
   ATLASSERT(m_rcData.left>0 || m_rcAscii.left>0);   // Oops! Not initialized! Call UpdateWindow() or delay the call!!!
   // Selection-mode does not display a caret
   if( m_dwSelStart != m_dwSelEnd ) return;
   // We'll try to determine where to place the caret
   SIZE_T dwPos = m_dwSelStart;
   SIZE_T ypos = m_szMargin.cy + (((dwPos - m_dwPos) / BYTES_PR_LINE) * GetLineHeight());
   if( m_bInDataPane ) {
      int xpos = m_rcData.left;
      xpos += (dwPos % BYTES_PR_LINE) / m_iDataSize * m_tmEditor.tmAveCharWidth * ((m_iDataSize * 2) + 1);
      if( m_dwDigitOffset > 0 ) xpos += m_tmEditor.tmAveCharWidth * (int)m_dwDigitOffset;
      ::SetCaretPos((int)xpos, (int)ypos);
   }
   else {
      int xpos = m_rcAscii.left;
      xpos += ((dwPos % BYTES_PR_LINE) + ((int)m_dwDigitOffset / 2)) * m_tmEditor.tmAveCharWidth;
      ::SetCaretPos((int)xpos, (int)ypos);
   }
}

void CHexEditorCtrl::RecalcPosition(SIZE_T dwPos)
{
   // Is new selection-position out of bounds?
   // If so, we need to set a new view position.
   SIZE_T dwPage = (GetLinesPrPage() - 1) * BYTES_PR_LINE;
   if( dwPos < m_dwPos ) {
      m_dwPos = dwPos - (dwPos % BYTES_PR_LINE);
      SetScrollPos(SB_VERT, (int)m_dwPos / BYTES_PR_LINE);
      Invalidate();
   }
   else if( dwPos > m_dwPos + dwPage ) {
      m_dwPos = dwPos - (dwPos % BYTES_PR_LINE);
      if( m_dwPos >= dwPage ) m_dwPos -= dwPage;
      else m_dwPos = 0;
      SetScrollPos(SB_VERT, (int)m_dwPos / BYTES_PR_LINE);
      Invalidate();
   }
}

void CHexEditorCtrl::AssignDigitValue(SIZE_T& dwPos, SIZE_T& dwCursorPos, BYTE bValue)
{
   ATLASSERT(m_Base!=NULL);
   //ATLASSERT(dwPos<m_Size);
   ATLASSERT(bValue<0x10);
   // Calculate new data value (byte oriented)
   LPBYTE pData = m_Base;
   SIZE_T dwOffset = dwPos + (m_iDataSize - 1 - (dwCursorPos / 2));
   if( (dwCursorPos % 2) == 0 ) bValue = (BYTE) ((*(pData + dwOffset) & 0x0F) | (bValue << 4));
   else bValue = (BYTE) ((*(pData + dwOffset) & 0xF0) | bValue);
   // Create undo action
   UNDOENTRY undo = { dwOffset, *(pData + dwOffset) };
   m_aUndostack.Add(undo);
   // Assign value
   *(pData + dwOffset) = bValue;
   // Advance cursor (might need to only move the caret to next digit).
   SIZE_T dwTotalDigits = m_iDataSize * 2;
   if( ++m_dwDigitOffset >= dwTotalDigits ) SetSel(dwPos + m_iDataSize, dwPos + m_iDataSize);
   else RecalcCaret();
   Invalidate();
}

void CHexEditorCtrl::AssignCharValue(SIZE_T& dwPos, SIZE_T& dwCursorPos, BYTE bValue)
{
   ATLASSERT(m_Base!=NULL);
   //ATLASSERT(dwPos<m_Size);
   // Calculate new data value (cursor moves one digit; a byte i 2 digits)
   LPBYTE pData = m_Base;
   SIZE_T dwOffset = dwPos + (dwCursorPos / 2);
   // Create undo action
   UNDOENTRY undo = { dwOffset, *(pData + dwOffset) };
   m_aUndostack.Add(undo);
   // Assign value
   *(pData + dwOffset) = bValue;
   // Advance cursor (probably to next 'char' only)
   dwCursorPos += 2;
   SIZE_T dwTotalDigits = m_iDataSize * 2;
   if( dwCursorPos >= dwTotalDigits ) SetSel(dwPos + m_iDataSize, dwPos + m_iDataSize);
   else RecalcCaret();
   Invalidate();
}

void CHexEditorCtrl::DoPaint(CDCHandle dc)
{
   RECT rcClient;
   GetClientRect(&rcClient);

   dc.FillSolidRect(&rcClient, ::GetSysColor(COLOR_WINDOW));

   rcClient.left += m_szMargin.cx;
   rcClient.top += m_szMargin.cy;

   HFONT hOldFont = dc.SelectFont(m_fontEditor);
   int nLines = GetLinesPrPage() + 1;
   int iHeight = GetLineHeight();
   
   LPBYTE pData = m_Base;
   SIZE_T dwSize = m_Size;
   SIZE_T dwPos = m_dwPos;

   if (!m_Base)
   {
	   return;
   }

   ::ZeroMemory(&m_rcData, sizeof(m_rcData));
   ::ZeroMemory(&m_rcAscii, sizeof(m_rcAscii));
   m_rcData.top = m_rcAscii.top = m_szMargin.cy;

   COLORREF clrTextH, clrBackH;
   COLORREF clrTextN = ::GetSysColor(m_bReadOnly ? COLOR_GRAYTEXT : COLOR_WINDOWTEXT);
   COLORREF clrBackN = ::GetSysColor(COLOR_WINDOW);
   bool bHighlighted = false;

   dc.SetBkMode(OPAQUE);

   SIZE_T dwSelStart = 0;
   SIZE_T dwSelEnd = 0;
   GetSel(dwSelStart, dwSelEnd);

   int ypos = rcClient.top;
   TCHAR szBuffer[64] = { 0 };
   for( int i = 0; i < nLines; i++ ) {
      int xpos = rcClient.left;
      // Draw address text
      if( m_bShowAddress && dwPos < dwSize ) 
      {
         dc.SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
         dc.SetBkColor(clrBackN);

#if defined(_WIN64)
         ::wsprintf(szBuffer, _T("%016X  "), dwPos + m_TargetBase);
#else
		 ::wsprintf(szBuffer, _T("        %08X  "), dwPos + m_TargetBase);
#endif
         RECT rcAddress = { xpos, ypos, xpos + 200, ypos + iHeight };
         dc.DrawText(szBuffer, -1, &rcAddress, DT_SINGLELINE | DT_TOP | DT_LEFT | DT_NOCLIP);
         xpos += (int)(m_tmEditor.tmAveCharWidth * _tcslen(szBuffer));
      }
      // Draw hex values
      if( m_bShowData ) 
      {
         if( m_rcData.left == 0 ) m_rcData.left = xpos;

         clrBackH = ::GetSysColor(m_bInDataPane ? COLOR_HIGHLIGHT : COLOR_BTNFACE);
         clrTextH = ::GetSysColor(m_bInDataPane ? COLOR_HIGHLIGHTTEXT : COLOR_WINDOWTEXT);

         dc.SetTextColor(clrTextN);
         dc.SetBkColor(clrBackN);
         bHighlighted = false;

         for( SIZE_T j = 0; j < BYTES_PR_LINE; j += m_iDataSize ) {
            if( dwPos + j >= dwSize ) break;

            if( dwPos + j >= dwSelStart && dwPos + j < dwSelEnd ) {
               if( !bHighlighted ) {
                  dc.SetTextColor(clrTextH);
                  dc.SetBkColor(clrBackH);
                  bHighlighted = true;
               }
            }
            else {
               if( bHighlighted ) {
                  dc.SetTextColor(clrTextN);
                  dc.SetBkColor(clrBackN);
                  bHighlighted = false;
               }
            }

            LPTSTR p = szBuffer + (j == 0 ? 0 : 1);
            szBuffer[0] = ' ';
            switch( m_iDataSize ) {
            case 1:
               ::wsprintf( p, _T("%02X"), (long) *(pData + dwPos + j) );
               break;
            case 2:
               ::wsprintf( p, _T("%04X"), (long) *(LPWORD) (pData + dwPos + j) );
               break;
            case 4:
               ::wsprintf( p, _T("%08X"), (long) *(LPDWORD) (pData + dwPos + j) );
               break;
            default:
               ATLASSERT(false);
            }
            RECT rcData = { xpos, ypos, xpos + 200, ypos + iHeight };
            dc.DrawText(szBuffer, -1, &rcData, DT_SINGLELINE | DT_TOP | DT_LEFT | DT_NOCLIP);
            xpos += (int)(m_tmEditor.tmAveCharWidth * _tcslen(szBuffer));
         }

         if( m_rcData.right == 0 ) m_rcData.right = xpos;
      }
      // Draw ASCII representation
      if( m_bShowAscii )
      {
         xpos += m_tmEditor.tmAveCharWidth * 3;

         if( m_rcAscii.left == 0 ) m_rcAscii.left = xpos;
         xpos = m_rcAscii.left;

         clrBackH = ::GetSysColor(m_bInDataPane ? COLOR_BTNFACE : COLOR_HIGHLIGHT);
         clrTextH = ::GetSysColor(m_bInDataPane ? COLOR_WINDOWTEXT : COLOR_HIGHLIGHTTEXT);

         dc.SetTextColor(clrTextN);
         dc.SetBkColor(clrBackN);
         bHighlighted = false;

         SIZE_T j = 0;
         for( ; j < BYTES_PR_LINE; j++ ) {
            if( dwPos + j >= dwSize ) break;

            if( dwPos + j >= dwSelStart && dwPos + j < dwSelEnd ) {
               if( !bHighlighted ) {
                  dc.SetTextColor(clrTextH);
                  dc.SetBkColor(clrBackH);
                  bHighlighted = true;
               }
            }
            else {
               if( bHighlighted ) {
                  dc.SetTextColor(clrTextN);
                  dc.SetBkColor(clrBackN);
                  bHighlighted = false;
               }
            }

            TCHAR ch = *(pData + dwPos + j);
            ch = isprint(ch) ? ch : m_cInvalidAscii;
            RECT rcAscii = { xpos, ypos, xpos + 100, ypos + iHeight };
            dc.DrawText(&ch, 1, &rcAscii, DT_SINGLELINE | DT_TOP | DT_LEFT | DT_NOCLIP);
            xpos += m_tmEditor.tmAveCharWidth;
         }

         if( m_rcAscii.right == 0 ) m_rcAscii.right = xpos;
      }
	  else
	  {
		  xpos += m_tmEditor.tmAveCharWidth * 3;

		  if( m_rcAscii.left == 0 ) m_rcAscii.left = xpos;
		  xpos = m_rcAscii.left;

		  clrBackH = ::GetSysColor(m_bInDataPane ? COLOR_BTNFACE : COLOR_HIGHLIGHT);
		  clrTextH = ::GetSysColor(m_bInDataPane ? COLOR_WINDOWTEXT : COLOR_HIGHLIGHTTEXT);

		  dc.SetTextColor(clrTextN);
		  dc.SetBkColor(clrBackN);
		  bHighlighted = false;

		  SIZE_T j = 0;
		  for( ; j < BYTES_PR_LINE / 2; j++ ) {
			  if( dwPos + j >= dwSize ) break;

			  if( dwPos + j >= dwSelStart && dwPos + j < dwSelEnd ) {
				  if( !bHighlighted ) {
					  dc.SetTextColor(clrTextH);
					  dc.SetBkColor(clrBackH);
					  bHighlighted = true;
				  }
			  }
			  else {
				  if( bHighlighted ) {
					  dc.SetTextColor(clrTextN);
					  dc.SetBkColor(clrBackN);
					  bHighlighted = false;
				  }
			  }

			  TCHAR ch = *(pData + dwPos + j);
			  ch = isprint(ch) ? ch : m_cInvalidAscii;
			  RECT rcAscii = { xpos, ypos, xpos + 100, ypos + iHeight };
			  WCHAR wch = *(PWCHAR)(pData + dwPos + j);
			  dc.DrawText(&wch, 1, &rcAscii, DT_SINGLELINE | DT_TOP | DT_LEFT | DT_NOCLIP);
			  xpos += m_tmEditor.tmAveCharWidth;
		  }

		  if( m_rcAscii.right == 0 ) m_rcAscii.right = xpos;
	  }
      dwPos += BYTES_PR_LINE;
      ypos += iHeight;
   }

   dc.SelectFont(hOldFont);

   m_rcData.bottom = m_rcAscii.bottom = ypos;

   // HACK: Delayed activation of first caret position.
   //       We need the sizes of m_rcData and m_rcAscii before
   //       we can set selection (position caret)!
   if( m_dwSelStart == (SIZE_T) -1 ) SetSel(0, 0);
} 

