#ifndef __HGLIB_H__
#define __HGLIB_H__

#include "hgdef.h"
#include "hgexport.h"

#define WRK

extern ULONG_PTR g_hg_map[HG_MAP_LENGTH];

PVOID
GetFunctionAddressByName(
	WCHAR* pwszName
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/mm/sysload.c
// Prototype: MmGetSystemRoutineAddress
// 
/************************************************************************/
WRK
PVOID
HgMmGetSystemRoutineAddress(
	IN	CHAR* SystemRoutineName
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/mm/sysload.c
// Prototype: MiFindExportedRoutineByName
// 
/************************************************************************/
WRK
PVOID
HgMiFindExportedRoutineByName(
	IN	PVOID DllBase, 
	IN	CHAR* RoutineName
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/imagedir.c
// Prototype: RtlImageDirectoryEntryToData
// 
/************************************************************************/
WRK
PVOID
HgRtlImageDirectoryEntryToData(
	IN PVOID Base, 
	IN BOOLEAN MappedAsImage, 
	IN USHORT DirectoryEntry, 
	OUT PULONG Size 
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/rtlnthdr.c
// Prototype: RtlImageNtHeader
// 
/************************************************************************/
WRK
PIMAGE_NT_HEADERS
NTAPI
HgRtlImageNtHeader(
	IN	PVOID	Base
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/rtlnthdr.c
// Prototype: RtlImageNtHeaderEx
// 
/************************************************************************/
WRK
NTSTATUS
NTAPI
HgRtlImageNtHeaderEx(
	IN	ULONG	Flags,
	IN	PVOID	Base,
	IN	ULONG64	Size,
	OUT	PIMAGE_NT_HEADERS * OutHeaders
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/imagedir.c
// Prototype: RtlpImageDirectoryEntryToDate32
// 
/************************************************************************/
WRK
PVOID
HgRtlpImageDirectoryEntryToData32(
	IN	PVOID	Base,
	IN	BOOLEAN	MappedAsImage,
	IN	USHORT	DirectoryEntry,
	OUT	PULONG	Size,
	PIMAGE_NT_HEADERS32 NtHeaders
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/imagedir.c
// Prototype: RtlpImageDirectoryEntryToDate64
// 
/************************************************************************/
WRK
PVOID
HgRtlpImageDirectoryEntryToData64(
	IN	PVOID	Base,
	IN	BOOLEAN	MappedAsImage,
	IN	USHORT	DirectoryEntry,
	OUT	PULONG	Size,
	PIMAGE_NT_HEADERS64 NtHeaders
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/imagedir.c
// Prototype: RtlAddressInSectionTable
// 
/************************************************************************/
WRK
PVOID
HgRtlAddressInSectionTable(
	IN	PIMAGE_NT_HEADERS NtHeaders,
	IN	PVOID	Base,
	IN	ULONG	Address
	);

/************************************************************************/
// This function is from WRK-v1.2
// File: WRK-v1.2/base/ntos/rtl/imagedir.c
// Prototype: RtlSectionTableFromVirtualAddress
// 
/************************************************************************/
WRK
PIMAGE_SECTION_HEADER
SzhRtlSectionTableFromVirtualAddress(
	IN	PIMAGE_NT_HEADERS NtHeaders,
	IN	PVOID	Base,
	IN	ULONG	Address
	);




__inline CHAR ChangePreMode(PETHREAD pThread){
	ULONG_PTR PreModeOffset = GetOffset(_KTHREAD_PreviousMode);
	CHAR PreMode = *(PCHAR)((ULONG_PTR)pThread + PreModeOffset);
	*(PCHAR)((ULONG_PTR)pThread + PreModeOffset) = KernelMode;
	return PreMode;
}

__inline VOID RecoverPreMode(PETHREAD pThread, CHAR PreMode){
	ULONG_PTR PreModeOffset = GetOffset(_KTHREAD_PreviousMode);
	*(PCHAR)((ULONG_PTR)pThread + PreModeOffset) = PreMode;
}



NTSTATUS
GetDriverObject(
	__in	PWCHAR			pwszDriverName,
	__in	PDRIVER_OBJECT*	pDriverObject
	);
VOID
SzhInitUnicodeString(
	__inout	PUNICODE_STRING DestinationString,
	__in	PCWSTR			SourceString
	);

BOOL
FindIatFunction(
	__in	PVOID	pBase,
	__in	ULONG_PTR	ulModuleSize,
	__in	PCHAR	ImportDllName,
	__in	PCHAR	ImportApiName,
	__out	PULONG_PTR	ImportFunOffset
	);

BOOL 
GetDriverEntryPoint(
	__in	PVOID				ImageBase,
	__out	PULONG_PTR			pOutDriverEntry,
	__in	ENTRY_POINT_TYPE	EntryType
	);

VOID
FixOriginDispatch(
	__in	PULONG_PTR	pOriginDispatchs
	);

VOID
GetIopInvalidDeviceRequest(
	);

VOID
FixReloadDispatch(
	__in	PULONG_PTR	pReloadDispatchs
	);

BOOL
IsUnicodeStringValid(
	__in	PUNICODE_STRING	uString
	);

size_t
GetUnicodeStringSafeLength(
	__in	PUNICODE_STRING	uString,
	__in	USHORT			nLen
	);

/************************************************************************/
/* KeGetObjectType                                                      */
/************************************************************************/
POBJECT_TYPE
KeGetObjectType(
	__in	PVOID	pObject
	);

BOOL
GetNtoskrnlInfo(
	__out	WCHAR*	pwszNtOSKrnlPath,
	__out	PSIZE_T	SystemKernelModuleBase,
	__out	PSIZE_T	SystemKernelModuleSize
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL ImagePEFile(
	__in	PVOID	FileBuffer,
	__inout	PVOID*	ImageModuleBase
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL FixImportTable(
	__in	PBYTE	ImageBase,
	__in	SIZE_T	ExistImageBase
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
PVOID GetKernelModuleBase(
	__in	PCHAR	KernelModuleName
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL InsertOriginalFirstThunk(
	__in	SIZE_T				ImageBase,
	__in	SIZE_T				ExistImageBase,
	__out	PIMAGE_THUNK_DATA	FirstThunk
	);

/************************************************************************/
// WRK-v1.2
// sysload.c 
/************************************************************************/
PVOID MiFindExportedRoutine(
	__in	PVOID	DllBase,
	__in	BOOL	ByName,
	__in	PCHAR	RoutineName,
	__in	DWORD	Ordinal
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
BOOL FixBaseRelocTable(
	__in	PVOID	NewImageBase,
	__out	SIZE_T	ExistImageBase
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
PIMAGE_BASE_RELOCATION
LdrProcessRelocationBlockLongLong(
	__in	ULONG_PTR	VA,
	__in	ULONG		SizeOfBlock,
	__in	PUSHORT		NextOffset,
	__in	LONGLONG	Diff
	);

/************************************************************************/
/* return Destination address                                                */
/************************************************************************/
SIZE_T RelativeAddressToAbsoluteAddress(
	__in	SIZE_T	CurrentAddress,
	__in	SSIZE_T	Offset,
	__in	ULONG32	CurrentInstructionSize
	);

/************************************************************************/
/*                                                                      */
/************************************************************************/
PVOID LookupKernelModuleByNameA(
	__in	PCHAR	pszModuleName,
	__in	PULONG32	nModuleSize);

#endif