

#include "stdafx.h"
#include "LVDriver.h"

CTabView* CLVDriver::m_top_tab = NULL;
CTabView* CLVDriver::m_bottom_tab = NULL;
CSplitterWindow* CLVDriver::m_vertical_splitter = NULL;
CHorSplitterWindow*	CLVDriver::m_horizontal_splitter = NULL;
CStatusBarCtrl* CLVDriver::m_pStatus = NULL;
CLVDriver* CLVDriver::_instance = NULL;