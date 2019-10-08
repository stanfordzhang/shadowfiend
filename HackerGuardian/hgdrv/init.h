#ifndef __INIT_H__
#define __INIT_H__

#include "stdhg.h"
#include "hgexport.h"

/************************************************************************/
/* Function Define                                                                     */
/************************************************************************/
BOOL
InitializeEnvironment(
	PDRIVER_OBJECT pDrvObj
	);

/************************************************************************/
/* Obtain Windows Version                                                                     */
/************************************************************************/
WIN_VERSION
GetWindowsVersion(
);

/************************************************************************/
/* Initialize map table                                                 */
/************************************************************************/
VOID
InitMapTable(
	);

VOID
GetNtoskrnlLdrDataTableEntry(
	);

VOID
InitWindowsApi(
	);

VOID
InitSSDT(
	);

VOID
GetCommonDriverSizeAndBase(
	);

#endif