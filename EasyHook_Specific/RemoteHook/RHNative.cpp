/*
Copyright (c) 2008 by Christoph Husse, SecurityRevolutions e.K.

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
associated documentation files (the "Software"), to deal in the Software without restriction, 
including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, 
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial 
portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT 
LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. 
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE 
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

Visit http://www.codeplex.com/easyhook for more information.
*/

#pragma managed(off)
#define _NATIVE_BIND_
#include "RemoteHook.h"
#include <windows.h>
#include <stdio.h>
#include <MSCorEE.h>

#define UNMANAGED_ERROR(code, attr) {ErrorCode = ((code) & 0xFF) | 0xF0000000; goto ABORT_ERROR;}

int __stdcall HookCompleteInjectionInternal(LPREMOTE_INFO InInfo);

extern "C" __declspec(dllexport) int __stdcall HookCompleteInjection(LPREMOTE_INFO InInfo)
{
	SetLastError(0);

	/*
		To increase stability we will now update all symbols with the
		real local ones...
	*/
	HMODULE hMod = GetModuleHandleA("kernel32.dll");

	InInfo->LoadLibraryW = GetProcAddress(hMod, "LoadLibraryW");
	InInfo->FreeLibrary = GetProcAddress(hMod, "FreeLibrary");
	InInfo->GetProcAddress = GetProcAddress(hMod, "GetProcAddress");
	InInfo->VirtualFree = GetProcAddress(hMod, "VirtualFree");
	InInfo->VirtualProtect = GetProcAddress(hMod, "VirtualProtect");
	InInfo->ExitThread = GetProcAddress(hMod, "ExitThread");
	InInfo->GetLastError = GetProcAddress(hMod, "GetLastError");

	// create helper thread to prevent unknown side effects with NtCreateThreadEx()
	HANDLE hHelper = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)HookCompleteInjectionInternal, InInfo, 0, NULL);

	WaitForSingleObject(hHelper, INFINITE);

	CloseHandle(hHelper);

	return 0;
}

int __stdcall HookCompleteInjectionInternal(LPREMOTE_INFO InInfo)
{
	ICLRRuntimeHost*		ClrHost = NULL;
	wchar_t*				PATH = NULL;
	DWORD					ErrorCode = 0;

	// Make directory of user library path available to current process...
	// This is to find dependencies without copying them into a global
	// directory which might cause trouble.

	DWORD DirSize = wcslen(InInfo->PATH);
	DWORD EnvSize = GetEnvironmentVariableW(L"PATH", NULL, 0) + DirSize;

	if((PATH = (wchar_t*)malloc(EnvSize * 2 + 10)) == NULL)
		UNMANAGED_ERROR(1, GetLastError());

	GetEnvironmentVariableW(L"PATH", PATH, EnvSize);

	// add library path to environment variable
	memmove(PATH + DirSize, PATH, (EnvSize - DirSize) * 2);
	memcpy(PATH, InInfo->PATH, DirSize * 2);

	if(!SetEnvironmentVariableW(L"PATH", PATH))
		UNMANAGED_ERROR(2, GetLastError());

	// load NET-Runtime and execute user defined method
	DWORD hRes = CorBindToRuntime(NULL, NULL, CLSID_CLRRuntimeHost, IID_ICLRRuntimeHost, (void**)&ClrHost);
	DWORD RetVal;

	if(!SUCCEEDED(hRes))
		UNMANAGED_ERROR(5, hRes);

	ClrHost->Start();

	/*
		Test library code.
		This is because if once we have set the remote signal, there is no way to
		notify the host about errors. If the following call succeeds, then it will
		also do so some lines later... If not, then we are still able to report an error.
	*/
	if(!SUCCEEDED(hRes = ClrHost->ExecuteInDefaultAppDomain(InInfo->ManagedPath, L"EasyHook.InjectionLoader", L"Main", InInfo->ParamString, &RetVal)))
		UNMANAGED_ERROR(7, hRes);

	switch(RetVal)
	{
	case 0: break;
	case -2: UNMANAGED_ERROR(9, RetVal);
	default:
		UNMANAGED_ERROR(6, RetVal);
	}

	// set and close event
	if(!SetEvent(InInfo->hRemoteSignal))
		UNMANAGED_ERROR(8, GetLastError());

	CloseHandle(InInfo->hRemoteSignal);

	InInfo->hRemoteSignal = NULL;

	// execute library code (no way for error reporting, so we dont need to check)
	ClrHost->ExecuteInDefaultAppDomain(InInfo->ManagedPath, L"EasyHook.InjectionLoader", L"Main", InInfo->ParamString, &RetVal);

ABORT_ERROR:

	// release resources
	if(PATH != NULL)
		free(PATH);

	if(InInfo->hRemoteSignal != NULL)
		CloseHandle(InInfo->hRemoteSignal);

	if(ClrHost != NULL)
		ClrHost->Release();

	return ErrorCode;
}

/*/////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////// GetInjectionSize
///////////////////////////////////////////////////////////////////////////////////////

Dynamically retrieves the size of the trampoline method.
*/
static DWORD ___InjectionSize = 0;

#ifdef _M_X64
	extern "C" void Injection_ASM_x64();
#else
	extern "C" void Injection_ASM_x86();
#endif

BYTE* GetInjectionPtr()
{
#ifdef _M_X64
	BYTE* Ptr = (BYTE*)Injection_ASM_x64;
#else
	BYTE* Ptr = (BYTE*)Injection_ASM_x86;
#endif

// bypass possible VS2008 debug jump table
	if(*Ptr == 0xE9)
		Ptr += *((int*)(Ptr + 1)) + 5;

	return Ptr;
}

DWORD GetInjectionSize()
{
	if(___InjectionSize != 0)
		return ___InjectionSize;
	
	// search for signature
	BYTE* Ptr = GetInjectionPtr();
	BYTE* BasePtr = Ptr;

	for(int i = 0; i < 2000 /* some always large enough value*/; i++)
	{
		int Signature = *((int*)Ptr);

		if(Signature == 0x12345678)	
		{
			___InjectionSize = (int)(Ptr - BasePtr);

			return ___InjectionSize;
		}

		Ptr++;
	}

	return -1;
}

