/*

64 Bit Detours

For: http://www.gamedeception.net

By: Matthew L (Azorbix)
	P47R!CK
	Loopnz

*/

#include "detours.h"

FARPROC Detour64(FARPROC source, FARPROC destination, const size_t len)
{
	if( len < 14 ) {
		return NULL;
	}

	BYTE *pSrc = (BYTE*)source;
	
	BYTE *pTramp = new BYTE[len+14];

	DWORD protectFlags;

	VirtualProtect(pSrc, len, PAGE_READWRITE, &protectFlags);

	memcpy(pTramp, pSrc, len);

	// JMP [0x00+RIP]; (RIP = &destination)
	pSrc[0] = 0xFF;
	pSrc[1] = 0x25;
	pSrc[2] = pSrc[3] = pSrc[4] = pSrc[5] = 0x00;
	*(FARPROC*)&pSrc[6] = destination;

	// INT 3;
	for( int i=14; i < len; i++ ) {
		pSrc[i] = 0xCC;
	}

	// same, but now for trampoline
	pTramp[len] = 0xFF;
	pTramp[len+1] = 0x25;
	pTramp[len+2] = pTramp[len+3] = pTramp[len+4] = pTramp[len+5] = 0x00;
	*(FARPROC*)&pTramp[len+6] = (FARPROC)&pSrc[len];

	VirtualProtect(source, len, protectFlags, &protectFlags);

	VirtualProtect(pTramp, len+14, PAGE_EXECUTE_READWRITE, &protectFlags);

	return (FARPROC)pTramp;
}

void RemoveDetour64(FARPROC source, FARPROC trampoline, const size_t len)
{
	DWORD protectFlags;

	// remove detour from original function
	VirtualProtect(source, len, PAGE_READWRITE, &protectFlags);
	memcpy(source, trampoline, len);
	VirtualProtect(source, len, protectFlags, &protectFlags);

	// make trampoline jmp to source
	BYTE *pTramp = (BYTE*)trampoline;	

	VirtualProtect(pTramp, 14, PAGE_READWRITE, &protectFlags);

	pTramp[0] = 0xFF;
	pTramp[1] = 0x25;
	pTramp[2] = pTramp[3] = pTramp[4] = pTramp[5] = 0x00;
	*(FARPROC*)&pTramp[6] = source;

	VirtualProtect(pTramp, 14, protectFlags, &protectFlags);
}


void *Detour32(BYTE *src, const BYTE *dst, const int len)
{
	BYTE *jmp = (BYTE*)malloc(len+5);
	DWORD dwback;

	VirtualProtect(jmp, len+5, PAGE_EXECUTE_READWRITE, &dwback);	// Needed to work on Win 7?

	VirtualProtect(src, len, PAGE_READWRITE, &dwback);

	memcpy(jmp, src, len);	jmp += len;

	jmp[0] = 0xE9;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - 5;

	src[0] = 0xE9;
	*(DWORD*)(src+1) = (DWORD)(dst - src) - 5;

	VirtualProtect(src, len, dwback, &dwback);

	return (jmp-len);
}

bool RemoveDetour32(BYTE *src, BYTE *restore, const int len)
{
	DWORD dwback;

	if(!VirtualProtect(src, len, PAGE_READWRITE, &dwback))	{ return false; }
	if(!memcpy(src, restore, len))							{ return false; }

	restore[0] = 0xE9;
	*(DWORD*)(restore+1) = (DWORD)(src - restore) - 5;

	if(!VirtualProtect(src, len, dwback, &dwback))			{ return false; }

	return true;
}	
