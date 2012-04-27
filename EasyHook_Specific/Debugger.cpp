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
#include "stdafx.h"

namespace EasyHook
{
	Debugger_Specific::UNIT::UNIT()
	{
		HRESULT					hRes = S_OK;
		IDebugClient*			hClient = NULL;
		IDebugControl*			hControl = NULL;
		IDebugSymbols*			hSymbols = NULL;
		IDebugDataSpaces2*		hDataSpaces = NULL;


		try
		{
			m_IsEnabled = true;

			if(hDbgEng == NULL)
			{
				/*
					The first one is usually located in either C:\Windows\System32 or C:\Windows\SysWOW64.
					The other two are expected to be located in the current directory...

					This library ships with a clean installation since Windows XP SP2. So in general there
					will be no need to distribute it with your application! But Windows 2000 SP4 does provide
					the dbghelp.dll only, which is unsuitable for us! If you want to have advanced hooking
					support on Windows 2000, you will have to deploy the "dbgeng.dll" and "dbghelp.dll" of the 
					"Debugging Tools for Windows 32-bit Version". Both shall be copied into the system directory!
				*/
				wchar_t			NativePath[MAX_PATH + 1];
				String^			Path = Environment::SystemDirectory + "\\dbgeng.dll";

				memset(NativePath, 0, sizeof(NativePath));

				Marshal::Copy(Path->ToCharArray(), 0, (IntPtr)NativePath, Math::Min(MAX_PATH, Path->Length));

				if((hDbgEng = LoadLibraryW(NativePath)) == NULL)
					THROW(System::NotSupportedException, "No microsoft debugging engine present.");

				if((_DebugCreate = (PROC_DebugCreate*)::GetProcAddress(hDbgEng, "DebugCreate")) == NULL)
					THROW(System::NotSupportedException, "Unable to find expected debugging method.");
			}

			// initialize microsoft debugging engine
			if(!SUCCEEDED(hRes = _DebugCreate(__uuidof(IDebugClient), (void**)&hClient)))
				THROW(System::NotSupportedException, "Unable to obtain interface for debugging engine. (Code 0x{0:X8})", hRes);

			DebugClient = hClient;

			if(!SUCCEEDED(hRes = DebugClient->QueryInterface(__uuidof(IDebugControl), (void**)&hControl)))
				THROW(System::NotSupportedException, "Unable to obtain interface for debugging control. (Code 0x{0:X8})", hRes);

			DebugControl = hControl;

			if(!SUCCEEDED(hRes = DebugClient->QueryInterface(__uuidof(IDebugSymbols), (void**)&hSymbols)))
				THROW(System::NotSupportedException, "Unable to obtain interface for debugging control. (Code 0x{0:X8})", hRes);

			DebugSymbols = hSymbols;

			// optional...
			DebugClient->QueryInterface(__uuidof(IDebugDataSpaces2), (void**)&hDataSpaces);

			DebugDataSpaces = hDataSpaces;
		}
		catch(...)
		{
			this->~UNIT();
		}

		// initialize native NT-DLL routines
		hNtDll = LoadLibraryA("ntdll.dll");
		hKernel32 = LoadLibraryA("kernel32.dll");

		// try to load newer methods first
		if((ZwGetProcessId = (ZwGetProcessId_PROC*)::GetProcAddress(hKernel32, "GetProcessId")) == NULL)
		{
			if((ZwQueryInformationProcess = (ZwQueryInformationProcess_PROC*)::GetProcAddress(
					hNtDll, "NtQueryInformationProcess")) == NULL)
				THROW(System::ApplicationException, "Unable to access native system utilities.");
		}

		if((ZwGetThreadId = (ZwGetThreadId_PROC*)::GetProcAddress(hKernel32, "GetThreadId")) == NULL)
		{
			if((ZwQueryInformationThread = (ZwQueryInformationThread_PROC*)::GetProcAddress(
					hNtDll, "NtQueryInformationThread")) == NULL)
				THROW(System::ApplicationException, "Unable to access native system utilities.");
		}
	}

	Debugger_Specific::UNIT::!UNIT()
	{
		if(DebugClient != NULL)
		{
			DebugClient->DetachProcesses();
			DebugClient->Release();
		}

		if(DebugControl != NULL)
			DebugControl->Release();

		if(DebugSymbols != NULL)
			DebugSymbols->Release();

		if(DebugDataSpaces != NULL)
			DebugDataSpaces->Release();

		if(hDbgEng != NULL)
			FreeLibrary(hDbgEng);

		if(hNtDll != NULL)
			FreeLibrary(hNtDll);

		if(hKernel32 != NULL)
			FreeLibrary(hKernel32);

		DebugClient = NULL;
		DebugControl = NULL;
		DebugSymbols = NULL;
		DebugDataSpaces = NULL;
		hKernel32 = NULL;
		hNtDll = NULL;
		hDbgEng = NULL;
	}

	void Debugger_Specific::AttachToProcess(Int32 InProcessID, Int32 InTimeoutMillis)
	{
		HRESULT			hRes;


		Lock();

		try
		{
			ForceAvailable();

			if(InTimeoutMillis == Timeout::Infinite)
				InTimeoutMillis = INFINITE;

			if(IsAttached)
			{
				if(AttachedProcess != InProcessID)
					DetachFromProcess();
				else
					return;
			}

			// attach to current process
			if(!SUCCEEDED(hRes = Unit->DebugClient->AttachProcess(0, InProcessID, DEBUG_ATTACH_NONINVASIVE |
					DEBUG_ATTACH_NONINVASIVE_NO_SUSPEND)))
				THROW(System::ApplicationException, "Unable to attach debugger to target process. Check for proper privileges and process exitence. (Code 0x{0:X8})", hRes);

			// wait for completion
			if(!SUCCEEDED(hRes = Unit->DebugControl->WaitForEvent(0, InTimeoutMillis)))
			{
				if(!SUCCEEDED(hRes = Unit->DebugClient->DetachProcesses()))
					THROW(System::ApplicationException, "Unable to detach semi attached process. (Code 0x{0:X8})", hRes);

				THROW(System::ApplicationException, "Unable to wait for debugger. (Code 0x{0:X8})", hRes);
			}

			Unit->AttachedProcess = InProcessID;
		}
		finally
		{
			Unlock();
		}
	}

	void Debugger_Specific::DetachFromProcess()
	{
		HRESULT			hRes;

		Lock();

		try
		{
			ForceAvailable();

			if(!IsAttached)
				return;

			if(!SUCCEEDED(hRes = Unit->DebugClient->DetachProcesses()))
				THROW(System::ApplicationException, "Unable to detach from target process. (Code 0x{0:X8})", hRes);

			Unit->AttachedProcess = 0;
		}
		finally
		{
			Unlock();
		}
	}

	/*
		Will use the debugger to directly read kernel memory. This way we even can 
		read handle data if the handle was not opened with query information access...
		For my surprise this will also work for non-admin users.
	*/
	Debugger::IObjectInfo^ Debugger_Specific::_QueryObjectInformation(IntPtr InHandle)
	{
		Lock();

		try
		{
			ForceAvailable();

			if(Unit->DebugDataSpaces == NULL)
				THROW(System::NotSupportedException, "Not supported on this platform.");

			DEBUG_HANDLE_DATA_BASIC			Basic;
			HRESULT							hRes;
			ULONG							BytesWritten;
			wchar_t*						ObjectName = NULL;
			char*							TypeName = NULL;

			try
			{
				if(IsAttached)
				{
					if(AttachedProcess != GetCurrentProcessId())
						THROW(System::InvalidOperationException, "Debugger_Specific is currently attached to another process. Please call DetachFromProcess() if this was intended.");
				}
				else
					AttachToCurrentProcess();

				// read basic information
				if(!SUCCEEDED(hRes = Unit->DebugDataSpaces->ReadHandleData((ULONG64)InHandle.ToPointer(), DEBUG_HANDLE_DATA_TYPE_BASIC,
						&Basic, sizeof(Basic), &BytesWritten)))
					THROW(System::ArgumentException, "The given handle is invalid.")

				// read type name
				if(!SUCCEEDED(hRes = Unit->DebugDataSpaces->ReadHandleData((ULONG64)InHandle.ToPointer(), DEBUG_HANDLE_DATA_TYPE_TYPE_NAME, 
						NULL, 0, &BytesWritten)))
					THROW(System::ApplicationException, "Unable to query handle's type name.");

				TypeName = (char*)Marshal::AllocCoTaskMem(BytesWritten).ToPointer();

				if(!SUCCEEDED(hRes = Unit->DebugDataSpaces->ReadHandleData((ULONG64)InHandle.ToPointer(), DEBUG_HANDLE_DATA_TYPE_TYPE_NAME,
						TypeName, BytesWritten, &BytesWritten)))
					THROW(System::ApplicationException, "Unable to query handle's type name.");

				// read object name
				if(!SUCCEEDED(hRes = Unit->DebugDataSpaces->ReadHandleData((ULONG64)InHandle.ToPointer(), DEBUG_HANDLE_DATA_TYPE_OBJECT_NAME_WIDE, 
						NULL, 0, &BytesWritten)))
					THROW(System::ApplicationException, "Unable to query handle's object name.");

				ObjectName = (wchar_t*)Marshal::AllocCoTaskMem(BytesWritten).ToPointer();

				if(!SUCCEEDED(hRes = Unit->DebugDataSpaces->ReadHandleData((ULONG64)InHandle.ToPointer(), DEBUG_HANDLE_DATA_TYPE_OBJECT_NAME_WIDE,
							ObjectName, BytesWritten, &BytesWritten)))
						THROW(System::ApplicationException, "Unable to query handle's object name.");

				// create object information
				ObjectInfo^ Result = gcnew ObjectInfo();

				Result->m_Attributes = Basic.Attributes;
				Result->m_GrantedAccess = Basic.GrantedAccess;
				Result->m_HandleCount = Basic.HandleCount;
				Result->m_PointerCount = Basic.PointerCount;
				Result->m_TypeName = Marshal::PtrToStringAnsi((IntPtr)TypeName);
				Result->m_ObjectName= Marshal::PtrToStringUni((IntPtr)ObjectName);

				try
				{
					Result->m_Type = (ObjectType)Enum::Parse(ObjectType::typeid, Result->m_TypeName);
				}
				catch(...)
				{
					Result->m_Type = ObjectType::Unknown;
				}

				return Result;
			}
			finally
			{
				if(TypeName != NULL)
					Marshal::FreeCoTaskMem((IntPtr)TypeName);

				if(ObjectName != NULL)
					Marshal::FreeCoTaskMem((IntPtr)ObjectName);
			}
		}
		finally
		{
			Unlock();
		}
	}

	/*
		Retrieves the underlying thread ID for the given handle.
		The handle shall have THREAD_QUERY_INFORMATION access...
	*/
	Int32 Debugger_Specific::_GetThreadIdByHandle(IntPtr InThreadHandle)
	{
		Int32						Result = 0;
		HRESULT						hRes = 0;
		THREAD_BASIC_INFORMATION	ThreadInfo;
		ULONG						Ret;

		try
		{
	#ifndef _M_X64
			if(Unit->ZwQueryInformationThread != NULL)
			{
				// use deprecated API
				if(!SUCCEEDED(hRes = Unit->ZwQueryInformationThread((HANDLE)InThreadHandle.ToPointer(), (THREADINFOCLASS)0, 
						&ThreadInfo, sizeof(ThreadInfo), &Ret)))
					THROW(System::ArgumentException, "Unable to query thread ID. This usually indicates an invalid handle or the handle was not opened with THREAD_QUERY_INFORMATION access.");

				return ThreadInfo.ClientId.UniqueThread;
			}
	#endif

			if(Unit->ZwGetThreadId == NULL)
				THROW(System::NotSupportedException, "Not supported on this platform.");

			// use new support API
			if((Result = Unit->ZwGetThreadId((HANDLE)InThreadHandle.ToPointer())) == 0)
				THROW(System::ArgumentException, "Unable to query thread ID. This usually indicates an invalid handle or the handle was not opened with THREAD_QUERY_INFORMATION access.");

			return Result;
		}
		catch(ArgumentException^ e)
		{
			if(!_IsEnabled || !_IsAvailable)
				throw e;

			// try to get extended error information
			try
			{
				Debugger::IObjectInfo^ Info = _QueryObjectInformation(InThreadHandle);

				if(Info->Type != ObjectType::Thread)
					THROW(System::InvalidCastException, "The given handle is valid but does not map to a thread; it refers to a \"" + Info->TypeName + "\".");
			}
			catch(ArgumentException^ arg)
			{
				throw arg;
			}
			catch(...)
			{
				throw e;
			}

			THROW(System::AccessViolationException, "The handle seems to be valid but was probably not opened with THREAD_QUERY_INFORMATION access.");
		}
	}

	/*
		Retrieves the underlying process ID for the given handle.
		The handle shall have PROCESS_QUERY_INFORMATION access...
	*/
	Int32 Debugger_Specific::_GetProcessIdByHandle(IntPtr InProcessHandle)
	{
		try
		{
			PROCESS_BASIC_INFORMATION ProcInfo;

			if(Unit->ZwQueryInformationProcess != NULL)
			{
				// use deprecated API
				NTSTATUS hRes;
			
				if(!SUCCEEDED(hRes = Unit->ZwQueryInformationProcess((HANDLE)InProcessHandle.ToPointer(), ProcessBasicInformation, 
						&ProcInfo, sizeof(ProcInfo), NULL)))
					THROW(System::ArgumentException, "Unable to query process ID. This usually indicates an invalid handle or the handle was not opened with PROCESS_QUERY_INFORMATION access.");

				return (Int32)ProcInfo.UniqueProcessId;
			}
			else
			{
				Int32 Result;

				if(Unit->ZwGetProcessId == NULL)
					THROW(System::NotSupportedException, "Not supported on this platform.");

				// use new support API
				if((Result = Unit->ZwGetProcessId((HANDLE)InProcessHandle.ToPointer())) == 0)
					THROW(System::ArgumentException, "Unable to query process ID. This usually indicates an invalid handle or the handle was not opened with PROCESS_QUERY_INFORMATION access.");

				return Result;
			}
		}
		catch(ArgumentException^ e)
		{
			if(!_IsEnabled || !_IsAvailable)
				throw e;

			// try to get extended error information
			try
			{
				Debugger::IObjectInfo^ Info = _QueryObjectInformation(InProcessHandle);

				if(Info->Type != ObjectType::Process)
					THROW(System::InvalidCastException, "The given handle is valid but does not map to a process; it refers to a \"" + Info->TypeName + "\".");
			}
			catch(ArgumentException^ arg)
			{
				throw arg;
			}
			catch(...)
			{
				throw e;
			}

			THROW(System::AccessViolationException, "The handle seems to be valid but was probably not opened with PROCESS_QUERY_INFORMATION access.");
		}
	}

	String^ Debugger_Specific::_Disassemble(
		System::IntPtr InCodePtr, 
		Int32 InMaxByteCount,
		Int32 InLineIndent)
	{
		HRESULT					hRes;
		char					Buf[MAX_PATH];
		ULONG					AsmSize;
		ULONG64					NextInstr = InCodePtr.ToInt64();
		StringBuilder^			Builder = gcnew StringBuilder();

		if(InMaxByteCount < 0)
			THROW(System::ArgumentOutOfRangeException, "Positive byte count expected.");

		if(IsBadReadPtr(InCodePtr.ToPointer(), InMaxByteCount))
			THROW(System::ArgumentException, "At least one byte of the given memory range (0x{0:X16}-0x{1:X16}) is not readable.",
				InCodePtr.ToInt64(), InCodePtr.ToInt64() + InMaxByteCount);

		Lock();

		try
		{
			ForceAvailable();

			if(!IsAttached)
				AttachToCurrentProcess();

			while(NextInstr < InCodePtr.ToInt64() + InMaxByteCount)
			{
				if(!SUCCEEDED(hRes = Unit->DebugControl->Disassemble(NextInstr, 0, Buf, sizeof(Buf), &AsmSize, &NextInstr)))
					THROW(System::ArgumentException, "Unable to disassemble given code block at (0x{1:X16}). (Code 0x{0:X8})", hRes,
						NextInstr);

				Builder->AppendLine(gcnew String(' ', InLineIndent) + gcnew String(Buf));
			}

			return Builder->ToString();
		}
		finally
		{
			Unlock();
		}
	}

	bool Debugger_Specific::RelocateRIPRelative(
		Int64 InOffset,
		Int64 InTargetOffset)
	{
#ifndef _M_X64
		return false;
#else
		HRESULT					hRes;
		char					Buf[MAX_PATH];
		ULONG					AsmSize;
		ULONG64					NextInstr;
		String^					Line;
		Int32					Pos;
		Int64					RelAddr;
		Int64					MemDelta = InTargetOffset - InOffset;

		BUGCHECK((MemDelta >= Int32::MinValue) && (MemDelta <= Int32::MaxValue), 
			"Relocated entry point would be out of reach.");

		Lock();

		try
		{
			ForceAvailable();

			if(!IsAttached)
				AttachToCurrentProcess();

			// test field...
			/*BYTE t[10] = {0x8b, 0x05, 0x12, 0x34, 0x56, 0x78};

			InOffset = (Int64)t;

			MemDelta = InTargetOffset - InOffset;*/

			if(!SUCCEEDED(hRes = Unit->DebugControl->Disassemble(InOffset, 0, Buf, sizeof(Buf), &AsmSize, &NextInstr)))
				THROW(System::ArgumentException, "Unable to disassemble entry point. (Code 0x{0:X8})", hRes);

			Line = Marshal::PtrToStringAnsi((IntPtr)Buf);
			Pos = Line->IndexOf('[');

			if(Pos < 0)
				return false;

			// parse content
			Line = Line->Substring(Pos + 1, Line->IndexOf(']', Pos + 1) - Pos - 1);

			if(Line->Length != 16)
				return false;

			for(int i = 0; i < Line->Length; i++)
			{
				Char c = Line[i];

				if(!(((c >= '0') && (c <= '9')) || ((c >= 'a') && (c <= 'f'))))
					return false;
			}

			if(!Int64::TryParse(Line, Globalization::NumberStyles::HexNumber, nullptr, RelAddr))
				return false;
			
			// verify that we are really RIP relative...
			RelAddr -= NextInstr;

			if((RelAddr < Int32::MinValue) || (RelAddr > Int32::MaxValue))
				return false;

			if(*((Int32*)(NextInstr - 4)) != RelAddr)
				return false;
		
			/*
				Just relocate this instruction...
			*/
			Int64			NewRelAddr = RelAddr - MemDelta;

			if((NewRelAddr < Int32::MinValue) || (NewRelAddr > Int32::MaxValue))
				THROW(System::NotSupportedException, "The given entry point contains at least one RIP-Relative instruction that could not be relocated!");

			// copy instruction
			memcpy((void*)InTargetOffset, (void*)InOffset, NextInstr - InOffset);

			*((Int32*)(InTargetOffset + (NextInstr - InOffset) - 4)) = (Int32)NewRelAddr;

			return true;
		}
		finally
		{
			Unlock();
		}
#endif
	}

	Int64 Debugger_Specific::GetNearInstruction(Int64 InOffset, Int32 InDelta)
	{
		Lock();

		try
		{
			ForceAvailable();

			if(!IsAttached)
				AttachToCurrentProcess();

			ULONG64				Next = 0;
			HRESULT				hRes = Unit->DebugControl->GetNearInstruction(InOffset, InDelta, &Next);

			if(!SUCCEEDED(hRes))
				THROW(System::ArgumentException, "The given pointer does not map to valid machine code. (Code 0x{0:X8})", hRes);

			return Next;
		}
		finally
		{
			Unlock();
		}
	}

	IntPtr Debugger_Specific::GetProcAddress(String^ InSymbolName)
	{
		Lock();

		try
		{
			ULONG64				Offset = 0;
			char*				SymName = NULL;
			HRESULT				hRes;

			ForceAvailable();

			if(!IsAttached)
				AttachToCurrentProcess();

			try
			{
				SymName = (char*)Marshal::StringToCoTaskMemAnsi(InSymbolName).ToPointer();

				if(!SUCCEEDED(hRes = Unit->DebugSymbols->GetOffsetByName(SymName, &Offset)))
					THROW(System::ArgumentException, "Unable to retrieve symbol offset. (code 0x{0:X8})", hRes);

				return (IntPtr)(void*)Offset;
			}
			finally
			{
				if(SymName != NULL)
					Marshal::FreeCoTaskMem((IntPtr)SymName);
			}
		}
		finally
		{
			Unlock();
		}
	}
};