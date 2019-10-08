
#ifndef __RELOAD64_H__
#define __RELOAD64_H__

#include <ntifs.h>
#include <WinDef.h>
#include <ntimage.h>

#include "hglib.h"
#include "hgdef.h"
#include "hgexport.h"

BOOL
ReloadKernel64(
	VOID
	);

VOID LookupSsdtAddress64(
	__in	PVOID	Base
	);

BOOL
ImageKernel64(
	__in	PWCHAR	KernelPath,
	__inout	PSIZE_T	ImageAddress,
	__in	SIZE_T	OriginalBase
	);

#endif