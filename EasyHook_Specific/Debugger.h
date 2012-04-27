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

#ifndef _LOCALDEBUGGER_H_
#define _LOCALDEBUGGER_H_

#include "stdafx.h"

namespace EasyHook
{
	// just to avoid static linking against "dbgeng.dll" because a debugger is optional in this case
	typedef HRESULT __stdcall PROC_DebugCreate(
			__in REFIID InterfaceId,
			__out PVOID* Interface
			);

	typedef NTSTATUS NTAPI ZwQueryInformationProcess_PROC(
		IN HANDLE ProcessHandle,
		IN PROCESSINFOCLASS ProcessInformationClass,
		OUT PVOID ProcessInformation,
		IN ULONG ProcessInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

	typedef DWORD WINAPI ZwGetProcessId_PROC(
	  IN HANDLE Process
	);

	typedef NTSTATUS NTAPI ZwQueryInformationThread_PROC(
		IN HANDLE ThreadHandle,
		IN THREADINFOCLASS ThreadInformationClass,
		OUT PVOID ThreadInformation,
		IN ULONG ThreadInformationLength,
		OUT PULONG ReturnLength OPTIONAL
		);

	typedef DWORD WINAPI ZwGetThreadId_PROC(
	  IN HANDLE Thread
	);

	typedef struct _CLIENT_ID
	{
		DWORD	    UniqueProcess;
		DWORD	    UniqueThread;
	}CLIENT_ID, * PCLIENT_ID;

	typedef struct _THREAD_BASIC_INFORMATION
	{
		NTSTATUS ExitStatus;
		PNT_TIB TebBaseAddress;
		CLIENT_ID ClientId;
		DWORD AffinityMask;
		LONG Priority;
		LONG BasePriority;
	}THREAD_BASIC_INFORMATION, *PTHREAD_BASIC_INFORMATION;


	public ref class Debugger_Specific : IDebugger
	{
	private:
		ref class UNIT : CriticalFinalizerObject, IDisposable
		{
		public:
			PROC_DebugCreate*				_DebugCreate;
			HMODULE							hDbgEng;
			IDebugClient*					DebugClient;
			IDebugControl*					DebugControl;
			IDebugSymbols*					DebugSymbols;
			IDebugDataSpaces2*				DebugDataSpaces;
			ZwGetProcessId_PROC*			ZwGetProcessId;
			ZwQueryInformationProcess_PROC* ZwQueryInformationProcess;
			ZwGetThreadId_PROC*				ZwGetThreadId;
			ZwQueryInformationThread_PROC*	ZwQueryInformationThread;
			HMODULE							hNtDll;
			HMODULE							hKernel32;
			Int32							AttachedProcess;
			Boolean							m_IsEnabled;
			
			UNIT();

			~UNIT()
			{ 
				this->!UNIT(); 
			}

			!UNIT();
		};

		static UNIT^				Unit = gcnew UNIT();

		Debugger_Specific(){}

		~Debugger_Specific(){}

		!Debugger_Specific(){}

		static void ForceAvailable()
		{
			if(!_IsAvailable)
				THROW(System::NotSupportedException, "The microsoft debugging engine is not available!");

			if(!Unit->m_IsEnabled)
				THROW(System::InvalidOperationException, "You have disabled debugging.");
		}

	internal:

		static bool RelocateRIPRelative(
			Int64 InOffset,
			Int64 InTargetOffset);

		static void AttachToCurrentProcess() { AttachToProcess(GetCurrentProcessId()); }

		static void AttachToProcess(Int32 InProcessID) { AttachToProcess(InProcessID, Timeout::Infinite); }

		static void AttachToProcess(Int32 InProcessID, Int32 InTimeoutMillis);

		static void DetachFromProcess();

		static property bool IsAttached { bool get() { return Unit->AttachedProcess != 0; } }

		static property Int32 AttachedProcess { Int32 get() { return Unit->AttachedProcess; } }

		static void Lock() { Monitor::Enter(Unit); }
		
		static void Unlock() { Monitor::Exit(Unit); }

		static Int64 GetNearInstruction(Int64 InOffset, Int32 InDelta);

		static IntPtr GetProcAddress(String^ InFullName);

	public:

		static IDebugger^ Constructor(){ return gcnew Debugger_Specific(); }

		static property Boolean _IsEnabled 
		{ Boolean get() { return Unit->m_IsEnabled; } void set(Boolean InValue) { Unit->m_IsEnabled = InValue; } }

		static property Boolean CanUse { Boolean get() { return _IsEnabled && _IsAvailable; } }

		virtual property bool IsEnabled 
		{ 
			bool get() { return _IsEnabled; }
			void set(bool InValue) 
			{ 
				_IsEnabled = InValue; 
				if(!InValue)
					DetachFromProcess();
			} 
		}

		static property bool _IsAvailable 
		{ bool get() { return Unit->hDbgEng != NULL; } }

		virtual property bool IsAvailable 
		{ bool get() { return _IsAvailable; } }

		static Int32 _GetThreadIdByHandle(IntPtr InThreadHandle);

		virtual Int32 GetThreadIdByHandle(IntPtr InThreadHandle)
		{ return _GetThreadIdByHandle(InThreadHandle); }

		static Int32 _GetProcessIdByHandle(IntPtr InProcessHandle);

		virtual Int32 GetProcessIdByHandle(IntPtr InProcessHandle)
		{ return _GetProcessIdByHandle(InProcessHandle); }

		static String^ _Disassemble(
			IntPtr InCodePtr,
			Int32 InMaxByteCount,
			Int32 InLineIndent);

		virtual String^ Disassemble(IntPtr InCodePtr, Int32 InMaxByteCount, Int32 InLineIndent)
		{ return _Disassemble(InCodePtr, InMaxByteCount, InLineIndent); }

		ref class ObjectInfo : Debugger::IObjectInfo
		{
		internal:
			Int32  m_Attributes;
			Int32  m_GrantedAccess;
			Int32  m_HandleCount;
			Int32  m_PointerCount;
			String^ m_TypeName;
			String^ m_ObjectName;
			ObjectType m_Type;

			ObjectInfo() {}

		public:
			virtual property Int32 Attributes { Int32 get() { return m_Attributes; } }
			virtual property Int32 GrantedAccess { Int32 get() { return m_GrantedAccess; } }
			virtual property Int32 HandleCount { Int32 get() { return m_HandleCount; } }
			virtual property Int32 PointerCount { Int32 get() { return m_PointerCount; } }
			virtual property String^ TypeName { String^ get() { return m_TypeName; } }
			virtual property String^ ObjectName { String^ get() { return m_ObjectName; } }
			virtual property ObjectType Type { ObjectType get() { return m_Type; } }
		};

		static Debugger::IObjectInfo^ _QueryObjectInformation(IntPtr InHandle);

		virtual Debugger::IObjectInfo^ QueryObjectInformation(IntPtr InHandle) { return _QueryObjectInformation(InHandle); }
	};
}

#endif