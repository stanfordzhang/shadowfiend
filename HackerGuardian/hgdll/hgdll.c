
#include "hgdll.h"
// #include "..\references\vld\include\vld.h"
// 
// #ifdef _WIN64 //_M_AMD64//_AMD64_
// #pragma comment(lib, "..\\references\\vld\\lib\\Win64\\vld.lib")
// #else
// #pragma comment(lib, "..\\references\\vld\\lib\\Win32\\vld.lib")
// #endif

HANDLE		g_hDriver = NULL;
 
BOOL SCMLoadNtDriver(
	__in	PWCHAR	pwszDriverName,
	__in	PWCHAR	pwszLinkName
	)
{
	BOOL bRet = FALSE;
	WCHAR szDriverImagePath[SZ_PATH_LEN] = {0};
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hService = NULL;
	DWORD dwErrorCode = 0;
	int nTry = 0;
	SERVICE_STATUS ss;
	GetFullPathNameW(	pwszDriverName, 
						SZ_PATH_LEN, 
						szDriverImagePath,
						NULL);

	// Open service control manager
	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL){
		dwErrorCode = GetLastError();
		SzLogA("OpenSCManager failed, error code: %d\n", dwErrorCode);
		goto HG_EXIT;
	}

	// create/open service
	hService = CreateServiceW(	hSCM,
								pwszLinkName,
								pwszLinkName,
								SERVICE_ALL_ACCESS,
								SERVICE_KERNEL_DRIVER,
								SERVICE_DEMAND_START,
								SERVICE_ERROR_NORMAL,
								szDriverImagePath,
								NULL,
								0,
								NULL,
								NULL,
								NULL);
	if (hService == NULL){
		dwErrorCode = GetLastError();
		// if this service existed, just open it.
		if ((dwErrorCode == ERROR_SERVICE_EXISTS)			||
			(dwErrorCode == ERROR_SERVICE_MARKED_FOR_DELETE)||
			(dwErrorCode == ERROR_IO_PENDING))
		{
			hService = OpenServiceW(hSCM,
									pwszLinkName, 
									SERVICE_ALL_ACCESS);

			if (hService == NULL){
				dwErrorCode = GetLastError();
				SzLogA("Open service failed, error code: %d\n", 
					dwErrorCode);
				goto HG_CLEAN_0;
			}
		}
	}

	bRet = StartService(hService, 0, NULL);
	if (bRet == FALSE){
		dwErrorCode = GetLastError();
		SzLogA("Error code: %d\n", dwErrorCode);
		if (dwErrorCode == ERROR_SERVICE_ALREADY_RUNNING){
			bRet = TRUE;
			goto HG_CLEAN_1;
		}else if (dwErrorCode == ERROR_IO_PENDING){
			SzLogA("Start service failed, device is pending!\n");
			goto HG_CLEAN_1;
		}else{
			DeleteService(hService);
			goto HG_CLEAN_1;
		}
	}else{
		QueryServiceStatus(hService, &ss);
		while (ss.dwCurrentState == SERVICE_START_PENDING && nTry++ < 64){
			Sleep(50);
			QueryServiceStatus(hService, &ss);
		}
		if (ss.dwCurrentState == SERVICE_RUNNING){
			bRet = TRUE;
			goto HG_CLEAN_1;
		}else{
			goto HG_CLEAN_1;
		}
	}
HG_CLEAN_1:
	if (hService != NULL){
		CloseServiceHandle(hService);
		hService = NULL;
	}	
HG_CLEAN_0:
	if (hSCM != NULL){
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}
HG_EXIT:
	return bRet;
}

void SCMUnloadNtDriver(
	__in	PWCHAR	pwszLinkName
	)
{
	SC_HANDLE hSCM = NULL;
	SC_HANDLE hService = NULL;
	SERVICE_STATUS	ss;
	DWORD dwErrorCode = 0;
	BOOL bRtn = FALSE;
	int nTry = 0;

	hSCM = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCM == NULL){
		SzLogA("OpenSCManager failed! Error code: %d\n", GetLastError());
		goto HG_EXIT;
	}

	hService = OpenServiceW(hSCM, pwszLinkName, SERVICE_ALL_ACCESS);
	if (hService == NULL){
		SzLogA("SCMUnloadNtDriver: Open service failed! Error code: %d\n", GetLastError());
		goto HG_CLEAN_0;
	}

	bRtn = ControlService(hService, SERVICE_CONTROL_STOP, &ss);
	if (bRtn == FALSE){
		SzLogA("Stop service failed! Error code: %d\n", GetLastError());
		goto HG_CLEAN_1;
	}

	while (ss.dwCurrentState == SERVICE_STOP_PENDING && nTry++ < 64){
		Sleep(50);
		QueryServiceStatus(hService, &ss);
	}

	if (ss.dwCurrentState == SERVICE_STOPPED){
		bRtn = DeleteService(hService);
		if (bRtn == FALSE){
			SzLogA("Delete service failed! Error code: %d\n", GetLastError());
		}
	}

HG_CLEAN_1:
	if (hService != NULL){
		CloseServiceHandle(hService);
		hService = NULL;
	}
HG_CLEAN_0:
	if (hSCM != NULL){
		CloseServiceHandle(hSCM);
		hSCM = NULL;
	}
HG_EXIT:
	return;
}

BOOL
ConnectDriver(
	)
{
	BOOL bRet = FALSE;
//	SzLogA("Entry ConnectDriver!");
	g_hDriver = CreateFileW(
		SZH_USER_MODE_DEVICE_NAME, 
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL, OPEN_EXISTING, 
		FILE_FLAG_NO_BUFFERING, NULL);

	if (g_hDriver != INVALID_HANDLE_VALUE){
		bRet = TRUE;
	}
//	SzLogA("Leave ConnectDriver! bRet = %d, g_hDriver = 0x%p", bRet, g_hDriver);
	return bRet;
}

BOOL
RequestDriver(
	__in		DWORD	dwIoControlCode,
	__in		LPVOID	lpInBuffer,
	__in		DWORD	nInBufferSize,
	__out		LPVOID	lpOutBuffer,
	__in		DWORD	nOutBufferSize,
	__in_opt	LPDWORD	lpBytesReturned
	)
{
	BOOL bRet = FALSE;
//	SzLogA("Entry RequestDriver!");
	if ((g_hDriver != INVALID_HANDLE_VALUE) && (g_hDriver != NULL)){
		bRet = DeviceIoControl(
			g_hDriver, 
			dwIoControlCode, 
			lpInBuffer, 
			nInBufferSize, 
			lpOutBuffer, 
			nOutBufferSize, 
			lpBytesReturned, 
			NULL);
	}

	return bRet;
}

BOOL
DisconnectDriver(
	)
{
	BOOL bRet = FALSE;
	if ((g_hDriver != NULL) && (g_hDriver != INVALID_HANDLE_VALUE)){
		CloseHandle(g_hDriver);
		g_hDriver = INVALID_HANDLE_VALUE;
		bRet = TRUE;
	}
	return bRet;
}

FARPROC SzhQueryInterface(INTERFACEID iid){
	switch (iid)
	{
	case eEnumerateProcess:
		return (FARPROC)SzhEnumerateProcess;
	case eFreeItp:
		return (FARPROC)SzhFreeItp;
	case eEnumerateProcessModule:
		return (FARPROC)HgEnumerateProcessModule;
	case eEnumerateProcessThread:
		return (FARPROC)HgEnumerateProcessThread;
	case eEnumerateProcessMemory:
		return (FARPROC)HgEnumerateProcessMemory;
	case eProcessMemoryZero:
		return (FARPROC)HgProcessMemoryZero;
	case eProcessMemoryFree:
		return (FARPROC)HgProcessMemoryFree;
	case eProcessMemoryChangeProtection:
		return (FARPROC)HgProcessMemoryChangeProtection;
	case eEnumerateDriver:
		return (FARPROC)HgEnumerateDriver;
	case eRegistryEnumerateKey:
		return (FARPROC)HgRegistryEnumerateKey;
	case eRegistryEnumerateValueKey:
		return (FARPROC)HgRegistryEnumerateValueKey;
	case eKernelEnumerateNotify:
		return (FARPROC)HgKernelEnumerateNotify;
	case eHookEnumerateSSDT:
		return (FARPROC)HgHookSSDT;
	case eHookEnumerateShadowSSDT:
		return (FARPROC)HgHookShadowSSDT;
	case eReadProcessMemoryAux:
		return (FARPROC)HgReadProcessMemoryAux;
	case eProcessHandleEnum:
		return (FARPROC)HgEnumerateProcessHandle;
	case eProcessHandleClose:
		return (FARPROC)HgCloseProcessHandle;
	}
	return NULL;
}

BOOL 
__stdcall
SzhEnumerateProcess(
	__inout	PITP*	ppItp
	)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	PITP	pItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t nCount = 128;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(PROCESS_DATA);
		if (pItp != NULL){
			free(pItp);
			pItp = NULL;
		}
		pItp = (PITP)malloc(nSize);
		if (pItp != NULL){
			*ppItp = pItp;
			ZeroMemory(pItp, nSize);
			pItp->op_type = OT_PROCESS_ENUMERATE;
			pItp->process.nCount = nCount;
			pItp->process.nRealCount = 0;
			bRet = RequestDriver(	/*SZH_IOCTL_IN_DIRECT*/SZH_IOCTL_NEITHER,
									pItp,
									sizeof(OPERATION_TYPE),
									pItp, 
									(DWORD)(nSize),
									&dwBytesReturned);
		}
		nCount = pItp->process.nCount + 32;
	}while(pItp->process.nRealCount > pItp->process.nCount);

	if (bRet == FALSE){
		free((void*)pItp);
		pItp = NULL;
	}else{
//		SzLogA("pItp = 0x%p, ppItp = 0x%p\n", pItp, ppItp);
		bRet = TRUE;
	}

	return bRet;
}

void
__stdcall
SzhFreeItp(
	__in	PITP	pItp
	)
{
// 	if (pItp != NULL){
// 		SzLogA("free pItp->data.process.pProcessHead!\n");
// 		if (pItp->data.process.pProcess != NULL){
// 			free((void*)pItp->data.process.pProcess);
// 			pItp->data.process.pProcess = NULL;
// 		}
// 
// 		SzLogA("free pItp!\n");
// 		free((void*)pItp);
// 		pItp = NULL;
// 	}
// 	
	if (pItp != NULL){
//		SzLogA("free pItp!\n");
		free((void*)pItp);
		pItp = NULL;
	}
}

BOOL __stdcall HgEnumerateProcessModule(SIZE_T nProcessId, SIZE_T pEProcess, PITP* ppItp)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	size_t	nCount			= 128;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_MODULE;
	InItp.in_process_module.nProcessId = nProcessId;
	InItp.in_process_module.pEProcess = pEProcess;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(PROCESS_MODULE_DATA);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_PROCESS_MODULE;
			pOutItp->out_process_module.nMaxCount = nCount;
			pOutItp->out_process_module.nCount = 0;
			bRtn = RequestDriver(	SZH_IOCTL_NEITHER,
									&InItp,
									sizeof(ITP),
									pOutItp, 
									(DWORD)nSize,
									&dwBytesRtn);
		}
		nCount = pOutItp->out_process_module.nMaxCount + 64;
	}while(pOutItp->out_process_module.nCount > pOutItp->out_process_module.nMaxCount);

	if (bRtn == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
//		SzLogA("[hgdll.c].[HgEnumerateProcessModule]: Enumerate process module successfully!");
		bRtn = TRUE;
	}

	return bRtn;
}

BOOL __stdcall HgEnumerateProcessThread(SIZE_T nProcessId, SIZE_T pEProcess, PITP* ppItp)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	size_t	nCount			= 64;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_THREAD;
	InItp.in_process_thread.nProcessId = nProcessId;
	InItp.in_process_thread.pEProcess = pEProcess;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(PROCESS_THREAD_DATA);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_PROCESS_THREAD;
			pOutItp->out_process_thread.nMaxCount = nCount;
			pOutItp->out_process_thread.nCount = 0;
			bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), pOutItp, (DWORD)nSize,	&dwBytesRtn);
		}
		nCount = pOutItp->out_process_thread.nMaxCount + 64;
	}while(pOutItp->out_process_thread.nCount > pOutItp->out_process_thread.nMaxCount);

	if (bRtn == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
//		SzLogA("[hgdll.c].[HgEnumerateProcessThread]: Enumerate process thread successfully!");
		bRtn = TRUE;
	}

	return bRtn;
}

BOOL __stdcall HgEnumerateProcessMemory(SIZE_T nProcessId, SIZE_T pEProcess, PITP* ppItp)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	size_t	nCount			= 0x500;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_MEMORY_ENUM;
	InItp.in_process_memory.nPId = nProcessId;
	InItp.in_process_memory.pEProcess = pEProcess;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(PROCESS_MEMORY_DATA);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_PROCESS_MEMORY_ENUM;
			pOutItp->out_process_memory.nMaxCount = nCount;
			pOutItp->out_process_memory.nCount = 0;
			bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), pOutItp, (DWORD)nSize,	&dwBytesRtn);
		}
		nCount = pOutItp->out_process_memory.nMaxCount + 0x100;
	}while(pOutItp->out_process_memory.nCount > pOutItp->out_process_memory.nMaxCount);

	if (bRtn == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
		bRtn = TRUE;
	}

	return bRtn;
}

BOOL __stdcall HgProcessMemoryZero(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	ITP		InItp;
	ITP		OutItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_MEMORY_ZERO;
	InItp.in_process_memory.Base = Base;
	InItp.in_process_memory.Size = Size;
	InItp.in_process_memory.nPId = nProcessId;
	InItp.in_process_memory.pEProcess = pEProcess;
	RtlCopyMemory(&OutItp, &InItp, sizeof(ITP));
	bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), &OutItp, sizeof(ITP), &dwBytesRtn);

	return bRtn;
}

BOOL __stdcall HgProcessMemoryChangeProtection(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size,
	__in	SIZE_T	NewProtection)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	ITP		InItp;
	ITP		OutItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_MEMORY_CHANGE_PROTECTION;
	InItp.in_process_memory.Base = Base;
	InItp.in_process_memory.Size = Size;
	InItp.in_process_memory.nPId = nProcessId;
	InItp.in_process_memory.pEProcess = pEProcess;
	InItp.in_process_memory.ModifyType = NewProtection;
	RtlCopyMemory(&OutItp, &InItp, sizeof(ITP));
	bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), &OutItp, sizeof(ITP), &dwBytesRtn);

	return bRtn;
}

BOOL __stdcall HgProcessMemoryFree(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	ITP		InItp;
	ITP		OutItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_MEMORY_FREE;
	InItp.in_process_memory.Base = Base;
	InItp.in_process_memory.Size = Size;
	InItp.in_process_memory.nPId = nProcessId;
	InItp.in_process_memory.pEProcess = pEProcess;
	RtlCopyMemory(&OutItp, &InItp, sizeof(ITP));
	bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), &OutItp, sizeof(ITP), &dwBytesRtn);

	return bRtn;
}

BOOL __stdcall HgEnumerateProcessHandle(SIZE_T nProcessId, SIZE_T pEProcess, PITP* ppItp)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	size_t	nCount			= 0x1000;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_HANDLE_ENUM;
	InItp.get_process_handles_params.Pid = nProcessId;
	InItp.get_process_handles_params.EProcess = pEProcess;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(HANDLE_DATA);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_PROCESS_HANDLE_ENUM;
			pOutItp->get_process_handles.nMaxCount = nCount;
			pOutItp->get_process_handles.nCount = 0;
			bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), pOutItp, (DWORD)nSize,	&dwBytesRtn);
		}
		nCount = pOutItp->get_process_handles.nMaxCount + 0x200;
	}while(pOutItp->get_process_handles.nCount > pOutItp->get_process_handles.nMaxCount);
	
	if (bRtn == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
		bRtn = TRUE;
	}

	return bRtn;
}


BOOL __stdcall HgCloseProcessHandle(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Handle,
	__in	SIZE_T	HandleObject,
	__in	SIZE_T	IsForceClose)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	ITP		InItp;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_PROCESS_HANDLE_CLOSE;
	InItp.close_process_handle_params.Pid = nProcessId;
	InItp.close_process_handle_params.EProcess = pEProcess;
	InItp.close_process_handle_params.Handle = Handle;
	InItp.close_process_handle_params.HandleObject = HandleObject;
	InItp.close_process_handle_params.IsForceKill = IsForceClose;

	bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), &InItp, sizeof(ITP), &dwBytesRtn);

	return bRtn;
}

BOOL __stdcall HgEnumerateDriver(
	__inout	PITP*	ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	PITP	pItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t	nCount = 256;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(DRIVER_DATA);
		if (pItp != NULL){
			free(pItp);
			pItp = NULL;
		}
		pItp = (PITP)malloc(nSize);
		if (pItp != NULL){
			*ppItp = pItp;
			ZeroMemory(pItp, nSize);
			pItp->op_type = OT_DRIVER_ENUMERATE;
			pItp->driver.nCount = 0;
			pItp->driver.nMaxCount = nCount;
			bRet = RequestDriver(	SZH_IOCTL_NEITHER,
									pItp,
									sizeof(OPERATION_TYPE),
									pItp, 
									(DWORD)(nSize),
									&dwBytesReturned);
		}
		nCount = pItp->driver.nMaxCount + 64;
	}while(pItp->driver.nMaxCount < pItp->driver.nCount);

	if (bRet == FALSE){
		free((void*)pItp);
		pItp = NULL;
	}else{
//		SzLogA("pItp = 0x%p, ppItp = 0x%p\n", pItp, ppItp);
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgRegistryEnumerateKey(
	__in	const PWCHAR	Path,
	__inout	PITP*			ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t	nCount			= 16 * MAX_PATH;
	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_REGISTRY_ENUMERATE_KEY;
	wcscpy_s(InItp.in_registry.Path, wcslen(Path) + 1, Path);

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(WCHAR);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_REGISTRY_ENUMERATE_KEY;
			pOutItp->out_registry.nMaxLength = (nCount + 1) * sizeof(WCHAR);
			bRet = RequestDriver(	SZH_IOCTL_NEITHER,
									&InItp,
									sizeof(ITP),
									pOutItp, 
									(DWORD)(nSize),
									&dwBytesReturned);
		}
		
		nCount = pOutItp->out_registry.nMaxLength * 2;
	}while(bRet == FALSE && GetLastError() == 234);

	if (bRet == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
//		SzLogA("pItp = 0x%p, ppItp = 0x%p\n", pOutItp, ppItp);
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgRegistryEnumerateValueKey(
	__in	const PWCHAR	Path,
	__inout	PITP*			ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t	nCount			= 0x2000;
	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_REGISTRY_ENUMERATE_VALUE_KEY;
	wcscpy_s(InItp.in_registry.Path, wcslen(Path) + 1, Path);

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(WCHAR);
		if (pOutItp != NULL){
			free(pOutItp);
			pOutItp = NULL;
		}
		pOutItp = (PITP)malloc(nSize);
		if (pOutItp != NULL){
			*ppItp = pOutItp;
			ZeroMemory(pOutItp, nSize);
			pOutItp->op_type = OT_REGISTRY_ENUMERATE_VALUE_KEY;
			pOutItp->out_registry.nMaxLength = (nCount + 1) * sizeof(WCHAR);
			bRet = RequestDriver(	SZH_IOCTL_NEITHER,
									&InItp,
									sizeof(ITP),
									pOutItp, 
									(DWORD)(nSize),
									&dwBytesReturned);
		}

		nCount = pOutItp->out_registry.nMaxLength * 2;
	}while(bRet == FALSE && GetLastError() == 234);

	if (bRet == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
//		SzLogA("pItp = 0x%p, ppItp = 0x%p\n", pOutItp, ppItp);
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgKernelEnumerateNotify(
	__inout	PITP*	ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	PITP	pItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t nCount = 256;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(NOTIFY_DATA);
		if (pItp != NULL){
			free(pItp);
			pItp = NULL;
		}
		pItp = (PITP)malloc(nSize);
		if (pItp != NULL){
			*ppItp = pItp;
			ZeroMemory(pItp, nSize);
			pItp->op_type = OT_KERNEL_ENUMERATE_NOTIFY;
			pItp->notify.nMaxCount = nCount;
			pItp->notify.nCount = 0;
			bRet = RequestDriver(SZH_IOCTL_NEITHER,	pItp, sizeof(OPERATION_TYPE), pItp, (DWORD)(nSize),	&dwBytesReturned);
		}
		nCount = pItp->notify.nMaxCount + 32;
	}while(pItp->notify.nCount > pItp->notify.nMaxCount);

	if (bRet == FALSE){
		free((void*)pItp);
		pItp = NULL;
	}else{
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgHookSSDT(
	__inout PITP*	ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	PITP	pItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t nCount = 512;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(SDT_DATA);
		if (pItp != NULL){
			free(pItp);
			pItp = NULL;
		}
		pItp = (PITP)malloc(nSize);
		if (pItp != NULL){
			*ppItp = pItp;
			ZeroMemory(pItp, nSize);
			pItp->op_type = OT_HOOK_SSDT;
			pItp->sdt.nMaxCount = nCount;
			pItp->sdt.nCount = 0;
			bRet = RequestDriver(SZH_IOCTL_NEITHER,	pItp, sizeof(OPERATION_TYPE), pItp, (DWORD)(nSize),	&dwBytesReturned);
		}
		nCount = pItp->sdt.nMaxCount + 128;
	}while(pItp->sdt.nCount > pItp->sdt.nMaxCount);

	if (bRet == FALSE){
		free((void*)pItp);
		pItp = NULL;
	}else{
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgHookShadowSSDT(
	__inout PITP*	ppItp)
{
	BOOL	bRet			= FALSE;
	size_t	ulProcessCount	= 0;
	PITP	pItp			= *ppItp;
	DWORD	dwInBufferSize	= 0;
	DWORD	dwOutBufferSize = 0;
	DWORD	dwBytesReturned = 0;
	ULONG	i				= 0;
	size_t nCount = 1024;

	do{
		size_t nSize = sizeof(ITP) + nCount * sizeof(SHADOW_SDT_DATA);
		if (pItp != NULL){
			free(pItp);
			pItp = NULL;
		}
		pItp = (PITP)malloc(nSize);
		if (pItp != NULL){
			*ppItp = pItp;
			ZeroMemory(pItp, nSize);
			pItp->op_type = OT_HOOK_SHADOW_SSDT;
			pItp->shadow_sdt.nMaxCount = nCount;
			pItp->shadow_sdt.nCount = 0;
			bRet = RequestDriver(SZH_IOCTL_NEITHER,	pItp, sizeof(OPERATION_TYPE), pItp, (DWORD)(nSize),	&dwBytesReturned);
		}
		nCount = pItp->shadow_sdt.nMaxCount + 128;
	}while(pItp->shadow_sdt.nCount > pItp->shadow_sdt.nMaxCount);

	if (bRet == FALSE){
		free((void*)pItp);
		pItp = NULL;
	}else{
		bRet = TRUE;
	}

	return bRet;
}

BOOL __stdcall HgReadProcessMemoryAux(SIZE_T Base, SIZE_T Size, SIZE_T pEProcess, PITP* ppItp)
{
	BOOL	bRtn			= FALSE;
	DWORD	dwBytesRtn		= 0;
	ITP		InItp;
	PITP	pOutItp			= *ppItp;
	size_t	nSize			= 0;

	ZeroMemory(&InItp, sizeof(ITP));
	InItp.op_type = OT_MEMORY_READ;
	InItp.mem.Base = Base;
	InItp.mem.nLength = Size;
	InItp.mem.pEProcess = pEProcess;

	nSize = sizeof(ITP) + Size;
	pOutItp = (PITP)malloc(nSize);
	if (pOutItp != NULL){
		*ppItp = pOutItp;
		ZeroMemory(pOutItp, nSize);
		pOutItp->op_type = OT_MEMORY_READ;
		pOutItp->mem.Base = Base;
		pOutItp->mem.nLength = Size;
		pOutItp->mem.pEProcess = pEProcess;
		bRtn = RequestDriver(SZH_IOCTL_NEITHER,	&InItp,	sizeof(ITP), pOutItp, (DWORD)nSize,	&dwBytesRtn);
	}

	if (bRtn == FALSE){
		free((void*)pOutItp);
		pOutItp = NULL;
	}else{
		bRtn = TRUE;
	}

	return bRtn;
}