

#include "stdafx.h"
#include "LVKernel.h"

CTabView* CLVKernel::m_top_tab = NULL;
CTabView* CLVKernel::m_bottom_tab = NULL;
CSplitterWindow* CLVKernel::m_vertical_splitter = NULL;
CHorSplitterWindow*	CLVKernel::m_horizontal_splitter = NULL;
CStatusBarCtrl* CLVKernel::m_pStatus = NULL;
CLVKernel* CLVKernel::_instance = NULL;