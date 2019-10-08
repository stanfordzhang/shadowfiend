#ifndef __LDASM_H__
#define __LDASM_H__

unsigned long 
__fastcall 
SizeOfCode(
	void *Code, 
	unsigned char **pOpcode
	);

unsigned long 
__fastcall 
SizeOfProc(
	void *Proc
	);

char 
__fastcall 
IsRelativeCmd(
	unsigned char *pOpcode
	);

#endif