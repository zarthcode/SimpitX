// Newloader DLL

////////////////////////////////////////////////////////////////////////////////////////////
// loader: command-line interface dll injector
// Copyright (C) 2011  Wadim Egorov
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _NEWLOADER_H
#define _NEWLOADER_H

#ifdef UNICODE
#undef UNICODE
#endif

#if defined(_WIN64)
#pragma message("_WIN64 defined")
#elif defined(_WIN32)
#pragma message("_WIN32 defined")
#endif

#include <Windows.h>
#include <string>



#endif // _NEWLOADER_H




#define EXTERN_DLL_EXPORT extern "C" __declspec(dllexport)

// Returns true if the process id is that of a 64 bit process.
EXTERN_DLL_EXPORT bool __stdcall Is64Bit(DWORD dwProcessId);

// Returns the last error message.
EXTERN_DLL_EXPORT std::wstring __stdcall GetErrorMsgW(void);

// Inject into process id (library path)
EXTERN_DLL_EXPORT bool __stdcall InjectIntoProcess( DWORD dwProcessId, LPCWSTR lpLibPath );

// Inject into process (library address)
EXTERN_DLL_EXPORT bool __stdcall InjectIntoProcessByLibAddr( DWORD dwProcessId, void* LibraryAddress);

// Map the PE file into the remote address space, without LoadLibrary
EXTERN_DLL_EXPORT bool __stdcall InjectIntoProcessMM( DWORD dwProcessId, LPCWSTR lpLibPath );

// Ejects the injection 
EXTERN_DLL_EXPORT bool __stdcall EjectFromProcess( DWORD dwProcessId, LPCWSTR lpLibPath );

// Launches the process and injects
EXTERN_DLL_EXPORT bool __stdcall LaunchProcessAndInject( LPCWSTR lpLibPath, LPCWSTR lpProcPath, LPCWSTR lpProcArgs, bool bWaitForInputIdle);

// Launches the process and Ejects
EXTERN_DLL_EXPORT bool __stdcall LaunchProcessAndEject( LPCWSTR lpLibPath, LPCWSTR lpProcPath, LPCWSTR lpProcArgs, bool bWaitForInputIdle);
