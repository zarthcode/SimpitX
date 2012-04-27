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
	extern BYTE* GetTrampolinePtr();
	extern Int32 GetTrampolineSize();
	extern size_t __stdcall UnmanagedHookIntro(HOOK_HANDLE_DATA* InHandle, void* InRetAddr, void* InRSP);
	extern void* __stdcall UnmanagedHookOutro(HOOK_HANDLE_DATA* InHandle, void** InAddrOfRetAddr);
	extern void* __stdcall GetCurrentHandle();

	/*
		Just to make sure that we never exceed allocation boundaries
		during unmanaged hook installation... This should of course
		never occur and is not raisable through user input but only
		through wrong code in this case!
	*/
	ref struct ProctectedMemory
	{
	private:
		IntPtr		m_Private;
		IntPtr		MaxValue;
		IntPtr		MinValue;
	public:

		property BYTE* Native
		{
			BYTE* get() { return (BYTE*)m_Private.ToPointer(); } 
			void set(BYTE* InValue)
			{
				Value = (IntPtr)InValue;
			}
		}

		property IntPtr Value 
		{ 
			IntPtr get() { return m_Private; } 
			void set(IntPtr InValue)
			{
				if((InValue.ToInt64() < MinValue.ToInt64()) || (InValue.ToInt64() > MaxValue.ToInt64()))
					THROW(System::ApplicationException, "Buffer overflow in unmanaged code parts detected.");

				m_Private = InValue;
			}
		}

		ProctectedMemory(
			IntPtr InStartValue,
			IntPtr InMinValue,
			IntPtr InMaxValue)
		{
			MaxValue = InMaxValue;
			MinValue = InMinValue;

			Value = InStartValue;
		}
	};


	// this will finally install a hook...
	void LocalHook_Specific::Install(
		Boolean InCanIgnoreWarnings, 
		LocalHook_Specific^ Result)
	{
		HOOK_HANDLE_DATA*			Handle = NULL;

		LOCK(GlobalRemovalList)
		{
			if(!IsUpdating)
				THROW(System::InvalidOperationException, "Thread is not in updating state.");

			// reserve unmanaged memory
			Int32				AllocatedSize;

			Handle = (HOOK_HANDLE_DATA*)LocalHook_Specific::AllocateMemory(Result->InstallInfo.TargetProc.ToPointer(), AllocatedSize);

			memset(Handle, 0, sizeof(HOOK_HANDLE_DATA));

			ProctectedMemory^	MemPtr = gcnew ProctectedMemory(
				(IntPtr)(BYTE*)(Handle + 1),
				(IntPtr)(BYTE*)(Handle + 1),
				(IntPtr)(((BYTE*)Handle) + AllocatedSize));

			// determine entry point size
			Int32			EntrySize = HookRoundToNextInstruction(Result->InstallInfo.TargetProc.ToPointer(), 5);

			/*
				Create a proper log entry... This will allow me to see why things are going wrong
				if someone got a problem with hooking.
			*/
			try
			{
				StringBuilder^	Entry = gcnew StringBuilder();

				Entry->AppendLine(  "Installing hook...");
				Entry->AppendFormat("    Allocation:  0x{0:X16}\r\n", (Int64)Handle);
				Entry->AppendFormat("    Entry point: 0x{0:X16} ({1})\r\n", Result->InstallInfo.TargetProc.ToInt64(), EntrySize);
				Entry->AppendLine(  "    DISASM-Pre:");

				if(Debugger_Specific::CanUse)
					Entry->AppendLine(Debugger_Specific::_Disassemble(Result->InstallInfo.TargetProc, EntrySize, 8));
				else
					Entry->AppendLine("        (not available)");

				Journal->Append(Entry->ToString());

#ifdef _DEBUG
				Console::Write("{0}", Entry->ToString());
#endif
			}
			catch(Exception^ e)
			{
				Journal->AppendLine("Unable to add hook installation journal due to exception of type \"" + e->GetType()->FullName + "\"...");
			}

			// create and initialize hook handle
			Handle->RandomValue = (void*)0x69FAB738962376EF;
			Handle->NewProc = (BYTE*)Result->InstallInfo.NewProc.ToPointer();
			Handle->IsExecutedPtr = (size_t*)((BYTE*)Handle + 2048);
			*Handle->IsExecutedPtr = false;

			Result->NativeSize = sizeof(HOOK_HANDLE_DATA);
			Result->TargetProc = (BYTE*)Result->InstallInfo.TargetProc.ToPointer();
			Result->EntrySize = EntrySize;	

			Result->hThis = GCHandle::Alloc(Result, GCHandleType::WeakTrackResurrection);

			/*
				The following will be called by the trampoline before the user defined handler is invoked.
				It will setup a proper environment for the hook handler which includes the "fiber deadlock barrier"
				and user specific callback.
			*/
			Handle->UnmanagedIntro = UnmanagedHookIntro;
			Handle->UnmanagedOutro = UnmanagedHookOutro;
			Handle->hThis = GCHandle::ToIntPtr(*Result->hThis).ToPointer();

			/*
				Ensure that no thread is within the entry point. Here is still the chance
				that a thread will resume execution within a subroutine that jumps or
				returns into the entry point. Such cases are in general not catchable.
			*/
			if(!IsCodeRegionWriteable(Result->InstallInfo.TargetProc.ToPointer(), EntrySize))
				THROW(System::InvalidProgramException, "At least one thread is hanging within the specified entry point.");

			// copy trampoline
			Result->Trampoline = MemPtr->Native; 
			MemPtr->Native += GetTrampolineSize();
			Result->NativeSize += GetTrampolineSize();

			memcpy(Result->Trampoline, GetTrampolinePtr(), GetTrampolineSize());

			/*
				Relocate entry point (the same for both archs)
				Has to be written directly into the target buffer, because to
				relocate RIP-relative addressing we need to know where the
				instruction will go to...
			*/
			Int32			RelocSize = 0;
			
			Handle->OldProc = MemPtr->Native; 

			memset(Handle->OldProc, 0, 100);

			if(!RelocateEntryPoint(Result->TargetProc, EntrySize, Handle->OldProc, &RelocSize) && !InCanIgnoreWarnings)
				THROW(System::ExecutionEngineException, "The given entry point is not recommended to be hooked.");

			MemPtr->Native += RelocSize + 5;
			Result->NativeSize += RelocSize + 5;

			// add jumper to relocated entry point that will proceed execution in original method
			BYTE*			pCode = Handle->OldProc;
			Int32			ShortAddr = (Int32)((ULONG64)(Result->TargetProc + EntrySize) - ((ULONG64)Handle->OldProc + RelocSize + 5));
			DWORD			OldProtect;

			pCode[RelocSize] = 0xE9;

			memcpy(pCode + RelocSize + 1, &ShortAddr, 4);

			// backup original entry point
			Result->EntryBackup = MemPtr->Native; 
			MemPtr->Native += EntrySize;
			Result->NativeSize += EntrySize;

			memcpy(Result->EntryBackup, Result->TargetProc, EntrySize);

#ifndef _M_X64
		
			/*
				Replace absolute placeholders with proper addresses...
			*/
			BYTE*			Ptr = Result->Trampoline;

			for(int i = 0; i < GetTrampolineSize(); i++)
			{
				switch(*((Int32*)(Ptr)))
				{
				/*Handle*/			case 0x1A2B3C05: *((Int32*)Ptr) = (Int32)Handle; break;
				/*UnmanagedIntro*/	case 0x1A2B3C03: *((Int32*)Ptr) = (Int32)Handle->UnmanagedIntro; break;
				/*OldProc*/			case 0x1A2B3C01: *((Int32*)Ptr) = (Int32)Handle->OldProc; break;
				/*Ptr:NewProc*/		case 0x1A2B3C07: *((Int32*)Ptr) = (Int32)&Handle->NewProc; break;
				/*NewProc*/			case 0x1A2B3C00: *((Int32*)Ptr) = (Int32)Handle->NewProc; break;
				/*UnmanagedOutro*/	case 0x1A2B3C06: *((Int32*)Ptr) = (Int32)Handle->UnmanagedOutro; break;
				/*IsExecuted*/		case 0x1A2B3C02: *((Int32*)Ptr) = (Int32)Handle->IsExecutedPtr; break;
				/*RetAddr*/			case 0x1A2B3C04: *((Int32*)Ptr) = (Int32)(Result->Trampoline + 92); break;
				}

				Ptr++;
			}

#endif
			/*
				Set a base ACL which will defaultly disable the hook.
			*/
			Result->m_ThreadACL = gcnew HookAccessControl(&Handle->LocalACL, false);

			// register in global HLS list
			Boolean Exists = false;

			Handle->HLSIdent = UniqueIDCounter++;

			for(int i = 0; i < MAX_HOOK_COUNT; i++)
			{
				if(GlobalSlotList[i] == 0)
				{
					GlobalSlotList[i] = Handle->HLSIdent;

					Handle->HLSIndex = i;

					Exists = true;

					break;
				}
			}

			if(!Exists)
				THROW(System::InvalidOperationException, "Limit of {0} hooks reached.", MAX_HOOK_COUNT);

			// install hook (the same on both archs)
			BYTE			Jumper[8] = {0xE9, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
			ULONG64			RelAddr = (ULONG64)Result->Trampoline - ((ULONG64)Result->TargetProc + 5);

			memcpy(Jumper + 1, &RelAddr, 4);

			if(!VirtualProtect(Result->TargetProc, 5, PAGE_EXECUTE_READWRITE, &OldProtect))
				THROW(System::ApplicationException, "Unable to make code space memory writeable.");

			// from now on the unrecoverable code section starts...
			try
			{
				memcpy(Result->TargetProc, Jumper, 5);

				// backup entry point for later comparsion
				Result->EntryCopy = MemPtr->Native; 
				MemPtr->Native += EntrySize;
				Result->NativeSize += EntrySize;

				memcpy(Result->EntryCopy, Result->TargetProc, Result->EntrySize);

				// from now on the managed class will care about unmanaged memory...
				Result->Handle = Handle;
			}
			catch(...)
			{
				BUGCHECK(false, "Unrecoverable internal error has occurred.");
			}
		}
		catch(Exception^ e)
		{
			if(Handle != NULL)
			{
				// cleanup resources in case of failure
				for(int i = 0; i < MAX_HOOK_COUNT; i++)
				{
					if(LocalHook_Specific::GlobalSlotList[i] == Handle->HLSIdent)
					{
						LocalHook_Specific::GlobalSlotList[i] = 0;

						break;
					}
				}

				LocalHook_Specific::ReleaseMemory(Handle);
			}

			throw e;
		}
		UNLOCK(GlobalRemovalList);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::IsProtectedEntryPoint
	/////////////////////////////////////////////////////////////////////////////////
	/*
		There are some entry points that are required to setup the "fiber deadlock barrier".
		Such entry points can't be used for hooking because they would lead into deadlocks
		unless the FDB is setup which is indeed not possible if you hook a method required
		to set it up.
	*/
	bool LocalHook_Specific::IsProtectedEntryPoint(IntPtr InEntryPoint)
	{
		Int64 e = InEntryPoint.ToInt64();
		HMODULE hKernel = GetModuleHandleA("kernel32.dll");

		if(		(e == (Int64)::GetProcAddress(hKernel, "TlsSetValue")) ||
				(e == (Int64)::GetProcAddress(hKernel, "FlsSetValue")) ||
				(e == (Int64)::GetProcAddress(hKernel, "TlsGetValue")) ||
				(e == (Int64)::GetProcAddress(hKernel, "FlsGetValue")))
			return true;

		return false;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::CreateUnmanaged
	/////////////////////////////////////////////////////////////////////////////////
	ILocalHook^ LocalHook_Specific::CreateUnmanaged(
			IntPtr InTargetProc,
			IntPtr InNewProc)
	{
		if(!IsUpdating)
			THROW(System::InvalidOperationException, "Context is not in an updating state.");

		if(IsProtectedEntryPoint(InTargetProc))
			THROW(System::NotSupportedException, "The given entry point is used internally and therefore can't be intercepted!");
			
		LocalHook_Specific^		Result = gcnew LocalHook_Specific();

		if(IsBadCodePtr((FARPROC)InTargetProc.ToPointer()))
			THROW(System::ArgumentException, "The given target method does not point to executable memory.");

		if(IsBadCodePtr((FARPROC)InNewProc.ToPointer()))
			THROW(System::ArgumentException, "A valid hook handler shall be given.");

		Result->InstallInfo.TargetProc = InTargetProc;
		Result->InstallInfo.NewProcDelegate = nullptr;
		Result->InstallInfo.NewProc = InNewProc;

		HookUpdateEntry::Current->Entries->Add(Result);

		return Result;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::Create
	/////////////////////////////////////////////////////////////////////////////////
	ILocalHook^ LocalHook_Specific::Create(
				IntPtr InTargetProc,
				Delegate^ InNewProc,
				Object^ InCallback)
	{
		if(!IsUpdating)
			THROW(System::InvalidOperationException, "Context is not in an updating state.");

		if(IsProtectedEntryPoint(InTargetProc))
			THROW(System::NotSupportedException, "The given entry point is used internally and therefore can't be intercepted!");

		LocalHook_Specific^		Result = gcnew LocalHook_Specific();

		if(IsBadCodePtr((FARPROC)InTargetProc.ToPointer()))
			THROW(System::ArgumentException, "The given target method does not point to valid binary code.");

		if(InNewProc == nullptr)
			THROW(System::ArgumentException, "A valid hook handler shall be given.");

		Result->InstallInfo.TargetProc = InTargetProc;
		Result->InstallInfo.NewProcDelegate = InNewProc;
		Result->InstallInfo.NewProc = Marshal::GetFunctionPointerForDelegate(InNewProc);
		Result->m_Callback = InCallback;

		HookUpdateEntry::Current->Entries->Add(Result);

		return Result;
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::Destroy
	/////////////////////////////////////////////////////////////////////////////////
	bool LocalHook_Specific::Destroy()
	{
		LOCK(GlobalRemovalList)
		{	
			if(Handle == NULL)
				THROW(System::InvalidOperationException, "Hook has already been removed!");

			// in all cases we have to ensure that the user handler is never called again...
			Handle->NewProc = NULL;

			// no other thread is allowed to currently be in any code portion associated with this hook...
			if(!IsCodeRegionWriteable(Handle, NativeSize) || // is a thread within the entry point?
				!IsCodeRegionWriteable(TargetProc, EntrySize) || // is a thread within the trampoline?
				(*Handle->IsExecutedPtr > 0) || // is the handler currently being executed?
				(memcmp(TargetProc, EntryCopy, EntrySize) != 0) // same entry point?
				)
			{
				return false;
			}
			else
			{
				// restore entry point
				memcpy(TargetProc, EntryBackup, EntrySize);

				// release unmanaged memory...
				LocalHook_Specific::ReleaseMemory(Handle);

				Handle = NULL;

				GlobalRemovalList->Remove(this);

				return true;
			}
		}
		UNLOCK(GlobalRemovalList);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::Dispose
	/////////////////////////////////////////////////////////////////////////////////
	LocalHook_Specific::~LocalHook_Specific() 
	{ 
		LOCK(GlobalRemovalList)
		{
			this->!LocalHook_Specific(); 
		}
		UNLOCK(GlobalRemovalList);
	}

	/////////////////////////////////////////////////////////////////////////////////
	//////////////////////// LocalHook_Specific::Finalize
	/////////////////////////////////////////////////////////////////////////////////
	LocalHook_Specific::!LocalHook_Specific()
	{
		bool IsShutdown = Environment::HasShutdownStarted;

		/*
			Ensure that related managed code is never called again...
		*/
		if(Handle != NULL)
		{
			Handle->NewProc = NULL;

			if(IsShutdown)
			{
				/*
					Wait until all threads are out of managed handlers...
					This may fail in some very rare circumstances, where a thread
					hangs within a handler while the CLR is shutting down and the
					CLR aborts our finalizer before an external thread has left 
					all managed code. 

					When injecting libraries using RemoteHooking, a cleanup
					will automatically be done before CLR shutdown!
				*/
				while(*Handle->IsExecutedPtr > 0)
				{
					Thread::Sleep(10);
				}
			}
			else
			{
				Boolean			Exists = false;

				for(int i = 0; i < MAX_HOOK_COUNT; i++)
				{
					if(LocalHook_Specific::GlobalSlotList[i] == Handle->HLSIdent)
					{
						LocalHook_Specific::GlobalSlotList[i] = 0;

						Exists = true;

						break;
					}
				}

				BUGCHECK(Exists, "The hook has already been removed.");

				LocalHook_Specific::GlobalRemovalList->Add(this);
			}
		}

		// release weak handles...
		if((hThis != nullptr) && (hThis->IsAllocated))
			hThis->Free();

		hThis = nullptr;
	}

	bool LocalHook_Specific::Release()
	{
		/*
			This method will only cleanup hooks that are not referenced by 3th party code...
		*/
		OnCleanup(nullptr);

		return (GlobalRemovalList->Count == 0);
	}

	void LocalHook_Specific::OnCleanup(Object^ InCallback)
	{
		GC::Collect();
		GC::WaitForPendingFinalizers();
		GC::Collect();

		LOCK(GlobalRemovalList)
		{
			cli::array<LocalHook_Specific^>^			List = GlobalRemovalList->ToArray();

			if(!LocalHook_Specific::SuspendAllThreads())
				return;

			try
			{
				// try to remove hooks...
				for(int i = 0; i < List->Length; i++)
				{
					List[i]->Destroy();
				}
			}
			finally
			{
				LocalHook_Specific::ResumeAllThreads();
			}
		}
		UNLOCK(GlobalRemovalList);
	}

	bool LocalHook_Specific::IsHandlerContext::get()
	{
		return GetCurrentHandle() != NULL;
	}

	LocalHook_Specific^ LocalHook_Specific::Current::get()
	{
		if(!IsHandlerContext)
			THROW(System::InvalidOperationException, "It is not supported to query this property outside a hook handler.");

		GCHandle Handle = GCHandle::FromIntPtr((IntPtr)GetCurrentHandle());

		return (LocalHook_Specific^)Handle.Target; 
	}

};