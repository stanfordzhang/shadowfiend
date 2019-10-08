
#ifndef __REGISTRY_H__
#define __REGISTRY_H__

#include "stdhg.h"
#include <ntifs.h>
#include "hgioctl.h"
#include "hgdef.h"
#include "hgexport.h"
#include "hgbase.h"
#include "hglib.h"

NTSTATUS RegistryEnumerateKey(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__in	SIZE_T	nOutSize,
	__inout	PSIZE_T	ReturnSize);

NTSTATUS RegistryEnumerateValueKey(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__in	SIZE_T	nOutSize,
	__inout	PSIZE_T	ReturnSize);

#endif