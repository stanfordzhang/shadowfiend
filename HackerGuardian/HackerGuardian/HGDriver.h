#ifndef __HG_DRIVER_H__
#define __HG_DRIVER_H__

/************************************************************************/
/* C++ Singleton Pattern
 * http://blog.csdn.net/boyhailong/article/details/6645681
 */
/************************************************************************/

#include "..\hgdll\hgdll.h"
#include "strdef.h"

class CHGDriver{
private:
	//
	// To forbid copy and assignment operation, because of Singleton Pattern.
	//
	CHGDriver(const CHGDriver& RightSides){

	}

	CHGDriver& operator=(const CHGDriver& RightSides){
		if (this == &RightSides){
			return *this;
		}
	}

private:/*public:*/
	CHGDriver()
		: m_pQueryInterface(NULL)
		, m_pEnumerateProcess(NULL)
		, m_pFreeItp(NULL)
		, m_HgEnumerateProcessModule(NULL)
		, m_HgEnumerateProcessThread(NULL)
		, m_HgEnumerateProcessMemory(NULL)
		, m_HgEnumerateDriver(NULL)
		, m_HgRegistryEnumerateKey(NULL)
		, m_HgRegistryEnumerateValueKey(NULL)
		, m_HgKernelEnumerateNotify(NULL)
		, m_HgHookSSDT(NULL)
		, m_HgHookShadowSSDT(NULL)
		, m_HgReadProcessMemoryAux(NULL)
		, m_HgProcessMemoryChangeProtection(NULL)
		, m_HgProcessMemoryFree(NULL)
		, m_HgProcessMemoryZero(NULL)
		, m_HgEnumerateProcessHandle(NULL)
		, m_HgCloseProcessHandle(NULL)
	{
		HMODULE hHG = LoadLibrary(ptszDll);
		if (hHG != NULL){
			m_pQueryInterface = (PSzhQueryInterface)GetProcAddress(hHG, pszQueryInterface);
		}
	}
public:
	~CHGDriver(){
		SzLogA("%s(%d): Enter CHGDriver Destructor!", __FILE__, __LINE__);
		HMODULE hHG = GetModuleHandle(ptszDll);
		if (hHG != NULL){
			FreeLibrary(hHG);
		}
	}

	//
	// interface list
	// 
private:
	PSzhQueryInterface				m_pQueryInterface;
	PSzhEnumerateProcess			m_pEnumerateProcess;
	PSzhFreeItp						m_pFreeItp;
	pfnHgEnumerateProcessModule		m_HgEnumerateProcessModule;
	pfnHgEnumerateProcessThread		m_HgEnumerateProcessThread;
	pfnHgEnumerateProcessMemory		m_HgEnumerateProcessMemory;
	pfnHgProcessMemoryFree			m_HgProcessMemoryFree;
	pfnHgProcessMemoryZero			m_HgProcessMemoryZero;
	pfnHgProcessMemoryChangeProtection	m_HgProcessMemoryChangeProtection;
	pfnHgEnumerateDriver			m_HgEnumerateDriver;
	pfnHgRegistryEnumerateKey		m_HgRegistryEnumerateKey;
	pfnHgRegistryEnumerateValueKey	m_HgRegistryEnumerateValueKey;
	pfnHgKernelEnumerateNotify		m_HgKernelEnumerateNotify;
	pfnHgHookSSDT					m_HgHookSSDT;
	pfnHgHookShadowSSDT				m_HgHookShadowSSDT;
	pfnHgReadProcessMemoryAux		m_HgReadProcessMemoryAux;
	pfnHgEnumerateProcessHandle		m_HgEnumerateProcessHandle;
	pfnHgCloseProcessHandle			m_HgCloseProcessHandle;

public:
	static CHGDriver* GetInstance(){
		// if you use local static variable,
		// you must overload copy/assignment constructor.
		static CHGDriver instance;
		return &instance;
	}

	// 
	// export interface
	// 
public:
	BOOL EnumerateProcess(PITP*	ppItp){
		if (m_pQueryInterface != NULL){
			if (m_pEnumerateProcess == NULL){
				m_pEnumerateProcess = (PSzhEnumerateProcess)m_pQueryInterface(eEnumerateProcess);
				if (m_pEnumerateProcess != NULL){
					return m_pEnumerateProcess(ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_pEnumerateProcess(ppItp);
			}
		}
		return FALSE;
	}

	void FreeItp(PITP pItp){
		if (m_pQueryInterface != NULL){
			if (m_pFreeItp == NULL){
				m_pFreeItp =
					(PSzhFreeItp)m_pQueryInterface(eFreeItp);
				if (m_pFreeItp != NULL){
					m_pFreeItp(pItp);
				}else{
					SzLogA("Query interface [FreeItp] failed!\n");
					return;
				}
			}else{
				m_pFreeItp(pItp);
			}
		}
	}

	BOOL EnumerateProcessModule(SIZE_T nProcessId, SIZE_T nEProcess, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgEnumerateProcessModule == NULL){
				m_HgEnumerateProcessModule = (pfnHgEnumerateProcessModule)m_pQueryInterface(eEnumerateProcessModule);
				if (m_HgEnumerateProcessModule != NULL){
					return m_HgEnumerateProcessModule(nProcessId, nEProcess, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgEnumerateProcessModule(nProcessId, nEProcess, ppItp);
			}
		}
		return FALSE;
	}

	BOOL EnumerateProcessThread(SIZE_T nProcessId, SIZE_T nEProcess, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgEnumerateProcessThread == NULL){
				m_HgEnumerateProcessThread = (pfnHgEnumerateProcessThread)m_pQueryInterface(eEnumerateProcessThread);
				if (m_HgEnumerateProcessThread != NULL){
					return m_HgEnumerateProcessThread(nProcessId, nEProcess, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgEnumerateProcessThread(nProcessId, nEProcess, ppItp);
			}
		}
		return FALSE;
	}

	BOOL EnumerateProcessMemory(SIZE_T nProcessId, SIZE_T nEProcess, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgEnumerateProcessMemory == NULL){
				m_HgEnumerateProcessMemory = (pfnHgEnumerateProcessMemory)m_pQueryInterface(eEnumerateProcessMemory);
				if (m_HgEnumerateProcessMemory != NULL){
					return m_HgEnumerateProcessMemory(nProcessId, nEProcess, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgEnumerateProcessMemory(nProcessId, nEProcess, ppItp);
			}
		}
		return FALSE;
	}

	BOOL ProcessMemoryZero(SIZE_T nProcessId, SIZE_T nEProcess, SIZE_T Base, SIZE_T Size){
		if (m_pQueryInterface != NULL){
			if (m_HgProcessMemoryZero == NULL){
				m_HgProcessMemoryZero = (pfnHgProcessMemoryZero)m_pQueryInterface(eProcessMemoryZero);
				if (m_HgProcessMemoryZero != NULL){
					return m_HgProcessMemoryZero(nProcessId, nEProcess, Base, Size);
				}else{
					return FALSE;
				}
			}else{
				return m_HgProcessMemoryZero(nProcessId, nEProcess, Base, Size);
			}
		}
		return FALSE;
	}

	BOOL ProcessMemoryFree(SIZE_T nProcessId, SIZE_T nEProcess, SIZE_T Base, SIZE_T Size){
		if (m_pQueryInterface != NULL){
			if (m_HgProcessMemoryFree == NULL){
				m_HgProcessMemoryFree = (pfnHgProcessMemoryFree)m_pQueryInterface(eProcessMemoryFree);
				if (m_HgProcessMemoryFree != NULL){
					return m_HgProcessMemoryFree(nProcessId, nEProcess, Base, Size);
				}else{
					return FALSE;
				}
			}else{
				return m_HgProcessMemoryFree(nProcessId, nEProcess, Base, Size);
			}
		}
		return FALSE;
	}

	BOOL ProcessMemoryChangeProtection(SIZE_T nProcessId, SIZE_T nEProcess, SIZE_T Base, SIZE_T Size, SIZE_T NewProtection){
		if (m_pQueryInterface != NULL){
			if (m_HgProcessMemoryChangeProtection == NULL){
				m_HgProcessMemoryChangeProtection = (pfnHgProcessMemoryChangeProtection)m_pQueryInterface(eProcessMemoryChangeProtection);
				if (m_HgProcessMemoryChangeProtection != NULL){
					return m_HgProcessMemoryChangeProtection(nProcessId, nEProcess, Base, Size, NewProtection);
				}else{
					return FALSE;
				}
			}else{
				return m_HgProcessMemoryChangeProtection(nProcessId, nEProcess, Base, Size, NewProtection);
			}
		}
		return FALSE;
	}

	BOOL EnumerateDriver(PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgEnumerateDriver == NULL){
				m_HgEnumerateDriver = (pfnHgEnumerateDriver)m_pQueryInterface(eEnumerateDriver);
				if (m_HgEnumerateDriver != NULL){
					return m_HgEnumerateDriver(ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgEnumerateDriver(ppItp);
			}
		}
		return FALSE;
	}

	BOOL RegistryEnumerateKey(const PWCHAR Path, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgRegistryEnumerateKey == NULL){
				m_HgRegistryEnumerateKey = (pfnHgRegistryEnumerateKey)m_pQueryInterface(eRegistryEnumerateKey);
				if (m_HgRegistryEnumerateKey != NULL){
					return m_HgRegistryEnumerateKey(Path, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgRegistryEnumerateKey(Path, ppItp);
			}
		}
		return FALSE;
	}

	BOOL RegistryEnumerateValueKey(const WCHAR* Path, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgRegistryEnumerateValueKey == NULL){
				m_HgRegistryEnumerateValueKey = (pfnHgRegistryEnumerateValueKey)m_pQueryInterface(eRegistryEnumerateValueKey);
				if (m_HgRegistryEnumerateValueKey != NULL){
					return m_HgRegistryEnumerateValueKey(const_cast<PWCHAR>(Path), ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgRegistryEnumerateValueKey(const_cast<PWCHAR>(Path), ppItp);
			}
		}
		return FALSE;
	}

	BOOL EnumerateKernelNotify(PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgKernelEnumerateNotify == NULL){
				m_HgKernelEnumerateNotify = (pfnHgKernelEnumerateNotify)m_pQueryInterface(eKernelEnumerateNotify);
				if (m_HgKernelEnumerateNotify != NULL){
					return m_HgKernelEnumerateNotify(ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgKernelEnumerateNotify(ppItp);
			}
		}
		return FALSE;
	}

	BOOL HookSSDT(PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgHookSSDT == NULL){
				m_HgHookSSDT = (pfnHgHookSSDT)m_pQueryInterface(eHookEnumerateSSDT);
				if (m_HgHookSSDT != NULL){
					return m_HgHookSSDT(ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgHookSSDT(ppItp);
			}
		}
		return FALSE;
	}

	BOOL HookShadowSSDT(PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgHookShadowSSDT == NULL){
				m_HgHookShadowSSDT = (pfnHgHookShadowSSDT)m_pQueryInterface(eHookEnumerateShadowSSDT);
				if (m_HgHookShadowSSDT != NULL){
					return m_HgHookShadowSSDT(ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgHookShadowSSDT(ppItp);
			}
		}
		return FALSE;
	}

	BOOL ReadProcessMemoryAux(SIZE_T Base, SIZE_T Size, SIZE_T nEProcess, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgReadProcessMemoryAux == NULL){
				m_HgReadProcessMemoryAux = (pfnHgReadProcessMemoryAux)m_pQueryInterface(eReadProcessMemoryAux);
				if (m_HgReadProcessMemoryAux != NULL){
					return m_HgReadProcessMemoryAux(Base, Size, nEProcess, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgReadProcessMemoryAux(Base, Size, nEProcess, ppItp);
			}
		}
		return FALSE;
	}

	BOOL EnumerateProcessHandle(SIZE_T nProcessId, SIZE_T nEProcess, PITP* ppItp){
		if (m_pQueryInterface != NULL){
			if (m_HgEnumerateProcessHandle == NULL){
				m_HgEnumerateProcessHandle = (pfnHgEnumerateProcessHandle)m_pQueryInterface(eProcessHandleEnum);
				if (m_HgEnumerateProcessHandle != NULL){
					return m_HgEnumerateProcessHandle(nProcessId, nEProcess, ppItp);
				}else{
					return FALSE;
				}
			}else{
				return m_HgEnumerateProcessHandle(nProcessId, nEProcess, ppItp);
			}
		}
		return FALSE;
	}

	BOOL CloseProcessHandle(SIZE_T nProcessId, SIZE_T nEProcess, SIZE_T Handle, SIZE_T HandleObject, SIZE_T IsForceClose){
		if (m_pQueryInterface != NULL){
			if (m_HgCloseProcessHandle == NULL){
				m_HgCloseProcessHandle = (pfnHgCloseProcessHandle)m_pQueryInterface(eProcessHandleClose);
				if (m_HgCloseProcessHandle != NULL){
					return m_HgCloseProcessHandle(nProcessId, nEProcess, Handle, HandleObject, IsForceClose);
				}else{
					return FALSE;
				}
			}else{
				return m_HgCloseProcessHandle(nProcessId, nEProcess, Handle, HandleObject, IsForceClose);
			}
		}
		return FALSE;
	}
};

#endif