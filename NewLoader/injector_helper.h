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
#ifndef _INJECTOR_HELPER_H
#define _INJECTOR_HELPER_H

#include <stdio.h>

#include <Windows.h>
#include <Psapi.h>

#pragma comment(lib, "Psapi.lib")

#include "misc.h"

////////////////////////////////////////////////////////////////////////////////////////////
// GetRemoteProcAddress - Get address of export in remote process
// @param:
//   hProcess - target process handle
//   hRemoteModule - a handle to the remote DLL module that contains the function
//   lpProcName - the function name
// @return:
//   function succeeds: address of the exported function in remote process
//   function fails: 0
////////////////////////////////////////////////////////////////////////////////////////////
FARPROC
GetRemoteProcAddress(
	HANDLE hProcess,
	HMODULE hRemoteModule,
	LPCSTR lpProcName
	);

////////////////////////////////////////////////////////////////////////////////////////////
// EnablePrivilegeW
////////////////////////////////////////////////////////////////////////////////////////////
BOOL
EnablePrivilegeW(
	LPCWSTR	lpPrivilegeName,
	BOOL bEnable
	);

////////////////////////////////////////////////////////////////////////////////////////////
// SuspendResumeProcess
////////////////////////////////////////////////////////////////////////////////////////////
BOOL
SuspendResumeProcess(
	DWORD dwProcessId,
	BOOL bResumeProcess
	);

////////////////////////////////////////////////////////////////////////////////////////////
// HideThreadFromDebugger
////////////////////////////////////////////////////////////////////////////////////////////
BOOL
HideThreadFromDebugger(
	DWORD dwThreadId
	);

////////////////////////////////////////////////////////////////////////////////////////////
// GetFileNameNtW
////////////////////////////////////////////////////////////////////////////////////////////
BOOL
GetFileNameNtW(
	LPCWSTR lpFileName,
	LPWSTR lpFileNameNt,
	DWORD nSize
	);

////////////////////////////////////////////////////////////////////////////////////////////
// ModuleInjectedW
// @param:
//   hProcess - handle to process
//   lpLibPathNt - full native path to library
// @return:
//   function succeeds: module handle
//   function fails: 0
////////////////////////////////////////////////////////////////////////////////////////////
LPVOID
ModuleInjectedW(
	HANDLE hProcess,
	LPCWSTR lpLibPathNt
	);

////////////////////////////////////////////////////////////////////////////////////////////
// ListModules
////////////////////////////////////////////////////////////////////////////////////////////
VOID
ListModules(
	DWORD dwProcessId
	);

////////////////////////////////////////////////////////////////////////////////////////////
// MyGetSystemInfo
////////////////////////////////////////////////////////////////////////////////////////////
void
__stdcall
MyGetSystemInfo(
	LPSYSTEM_INFO lpSystemInfo
	);

////////////////////////////////////////////////////////////////////////////////////////////
// IsProcess64
// @param:
//   dwProcessId - process id
// @return:
//   function succeeds: 0 - x86 process / 1 - x64 process
//   function fails: -1
////////////////////////////////////////////////////////////////////////////////////////////
INT
IsProcess64(
	DWORD dwProcessId
	);

////////////////////////////////////////////////////////////////////////////////////////////
// Structures and definitions undocumented or included in the NTDDK.
////////////////////////////////////////////////////////////////////////////////////////////
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

typedef enum _THREAD_INFORMATION_CLASS
{
	ThreadBasicInformation,
	ThreadTimes,
	ThreadPriority,
	ThreadBasePriority,
	ThreadAffinityMask,
	ThreadImpersonationToken,
	ThreadDescriptorTableEntry,
	ThreadEnableAlignmentFaultFixup,
	ThreadEventPair,
	ThreadQuerySetWin32StartAddress,
	ThreadZeroTlsCell,
	ThreadPerformanceCount,
	ThreadAmILastThread,
	ThreadIdealProcessor,
	ThreadPriorityBoost,
	ThreadSetTlsArrayAddress,
	ThreadIsIoPending,
	ThreadHideFromDebugger
} THREAD_INFORMATION_CLASS, *PTHREAD_INFORMATION_CLASS;

#endif // _INJECTOR_HELPER_H