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

#ifndef __LVHOOK_H__
#define __LVHOOK_H__

#include "stdafx.h"
#include "HGDriver.h"
#include "LVDriver.h"

#include <TlHelp32.h>

/************************************************************************/
/* Define process info structure                                        */
/************************************************************************/
#define R3_HOOK_SDT			0
#define R3_HOOK_SHADOW_SDT	1

#define SDT_OTHER			0xFFFF0000
#define SDT_INLINE_HOOK		0xFFFF0001
#define SDT_SDT_HOOK		0xFFFF0002
#define SDT_SDT_INLINE_HOOK	0xFFFF0003

typedef struct _HG_SDTINFO: public CSortListViewItem<_HG_SDTINFO>{
	SIZE_T		m_Ordinal;
	WCHAR		m_Name[64];
	SIZE_T		m_CurrentAddress;
	SIZE_T		m_HookType;
	SIZE_T		m_AddressOfHook;
	SIZE_T		m_OffsetOfHook;
	SIZE_T		m_OriginalAddress;
	WCHAR		m_ModulePath[SZ_PATH_LEN];
	COLORREF	m_RGB;

	_HG_SDTINFO()
		: m_Ordinal(0)
		, m_CurrentAddress(0)
		, m_HookType(0)
		, m_AddressOfHook(0)
		, m_OffsetOfHook(0)
		, m_OriginalAddress(0)
		, m_RGB(RGB(255, 0, 0))
	{
		ZeroMemory(m_ModulePath, sizeof(WCHAR) * SZ_PATH_LEN);
	}

	_HG_SDTINFO(
		SIZE_T		Ordinal, 
		PWCHAR		Name,
		SIZE_T		CurrentAddress,
		SIZE_T		HookType,
		SIZE_T		AddressOfHook,
		SIZE_T		OffsetOfHook,
		SIZE_T		OriginalAddress,
		PWCHAR		ModulePath,
		COLORREF	rgb)
		: m_Ordinal(Ordinal)
		, m_CurrentAddress(CurrentAddress)
		, m_HookType(HookType)
		, m_AddressOfHook(AddressOfHook)
		, m_OffsetOfHook(OffsetOfHook)
		, m_OriginalAddress(OriginalAddress)
		, m_RGB(rgb)
	{
		wcscpy_s(m_Name, Name);
		wcscpy_s(m_ModulePath, ModulePath);
	}

	int Compare(_HG_SDTINFO* arg0, int nField){
		switch (nField){
		case 0:
			return m_Ordinal > arg0->m_Ordinal ? 1 : (m_Ordinal > arg0->m_Ordinal ? -1 : 0);
		case 1:
			return _tcsicmp(m_Name, arg0->m_Name);
		case 2:	// 
			return m_CurrentAddress > arg0->m_CurrentAddress ? 1 : (m_CurrentAddress > arg0->m_CurrentAddress ? -1 : 0);
		case 3:	// 
			return m_HookType > arg0->m_HookType ? 1 : (m_HookType > arg0->m_HookType ? 0 : -1);
		case 4:	// 
			return m_OriginalAddress > arg0->m_OriginalAddress ? 1 : (m_OriginalAddress > arg0->m_OriginalAddress ? -1 : 0);
		case 5:	// access
			return _tcsicmp(m_ModulePath, arg0->m_ModulePath);
		}
		return 0;
	}
}HG_SDTINFO, *PHG_SDTINFO;

/************************************************************************/
/* CLVHook                                                           */
/************************************************************************/
struct CLVHook{
	CLVHook()
		: m_pView(NULL){}

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

		switch (GetWinVer())
		{
		case eWin_XP:
			m_ShadowSDTName = WinXPShadowSDTName;
			break;
		case eWin_7:
			m_ShadowSDTName = Win7ShadowSDTName;
			break;
		case eWin_8:
			m_ShadowSDTName = Win8ShadowSDTName;
			break;
		}
	}

	static CLVHook* GetInstance(){
		if (_instance == NULL){
			_instance = new CLVHook();
		}

		return _instance;
	}

	HRESULT DisplaySDT(SIZE_T Type){
		HRESULT hr = S_OK;

		// Check m_top_tab
		if (m_top_tab == NULL || m_bottom_tab == NULL){
			return S_FALSE;
		}
		m_horizontal_splitter->SetSinglePaneMode(SPLIT_PANE_TOP);
		// 
// 		for (int i = 0; i < m_top_tab->GetPageCount(); i++){
// 			if (_tcscmp(ptszTabProcess[eEnglish], m_top_tab->GetPageTitle(i)) == 0){
// 				m_top_tab->RemovePage(i);
// 				break;
// 			}
// 		}
		
		// Create list view
		m_pView = new CHackerGuardianView<HG_SDTINFO>;

		m_pView->Create(m_top_tab->m_hWnd, CWindow::rcDefault, NULL, WS_CHILD | LVS_REPORT | LVS_ALIGNLEFT | WS_BORDER | WS_TABSTOP, 0);

		//
		m_pView->SetTab(m_top_tab, m_bottom_tab, m_vertical_splitter, m_horizontal_splitter);

//		LONG lStyle;
//		lStyle = GetWindowLong(m_pView->m_hWnd, GWL_STYLE);
//		lStyle &= ~LVS_TYPEMASK;
//		lStyle |= LVS_REPORT; 
//		::SetWindowLong(m_pView->m_hWnd, GWL_STYLE, lStyle);

		DWORD dwStyle = m_pView->GetExtendedListViewStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;
		dwStyle |= LVS_EX_GRIDLINES;
//		dwStyle |= LVS_EX_CHECKBOXES;
		m_pView->SetExtendedListViewStyle(dwStyle);

		//
		switch (Type){
		case R3_HOOK_SDT:
			m_top_tab->AddPage(m_pView->m_hWnd, pwszTabSdt[eEnglish]);
			m_pView->SetTabId(eHookSSDT);
			break;
		case R3_HOOK_SHADOW_SDT:
			m_top_tab->AddPage(m_pView->m_hWnd, pwszTabShadowSdt[eEnglish]);
			m_pView->SetTabId(eHookShadowSSDT);
			break;
		}

		// Clear
		m_pView->DeleteAllItems();

		m_pView->InsertColumn(0, pwszTabSdtColumn[eEnglish][0], LVCFMT_LEFT, 60);
		m_pView->InsertColumn(1, pwszTabSdtColumn[eEnglish][1], LVCFMT_LEFT, 200);
#if defined(_WIN64)
		m_pView->InsertColumn(2, pwszTabSdtColumn[eEnglish][2], LVCFMT_LEFT, 150);
#else
		m_pView->InsertColumn(2, pwszTabSdtColumn[eEnglish][2], LVCFMT_LEFT, 110);
#endif
		m_pView->InsertColumn(3, pwszTabSdtColumn[eEnglish][3], LVCFMT_CENTER, 90);
#if defined(_WIN64)
		m_pView->InsertColumn(4, pwszTabSdtColumn[eEnglish][4], LVCFMT_LEFT, 150);
#else
		m_pView->InsertColumn(4, pwszTabSdtColumn[eEnglish][4], LVCFMT_LEFT, 110);
#endif
		m_pView->InsertColumn(5, pwszTabSdtColumn[eEnglish][5], LVCFMT_LEFT, 360);

		//
 		//
		//
		switch (Type){
		case R3_HOOK_SDT:
			EnumSSDTR0();
			break;
		case R3_HOOK_SHADOW_SDT:
			EnumShadowSSDTR0();
			break;
		}

		return hr;
	}

	HRESULT InsertDriverItem(PHG_SDTINFO arg0){
		HRESULT hr = S_OK;
		int nRow = 0;
		CString strCurrentAddress;
		CString strOriginalAddress;
		CString strOffsetOfAddress;

		if (arg0 == NULL){
			return S_FALSE;
		}
		nRow = m_pView->InsertItem(m_pView->GetItemCount(), size_tToCString(arg0->m_Ordinal));
		m_pView->SetItemText(nRow, 1, arg0->m_Name);
		if (arg0->m_HookType == SDT_SDT_HOOK)
		{
			strCurrentAddress = size_tToHexCString(arg0->m_CurrentAddress);
			strOriginalAddress = size_tToHexCString(arg0->m_OriginalAddress);
			m_pView->SetItemText(nRow, 2, strCurrentAddress);
			m_pView->SetItemText(nRow, 3, L"SDT Hook");
			m_pView->SetItemText(nRow, 4, strOriginalAddress);
		}
		else if (arg0->m_HookType == SDT_INLINE_HOOK)
		{
			strCurrentAddress = size_tToHexCString(arg0->m_CurrentAddress);
			strOriginalAddress = size_tToHexCString(arg0->m_OriginalAddress);
			if (arg0->m_AddressOfHook)
			{
				strOriginalAddress += L" -> ";
				strOffsetOfAddress.Format(L"[+%04X]", arg0->m_OffsetOfHook);
				strOriginalAddress += strOffsetOfAddress;
				strOriginalAddress += size_tToHexCString(arg0->m_AddressOfHook);
			}
			m_pView->SetItemText(nRow, 2, strCurrentAddress);
			m_pView->SetItemText(nRow, 3, L"Inline Hook");
			m_pView->SetItemText(nRow, 4, strOriginalAddress);
		}
		else if (arg0->m_HookType == SDT_SDT_INLINE_HOOK)
		{
			strCurrentAddress = size_tToHexCString(arg0->m_CurrentAddress);
			strOriginalAddress = size_tToHexCString(arg0->m_OriginalAddress);
			if (arg0->m_AddressOfHook)
			{
				strOriginalAddress += L" -> ";
				strOffsetOfAddress.Format(L"[+%04X]", arg0->m_OffsetOfHook);
				strOriginalAddress += strOffsetOfAddress;
				strOriginalAddress += size_tToHexCString(arg0->m_AddressOfHook);
			}
			m_pView->SetItemText(nRow, 2, strCurrentAddress);
			m_pView->SetItemText(nRow, 3, L"SDT & Inline");
			m_pView->SetItemText(nRow, 4, strOriginalAddress);
		}
		else
		{
			strCurrentAddress = size_tToHexCString(arg0->m_CurrentAddress);
			strOriginalAddress = size_tToHexCString(arg0->m_OriginalAddress);
			m_pView->SetItemText(nRow, 2, strCurrentAddress);
			m_pView->SetItemText(nRow, 3, L" - ");
			m_pView->SetItemText(nRow, 4, strOriginalAddress);
		}
		m_pView->SetItemText(nRow, 5, arg0->m_ModulePath);
		m_pView->SetItemData(nRow, (DWORD_PTR)arg0);

		return hr;
	}

	CString GetDriverPathByAddress(std::vector<HG_DRIVERINFO>& DriverVector, SIZE_T Address)
	{
		CString strRtn;
		size_t DriverVectorSize = DriverVector.size();

		for (size_t j = 0; j < DriverVectorSize; j ++)
		{
			if (Address >= DriverVector[j].m_nBase &&
				Address <= DriverVector[j].m_nBase + DriverVector[j].m_nSize)
			{
				strRtn = DriverVector[j].m_pwszPath;
				break;
			}
		}

		return strRtn;
	}

	void EnumSSDTR0()
	{
		PITP pSSDTItp = NULL;
		CHGDriver::GetInstance()->HookSSDT(&pSSDTItp);
		SIZE_T i = 0;
		SIZE_T HookType = 0;
		SIZE_T AddressOfHook = 0;
		SIZE_T OffsetOfHook = 0;
		PHG_SDTINFO pSdt = NULL;
		COLORREF rgb = RGB(255, 0, 0);
		std::vector<HG_DRIVERINFO> DriverVector = EnumDriverR0();
		CString strPath;
		CString strNtoskrnl;
		SIZE_T CurrentAddress = NULL;

		if (pSSDTItp != NULL){
			for (i = 0; i < pSSDTItp->sdt.nCount; i++)
			{
				strPath = "";
				HookType = 0;
				AddressOfHook = 0;
				rgb = RGB(0, 0, 0);
				if (pSSDTItp->sdt.sdt[i].AddressOfHook && pSSDTItp->sdt.sdt[i].CurrentAddress != pSSDTItp->sdt.sdt[i].OriginalAddress)
				{
					rgb = RGB(255, 0, 0);
					HookType = SDT_SDT_INLINE_HOOK;
					strPath = L"[SDT]: ";
					strPath += GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress);
					strPath += L"; [Inline]: ";
					strPath += GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].AddressOfHook);
					CurrentAddress = (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress;
					AddressOfHook = (SIZE_T)pSSDTItp->sdt.sdt[i].AddressOfHook;
					OffsetOfHook = pSSDTItp->sdt.sdt[i].OffsetOfHook;
				}
				else if (pSSDTItp->sdt.sdt[i].AddressOfHook)
				{
					//SzLogA("Inline === %d", pSSDTItp->sdt.sdt[i].Hint);
					rgb = RGB(255, 0, 0);
					HookType = SDT_INLINE_HOOK;
					strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].AddressOfHook);
					CurrentAddress = (SIZE_T)pSSDTItp->sdt.sdt[i].AddressOfHook;
					AddressOfHook = (SIZE_T)pSSDTItp->sdt.sdt[i].AddressOfHook;
					OffsetOfHook = pSSDTItp->sdt.sdt[i].OffsetOfHook;

					if (GetFileCompany(strPath).Find(TEXT("Microsoft")) >= 0){
						rgb = RGB(0, 0, 0);
						if (strNtoskrnl.GetLength() == 0)
						{
							strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress);
							strNtoskrnl = strPath;
						}
						else
						{
							strPath = strNtoskrnl;
						}
						CurrentAddress = (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress;
						AddressOfHook = 0;
						OffsetOfHook = 0;
						HookType = SDT_OTHER;
					}
				}
				else if (pSSDTItp->sdt.sdt[i].CurrentAddress != pSSDTItp->sdt.sdt[i].OriginalAddress)
				{
					//SzLogA("SDT === %d", pSSDTItp->sdt.sdt[i].Hint);
					rgb = RGB(255, 0, 0);
					HookType = SDT_SDT_HOOK;
					strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress);
					CurrentAddress = (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress;
					AddressOfHook = 0;
					OffsetOfHook = 0;
				}
				else
				{
					//SzLogA("Other === %d", pSSDTItp->sdt.sdt[i].Hint);
					rgb = RGB(0, 0, 0);
					if (strNtoskrnl.GetLength() == 0)
					{
						strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress);
						strNtoskrnl = strPath;
					}
					else
					{
						strPath = strNtoskrnl;
					}
					CurrentAddress = (SIZE_T)pSSDTItp->sdt.sdt[i].CurrentAddress;
					HookType = SDT_OTHER;
					AddressOfHook = 0;
					OffsetOfHook = 0;
				}
				pSdt = new HG_SDTINFO(	pSSDTItp->sdt.sdt[i].Hint,
										pSSDTItp->sdt.sdt[i].Name,
										(SIZE_T)CurrentAddress,
										HookType,
										AddressOfHook,
										OffsetOfHook,
										(SIZE_T)pSSDTItp->sdt.sdt[i].OriginalAddress,
										strPath.GetBuffer(strPath.GetLength()),
										rgb);
				InsertDriverItem(pSdt);
			}
			CHGDriver::GetInstance()->FreeItp(pSSDTItp);
		}else{
			SzLogA("HookSSDT Error!");
		}
	}

	void EnumShadowSSDTR0()
	{
		PITP pShadowSSDTItp = NULL;
		CHGDriver::GetInstance()->HookShadowSSDT(&pShadowSSDTItp);
		SIZE_T i = 0;
		SIZE_T HookType = 0;
		SIZE_T AddressOfHook = 0;
		PHG_SDTINFO pSdt = NULL;
		COLORREF rgb = RGB(255, 0, 0);
		std::vector<HG_DRIVERINFO> DriverVector = EnumDriverR0();
		CString strPath;
		CString strNtoskrnl;
		SIZE_T CurrentAddress = 0;
		SIZE_T OffsetOfHook = 0;

		if (pShadowSSDTItp != NULL){
			for (i = 0; i < pShadowSSDTItp->shadow_sdt.nCount; i++)
			{
				CurrentAddress = 0;
				strPath = "";
				HookType = 0;
				AddressOfHook = 0;
				rgb = RGB(0, 0, 0);

				if (pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook && pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress != pShadowSSDTItp->shadow_sdt.shadow_sdt[i].OriginalAddress)
				{
					rgb = RGB(255, 0, 0);
					HookType = SDT_SDT_INLINE_HOOK;
					AddressOfHook = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook;
					strPath = L"[SDT]: ";
					strPath += GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress);
					strPath += L"; [Inline]: ";
					strPath += GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook);
					CurrentAddress = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress;
					OffsetOfHook = pShadowSSDTItp->shadow_sdt.shadow_sdt[i].OffsetOfHook;
				}
				else if (pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook)
				{
					//SzLogA("Inline === %d", pSSDTItp->shadow_sdt.shadow_sdt[i].Hint);
					rgb = RGB(255, 0, 0);
					HookType = SDT_INLINE_HOOK;
					AddressOfHook = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook;
					strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook);
					CurrentAddress = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].AddressOfHook;
					OffsetOfHook = pShadowSSDTItp->shadow_sdt.shadow_sdt[i].OffsetOfHook;

					if (GetFileCompany(strPath).Find(TEXT("Microsoft")) >= 0){
						rgb = RGB(0, 0, 0);
						if (strNtoskrnl.GetLength() == 0)
						{
							strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress);
							strNtoskrnl = strPath;
						}
						else
						{
							strPath = strNtoskrnl;
						}
						CurrentAddress = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress;
						AddressOfHook = 0;
						OffsetOfHook = 0;
						HookType = SDT_OTHER;
					}
				}
				else if (pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress != pShadowSSDTItp->shadow_sdt.shadow_sdt[i].OriginalAddress)
				{
					//SzLogA("SDT === %d", pSSDTItp->shadow_sdt.shadow_sdt[i].Hint);
					rgb = RGB(255, 0, 0);
					HookType = SDT_SDT_HOOK;
					strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress);
					CurrentAddress = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress;
					AddressOfHook = 0;
					OffsetOfHook = 0;
				}
				else
				{
					//SzLogA("Other === %d", pSSDTItp->shadow_sdt.shadow_sdt[i].Hint);
					rgb = RGB(0, 0, 0);
					if (strNtoskrnl.GetLength() == 0)
					{
						strPath = GetDriverPathByAddress(DriverVector, (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress);
						strNtoskrnl = strPath;
					}
					else
					{
						strPath = strNtoskrnl;
					}
					CurrentAddress = (SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].CurrentAddress;
					AddressOfHook = 0;
					OffsetOfHook = 0;
					HookType = SDT_OTHER;
				}
				pSdt = new HG_SDTINFO(	pShadowSSDTItp->shadow_sdt.shadow_sdt[i].Hint,
										m_ShadowSDTName[pShadowSSDTItp->shadow_sdt.shadow_sdt[i].Hint],
										CurrentAddress,
										HookType,
										AddressOfHook,
										OffsetOfHook,
										(SIZE_T)pShadowSSDTItp->shadow_sdt.shadow_sdt[i].OriginalAddress,
										strPath.GetBuffer(strPath.GetLength()),
										rgb);
				InsertDriverItem(pSdt);
			}
			CHGDriver::GetInstance()->FreeItp(pShadowSSDTItp);
		}else{
			SzLogA("HookSSDT Error!");
		}
	}

	std::vector<HG_DRIVERINFO> EnumDriverR0()
	{
		std::vector<HG_DRIVERINFO> DriverVector;
		PITP pItp = NULL;
		PITP pServicesKey = NULL;
		PHG_DRIVERINFO pDiTmp = NULL;
		COLORREF rgb = RGB(0, 0, 0);
		SIZE_T i = 0;
		BOOL IsExist = TRUE;

		CString strName;
		CString strPath;
		CString strCompany;
		std::map<std::wstring, std::wstring> mServices;
		std::wstring strServiceName;

		CHGDriver::GetInstance()->EnumerateDriver(&pItp);
		WCHAR ServiceKeyPath[] = L"\\Registry\\Machine\\SYSTEM\\ControlSet001\\Services";
		CHGDriver::GetInstance()->RegistryEnumerateKey(ServiceKeyPath, &pServicesKey);

		std::vector<std::wstring> vKey = ParseStringSequenceW(pServicesKey->out_registry.Keys, pServicesKey->out_registry.nMaxLength);
		std::wstring strValueKeyPath;
		for (std::vector<std::wstring>::iterator it = vKey.begin(); it != vKey.end(); it++)
		{
			strValueKeyPath.clear();
			strValueKeyPath.append(ServiceKeyPath);
			strValueKeyPath += L"\\";
			strValueKeyPath += *it;
			PITP pValueKeyItp = NULL;
			CHGDriver::GetInstance()->RegistryEnumerateValueKey(strValueKeyPath.c_str(), &pValueKeyItp);

			if (pValueKeyItp == NULL){
				continue;
			}
			PBYTE ValueKey = (PBYTE)pValueKeyItp->out_registry.Keys;
			SIZE_T nOffset = 0;
			while (TRUE)
			{
				SIZE_T nSize = *(PSIZE_T)(ValueKey + nOffset);
				nOffset += sizeof(SIZE_T);
				if (nSize == 0){
					break;
				}
				// PKEY_VALUE_FULL_INFORMATION
				// 0x14 - Name Offset(WCHAR)
				// 0x10 - NameLength Offset(ULONG)
				ULONG NameLength = *(PULONG)(ValueKey + nOffset + 0x10);
				std::wstring ValueKeyName((PWCHAR)(ValueKey + nOffset + 0x14));
				std::wstring ValueKeyImagePath((PWCHAR)(ValueKey + nOffset + 0x14) + NameLength / sizeof(WCHAR) + 1);
				nOffset += nSize;

				if (!_wcsicmp(L"ImagePath", ValueKeyName.c_str()))
				{
					mServices[*it] = FormatDosPath(ValueKeyImagePath.c_str());
					break;
				}
			}

			CHGDriver::GetInstance()->FreeItp(pValueKeyItp);
		}

		if (pItp != NULL){
			SzLog(TEXT("pItp->driver.nCount: %d"), pItp->driver.nCount);
			for (i = 0; i < pItp->driver.nCount; i++)
			{
				IsExist = TRUE;
				strName = pItp->driver.pDriver[i].DriverPath;
				strName = strName.Right(strName.GetLength() - strName.ReverseFind(TEXT('\\')) - 1);
				//strPath = FormatDosPath(pItp->driver.pDriver[i].DriverPath).GetBuffer(SZ_PATH_LEN);

				strCompany = GetFileCompany(pItp->driver.pDriver[i].DriverPath);
				if (strCompany.GetLength() == 0){
					strCompany = L"[N/A] The file doesn't exist.";
					IsExist = FALSE;
				}			
				if (strCompany.Find(TEXT("Microsoft")) >= 0){
					rgb = RGB(0, 0, 0);
				}else{
					rgb = RGB(0, 0, 255);
				}
				if (IsExist == FALSE){
					rgb = RGB(0, 255, 255);
				}
				for (std::map<std::wstring, std::wstring>::iterator it = mServices.begin(); it != mServices.end(); it++)
				{
					CString str1 = it->second.c_str();
					CString str2 = pItp->driver.pDriver[i].DriverPath;
					str1.MakeLower();
					str2.MakeLower();
					if (str1.Find(str2) >= 0)
					{
						strServiceName = it->first;
						break;
					}
				}
				pDiTmp = new HG_DRIVERINFO(strName.GetBuffer(strName.GetLength()),
					pItp->driver.pDriver[i].Base,
					pItp->driver.pDriver[i].Size,
					pItp->driver.pDriver[i].DriverObject,
					pItp->driver.pDriver[i].DriverPath,
					const_cast<PWCHAR>(strServiceName.c_str()), 
					pItp->driver.pDriver[i].LoadOrder, 
					strCompany.GetBuffer(strCompany.GetLength()),
					rgb);
				strServiceName.clear();
				DriverVector.push_back(*pDiTmp);
			}
			CHGDriver::GetInstance()->FreeItp(pServicesKey);
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVKernel.h].[EnumDriver]: ERROR: pItp == NULL");
		}
		return DriverVector;
	}

	void RefreshKernelNotify()
	{
		m_pView->DeleteAllItems();
		//
	}

	void BottomTabRemoveAllPage()
	{
		m_bottom_tab->RemoveAllPages();
	}

private:
	static CTabView*						m_top_tab;
	static CTabView*						m_bottom_tab;
	static CSplitterWindow*					m_vertical_splitter;
	static CHorSplitterWindow*				m_horizontal_splitter;
	static CStatusBarCtrl*					m_pStatus;
	CHackerGuardianView<HG_SDTINFO>*		m_pView;
	static WCHAR**							m_ShadowSDTName;

private:
	class CFreeInstance{
	public:
		~CFreeInstance(){
			if (CLVHook::_instance != NULL){
				if (CLVHook::_instance != NULL){
					delete CLVHook::_instance;
					CLVHook::_instance = NULL;
				}
			}
		}
	protected:
	private:
	};

private:
	static CLVHook*		_instance;
	static CFreeInstance	m_garbo;
};

#endif