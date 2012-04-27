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
#include <tlhelp32.h>

namespace EasyHook
{
	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::IsThreadSuspended
	//////////////////////////////////////////////////////////////////////////////////
	bool LocalHook_Specific::IsThreadSuspended(HANDLE InThread)
	{
		DWORD		IsSuspended = SuspendThread(InThread);

		if(IsSuspended != (DWORD)-1)
		{
			ResumeThread(InThread);

			if(IsSuspended == 0)
				return false;
			else	
				return true;
		}
		else
		{
			if(GetLastError() != ERROR_SUCCESS)
				THROW(System::ApplicationException, "Unable to get thread suspend state.")
			else
				return true;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::ThreadGetCurrentRIP
	//////////////////////////////////////////////////////////////////////////////////
#pragma managed(off)

	void* ThreadGetCurrentRIP(HANDLE InThread)
	{
		CONTEXT			Ctx;

		Ctx.ContextFlags = CONTEXT_CONTROL;

		if(!GetThreadContext(InThread, (CONTEXT*)&Ctx))
			return NULL;

	#ifdef _M_X64
		return (void*)Ctx.Rip;
	#else
		return (void*)Ctx.Eip;
	#endif
	}

#pragma managed(on)

	Int64 LocalHook_Specific::ThreadGetCurrentRIP(HANDLE InThread)
	{
		Int64		RIP = (Int64)EasyHook::ThreadGetCurrentRIP(InThread);

		if(RIP == 0)
			THROW(System::ArgumentException, "Unable to query thread context. Maybe the given handle is invalid.");

		return RIP;
	}

	#define DEBUG_HOOK_INSTALLATION

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::SuspendAllThreads
	//////////////////////////////////////////////////////////////////////////////////
	/*
		This code is potentially undebuggable... 
	*/
	bool LocalHook_Specific::SuspendAllThreads()
	{
		HANDLE					hThreadSnap = INVALID_HANDLE_VALUE; 
		THREADENTRY32			NativeEntry;

		Monitor::Enter(ThreadList);

		try
		{	
			// only needed once
			if(AreThreadsSuspended == 0)
			{
#ifndef DEBUG_HOOK_INSTALLATION
				NativeEntry.dwSize = sizeof(NativeEntry); 

				ThreadList->Clear();

				//////////////////////////////////////////////////////////////////////
				// suspend threads
				Int32				Count;

				do
				{
					// take a snapshot of all running threads  
					if((hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)) == INVALID_HANDLE_VALUE)
						THROW(System::ApplicationException, "Unable to enumerate system threads.");

					// count threads
					if(!Thread32First(hThreadSnap, &NativeEntry)) 
						THROW(System::ApplicationException, "Unable to get first thread in enumeration.");

					Count = 0;

					do 
					{ 
						if((NativeEntry.th32OwnerProcessID == GetCurrentProcessId()) && (NativeEntry.th32ThreadID != GetCurrentThreadId()))
						{
							// was thread already added to list?
							Boolean				Exists = false;

							for(int iThread = 0; iThread < ThreadList->Count; iThread++)
							{
								ThreadEntry^		Entry = ThreadList[iThread];

								if(Entry->ID == NativeEntry.th32ThreadID)
								{
									// is thread still suspended?
									if(!IsThreadSuspended(Entry->Handle))
									{
										// this is something that should never occur...
										Entry->SuspendCount++;

										SuspendThread(Entry->Handle);

										// more rounds needed to check again...
										Count++;
									}
									else
									{
										// now we can safely obtain the current RIP-Value
										Entry->RIP = ThreadGetCurrentRIP(Entry->Handle);
									}

									Exists = true;

									break;
								}
							}

							if(Exists)
								continue;

							////////////////////////////////////////////////////////
							// add new thread

							ThreadEntry^		NewThread = gcnew ThreadEntry();

							ThreadList->Add(NewThread);

							NewThread->ID = NativeEntry.th32ThreadID;

							// open thread with proper permissions
							if((NewThread->Handle = OpenThread(THREAD_SUSPEND_RESUME | THREAD_QUERY_INFORMATION |
									THREAD_GET_CONTEXT | SYNCHRONIZE, FALSE, NativeEntry.th32ThreadID)) == NULL)
									THROW(System::ApplicationException, "Unable to open one or more threads.");
							
							if(SuspendThread(NewThread->Handle) == (DWORD)-1)
							{
								// maximum suspend count reached... This thread shall not be resumed
							}
							else
								NewThread->SuspendCount++;

							Count++;
						}

					}while(Thread32Next(hThreadSnap, &NativeEntry)); 

				}while(Count != 0);
#endif
			}

			// threads are now suspended!
			AreThreadsSuspended++;
		}
		catch(...)
		{
#ifndef DEBUG_HOOK_INSTALLATION
			for(int iThread = 0; iThread < ThreadList->Count; iThread++)
			{
				ThreadEntry^		Entry = ThreadList[iThread];

				for(int i = 0; i < Entry->SuspendCount; i++)
				{
					ResumeThread(Entry->Handle);
				}

				CloseHandle(Entry->Handle);

				Entry->Handle = NULL;
			}
#endif

			ThreadList->Clear();

			/*
				As we can recover, it is possible to just tell if we have succeeded or not.
			*/
			return false;
		}
		finally
		{
			Monitor::Exit(ThreadList);

			if(hThreadSnap != INVALID_HANDLE_VALUE)
				CloseHandle(hThreadSnap);
		}

		return true;
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::ResumeAllThreads
	//////////////////////////////////////////////////////////////////////////////////
	/*
		This code is potentially undebuggable... 
	*/
	void LocalHook_Specific::ResumeAllThreads()
	{
		Monitor::Enter(ThreadList);

		try
		{
			BUGCHECK(AreThreadsSuspended > 0, "Attempt to resume threads without them being suspended!");

			if(--AreThreadsSuspended == 0)
			{
#ifndef DEBUG_HOOK_INSTALLATION
				// resume all threads of current process
				for(int iThread = 0; iThread < ThreadList->Count; iThread++)
				{
					ThreadEntry^		Entry = ThreadList[iThread];

					// release entry
					for(Int32 i = 0; i < Entry->SuspendCount; i++)
					{
						ResumeThread(Entry->Handle);
					}

					CloseHandle(Entry->Handle);

					Entry->Handle = NULL;
				}
#endif
			}
		}
		finally
		{
			Monitor::Exit(ThreadList);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::BeginUpdate
	//////////////////////////////////////////////////////////////////////////////////
	/*
		Will enter a global lock to prevent other threads from simultanously executing code between
		<see cref="LocalHook_Specific::BeginUpdate"/> and <see cref="LocalHook_Specific::EndUpdate"/>. A thread can
		recursively obtain this lock. You have to release the lock in the same thread and you also have
		to release it as often as you have called <see cref="LocalHook_Specific::BeginUpdate"/>.
	*/
	void LocalHook_Specific::BeginUpdate(Boolean InIgnoreWarnings)
	{
		HookUpdateEntry^			CurrentLevel;
		HookUpdateEntry^			ParentLevel;

		Monitor::Enter(HookUpdateEntry::Lock);

		try
		{
			if(HookUpdateEntry::Current != nullptr)
			{
				// start a new sublevel
				ParentLevel = HookUpdateEntry::Current;
			}
			else
			{
				// start the root level
				ParentLevel = nullptr;
			}

			// initialize sublevel
			CurrentLevel = gcnew HookUpdateEntry();
			CurrentLevel->Entries = gcnew List<LocalHook_Specific^>();
			CurrentLevel->Childs = gcnew List<HookUpdateEntry^>();
			CurrentLevel->CanIgnoreWarnings = InIgnoreWarnings;
			CurrentLevel->Prev = ParentLevel;

			// add sublevel to current one...
			if(ParentLevel != nullptr)
			{
				ParentLevel->Childs->Add(CurrentLevel);
			}

			HookUpdateEntry::Current = CurrentLevel;
		}
		catch(Exception^ e)
		{
			Monitor::Exit(HookUpdateEntry::Lock);

			throw e;
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::CancelUpdate
	//////////////////////////////////////////////////////////////////////////////////
	void LocalHook_Specific::CancelUpdate()
	{
		if(!IsUpdating)
			THROW(System::InvalidOperationException, "The library is not in an updating state.");

		HookUpdateEntry^			List = HookUpdateEntry::Current;

		HookUpdateEntry::Current = List->Prev;
		HookUpdateEntry::Current->Childs->Remove(List);

		Monitor::Exit(HookUpdateEntry::Lock);
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::CollectUpdateEntries
	//////////////////////////////////////////////////////////////////////////////////
	void LocalHook_Specific::CollectUpdateEntries(
		Stack<HookUpdateEntry^>^ InTodoList,
		HookUpdateEntry^ InCurrent)
	{
		InTodoList->Push(InCurrent);

		for(int i = 0; i < InCurrent->Childs->Count; i++)
		{
			CollectUpdateEntries(InTodoList, InCurrent->Childs[i]);
		}

		InCurrent->Childs = nullptr;
	}

	//////////////////////////////////////////////////////////////////////////////////
	///////////////// LocalHook_Specific::EndUpdate
	//////////////////////////////////////////////////////////////////////////////////
	void LocalHook_Specific::EndUpdate()
	{
		if(!IsUpdating)
			THROW(System::InvalidOperationException, "The library is not in an updating state.");

		// is there a parent lock level?
		if(HookUpdateEntry::Current->Prev != nullptr)
		{
			/*
				Just leave the current lock level.
			*/
			HookUpdateEntry::Current = HookUpdateEntry::Current->Prev;

			Monitor::Exit(HookUpdateEntry::Lock);
		}

		/*
			Collect all hooks within the update frame...
			This is just to ease installation and has no specific effect.
		*/
		Stack<HookUpdateEntry^>^		TodoList = gcnew Stack<HookUpdateEntry^>();

		CollectUpdateEntries(TodoList, HookUpdateEntry::Current);

		HookUpdateEntry::Current = gcnew HookUpdateEntry();

		try
		{
			/*
				This code is potentially undebuggable... Set DEBUG_HOOK_INSTALLATION
				in "stdafx.h" to make this section debuggable! Otherwise Visual Studio
				will freeze when doing so. 
			*/

			if(!SuspendAllThreads())
				THROW(System::ApplicationException, "Unable to suspend one or more threads.");

			try
			{
				Debugger_Specific::Lock();

				/*
					This section is in general not debugable...
					So dont set any breakpoint here or just disable "SuspendAllThreads" before you do so...
				*/
				Stack<LocalHook_Specific^>^		InstallList = gcnew Stack<LocalHook_Specific^>();

				while(TodoList->Count > 0)
				{
					HookUpdateEntry^		Entry = TodoList->Pop();

					for(int i = 0; i < Entry->Entries->Count; i++)
					{
						LocalHook_Specific^		Data = Entry->Entries[i];

						try
						{
							Install(Entry->CanIgnoreWarnings, Data);
						}
						catch(Exception^ e)
						{
							/*
								Hook couldn't be installed due to warnings. We have to ensure that
								no hook will be installed in this case... So just remove them in reverse
								order!
							*/
							while(InstallList->Count > 0)
							{
								Data = InstallList->Pop();

								BUGCHECK(Data->Destroy(), "Unable to remove hooks after failure. This usually indicates a heavily damaged environment!");
							}

							throw e;
						}

						InstallList->Push(Data);
					}
				}
			}
			finally
			{
				Debugger_Specific::Unlock();

				ResumeAllThreads();
			}
		}
		finally
		{
			HookUpdateEntry::Current = nullptr;

			Monitor::Exit(HookUpdateEntry::Lock);
		}
	}
};