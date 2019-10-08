
#include "hgheader.h"

NTSTATUS EnumFSDDispatch(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize)
{
	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pSdtItp = (PITP)pOutBuffer;

	return status;
}