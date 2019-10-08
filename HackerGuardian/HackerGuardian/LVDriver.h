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

#ifndef __LVDRIVER_H__
#define __LVDRIVER_H__

#include "stdafx.h"
#include "HGDriver.h"

#include <TlHelp32.h>

/************************************************************************/
/* Define process info structure                                        */
/************************************************************************/
typedef struct _HG_DRIVERINFO: public CSortListViewItem<_HG_DRIVERINFO>{
	WCHAR		m_pwszName[MAX_PATH];
	SIZE_T		m_nBase;
	SIZE_T		m_nSize;
	SIZE_T		m_pDriverObject;
	WCHAR		m_pwszPath[SZ_PATH_LEN];
	WCHAR		m_pwszServiceName[MAX_PATH];
	SIZE_T		m_nLoadOrder;
	WCHAR		m_pwszCompany[MAX_PATH];
	COLORREF	m_RGB;

	_HG_DRIVERINFO()
		: m_nBase(0)
		, m_nSize(0)
		, m_pDriverObject(0)
		, m_nLoadOrder(0)
		, m_RGB(RGB(255, 0, 0))
	{
			memset(m_pwszName, 0, sizeof(WCHAR) * MAX_PATH);
			memset(m_pwszPath, 0, sizeof(WCHAR) * SZ_PATH_LEN);
			ZeroMemory(m_pwszServiceName, sizeof(WCHAR) * MAX_PATH);
			ZeroMemory(m_pwszCompany, sizeof(WCHAR) * MAX_PATH);
	}

	_HG_DRIVERINFO(
		PWCHAR		pwszName, 
		SIZE_T		nBase, 
		SIZE_T		nSize,
		SIZE_T		pDriverObject, 
		PWCHAR		pwszPath,
		PWCHAR		pwszServiceName,
		SIZE_T		nLoadOrder,
		PWCHAR		pwszCompany,
		COLORREF	rgb = RGB(255, 0, 0))
		: m_nBase(nBase)
		, m_nSize(nSize)
		, m_pDriverObject(pDriverObject)
		, m_nLoadOrder(nLoadOrder)
		, m_RGB(rgb)
	{
			wcscpy_s(m_pwszName, pwszName);
			wcscpy_s(m_pwszPath, pwszPath);
			wcscpy_s(m_pwszServiceName, pwszServiceName);
			wcscpy_s(m_pwszCompany, pwszCompany);
	}

	int Compare(_HG_DRIVERINFO* di, int nField){
		switch (nField){
		case 0:	// Compare only the filename - regardless of the path
			return m_nLoadOrder > di->m_nLoadOrder ? 1 : (m_nLoadOrder > di->m_nLoadOrder ? -1 : 0);
		case 1:	// Do you know any good comparison function for 64 bits integers???
			return _wcsicmp(m_pwszName, di->m_pwszName);
		case 2:	// 
			return m_nBase > di->m_nBase ? 1 : (m_nBase > di->m_nBase ? -1 : 0);
		case 3:	// 
			return m_nSize > di->m_nSize ? 1 : (m_nSize > di->m_nSize ? -1 : 0);
		case 4:	// access
			return m_pDriverObject > di->m_pDriverObject ? 1 : (m_pDriverObject > di->m_pDriverObject ? -1 : 0);
		case 5:	// access
			return _tcsicmp(m_pwszPath, di->m_pwszPath);
		case 6:	// 
			return _tcsicmp(m_pwszServiceName, di->m_pwszServiceName);
		case 7:	// access
			return _tcsicmp(m_pwszCompany, di->m_pwszCompany);
		}
		return 0;
	}
}HG_DRIVERINFO, *PHG_DRIVERINFO;

/************************************************************************/
/* CLVDriver                                                           */
/************************************************************************/
struct CLVDriver{
	CLVDriver()
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
	}

	static CLVDriver* GetInstance(){
		if (_instance == NULL){
			_instance = new CLVDriver();
		}

		return _instance;
	}

	HRESULT DisplayDriver(){
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
		m_pView = new CHackerGuardianView<HG_DRIVERINFO>;
		m_pView->SetTabId(eDriver);

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
		m_top_tab->AddPage(m_pView->m_hWnd, pwszTabDriver[eEnglish]);

		// Clear
		m_pView->DeleteAllItems();

		m_pView->InsertColumn(0, pwszTabDriverColumn[eEnglish][0], LVCFMT_RIGHT, 80);
		m_pView->InsertColumn(1, pwszTabDriverColumn[eEnglish][1], LVCFMT_LEFT, 100);//²åÈëÁÐ
#if defined(_WIN64)
		m_pView->InsertColumn(2, pwszTabDriverColumn[eEnglish][2], LVCFMT_LEFT, 150);
#else
		m_pView->InsertColumn(2, pwszTabDriverColumn[eEnglish][2], LVCFMT_LEFT, 90);
#endif
		m_pView->InsertColumn(3, pwszTabDriverColumn[eEnglish][3], LVCFMT_LEFT, 90);
#if defined(_WIN64)
		m_pView->InsertColumn(4, pwszTabDriverColumn[eEnglish][4], LVCFMT_LEFT, 150);
#else
		m_pView->InsertColumn(4, pwszTabDriverColumn[eEnglish][4], LVCFMT_LEFT, 90);
#endif
		m_pView->InsertColumn(5, pwszTabDriverColumn[eEnglish][5], LVCFMT_LEFT, 260);
		m_pView->InsertColumn(6, pwszTabDriverColumn[eEnglish][6], LVCFMT_LEFT, 100);
		m_pView->InsertColumn(7, pwszTabDriverColumn[eEnglish][7], LVCFMT_LEFT, 200);

		//
		// Traverse all processes.
		//
 		EnumDriver0();

		return hr;
	}

	HRESULT InsertDriverItem(PHG_DRIVERINFO di){
		HRESULT hr = S_OK;
		int nRow = 0;
		if (di == NULL){
			return S_FALSE;
		}
		CString strLoadOrder;
		strLoadOrder.Format(TEXT("%04X"), di->m_nLoadOrder);
		nRow = m_pView->InsertItem(m_pView->GetItemCount(), strLoadOrder);
		m_pView->SetItemText(nRow, 1, di->m_pwszName);
		m_pView->SetItemText(nRow, 2, size_tToHexCString(di->m_nBase));
		CString strSize;
		strSize.Format(TEXT("0x%08X"), di->m_nSize);
		m_pView->SetItemText(nRow, 3, strSize);
		m_pView->SetItemText(nRow, 4, di->m_pDriverObject == 0 ? "" : size_tToHexCString(di->m_pDriverObject));
		m_pView->SetItemText(nRow, 5, di->m_pwszPath);
		m_pView->SetItemText(nRow, 6, di->m_pwszServiceName);
		m_pView->SetItemText(nRow, 7, di->m_pwszCompany);
		m_pView->SetItemData(nRow, (DWORD_PTR)di);

		return hr;
	}

	void EnumDriver0()
	{
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

		std::deque<SIZE_T> queBase = EnumDriver3();

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
// 					SzLog(TEXT("Ring3 - : %s"), ValueKeyImagePath.c_str());
// 					SzLog(TEXT("Ring3 - : %s"), it->c_str());
// 					SzLog(TEXT("Ring3 - : %s"), FormatDosPath(ValueKeyImagePath.c_str()));
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
				//
				// if the process is hidden, display by red.
				// 
				if (queBase.end() != find(queBase.begin(), queBase.end(), pItp->driver.pDriver[i].Base)){
					if (strCompany.Find(TEXT("Microsoft")) >= 0){
						rgb = RGB(0, 0, 0);
					}else{
						rgb = RGB(0, 0, 255);
					}
				}else{
					rgb = RGB(255, 0, 0);
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
				InsertDriverItem(pDiTmp);
			}
			CHGDriver::GetInstance()->FreeItp(pServicesKey);
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVDriver.h].[EnumDriver]: ERROR: pItp == NULL");
		}
	}

	std::deque<SIZE_T> EnumDriver3(){
		std::deque<SIZE_T> queBase;

		HINSTANCE hNtdll = GetModuleHandleW(L"ntdll.dll");
		if (hNtdll == NULL){
			return queBase;
		}
		pfnZwQuerySystemInformation pZwQuerySystemInformation = (pfnZwQuerySystemInformation)GetProcAddress(hNtdll, "ZwQuerySystemInformation");
		if (pZwQuerySystemInformation == NULL){
			return queBase;
		}
		ULONG nBufferSize = 0;
		NTSTATUS status = pZwQuerySystemInformation(SystemModuleInformation, NULL, 0, &nBufferSize);
		if (status == STATUS_INFO_LENGTH_MISMATCH){
			PBYTE pBuffer = new BYTE[nBufferSize];
			if (pBuffer != NULL){
				status = pZwQuerySystemInformation(SystemModuleInformation, pBuffer, nBufferSize, &nBufferSize);
				if (status == STATUS_SUCCESS)
				{
					PRTL_PROCESS_MODULES Modules = (PRTL_PROCESS_MODULES)pBuffer;
					ULONG nCount = Modules->NumberOfModules;
					for (ULONG i = 0; i < nCount; i++){
						queBase.push_back((SIZE_T)(Modules->Modules[i].ImageBase));
					}
				}
				delete[] pBuffer;
				pBuffer = NULL;
			}
		}

		return queBase;
	}

	void RefreshDriver()
	{
		m_pView->DeleteAllItems();
		EnumDriver0();
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
	CHackerGuardianView<HG_DRIVERINFO>*		m_pView;

private:
	class CFreeInstance{
	public:
		~CFreeInstance(){
			if (CLVDriver::_instance != NULL){
				if (CLVDriver::_instance != NULL){
					delete CLVDriver::_instance;
					CLVDriver::_instance = NULL;
				}
			}
		}
	protected:
	private:
	};

private:
	static CLVDriver*		_instance;
	static CFreeInstance	m_garbo;
};

#endif