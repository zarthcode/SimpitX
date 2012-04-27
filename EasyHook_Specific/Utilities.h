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

#pragma once

#include "stdafx.h"

namespace EasyHook 
{
	#define LOCK(Class)				Monitor::Enter(Class); try
	#define UNLOCK(Class)			finally{Monitor::Exit(Class);}

	#define BUGCHECK(Exp, Message, ...)\
		if(!(Exp))\
		{\
		try{ THROW(System::ApplicationException, Message, __VA_ARGS__); }\
			finally{ DebugBreak();System::Diagnostics::Process::GetCurrentProcess()->Kill(); }\
		}

	#define UNMANAGED_ASSERT(Exp)	UNMANAGED_BUGCHECK(Exp, "Assertion failed.");
	#define UNMANAGED_BUGCHECK(Exp, Message, ...)\
		if(!(Exp))\
		{\
			printf(Message, __VA_ARGS__);\
			MessageBoxA(NULL, Message, "Bugcheck...", MB_OK | MB_ICONERROR);\
			DebugBreak();\
			ExitProcess(-1);\
		}

	#define THROW(ExceptionType, Msg, ...)\
	{\
		String^ __Msg = String::Format(Msg + "(Win32-Error: " + GetLastError().ToString() + ")", __VA_ARGS__);\
		if(ExceptionType::typeid->GUID == ApplicationException::typeid->GUID)\
			Config::PrintError(__Msg + "\r\n" + "Stack trace:\r\n" + (gcnew StackTrace())->ToString());\
		else\
			Config::PrintWarning(__Msg + "\r\n" + "Stack trace:\r\n" + (gcnew StackTrace())->ToString());\
		throw gcnew ExceptionType(__Msg);\
	}

	#define COMMENT(Msg, ...)\
		{\
			String^ __Msg = String::Format(Msg, __VA_ARGS__);\
			__Msg += "\r\n" + "Stack trace:\r\n" + (gcnew StackTrace())->ToString();\
			Config::PrintComment(__Msg);\
		}


};