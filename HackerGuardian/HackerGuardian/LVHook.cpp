

#include "stdafx.h"
#include "LVHook.h"

CTabView* CLVHook::m_top_tab = NULL;
CTabView* CLVHook::m_bottom_tab = NULL;
CSplitterWindow* CLVHook::m_vertical_splitter = NULL;
CHorSplitterWindow*	CLVHook::m_horizontal_splitter = NULL;
CStatusBarCtrl* CLVHook::m_pStatus = NULL;
CLVHook* CLVHook::_instance = NULL;
WCHAR** CLVHook::m_ShadowSDTName = NULL;