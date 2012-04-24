#pragma once

#include <windows.h>


FARPROC Detour64(FARPROC source, FARPROC destination, const size_t len);
void RemoveDetour64(FARPROC source, FARPROC trampoline, const size_t len);

void* Detour32(BYTE *src, const BYTE *dst, const int len);
bool RemoveDetour32(BYTE *src, BYTE *restore, const int len);


#if defined(_WIN64)

#define DETOUR(src, dest, len) Detour64((FARPROC)src, (FARPROC)dest, len)
#define REMOVE_DETOUR(src, tramp, len) RemoveDetour64((FARPROC)src, (FARPROC)tramp, len)

#elif defined(_WIN32)

#define DETOUR(src, dest, len) Detour32((BYTE*)src, (BYTE*)dest, len)
#define REMOVE_DETOUR(src, tramp, len) RemoveDetour32((BYTE*)src, (BYTE*)tramp, len)

#endif