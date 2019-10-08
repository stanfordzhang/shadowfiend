// TestDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
//ʹ��CTL_CODE�������winioctl.h
#include <winioctl.h>
#include "..\hgdrv\hgioctl.h"
#include "..\hgdll\hgdll.h"
#include "..\references\szdef.h"
#include <atlstr.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <iomanip> 
#include <map>
#include <vector>
#include <dbghelp.h>
#include "EvaluateBeaEngine.h"

#pragma comment(lib,"dbghelp")

#include <winsock.h>
#pragma comment(lib,"ws2_32.lib")

#include "test.h"

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */ 

#ifdef __cplusplus
}
#endif /* __cplusplus */ 

using namespace std;

BOOL CALLBACK EnumDirTreeProc(LPCTSTR FilePath,PVOID CallerData)
{
   printf("%ws,%ws\n",FilePath,CallerData);
   return FALSE;
   /*
   2������������EnumDirTree��������
   FilePath�ǵ�һ���ҵ���ȫ·��
   CallerData��EnumDirTree�����һ������
   ������Ϊ�ڻص����������Ϣ��������������Ƚ�֮���
   ����false����Լ��������������stop��
   */
}

class _A{
public:
	_A(){
		printf("_A::constructor!\n");
	}
	~_A(){
		printf("_A::destructor!\n");
	}
};

int _tmain(int argc, _TCHAR* argv[])
{
	TestMov();
	system("pause");
	return 0;
	//
	// http://blog.csdn.net/vito13
	// 
   HANDLE hProcess;
 
   SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);
 
   hProcess = GetCurrentProcess();
 
   if (SymInitialize(hProcess, NULL, TRUE))
   {
      // SymInitialize returned success
      /*
      BOOL WINAPI EnumDirTree( �����ļ�������·��
      __in_opt      HANDLE hProcess,
      __in          PCTSTR RootPath, ��ʼ������·��
      __in          PCTSTR InputPathName, �������ļ���
      __out         LPTSTR OutputPathBuffer, ������ȫ·��
      __in          PENUMDIRTREE_CALLBACK Callback, �ص�����
      __in          PVOID CallbackData �ص������õĵ�������
      );
     
      BOOL CALLBACK EnumDirTreeProc(
      [in]                 LPCTSTR FilePath,
      [in]                 PVOID CallerData
      );
      */
 
      WCHAR buf[MAX_PATH];
      buf[0] = L'\0';
      WCHAR bufP[] = L"lib";
      if(FALSE == EnumDirTreeW(hProcess,L"D:\\VS2010Temp\\output",L"hgdrv.sys",buf,EnumDirTreeProc,(PVOID)bufP))
      {
         // ������ӡ����Ϊ����Ļص��Ƿ���false��
         printf("EnumDirTree returned error : %d\n", GetLastError());
      }
   }
   else
   {
      // SymInitialize failed
      printf("SymInitialize returned error : %d\n", GetLastError());
   }
 
 
   system("pause");
   return 0;
// 	TTT t = {0, 1, 7, 9};
// 	int a = 3;

// 	printf("sizeof(long double) = %d\n", sizeof(long double));
	/*
	* ö��ע���
	HKEY hKey;
	RegOpenKeyA(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\Shell\\BagMRU", &hKey);
	char lpValueName[MAX_PATH] = {0};
	DWORD lpcchValueName = 24;
	DWORD lpType = 0;
	BYTE lpData[MAX_PATH] = {0};
	DWORD lpcbData = 24;
	DWORD i = 0;
	LSTATUS status;
	status = RegEnumValueA(hKey, 0, lpValueName, &lpcchValueName, NULL, &lpType, lpData, &lpcbData);
	if (status != 0x103)
	{
	do 
	{
	lpcchValueName = 200;
	lpcbData = 200;
	//if (strstr((const char*)lpData, "asktao"))
	{
	if (lpType == REG_BINARY){
	for (int j = 0; j < 8; j++)
	{
	printf("%02X", lpData[j]);
	}
	printf("\n");
	}else if (lpType == REG_DWORD){
	printf("0x%08X\n", *(PDWORD)lpData);
	}
	//printf("Value Name = %s\n", lpData);
	}
	++ i;
	status = RegEnumValueA(hKey, i, lpValueName, &lpcchValueName, NULL, &lpType, lpData, &lpcbData);
	} while (status != 0x103);
	}
	RegCloseKey(hKey);
	*/
	//////////////////////////////////////////////////////////////////////////

// 	Test32();
// 	system("pause");
// 	return 0;

	TCHAR strCache[MAX_PATH] = TEXT("c:");
	UINT nRtn = 0;
	nRtn = GetDriveType(strCache);
	_tprintf(TEXT("nRtn = %d, driver type = %s\n"), nRtn, strCache);

	TCHAR strPath[MAX_PATH] = TEXT("c:\\asdfasdf dd\\fasd\\sdfas\\asdfasdf");
	PathRemoveFileSpec(strPath);
	_tprintf(TEXT("strPath = %s\n"), strPath);

	wchar_t * pCh = L"www.inkcool.com";
	const wchar_t * pFind = wcsrchr(L"www.inkcool.com", L'.');
	if ( pFind != NULL)
	{
		wprintf(L"%s\n", pFind);    //����ֱ��printf(pFind);printf("/n");��ߵı��ʽ�Ǻ϶�Ϊһ�ı�﷽����
	}

	printf("sizeof(size_t) = %d\n", sizeof(size_t));
	printf("sizeof(intptr_t) = %d\n", sizeof(intptr_t));
	printf("sizeof(uintptr_t) = %d\n", sizeof(uintptr_t));
	printf("sizeof(ULONG_PTR) = %d\n", sizeof(ULONG_PTR));
	printf("sizeof(void*) = %d\n", sizeof(void*));
	printf("sizeof(ULONG) = %d\n", sizeof(ULONG));
	printf("sizeof(DWORD) = %d\n", sizeof(DWORD));
	printf("sizeof(DWORD_PTR) = %d\n", sizeof(DWORD_PTR));
	printf("sizeof(SIZE_T) = %d\n", sizeof(SIZE_T));
	printf("sizeof(LONG_PTR) = %d\n", sizeof(LONG_PTR));
	printf("sizeof(LARGE_INTEGER) = %d\n", sizeof(LARGE_INTEGER));// ö��������x86��x64�ж���ռ4���ֽ�
	printf("sizeof(LARGE_INTEGER) = %d\n", sizeof(LARGE_INTEGER));

	LONG32 a32 = -123;
	SSIZE_T s64 = a32;
	printf("s64 = 0x%p\n", s64);

	string strTmp = "my name is stanford1";
	if (strTmp.length() > (16 - 1)){
		strTmp = strTmp.substr(0, 16 - 1);
	}
	printf("strTmp = %s\n", strTmp.c_str());

	// 	HANDLE hDevice = 
	// 		CreateFileW(SZH_USER_MODE_DEVICE_NAME,
	// 		GENERIC_READ | GENERIC_WRITE,
	// 		0,		// share mode none
	// 		NULL,	// no security
	// 		OPEN_EXISTING,
	// 		FILE_ATTRIBUTE_NORMAL,
	// 		NULL );		// no template
	// 
	// 	if (hDevice == INVALID_HANDLE_VALUE)
	// 	{
	// 		SzLogA("Failed to obtain file handle to device: "
	// 			"%s with Win32 error code: %d\n",
	// 			"MyWDMDevice", GetLastError() );
	// 		return 1;
	// 	}
	// 
	// 	UCHAR InputBuffer[10];
	// 	UCHAR OutputBuffer[10];
	// 	//�����뻺����ȫ���ó�0XBB
	// 	memset(InputBuffer,0xBB,10);
	// 	DWORD dwOutput;
	// 	//���뻺������Ϊ���룬�����������Ϊ���
	// 
	// 	BOOL bRet;
	// 	bRet = DeviceIoControl(hDevice, SZH_IOCTL_BUFFERED, InputBuffer, 10, &OutputBuffer, 10, &dwOutput, NULL);
	// 	SzLogA(">>>>>User Mode<<<<<%d ",bRet);
	// 	if (bRet)
	// 	{
	// 		SzLogA("Output buffer:%d bytes\n",dwOutput);
	// 		for (int i=0;i<(int)dwOutput;i++)
	// 		{
	// 			SzLogA(">>>>>User Mode<<<<<%02X ",OutputBuffer[i]);
	// 		}
	// 		SzLogA("\n");
	// 	}

	// 	bRet = DeviceIoControl(hDevice, IOCTL_TEST2, InputBuffer, 10, &OutputBuffer, 10, &dwOutput, NULL);
	// 	if (bRet)
	// 	{
	// 		printf("Output buffer:%d bytes\n",dwOutput);
	// 		for (int i=0;i<(int)dwOutput;i++)
	// 		{
	// 			printf("%02X ",OutputBuffer[i]);
	// 		}
	// 		printf("\n");
	// 	}
	// 
	// 	bRet = DeviceIoControl(hDevice, IOCTL_TEST3, InputBuffer, 10, &OutputBuffer, 10, &dwOutput, NULL);
	// 	if (bRet)
	// 	{
	// 		printf("Output buffer:%d bytes\n",dwOutput);
	// 		for (int i=0;i<(int)dwOutput;i++)
	// 		{
	// 			printf("%02X ",OutputBuffer[i]);
	// 		}
	// 		printf("\n");
	// 	}

	//	CloseHandle(hDevice);

	int nTestAddr = 0;
#ifdef _WIN64 // _M_IX86 _M_AMD64 
	printf("nTestAddr address: %p\n", &nTestAddr);
#else
	printf("nTestAddr address: %p\n", &nTestAddr);
#endif
	system("pause");
	return 0;
}