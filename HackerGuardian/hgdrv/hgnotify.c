
#include "hgnotify.h"

//
// dummy notify routine
//
VOID DummyCreateProcessNotify(
	__in	HANDLE	ParentId,
	__in	HANDLE	ProcessId,
	__in	BOOLEAN	Create)
{
	NotifyLog(("[hgnotify].[DummyCreateProcessNotify]: Enter! ParentId = 0x%p, ProcessId = 0x%p, Create = %d\n", ParentId, ProcessId, Create));
}

VOID DummyCreateThreadNotify(
	__in	HANDLE	ProcessId,
	__in	HANDLE	ThreadId,
	__in	BOOLEAN	Create)
{
	NotifyLog(("[hgnotify].[DummyCreateThreadNotify]: Enter! ProcessId = 0x%p, ThreadId = 0x%p, Create = %d\n", ProcessId, ThreadId, Create));
}

VOID DummyLoadImageNotify(
	__in	PUNICODE_STRING FullImageName,
	__in	HANDLE			ProcessId,
	__in	PIMAGE_INFO		ImageInfo)
{
	NotifyLog(("[hgnotify].[DummyLoadImageNotify]: Enter! FullImageName = %S, ProcessId = 0x%p, Create = 0x%p\n", FullImageName->Buffer, ProcessId, ImageInfo->ImageBase));
}

NTSTATUS EnumNotify(
	__in	PVOID	pInBuffer,
	__in	ULONG	nInSize,
	__inout	PVOID	pOutBuffer,
	__inout	ULONG	nOutSize,
	__out	ULONG*	pReturnLength)
{
	NTSTATUS	status = STATUS_UNSUCCESSFUL;
	PITP		pOutItp = (PITP)pOutBuffer;

	NotifyLog(("[hgnotify.c].[EnumNotify]: Enter!\n"));

	EnumCreateProcessNotify(pOutItp);
 	EnumCreateThreadNotify(pOutItp);
 	EnumLoadImageNotify(pOutItp);
   	EnumRegistryNotify(pOutItp);
   	EnumShutdownNotify(pOutItp);

	status = STATUS_SUCCESS;
	return status;
}

void EnumCreateProcessNotify(PITP pItp)
{
	SIZE_T PspCreateProcessNotifyRoutine = g_hg_map[eHG_PspCreateProcessNotifyRoutine];

	SIZE_T nPspMaxCreateProcessNotify = 0;
	SIZE_T i = 0;
	SIZE_T NotifyAddress = 0;
	SIZE_T EX_CALLBACK_MASK = 0;
	SIZE_T KnownAddress = 0;

#if defined(_WIN64)
	EX_CALLBACK_MASK = (SIZE_T)~0x0F;
#else
	EX_CALLBACK_MASK = (SIZE_T)~0x07;
#endif

	if (!PspCreateProcessNotifyRoutine){
		GetPspCreateProcessNotifyRoutine();
		PspCreateProcessNotifyRoutine = g_hg_map[eHG_PspCreateProcessNotifyRoutine];
	}

	if (PspCreateProcessNotifyRoutine)
	{
		NotifyLog(("%s(%d): PspCreateProcessNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateProcessNotifyRoutine));
		switch (g_WindowsVersion){
		case eWin_XP:
			nPspMaxCreateProcessNotify = 8;
			break;
		case eWin_7:
		case eWin_8:
			nPspMaxCreateProcessNotify = 64;
			break;
		}

		for (i = 0; i < nPspMaxCreateProcessNotify; i++){
			if (HgMmIsAddressValid((PVOID)(PspCreateProcessNotifyRoutine + i * sizeof(SIZE_T))) == FALSE	||
				(PspCreateProcessNotifyRoutine + i * sizeof(SIZE_T)) < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			NotifyAddress = *(PSIZE_T)(PspCreateProcessNotifyRoutine + i * sizeof(SIZE_T));

			//
			// EX_CALLBACK_MASK depend on struct _EX_CALLBACK and struct _EX_FAST_REF.
			// sizeof(PVOID) depend on Value(field in struct _EX_FAST_REF), 
			// this field point to struct _EX_CALLBACK_ROUTINE_BLOCK.
			// The function pointer is the second field, 
			// so the offset is that base plus sizeof(EX_RUNDOWN_REF)(that is sizeof(PVOID)).
			// 
			if (HgMmIsAddressValid((PVOID)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID))) == FALSE	||
				NotifyAddress < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			else
			{
				KnownAddress = *(PSIZE_T)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID));
			}

			if (KnownAddress && HgMmIsAddressValid((PVOID)KnownAddress))
			{
				if (pItp->notify.nMaxCount > pItp->notify.nCount){
					pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_CreateProcess;
					pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = KnownAddress;
					pItp->notify.NotifyData[pItp->notify.nCount].Comment = NotifyAddress;
				}
				pItp->notify.nCount ++;
			}
		}
	}
}

void GetPspCreateProcessNotifyRoutine()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T PspCreateProcessNotifyRoutine = 0;
	SIZE_T Address = 0;
	NotifyLog(("%s(%d): Enter\n", __FILE__, __LINE__));
	if (g_hg_map[eHG_PsSetCreateProcessNotifyRoutine] != 0){
		Address = GetPsSetCreateProcessNotifyRoutineAux(g_hg_map[eHG_PsSetCreateProcessNotifyRoutine]);
	}else{
		Address = GetPsSetCreateProcessNotifyRoutineAux((SIZE_T)&PsSetCreateProcessNotifyRoutine);
	}
	NotifyLog(("Address = 0x%p\n", Address));

	//
	// Method 1: by setting a user-defined notify routine.
	// 
	if (Address){
		status = HgPsSetCreateProcessNotifyRoutine(DummyCreateProcessNotify, FALSE);
		if (NT_SUCCESS(status)){
			PspCreateProcessNotifyRoutine = LookupPspSetXNotifyRoutineAddress((PVOID)Address, (PVOID)&DummyCreateProcessNotify, eNT_CreateProcess);
			NotifyLog(("%s(%d): Method 1: PspCreateProcessNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateProcessNotifyRoutine));
			
			if (PspCreateProcessNotifyRoutine != 0 && HgMmIsAddressValid((PVOID)PspCreateProcessNotifyRoutine) == TRUE)
			{
				g_hg_map[eHG_PspCreateProcessNotifyRoutine] = PspCreateProcessNotifyRoutine;
			}

			HgPsSetCreateProcessNotifyRoutine(DummyCreateProcessNotify, TRUE);
		}
	}
	return;
	//
	// Method 2: by searching magic code.
	// 
	if (Address && !PspCreateProcessNotifyRoutine)
	{
		BYTE MagicByte = 0;
		WORD MagicWord = 0;
		SIZE_T i = 0;
		SIZE_T nInstructionSize = 0;
		SIZE_T nStart = (SIZE_T)Address;
		SIZE_T nEnd = (SIZE_T)Address + PAGE_SIZE;
		
		switch(g_WindowsVersion){
		case eWin_2000:
			MagicByte = 0xBA;
			break;
		case eWin_XP:
		case eWin_2003:
		case eWin_2003_SP1_SP2:
		case eWin_Vista:
			MagicByte = 0xBF;
			break;
		case eWin_Vista_SP1_SP2:
		case eWin_7:
#if defined(_WIN64)
			MagicWord = 0x8D4C;
			MagicByte = 0x35;
#else
			MagicByte = 0xC7;
#endif
			break;
		case eWin_8:
#if defined(_WIN64)
			MagicWord = 0x8D4C;
			MagicByte = 0x3D;
#else
			MagicByte = 0xB8;
#endif
			break;
		}

		if (MagicByte == 0){
			return;
		}

		for (i = nStart; i < nEnd; i += nInstructionSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE)
			{
				DISASM MyDisasm;

				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
				MyDisasm.Archi = 64;
#else
				MyDisasm.Archi = 0;
#endif
				nInstructionSize = Disasm(&MyDisasm);

				if (nInstructionSize == 0){
					break;
				}
#if defined(_WIN64)
				else if ((nInstructionSize == 7) && (MagicWord == *(PWORD)((SIZE_T)i)) && (MagicByte == *(PBYTE)((SIZE_T)i + 2))){
					DWORD dwOffset = *(PDWORD)(i + 3);
					PspCreateProcessNotifyRoutine = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);
				}
#else
				else if ((nInstructionSize == 5) && (MagicByte == *(PBYTE)((SIZE_T)i))){
					PspCreateProcessNotifyRoutine = *(PSIZE_T)(i + 1);
				}
#endif

				if (PspCreateProcessNotifyRoutine > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
					HgMmIsAddressValid((PVOID)PspCreateProcessNotifyRoutine) == TRUE)
				{
					NotifyLog(("%s(%d): Method2: PspCreateProcessNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateProcessNotifyRoutine));
					g_hg_map[eHG_PspCreateProcessNotifyRoutine] = PspCreateProcessNotifyRoutine;
					break;
				}
			}
		}
	}
}

SIZE_T GetPsSetCreateProcessNotifyRoutineAux(
	__in	SIZE_T PsSetCreateProcessNotifyRoutineAddress)
{
	BYTE MagicByte = 0;
	switch(g_WindowsVersion){
	case eWin_2000:
	case eWin_XP:
	case eWin_2003:
	case eWin_2003_SP1_SP2:
	case eWin_Vista:
		return PsSetCreateProcessNotifyRoutineAddress;
	//
	// PsSetCreateProcessNotifyRoutine is a stub, it call PspSetCreateProcessNotifyRoutine.
	// 
	case eWin_Vista_SP1_SP2:
	case eWin_7:
		{
			SIZE_T i = 0;
			SIZE_T nCodeLen = 0;
			SIZE_T nStart = PsSetCreateProcessNotifyRoutineAddress;
			SIZE_T nEnd = PsSetCreateProcessNotifyRoutineAddress + PAGE_SIZE;

#if defined(_WIN64)
			MagicByte = 0xE9;
#else
			MagicByte = 0xE8;
#endif

			for (i = nStart; i < nEnd; i += nCodeLen){
				if (HgMmIsAddressValid((PVOID)i) == TRUE)
				{
					DISASM MyDisasm;
					(void) memset (&MyDisasm, 0, sizeof(DISASM));
					MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
					MyDisasm.Archi = 64;
#else
					MyDisasm.Archi = 0;
#endif
					nCodeLen = Disasm(&MyDisasm);
					//////////////////////////////////////////////////////////////////////////
					if (nCodeLen == 0)
					{
						return 0;
					}
					else if (nCodeLen == 5	&& *(PBYTE)i == MagicByte)
					{
						DWORD dwOffset = *(PDWORD)(i + 1);
#if defined(_WIN64)
						return i + 5 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);
#else
						return i + 5 + (SIZE_T)dwOffset;
#endif
					}
				}
			}
		}
	case eWin_8:
		{
			SIZE_T i = 0;
			SIZE_T nCodeLen = 0;
			SIZE_T nStart = PsSetCreateProcessNotifyRoutineAddress;
			SIZE_T nEnd = PsSetCreateProcessNotifyRoutineAddress + PAGE_SIZE;

#if defined(_WIN64)
			MagicByte = 0xEB;
#else
			MagicByte = 0xE8;
#endif

			for (i = nStart; i < nEnd; i += nCodeLen){
				if (HgMmIsAddressValid((PVOID)i) == TRUE)
				{
					DISASM MyDisasm;
					(void) memset (&MyDisasm, 0, sizeof(DISASM));
					MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
					MyDisasm.Archi = 64;
#else
					MyDisasm.Archi = 0;
#endif
					nCodeLen = Disasm(&MyDisasm);
					//////////////////////////////////////////////////////////////////////////
					if (nCodeLen == 0)
					{
						return 0;
					}
#if defined(_WIN64)
					else if (nCodeLen == 2 && *(PBYTE)i == MagicByte)
					{
						UINT8 dwOffset = *(PUINT8)(i + 1);
						return i + 2 + (SIZE_T)dwOffset;
					}
#else
					else if (nCodeLen == 5 && *(PBYTE)i == MagicByte)
					{
						DWORD dwOffset = *(PDWORD)(i + 1);
						return i + 5 + (SIZE_T)dwOffset;
					}
#endif
				}
			}
		}
	}
	return 0;
}

SIZE_T LookupPspSetXNotifyRoutineAddress(
	__in	PVOID	PsSetXNotifyRoutineAddress,
	__in	PVOID	KnownAddress,
	__in	NOTIFY_TYPE	Type)
{
	SIZE_T ReturnAddress = 0;
	SIZE_T nPspMaxCreateProcessNotify = 0;
	SIZE_T nPspMaxCreateThreadNotify = 0;
	SIZE_T nPspMaxLoadImageNotify = 0;
	SIZE_T nPspMaxNotify = 0;

	SIZE_T i = 0;
	SIZE_T nInstructionSize = 0;
	SIZE_T nStart = (SIZE_T)PsSetXNotifyRoutineAddress;
	SIZE_T nEnd = (SIZE_T)PsSetXNotifyRoutineAddress + PAGE_SIZE;

	SIZE_T TargetAddress = 0;

	if (PsSetXNotifyRoutineAddress == 0	||
		KnownAddress == 0)
	{
		NotifyLog(("%s(%d): Incorrect parameter(s)!\n", __FILE__, __LINE__));
		return 0;
	}

	// XP: 
	//	PSP_MAX_CREATE_PROCESS_NOTIFY = 8
	//	PSP_MAX_CREATE_THREAD_NOTIFY = 8
	//	PSP_MAX_LOAD_IMAGE_NOTIFY = 8
	// 
	// Win7 (x86): 
	//	83fe686d 43              inc     ebx
	//	83fe686e 83fb40          cmp     ebx,40h
	//	83fe6871 72af            jb      nt!PspSetCreateProcessNotifyRoutine+0x29 (83fe6822)
	//	PSP_MAX_CREATE_PROCESS_NOTIFY = 64
	//
	//	841314db 83c704          add     edi,4
	//	841314de 83c604          add     esi,4
	//	841314e1 81ff00010000    cmp     edi,100h
	//	841314e7 72e3            jb      nt!PsSetCreateThreadNotifyRoutine+0x25 (841314cc)
	//	PSP_MAX_CREATE_THREAD_NOTIFY = 64
	//	
	//	83fc29e0 83c704          add     edi,4
	//	83fc29e3 83c604          add     esi,4
	//	83fc29e6 83ff20          cmp     edi,20h
	//	83fc29e9 72e6            jb      nt!PsSetLoadImageNotifyRoutine+0x1e (83fc29d1)
	//	PSP_MAX_LOAD_IMAGE_NOTIFY = 8 = 0x20 / 0x04 = 0x8 = 8
	//	
	// Win7 (x64)
	//	fffff800`02cb1bb3 4403e3          add     r12d,ebx
	//	fffff800`02cb1bb6 4183fc40        cmp     r12d,40h
	//	fffff800`02cb1bba 72a1            jb      nt!PspSetCreateProcessNotifyRoutine+0x4d (fffff800`02cb1b5d)
	//	PSP_MAX_CREATE_PROCESS_NOTIFY = 64
	//	
	//	fffff800`02c8210c ffc3            inc     ebx
	//	fffff800`02c8210e 83fb40          cmp     ebx,40h
	//	fffff800`02c82111 72df            jb      nt!PsSetCreateThreadNotifyRoutine+0x22 (fffff800`02c820f2)
	//	PSP_MAX_CREATE_THREAD_NOTIFY = 64
	//	
	//	fffff800`02c8208c ffc3            inc     ebx
	//	fffff800`02c8208e 83fb08          cmp     ebx,8
	//	fffff800`02c82091 72df            jb      nt!PsSetLoadImageNotifyRoutine+0x22 (fffff800`02c82072)
	//	PSP_MAX_LOAD_IMAGE_NOTIFY = 8
	//	
	// Win8 (x86)
	//	8132884d 83c704          add     edi,4
	//	81328850 83c004          add     eax,4
	//	81328853 89450c          mov     dword ptr [ebp+0Ch],eax
	//	81328856 81ff00010000    cmp     edi,100h
	//	8132885c 72dc            jb      nt!PspSetCreateProcessNotifyRoutine+0x40 (8132883a)
	//	PSP_MAX_CREATE_PROCESS_NOTIFY = 64
	//	
	//	81328364 83c704          add     edi,4
	//	81328367 83c304          add     ebx,4
	//	8132836a 81ff00010000    cmp     edi,100h
	//	81328370 72bc            jb      nt!PsSetCreateThreadNotifyRoutine+0x1f (8132832e)
	//	PSP_MAX_CREATE_THREAD_NOTIFY = 64
	//	
	//	8123b1d0 40              inc     eax
	//	8123b1d1 83c604          add     esi,4
	//	8123b1d4 89450c          mov     dword ptr [ebp+0Ch],eax
	//	8123b1d7 83f840          cmp     eax,40h
	//	8123b1da 72c3            jb      nt! ?? ::NNGAKEGL::`string'+0x42841 (8123b19f)
	//	PSP_MAX_LOAD_IMAGE_NOTIFY = 16 = 40h / 4h
	//	
	// Win8 (0x64)
	//	fffff800`5332c5b4 03df            add     ebx,edi
	//	fffff800`5332c5b6 83fb40          cmp     ebx,40h
	//	fffff800`5332c5b9 72e4            jb      nt!PspSetCreateProcessNotifyRoutine+0x57 (fffff800`5332c59f)
	//	PSP_MAX_CREATE_PROCESS_NOTIFY = 64
	//	
	//	fffff800`532ff3f0 ffc3            inc     ebx
	//	fffff800`532ff3f2 83fb40          cmp     ebx,40h
	//	fffff800`532ff3f5 72b8            jb      nt!PsSetCreateThreadNotifyRoutine+0x1f (fffff800`532ff3af)
	//	PSP_MAX_CREATE_THREAD_NOTIFY = 64 = 40h
	//
	//	fffff800`532ff380 ffc3            inc     ebx
	//	fffff800`532ff382 83fb08          cmp     ebx,8
	//	fffff800`532ff385 72b8            jb      nt!PsSetLoadImageNotifyRoutine+0x1f (fffff800`532ff33f)
	//	PSP_MAX_LOAD_IMAGE_NOTIFY = 8
	switch (g_WindowsVersion){
	case eWin_XP:
		{
			nPspMaxCreateProcessNotify = 8;
			nPspMaxCreateThreadNotify = 8;
			nPspMaxLoadImageNotify = 8;
			switch (Type){
			case eNT_CreateProcess:
				nPspMaxNotify = nPspMaxCreateProcessNotify;
				break;
			case eNT_CreateThread:
				nPspMaxNotify = nPspMaxCreateThreadNotify;
				break;
			case eNT_LoadImage:
				nPspMaxNotify = nPspMaxLoadImageNotify;
				break;
			}
			break;
		}
	case eWin_7:
		{
#if	defined(_WIN64)
			nPspMaxCreateProcessNotify = 64;
			nPspMaxCreateThreadNotify = 64;
			nPspMaxLoadImageNotify = 8;
			switch (Type){
			case eNT_CreateProcess:
				nPspMaxNotify = nPspMaxCreateProcessNotify;
				break;
			case eNT_CreateThread:
				nPspMaxNotify = nPspMaxCreateThreadNotify;
				break;
			case eNT_LoadImage:
				nPspMaxNotify = nPspMaxLoadImageNotify;
				break;
			}
#else
			nPspMaxCreateProcessNotify = 64;
			nPspMaxCreateThreadNotify = 64;
			nPspMaxLoadImageNotify = 8;
			switch (Type){
			case eNT_CreateProcess:
				nPspMaxNotify = nPspMaxCreateProcessNotify;
				break;
			case eNT_CreateThread:
				nPspMaxNotify = nPspMaxCreateThreadNotify;
				break;
			case eNT_LoadImage:
				nPspMaxNotify = nPspMaxLoadImageNotify;
				break;
			}
#endif
			break;
		}
	case eWin_8:
		{
#if	defined(_WIN64)
			nPspMaxCreateProcessNotify = 64;
			nPspMaxCreateThreadNotify = 64;
			nPspMaxLoadImageNotify = 8;
			switch (Type){
			case eNT_CreateProcess:
				nPspMaxNotify = nPspMaxCreateProcessNotify;
				break;
			case eNT_CreateThread:
				nPspMaxNotify = nPspMaxCreateThreadNotify;
				break;
			case eNT_LoadImage:
				nPspMaxNotify = nPspMaxLoadImageNotify;
				break;
			}
#else
			nPspMaxCreateProcessNotify = 64;
			nPspMaxCreateThreadNotify = 64;
			nPspMaxLoadImageNotify = 16;
			switch (Type){
			case eNT_CreateProcess:
				nPspMaxNotify = nPspMaxCreateProcessNotify;
				break;
			case eNT_CreateThread:
				nPspMaxNotify = nPspMaxCreateThreadNotify;
				break;
			case eNT_LoadImage:
				nPspMaxNotify = nPspMaxLoadImageNotify;
				break;
			}
#endif
			break;
		}
	}

	for (i = nStart; i < nEnd; i += nInstructionSize)
	{
		if (HgMmIsAddressValid((PVOID)i) == TRUE)
		{
			DISASM MyDisasm;

			(void) memset (&MyDisasm, 0, sizeof(DISASM));
			MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
			MyDisasm.Archi = 64;
#else
			MyDisasm.Archi = 0;
#endif
			nInstructionSize = Disasm(&MyDisasm);

			if (nInstructionSize == 0){
				return 0;
			}
#if defined(_WIN64)
			else if (nInstructionSize == 7){
				DWORD dwOffset = *(PDWORD)(i + 3);
				TargetAddress = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);
			}
#else
			else if (nInstructionSize == 5){
				TargetAddress = *(PSIZE_T)(i + 1);
			}
			else if (nInstructionSize == 6){
				TargetAddress = *(PSIZE_T)(i + 2);
			}
			else if (nInstructionSize == 7){
				TargetAddress = *(PSIZE_T)(i + 3);
			}
#endif

			if (TargetAddress > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
				HgMmIsAddressValid((PVOID)TargetAddress) == TRUE)
			{
				ReturnAddress = FindNotifyBase(TargetAddress, nPspMaxNotify, (SIZE_T)KnownAddress);
				if (ReturnAddress){
//					NotifyLog(("%s(%d): ReturnAddress = 0x%p\n", __FILE__, __LINE__, ReturnAddress));
					return ReturnAddress;
				}
			}
		}
	}

	return ReturnAddress;
}

/************************************************************************/
/* NOTE: The following is an significant statement.
 * wrk-v1.2\base\ntos\ps\Psp.h
#define PSP_MAX_CREATE_PROCESS_NOTIFY 8

//
// Define process callouts. These are of type PCREATE_PROCESS_NOTIFY_ROUTINE 
// Called on process create and delete.
//
ULONG PspCreateProcessNotifyRoutineCount;
EX_CALLBACK PspCreateProcessNotifyRoutine[PSP_MAX_CREATE_PROCESS_NOTIFY];

#define PSP_MAX_CREATE_THREAD_NOTIFY 8

//
// Define thread callouts. These are of type PCREATE_THREAD_NOTIFY_ROUTINE
// Called on thread create and delete.
//
ULONG PspCreateThreadNotifyRoutineCount;
EX_CALLBACK PspCreateThreadNotifyRoutine[PSP_MAX_CREATE_THREAD_NOTIFY];


#define PSP_MAX_LOAD_IMAGE_NOTIFY 8

//
// Define image load callbacks. These are of type PLOAD_IMAGE_NOTIFY_ROUTINE 
// Called on image load.
//
ULONG PspLoadImageNotifyRoutineCount;
EX_CALLBACK PspLoadImageNotifyRoutine[PSP_MAX_LOAD_IMAGE_NOTIFY];  


	//
	// Define a structure the caller uses to hold the callbacks
	//
	typedef struct _EX_CALLBACK {
		EX_FAST_REF RoutineBlock;
	} EX_CALLBACK, *PEX_CALLBACK;


	//
	// Fast reference routines. See ntos\ob\fastref.c for algorithm description.
	//
	#if defined (_WIN64)
	#define MAX_FAST_REFS 15
	#else
	#define MAX_FAST_REFS 7
	#endif

	typedef struct _EX_FAST_REF {
		union {
			PVOID Object;
#if defined (_WIN64)
			ULONG_PTR RefCnt : 4;
#else
			ULONG_PTR RefCnt : 3;
#endif
			ULONG_PTR Value;	// point to EX_CALLBACK_ROUTINE_BLOCK
		};
	} EX_FAST_REF, *PEX_FAST_REF;
	
	typedef struct _EX_CALLBACK_ROUTINE_BLOCK {
		EX_RUNDOWN_REF        RundownProtect;
		PEX_CALLBACK_FUNCTION Function;
		PVOID                 Context;
	} EX_CALLBACK_ROUTINE_BLOCK, *PEX_CALLBACK_ROUTINE_BLOCK;
	
	//
	// Rundown protection structure
	//
	typedef struct _EX_RUNDOWN_REF {

	#define EX_RUNDOWN_ACTIVE      0x1
	#define EX_RUNDOWN_COUNT_SHIFT 0x1
	#define EX_RUNDOWN_COUNT_INC   (1<<EX_RUNDOWN_COUNT_SHIFT)
		union {
			ULONG_PTR Count;
			PVOID Ptr;
		};
	} EX_RUNDOWN_REF, *PEX_RUNDOWN_REF;
	
	//
	// Define a block to hold the actual routine registration.
	//
	typedef NTSTATUS (*PEX_CALLBACK_FUNCTION ) (
		IN PVOID CallbackContext,
		IN PVOID Argument1,
		IN PVOID Argument2
	);
*/
/************************************************************************/
SIZE_T FindNotifyBase(
	__in	SIZE_T	Base,
	__in	SIZE_T	Count,
	__in	SIZE_T	MatchingAddress)
{
	SIZE_T Address = 0;
	SIZE_T i = 0;
	SIZE_T NotifyAddress = 0;
	SIZE_T EX_CALLBACK_MASK = 0;

#if defined(_WIN64)
	EX_CALLBACK_MASK = (SIZE_T)~0x0F;
#else
	EX_CALLBACK_MASK = (SIZE_T)~0x07;
#endif

	if (Base == 0	||
		HgMmIsAddressValid((PVOID)Base) == FALSE	||
		MatchingAddress == 0	||
		HgMmIsAddressValid((PVOID)MatchingAddress) == FALSE	||
		Count == 0)
	{
		NotifyLog(("%s(%d): Incorrect parameter(s)!\n", __FILE__, __LINE__));
		return 0;
	}

	for (i = 0; i < Count; i++)
	{
		if (HgMmIsAddressValid((PVOID)(Base + i * sizeof(SIZE_T))) == FALSE	||
			(Base + i * sizeof(SIZE_T)) < (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			break;
		}
		NotifyAddress = *(PSIZE_T)(Base + i * sizeof(SIZE_T));

		//
		// EX_CALLBACK_MASK depend on struct _EX_CALLBACK and struct _EX_FAST_REF.
		// sizeof(PVOID) depend on Value(field in struct _EX_FAST_REF), 
		// this field point to struct _EX_CALLBACK_ROUTINE_BLOCK.
		// The function pointer is the second field, 
		// so the offset is that base plus sizeof(EX_RUNDOWN_REF)(that is sizeof(PVOID)).
		// 
		if (HgMmIsAddressValid((PVOID)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID))) == FALSE	||
			NotifyAddress < (SIZE_T)MM_SYSTEM_RANGE_START)
		{
			break;
		}
		else
		{
			if (*(PSIZE_T)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID)) == MatchingAddress)
			{
				return Base;
			}
		}
	}

	return Address;
}

void EnumCreateThreadNotify(PITP pItp)
{
	SIZE_T PspCreateThreadNotifyRoutine = g_hg_map[eHG_PspCreateThreadNotifyRoutine];

	SIZE_T nPspMaxCreateThreadNotify = 0;
	SIZE_T i = 0;
	SIZE_T NotifyAddress = 0;
	SIZE_T EX_CALLBACK_MASK = 0;
	SIZE_T KnownAddress = 0;

#if defined(_WIN64)
	EX_CALLBACK_MASK = (SIZE_T)~0x0F;
#else
	EX_CALLBACK_MASK = (SIZE_T)~0x07;
#endif

	if (!PspCreateThreadNotifyRoutine){
		GetPspCreateThreadNotifyRoutine();
		PspCreateThreadNotifyRoutine = g_hg_map[eHG_PspCreateThreadNotifyRoutine];
	}

	if (PspCreateThreadNotifyRoutine){
		NotifyLog(("%s(%d): PspCreateThreadNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateThreadNotifyRoutine));
		switch (g_WindowsVersion){
		case eWin_XP:
			nPspMaxCreateThreadNotify = 8;
			break;
		case eWin_7:
		case eWin_8:
			nPspMaxCreateThreadNotify = 64;
			break;
		}

		for (i = 0; i < nPspMaxCreateThreadNotify; i++)
		{
			if (HgMmIsAddressValid((PVOID)(PspCreateThreadNotifyRoutine + i * sizeof(SIZE_T))) == FALSE	||
				(PspCreateThreadNotifyRoutine + i * sizeof(SIZE_T)) < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			NotifyAddress = *(PSIZE_T)(PspCreateThreadNotifyRoutine + i * sizeof(SIZE_T));
			NotifyLog(("NotifyAddress = 0x%p\n", NotifyAddress));
			//
			// EX_CALLBACK_MASK depend on struct _EX_CALLBACK and struct _EX_FAST_REF.
			// sizeof(PVOID) depend on Value(field in struct _EX_FAST_REF), 
			// this field point to struct _EX_CALLBACK_ROUTINE_BLOCK.
			// The function pointer is the second field, 
			// so the offset is that base plus sizeof(EX_RUNDOWN_REF)(that is sizeof(PVOID)).
			// 
			if (HgMmIsAddressValid((PVOID)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID))) == FALSE	||
				NotifyAddress < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			else
			{
				KnownAddress = *(PSIZE_T)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID));
			}
			NotifyLog(("KnownAddress = 0x%p\n", KnownAddress));

			if (KnownAddress && HgMmIsAddressValid((PVOID)KnownAddress))
			{
				if (pItp->notify.nMaxCount > pItp->notify.nCount){
					pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_CreateThread;
					pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = KnownAddress;
					pItp->notify.NotifyData[pItp->notify.nCount].Comment = NotifyAddress;
				}
				pItp->notify.nCount ++;
			}
		}
	}
}

void GetPspCreateThreadNotifyRoutine()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T PspCreateThreadNotifyRoutine = 0;
	SIZE_T Address = 0;

	if (g_hg_map[eHG_PsSetCreateThreadNotifyRoutine] != 0){
		Address = g_hg_map[eHG_PsSetCreateThreadNotifyRoutine];
	}else{
		Address = (SIZE_T)&PsSetCreateThreadNotifyRoutine;
	}

	if (Address)
	{
		status = HgPsSetCreateThreadNotifyRoutine(DummyCreateThreadNotify);
		if (NT_SUCCESS(status)){
			PspCreateThreadNotifyRoutine = LookupPspSetXNotifyRoutineAddress((PVOID)Address, (PVOID)&DummyCreateThreadNotify, eNT_CreateThread);
			if (PspCreateThreadNotifyRoutine && HgMmIsAddressValid((PVOID)PspCreateThreadNotifyRoutine))
			{
				NotifyLog(("%s(%d): PspCreateThreadNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateThreadNotifyRoutine));
				g_hg_map[eHG_PspCreateThreadNotifyRoutine] = PspCreateThreadNotifyRoutine;
			}
			HgPsRemoveCreateThreadNotifyRoutine(DummyCreateThreadNotify);
		}
	}

	if (Address && !PspCreateThreadNotifyRoutine)
	{
		BYTE MagicByte = 0;
		WORD MagicWord = 0;
		SIZE_T i = 0;
		SIZE_T nInstructionSize = 0;
		SIZE_T nStart = (SIZE_T)Address;
		SIZE_T nEnd = (SIZE_T)Address + PAGE_SIZE;

		switch(g_WindowsVersion){
		case eWin_2000:
			MagicByte = 0xBA;
			break;
		case eWin_XP:
		case eWin_2003:
		case eWin_2003_SP1_SP2:
		case eWin_Vista:
		case eWin_Vista_SP1_SP2:
		case eWin_7:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBE;
#endif
			break;
		case eWin_8:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBB;
#endif
			break;
		}

		if (MagicByte == 0){
			return;
		}

		for (i = nStart; i < nEnd; i += nInstructionSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE)
			{
				DISASM MyDisasm;

				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
				MyDisasm.Archi = 64;
#else
				MyDisasm.Archi = 0;
#endif
				nInstructionSize = Disasm(&MyDisasm);

				if (nInstructionSize == 0){
					break;
				}
#if defined(_WIN64)
				else if ((nInstructionSize == 7) && (MagicByte == *(PBYTE)((SIZE_T)i + 2)) && (MagicWord == *(PDWORD)(SIZE_T)i))
				{
					DWORD dwOffset = *(PDWORD)(i + 3);
					PspCreateThreadNotifyRoutine = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);

					if (PspCreateThreadNotifyRoutine > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)PspCreateThreadNotifyRoutine) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: PspCreateThreadNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateThreadNotifyRoutine));
						g_hg_map[eHG_PspCreateThreadNotifyRoutine] = PspCreateThreadNotifyRoutine;
						break;
					}
				}
#else
				else if ((nInstructionSize == 5) && (MagicByte == *(PBYTE)((SIZE_T)i)))
				{
					PspCreateThreadNotifyRoutine = *(PSIZE_T)(i + 1);

					if (PspCreateThreadNotifyRoutine > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)PspCreateThreadNotifyRoutine) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: PspCreateThreadNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspCreateThreadNotifyRoutine));
						g_hg_map[eHG_PspCreateThreadNotifyRoutine] = PspCreateThreadNotifyRoutine;
						break;
					}
				}
#endif
			}
		}
	}
}

void EnumLoadImageNotify(PITP pItp)
{
	SIZE_T PspLoadImageRoutine = g_hg_map[eHG_PspLoadImageNotifyRoutine];

	SIZE_T nPspMaxLoadImageNotify = 0;
	SIZE_T i = 0;
	SIZE_T NotifyAddress = 0;
	SIZE_T EX_CALLBACK_MASK = 0;
	SIZE_T KnownAddress = 0;

#if defined(_WIN64)
	EX_CALLBACK_MASK = (SIZE_T)~0x0F;
#else
	EX_CALLBACK_MASK = (SIZE_T)~0x07;
#endif

	if (!PspLoadImageRoutine){
		GetPspLoadImageNotifyRoutine();
		PspLoadImageRoutine = g_hg_map[eHG_PspLoadImageNotifyRoutine];
	}

	if (PspLoadImageRoutine){
		NotifyLog(("%s(%d): PspLoadImageNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspLoadImageRoutine));
		switch (g_WindowsVersion){
		case eWin_XP:
			nPspMaxLoadImageNotify = 8;
			break;
		case eWin_7:
		case eWin_8:
			nPspMaxLoadImageNotify = 64;
			break;
		}

		for (i = 0; i < nPspMaxLoadImageNotify; i++){
			if (HgMmIsAddressValid((PVOID)(PspLoadImageRoutine + i * sizeof(SIZE_T))) == FALSE	||
				(PspLoadImageRoutine + i * sizeof(SIZE_T)) < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			NotifyAddress = *(PSIZE_T)(PspLoadImageRoutine + i * sizeof(SIZE_T));

			//
			// EX_CALLBACK_MASK depend on struct _EX_CALLBACK and struct _EX_FAST_REF.
			// sizeof(PVOID) depend on Value(field in struct _EX_FAST_REF), 
			// this field point to struct _EX_CALLBACK_ROUTINE_BLOCK.
			// The function pointer is the second field, 
			// so the offset is that base plus sizeof(EX_RUNDOWN_REF)(that is sizeof(PVOID)).
			// 
			if (HgMmIsAddressValid((PVOID)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID))) == FALSE	||
				NotifyAddress < (SIZE_T)MM_SYSTEM_RANGE_START)
			{
				break;
			}
			else
			{
				KnownAddress = *(PSIZE_T)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID));
			}

			if (KnownAddress && HgMmIsAddressValid((PVOID)KnownAddress))
			{
				if (pItp->notify.nMaxCount > pItp->notify.nCount){
					pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_LoadImage;
					pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = KnownAddress;
					pItp->notify.NotifyData[pItp->notify.nCount].Comment = NotifyAddress;
				}
				pItp->notify.nCount ++;
			}
		}
	}
}

void GetPspLoadImageNotifyRoutine()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T PspLoadImageNotifyRoutine = 0;
	SIZE_T Address = 0;

	if (g_hg_map[eHG_PsSetLoadImageNotifyRoutine] != 0){
		Address = g_hg_map[eHG_PsSetLoadImageNotifyRoutine];
	}else{
		Address = (SIZE_T)&PsSetLoadImageNotifyRoutine;
	}

	if (Address)
	{
		status = HgPsSetLoadImageNotifyRoutine(DummyLoadImageNotify);
		if (NT_SUCCESS(status)){
			PspLoadImageNotifyRoutine = LookupPspSetXNotifyRoutineAddress((PVOID)Address, (PVOID)&DummyLoadImageNotify, eNT_LoadImage);
			if (PspLoadImageNotifyRoutine && HgMmIsAddressValid((PVOID)PspLoadImageNotifyRoutine))
			{
				NotifyLog(("%s(%d): Method 1: PspLoadImageNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspLoadImageNotifyRoutine));
				g_hg_map[eHG_PspLoadImageNotifyRoutine] = PspLoadImageNotifyRoutine;
			}
			HgPsRemoveLoadImageNotifyRoutine(DummyLoadImageNotify);
		}
	}

	if (Address && !PspLoadImageNotifyRoutine)
	{
		BYTE MagicByte = 0;
		WORD MagicWord = 0;
		SIZE_T i = 0;
		SIZE_T nInstructionSize = 0;
		SIZE_T nStart = (SIZE_T)Address;
		SIZE_T nEnd = (SIZE_T)Address + PAGE_SIZE;

		switch(g_WindowsVersion){
		case eWin_2000:
			MagicByte = 0xBA;
			break;
		case eWin_XP:
		case eWin_2003:
		case eWin_2003_SP1_SP2:
		case eWin_Vista:
		case eWin_Vista_SP1_SP2:
		case eWin_7:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBE;
#endif
			break;
		case eWin_8:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBB;
#endif
			break;
		}

		if (MagicByte == 0){
			return;
		}

		for (i = nStart; i < nEnd; i += nInstructionSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE)
			{
				DISASM MyDisasm;

				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
				MyDisasm.Archi = 64;
#else
				MyDisasm.Archi = 0;
#endif
				nInstructionSize = Disasm(&MyDisasm);

				if (nInstructionSize == 0){
					break;
				}
#if defined(_WIN64)
				else if ((nInstructionSize == 7) && (MagicByte == *(PBYTE)((SIZE_T)i + 2)) && (MagicWord == *(PDWORD)(SIZE_T)i))
				{
					DWORD dwOffset = *(PDWORD)(i + 3);
					PspLoadImageNotifyRoutine = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);

					if (PspLoadImageNotifyRoutine > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)PspLoadImageNotifyRoutine) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: PspLoadImageNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspLoadImageNotifyRoutine));
						g_hg_map[eHG_PspLoadImageNotifyRoutine] = PspLoadImageNotifyRoutine;
						break;
					}
				}
#else
				else if ((nInstructionSize == 5) && (MagicByte == *(PBYTE)((SIZE_T)i)))
				{
					PspLoadImageNotifyRoutine = *(PSIZE_T)(i + 1);

					if (PspLoadImageNotifyRoutine > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)PspLoadImageNotifyRoutine) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: PspLoadImageNotifyRoutine = 0x%p\n", __FILE__, __LINE__, PspLoadImageNotifyRoutine));
						g_hg_map[eHG_PspLoadImageNotifyRoutine] = PspLoadImageNotifyRoutine;
						break;
					}
				}
#endif
			}
		}
	}
}

/************************************************************************/
/* 
 * wrk-v1.2\base\ntos\cofig\Cmhook.c

#define CM_MAX_CALLBACKS    100

typedef struct _CM_CALLBACK_CONTEXT_BLOCK {
	LARGE_INTEGER               Cookie;             // to identify a specific callback for deregistration purposes
	LIST_ENTRY                  ThreadListHead;     // Active threads inside this callback
	EX_PUSH_LOCK                ThreadListLock;     // synchronize access to the above
	PVOID                       CallerContext;
} CM_CALLBACK_CONTEXT_BLOCK, *PCM_CALLBACK_CONTEXT_BLOCK;

typedef struct _CM_ACTIVE_NOTIFY_THREAD {
	LIST_ENTRY  ThreadList;
	PETHREAD    Thread;
} CM_ACTIVE_NOTIFY_THREAD, *PCM_ACTIVE_NOTIFY_THREAD;

#define CmpLockContext(Context)    ExAcquirePushLockExclusive(&((Context)->ThreadListLock))
#define CmpUnlockContext(Context)  ExReleasePushLock(&((Context)->ThreadListLock))


#ifdef ALLOC_DATA_PRAGMA
#pragma data_seg("PAGEDATA")
#endif

ULONG       CmpCallBackCount = 0;
EX_CALLBACK CmpCallBackVector[CM_MAX_CALLBACKS] = {0};

#ifdef ALLOC_DATA_PRAGMA
#pragma data_seg()
#endif
*/
/************************************************************************/
void EnumRegistryNotify(PITP pItp)
{
	SIZE_T CmpCallBackVector = g_hg_map[eHG_CmpCallBackVector];

	SIZE_T NotifyAddress = 0;
	SIZE_T EX_CALLBACK_MASK = 0;
	SIZE_T KnownAddress = 0;

	SIZE_T CallbackFunctionOffset = 0;

#if defined(_WIN64)
	EX_CALLBACK_MASK = (SIZE_T)~0x0F;
#else
	EX_CALLBACK_MASK = (SIZE_T)~0x07;
#endif

	if (!CmpCallBackVector){
		GetCmpCallBackVector();
		CmpCallBackVector = g_hg_map[eHG_CmpCallBackVector];
	}

	if (CmpCallBackVector && HgMmIsAddressValid((PVOID)CmpCallBackVector))
	{
		switch (g_WindowsVersion){
		case eWin_XP:
			{
				SIZE_T nCmMaxCallback = 0;
				SIZE_T Entry = CmpCallBackVector;

				while (nCmMaxCallback < 0x64 && HgMmIsAddressValid((PVOID)Entry))
				{
					NotifyAddress = *(PSIZE_T)Entry;
					if (NotifyAddress && HgMmIsAddressValid((PVOID)NotifyAddress))
					{
						if (NotifyAddress < (SIZE_T)MM_SYSTEM_RANGE_START	||
							HgMmIsAddressValid((PVOID)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID))) == FALSE)
						{
							break;
						}

						KnownAddress = *(PSIZE_T)(NotifyAddress & EX_CALLBACK_MASK + sizeof(PVOID));
						if (pItp->notify.nMaxCount > pItp->notify.nCount){
							pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_CmCallBack;
							pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = KnownAddress;
							pItp->notify.NotifyData[pItp->notify.nCount].Comment = NotifyAddress;
						}
						pItp->notify.nCount ++;
					}

//					nCmMaxCallback += sizeof(PVOID);
					nCmMaxCallback ++;
					Entry += sizeof(PVOID);
				}
			}
			break;
		case eWin_7:
		case eWin_8:
			{
				PLIST_ENTRY pFirstEntry = *(PLIST_ENTRY*)CmpCallBackVector;
				PLIST_ENTRY pEntry = pFirstEntry;
#if defined(_WIN64)
				CallbackFunctionOffset = 0x28;
#else
				CallbackFunctionOffset = 0x1C;
#endif
				while ((SIZE_T)pEntry > (SIZE_T)MM_SYSTEM_RANGE_START	&&
					HgMmIsAddressValid((PVOID)pEntry)					&&
					pEntry/*->Flink*/ != (PLIST_ENTRY)pFirstEntry)
				{
					NotifyLog(("(SIZE_T)pEntry + 0x28 = 0x%p\n", *(PSIZE_T)((SIZE_T)pEntry + 0x28)));
					if (HgMmIsAddressValid((PVOID)((SIZE_T)pEntry + CallbackFunctionOffset)))
					{
						NotifyLog(("(SIZE_T)pEntry + CallbackFunctionOffset = 0x%p\n", *(PSIZE_T)((SIZE_T)pEntry + CallbackFunctionOffset)));
						KnownAddress = *(PSIZE_T)((SIZE_T)pEntry + CallbackFunctionOffset);
						if (pItp->notify.nMaxCount > pItp->notify.nCount && HgMmIsAddressValid((PVOID)KnownAddress)){
							pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_CmCallBack;
							pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = KnownAddress;
							pItp->notify.NotifyData[pItp->notify.nCount].Comment = (SIZE_T)pEntry;
						}
						pItp->notify.nCount ++;
					}
					pEntry = pEntry->Flink;
				}
			}
			break;
		}
	}
}

void GetCmpCallBackVector()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T CmpCallBackVector = 0;
	SIZE_T Address = 0;

	if (g_hg_map[eHG_CmUnRegisterCallback] != 0){
		Address = g_hg_map[eHG_CmUnRegisterCallback];
	}else{
		Address = (SIZE_T)&CmUnRegisterCallback;
	}
	
	if (Address && !CmpCallBackVector)
	{
		WORD MagicWord = 0;
		BYTE MagicByte = 0;
		SIZE_T i = 0;
		SIZE_T nInstructionSize = 0;
		SIZE_T nStart = (SIZE_T)Address;
		SIZE_T nEnd = (SIZE_T)Address + PAGE_SIZE;

		switch(g_WindowsVersion){
		case eWin_2000:
		case eWin_XP:
		case eWin_2003:
		case eWin_2003_SP1_SP2:
			MagicByte = 0xBB;
			break;
		case eWin_Vista:
		case eWin_Vista_SP1_SP2:
			MagicByte = 0xBE;
			break;
		case eWin_7:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBF;
#endif
			break;
		case eWin_8:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBE;
#endif
			break;
		}

		if (MagicByte == 0){
			return;
		}

		for (i = nStart; i < nEnd; i += nInstructionSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE)
			{
				DISASM MyDisasm;

				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
				MyDisasm.Archi = 64;
#else
				MyDisasm.Archi = 0;
#endif
				nInstructionSize = Disasm(&MyDisasm);

				if (nInstructionSize == 0){
					break;
				}
#if defined(_WIN64)
				else if ((nInstructionSize == 7) && (MagicByte == *(PBYTE)((SIZE_T)i + 2)) && (MagicWord == *(PWORD)i))
				{
					DWORD dwOffset = *(PDWORD)(i + 3);
					CmpCallBackVector = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);

					if (CmpCallBackVector > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)CmpCallBackVector) == TRUE)
					{
						NotifyLog(("%s(%d): Method2(x64): CmpCallBackVector = 0x%p\n", __FILE__, __LINE__, CmpCallBackVector));
						g_hg_map[eHG_CmpCallBackVector] = CmpCallBackVector;
						break;
					}
				}
#else
				else if ((nInstructionSize == 5) && (MagicByte == *(PBYTE)((SIZE_T)i)))
				{
					CmpCallBackVector = *(PSIZE_T)(i + 1);

					if (CmpCallBackVector > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)CmpCallBackVector) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: CmpCallBackVector = 0x%p\n", __FILE__, __LINE__, CmpCallBackVector));
						g_hg_map[eHG_CmpCallBackVector] = CmpCallBackVector;
						break;
					}
				}
#endif
			}
		}
	}
}

void EnumShutdownNotify(PITP pItp)
{
	PLIST_ENTRY IopNotifyShutdownQueueHead = (PLIST_ENTRY)g_hg_map[eHG_IopNotifyShutdownQueueHead];
	SIZE_T KnownAddress = 0;

	if (IopNotifyShutdownQueueHead == NULL)
	{
		GetIopNotifyShutdownQueueHead();
		IopNotifyShutdownQueueHead = (PLIST_ENTRY)g_hg_map[eHG_IopNotifyShutdownQueueHead];
	}
//	NotifyLog(("----------------------------------IopNotifyShutdownQueueHead = %p\n", IopNotifyShutdownQueueHead));
	if (IopNotifyShutdownQueueHead && HgMmIsAddressValid(IopNotifyShutdownQueueHead))
	{
		PLIST_ENTRY pEntry = IopNotifyShutdownQueueHead->Flink;
		while (HgMmIsAddressValid(pEntry) && pEntry != IopNotifyShutdownQueueHead)
		{
			KnownAddress = (SIZE_T)pEntry + sizeof(LIST_ENTRY);
			if (KnownAddress && HgMmIsAddressValid((PVOID)KnownAddress))
			{
				SIZE_T DeviceObject = *(PSIZE_T)KnownAddress;
				if (DeviceObject && HgMmIsAddressValid((PVOID)DeviceObject))
				{
					if (pItp->notify.nMaxCount > pItp->notify.nCount){
						pItp->notify.NotifyData[pItp->notify.nCount].nt = eNT_Shutdown;
						pItp->notify.NotifyData[pItp->notify.nCount].pCallbackAddress = GetShutdownDispatch((PDEVICE_OBJECT)DeviceObject);
						pItp->notify.NotifyData[pItp->notify.nCount].Comment = DeviceObject;
					}
					pItp->notify.nCount ++;
				}
			}
			pEntry = pEntry->Flink;
		}
	}
}

void GetIopNotifyShutdownQueueHead()
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	SIZE_T IopNotifyShutdownQueueHead = 0;
	SIZE_T Address = 0;

	if (g_hg_map[eHG_IoRegisterShutdownNotification] != 0){
		Address = g_hg_map[eHG_IoRegisterShutdownNotification];
	}else{
		Address = (SIZE_T)&IoRegisterShutdownNotification;
	}

	if (Address && !IopNotifyShutdownQueueHead)
	{
		WORD MagicWord = 0;
		BYTE MagicByte = 0;
		SIZE_T i = 0;
		SIZE_T nInstructionSize = 0;
		SIZE_T nStart = (SIZE_T)Address;
		SIZE_T nEnd = (SIZE_T)Address + PAGE_SIZE;

		switch(g_WindowsVersion){
		case eWin_2000:
		case eWin_XP:
		case eWin_2003:
		case eWin_2003_SP1_SP2:
			MagicByte = 0xB9;
			break;
		case eWin_Vista:
		case eWin_Vista_SP1_SP2:
		case eWin_7:
		case eWin_8:
#if defined(_WIN64)
			MagicWord = 0x8D48;
			MagicByte = 0x0D;
#else
			MagicByte = 0xBF;
#endif
			break;
		}

		if (MagicByte == 0){
			return;
		}

		for (i = nStart; i < nEnd; i += nInstructionSize)
		{
			if (HgMmIsAddressValid((PVOID)i) == TRUE)
			{
				DISASM MyDisasm;

				(void) memset (&MyDisasm, 0, sizeof(DISASM));
				MyDisasm.EIP = (UIntPtr)i;
#if defined(_WIN64)
				MyDisasm.Archi = 64;
#else
				MyDisasm.Archi = 0;
#endif
				nInstructionSize = Disasm(&MyDisasm);

				if (nInstructionSize == 0){
					break;
				}
#if defined(_WIN64)
				else if ((nInstructionSize == 7) && (MagicByte == *(PBYTE)((SIZE_T)i + 2)) && (MagicWord == *(PWORD)i))
				{
					DWORD dwOffset = *(PDWORD)(i + 3);
					IopNotifyShutdownQueueHead = i + 7 + ((SIZE_T)dwOffset | 0xFFFFFFFF00000000);

					if (IopNotifyShutdownQueueHead > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)IopNotifyShutdownQueueHead) == TRUE)
					{
						NotifyLog(("%s(%d): Method2(x64): IopNotifyShutdownQueueHead = 0x%p\n", __FILE__, __LINE__, IopNotifyShutdownQueueHead));
						g_hg_map[eHG_IopNotifyShutdownQueueHead] = IopNotifyShutdownQueueHead;
						break;
					}
				}
#else
				else if ((nInstructionSize == 5) && (MagicByte == *(PBYTE)((SIZE_T)i)))
				{
					IopNotifyShutdownQueueHead = *(PSIZE_T)(i + 1);

					if (IopNotifyShutdownQueueHead > (SIZE_T)MM_SYSTEM_RANGE_START	&& 
						HgMmIsAddressValid((PVOID)IopNotifyShutdownQueueHead) == TRUE)
					{
						NotifyLog(("%s(%d): Method2: IopNotifyShutdownQueueHead = 0x%p\n", __FILE__, __LINE__, IopNotifyShutdownQueueHead));
						g_hg_map[eHG_IopNotifyShutdownQueueHead] = IopNotifyShutdownQueueHead;
						break;
					}
				}
#endif
			}
		}
	}
}

SIZE_T GetShutdownDispatch(PDEVICE_OBJECT DeviceObject)
{
	PDRIVER_OBJECT DriverObject = NULL;
	if (DeviceObject && HgMmIsAddressValid(DeviceObject))
	{
		DriverObject = DeviceObject->DriverObject;
		if (DriverObject && HgMmIsAddressValid(DriverObject))
		{
			return (SIZE_T)DriverObject->MajorFunction[IRP_MJ_SHUTDOWN];
		}
	}
	return 0;
}