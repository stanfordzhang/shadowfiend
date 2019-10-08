
#include "stdafx.h"
#include "test.h"
#include "..\\references\\szdef.h"

PVOID ReadFileToMemory(LPCWSTR FilePath)
{
	BOOL bRtn = FALSE;
	PVOID pRtn = NULL;

	HANDLE hFile = NULL;
	LARGE_INTEGER FileSize = {0};
	HANDLE hFileMapping = NULL;
	LPBYTE FileBuffer = NULL;
	DWORD NumberOfBytesRead = 0;

	hFile = CreateFileW(FilePath,
						GENERIC_READ,
						FILE_SHARE_READ | FILE_SHARE_WRITE,
						NULL,
						OPEN_EXISTING,
						FILE_ATTRIBUTE_ARCHIVE,
						NULL);

	if (hFile == INVALID_HANDLE_VALUE){
		SzLogA("Open file unsuccessfully! ERROR CODE = %d", GetLastError());
		goto SZ_EXIT_FAILURE_0;
	}

	bRtn = GetFileSizeEx(hFile, &FileSize);
	if (bRtn == FALSE){
		SzLogA("Get file size unsuccessfully! ERROR CODE = %d", GetLastError());
		goto SZ_EXIT_FAILURE_1;
	}

	FileBuffer = (LPBYTE)malloc(FileSize.LowPart);
	ZeroMemory(FileBuffer, FileSize.LowPart);
	if (FileBuffer == NULL){
		SzLogA("Allocate file buffer unsuccessfully!");
		goto SZ_EXIT_FAILURE_1;
	}

	bRtn = ReadFile(hFile, FileBuffer, FileSize.LowPart, &NumberOfBytesRead, NULL);
	//bRtn = ReadFileEx(hFile, FileBuffer, FileSize.QuadPart, NULL, NULL);

	if (bRtn == FALSE){
		SzLogA("Read file unsuccessfully! ERROR CODE = %d", GetLastError());
		goto SZ_EXIT_FAILURE_1;
	}

	pRtn = (LPVOID)FileBuffer;
SZ_EXIT_FAILURE_1:
	CloseHandle(hFile);
SZ_EXIT_FAILURE_0:
	return pRtn;
}

void Test32()
{
	SzLogA("===============================================================");
	BOOL bRtn = FALSE;
	//LPVOID FileBuffer = ReadFileToMemory(L"C:\\Windows\\System32\\ntoskrnl.exe");
	//LPVOID FileBuffer = ReadFileToMemory(L"D:\\VS2010Temp\\Release\\x86\\hgdll.dll");
	//LPVOID FileBuffer = ReadFileToMemory(L"C:\\Users\\hzzhangjian\\Desktop\\WXP_32_ntoskrnl.exe");
	//LPVOID FileBuffer = ReadFileToMemory(L"D:\\VS2010Temp\\output\\amd64\\hgdll.dll");
	//LPVOID FileBuffer = ReadFileToMemory(L"D:\\VS2010Temp\\output\\i386\\hgdll.dll");
	// 
	LPVOID FileBuffer = ReadFileToMemory(L"C:\\Users\\hzzhangjian\\Desktop\\Win7_64_ntoskrnl.exe");
	//LPVOID FileBuffer = ReadFileToMemory(L"Win7_32_ntoskrnl.exe");

	PIMAGE_DOS_HEADER DosHeader = NULL;
	PIMAGE_NT_HEADERS32 NtHeaders32 = NULL;
	PIMAGE_NT_HEADERS64 NtHeaders64 = NULL;
	PIMAGE_SECTION_HEADER SectionHeader = NULL;
	DWORD SizeOfSections = 0;
	PIMAGE_EXPORT_DIRECTORY ExportDirectory = NULL;
	PIMAGE_BASE_RELOCATION BaseRelc = NULL;
	WORD PEType = 0x0000;

	bRtn = GetDosHeader(FileBuffer, &DosHeader);
	if (DosHeader->e_magic != IMAGE_DOS_SIGNATURE){
		SzLogA("This file is not a PE file!");
	}else{
		SzLogA("This file is a PE File!");
	}

	bRtn = GetNtHeader32(FileBuffer, &NtHeaders32);

	if ((bRtn != TRUE) || (NtHeaders32->Signature != IMAGE_NT_SIGNATURE)){
		SzLogA("NT Header is error!");
	}

	PEType = NtHeaders32->OptionalHeader.Magic;
	// PE
	if (PEType == IMAGE_NT_OPTIONAL_HDR32_MAGIC){
		// DOS header
		SzLogA("e_lfanew = 0x%08X", DosHeader->e_lfanew);
		// File header
		WCHAR Machine[64] = {0};
		GetMachineString(NtHeaders32->FileHeader.Machine, Machine);
		SzLogA("Machine = %S", Machine);
		SzLogA("Number Of Sections = 0x%04X", NtHeaders32->FileHeader.NumberOfSections);
		// Optional header
		WCHAR Magic[64] = {0};
		GetPEFileType(NtHeaders32->OptionalHeader.Magic, Magic);
		SzLogA("PE File Type: %S", Magic);
		SzLogA("AddressOfEntryPoint = 0x%08X", NtHeaders32->OptionalHeader.AddressOfEntryPoint);
		SzLogA("FileAlignment = 0x%08X", NtHeaders32->OptionalHeader.FileAlignment);
		SzLogA("SectionAlignment = 0x%08X", NtHeaders32->OptionalHeader.SectionAlignment);
		SzLogA("ImageBase = 0x%p", NtHeaders32->OptionalHeader.ImageBase);
		WCHAR Subsystem[64] = {0};
		GetSubsystemString(NtHeaders32->OptionalHeader.Subsystem, Subsystem);
		SzLogA("Subsystem = %S", Subsystem);

		CHAR SectionName[16] = {0};

		if (GetSections32(FileBuffer, &SectionHeader, &SizeOfSections) == TRUE){
			SzLogA("No.     \tName\t\tVirtual Offset\tVirtual Size\tRaw Offset\t\tRaw Size\t\tCharacteristics");
			for (DWORD i = 0; i < SizeOfSections; i++){
				strncpy_s(SectionName, (const char*)(SectionHeader[i].Name), 8);
				SzLogA("%8d\t%8s\t0x%08X\t\t0x%08X\t\t0x%08X\t\t0x%08X\t\t0x%08X",
					i + 1,
					SectionName,
					SectionHeader[i].VirtualAddress,
					SectionHeader[i].Misc.VirtualSize,
					SectionHeader[i].PointerToRawData,
					SectionHeader[i].SizeOfRawData,
					SectionHeader[i].Characteristics);
			}
		}
		SzLogA("========================= Data Directory ======================");
		SzLogA("%32s\tVirtual Address\tSize", "Directory");
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[0]Export Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[1]Import Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[2]Resource Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[3]Exception Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[4]Security Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[5]Base Relocation Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[6]Debug Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[7]Architechture Specific Data",
			NtHeaders32->OptionalHeader.DataDirectory[0x0007].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[0x0007].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[8]RVA Of GP",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[9]TLS Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[10]Load Config Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[11]Bound Import Directory",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[12]Import Address Table",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[13]Delay Import Descriptors",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[14]COM Runtime Descriptor",
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[15]Reserved",
			NtHeaders32->OptionalHeader.DataDirectory[0x000F].VirtualAddress,
			NtHeaders32->OptionalHeader.DataDirectory[0x000F].Size);

		SzLogA("======================= Export Directory ======================");
		if (GetExportDirectory32(FileBuffer, &ExportDirectory) == TRUE){
			SzLogA("Name = %s", (char*)((SIZE_T)FileBuffer + RVAToOffset32(ExportDirectory->Name, NtHeaders32, SectionHeader)));
			SzLogA("Base = 0x%08X", ExportDirectory->Base);
			SzLogA("NumberOfFunctions = 0x%08X", ExportDirectory->NumberOfFunctions);
			SzLogA("NumberOfNames = 0x%08X", ExportDirectory->NumberOfNames);
			SzLogA("AddressOfFunctions = 0x%08X", ExportDirectory->AddressOfFunctions);
			SzLogA("AddressOfNames = 0x%08X", ExportDirectory->AddressOfNames);
			SzLogA("AddressOfNameOrdinals = 0x%08X", ExportDirectory->AddressOfNameOrdinals);

			DWORD NumberOfFunctions = ExportDirectory->NumberOfFunctions;
			DWORD NumberOfNames = ExportDirectory->NumberOfNames;

			WORD* OrdinalAddr = (PWORD)((SIZE_T)FileBuffer + RVAToOffset32(ExportDirectory->AddressOfNameOrdinals, NtHeaders32, SectionHeader));
			DWORD* FunctionsAddr = (PDWORD)((SIZE_T)FileBuffer + RVAToOffset32(ExportDirectory->AddressOfFunctions, NtHeaders32, SectionHeader));
			DWORD* NamesAddr = (PDWORD)((SIZE_T)FileBuffer + RVAToOffset32(ExportDirectory->AddressOfNames, NtHeaders32, SectionHeader));

			int FuncIndex = 0;
			DWORD Ordinal = ExportDirectory->Base;
			SIZE_T VirtualAddress = 0;
			SIZE_T RawAddress = 0;
			PCHAR FunctionName = NULL;
			for (Ordinal = ExportDirectory->Base; Ordinal != ExportDirectory->NumberOfFunctions + ExportDirectory->Base; Ordinal++, FuncIndex++){
				VirtualAddress = FunctionsAddr[FuncIndex];
				RawAddress = (SIZE_T)RVAToOffset32(VirtualAddress, NtHeaders32, SectionHeader);
				FunctionName = (PCHAR)((SIZE_T)FileBuffer + RVAToOffset32(NamesAddr[FuncIndex], NtHeaders32, SectionHeader));
				//if (strcmp(FunctionName, "KeServiceDescriptorTable") == 0)
				{
					SzLogA("0x%04X, 0x%08X, 0x%08X, %s", Ordinal, VirtualAddress, RawAddress, FunctionName);
				}
			}
		}

		return;
		if (GetBaseRelocationDirectory(FileBuffer, &BaseRelc) == TRUE){
			SIZE_T VirtualAddress = BaseRelc->VirtualAddress;
			DWORD SizeOfBlock = BaseRelc->SizeOfBlock;
			PWORD TypeOffset = (PWORD)((SIZE_T)VirtualAddress + sizeof(IMAGE_BASE_RELOCATION));
			DWORD CurrentSize = sizeof(IMAGE_BASE_RELOCATION);
			SzLogA("VirtualAddress = 0x%08X, SizeOfBlock = 0x%08X", BaseRelc->VirtualAddress, BaseRelc->SizeOfBlock);
			do 
			{
				while (CurrentSize < SizeOfBlock) 
				{
					switch ((*TypeOffset) >> 12)
					{
					case IMAGE_REL_BASED_ABSOLUTE:	// 0
						break;
					case IMAGE_REL_BASED_HIGH:
						break;
					case IMAGE_REL_BASED_LOW:
						break;
					case IMAGE_REL_BASED_HIGHLOW:	// 3
						{

						}
						break;
					case IMAGE_REL_BASED_HIGHADJ:
						break;
					case IMAGE_REL_BASED_MIPS_JMPADDR:
						break;
					case IMAGE_REL_BASED_MIPS_JMPADDR16:
						break;
					case IMAGE_REL_BASED_DIR64:		// 10
						{
							//*(DWORD*)((SIZE_T)FileBuffer + VirtualAddress + (*TypeOffset & 0x0FFF)) += deltaVA
						}
						break;
					}
					// Next
					TypeOffset++;
					CurrentSize += sizeof(WORD);
				} 
			} while (SizeOfBlock);
		}
		// PE32+
	}else if (PEType == IMAGE_NT_OPTIONAL_HDR64_MAGIC){
		bRtn = GetNtHeader64(FileBuffer, &NtHeaders64);

		// DOS header
		SzLogA("e_lfanew = 0x%08X", DosHeader->e_lfanew);
		// File header
		WCHAR Machine[64] = {0};
		GetMachineString(NtHeaders64->FileHeader.Machine, Machine);
		SzLogA("Machine = %S", Machine);
		SzLogA("Number Of Sections = 0x%04X", NtHeaders64->FileHeader.NumberOfSections);
		// Optional header
		WCHAR Magic[64] = {0};
		GetPEFileType(NtHeaders64->OptionalHeader.Magic, Magic);
		SzLogA("PE File Type: %S", Magic);
		SzLogA("AddressOfEntryPoint = 0x%08X", NtHeaders64->OptionalHeader.AddressOfEntryPoint);
		SzLogA("FileAlignment = 0x%08X", NtHeaders64->OptionalHeader.FileAlignment);
		SzLogA("SectionAlignment = 0x%08X", NtHeaders64->OptionalHeader.SectionAlignment);
		SzLogA("ImageBase = 0x%016I64X", NtHeaders64->OptionalHeader.ImageBase);
		WCHAR Subsystem[64] = {0};
		GetSubsystemString(NtHeaders64->OptionalHeader.Subsystem, Subsystem);
		SzLogA("Subsystem = %S", Subsystem);

		CHAR SectionName[16] = {0};

		if (GetSections64(FileBuffer, &SectionHeader, &SizeOfSections) == TRUE){
			SzLogA("No.     \tName\t\tVirtual Offset\tVirtual Size\tRaw Offset\t\tRaw Size\t\tCharacteristics");
			for (DWORD i = 0; i < SizeOfSections; i++){
				strncpy_s(SectionName, (const char*)(SectionHeader[i].Name), 8);
				SzLogA("%8d\t%8s\t0x%08X\t\t0x%08X\t\t0x%08X\t\t0x%08X\t\t0x%08X",
					i + 1,
					SectionName,
					SectionHeader[i].VirtualAddress,
					SectionHeader[i].Misc.VirtualSize,
					SectionHeader[i].PointerToRawData,
					SectionHeader[i].SizeOfRawData,
					SectionHeader[i].Characteristics);
			}
		}
		SzLogA("========================= Data Directory ======================");
		SzLogA("%32s\tVirtual Address\tSize", "Directory");
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[0]Export Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[1]Import Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[2]Resource Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_RESOURCE].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[3]Exception Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[4]Security Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_SECURITY].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[5]Base Relocation Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[6]Debug Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DEBUG].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[7]Architechture Specific Data",
			NtHeaders64->OptionalHeader.DataDirectory[0x0007].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[0x0007].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[8]RVA Of GP",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_GLOBALPTR].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[9]TLS Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_TLS].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[10]Load Config Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_LOAD_CONFIG].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[11]Bound Import Directory",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[12]Import Address Table",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IAT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[13]Delay Import Descriptors",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[14]COM Runtime Descriptor",
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_COM_DESCRIPTOR].Size);
		SzLogA(	"%32s\t0x%08X\t\t0x%08X", "[15]Reserved",
			NtHeaders64->OptionalHeader.DataDirectory[0x000F].VirtualAddress,
			NtHeaders64->OptionalHeader.DataDirectory[0x000F].Size);

		SzLogA("======================= Export Directory ======================");
		if (GetExportDirectory64(FileBuffer, &ExportDirectory) == TRUE){
			SzLogA("Name = %s", (char*)((SIZE_T)FileBuffer + RVAToOffset64(ExportDirectory->Name, NtHeaders64, SectionHeader)));
			SzLogA("Base = 0x%08X", ExportDirectory->Base);
			SzLogA("NumberOfFunctions = 0x%08X", ExportDirectory->NumberOfFunctions);
			SzLogA("NumberOfNames = 0x%08X", ExportDirectory->NumberOfNames);
			SzLogA("AddressOfFunctions = 0x%08X", ExportDirectory->AddressOfFunctions);
			SzLogA("AddressOfNames = 0x%08X", ExportDirectory->AddressOfNames);
			SzLogA("AddressOfNameOrdinals = 0x%08X", ExportDirectory->AddressOfNameOrdinals);

			DWORD NumberOfFunctions = ExportDirectory->NumberOfFunctions;
			DWORD NumberOfNames = ExportDirectory->NumberOfNames;

			WORD* OrdinalAddr = (PWORD)((SIZE_T)FileBuffer + RVAToOffset64(ExportDirectory->AddressOfNameOrdinals, NtHeaders64, SectionHeader));
			DWORD* FunctionsAddr = (PDWORD)((SIZE_T)FileBuffer + RVAToOffset64(ExportDirectory->AddressOfFunctions, NtHeaders64, SectionHeader));
			DWORD* NamesAddr = (PDWORD)((SIZE_T)FileBuffer + RVAToOffset64(ExportDirectory->AddressOfNames, NtHeaders64, SectionHeader));

			int FuncIndex = 0;
			DWORD Ordinal = ExportDirectory->Base;
			SIZE_T VirtualAddress = 0;
			SIZE_T RawAddress = 0;
			PCHAR FunctionName = NULL;
			for (Ordinal = ExportDirectory->Base; Ordinal != ExportDirectory->NumberOfFunctions + ExportDirectory->Base; Ordinal++, FuncIndex++){
				VirtualAddress = FunctionsAddr[FuncIndex];
				RawAddress = (SIZE_T)RVAToOffset64(VirtualAddress, NtHeaders64, SectionHeader);
				FunctionName = (PCHAR)((SIZE_T)FileBuffer + RVAToOffset64(NamesAddr[FuncIndex], NtHeaders64, SectionHeader));
				if (strcmp(FunctionName, "KeServiceDescriptorTable") == 0)
				{
					SzLogA("0x%04X, 0x%08X, 0x%08X, %s", Ordinal, VirtualAddress, RawAddress, FunctionName);
				}
			}
		}
	}

	free(FileBuffer);
	SzLogA("===============================================================");
}