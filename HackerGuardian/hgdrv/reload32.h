#ifndef __RELOAD32_H__
#define __RELOAD32_H__

#include <ntifs.h>
#include <WinDef.h>
#include <ntimage.h>

#include "hglib.h"
#include "hgdef.h"
#include "hgexport.h"
#include "irpfile.h"
#include "libdasm.h"

BOOL
ReloadNtoskrnl32(
);

BOOL 
PeLoad_Unrepair(
	__in	PWCHAR	pwszPath,
	__out	PSIZE_T	pNewBase
	);

NTSTATUS
KernelOpenFile(
	__in	PWCHAR		pwszFilePath,
	__out	PHANDLE		FileHandle,
	__in	ACCESS_MASK	DesiredAccess,
	__in	ULONG		FileAttributes,
	__in	ULONG		ShareAccess,
	__in	ULONG		CreateDisposition,
	__in	ULONG		CreateOptions
	);

BOOL
GetWindowsRootName(
	__in	WCHAR* pwszRootName
	);

BOOL
GetDeviceObjectFromFileFullName(
	__in	PWCHAR			FileFullName,
	__out	PDEVICE_OBJECT*	RealDevice,
	__out	PDEVICE_OBJECT*	DeviceObject
	);

BOOL
IoGetFileSystemVpbInfo(
	__in	PFILE_OBJECT	FileObject,
	__out	PDEVICE_OBJECT*	DeviceObject,
	__out	PDEVICE_OBJECT* RealDevice
	);

NTSTATUS
KernelGetFileSize(
	__in	HANDLE			hFile,
	__out	PLARGE_INTEGER	FileSize
	);

NTSTATUS
KernelReadFile(
	__in	HANDLE				hFile,
	__out	PLARGE_INTEGER		ByteOffset,
	__out	ULONG				Length,
	__out	PVOID				FileBuffer,
	__out	PIO_STATUS_BLOCK	IoStatusBlock
	);

BOOL
PeLoad_Repair(
	__in	PWCHAR	pwszFilePath,
	__out	PSIZE_T	NewBase,
	__in	SIZE_T	OriginBase
	);

BOOL
GetOriginalKiServiceTable(
	__in	SIZE_T	NewImageBase,
	__in	SIZE_T	ExistImageBase,
	__out	PSIZE_T	NewKiServiceTable
	);

BOOL
FixOriginalKiServiceTable(
	__in	PSIZE_T	OriginKiServiceTable,
	__out	PSIZE_T	ReloadKiServiceTable,
	__in	SIZE_T	NewBase,
	__in	SIZE_T	OldBase
	);

PVOID
LookupKernelModuleByNameW(
	__in	PWCHAR	pwszModuleName,
	__out	PDWORD	nModuleSize
	);

BOOL
ReloadHalDll(
	);

PEPROCESS
LookupProcessByName(
	__in	PWCHAR	ProcessName
	);

PKSERVICE_TABLE_DESCRIPTOR
	GetAddrOfShadowTable(
	);

BOOL
InitReloadWin32K(
	__in	PKSERVICE_TABLE_DESCRIPTOR	ShadowSSDT,
	__in	SIZE_T						OldWin32kBase
	);

BOOL
GetOriginalW32pTable(
	__in	PVOID	NewWin32kBase,
	__in	DWORD	OldWin32kBase,
	__out	PDWORD*	W32pTable
	);

size_t
GetProcFromIAT(
	__in	PVOID	ImageBase,
	__in	PCHAR	pszProcName
	);

NTSTATUS
GetRawShadowAddr(
	__in	PKSERVICE_TABLE_DESCRIPTOR	ShadowSSDTAddr,
	__in	PVOID						BaseOfwin32k_sys,
	__out	PVOID*						pOutShadowServiceTable
	);

BOOL 
ReloadWin32k(
	);

VOID
InitShadowSSDT(
	);

BOOLEAN
IsSafeAddress(
	__in	PVOID	Address,
	__in	ULONG	Size
	);

VOID
InitNtfs(
);

VOID
InitFastfat(
	);

#endif