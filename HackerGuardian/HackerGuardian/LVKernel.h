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

#ifndef __LVKERNEL_H__
#define __LVKERNEL_H__

#include "stdafx.h"
#include "HGDriver.h"
#include "LVDriver.h"

#include <TlHelp32.h>

/************************************************************************/
/* Define process info structure                                        */
/************************************************************************/
typedef struct _HG_NOTIFYINFO: public CSortListViewItem<_HG_NOTIFYINFO>{
	SIZE_T		m_RoutineEntry;
	SIZE_T		m_Type;
	WCHAR		m_Path[SZ_PATH_LEN];
	WCHAR		m_Company[MAX_PATH];
	WCHAR		m_Comment[SZ_PATH_LEN];
	COLORREF	m_RGB;

	_HG_NOTIFYINFO()
		: m_RoutineEntry(0)
		, m_Type(0)
		, m_RGB(RGB(255, 0, 0))
	{
			memset(m_Path, 0, sizeof(WCHAR) * SZ_PATH_LEN);
			ZeroMemory(m_Company, sizeof(WCHAR) * MAX_PATH);
			ZeroMemory(m_Comment, sizeof(WCHAR) * SZ_PATH_LEN);
	}

	_HG_NOTIFYINFO(
		SIZE_T		RoutineEntry, 
		SIZE_T		Type,
		PWCHAR		Path,
		PWCHAR		Company,
		PWCHAR		Comment,
		COLORREF	rgb)
		: m_RoutineEntry(RoutineEntry)
		, m_Type(Type)
		, m_RGB(rgb)
	{
		wcscpy_s(m_Path, Path);
		wcscpy_s(m_Company, Company);
		wcscpy_s(m_Comment, Comment);
	}

	int Compare(_HG_NOTIFYINFO* arg0, int nField){
		switch (nField){
		case 0:	// Compare only the filename - regardless of the path
			return m_RoutineEntry > arg0->m_RoutineEntry ? 1 : (m_RoutineEntry > arg0->m_RoutineEntry ? -1 : 0);
		case 1:	// 
			return m_Type > arg0->m_Type ? 1 : (m_Type > arg0->m_Type ? -1 : 0);
		case 2:	// access
			return _tcsicmp(m_Path, arg0->m_Path);
		case 3:	// 
			return _tcsicmp(m_Company, arg0->m_Company);
		case 4:	// access
			return _tcsicmp(m_Comment, arg0->m_Comment);
		}
		return 0;
	}
}HG_NOTIFYINFO, *PHG_NOTIFYINFO;

/************************************************************************/
/* CLVKernel                                                           */
/************************************************************************/
struct CLVKernel{
	CLVKernel()
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

	static CLVKernel* GetInstance(){
		if (_instance == NULL){
			_instance = new CLVKernel();
		}

		return _instance;
	}

	HRESULT DisplayNotify(){
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
		m_pView = new CHackerGuardianView<HG_NOTIFYINFO>;
		m_pView->SetTabId(eKernelNotify);

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
		m_top_tab->AddPage(m_pView->m_hWnd, pwszTabNotifyRoutine[eEnglish]);

		// Clear
		m_pView->DeleteAllItems();

#if defined(_WIN64)
		m_pView->InsertColumn(0, pwszTabNotifyRoutineColumn[eEnglish][0], LVCFMT_LEFT, 150);
#else
		m_pView->InsertColumn(0, pwszTabNotifyRoutineColumn[eEnglish][0], LVCFMT_LEFT, 90);
#endif
		m_pView->InsertColumn(1, pwszTabNotifyRoutineColumn[eEnglish][1], LVCFMT_LEFT, 100);
		m_pView->InsertColumn(2, pwszTabNotifyRoutineColumn[eEnglish][2], LVCFMT_LEFT, 260);
		m_pView->InsertColumn(3, pwszTabNotifyRoutineColumn[eEnglish][3], LVCFMT_LEFT, 120);
		m_pView->InsertColumn(4, pwszTabNotifyRoutineColumn[eEnglish][4], LVCFMT_LEFT, 240);

		//
		// Traverse all processes.
		//
 		EnumKernelNotifyR0();

		return hr;
	}

	HRESULT InsertDriverItem(PHG_NOTIFYINFO arg0){
		HRESULT hr = S_OK;
		int nRow = 0;
		if (arg0 == NULL){
			return S_FALSE;
		}
		nRow = m_pView->InsertItem(m_pView->GetItemCount(), size_tToHexCString(arg0->m_RoutineEntry));
		m_pView->SetItemText(nRow, 1, pwszNotifyRoutineType[arg0->m_Type]);
		m_pView->SetItemText(nRow, 2, arg0->m_Path);
		m_pView->SetItemText(nRow, 3, arg0->m_Company);
		m_pView->SetItemText(nRow, 4, arg0->m_Comment);
		m_pView->SetItemData(nRow, (DWORD_PTR)arg0);

		return hr;
	}

	void EnumKernelNotifyR0()
	{
		PITP pItp = NULL;
		PHG_NOTIFYINFO hgNi = NULL;
		SIZE_T i = 0;
		std::vector<HG_DRIVERINFO> DriverVector = EnumDriverR0();
		size_t DriverVectorSize = DriverVector.size();

		CString strPath;
		CString strCompany;
		COLORREF rgb;
		CString strComment;
		SIZE_T CurrentDriverObject = 0;

		CHGDriver::GetInstance()->EnumerateKernelNotify(&pItp);

		if (pItp != NULL){
			for (i = 0; i < pItp->driver.nCount; i++)
			{
				for (size_t j = 0; j < DriverVectorSize; j ++){
					if (pItp->notify.NotifyData[i].pCallbackAddress >= DriverVector[j].m_nBase &&
						pItp->notify.NotifyData[i].pCallbackAddress <= DriverVector[j].m_nBase + DriverVector[j].m_nSize)
					{
						strPath = DriverVector[j].m_pwszPath;
						strCompany = DriverVector[j].m_pwszCompany;
						rgb = DriverVector[j].m_RGB;
						CurrentDriverObject = DriverVector[j].m_pDriverObject;
						break;
					}
				}
				switch (pItp->notify.NotifyData[i].nt){
				case eNT_CreateProcess:
				case eNT_CreateThread:
				case eNT_LoadImage:
				case eNT_CmCallBack:
					if (pItp->notify.NotifyData[i].Comment != 0)	{
						strComment.Format(TEXT("PEX_FAST_REF = 0x%p"), pItp->notify.NotifyData[i].Comment);
					}
					break;
				case eNT_Shutdown:
					if (pItp->notify.NotifyData[i].Comment != 0)	{
						strComment.Format(TEXT("PDEVICE_OBJECT = 0x%p"), pItp->notify.NotifyData[i].Comment);
					}
					break;
				}
				hgNi = new HG_NOTIFYINFO(pItp->notify.NotifyData[i].pCallbackAddress,
					pItp->notify.NotifyData[i].nt,
					strPath.GetBuffer(strPath.GetLength()),
					strCompany.GetBuffer(strCompany.GetLength()),
					strComment.GetBuffer(strComment.GetLength()),
					rgb);
				InsertDriverItem(hgNi);
			}
			CHGDriver::GetInstance()->FreeItp(pItp);
		}else{
			SzLogA("[LVKernel.h].[EnumKernelNotifyR0]: ERROR: pItp == NULL");
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
		EnumKernelNotifyR0();
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
	CHackerGuardianView<HG_NOTIFYINFO>*		m_pView;

private:
	class CFreeInstance{
	public:
		~CFreeInstance(){
			if (CLVKernel::_instance != NULL){
				if (CLVKernel::_instance != NULL){
					delete CLVKernel::_instance;
					CLVKernel::_instance = NULL;
				}
			}
		}
	protected:
	private:
	};

private:
	static CLVKernel*		_instance;
	static CFreeInstance	m_garbo;
};

#endif