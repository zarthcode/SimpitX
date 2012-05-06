#define WIN32_LEAN_AND_MEAN

#ifndef _MAIN_H
#define _MAIN_H
#include "easyhook.h"

#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

EXTERN_DLL_EXPORT void __stdcall NativeInjectionEntryPoint(REMOTE_ENTRY_INFO* InRemoteInfo);

char *GetDirectoryFile(char *filename);

void __cdecl add_log (const char * fmt, ...);

#endif