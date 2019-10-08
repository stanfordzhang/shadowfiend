
#ifndef __HG_PROCESS_H__
#define __HG_PROCESS_H__

#include "stdhg.h"
#include <ntifs.h>
#include "hgioctl.h"
#include "hgdef.h"
#include "hgexport.h"
#include "hgbase.h"
#include "hglib.h"

/************************************************************************/
/* Enumerate processes                                                                     */
/************************************************************************/
NTSTATUS
EnumProcess(
	__in	PVOID	pInBuffer,
	__in	ULONG	uInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	uOutSize,
	__out	ULONG*	dwRet
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
NTSTATUS
EnumProcess_NtQuerySystemInformation(
	__in	PITP	pItp
	);

/************************************************************************/
/* Lookup System Idle Process process                                     */
/************************************************************************/
VOID
LookupIdleProcess(
	__inout	PITP	pItp
	);

/************************************************************************/
/* Lookup System process                                                */
/************************************************************************/
VOID 
LookupSystemProcess(
	__inout	PITP	pItp
	);

/************************************************************************/
/* Get process image path by EPROCESS                                   */
/************************************************************************/
BOOL
GetProcessImagePathByEProcess(
	__in	PEPROCESS	pEProcess,
	__out	PWCHAR		pwszPath
	);

/************************************************************************/
/* Get process image path by _SECTION_OBJECT                             */
/************************************************************************/
BOOL
GetProcessImagePathBySectionObject(
	__in	PSECTION_OBJECT	pSectionObject,
	__out	PWCHAR			pwszPath
	);

/************************************************************************/
/* Get process image path by _FILE_OBJECT                               */
/************************************************************************/
BOOL
GetProcessImagePathByFileObject(
	__in	PFILE_OBJECT	pFileObject,
	__out	PWCHAR			pwszPath
	);

/************************************************************************/
/* Get process image path by PEB                                        */
/************************************************************************/
BOOL
GetProcessImagePathByPeb32(
	__in	PEPROCESS		pEProcess,
	__out	PWCHAR			pwszPath
	);

/************************************************************************/
/* Get process image path by PEB                                        */
/************************************************************************/
BOOL
GetProcessImagePathByPeb64(
	__in	PEPROCESS		pEProcess,
	__out	PWCHAR			pwszPath
	);

/************************************************************************/
/* Enumerate process by PspCidTable                                     */
/************************************************************************/
VOID
EnumProcess_PspCidTable(
	__in	PITP	pItp
	);

/************************************************************************/
/* Get PspCidTable                                                      */
/************************************************************************/
VOID
GetPspCidTable(
	VOID
	);

/************************************************************************/
/* Get Win8 PspCidTable                                                 */
/************************************************************************/
VOID
GetPspCidTableByLookupFunction_Win8(
	VOID
	);

/************************************************************************/
/* Get PspCidTable                                                      */
/************************************************************************/
BOOL
GetPspCidTableByLookupFunction(
	VOID
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL
GetPspCidTableByLookupFunction64(
	VOID
	);

/************************************************************************/
/* Get PspCidTable by _KPCR.KdVersionBlock                              */
/************************************************************************/
VOID
GetPspCidTableByKdVersionBlock(
	VOID
	);

/************************************************************************/
/* Insert DPC to find KdVersionBlock                                    */
/************************************************************************/
VOID
InsertDpcToFindKdVersionBlock(
	VOID
	);

//
//
// 
VOID
GetKdVersionBlock(
	VOID
	);

/************************************************************************/
/* DPC routine                                                          */
/************************************************************************/
VOID
DpcRoutine(
	__in		struct _KDPC	*dpc,
	__in_opt	PVOID			DeferredContext,
	__in_opt	PVOID			SystemArgument1,
	__in_opt	PVOID			SystemArgument2
	);

/************************************************************************/
/* Scan CidTable                                                        */
/************************************************************************/
VOID
ScanCidTable(
	__in	ULONG_PTR	uHandleTable,
	__in	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
ScanCidTable_Level_0(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
ScanCidTable_Level_1(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
ScanCidTable_Level_2(
	__in	ULONG_PTR	TableAddr,
	__inout	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
ThreadToProcess(
	__in	PETHREAD	pThread,
	__inout	PITP		pItp
	);

/************************************************************************/
/* Insert Process to buffer                                             */
/************************************************************************/
VOID
InsertProcessToBuffer(
	__in	PEPROCESS	pEProcess,
	__in	PITP		pItp
	);

/************************************************************************/
/* Inspect EPROCESS                                                     */
/************************************************************************/
BOOL
IsRealProcess(
	__in	PEPROCESS pEProcess
	);

/************************************************************************/
/* Check the process whether the death                                  */
/************************************************************************/
BOOL
IsProcessDead(
	__in	PEPROCESS	pEProcess
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL
IsProcessExist(
	__in	PEPROCESS	pEProcess,
	__in	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL
GetProcessImagePathBy_SE_AUDIT_PROCESS_CREATION_INFO_64(
	__in	PEPROCESS	pEProcess,
	__out	PWCHAR		pwszPath
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
EnumProcessByThreadList(
	__in	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
GetWaitListHead_DispatcherReadyListHead(
	VOID
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID
TraverseThreadList(
	__in	PLIST_ENTRY		pList,
	__inout	PITP			pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
NTSTATUS EnumProcessModule(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pRtnSize
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID EnumProcessModule_PEB(
	__in	PEPROCESS pEProcess,
	__inout	PITP pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
#define		LoadOrderLinks				0
#define		MemoryOrderLinks			1
#define		InitializationOrderLinks	2
VOID TraverseModuleList(
	__in	PLIST_ENTRY pList, 
	__in	SIZE_T nType, 
	__inout	PITP pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL IsModuleExist(
	__in	SIZE_T Base, 
	__in	SIZE_T Size, 
	__in	PITP pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID EnumProcessModule_VadRoot(
	__in	PEPROCESS pEProcess,
	__inout	PITP pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseVadTree_XP(
	__in	PMMVAD_XP	root,
	__inout	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseVadTree32_Win7(
	__in	PMMVAD32_WIN7	root,
	__inout	PITP		pItp
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseVadTree32_Win8(
	__in	PMMVAD_WIN8	root,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseVadTree64_Win7(
	__in	PMMVAD64_WIN7	root,
	__inout	PITP			pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseVadTree64_Win8(
	__in	PMMVAD64_WIN8	root,
	__inout	PITP			pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
NTSTATUS EnumProcessThread(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pRtnSize);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID EnumProcessThread_ThreadList(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID EnumProcessThread_PspCidTable(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID EnumProcessThread_WaitThreadList(
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID AddThreadDataToItp(
	__in	PETHREAD	pEThread,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ContainThreadData(
	__in	PETHREAD pEThread, 
	__inout	PITP pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID ScanCidTableToFindThread(
	__in	SIZE_T		PspCidTable, 
	__in	PEPROCESS	pEProcess, 
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID ScanCidTableToFindThread_Level_0(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID ScanCidTableToFindThread_Level_1(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID ScanCidTableToFindThread_Level_2(
	__in	SIZE_T		TableCode,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

/************************************************************************/
/*                                                                      */
/************************************************************************/
VOID TraverseProcessThreadList(
	__in	PLIST_ENTRY	pList,
	__in	PEPROCESS	pEProcess,
	__inout	PITP		pItp);

#endif