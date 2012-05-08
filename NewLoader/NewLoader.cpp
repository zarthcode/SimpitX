
#include "NewLoader.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>

#include "misc.h"
#include "findproc.h"
#include "injector_helper.h"
#include "generic_injector.h"
#include "manualmap.h"

std::stringstream error_str;

bool __stdcall Is64Bit(DWORD dwProcessId)
{
	if (IsProcess64(dwProcessId))
	{
		return true;
	}

	return false;
}

std::wstring __stdcall GetErrorMsgW(void)
{
	// Return a copy of the error string
	return widen(error_str.str());
}

bool __stdcall InjectIntoProcess( DWORD dwProcessId, LPCWSTR lpLibPath )
{
	// clear the error string
	error_str.str("");

	try
	{
		if(InjectLibraryW(dwProcessId, lpLibPath) != TRUE)
		{
			// Create an exception and throw it
			error_str << "Injection failed (No exception thrown).";
			return false;
		}
	}
	catch(std::exception &e)
	{
		// Store the error message
		error_str << "NewLoader::InjectIntoProcess(" << dwProcessId << ", " << lpLibPath << ") threw an exception:\n" << e.what();
		return false;
	}

	return true;
}

// Map the PE file into the remote address space, without LoadLibrary
bool __stdcall InjectIntoProcessMM( DWORD dwProcessId, LPCWSTR lpLibPath )
{
	// clear the error string
	error_str.str("");

	error_str << "Unimplemented.";
	return false;
}

// Ejects the injection 
bool __stdcall EjectFromProcess( DWORD dwProcessId, LPCWSTR lpLibPath )
{
	// clear the error string
	error_str.str("");

	error_str << "Unimplemented.";
	return false;
}

// Launches the process and injects
bool __stdcall LaunchProcessAndInject( LPCWSTR lpLibPath, LPCWSTR lpProcPath, LPCWSTR lpProcArgs, bool bWaitForInputIdle)
{
	// clear the error string
	error_str.str("");

	error_str << "Unimplemented.";
	return false;
}

// Launches the process and Ejects
bool __stdcall LaunchProcessAndEject( LPCWSTR lpLibPath, LPCWSTR lpProcPath, LPCWSTR lpProcArgs, bool bWaitForInputIdle)
{
	// clear the error string
	error_str.str("");

	error_str << "Unimplemented.";
	return false;
}


