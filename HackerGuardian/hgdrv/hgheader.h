
#ifndef __HG_HEADER_H__
#define __HG_HEADER_H__

#include "stdhg.h"
#include <ntifs.h>
#include "hgioctl.h"
#include "hgdef.h"
#include "hgexport.h"
#include "hgbase.h"
#include "hglib.h"

//////////////////////////////////////////////////////////////////////////
// Process Memory
// 
NTSTATUS EnumProcessMemory(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize,
	__out	ULONG*	pReturnLength
	);

NTSTATUS ProcessMemoryZeroBlock(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize);

NTSTATUS ProcessMemoryChangeProtection(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize);

NTSTATUS ProcessMemoryFree(
	PVOID pInBuffer, 
	ULONG nInSize, 
	PVOID pOutBuffer, 
	ULONG nOutSize);

//////////////////////////////////////////////////////////////////////////
// Process Handles
// 
NTSTATUS EnumProcessHandle(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pReturnLength);

NTSTATUS CloseProcessHandle(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pReturnLength);

//////////////////////////////////////////////////////////////////////////
// SSDT
// 
NTSTATUS EnumSSDT(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize);

NTSTATUS EnumShadowSSDT(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize);

NTSTATUS EnumFSDDispatch(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize);

NTSTATUS EnumIdtHook(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize);

NTSTATUS ReadProcessMemoryAux(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize);

#if defined(_WIN64)
SIZE_T CalculateFunctionAddressFromSDT(SIZE_T sdt, SIZE_T Ordinal);
#endif

#endif