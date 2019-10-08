#ifndef __HG_NOTIFY_H__
#define __HG_NOTIFY_H__

#include "stdhg.h"
#include <ntifs.h>
#include "hgioctl.h"
#include "hgdef.h"
#include "hgexport.h"
#include "hgbase.h"
#include "hglib.h"

NTSTATUS EnumNotify(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize,
	__out	ULONG*	pReturnLength);

void EnumCreateProcessNotify(PITP pItp);
void GetPspCreateProcessNotifyRoutine();
SIZE_T GetPsSetCreateProcessNotifyRoutineAux(
	__in	SIZE_T PsSetCreateProcessNotifyRoutineAddress);

SIZE_T LookupPspSetXNotifyRoutineAddress(
	__in	PVOID	PsSetXNotifyRoutineAddress,
	__in	PVOID	KnownAddress,
	__in	NOTIFY_TYPE	Type);

SIZE_T FindNotifyBase(
	__in	SIZE_T	Base,
	__in	SIZE_T	Count,
	__in	SIZE_T	MatchingAddress);

void EnumCreateThreadNotify(PITP pItp);
void GetPspCreateThreadNotifyRoutine();

void EnumLoadImageNotify(PITP pItp);
void GetPspLoadImageNotifyRoutine();

void EnumRegistryNotify(PITP pItp);
void GetCmpCallBackVector();

void EnumShutdownNotify(PITP pItp);
void GetIopNotifyShutdownQueueHead();
SIZE_T GetShutdownDispatch(PDEVICE_OBJECT DeviceObject);

#endif