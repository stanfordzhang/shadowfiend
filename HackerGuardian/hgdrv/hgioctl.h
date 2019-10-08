/************************************************************************/
/* 
 * Copyright (c) Stanford Studio. All rights reserved.
 * 
 * Module Name:
 *	hgioctl.h
 *	
 * Abstract:
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
 * Revision History:
 *	
 */
/************************************************************************/

#ifndef __HG_IOCTL_H__
#define __HG_IOCTL_H__

#include <WinDef.h>
//#include "hgdef.h"
//
// File path length
// 
#define		SZ_PATH_LEN					2 * MAX_PATH
#define		SZ_PATH_LEN_WCHAR			SZ_PATH_LEN * sizeof(WCHAR)

/************************************************************************/
/* IO Control Code                                                      */
/************************************************************************/

//
// METHOD_BUFFERED
// 
#define		SZH_IOCTL_BUFFERED		\
	CTL_CODE				\
	(						\
	FILE_DEVICE_UNKNOWN,\
	0x800,				\
	METHOD_BUFFERED,	\
	FILE_ANY_ACCESS		\
	)

//
// METHOD_IN_DIRECT
// 
#define		SZH_IOCTL_IN_DIRECT		\
	CTL_CODE				\
	(						\
	FILE_DEVICE_UNKNOWN,\
	0x801,				\
	METHOD_IN_DIRECT,	\
	FILE_ANY_ACCESS		\
	)

//
// METHOD_OUT_DIRECT
// 
#define		SZH_IOCTL_OUT_DIRECT	\
	CTL_CODE				\
	(						\
	FILE_DEVICE_UNKNOWN,\
	0x802,				\
	METHOD_OUT_DIRECT,	\
	FILE_ANY_ACCESS		\
	)

//
// METHOD_NEITHER
// 
#define		SZH_IOCTL_NEITHER		\
	CTL_CODE				\
	(						\
	FILE_DEVICE_UNKNOWN,\
	0x803,				\
	METHOD_NEITHER,	\
	FILE_ANY_ACCESS		\
	)

/************************************************************************/
/* Define operation type                                                */
/************************************************************************/
typedef enum _OPERATION_TYPE{
	OT_UNKNOWN = 0,
	// Process
	OT_PROCESS_ENUMERATE,					// enumerate process
	OT_PROCESS_MODULE,						// enumerate process module
	OT_PROCESS_THREAD,
	OT_PROCESS_KILL,
	OT_PROCESS_FORCE_KILL,
	OT_PROCESS_DUMP,
	OT_PROCESS_GET_PROCESS_SUSPEND_STATUS,
	OT_PROCESS_SUSPEND_OR_RESUME,
	OT_PROCESS_FORCE_SUSPEND_OR_RESUME,
	OT_PROCESS_DUMP_DLL_MODULE,
	OT_PROCESS_UNLOAD_DLL_MODULE,
	OT_PROCESS_KILL_THREAD,
	OT_PROCESS_RESUME_SUSPEND_THREAD,
	OT_PROCESS_FORCE_RESUME_SUSPEND_THREAD,
	OT_PROCESS_GET_THREAD_SUSPEND_COUNT,
	OT_PROCESS_ENUMERATE_HANDLE,
	OT_PROCESS_CLOSE_HANDLE,
	// process memory
	OT_PROCESS_MEMORY_ENUM,
	OT_PROCESS_MEMORY_ZERO,
	OT_PROCESS_MEMORY_CHANGE_PROTECTION,
	OT_PROCESS_MEMORY_FREE,
	// process handle
	OT_PROCESS_HANDLE_ENUM,
	OT_PROCESS_HANDLE_CLOSE,
	//
	OT_PROCESS_ENUMERATE_WND,
	OT_PROCESS_ENUMERATE_TIMER,
	OT_PROCESS_REMOVE_TIMER,
	OT_PROCESS_MODIFY_TIMEOUT,
	OT_PROCESS_ENUMERATE_HOTKEY,
	OT_PROCESS_REMOVE_HOTKEY,
	OT_PROCESS_ENUMERATE_PROCESS_PRIVILEGE,
	OT_PROCESS_ADJUST_TOKEN_PRIVILEGE,
	// Driver
	OT_DRIVER_ENUMERATE,
	// Registry
	OT_REGISTRY_ENUMERATE_KEY,
	OT_REGISTRY_ENUMERATE_VALUE_KEY,
	// Kernel
	OT_KERNEL_ENUMERATE_NOTIFY,
	// Hook
	OT_HOOK_SSDT,
	OT_HOOK_SHADOW_SSDT,
	// Memory read/write
	OT_MEMORY_READ,
}OPERATION_TYPE;

/************************************************************************/
/* Define communication structure                                       */
/************************************************************************/
//
// process information
//
typedef struct _PROCESS_DATA{
	size_t		nPid;
	size_t		nParentPid;
	ULONG_PTR	pEProcess;
	size_t		bHide;			// 0 - not be hidden; 1 - be hidden.
	WCHAR		pwszPath[SZ_PATH_LEN];
}PROCESS_DATA, *PPROCESS_DATA;
//
// process list
// 
typedef struct _HG_PROCESS{
	OPERATION_TYPE	op_type;
	ULONG			ulProcessCount;
	PPROCESS_DATA	pProcessHead;
}HG_PROCESS, *PHG_PROCESS;
//
// 
// 
typedef struct _PROCESS_MODULE_DATA{
	SIZE_T			Base;
	SIZE_T			Size;
	WCHAR			Path[SZ_PATH_LEN];
}PROCESS_MODULE_DATA, *PPROCESS_MODULE_DATA;
//
// 
// 
typedef struct _PROCESS_THREAD_DATA 
{
	SIZE_T	pEThread;
	SIZE_T	Tid;
	SIZE_T	Teb;
	SIZE_T	Priority;
	SIZE_T	Win32StartAddress;
	SIZE_T	StartAddress;
	SIZE_T	SwitchCount;
	SIZE_T	State;
}PROCESS_THREAD_DATA, *PPROCESS_THREAD_DATA;
//
// 
// 
typedef struct _PROCESS_MEMORY_DATA
{
	SIZE_T	Base;
	SIZE_T	Size;
	SIZE_T	Protect;
	SIZE_T	State;
	SIZE_T	Type;
}PROCESS_MEMORY_DATA, *PPROCESS_MEMORY_DATA;
//
// Driver Information
// 
typedef enum _HG_DRIVER_TYPE{
	eDT_Hide,
	eDT_HaveOrder,
	eDT_SuspiciousPe,
}HG_DRIVER_TYPE;
typedef struct _DRIVER_DATA 
{
	HG_DRIVER_TYPE	DriverType;
	SIZE_T			LoadOrder;
	SIZE_T			Base;
	SIZE_T			Size;
	SIZE_T			DriverObject;
	WCHAR			DriverPath[SZ_PATH_LEN];
}DRIVER_DATA, *PDRIVER_DATA;
//
// Callback notify routine
// 
typedef enum _NOTIFY_TYPE{
	eNT_CreateProcess,
	eNT_CreateThread,
	eNT_LoadImage,
	eNT_Shutdown,
	eNT_CmCallBack
}NOTIFY_TYPE;
typedef struct _NOTIFY_DATA 
{
	NOTIFY_TYPE nt;
	SIZE_T		pCallbackAddress;
	SIZE_T		Comment;
}NOTIFY_DATA, *PNOTIFY_DATA;
//
// SSDT
// 
typedef struct _SDT_DATA
{
	SIZE_T	Hint;
	PVOID	OriginalAddress;
	PVOID	CurrentAddress;
	PVOID	AddressOfHook;
	SIZE_T	OffsetOfHook;
	WCHAR	Name[64];
}SDT_DATA, *PSDT_DATA;
//
// Shadow SSDT
// 
typedef struct _SHADOW_SDT_DATA
{
	SIZE_T	Hint;
	PVOID	OriginalAddress;
	PVOID	CurrentAddress;
	PVOID	AddressOfHook;
	SIZE_T	OffsetOfHook;
}SHADOW_SDT_DATA, *PSHADOW_SDT_DATA;
//
// IDT
// 
typedef struct _IDT_DATA 
{
	ULONG32	Index;
	SIZE_T	CurrentAddress;
	SIZE_T	OriginalAddress;
	SIZE_T	HookAddress;
}IDT_DATA, PIDT_DATA;
// 
// Handle
// 
typedef struct _HANDLE_DATA
{
	SIZE_T	Handle;
	SIZE_T	Object;
	SIZE_T	ReferenceCount;
	WCHAR	ObjectName[64];
	WCHAR	HandleName[MAX_PATH];
}HANDLE_DATA, *PHANDLE_DATA;
//
// define I/O Transmission Packet type between ring0 and ring3
// 
typedef struct _IO_TRANSMISSION_PACKET{
	OPERATION_TYPE	op_type;

	//
	// data
	// 
	union{
		//
		// process
		// 
		union {
			struct {
				SIZE_T			nCount;
				SIZE_T			nRealCount;
				PROCESS_DATA	pProcess[1];	
			}process;
		}DUMMYSTRUCTNAME;
		//
		// process module
		//
		union {
			struct {
				SIZE_T				nProcessId;
				SIZE_T				pEProcess;
			}in_process_module;
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				PROCESS_MODULE_DATA	pModule[1];
			}out_process_module;
		}DUMMYSTRUCTNAME;
		//
		// process thread
		//
		union {
			struct {
				SIZE_T				nProcessId;
				SIZE_T				pEProcess;
			}in_process_thread;
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				PROCESS_THREAD_DATA	pThread[1];
			}out_process_thread;
		}DUMMYSTRUCTNAME;
		//
		// Process memory
		//
		union {
			struct {
				SIZE_T				nPId;
				SIZE_T				pEProcess;
				SIZE_T				Base;
				SIZE_T				Size;
				SIZE_T				ModifyType;
			}in_process_memory;
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				PROCESS_MEMORY_DATA	MemoryBlocks[1];
			}out_process_memory;
		}DUMMYSTRUCTNAME;
		//
		// driver
		// 
		union {
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				DRIVER_DATA			pDriver[1];
			}driver;
		}DUMMYSTRUCTNAME;
		//
		// registry
		// 
		union {
			struct {
				WCHAR				Path[SZ_PATH_LEN];
			}in_registry;
			struct {
				SIZE_T				nMaxLength;
				WCHAR				Keys[1];
			}out_registry;
		}DUMMYSTRUCTNAME;
		//
		// callback notify routine
		// 
		union {
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				NOTIFY_DATA			NotifyData[1];
			}notify;
		}DUMMYSTRUCTNAME;
		//
		// SSDT
		// 
		union {
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				SDT_DATA			sdt[1];
			}sdt;
		}DUMMYSTRUCTNAME;
		//
		// Shadow SSDT
		// 
		union {
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				SHADOW_SDT_DATA		shadow_sdt[1];
			}shadow_sdt;
		}DUMMYSTRUCTNAME;
		//
		// IDT
		// 
		union {
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				IDT_DATA			idt[1];
			}idt;
		}DUMMYSTRUCTNAME;
		//
		// Memory read
		// 
		union {
			struct {
				SIZE_T				Base;
				SIZE_T				nLength;
				SIZE_T				pEProcess;
				BYTE				MemoryBlock[1];
			}mem;
		}DUMMYSTRUCTNAME;
		//
		// Process Handle
		// 
		union {
			struct {
				SIZE_T				Pid;
				SIZE_T				EProcess;
			}get_process_handles_params;
			struct {
				SIZE_T				nCount;
				SIZE_T				nMaxCount;
				HANDLE_DATA			HandleData[1];
			}get_process_handles;
			struct {
				SIZE_T				Pid;
				SIZE_T				EProcess;
				SIZE_T				Handle;
				SIZE_T				HandleObject;
				SIZE_T				IsForceKill;	// 0 - FALSE; 1 - TRUE.l
			}close_process_handle_params;
		}DUMMYSTRUCTNAME;
	}DUMMYSTRUCTNAME;
}ITP, *PITP;

#endif