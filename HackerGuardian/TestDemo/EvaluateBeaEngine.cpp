

#include "stdafx.h"
#include "EvaluateBeaEngine.h"
#include <string.h>
#include <Windows.h>
#include <string>
#include <iostream>
using namespace std;

#define BEA_ENGINE_STATIC  /* specify the usage of a static version of BeaEngine */
#define BEA_USE_STDCALL    /* specify the usage of a stdcall version of BeaEngine */

#ifdef __cplusplus
extern "C"{
#endif

#if defined(_WIN64)
#include "../references/disassembler/beaengine-win64/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win64\\Win64\\Lib\\BeaEngine64.lib")
#else
#include "../references/disassembler/beaengine-win32/headers/BeaEngine.h"
#pragma comment(lib, "..\\references\\disassembler\\beaengine-win32\\Win32\\Lib\\BeaEngine.lib")
#endif

#include "libdasm.h"

#ifdef __cplusplus
};
#endif

string PrintHex(PVOID Address, SIZE_T nLen)
{
	string strRtn;
	SIZE_T i = 0;
	char buffer[8] = {0};
	char placeholders[16] = {0};

	for (i = 0; i < nLen; i++)
	{
		sprintf_s(buffer, "%02X", ((PBYTE)Address)[i]);
		strRtn.append(buffer);
	}
	if (nLen < 12){
		for (i = 0; i < 16 - nLen * 2; i++)
		{
			strRtn.append(" ");
		}
	}
	return strRtn;
}

void TestMov()
{
#if defined(_WIN64)
	BYTE MemCode[] = {
		0x00, 0x00,						// 
		0x45, 0x33, 0xC0,				// xor r8d,r8d
		0xE9, 0xE8, 0xFD, 0xFF, 0xFF,	// jmp 0015F7C2
		0x48, 0x89, 0x5C, 0x24, 0x08,	// mov qword ptr [rsp+8],rbx
		0x57,							// push rdi
		0x48, 0x83, 0xEC, 0x20,			// sub rsp, 20h
		0xE8, 0xBD, 0x4B, 0xB8, 0xFF,	// call FFFFFFFF FFCE45A6
		0x48, 0x8B, 0x1D, 0x0A, 0xE9, 0xD6, 0xFF,	// mov rbx, qword ptr [FFFFFFFF FFECE2FA]
		0x48, 0x8D, 0x3D, 0x03, 0xE9, 0xD6, 0xFF,	// lea rdi, [FFFFFFFF FFECE2FA]
		0xEB, 0x17,						// jmp 0015FA10
		0x48, 0x8B, 0xCB,				// mov rcx,rbx
		0xE8, 0xFD, 0x53, 0xB8, 0xFF,	// call FFFFFFFF FFCE4DFD
		0x85, 0xC0,						// test eax, eax
		0x78, 0x08,						// js 0015FA0D
		0x48, 0x8B, 0xCB,				// mov rcx, rbx
		0xE8, 0x1D, 0xBE, 0xBC, 0xFF,	// call FFFFFFFF FFCFFB5AA
		0x48, 0x8B, 0x1B,				// mov rbx, qword ptr [rbx]
		0x48, 0x3B, 0xDF,				// cmp rdx, rdi
		0x75, 0xE4,						// jne 0012F779
		0xC6, 0x05, 0xFF, 0xDD, 0xD6, 0xFF, 0x01,	// mov byte ptr [FFFFFFFF FFE9D59B], 1
		0x48, 0x83, 0xC4, 0x20,			// add rsp, 20
		0x5F,							// pop rdi
		0xE9, 0xCF, 0x4B, 0xB8, 0xFF,	// jmp XXX
		0xFF, 0x15, 0xCF, 0x4B, 0xB8, 0xFF,	// call qword ptr [FFFFFFFF FFCB437B]
		0xFF, 0x25, 0xCF, 0x4B, 0xB8, 0xFF,	// jmp qword ptr [FFFFFFFF FFCB4381]
		0x00, 0x00,
		0x90
	};
#else
	BYTE MemCode[] = {
		0x00, 0x00, 0x00, 0x00,
		0x50,										// push eax
		0xC3,										// retn
		0x68, 0x34, 0x12, 0x00, 0x00,				// push 1234
		0xC3,										// retn
		0xFF, 0x30,									// push dword ptr [eax]
		0xC3,										// retn
		0xFF, 0x35, 0x34, 0x12, 0x00, 0x00,			// push dword ptr [1234]
		0xC3,										// retn
		0xB8, 0x34, 0x12, 0x00, 0x00,				// mov eax, 1234
		0xA1, 0x34, 0x12, 0x00, 0x00,				// mov eax, dword ptr [1234]
		0xFF, 0xE0,									// jmp eax
		0xE9, 0x5B, 0x62, 0x66, 0x83,				// jmp 00001234
		0xFF, 0x20,									// jmp dword ptr [eax]
		0xFF, 0x25, 0x34, 0x12, 0x00, 0x00,			// jmp dword ptr [1234]
		0x90, 0x90,
		0xFF, 0x25, 0x78, 0x56, 0x34, 0x12,			// jmp dword ptr [12345678]
		0xFF, 0x20,									// jmp dword ptr [eax]
		0xFF, 0xD0,									// call eax
		0xE8, 0x86, 0xA6, 0x9A, 0x95,				// call 12345678
		0xFF, 0x10,									// call dword ptr [eax]
		0xFF, 0x15, 0x78, 0x56, 0x34, 0x12,			// call dword ptr [12345678]
		0x90,										// nop
		0xEB, 0x1D,									// jmp short 7C99B01C
		0x00, 0x94, 0xAF, 0x07, 0x00, 0x94, 0xA3,	// add byte ptr [edi + ebp*4 + A3940007],dl
		0x8B, 0x44, 0x4F, 0x13,						// mov eax, dword ptr [edi + ecx*2 + 13]
// 		0xF7, 0xD0,									// not eax
// 		0xF7, 0xE0,									// mul eax
// 		0xF7, 0xF0,									// div eax
		0x03, 0xC0									// add eax, eax
	};
#endif
	DISASM MyDisasm;
	int len, i = 0;
	int Error = 0;
	int nOffset = 0;

	printf("Version : %s\n", BeaEngineVersion());
	printf("Revision : %s\n", BeaEngineRevision());

	(void) memset (&MyDisasm, 0, sizeof(DISASM));
	MyDisasm.EIP = (UIntPtr)MemCode;
//	MyDisasm.Options = ATSyntax | ShowSegmentRegs;
	MyDisasm.Options = Tabulation + MasmSyntax/* + PrefixedNumeral*/ + ShowSegmentRegs;
	printf("MyDisasm.EIP = 0x%p\n", MyDisasm.EIP);
	while ((!Error) && (i < 25))
	{
#if defined(_WIN64)
		/* =================== Define 64 bits architecture =================== */
		MyDisasm.Archi = 64;
#else
		MyDisasm.Archi = 0;
#endif
		len = Disasm(&MyDisasm);
		if (len != UNKNOWN_OPCODE)
		{
			printf(">>> %p %s %s\n", MyDisasm.EIP, PrintHex((PVOID)(MyDisasm.EIP), len).c_str(), MyDisasm.CompleteInstr);

			printf("=== Instruction ===\n");
 			printf("%20s:%08X\n", "Category", MyDisasm.Instruction.Category);
			printf("\t%20s:%08X\n\t%20s:%08X\n", "Family Instruction", MyDisasm.Instruction.Category & 0xFFFF0000, "Instruction Type", LOWORD(MyDisasm.Instruction.Category));
			printf("%20s:%08X\n", "Opcode", MyDisasm.Instruction.Opcode);
			printf("%20s:%s\n", "Mnemonic", MyDisasm.Instruction.Mnemonic);
			printf("%20s:%08X\n", "BranchType", MyDisasm.Instruction.BranchType);
			printf("%20s:%08X\n","Flags",  MyDisasm.Instruction.Flags);
			printf("%20s:%016X\n", "AddrValue", MyDisasm.Instruction.AddrValue);
			printf("%20s:%016X\n", "Immediat", MyDisasm.Instruction.Immediat);
			printf("%20s:%08X\n", "ImplicitModifiedRegs", MyDisasm.Instruction.ImplicitModifiedRegs);
			printf("=== Operand1 ===\n");
			printf("%20s:%s\n", "Arg1.ArgMnemonic", MyDisasm.Argument1.ArgMnemonic);
			printf("%20s:%08X\n", "Arg1.ArgType", MyDisasm.Argument1.ArgType);
			printf("%20s:%08X\n", "Arg1.ArgSize", MyDisasm.Argument1.ArgSize);
			printf("%20s:%08X\n", "Arg1.ArgPosition", MyDisasm.Argument1.ArgPosition);
			printf("%20s:%08X\n", "Arg1.AccessMode", MyDisasm.Argument1.AccessMode);
			printf("---=== Operand1.Memory ===\n");
			printf("\t%20s:%08X\n", "Memory.BaseRegister", MyDisasm.Argument1.Memory.BaseRegister);
			printf("\t%20s:%08X\n", "Memory.IndexRegister", MyDisasm.Argument1.Memory.IndexRegister);
			printf("\t%20s:%08X\n", "Memory.Scale", MyDisasm.Argument1.Memory.Scale);
			printf("\t%20s:%016X\n", "Memory.Displacement", MyDisasm.Argument1.Memory.Displacement);
			printf("%20s:%08X\n", "Arg1.SegmentReg", MyDisasm.Argument1.SegmentReg);
			printf("=== Operand2 ===\n");
			printf("%20s:%s\n", "Arg2.ArgMnemonic", MyDisasm.Argument2.ArgMnemonic);
			printf("%20s:%08X\n", "Arg2.ArgType", MyDisasm.Argument2.ArgType);
			printf("%20s:%08X\n", "Arg2.ArgSize", MyDisasm.Argument2.ArgSize);
			printf("%20s:%08X\n", "Arg2.ArgPosition", MyDisasm.Argument2.ArgPosition);
			printf("%20s:%08X\n", "Arg2.AccessMode", MyDisasm.Argument2.AccessMode);
			printf("---=== Operand2.Memory ===\n");
			printf("\t%20s:%08X\n", "Memory.BaseRegister", MyDisasm.Argument2.Memory.BaseRegister);
			printf("\t%20s:%08X\n", "Memory.IndexRegister", MyDisasm.Argument2.Memory.IndexRegister);
			printf("\t%20s:%08X\n", "Memory.Scale", MyDisasm.Argument2.Memory.Scale);
			printf("\t%20s:%016X\n", "Memory.Displacement", MyDisasm.Argument2.Memory.Displacement);
			printf("%20s:%08X\n", "Arg2.SegmentReg", MyDisasm.Argument2.SegmentReg);

// 			printf("=== Operand3 ===\n");
// 			printf("%20s:%s\n", "Arg3.ArgMnemonic", MyDisasm.Argument3.ArgMnemonic);
// 			printf("%20s:%08X\n", "Arg3.ArgType", MyDisasm.Argument3.ArgType);
// 			printf("%20s:%08X\n", "Arg3.ArgSize", MyDisasm.Argument3.ArgSize);
// 			printf("%20s:%08X\n", "Arg3.ArgPosition", MyDisasm.Argument3.ArgPosition);
// 			printf("%20s:%08X\n", "Arg3.AccessMode", MyDisasm.Argument3.AccessMode);
// 			printf("---=== Operand3.Memory ===\n");
// 			printf("\t%20s:%08X\n", "Memory.BaseRegister", MyDisasm.Argument3.Memory.BaseRegister);
// 			printf("\t%20s:%08X\n", "Memory.IndexRegister", MyDisasm.Argument3.Memory.IndexRegister);
// 			printf("\t%20s:%08X\n", "Memory.Scale", MyDisasm.Argument3.Memory.Scale);
// 			printf("\t%20s:%016X\n", "Memory.Displacement", MyDisasm.Argument3.Memory.Displacement);
// 			printf("%20s:%08X\n", "Arg3.SegmentReg", MyDisasm.Argument3.SegmentReg);
			MyDisasm.EIP = MyDisasm.EIP + (UIntPtr)len;
			i++;
			nOffset += len;
		}else{
			Error = 1;
		}
	}

	PBYTE pCodeAddress = (PBYTE)MemCode;
	ULONG nCodeLen = 0;
	for (i = 0; i < 15; i ++)
	{
		INSTRUCTION Inst;
		memset(&Inst, 0, sizeof(INSTRUCTION));
		get_instruction(&Inst, (PBYTE)pCodeAddress + nCodeLen, MODE_32);

		printf("Hex:%s\tInst.op1.immediate: 0x%08X\n", PrintHex((PVOID)((PBYTE)pCodeAddress + nCodeLen), Inst.length).c_str(),Inst.op1.immediate);
		printf("Hex:%s\tInst.op2.immediate: 0x%08X\n", PrintHex((PVOID)((PBYTE)pCodeAddress + nCodeLen), Inst.length).c_str(),Inst.op2.immediate);

		nCodeLen += Inst.length;
	}

}
void TestJmp()
{

}
void TestCall()
{

}
void TestPush()
{

}
void TestRet()
{

}