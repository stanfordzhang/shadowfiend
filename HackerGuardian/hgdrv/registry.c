
#include "registry.h"

NTSTATUS
RegistryEnumerateKey(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__in	SIZE_T	nOutSize,
	__inout	PSIZE_T	ReturnSize
	)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PITP pInItp = (PITP)pInBuffer;
	PITP pOutItp = (PITP)pOutBuffer;

	PWCHAR pwszKey = pInItp->in_registry.Path;
	PWCHAR pwszOutKey = pOutItp->out_registry.Keys;

	UNICODE_STRING usKey;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE hKey = NULL;
	ULONG i = 0;
	SIZE_T nTotalLen = 0;
	PETHREAD pThread = NULL;
	CHAR PreMode = 0;
	SIZE_T nMaxLength = pOutItp->out_registry.nMaxLength / sizeof(WCHAR);

	if (pInBuffer == NULL || pOutBuffer == NULL){
		return status;
	}

	SzhInitUnicodeString(&usKey, pwszKey);
	InitializeObjectAttributes(&ObjectAttributes, &usKey, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	pThread = HgPsGetCurrentThread();
	PreMode = ChangePreMode(pThread);

	status = HgZwOpenKey(&hKey, GENERIC_ALL, &ObjectAttributes);
	if (!NT_SUCCESS(status)){
		RecoverPreMode(pThread, PreMode);
		return status;
	}

	while (TRUE){
		ULONG nSize = 0;
		ULONG NumberOfBytes = 0;
		status = HgZwEnumerateKey(hKey, i, KeyBasicInformation, NULL, 0, &nSize);
		
		if (STATUS_BUFFER_TOO_SMALL == status){
			PKEY_BASIC_INFORMATION pBasicInfo = NULL;
			NumberOfBytes = nSize + 0x100;
			pBasicInfo = (PKEY_BASIC_INFORMATION)HgExAllocatePoolWithTag(PagedPool, NumberOfBytes, HG_MEM_TAG);
			if (pBasicInfo == NULL){
				status = STATUS_UNSUCCESSFUL;
				break;
			}
			memset(pBasicInfo, 0, NumberOfBytes);
			status = HgZwEnumerateKey(hKey, i, KeyBasicInformation, pBasicInfo, NumberOfBytes, &nSize);
			if (NT_SUCCESS(status)){
				if (nMaxLength - nTotalLen <= pBasicInfo->NameLength){
					HgExFreePoolWithTag(pBasicInfo, 0);
					status = STATUS_BUFFER_OVERFLOW;
					break;
				}
//				RegLog(("Name = %S, Length = %d\n", pBasicInfo->Name, pBasicInfo->NameLength / sizeof(WCHAR)));
				wcsncpy(pwszOutKey + nTotalLen, pBasicInfo->Name, pBasicInfo->NameLength / sizeof(WCHAR));
				nTotalLen += pBasicInfo->NameLength / sizeof(WCHAR);
				pwszOutKey[nTotalLen] = L'\0';
				nTotalLen ++;
			}
			HgExFreePoolWithTag(pBasicInfo, 0);
		}else if (STATUS_NO_MORE_ENTRIES == status){
			status = STATUS_SUCCESS;
			break;
		}
		i++;
	}
//	RegLog(("nTotalLen = %d\n", nTotalLen));
	HgNtClose(hKey);
	RecoverPreMode(pThread, PreMode);

	return status;
}

NTSTATUS RegistryEnumerateValueKey(
	__in	PVOID	pInBuffer,
	__in	SIZE_T	nInSize,
	__inout	PVOID	pOutBuffer,
	__in	SIZE_T	nOutSize,
	__inout	PSIZE_T	ReturnSize)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	PITP pInItp = (PITP)pInBuffer;
	PITP pOutItp = (PITP)pOutBuffer;

	PWCHAR pwszKey = pInItp->in_registry.Path;
	PWCHAR pwszOutKey = pOutItp->out_registry.Keys;

	UNICODE_STRING usKey;
	OBJECT_ATTRIBUTES ObjectAttributes;
	HANDLE hKey = NULL;
	ULONG i = 0;
	SIZE_T nOffset = 0;
	PETHREAD pThread = NULL;
	CHAR PreMode = 0;
	SIZE_T nMaxLength = pOutItp->out_registry.nMaxLength / sizeof(WCHAR);

	if (pInBuffer == NULL || pOutBuffer == NULL){
		return status;
	}
	
	SzhInitUnicodeString(&usKey, pwszKey);
	InitializeObjectAttributes(&ObjectAttributes, &usKey, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
	pThread = HgPsGetCurrentThread();
	PreMode = ChangePreMode(pThread);

	status = HgZwOpenKey(&hKey, GENERIC_ALL, &ObjectAttributes);
	if (!NT_SUCCESS(status)){
		RecoverPreMode(pThread, PreMode);
		return status;
	}

	while (TRUE){
		ULONG nSize = 0;
		ULONG NumberOfBytes = 0;
		status = HgZwEnumerateValueKey(hKey, i, KeyValueFullInformation, NULL, 0, &nSize);

		if (STATUS_BUFFER_TOO_SMALL == status){
			PKEY_VALUE_FULL_INFORMATION pValueFullInfo = NULL;
			NumberOfBytes = nSize + 0x100;
			pValueFullInfo = (PKEY_VALUE_FULL_INFORMATION)HgExAllocatePoolWithTag(PagedPool, NumberOfBytes, HG_MEM_TAG);
			if (pValueFullInfo == NULL){
				status = STATUS_UNSUCCESSFUL;
				break;
			}
			memset(pValueFullInfo, 0, NumberOfBytes);
			status = HgZwEnumerateValueKey(hKey, i, KeyValueFullInformation, pValueFullInfo, NumberOfBytes, &nSize);
			if (NT_SUCCESS(status)){
				if (nMaxLength - nOffset <= pValueFullInfo->NameLength){
					HgExFreePoolWithTag(pValueFullInfo, 0);
					status = STATUS_BUFFER_OVERFLOW;
					break;
				}
// 				KdPrint(("ulSize: %d, DataOffset: %d, DataLength: %d, NameLength: %d\n", 
// 					nSize, pValueFullInfo->DataOffset, pValueFullInfo->DataLength, pValueFullInfo->NameLength));

// 				if (pValueFullInfo->Type == REG_SZ)
// 				{
// 					KdPrint(("Value: %S, Data: %S\n", pValueFullInfo->Name, pValueFullInfo->Name + pValueFullInfo->NameLength / sizeof(WCHAR) + 1));
// 				}
				//RegLog(("Name = %S, Length = %d\n", pValueFullInfo->Name, pValueFullInfo->NameLength / sizeof(WCHAR)));

				*(PSIZE_T)((SIZE_T)pwszOutKey + nOffset) = (SIZE_T)nSize;
				nOffset += sizeof(SIZE_T);
				memcpy((PVOID)((SIZE_T)pwszOutKey + nOffset), pValueFullInfo, nSize);
				nOffset += nSize;
			}
			HgExFreePoolWithTag(pValueFullInfo, 0);
		}else if (STATUS_NO_MORE_ENTRIES == status){
			status = STATUS_SUCCESS;
			break;
		}
		i++;
	}
//	RegLog(("nTotalLen = %d\n", nOffset));
	HgNtClose(hKey);
	RecoverPreMode(pThread, PreMode);

	return status;
}