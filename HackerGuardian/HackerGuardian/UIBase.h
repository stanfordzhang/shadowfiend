#pragma once
class CUIBase
{
public:
	CUIBase(void);
	~CUIBase(void);

public:
	static void PropertiesDialog(CString strPath)
	{
		if (!strPath.IsEmpty() && PathFileExists(strPath))
		{
			SHELLEXECUTEINFO execInfo ;
			memset( &execInfo, 0, sizeof( execInfo ) ) ;
			execInfo.fMask = SEE_MASK_INVOKEIDLIST ;   
			execInfo.cbSize = sizeof( execInfo ) ;   
			execInfo.hwnd = NULL ;   
			execInfo.lpVerb = _T( "properties" ) ;   
			execInfo.lpFile = strPath ;   
			execInfo.lpParameters = NULL ;   
			execInfo.lpDirectory = NULL ;   
			execInfo.nShow = SW_SHOWNORMAL ;   
			execInfo.hProcess = NULL ;   
			execInfo.lpIDList = 0 ;   
			execInfo.hInstApp = 0 ;   
			ShellExecuteEx( &execInfo ) ;
		}
		else
		{
			MessageBox(NULL, ptszCannotFindFile, NULL, MB_OK | MB_ICONWARNING);
		}
	}
};

