#ifndef __HG_H__
#define __HG_H__

#include <Windows.h>

#include "..\hgdrv\hgioctl.h"
#include "..\references\szdef.h"

#pragma comment(lib, "Advapi32.lib")

#define SZH_HG_DRIVER32_NAME		L"hgdrv.sys"
#define SZH_HG_LINK_NAME			L"HackerGuardian"
#define SZH_USER_MODE_DEVICE_NAME	L"\\\\.\\HackerGuardian"

typedef enum _INTERFACEID{
	eEnumerateProcess = 0,
	eEnumerateProcessModule,
	eEnumerateProcessThread,
	eEnumerateProcessMemory,
	eProcessMemoryZero,
	eProcessMemoryFree,
	eProcessMemoryChangeProtection,
	eProcessHandleEnum,
	eProcessHandleClose,
	eEnumerateDriver,
	eRegistryEnumerateKey,
	eRegistryEnumerateValueKey,
	eKernelEnumerateNotify,
	eHookEnumerateSSDT,
	eHookEnumerateShadowSSDT,
	eReadProcessMemoryAux,
	eFreeItp
}INTERFACEID;

extern HANDLE		g_hDriver;

/************************************************************************/
/* private method                                                       */
/************************************************************************/
//
// 
// 
BOOL SCMLoadNtDriver(
	__in	PWCHAR	pwszDriverName,
	__in	PWCHAR	pwszLinkName
	);

//
//
// 
void SCMUnloadNtDriver(
	__in	PWCHAR	pwszLinkName
	);

// 
// connect driver
// 
BOOL
ConnectDriver(
	);

// 
// dll communicates with driver
// 
BOOL
RequestDriver(
	__in		DWORD	dwIoControlCode,
	__in		LPVOID	lpInBuffer,
	__in		DWORD	nInBufferSize,
	__out		LPVOID	lpOutBuffer,
	__in		DWORD	nOutBufferSize,
	__in_opt	LPDWORD	lpBytesReturned
	);

//
// disconnect driver
// 
BOOL
DisconnectDriver(
	);

/************************************************************************/
/* export methods                                                       */
/************************************************************************/ 

typedef FARPROC (*PSzhQueryInterface)(INTERFACEID iid);
FARPROC SzhQueryInterface(INTERFACEID iid);

//
// process
// 
typedef BOOL (__stdcall *PSzhEnumerateProcess)(PITP *ppItp);
BOOL
__stdcall
SzhEnumerateProcess(
	__inout	PITP	*ppItp
	);

typedef void (__stdcall *PSzhFreeItp)(PITP pItp);
void
__stdcall
SzhFreeItp(
	__in	PITP	pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgEnumerateProcessModule)(SIZE_T, SIZE_T, PITP*);
BOOL __stdcall HgEnumerateProcessModule(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__inout	PITP*	ppItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgEnumerateProcessThread)(SIZE_T, SIZE_T, PITP*);
BOOL __stdcall HgEnumerateProcessThread(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__inout	PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgEnumerateProcessMemory)(SIZE_T, SIZE_T, PITP*);
BOOL __stdcall HgEnumerateProcessMemory(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__inout	PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgProcessMemoryZero)(SIZE_T, SIZE_T, SIZE_T, SIZE_T);
BOOL __stdcall HgProcessMemoryZero(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgProcessMemoryChangeProtection)(SIZE_T, SIZE_T, SIZE_T, SIZE_T, SIZE_T);
BOOL __stdcall HgProcessMemoryChangeProtection(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size,
	__in	SIZE_T	NewProtection);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgProcessMemoryFree)(SIZE_T, SIZE_T, SIZE_T, SIZE_T);
BOOL __stdcall HgProcessMemoryFree(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Base,
	__in	SIZE_T	Size);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgEnumerateProcessHandle)(SIZE_T, SIZE_T, PITP*);
BOOL __stdcall HgEnumerateProcessHandle(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__inout	PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgCloseProcessHandle)(SIZE_T, SIZE_T, SIZE_T, SIZE_T, SIZE_T);
BOOL __stdcall HgCloseProcessHandle(
	__in	SIZE_T	nProcessId, 
	__in	SIZE_T	pEProcess,
	__in	SIZE_T	Handle,
	__in	SIZE_T	HandleObject,
	__in	SIZE_T	IsForceClose);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgEnumerateDriver)(PITP*);
BOOL __stdcall HgEnumerateDriver(
	__inout	PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgRegistryEnumerateKey)(PWCHAR, PITP*);
BOOL __stdcall HgRegistryEnumerateKey(
	__in	const PWCHAR	Path,
	__inout	PITP*			ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgRegistryEnumerateValueKey)(PWCHAR, PITP*);
BOOL __stdcall HgRegistryEnumerateValueKey(
	__in	const PWCHAR	Path,
	__inout	PITP*			ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgKernelEnumerateNotify)(PITP*);
BOOL __stdcall HgKernelEnumerateNotify(
	__inout	PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgHookSSDT)(PITP*);
BOOL __stdcall HgHookSSDT(
	__inout PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgHookShadowSSDT)(PITP*);
BOOL __stdcall HgHookShadowSSDT(
	__inout PITP*	ppItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
typedef BOOL (__stdcall *pfnHgReadProcessMemoryAux)(SIZE_T, SIZE_T, SIZE_T, PITP*);
BOOL __stdcall HgReadProcessMemoryAux(
	__in	SIZE_T Base,
	__in	SIZE_T Size, 
	__in	SIZE_T pEProcess, 
	__inout	PITP* ppItp);

#endif