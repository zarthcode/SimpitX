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
#ifndef _STDAFX_H_
#define _STDAFX_H_

	#pragma once
	#pragma warning (disable:4677)
	#pragma warning (disable:4800) // forcing value to bool 'true' or 'false' (performance warning)	
	#pragma warning (disable:4201) // nonstandard extension used : nameless struct/union	

	#define WINVER 0x0500
	#define _WIN32_WINNT 0x0500

	#include <windows.h>
	#include <winternl.h>
	#include <winnt.h>
	#include <sddl.h>
	#include <dbgeng.h>
	#include <stdio.h>
	#include <crtdbg.h>

	#pragma comment(lib, "Advapi32.lib")

	using namespace System;
	using namespace System::IO;
	using namespace System::Threading;
	using namespace System::Diagnostics;
	using namespace System::Runtime::ConstrainedExecution;
	using namespace System::Runtime::InteropServices;
	using namespace System::Collections::Generic;
	using namespace System::Security::Cryptography;
	using namespace System::Security;
	using namespace System::Security::Principal;
	using namespace System::Security::AccessControl;
	using namespace System::Reflection;
	using namespace System::Runtime::Remoting;
	using namespace System::Runtime::Remoting::Channels;
	using namespace System::Runtime::Remoting::Channels::Ipc;
	using namespace System::Text;
	using namespace Microsoft::Win32::SafeHandles;
	using namespace System;
	using namespace System::Runtime::CompilerServices;
	using namespace System::Security::Permissions;
	using namespace System::Runtime::Serialization;
	using namespace System::Runtime::Serialization::Formatters;
	using namespace System::Runtime::Serialization::Formatters::Binary;


	[assembly:AssemblyTitleAttribute("EasyHookSpecific")];
	[assembly:AssemblyDescriptionAttribute("A platform dependend library providing the native EasyHook interface.")];
	[assembly:AssemblyConfigurationAttribute("")];
	[assembly:AssemblyCompanyAttribute("SecurityRevolutions")];
	[assembly:AssemblyProductAttribute("EasyHookSpecific")];
	[assembly:AssemblyCopyrightAttribute("Copyright (c) 2008 by Christoph Husse")];
	[assembly:AssemblyTrademarkAttribute("EasyHook")];
	[assembly:AssemblyCultureAttribute("")];
	[assembly:AssemblyVersionAttribute("2.0.0.0")];
	[assembly:ComVisible(false)];
	[assembly:CLSCompliantAttribute(true)];
	[assembly:SecurityPermission(SecurityAction::RequestMinimum, UnmanagedCode = true)];


	#include "Utilities.h"

	namespace EasyHook
	{
		// predeclarations
		ref class Debugger_Specific;
		ref class LocalHook_Specific;
		ref class HookRuntimeInfo_Specific;
		ref class HookAccessControl;
	};

	#define MAX_ACE_COUNT			128
	#define MAX_THREAD_COUNT		1024
	#define MAX_HOOK_COUNT			1024

	#include "FixedList.h"
	#include "Debugger.h"

	#include "LocalHook\HookAccessControl.h"
	#include "LocalHook\HookRuntimeInfo.h"
	#include "LocalHook\LocalHook.h"
	#include "RemoteHook\RemoteHook.h"

#endif