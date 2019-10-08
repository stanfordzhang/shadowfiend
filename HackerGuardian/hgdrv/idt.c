
#include "hgheader.h"

#define	HGMAKESIZE(a,b)			((ULONG64)(((ULONG32)(((ULONG64)(a)) & 0xFFFFFFFF)) | ((ULONG64)((ULONG32)(((ULONG64)(b)) & 0xFFFFFFFF))) << 32))

//
// Define struct
// 
typedef struct _HG_IDT_ENTRY 
{
	unsigned short	LowOffset;
	unsigned short	selector;
	unsigned char	unused_lo;
	unsigned char	segment_type:4;			// 0x0E is an interrupt gate
	unsigned char	system_segment_flag:1;
	unsigned char	DPL:2;					// descriptor privilege level
	unsigned char	P:1;					// present
#if defined(_WIN64)
	UINT16			MidOffset;
	ULONG32			HiOffset;
	ULONG32			Placeholder;			// not use
#else
	unsigned short	HiOffset;
#endif
}HG_IDT_ENTRY, *PHG_IDT_ENTRY;

//
// sidt return idt in this format
//  Stores the interrupt descriptor table register (IDTR) in the destination operand. In legacy and
//  compatibility mode, the destination operand is 6 bytes; in 64-bit mode it is 10 bytes. In all modes,
//	operand-size prefixes are ignored.
//	In non-64-bit mode, the lower two bytes of the operand specify the 16-bit limit and the upper 4 bytes
//	specify the 32-bit base address.
//	In 64-bit mode, the lower two bytes of the operand specify the 16-bit limit and the upper 8 bytes
//	specify the 64-bit base address.
// 
#pragma pack(1)	
typedef struct _HG_IDT_INFO 
{
	unsigned short IDTLimit;
#if defined(_WIN64)
	ULONG32 LowIDTbase;
	ULONG32 HiIDTbase;
#else
	unsigned short LowIDTbase;
	unsigned short HiIDTbase;
#endif
}HG_IDT_INFO, *PHG_IDT_INFO;
#pragma pack()

typedef ULONG (NTAPI *pfnKeQueryActiveProcessorCount)(PKAFFINITY ActiveProcessors);

KEVENT g_Event;
ULONG_PTR g_CurrentCpuAffinity = 0;

//
// Private methods
// 
VOID TraverseIdt(ULONG_PTR CpuOrdinal, PITP pItp)
{
	HG_IDT_INFO idt;
	SIZE_T i = 0;
	SIZE_T address = 0;
	PHG_IDT_ENTRY idt_entry = NULL;
	PHG_IDT_ENTRY idtTmp = NULL;

	if (!pItp){
		return;
	}

#if defined(_WIN64)
	__sidt(&idt);
	idt_entry = (PHG_IDT_ENTRY)HGMAKESIZE(idt.LowIDTbase, idt.HiIDTbase);
#else
	_asm{
		sidt idt
	}
	idt_entry = (PHG_IDT_ENTRY)MAKELONG(idt.LowIDTbase, idt.HiIDTbase);
#endif
	IdtLog(("---------idt_entry = %p\n", idt_entry));
	if (idt_entry)
	{
		for (i = 0; i <= 0xFF; i++)
		{
			idtTmp = &idt_entry[i];
#if defined(_WIN64)
			address = HGMAKESIZE(MAKELONG(idtTmp->LowOffset, idtTmp->MidOffset), idtTmp->HiOffset);
#else
			address = MAKELONG(idtTmp->LowOffset, idtTmp->HiOffset);
#endif
			IdtLog(("CPU Ordinal: %d, Index: %02X, Address: %p\n", CpuOrdinal, i, address));
		}
	}
}

VOID TraverseIdtDpc(
	__in	struct _KDPC *Dpc,
	__in	ULONG	DeferredContext,
	__in	PVOID	SystemArgument1,
	__in	PVOID	SystemArgument2)
{
	HG_IDT_INFO idt;
	SIZE_T i = 0;
	SIZE_T address = 0;
	PHG_IDT_ENTRY idt_entry = NULL;
	PHG_IDT_ENTRY idtTmp = NULL;

	TraverseIdt(g_CurrentCpuAffinity, (PITP)DeferredContext);

	HgKeSetEvent(&g_Event, IO_NO_INCREMENT, FALSE);
}

VOID QueryCurrentIdt(PITP pItp)
{
	KAFFINITY CpuAffinity;
	size_t nCpuCount = 0;
	size_t i = 0;
	KDPC Dpc;

	CpuAffinity = HgKeQueryActiveProcessors();

	for(i = 0; i < sizeof(KAFFINITY); i ++){
		if ((CpuAffinity >> i) & 1){
			nCpuCount ++;
		}
	}

	if (nCpuCount == 1){
		KIRQL OldIrql = KeRaiseIrqlToDpcLevel();
		TraverseIdt(0, pItp);
		KeLowerIrql(OldIrql);
	}else{
		for(i = 0; i < sizeof(KAFFINITY); i ++){
			if ((CpuAffinity >> i) & 1){
				g_CurrentCpuAffinity = i;
				HgKeInitializeEvent(&g_Event, NotificationEvent, FALSE);
				HgKeInitializeDpc(&Dpc, (PKDEFERRED_ROUTINE)TraverseIdtDpc, (PVOID)pItp);
				HgKeSetTargetProcessorDpc(&Dpc, (CCHAR)i);
				HgKeSetImportanceDpc(&Dpc, HighImportance);
				HgKeInsertQueueDpc(&Dpc, NULL, NULL);

				if (HgKeWaitForSingleObject(&g_Event, (KWAIT_REASON)0, 0, 0, 0) == STATUS_SUCCESS)
				{
					continue;
				}
			}
		}
	}
}

NTSTATUS EnumIdtHook(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize)
{
	NTSTATUS	status	= STATUS_UNSUCCESSFUL;
	PITP		pIdtItp = (PITP)pOutBuffer;

	QueryCurrentIdt(pIdtItp);

	status = STATUS_SUCCESS;
	return status;
}