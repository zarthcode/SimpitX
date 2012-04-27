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



#ifndef _REMOTEHOOKSERVICE_H_
#define _REMOTEHOOKSERVICE_H_

#include <windows.h>

// this is just to make machine code management easier
#define WRAP_ULONG64(Decl)\
union\
{\
	ULONG64 UNUSED;\
	Decl;\
}\

typedef struct _REMOTE_INFO_
{
	// will be the same for all processes
	WRAP_ULONG64(wchar_t* ParamString); // fixed 0
	WRAP_ULONG64(wchar_t* EasyHookPath); // fixed 8
	WRAP_ULONG64(wchar_t* PATH); // fixed 16
	WRAP_ULONG64(char* EasyHookEntry); // fixed 24
	WRAP_ULONG64(void* RemoteEntryPoint); // fixed 32
	WRAP_ULONG64(void* LoadLibraryW); // fixed; 40
	WRAP_ULONG64(void* FreeLibrary); // fixed; 48
	WRAP_ULONG64(void* GetProcAddress); // fixed; 56
	WRAP_ULONG64(void* VirtualFree); // fixed; 64
	WRAP_ULONG64(void* VirtualProtect); // fixed; 72
	WRAP_ULONG64(void* ExitThread); // fixed; 80
	WRAP_ULONG64(void* GetLastError); // fixed; 88
	
	wchar_t* ManagedPath;
	HANDLE hRemoteSignal; 
	DWORD HostProcess;
	DWORD Size;
	BYTE* UserData;
	DWORD UserDataSize;
}REMOTE_INFO, *LPREMOTE_INFO;

#ifndef _NATIVE_BIND_

#include "stdafx.h"

BYTE* GetInjectionPtr();

DWORD GetInjectionSize();


namespace EasyHook
{
	
	ref class HelperService
	{
	private:
		HelperServiceInterface^		m_Interface;
		Mutex^						TermMutex;

	internal:

		HelperService();

		property HelperServiceInterface^ Interface { HelperServiceInterface^ get() { return m_Interface; } }

		static void GenerateServiceName(String^% OutName);
	};

	ref class WOW64Bypass
	{
	private:
		HelperServiceInterface^		m_Interface;
		Mutex^						TermMutex;

	internal:

		property HelperServiceInterface^ Interface { HelperServiceInterface^ get() { return m_Interface; } }

		WOW64Bypass();
	};

	public ref class RemoteHooking_Specific : IRemoteHooking
	{
	private:
		static HelperService^				m_x86Service;
		static HelperService^				m_x64Service;
		static WOW64Bypass^					m_WOWBypass;
		static Object^						m_ThreadSafe = gcnew Object();

		static HelperServiceInterface^		m_x86ServiceInterface;
		static HelperServiceInterface^		m_x64ServiceInterface;
		static HelperServiceInterface^		m_WOWBypassInterface;
		static IpcServerChannel^			m_HookingChannel;
		[ThreadStatic()]
		static Int32						m_WakeUpThreadID = 0;

	internal:
		static property HelperServiceInterface^		x86ServiceInterface
		{
			HelperServiceInterface^ get();
			void set(HelperServiceInterface^ InValue);
		}

		static property HelperServiceInterface^		x64ServiceInterface
		{
			HelperServiceInterface^ get();
			void set(HelperServiceInterface^ InValue);
		}

		static property HelperServiceInterface^		WOWBypassInterface
		{
			HelperServiceInterface^ get();
			void set(HelperServiceInterface^ InValue);
		}

	public:

		virtual /*static*/ void Impersonate(Int32 InClientTID);

		static IRemoteHooking^ Constructor(){ return gcnew RemoteHooking_Specific(); }

		static property bool _IsAdministrator{ bool get(); }

		virtual /*static*/ property bool IsAdministrator{ bool get() { return _IsAdministrator; } }

		generic<class TObject> where TObject : MarshalByRefObject
		static IpcServerChannel^ _IpcCreateServer(
			String^% RefChannelName,
			WellKnownObjectMode InObjectMode,
			...
			cli::array<WellKnownSidType>^ InAllowedClientSIDs);

		generic<typename TClass>
		virtual /*static*/Object^ ExecuteAsService(
			String^ InMethodName,
			...
			cli::array<Object^>^ InParams);

		generic<class TObject> where TObject : MarshalByRefObject
		static TObject _IpcConnectClient(
			String^ InChannelName);

		generic<class TObject> where TObject : MarshalByRefObject
		virtual /*static*/ IpcServerChannel^ IpcCreateServer(
			String^% RefChannelName,
			WellKnownObjectMode InObjectMode,
			...
			cli::array<WellKnownSidType>^ InAllowedClientSIDs)
		{ return _IpcCreateServer<TObject>(RefChannelName, InObjectMode, InAllowedClientSIDs);}

		generic<class TObject> where TObject : MarshalByRefObject
		virtual /*static*/ TObject IpcConnectClient(
			String^ InChannelName)
		{ return _IpcConnectClient<TObject>(InChannelName); }

		virtual /*static*/ void Inject(
			Int32 InTargetPID,
			Int32 InWakeUpTID,
			InjectionOptions InOptions,
			String^ InLibraryPath_x86,
			String^ InLibraryPath_x64,
			...
			cli::array<Object^>^ InPassThruArgs);

		virtual /*static*/ Int32 CreateAndInject(
			String^ InEXEPath,
			InjectionOptions InOptions,
			String^ InLibraryPath_x86,
			String^ InLibraryPath_x64,
			...
			cli::array<Object^>^ InPassThruArgs);

		virtual /*static*/ void WakeUpProcess();

		virtual /*static*/ void SetWakeUpThreadID(Int32 InThreadID);

		static bool _IsX64Process(Int32 InTargetPID);

		virtual /*static*/ WindowsIdentity^ GetProcessIdentity(Int32 InTargetPID);
		virtual /*static*/ bool IsX64Process(Int32 InTargetPID) { return _IsX64Process(InTargetPID); }
		virtual /*static*/ Int32 GetCurrentProcessId() { return ::GetCurrentProcessId(); }
		virtual /*static*/ Int32 GetCurrentThreadId() { return ::GetCurrentThreadId(); }
		
		ref class Context : RemoteHooking::IContext
		{
		internal:
			Int32 m_HostPID;
			cli::array<Byte>^ m_UserData;
		public:
			virtual property Int32 HostPID { Int32 get() { return m_HostPID;} }
			virtual property cli::array<Byte>^ UserData { cli::array<Byte>^ get() { return m_UserData;} }
		};

		virtual /*static*/ RemoteHooking::IContext^ QueryContext(String^ InInjectParam);
	};
};

#endif
#endif