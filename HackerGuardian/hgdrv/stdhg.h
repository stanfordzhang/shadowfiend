#ifndef __STDHG_H__
#define __STDHG_H__

#include <ntifs.h>
#include <windef.h>
#include <ntiologc.h>
#include <ntimage.h>

#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */

#if defined(_WIN64)
#include "../references/disassembler/beaengine-win64/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win64\\Win64\\Lib\\BeaEngine64.lib")
#else
#include "../references/disassembler/beaengine-win32/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win32\\Win32\\Lib\\BeaEngine.lib")
#endif


//#define DBG_DRIVER_ENTRY
#define DBG_INIT
#define DBG_PROCESS
//#define DBG_EXPORT
//#define DBG_PE
#define DBG_RELOAD32
#define DBG_RELOAD64
#define DBG_LIB
#define DBG_DRV_SYS
#define DBG_REG
#define DBG_NOTIFY
#define DBG_SDT
#define DBG_IDT

//
// for hgdrv.c
// 
#ifdef	DBG_DRIVER_ENTRY
#define	DeLog(_x_) KdPrint(_x_)
#else
#define DeLog(_x_)
#endif

//
// for process.c
// 
#ifdef	DBG_INIT
#define	InitLog(_x_) KdPrint(_x_)
#else
#define InitLog(_x_)
#endif

//
// for process.c
// 
#ifdef	DBG_PROCESS
#define	ProcessLog(_x_) KdPrint(_x_)
#else
#define ProcessLog(_x_)
#endif

//
// for peload.c
// 
#ifdef	DBG_PE
#define	PELog(_x_) KdPrint(_x_)
#else
#define PELog(_x_)
#endif

//
// hgprocess.c
// 
#ifdef	DBG_PROCESS
#define	ProcessLog(_x_) KdPrint(_x_)
#else
#define ProcessLog(_x_)
#endif

//
// hgexport.h
// 
#ifdef	DBG_EXPORT
#define	ExportLog(_x_) KdPrint(_x_)
#else
#define ExportLog(_x_)
#endif

//
// reload32.h
// 
#ifdef	DBG_RELOAD32
#define	Reload32Log(_x_) KdPrint(_x_)
#else
#define Reload32Log(_x_)
#endif

//
// reload64.h
// 
#ifdef	DBG_RELOAD64
#define	Reload64Log(_x_) KdPrint(_x_)
#else
#define Reload64Log(_x_)
#endif

//
// hglib.h
// 
#ifdef	DBG_LIB
#define	LibLog(_x_) KdPrint(_x_)
#else
#define LibLog(_x_)
#endif

//
// drvsys.h
// 
#ifdef	DBG_DRV_SYS
#define	DrvLog(_x_) KdPrint(_x_)
#else
#define DrvLog(_x_)
#endif

//
// registry.h
// 
#ifdef	DBG_REG
#define	RegLog(_x_) KdPrint(_x_)
#else
#define RegLog(_x_)
#endif

//
// hgnotify.h
// 
#ifdef	DBG_NOTIFY
#define	NotifyLog(_x_) KdPrint(_x_)
#else
#define NotifyLog(_x_)
#endif

//
// sdt.c
// 
#ifdef	DBG_SDT
#define	SdtLog(_x_) KdPrint(("%s(%d): ", __FILE__, __LINE__));KdPrint(_x_)
#else
#define SdtLog(_x_)
#endif

//
// idt.c
// 
#ifdef	DBG_IDT
#define	IdtLog(_x_) KdPrint(("%s(%d): ", __FILE__, __LINE__));KdPrint(_x_)
#else
#define IdtLog(_x_)
#endif

#endif