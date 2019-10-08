#if !defined(AFX_HEXEDITOR_H__20030608_F1CD_9419_8871_0080AD509054__INCLUDED_)
#define AFX_HEXEDITOR_H__20030608_F1CD_9419_8871_0080AD509054__INCLUDED_

/////////////////////////////////////////////////////////////////////////////
// Hex Editor - A hex editor control
//
// Written by Bjarke Viksoe (bjarke@viksoe.dk)
// Copyright (c) 2004 Bjarke Viksoe.
//
// This code may be used in compiled form in any way you desire. This
// source file may be redistributed by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included. 
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability if it causes any damage to you or your
// computer whatsoever. It's free, so don't hassle me about it.
//
// Beware of bugs.
//

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// For WM_MOUSEWHEEL support
#include <zmouse.h>


// The memory mapped file wrapper
class CMapFile
{
public:
   CFile f;
   HANDLE m_hMapFile;
   SIZE_T m_dwSize;
   LPBYTE m_p;

   CMapFile() : m_hMapFile(NULL), m_p(NULL), m_dwSize(0UL)
   {
   }
   ~CMapFile()
   {
      Close();
   }

   bool Open(LPCTSTR pstrFilename)
   {
      Close();
      if( !f.Open(pstrFilename, GENERIC_READ | GENERIC_WRITE) ) return false;
      m_dwSize = f.GetSize();
      m_hMapFile = ::CreateFileMapping(f, NULL, PAGE_READWRITE, 0, 0, NULL);
      if( m_hMapFile == NULL ) return false;
      m_p = (LPBYTE) ::MapViewOfFile(m_hMapFile, SECTION_ALL_ACCESS, 0, 0, 0);
      if( m_p == NULL ) return false;
      return true;
   }
   void Close()
   {
      if( m_p ) ::UnmapViewOfFile(m_p);
      m_p = NULL;
      m_dwSize = 0;
      if( m_hMapFile ) ::CloseHandle(m_hMapFile);
      m_hMapFile = NULL;
      f.Close();
   }
   inline BOOL IsOpen() const
   {
      return m_hMapFile != NULL;
   }
   inline LPBYTE GetData() const
   {
      return (LPBYTE) m_p;
   }
   inline SIZE_T GetSize() const
   {
      return m_dwSize;
   }
};


class CHexEditorCtrl : 
   public CWindowImpl<CHexEditorCtrl>,
   public COffscreenDraw<CHexEditorCtrl>
{
public:
   DECLARE_WND_CLASS_EX(_T("WTL_HexEditor"), CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS, COLOR_WINDOW + 1)

   enum { BYTES_PR_LINE = 16 };

   CHexEditorCtrl();
   virtual ~CHexEditorCtrl();

   typedef struct {
      SIZE_T dwPos;
      BYTE bValue;
   } UNDOENTRY;

   CString m_sFilename;                    // Filename of displayed file
   CMapFile m_File;                        // Memory mapped file image
   SIZE_T m_dwPos;                          // File Position of top of view
   CFont m_fontEditor;                     // Font used in Editor
   TEXTMETRIC m_tmEditor;                  // Font text metrics
   bool  m_bShowAddress;                   // Show address dump?
   bool  m_bShowData;                      // Show Hex dump?
   bool  m_bShowAscii;                     // Show ASCII dump?
   TCHAR m_cInvalidAscii;                  // Character to use when char is unprintable
   BYTE  m_iDataSize;                      // Size of address (1, 2, 4)
   RECT  m_rcData;                         // Rectangle of data (hex) area
   RECT  m_rcAscii;                        // Rectangle of ASCII area
   SIZE_T m_dwSelStart;                     // Selection start
   SIZE_T m_dwSelEnd;                       // Selection end
   SIZE_T m_dwDigitOffset;                  // Cursor digit-position inside value
   bool  m_bInDataPane;                    // Selection is in data pane!
   SIZE  m_szMargin;                       // Left/top margin
   BOOL  m_bReadOnly;                      // Is control read-only?
   CSimpleArray<UNDOENTRY> m_aUndostack;   // Undo stack

   BOOL m_bIsFile;
   PBYTE m_Base;
   SIZE_T m_Size;
   SIZE_T m_TargetBase;


   // Management

   BOOL PreTranslateMessage(MSG* pMsg);

   // Operations

   BOOL SetFilename(LPCTSTR pstrFilename);
   BOOL SetBuffer(PBYTE pBuffer, SIZE_T dwSize, SIZE_T TargetBase);
   BOOL GetModify() const;
   int GetLineHeight() const;
   int GetLinesPrPage() const;
   int GetDataSize() const;
   void SetDataSize(int iSize);
   void SetInvalidChar(TCHAR ch);
   void SetReadOnly(BOOL bReadOnly = TRUE);
   BOOL GetReadOnly() const;
   void SetDisplayOptions(BOOL bShowAddress, BOOL bShowData, BOOL bShowAscii);
   BOOL SetSel(SIZE_T dwStart, SIZE_T dwEnd, BOOL bNoScroll = FALSE);
   void GetSel(SIZE_T& dwStart, SIZE_T& dwEnd) const;
   SIZE GetMargins() const;
   void SetMargins(SIZE szMargin);
   BOOL Undo();

   BOOL CanUndo() const;
   BOOL CanCopy() const;

   // Implementation

   void RecalcCaret();
   void RecalcPosition(SIZE_T dwPos);
   void AssignDigitValue(SIZE_T& dwPos, SIZE_T& dwCursorPos, BYTE bValue);
   void AssignCharValue(SIZE_T& dwPos, SIZE_T& dwCursorPos, BYTE bValue);
   BOOL GetPosFromPoint(POINT pt, SIZE_T& dwPos, bool& bDataActive);

   ATLINLINE TCHAR toupper(WPARAM c) const { return (TCHAR)( c - 'a' + 'A' ); };
   ATLINLINE bool isdigit(WPARAM c) const { return c >= '0' && c <= '9'; };
   //ATLINLINE bool isprint(TCHAR c) const { WORD w; ::GetStringTypeA(LOCALE_USER_DEFAULT, CT_CTYPE1, &c, 1, &w); return w != C1_CNTRL; };
   ATLINLINE bool isprint(WCHAR c) const { WORD w; ::GetStringTypeW(CT_CTYPE1, &c, 1, &w); return w != C1_CNTRL; };

   void DoPaint(CDCHandle dc);

   // Message map and handlers

   BEGIN_MSG_MAP(CHexEditorCtrl)
      MESSAGE_HANDLER(WM_CREATE, OnCreate)
      MESSAGE_HANDLER(WM_SIZE, OnSize)
      MESSAGE_HANDLER(WM_VSCROLL, OnVScroll)
      MESSAGE_HANDLER(WM_MOUSEWHEEL, OnMouseWheel)
      MESSAGE_HANDLER(WM_SETFOCUS, OnSetFocus)
      MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
      MESSAGE_HANDLER(WM_KILLFOCUS, OnKillFocus)
      MESSAGE_HANDLER(WM_KEYDOWN, OnKeyDown)
      MESSAGE_HANDLER(WM_CHAR, OnChar)
      MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
      MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
      MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDblClk)
      MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
      COMMAND_ID_HANDLER(ID_EDIT_COPY, OnEditCopy)
      COMMAND_ID_HANDLER(ID_EDIT_UNDO, OnEditUndo)
      COMMAND_ID_HANDLER(ID_NEXT_PANE, OnNextPane)
      CHAIN_MSG_MAP( COffscreenDraw<CHexEditorCtrl> )
   END_MSG_MAP()

   LRESULT OnCreate(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnPaint(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnVScroll(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
   LRESULT OnSetFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnKillFocus(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnChar(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);      
   LRESULT OnLButtonDblClk(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnLButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnKeyDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);   
   LRESULT OnEditCopy(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnEditUndo(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
   LRESULT OnNextPane(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
};


#endif // !defined(AFX_HEXEDITOR_H__20030608_F1CD_9419_8871_0080AD509054__INCLUDED_)
