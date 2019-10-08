
#ifndef __HG_BASE_H__
#define __HG_BASE_H__

#include <ntifs.h>
#include <windef.h>
#include <limits.h>

#include "stdhg.h"
#include "hgdef.h"
#include "hgexport.h"

extern ULONG_PTR g_hg_map[HG_MAP_LENGTH];

FORCEINLINE
ULONG_PTR
EPROCESS_ObjectTable(
	IN	PEPROCESS P	// this pointer point to _EPROCESS
	)
{
	WIN_VERSION WinVer = (WIN_VERSION)g_WindowsVersion;

	if (WinVer == eWin_XP){
		return *(PULONG_PTR)((ULONG)P + 0x0C4);
	}

	return 0;
}

FORCEINLINE
ULONG
HANDLE_TABLE_UniqueProcessId(
	IN	ULONG_PTR	H	// this pointer point to _HANDLE_TABLE
	)
{
	WIN_VERSION WinVer = (WIN_VERSION)g_WindowsVersion;

	if (WinVer == eWin_2000){
	}else if (WinVer = eWin_XP){
		return *(PULONG)((ULONG)H + 0x008);
	}else if (WinVer = eWin_2003){
	}else if (WinVer = eWin_2003_SP1_SP2){
	}else if (WinVer = eWin_Vista){
	}else if (WinVer = eWin_Vista_SP1_SP2){
	}else if (WinVer = eWin_7){
	}else if (WinVer = eWin_8){
	}

	return ULONG_MAX;
}

#endif