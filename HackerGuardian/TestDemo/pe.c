

#include "pe.h"

BOOL GetDosHeader(PVOID Base, PIMAGE_DOS_HEADER* DosHeader)
{
	BOOL bRtn = FALSE;
	if (Base == NULL || DosHeader == NULL){
		return FALSE;
	}

	*DosHeader = (PIMAGE_DOS_HEADER)Base;

	return TRUE;
}

BOOL GetNtHeader32(PVOID Base, PIMAGE_NT_HEADERS32* NtHeader)
{
	BOOL bRtn = FALSE;

	if (Base == NULL || NtHeader == NULL){
		return FALSE;
	}

	*NtHeader = (PIMAGE_NT_HEADERS32)((SIZE_T)Base + ((PIMAGE_DOS_HEADER)Base)->e_lfanew);

	return TRUE;
}

BOOL GetNtHeader64(PVOID Base, PIMAGE_NT_HEADERS64* NtHeader)
{
	BOOL bRtn = FALSE;

	if (Base == NULL || NtHeader == NULL){
		return FALSE;
	}

	*NtHeader = (PIMAGE_NT_HEADERS64)((SIZE_T)Base + ((PIMAGE_DOS_HEADER)Base)->e_lfanew);

	return TRUE;
}

void GetMachineString(WORD Machine,PWCHAR s)
{
	swprintf_s(s, 64, L"0x%04X - ", Machine);
	switch (Machine)
	{
	case 0x014C:
		wcscat_s(s, 64, L"Intel386");
		break;
	case 0x0162:
		wcscat_s(s, 64, L"MIPS(R2000, R3000)");
		break;
	case 0x0166:
		wcscat_s(s, 64, L"MIPS(R4000)");
		break;
	case 0x0168:
		wcscat_s(s, 64, L"MIPS(R10000)");
		break;
	case 0x0169:
		wcscat_s(s, 64, L"MIPS WCE v2");
		break;
	case 0x0184:
		wcscat_s(s, 64, L"DEC Alpha AXP");
		break;
	case 0x01F0:
		wcscat_s(s, 64, L"IBM PowerPC");
		break;
	case 0x01A2:
		wcscat_s(s, 64, L"Hitachi SH-3");
		break;
	case 0x01A3:
		wcscat_s(s, 64, L"Hitachi SH-3E");
		break;
	case 0x01A4:
		wcscat_s(s, 64, L"Hitachi SH-3DSP");
		break;
	case 0x01A6:
		wcscat_s(s, 64, L"Hitachi SH-4");
		break;
	case 0x01C0:
		wcscat_s(s, 64, L"ARM");
		break;
	case 0x01C2:
		wcscat_s(s, 64, L"Thumb");
		break;
	case 0x0200:
		wcscat_s(s, 64, L"Intel 64");
		break;
	case 0x0266:
		wcscat_s(s, 64, L"MIPS 16");
		break;
	case 0x0366:
		wcscat_s(s, 64, L"MIPS FPU");
		break;
	case 0x0466:
		wcscat_s(s, 64, L"MIPS FPU 16");
		break;
	case 0x0284:
		wcscat_s(s, 64, L"Alpha 64/AXP 64");
		break;
	case 0x0520:
		wcscat_s(s, 64, L"Tricore");
		break;
	case 0x8664:
		wcscat_s(s, 64, L"AMD 64");
		break;
	case 0x9041:
		wcscat_s(s, 64, L"M32R");
		break;
	case 0x0EBC:
		wcscat_s(s, 64, L"EBC");
		break;
	case 0xC0EE:
		wcscat_s(s, 64, L"CEE");
		break;
	default:
		wcscat_s(s, 64, L"Unknown");
	}
}

void GetSubsystemString(WORD Subsystem, PWCHAR s)
{
	swprintf_s(s, 64, L"0x%04X - ", Subsystem);
	switch (Subsystem)
	{
	case 0x0001:
		wcscat_s(s, 64, L"Native");
		break;
	case 0x0002:
		wcscat_s(s, 64, L"Windows GUI");
		break;
	case 0x0003:
		wcscat_s(s, 64, L"Windows Console");
		break;
	case 0x0005:
		wcscat_s(s, 64, L"OS/2 Console");
		break;
	case 0x0007:
		wcscat_s(s, 64, L"Posix Console");
		break;
	case 0x0008:
		wcscat_s(s, 64, L"Win9x Driver");
		break;
	case 0x0009:
		wcscat_s(s, 64, L"WinCE GUI");
		break;
	case 0x000A:
		wcscat_s(s, 64, L"EFI Application");
		break;
	case 0x000B:
		wcscat_s(s, 64, L"EFI Boot Service Device");
		break;
	case 0x000C:
		wcscat_s(s, 64, L"EFI Run-Time Driver");
		break;
	case 0x000D:
		wcscat_s(s, 64, L"EFI Rom");
		break;
	case 0x000E:
		wcscat_s(s, 64, L"EFI X-Box");
		break;
	default:
		wcscat_s(s, 64, L"Unknown");
	}
}

BOOL GetSections32(PVOID Base, PIMAGE_SECTION_HEADER* SectionHeader, PDWORD NumberOfSections)
{
	BOOL bRtn = FALSE;
	PIMAGE_NT_HEADERS32 NtHeaders32 = NULL;
	WORD SizeOfNtHeader = 0;

	if (Base == NULL || SectionHeader == NULL || NumberOfSections == NULL){
		return FALSE;
	}

	if (GetNtHeader32(Base, &NtHeaders32) == FALSE){
		return FALSE;
	}

	*NumberOfSections = NtHeaders32->FileHeader.NumberOfSections;
	SizeOfNtHeader = sizeof(NtHeaders32->Signature) + sizeof(NtHeaders32->FileHeader) + NtHeaders32->FileHeader.SizeOfOptionalHeader;

	*SectionHeader = (PIMAGE_SECTION_HEADER)((SIZE_T)NtHeaders32 + SizeOfNtHeader);
	return TRUE;
}

BOOL GetSections64(PVOID Base, PIMAGE_SECTION_HEADER* SectionHeader, PDWORD NumberOfSections)
{
	BOOL bRtn = FALSE;
	PIMAGE_NT_HEADERS64 NtHeaders64 = NULL;
	WORD SizeOfNtHeader = 0;

	if (Base == NULL || SectionHeader == NULL || NumberOfSections == NULL){
		return FALSE;
	}

	if (GetNtHeader64(Base, &NtHeaders64) == FALSE){
		return FALSE;
	}

	*NumberOfSections = NtHeaders64->FileHeader.NumberOfSections;
	SizeOfNtHeader = sizeof(NtHeaders64->Signature) + sizeof(NtHeaders64->FileHeader) + NtHeaders64->FileHeader.SizeOfOptionalHeader;

	*SectionHeader = (PIMAGE_SECTION_HEADER)((SIZE_T)NtHeaders64 + SizeOfNtHeader);
	return TRUE;
}

DWORD RVAToOffset32(DWORD dwRVA, PIMAGE_NT_HEADERS32 NtHeader32, PIMAGE_SECTION_HEADER SectionHeader)
{
	int i = 0;
	DWORD dwOffset = 0;

	if (NtHeader32 == NULL){
		goto EXIT_FAILED_0;
	}

	for (i = 0; i < NtHeader32->FileHeader.NumberOfSections; i++)
	{
		if ((dwRVA >= SectionHeader[i].VirtualAddress)		// 
			&& (dwRVA < (SectionHeader[i].VirtualAddress + SectionHeader[i].SizeOfRawData)))
		{
			dwOffset = dwRVA - (SectionHeader[i].VirtualAddress - SectionHeader[i].PointerToRawData);
			goto EXIT_SUCCEEDED;
		}
	}
EXIT_FAILED_0:
	dwOffset = -1;
EXIT_SUCCEEDED:
	return dwOffset;
}

DWORD OffsetToRVA32(DWORD dwOffset, PIMAGE_NT_HEADERS32 NtHeader32, PIMAGE_SECTION_HEADER SectionHeader)
{
	int i = 0;
	DWORD dwRVA = 0;

	if (NtHeader32 == NULL){
		goto EXIT_FAILED_0;
	}

	for (i = 0; i < NtHeader32->FileHeader.NumberOfSections; i++)
	{
		if ((dwOffset >= SectionHeader[i].PointerToRawData) 
			&& (dwOffset < (SectionHeader[i].PointerToRawData + SectionHeader[i].SizeOfRawData)))
		{
			dwRVA = dwOffset - (SectionHeader[i].PointerToRawData - SectionHeader[i].VirtualAddress);
			goto EXIT_SUCCEEDED;
		}
	}
EXIT_FAILED_0:
	dwRVA = -1;
EXIT_SUCCEEDED:
	return dwRVA;
}

DWORD RVAToOffset64(DWORD dwRVA, PIMAGE_NT_HEADERS64 NtHeader64, PIMAGE_SECTION_HEADER SectionHeader)
{
	int i = 0;
	DWORD dwOffset = 0;

	if (NtHeader64 == NULL){
		goto EXIT_FAILED_0;
	}

	for (i = 0; i < NtHeader64->FileHeader.NumberOfSections; i++)
	{
		if ((dwRVA >= SectionHeader[i].VirtualAddress)		// 
			&& (dwRVA < (SectionHeader[i].VirtualAddress + SectionHeader[i].SizeOfRawData)))
		{
			dwOffset = dwRVA - (SectionHeader[i].VirtualAddress - SectionHeader[i].PointerToRawData);
			goto EXIT_SUCCEEDED;
		}
	}
EXIT_FAILED_0:
	dwOffset = -1;
EXIT_SUCCEEDED:
	return dwOffset;
}

DWORD OffsetToRVA64(DWORD dwOffset, PIMAGE_NT_HEADERS64 NtHeader64, PIMAGE_SECTION_HEADER SectionHeader)
{
	int i = 0;
	DWORD dwRVA = 0;

	if (NtHeader64 == NULL){
		goto EXIT_FAILED_0;
	}

	for (i = 0; i < NtHeader64->FileHeader.NumberOfSections; i++)
	{
		if ((dwOffset >= SectionHeader[i].PointerToRawData) 
			&& (dwOffset < (SectionHeader[i].PointerToRawData + SectionHeader[i].SizeOfRawData)))
		{
			dwRVA = dwOffset - (SectionHeader[i].PointerToRawData - SectionHeader[i].VirtualAddress);
			goto EXIT_SUCCEEDED;
		}
	}
EXIT_FAILED_0:
	dwRVA = -1;
EXIT_SUCCEEDED:
	return dwRVA;
}

void GetPEFileType(WORD Magic, PWCHAR s)
{
	swprintf_s(s, 64, L"0x%04X - ", Magic);
	switch (Magic)
	{
	case IMAGE_ROM_OPTIONAL_HDR_MAGIC:
		wcscat_s(s, 64, L"ROM Image");
		break;
	case IMAGE_NT_OPTIONAL_HDR32_MAGIC:
		wcscat_s(s, 64, L"PE");
		break;
	case IMAGE_NT_OPTIONAL_HDR64_MAGIC:
		wcscat_s(s, 64, L"PE32+");
		break;
	default:
		wcscat_s(s, 64, L"Unknown");
	}
}

BOOL GetExportDirectory32(PVOID Base, PIMAGE_EXPORT_DIRECTORY* ExportDirectory)
{
	PIMAGE_NT_HEADERS32 NtHeaders32 = NULL;
	PIMAGE_SECTION_HEADER SectionHeader = NULL;
	DWORD NumberOfSections = 0;

	if (Base == NULL || ExportDirectory == NULL){
		return FALSE;
	}

	if (GetNtHeader32(Base, &NtHeaders32) == FALSE){
		return FALSE;
	}

	if (GetSections32(Base, &SectionHeader, &NumberOfSections) == FALSE){
		return FALSE;
	}

	*ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((SIZE_T)Base + RVAToOffset32(NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, NtHeaders32, SectionHeader));
	return TRUE;
}

BOOL GetExportDirectory64(PVOID Base, PIMAGE_EXPORT_DIRECTORY* ExportDirectory)
{
	PIMAGE_NT_HEADERS64 NtHeaders64 = NULL;
	PIMAGE_SECTION_HEADER SectionHeader = NULL;
	DWORD NumberOfSections = 0;

	if (Base == NULL || ExportDirectory == NULL){
		return FALSE;
	}

	if (GetNtHeader64(Base, &NtHeaders64) == FALSE){
		return FALSE;
	}

	if (GetSections64(Base, &SectionHeader, &NumberOfSections) == FALSE){
		return FALSE;
	}

	*ExportDirectory = (PIMAGE_EXPORT_DIRECTORY)((SIZE_T)Base + RVAToOffset64(NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress, NtHeaders64, SectionHeader));
	return TRUE;
}

BOOL GetBaseRelocationDirectory(PVOID Base,	PIMAGE_BASE_RELOCATION* BaseReloc)
{
	PIMAGE_NT_HEADERS NtHeaders = NULL;
	PIMAGE_SECTION_HEADER SectionHeader = NULL;
	DWORD NumberOfSections = 0;

	if (Base == NULL || BaseReloc == NULL){
		return FALSE;
	}

	if (GetNtHeader32(Base, &NtHeaders) == FALSE){
		return FALSE;
	}

	if (GetSections32(Base, &SectionHeader, &NumberOfSections) == FALSE){
		return FALSE;
	}

	*BaseReloc = (PIMAGE_BASE_RELOCATION)((SIZE_T)Base + RVAToOffset32(NtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress, NtHeaders, SectionHeader));
	return TRUE;
}