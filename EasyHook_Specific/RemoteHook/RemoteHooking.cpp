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
	bool RemoteHooking_Specific::_IsAdministrator::get()
	{
		SC_HANDLE			hSCManager = NULL;

		/*
			If someone is able to open the SCM with all access he is also able to create and start system services
			and so he is also able to act as a part of the system! We are just letting
			windows decide and don't enforce that the user is in the builtin admin group.
		*/

		if((hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
			return false;

		CloseServiceHandle(hSCManager);

		return true;
	}

	/*
		This is a stable impersonation and guarantees that the thread token is only
		upgraded during handler execution...
	*/
	generic<typename TClass>
	/*static*/ Object^ RemoteHooking_Specific::ExecuteAsService(
			String^ InMethodName,
			...
			cli::array<Object^>^ InParams)
	{
#ifdef _M_X64
		return x64ServiceInterface->ExecuteAsService<TClass>(InMethodName, InParams);
#else
		return x86ServiceInterface->ExecuteAsService<TClass>(InMethodName, InParams);
#endif
	}

	// will only be executed on service side...
	/*static*/ void RemoteHooking_Specific::Impersonate(Int32 InClientTID)
	{
		////////////////////////////////////////////////////////
		// upgrade thread token to local system account...
		HANDLE hTargetThread = NULL;
		HANDLE hCurrentToken = NULL;

		try
		{
			if((hTargetThread = OpenThread(THREAD_SET_THREAD_TOKEN | THREAD_QUERY_INFORMATION, FALSE, InClientTID)) == NULL)
				THROW(System::AccessViolationException, "Unable to open remote thread.");

			// copy thread token...
			if(!ImpersonateSelf(SecurityImpersonation))
				THROW(System::ApplicationException, "Unable to impersonate self.");

			if(!OpenThreadToken(GetCurrentThread(), TOKEN_IMPERSONATE, TRUE, &hCurrentToken))
				THROW(System::ApplicationException, "Unable to open current thread token.");

			if(!SetThreadToken(&hTargetThread, hCurrentToken))
				THROW(System::ApplicationException, "Unable to set remote thread token.");
		}
		finally
		{
			if(hTargetThread != NULL)
				CloseHandle(hTargetThread);

			if(hCurrentToken != NULL)
				CloseHandle(hCurrentToken);
		}
	}



	/*static*/ WindowsIdentity^ RemoteHooking_Specific::GetProcessIdentity(Int32 InTargetPID)
	{
		HANDLE					hProc = NULL;
		HANDLE					hProcToken = NULL;

		try
		{
			if((hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, InTargetPID)) == NULL)
			{
				if(GetLastError() == ERROR_ACCESS_DENIED)
					THROW(System::AccessViolationException, "The given process is not accessible.")
				else
					THROW(System::ArgumentException, "The given process does not exist.");
			}

			if(!OpenProcessToken(hProc, TOKEN_READ, &hProcToken))
					THROW(System::ApplicationException, "Unable to query process token.");

			WindowsIdentity^		Result = gcnew WindowsIdentity((IntPtr)hProcToken);


			return Result;
		}
		finally
		{
			if(hProc != NULL)
				CloseHandle(hProc);

			if(hProcToken != NULL)
				CloseHandle(hProcToken);
		}
	}

	/*static*/ Int32 RemoteHooking_Specific::CreateAndInject(
			String^ InEXEPath,
			InjectionOptions InOptions,
			String^ InLibraryPath_x86,
			String^ InLibraryPath_x64,
			...
			cli::array<Object^>^ InPassThruArgs)
	{
		STARTUPINFO				StartInfo;
		PROCESS_INFORMATION		ProcessInfo;
		wchar_t					pEXEPath[MAX_PATH + 1];
		wchar_t					pCurrentDir[MAX_PATH + 1];
		String^					CurrentDir;

		// parse path
		InEXEPath = Path::GetFullPath(InEXEPath);

		if(!File::Exists(InEXEPath))
			THROW(System::IO::FileNotFoundException, "The given file \"{0}\" does not exist.", InEXEPath);

		if(InEXEPath->Length > MAX_PATH)
			THROW(System::ArgumentOutOfRangeException, "The given file \"{0}\" is too long.", InEXEPath);

		CurrentDir = Path::GetDirectoryName(InEXEPath);

		memset(&StartInfo, 0, sizeof(StartInfo));
		memset(&pEXEPath[0], 0, sizeof(pEXEPath));

		StartInfo.cb = sizeof(StartInfo);

		Marshal::Copy(InEXEPath->ToCharArray(), 0, (IntPtr)&pEXEPath[0], InEXEPath->Length);
		Marshal::Copy(CurrentDir->ToCharArray(), 0, (IntPtr)&pCurrentDir[0], CurrentDir->Length);

		// create suspended process
		if(!CreateProcessW(
				pEXEPath, 
				NULL, NULL, NULL, FALSE, 
				DETACHED_PROCESS | CREATE_SUSPENDED,
				NULL,
				pCurrentDir,
				&StartInfo,
				&ProcessInfo))
			THROW(System::ApplicationException, "Unable to start process; please check the given parameters.");


		// inject library
		try
		{
			Inject(
				ProcessInfo.dwProcessId,
				ProcessInfo.dwThreadId,
				InOptions,
				InLibraryPath_x86,
				InLibraryPath_x64,
				InPassThruArgs);
		}
		catch(Exception^ e)
		{
			if(!TerminateProcess(ProcessInfo.hProcess, 0))
				THROW(System::ApplicationException, "Unable to terminate target process after internal failure.");

			throw e;
		}
		finally
		{
			CloseHandle(ProcessInfo.hProcess);
			CloseHandle(ProcessInfo.hThread);
		}

		return ProcessInfo.dwProcessId;
	}

	/*static*/ void RemoteHooking_Specific::WakeUpProcess()
	{
		if(m_WakeUpThreadID == 0)
			return;

		// open and resume thread...
		HANDLE			hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, m_WakeUpThreadID);

		if((hThread == NULL) || !ResumeThread(hThread))
			THROW(System::ApplicationException, "Unable to wakeup process.");

		CloseHandle(hThread);

		m_WakeUpThreadID = 0;
	}


	/*
		Only intended for Vista and later... Will return NULL for all others which
		should use CreateRemoteThread() and services instead!
	
		In contrast to RtlCreateUserThread() this one will fortunately setup a proper activation
		context stack, which is required to load the NET framework and many other
		common APIs. This is why RtlCreateUserThread() can't be used for Windows XP
		,for example, even if it would replace the windows service approach which is required
		in order to get CreateRemoteThread() working.

		Injection through WOW64 boundaries is still not directly supported and requires
		a WOW64 bypass helper process.
	*/
	HANDLE NtCreateThreadEx(
		HANDLE InProcess,
		void* InRemoteThreadStart,
		void* InRemoteCallback)
	{
		typedef DWORD WINAPI NtCreateThreadEx_PROC(
			PHANDLE ThreadHandle,
			ACCESS_MASK DesiredAccess,
			LPVOID ObjectAttributes,
			HANDLE ProcessHandle,
			LPTHREAD_START_ROUTINE lpStartAddress,
			LPVOID lpParameter,
			BOOL CreateSuspended,
			DWORD dwStackSize,
			LPVOID Unknown1,
			LPVOID Unknown2,
			LPVOID Unknown3
		); 

		// this will only work for vista and later...
		NtCreateThreadEx_PROC* VistaCreateThread = (NtCreateThreadEx_PROC*)
			GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx"); 

		if(VistaCreateThread == NULL)
			return NULL;

		HANDLE			hRemoteThread = NULL;
		HRESULT			hRes = 0;

		if(!SUCCEEDED(hRes = VistaCreateThread(
				&hRemoteThread,
				0x1FFFFF, // all access
				NULL,
				InProcess,
				(LPTHREAD_START_ROUTINE)InRemoteThreadStart,
				InRemoteCallback,
				FALSE,
				NULL,
				NULL,
				NULL,
				NULL
				)))
			return NULL;

		return hRemoteThread;
	}

	/*
	 * This method is one of the trickiest ;-) but also most important ones!
	 * Without NET remoting the following would be a real burden... In the first
	 * version which was pure unmanaged code I had to do this with windows API
	 * and it was rather frustrating... In particular it didn't work quiet well,
	 * only when windows was in the mood ;-)...
	*/
	/*static*/ void RemoteHooking_Specific::Inject(
			Int32 InTargetPID,
			Int32 InWakeUpTID,
			InjectionOptions InOptions,
			String^ InLibraryPath_x86,
			String^ InLibraryPath_x64,
			cli::array<Object^>^ InPassThruArgs)
	{
		HANDLE					hProc = NULL;
		HANDLE					hRemoteThread = NULL;
		HANDLE					hSignal = NULL;
		BYTE*					RemoteInjectCode = NULL;
		LPREMOTE_INFO			Info = NULL;
		String^					EasyHookPath;
		String^					PATH;
		String^					ManagedPath;
		DWORD					RemoteInfoSize = 0;
		BYTE*					Offset = 0;
		Boolean					IsAttached = false;
		String^					LibraryPath;
		HANDLE					hOldToken = NULL;
		WindowsIdentity^		CurrentAccount = WindowsIdentity::GetCurrent();

#ifdef _M_X64
		String^					EasyHookEntry = "HookCompleteInjection";
#else
		String^					EasyHookEntry = "_HookCompleteInjection@4";
#endif

		try
		{
			if(InTargetPID == ::GetCurrentProcessId())
				THROW(System::InvalidOperationException, "It is unstable to inject libraries into the same process.");

			// open target process
			if((hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, InTargetPID)) == NULL)
			{
				if(!CurrentAccount->IsSystem)
				{
					// only applies to non-services...
					if(!ImpersonateSelf(SecurityImpersonation))
						THROW(System::ApplicationException, "Unable to impersonate self.");

					if(!OpenThreadToken(GetCurrentThread(), TOKEN_ALL_ACCESS, FALSE, &hOldToken))
						THROW(System::ApplicationException, "Unable to backup current thread token.");

					// 32-bit service is always available
					x86ServiceInterface->Impersonate(GetCurrentThreadId());

					/*
						Now we are acting as part of the system and this will succeed in any case.
						If the target itself is a part of the system, we will finally fail to
						create the remote thread, which is handled there!
					*/
					if((hProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, InTargetPID)) == NULL)
					{
						if(GetLastError() == ERROR_ACCESS_DENIED)
							THROW(System::AccessViolationException, "Access to target process denied.")
						else
							THROW(System::ArgumentException, "The given process does not exist.");
					}
				}
				else
					THROW(System::ArgumentException, "The given process does not exist.");
			}

			/*
				Check bitness...

				After this we can assume hooking a target that is running in the same
				WOW64 level.
			*/
#ifdef _M_X64
			if(!_IsX64Process(InTargetPID))
			{
				WOWBypassInterface->Inject(
					InTargetPID, InWakeUpTID, InOptions,
					Path::GetFullPath(InLibraryPath_x86),
					Path::GetFullPath(InLibraryPath_x64),
					InPassThruArgs);

				return;
			}

			LibraryPath = InLibraryPath_x64;
#else
			if(_IsX64Process(InTargetPID))
			{
				WOWBypassInterface->Inject(
					InTargetPID, InWakeUpTID, InOptions, 
					Path::GetFullPath(InLibraryPath_x86),
					Path::GetFullPath(InLibraryPath_x64),
					InPassThruArgs);

				return;
			}

			LibraryPath = InLibraryPath_x86;
#endif
			/*
				Validate library path... It may either be a partial assembly name or path.
			*/
			try
			{
				if(File::Exists(LibraryPath))
				{
					// translate to assembly name
					LibraryPath = AssemblyName::GetAssemblyName(Path::GetFullPath(LibraryPath))->FullName;
				}

				// validate assembly name
				Assembly^		UserAsm;

				if((UserAsm = Assembly::LoadWithPartialName(LibraryPath)) == nullptr)
					throw gcnew Exception();

				if((Int32)(UserAsm->GetName()->Flags & AssemblyNameFlags::PublicKey) == 0)
					throw gcnew Exception();
			}
			catch(...)
			{
				String^		Lib = InLibraryPath_x86;

#ifdef _M_X64
				Lib = InLibraryPath_x64;
#endif

				THROW(System::IO::FileNotFoundException, "Couldn't find user library \"" + Lib + 
					"\". Make sure that this is either a valid assembly name or a file system path mapping to an existing " +
					"and valid NET assembly with a strong name.");
			}

			// check pass thru parameters
			cli::array<Byte>^		UserData = nullptr;
			Int32					UserDataSize = 0;

			try
			{
				if(InPassThruArgs != nullptr)
				{
					MemoryStream^		Stream = gcnew MemoryStream();
					BinaryFormatter^	Format = gcnew BinaryFormatter();

					Format->Serialize(Stream, InPassThruArgs);

					UserData = Stream->GetBuffer();
					UserDataSize = Stream->Length;
				}
			}
			catch(Exception^ e)
			{
				THROW(System::ArgumentException, "Unable to serialize one or more pass thru arguments.\r\n" + e->ToString()); 
			}

			// import strings...
			EasyHookPath = RemoteHooking_Specific::typeid->Assembly->Location;
			ManagedPath = InjectionLoader::typeid->Assembly->Location;
			PATH = Path::GetDirectoryName(Process::GetCurrentProcess()->MainModule->FileName) + ";";

			// compute maximum length of unmanaged parameter string for ExecuteInDefaultAppDomain()
			String^			ChannelName;

			IpcCreateServer<HelperServiceInterface^>(ChannelName, WellKnownObjectMode::SingleCall);

			String^			ParamString = ChannelName + "|" + LibraryPath + "|" + InWakeUpTID + "|" + (Int64::MinValue).ToString();

			// allocate remote information block
			RemoteInfoSize = (EasyHookPath->Length + 1) * 2 + (EasyHookEntry->Length + 1) + (ManagedPath->Length + 1) * 2 +
				(PATH->Length + 1) * 2 + (ParamString->Length + 1) * 2 + UserDataSize;

			RemoteInfoSize += sizeof(REMOTE_INFO);

			if((Info = (LPREMOTE_INFO)Marshal::AllocCoTaskMem(RemoteInfoSize).ToPointer()) == NULL)
				THROW(System::OutOfMemoryException, "Unable to allocate unmanaged memory.");

			memset(Info, 0, RemoteInfoSize);

			Debugger_Specific::Lock();

			try
			{
				// try to attach to target process...
				Boolean			HasSymbols = false;

				if(Debugger_Specific::CanUse)
				{
					Int32 SymbolPID = InTargetPID;

					if((Int32)(InOptions & InjectionOptions::AlwaysUseLocalSymbols) != 0)
					{
						/*
							GetProcAddress() will ironically return a wrong address for itself ;-) on
							Windows Vista x64; the address of StubGetProcAddress(), no idea why but
							it will fail for most target processes. This is why we are trying to use the 
							debugger in any case, which will provide reliable output. Even if our
							C++ injection code will again update the symbols with the exact ones in
							any case, a wrong address for GetProcAddress() will crash the target 
							before we get there.
						*/
						SymbolPID = ::GetCurrentProcessId();
					}

					/*
						In general it's much more stable to directly obtain symbol
						addresses from the target process. But some processes might
						not like an attached debugger. In this case just call the
						method with InjectionOptions::AlwaysUseLocalSymbols.
					*/
					try
					{
						Debugger_Specific::AttachToProcess(SymbolPID);

						// to speed up things, we will just detach from targets later, but not from our current process...
						IsAttached = (SymbolPID != ::GetCurrentProcessId());
						
						Info->LoadLibraryW = Debugger_Specific::GetProcAddress("kernel32!LoadLibraryW").ToPointer();
						Info->FreeLibrary = Debugger_Specific::GetProcAddress("kernel32!FreeLibrary").ToPointer();
						Info->GetProcAddress = Debugger_Specific::GetProcAddress("kernel32!GetProcAddress").ToPointer();
						Info->VirtualFree = Debugger_Specific::GetProcAddress("kernel32!VirtualFree").ToPointer();
						Info->VirtualProtect = Debugger_Specific::GetProcAddress("kernel32!VirtualProtect").ToPointer();
						Info->ExitThread = Debugger_Specific::GetProcAddress("kernel32!ExitThread").ToPointer();
						Info->GetLastError = Debugger_Specific::GetProcAddress("kernel32!GetLastError").ToPointer();

						HasSymbols = true;
					}
					catch(...)
					{
						// we will just use the local symbols...
					}
				}

				if(!HasSymbols)
				{
					/*
						This is not very stable but better than nothing...
					*/
					HMODULE hMod = GetModuleHandleA("kernel32.dll");

					Info->LoadLibraryW = GetProcAddress(hMod, "LoadLibraryW");
					Info->FreeLibrary = GetProcAddress(hMod, "FreeLibrary");
					Info->GetProcAddress = GetProcAddress(hMod, "GetProcAddress");
					Info->VirtualFree = GetProcAddress(hMod, "VirtualFree");
					Info->VirtualProtect = GetProcAddress(hMod, "VirtualProtect");
					Info->ExitThread = GetProcAddress(hMod, "ExitThread");
					Info->GetLastError = GetProcAddress(hMod, "GetLastError");
				}
			}
			finally
			{
				try
				{
					if(IsAttached)
						Debugger_Specific::DetachFromProcess();
				}
				finally
				{
					Debugger_Specific::Unlock();
				}
			}

			// allocate memory in target process
			UInt32 CodeSize = GetInjectionSize();

			if((RemoteInjectCode = (BYTE*)VirtualAllocEx(hProc, NULL, CodeSize + RemoteInfoSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE)) == NULL)
				THROW(System::OutOfMemoryException, "Failed to allocate executable data space memory in target process.");

			// compose unmanaged parameter string for ExecuteInDefaultAppDomain()
			ParamString = ChannelName + "|" + LibraryPath + "|" + InWakeUpTID + "|" + ((Int64)(RemoteInjectCode + CodeSize)).ToString();

			// save strings
			Offset = (BYTE*)(Info + 1);

			Info->EasyHookEntry = (char*)Offset;
			Info->EasyHookPath = (wchar_t*)(Offset += (EasyHookEntry->Length + 1));
			Info->PATH = (wchar_t*)(Offset += (EasyHookPath->Length + 1) * 2);
			Info->ParamString = (wchar_t*)(Offset += (PATH->Length + 1) * 2);
			Info->ManagedPath = (wchar_t*)(Offset += (ParamString->Length + 1) * 2);
			Info->UserData = (BYTE*)(Offset += (ManagedPath->Length + 1) * 2);

			Info->Size = RemoteInfoSize;
			Info->HostProcess = ::GetCurrentProcessId();
			Info->UserDataSize = 0;

			Offset += UserDataSize;

			if(Offset - ((BYTE*)Info) > Info->Size)
				THROW(System::ApplicationException, "Buffer overflow in unmanaged code detected.");

			Marshal::Copy(ParamString->ToCharArray(), 0, (IntPtr)Info->ParamString, ParamString->Length);
			Marshal::Copy(EasyHookPath->ToCharArray(), 0, (IntPtr)Info->EasyHookPath, EasyHookPath->Length);
			Marshal::Copy(PATH->ToCharArray(), 0, (IntPtr)Info->PATH, PATH->Length);
			Marshal::Copy(ManagedPath->ToCharArray(), 0, (IntPtr)Info->ManagedPath, ManagedPath->Length);
			Marshal::Copy(Encoding::ASCII->GetBytes(EasyHookEntry), 0, (IntPtr)Info->EasyHookEntry, EasyHookEntry->Length);

		
			if(UserData != nullptr)
			{
				Marshal::Copy(UserData, 0, (IntPtr)Info->UserData, UserDataSize);

				Info->UserDataSize = UserDataSize;
			}

			// copy code into target process
			SIZE_T BytesWritten = 0;

			if(!WriteProcessMemory(hProc, RemoteInjectCode, GetInjectionPtr(), CodeSize, &BytesWritten) || (BytesWritten != CodeSize))
				THROW(System::ApplicationException, "Failed to write injection code into target process.");

			// create and export signal event>
			if((hSignal = CreateEvent(NULL, TRUE, FALSE, NULL)) == NULL)
				THROW(System::ApplicationException, "Unable to create event.");

			// Possible resource leck: the remote handles cannt be closed here if an error occurs
			if(!DuplicateHandle(GetCurrentProcess(), hSignal, hProc, &Info->hRemoteSignal, EVENT_ALL_ACCESS, FALSE, 0))
				THROW(System::ApplicationException, "Unable to export signal handle for target process.");

			// relocate remote information
			LPREMOTE_INFO RemoteInfo = (LPREMOTE_INFO)(RemoteInjectCode + CodeSize);
			__int64 Diff = ((BYTE*)RemoteInfo - (BYTE*)Info);

			Info->EasyHookEntry = (char*)(((BYTE*)Info->EasyHookEntry) + Diff);
			Info->EasyHookPath = (wchar_t*)(((BYTE*)Info->EasyHookPath) + Diff);
			Info->ParamString = (wchar_t*)(((BYTE*)Info->ParamString) + Diff);
			Info->ManagedPath = (wchar_t*)(((BYTE*)Info->ManagedPath) + Diff);
			Info->PATH = (wchar_t*)(((BYTE*)Info->PATH) + Diff);

			if(Info->UserData != NULL)
				Info->UserData = (BYTE*)(((BYTE*)Info->UserData) + Diff);

			Info->RemoteEntryPoint = RemoteInjectCode;

			if(!WriteProcessMemory(hProc, RemoteInfo, Info, RemoteInfoSize, &BytesWritten) || (BytesWritten != RemoteInfoSize))
				THROW(System::ApplicationException, "Failed to write injection information into target process.");


			if((hRemoteThread = NtCreateThreadEx(hProc, RemoteInjectCode, RemoteInfo)) == NULL)
			{
				// create remote thread and wait for injection completion
				if((hRemoteThread = CreateRemoteThread(hProc, NULL, 0, (LPTHREAD_START_ROUTINE)RemoteInjectCode, RemoteInfo, 0, NULL)) == NULL)
				{
					/*
						Occurs when services are trying to hook non-services or vice versa...
					*/
					if(CurrentAccount->IsSystem)
					{
						THROW(System::NotSupportedException, "Unable to create remote thread!");
					}
					else
					{
						// try to use service for hooking services...
					#ifdef _M_X64
						x64ServiceInterface->Inject(
								InTargetPID, InWakeUpTID, InOptions,
								Path::GetFullPath(InLibraryPath_x86),
								Path::GetFullPath(InLibraryPath_x64),
								InPassThruArgs);
					#else
						x86ServiceInterface->Inject(
								InTargetPID, InWakeUpTID, InOptions,
								Path::GetFullPath(InLibraryPath_x86),
								Path::GetFullPath(InLibraryPath_x64),
								InPassThruArgs);
					#endif
						return;
					}
				}
			}

			/*
			 * The assembler codes are designed to let us derive extensive error information...
			*/
			HANDLE Handles[2] = {hRemoteThread, hSignal};
			DWORD Code = WaitForMultipleObjects(2, Handles, FALSE, 10000);

			if(Code == WAIT_OBJECT_0)
			{
				// parse error code
				GetExitCodeThread(hRemoteThread, &Code);

				SetLastError(Code & 0x0FFFFFFF);

				switch(Code & 0xF0000000)
				{
				case 0x10000000: THROW(System::ApplicationException, "Unable to find internal entry point.");
				case 0x20000000: THROW(System::AccessViolationException, "Unable to make stack executable.");
				case 0x30000000: THROW(System::ApplicationException, "Unable to release injected library.");
				case 0x40000000: THROW(System::ApplicationException, "Unable to find EasyHook library in target process context.");
				case 0xF0000000: // error in C++ injection completion
					{
						switch(Code & 0xFF)
						{
						case 1: THROW(System::ApplicationException, "Unable to allocate memory in target process.");
						case 2: THROW(System::ApplicationException, "Unable to adjust target's PATH variable.");
						case 5: THROW(System::ApplicationException, "Unable to bind NET Runtime to target process.");
						case 7: THROW(System::ApplicationException, "Unable to find internal entry point.");
						case 8: THROW(System::ApplicationException, "Unable to signal remote event.");
						case 9: THROW(System::ApplicationException, "Unable connect to hooking host from target process.");
						default: THROW(System::ApplicationException, "Unknown error in injected C++ completion routine.");
						}
					}break;
				case 0:
					THROW(System::ApplicationException, "C++ completion routine has returned success but didn't raise the remote event.");
				default:
					THROW(System::ApplicationException, "Unknown error in injected assembler code.");
				}
			}
			else if(Code != WAIT_OBJECT_0 + 1)
				THROW(System::ApplicationException, "Unable to wait for injection completion due to timeout. ");

			/*
				In case of no basic error, we are waiting for the initial injection code to complete...
				This allows us to listen for extensive error reports in case of something is going wrong!
			*/
			HelperServiceInterface::WaitForInjection(InTargetPID, 5000);

			// now we can be sure that the user library is running... So we have done our part!
		}
		finally
		{
			// release resources
			if(hProc != NULL)
				CloseHandle(hProc);

			if(Info != NULL)
				Marshal::FreeCoTaskMem((IntPtr)Info);

			if(hRemoteThread != NULL)
				CloseHandle(hRemoteThread);

			if(hSignal != NULL)
				CloseHandle(hSignal);


			// restore identity
			if(hOldToken != NULL)
			{
				if(!SetThreadToken(NULL, hOldToken))
				{
					CloseHandle(hOldToken);

					THROW(System::ApplicationException, "Unable to restore thread token.");
				}

				CloseHandle(hOldToken);
			}
		}
	}

	/*static*/ void RemoteHooking_Specific::SetWakeUpThreadID(Int32 InThreadID)
	{
		if(InThreadID == 0)
		{
			m_WakeUpThreadID = 0;

			return;
		}

		HANDLE			hThread = OpenThread(THREAD_SUSPEND_RESUME, FALSE, InThreadID);

		if(hThread != NULL)
		{
			CloseHandle(hThread);

			m_WakeUpThreadID = InThreadID;
		}
		else
			THROW(System::ArgumentException, "The given thread ID is not valid.");
	}

	bool RemoteHooking_Specific::_IsX64Process(Int32 InTargetPID)
	{
		BOOL			IsTarget64Bit = false;
		HANDLE			hProc = NULL;

		try
		{
			// open target process
			if((hProc = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, InTargetPID)) == NULL)
			{
				if(GetLastError() == ERROR_ACCESS_DENIED)
					THROW(System::AccessViolationException, "The given process is not accessible.")
				else
					THROW(System::ArgumentException, "The given process does not exist.");
			}

			// if WOW64 is not available then target must be 32-bit
			typedef BOOLEAN __stdcall IsWow64Process_PROC(HANDLE InProc, BOOL* OutResult);

			HMODULE			hMod = GetModuleHandleA("kernel32.dll");

			if(hMod == NULL)
				THROW(System::ApplicationException, "Unable to load \"kernel32.dll\".");

			IsWow64Process_PROC* pIsWow64Process = (IsWow64Process_PROC*)GetProcAddress(hMod, "IsWow64Process");

	#ifdef _M_X64
			// if the target is not WOW64, then it is 64-bit
			if(!pIsWow64Process(hProc, &IsTarget64Bit))
				THROW(System::ApplicationException, "Unable to detect wether target process is 64-bit or not.");

			IsTarget64Bit = !IsTarget64Bit;

	#else

			if(pIsWow64Process == NULL)
			{
				return false;
			}
			else
			{
				// check if we are running on a 32-bit OS
				typedef void GetNativeSystemInfo_PROC(LPSYSTEM_INFO OutSysInfo);

				GetNativeSystemInfo_PROC* pGetNativeSystemInfo = (GetNativeSystemInfo_PROC*)
					::GetProcAddress(GetModuleHandleA("ntdll.dll"), "GetNativeSystemInfo");

				if(pGetNativeSystemInfo == NULL)
					return false;

				SYSTEM_INFO		SysInfo;

				pGetNativeSystemInfo(&SysInfo);

				if(SysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
					return false;

				// if not, then and only then a 32-bit process will be marked as WOW64 process!
				if(!pIsWow64Process(hProc, &IsTarget64Bit))
					THROW(System::ApplicationException, "Unable to detect wether target process is 64-bit or not.");

				IsTarget64Bit = !IsTarget64Bit;
			}
	#endif

			return IsTarget64Bit;
		}
		finally
		{
			if(hProc != NULL)
				CloseHandle(hProc);
		}
	}

	RemoteHooking::IContext^ RemoteHooking_Specific::QueryContext(String^ InInjectParam)
	{
		// get memory address
		if((InInjectParam == nullptr) || (InInjectParam->Length == 0))
			return nullptr;

		LPREMOTE_INFO Info = (LPREMOTE_INFO)Int64::Parse(InInjectParam, System::Globalization::NumberStyles::Integer);

		// access unmanaged memory...
		if(IsBadReadPtr(Info, sizeof(REMOTE_INFO)) || IsBadReadPtr(Info, Info->Size) ||
				IsBadReadPtr(Info->UserData, Info->UserDataSize))
			THROW(System::AccessViolationException, "The given string does map to an invalid piece of memory.");
		
		Context^ Result = gcnew Context();

		Result->m_HostPID = Info->HostProcess;
		
		if(Info->UserDataSize > 0)
		{
			Result->m_UserData = gcnew cli::array<Byte>(Info->UserDataSize);

			Marshal::Copy((IntPtr)Info->UserData, Result->UserData, 0, Info->UserDataSize);
		}

		return Result;
	}
};