
#include "hgdll.h"
#include <Windows.h>

#if defined (_WIN64)
#pragma  message("_x64  macro  activated!")
#else
#pragma  message("_x86  macro  activated!")
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
	)
{
	BOOL bRtn = FALSE;
	SzLogA("Entry DllMain!");
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		SzLogA("DLL_PROCESS_ATTACH!");
		bRtn = SCMLoadNtDriver(SZH_HG_DRIVER32_NAME, SZH_HG_LINK_NAME);
		//
		// connect driver
		// 
		if (bRtn == TRUE){
			SzLog(TEXT("Load %s success!\n"), SZH_HG_DRIVER32_NAME);
			ConnectDriver();
		}else{
			SzLog(TEXT("Load %s failed!\n"), SZH_HG_DRIVER32_NAME);
		}
		break;
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		SzLogA("DLL_PROCESS_DETACH!");
		//
		// disconnect driver
		// 
		bRtn = DisconnectDriver();
		if (bRtn == TRUE){
			SzLog(TEXT("Unload %s success!\n"), SZH_HG_DRIVER32_NAME);
			SCMUnloadNtDriver(SZH_HG_LINK_NAME);
		}else{
			SzLog(TEXT("Unload %s failed!\n"), SZH_HG_DRIVER32_NAME);
		}
		break;
	}
	return TRUE;
}