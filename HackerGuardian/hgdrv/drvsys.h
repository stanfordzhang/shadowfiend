#ifndef __DRV_SYS_H__
#define __DRV_SYS_H__

#include "stdhg.h"
#include <ntifs.h>
#include "hgioctl.h"
#include "hgdef.h"
#include "hgexport.h"
#include "hgbase.h"
#include "hglib.h"

NTSTATUS EnumDriver(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	SIZE_T	nOutSize,
	__out	SIZE_T*	pNumberOfBytesReturned);

VOID EnumDriver_LDR_DATA_TABLE_ENTRY(
	__inout	PITP	pItp);

VOID EnumDriver_DeviceObjectType(
	__inout	PITP	pItp);

VOID AddDriverToItp(
	__in	PDRIVER_OBJECT	DrvObj,
	__inout	PITP			pItp);

BOOL IsDriverExist(
	__in	PDRIVER_OBJECT	DrvObj,
	__inout	PITP			pItp);

VOID EnumDriver_DriverObjectType(
	__inout	PITP	pItp);

VOID EnumDriver_ObjectDirectory(
	__inout	PITP	pItp);

VOID
TraverseObjectDirectory(
	__in	PVOID	pDirectoryObject,
	__inout	PITP	pItp
	);

VOID
EnumDriver_Memory(
	__inout	PITP	pItp
	);

BOOL
IsDriverObject(
	__in	PDRIVER_OBJECT DriverObject
	);

VOID
RepairDriverPath(
	__inout	PITP	pItp
	);

#endif