// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0601
#define _WIN32_WINNT	0x0601
#define _WIN32_IE	0x0700
#define _RICHEDIT_VER	0x0200

#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */

#ifdef	__cplusplus
extern "C"{
#endif

#if defined(_WIN64)
#include "../references/disassembler/beaengine-win64/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win64\\Win64\\Lib\\BeaEngine64.lib")
#else
#include "../references/disassembler/beaengine-win32/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win32\\Win32\\Lib\\BeaEngine.lib")
#endif

#ifdef	__cplusplus
}
#endif

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>
#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atlctrlw.h>
#include <atlctrlx.h>
#include <atlsplit.h>
#include <atlribbon.h>
#include <atlcrack.h>
#include <atlmisc.h>
#include <atlddx.h>

#include "atlgdix.h"
#include "atlwfile.h"

#include <vector>
#include <algorithm>
#include <map>
#include <queue>

#include <time.h>
#include <DbgHelp.h>
// #pragma auto_inline(off)
#pragma comment(lib, "DbgHelp")

#include "Ribbon.h"

#include "strdef.h"
#include "hguidef.h"

#include "ListDict.h"

#include "HackerGuardianView.h"
#include "HexEditor.h"
#include "HexEditorDlg.h"
#include "MemoryChangeProtectionDlg.h"

#include "DisassemblerDlg.h"

#include "LVProcess.h"
#include "LVDriver.h"
#include "UIBase.h"
#include "LVKernel.h"
#include "LVHook.h"

//#include "..\references\vld\include\vld.h"
// 
// #ifdef _WIN64 //_M_AMD64//_AMD64_
// #pragma comment(lib, "..\\references\\vld\\lib\\Win64\\vld.lib")
// #else
// #pragma comment(lib, "..\\references\\vld\\lib\\Win32\\vld.lib")
// #endif

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */ 

#include "..\..\hgdll\hgdll.h"

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
