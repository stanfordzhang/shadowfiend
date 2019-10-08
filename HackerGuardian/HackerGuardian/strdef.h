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

#ifndef __STRDEF_H__
#define __STRDEF_H__

typedef enum _LANGUAGEID{
	eEnglish = 0
}LANGUAGEID;

extern LANGUAGEID gLanguageId;

extern TCHAR*	ptszDll;
extern CHAR*	pszQueryInterface;
extern TCHAR*	ptszCannotFindFile;

extern TCHAR*	ptszTabProcess[];
extern TCHAR*	ptszTabProcessColumn[1][7];
extern TCHAR*	ptszTabProcessPid[];
extern TCHAR*	ptszTabProcessPath[];

extern TCHAR*	ptszTabProcessModule[];
extern TCHAR*	ptszTabProcessModuleColumn[1][5];

extern TCHAR*	ptszTabProcessThread[];
extern TCHAR*	ptszTabProcessThreadColumn[1][9];

extern TCHAR*	ptszTabProcessHandle[];
extern TCHAR*	ptszTabProcessHandleColumn[1][6];

extern TCHAR*	ptszTabProcessWindow[];
extern TCHAR*	ptszTabProcessWindowColumn[1][6];

extern TCHAR*	ptszTabProcessMemory[];
extern TCHAR*	ptszTabProcessMemoryColumn[1][6];

extern TCHAR*	ptszTabProcessTimer[];
extern TCHAR*	ptszTabProcessTimerColumn[1][6];

extern TCHAR*	ptszTabProcessHotKey[];
extern TCHAR*	ptszTabProcessHotKeyColumn[1][6];

// Driver
extern WCHAR*	pwszTabDriver[];
extern WCHAR*	pwszTabDriverColumn[1][8];

/************************************************************************/
/* Kernel                                                               */
/************************************************************************/
// Notify
extern WCHAR*	pwszTabNotifyRoutine[];
extern WCHAR*	pwszTabNotifyRoutineColumn[1][5];
extern WCHAR*	pwszNotifyRoutineType[];

/************************************************************************/
/* Hook                                                                 */
/************************************************************************/
// SDT
extern WCHAR*	pwszTabSdt[];
extern WCHAR*	pwszTabShadowSdt[];
extern WCHAR*	pwszTabSdtColumn[1][6];

/************************************************************************/
/* Shadow SSDT Function Name                                            */
/************************************************************************/
extern WCHAR *Win2000ShadowSDTName[];
extern WCHAR *WinXPShadowSDTName[];
extern WCHAR *Win2003ShadowSDTName[];
extern WCHAR *WinVistaShadowSDTName[];
extern WCHAR *Win7ShadowSDTName[];
extern WCHAR *Win8ShadowSDTName[];

#endif