
#ifndef __PE_H__
#define __PE_H__

#include <Windows.h>
#include <wchar.h>
#include <tchar.h>

#define		MEMORY_TYPE_RAW		0x00000001
#define		MEMORY_TYPE_VIRTUAL	0x00000002

BOOL 
GetDosHeader(
	__in	PVOID Base, 
	__out	PIMAGE_DOS_HEADER* DosHeader
	);

BOOL 
GetNtHeader32(
	__in	PVOID Base,
	__out	PIMAGE_NT_HEADERS32* NtHeader
	);

BOOL 
GetNtHeader64(
	__in	PVOID Base,
	__out	PIMAGE_NT_HEADERS64* NtHeader
	);

void
GetMachineString(
	__in	WORD	Machine,
	__out	PWCHAR	s
	);

void
GetSubsystemString(
	__in	WORD	Subsystem,
	__out	PWCHAR	s
	);

BOOL
GetSections32(
	__in	PVOID	Base,
	__out	PIMAGE_SECTION_HEADER* SectionHeader, 
	__out	PDWORD NumberOfSections
	);

BOOL
GetSections64(
	__in	PVOID	Base,
	__out	PIMAGE_SECTION_HEADER* SectionHeader, 
	__out	PDWORD NumberOfSections
	);

DWORD 
RVAToOffset32(
	__in	DWORD dwRVA, 
	__in	PIMAGE_NT_HEADERS32 NtHeader32, 
	__in	PIMAGE_SECTION_HEADER SectionHeader
	);

DWORD 
OffsetToRVA32(
	__in	DWORD dwOffset,
	__in	PIMAGE_NT_HEADERS32 NtHeader32,
	__in	PIMAGE_SECTION_HEADER SectionHeader
	);

DWORD 
RVAToOffset64(
	__in	DWORD dwRVA, 
	__in	PIMAGE_NT_HEADERS64 NtHeader64, 
	__in	PIMAGE_SECTION_HEADER SectionHeader
	);

DWORD 
OffsetToRVA64(
	__in	DWORD dwOffset,
	__in	PIMAGE_NT_HEADERS64 NtHeader64,
	__in	PIMAGE_SECTION_HEADER SectionHeader
	);

void 
GetPEFileType(
	__in	WORD Magic,
	__out	PWCHAR	s
	);

BOOL 
GetExportDirectory32(
	__in	PVOID Base,
	__out	PIMAGE_EXPORT_DIRECTORY* ExportDirectory
	);

BOOL 
GetExportDirectory64(
	__in	PVOID Base,
	__out	PIMAGE_EXPORT_DIRECTORY* ExportDirectory
	);

BOOL 
GetBaseRelocationDirectory(
	__in	PVOID Base,
	__out	PIMAGE_BASE_RELOCATION* BaseReloc
	);

#endif