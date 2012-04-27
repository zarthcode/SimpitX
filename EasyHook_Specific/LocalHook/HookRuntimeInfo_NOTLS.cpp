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
#include "TLS.h"

/*
	I am using a >>well known<< library to check for OS loader lock ;-)
*/
#include "Aux_ulib.h"

#ifndef _M_X64
	#pragma comment (lib, "Aux_ulib_x86.lib")
#else
	#pragma comment (lib, "Aux_ulib_x64.lib")
#endif

#pragma comment (lib, "user32.lib")

/*
	This file provides most of the convenience you will get with EasyHook,
	like "Fiber deadlock barrier", hook handler runtime information, as well
	as thread based ACLs for hook interception.

	If I am speaking about "fibers" I am also speaking about "threads" if
	fiber local storage is not supported or the target application does not
	use fibers. In both cases each thread is threaded as a fiber in particular.

	Please note that there is a reason why no dynamic memory allocation is being used.
	For hook entry and outro we have no fiber deadlock barrier and so we should
	only call very well known APIs and memory management is definitely out of this scope!
	It is also a burden to track down possible memory issues at this low level. So we
	just avoid it at all and as this library is OpenSource you may adjust the static
	allocation to suite your needs!
	Commercial users of the 64-bit version may also request binary files with custom
	build flag values (free of charge).
*/
namespace EasyHook
{
	/*
		Returns the managed hook handle associated with the current fiber.
		This will throw an InvalidOperationException if called outside a hook handler.
	*/
	ILocalHook^ HookRuntimeInfo_Specific::Handle::get()
	{
		return LocalHook_Specific::Current; 
	}

	/*
		Will return "true" if the current fiber is within any hook handler or one of
		its subcalls, "false" otherwise.
	*/
	Boolean HookRuntimeInfo_Specific::IsHandlerContext::get()
	{ 
		return LocalHook_Specific::IsHandlerContext;
	}

	/*
		Returns the managed hook specific user callback associated with the current fiber.
		This will throw an InvalidOperationException if called outside a hook handler.
	*/
	Object^	HookRuntimeInfo_Specific::Callback::get() 
	{ 
		return LocalHook_Specific::Current->m_Callback; 
	} 

	void* __stdcall GetCurrentReturnAddress();

	IntPtr HookRuntimeInfo_Specific::ReturnAddress::get() 
	{ 
		LocalHook_Specific^ Current = LocalHook_Specific::Current;

		return (IntPtr)GetCurrentReturnAddress(); 
	}

	void* __stdcall GetInitialRSP();

	IntPtr HookRuntimeInfo_Specific::InitialRSP::get() 
	{ 
		LocalHook_Specific^ Current = LocalHook_Specific::Current;

		return (IntPtr)GetInitialRSP(); 
	} 

#pragma managed(off)


	typedef struct _RUNTIME_INFO_
	{
		// "true" if the current thread is within the related hook handler
		bool IsExecuting;
		DWORD HLSIdent;
		// the return address of the current thread's hook handler...
		void* RetAddress;
		void* InitialRSP;
	}RUNTIME_INFO;

	typedef struct _THREAD_RUNTIME_INFO_
	{
		RUNTIME_INFO*		Entries;
		RUNTIME_INFO*		Current;
		void*				Handle;
		bool				IsProtected;
	}THREAD_RUNTIME_INFO, *LPTHREAD_RUNTIME_INFO;

	bool AcquireSelfProtection();

	static class UNIT
	{
	public:
		BOOL					HasFiberSupport;
		HOOK_ACL				GlobalACL;
		HANDLE					RuntimeHeap;
		bool					IsInitialized;

		ThreadLocalStorage<THREAD_RUNTIME_INFO, MAX_THREAD_COUNT>		TLS;

		/*
			Returns "true" if the current thread is holding the OS loader lock which is
			the case in DllMain for example...
		*/
		bool IsLoaderLock()
		{
			BOOL IsLoaderLock = false;

			return (!AuxUlibIsDLLSynchronizationHeld(&IsLoaderLock) || IsLoaderLock || !IsInitialized);
		}
	}Unit;

	bool AcquireSelfProtection()
	{
		LPTHREAD_RUNTIME_INFO		Runtime = NULL;

		if(!Unit.TLS.GetValue(&Runtime) || Runtime->IsProtected)
			return false;

		Runtime->IsProtected = true;

		return true;
	}

	void ReleaseSelfProtection()
	{
		LPTHREAD_RUNTIME_INFO		Runtime = NULL;

		UNMANAGED_BUGCHECK(Unit.TLS.GetValue(&Runtime) && Runtime->IsProtected, "Attempt to release self protection without acquiring it.");

		Runtime->IsProtected = false;
	}

	/*
		Is thread ID in ACL?
	*/
	bool ACLContains(
		HOOK_ACL* InACL,
		DWORD InThreadID)
	{
		for(int i = 0; i < InACL->Count; i++)
		{
			if(InACL->Entries[i] == InThreadID)
				return true;
		}

		return false;
	}

	/*
		Should thread be intercepted by this hook? Please note that in case of
		fibers access is still evaluated on a per thread basis. That is even if
		you only grant one thread access, multiple fibers may invoke your code,
		so don't assume that your code is not executed simultanously even if only
		one thread can access it.

		Please refer to the public documentation for more information!
		But it works similarily to windows ACLs...
	*/
	bool __stdcall IsThreadIntercepted(
		HOOK_ACL* LocalACL, 
		DWORD InThreadID)
	{
		if(InThreadID == 0)
			InThreadID = GetCurrentThreadId();

		if(ACLContains(&Unit.GlobalACL, InThreadID))
		{
			if(ACLContains(LocalACL, InThreadID))
			{
				if(LocalACL->IsExclusive)
					return false;
			}
			else
			{
				if(Unit.GlobalACL.IsExclusive)
					return false;

				if(!LocalACL->IsExclusive)
					return false;
			}
		}
		else
		{
			if(ACLContains(LocalACL, InThreadID))
			{
				if(LocalACL->IsExclusive)
					return false;
			}
			else
			{
				if(!Unit.GlobalACL.IsExclusive)
					return false;

				if(!LocalACL->IsExclusive)
					return false;
			}
		}

		return true;
	}

	/*
		Provides an easy way for managed code to update the native global ACL.
	*/
	void __stdcall SetGlobalACL(
		DWORD* InACL,
		DWORD InACECount,
		BOOL InIsExclusive)
	{
		UNMANAGED_BUGCHECK(InACECount <= MAX_ACE_COUNT, "Too many ACEs.");
		
		Unit.GlobalACL.Count = InACECount;
		Unit.GlobalACL.IsExclusive = InIsExclusive;

		memcpy(Unit.GlobalACL.Entries, InACL, InACECount * sizeof(DWORD));
	}

	/*
		Returns a weak GCHandle to the underlying managed hook object.
		Use GCHandle::FromIntPtr() to assign it... Don't forget to check
		for NULL, because this is an invalid value and will crash the application
		if used as GCHandle!
	*/
	void* __stdcall GetCurrentHandle()
	{
		LPTHREAD_RUNTIME_INFO Runtime;

		Unit.TLS.GetValue(&Runtime);

		return Runtime->Handle;
	}

	void* __stdcall GetCurrentReturnAddress()
	{
		LPTHREAD_RUNTIME_INFO Runtime;

		Unit.TLS.GetValue(&Runtime);

		if(Runtime->Current != NULL)
			return Runtime->Current->RetAddress;
		else
			return NULL;
	}

	void* __stdcall GetInitialRSP()
	{
		LPTHREAD_RUNTIME_INFO Runtime;

		Unit.TLS.GetValue(&Runtime);

		if(Runtime->Current != NULL)
			return Runtime->Current->InitialRSP;
		else
			return NULL;
	}

	bool __stdcall IsRuntimeInitialized()
	{
		return Unit.IsInitialized;
	}

#ifdef _M_X64
	extern "C" BOOL __stdcall DllMain(
#else
	BOOL WINAPI DllMain(
#endif
		HINSTANCE hinstDLL,
		DWORD fdwReason,
		LPVOID lpvReserved)
	{

		switch(fdwReason)
		{
		case DLL_THREAD_DETACH:
			{
				LPTHREAD_RUNTIME_INFO		Info;

				if(Unit.TLS.GetValue(&Info))
				{
					if(Info->Entries != NULL)
						HeapFree(Unit.RuntimeHeap, 0, Info->Entries);

					Info->Entries = NULL;
				}

				Unit.TLS.RemoveCurrentThread();
			}break;
		case DLL_PROCESS_ATTACH:
			{
				memset(&Unit, 0, sizeof(Unit));

				Unit.RuntimeHeap = NULL;

				// globally accept all threads...
				Unit.GlobalACL.IsExclusive = true;

				// allocate private heap
				Unit.RuntimeHeap = HeapCreate(0, 0, 0);

				Unit.TLS.Create();

				Unit.IsInitialized = AuxUlibInitialize() && (Unit.RuntimeHeap != NULL);

			}break;

		case DLL_PROCESS_DETACH:
			{
				Unit.TLS.Destroy();

				if(Unit.RuntimeHeap != NULL)
					HeapDestroy(Unit.RuntimeHeap);

				memset(&Unit, 0, sizeof(Unit));
			}break;
		}

		return TRUE;
	}

	

	/*
		The unmanaged intro and outro are very specific and we can't call any managed code.
		Even if I think we should be able to call managed code after once the deadlock barrier has
		been created; a pratical test has shown, that I am wrong ;-). The application simply
		hangs or terminates if we are calling managed code within intro or outro. For the
		handler it works well of course :-)... But I have no clue why, it's like with the
		Windows 2000 Memory Management; nobody knows why it works but everybody is happy that it does...

		As we go the way through fiber local storage we will get support for nearly all
		kinds of applications (IIS or MSSQL) and future NET versions by creating a
		fiber based deadlock barrier. If fibers are used and we are not using FLS, the
		deadlock barrier would go insane. 
	*/
	size_t __stdcall UnmanagedHookIntro(HOOK_HANDLE_DATA* InHandle, void* InRetAddr, void* InRSP)
	{
		#ifdef _M_X64
			InHandle -= 1;
		#endif

		// are we in OS loader lock?
		if(Unit.IsLoaderLock())
		{
			/*
				Execution of managed code or even any other code within any loader lock
				may lead into unpredictable application behavior and therefore we just
				execute without intercepting the call...
			*/

			/*  !!Note that the assembler code does not invoke UnmanagedHookOutro() in this case!! */

			return false;
		}

		// open pointer table
		LPTHREAD_RUNTIME_INFO		Info;
		BOOL						Exists = Unit.TLS.GetValue(&Info);

		if(!Exists)
		{
			if(!Unit.TLS.AddCurrentThread())
				return false;
		}

		/*
			To minimize APIs that can't be hooked, we are now entering the self protection.
			This will allow anybody to hook any APIs except those required to setup
			self protection.

			Self protection prevents any further hook interception for the current fiber,
			while setting up the "Fiber Deadlock Barrier"...
		*/
		if(!AcquireSelfProtection())
		{
			/*  !!Note that the assembler code does not invoke UnmanagedHookOutro() in this case!! */

			return false;
		}

		UNMANAGED_BUGCHECK(InHandle->HLSIndex < MAX_HOOK_COUNT, "Fatal error: More than %d hooks installed.", MAX_HOOK_COUNT);

		if(!Exists)
		{
			Unit.TLS.GetValue(&Info);

			Info->Entries = (RUNTIME_INFO*)HeapAlloc(Unit.RuntimeHeap, HEAP_ZERO_MEMORY, sizeof(RUNTIME_INFO) * MAX_HOOK_COUNT);

			if(Info->Entries == NULL)
				goto DONT_INTERCEPT;
		}

		// get hook runtime info...
		RUNTIME_INFO*		Runtime = &Info->Entries[InHandle->HLSIndex];

		if(Runtime->HLSIdent != InHandle->HLSIdent)
		{
			// just reset execution information
			Runtime->HLSIdent = InHandle->HLSIdent;
			Runtime->IsExecuting = false;
		}

		// detect loops in hook execution hiearchy.
		if(Runtime->IsExecuting)
		{
			/*
				This implies that actually the handler has invoked itself. Because of
				the special HookLocalStorage, this is now also signaled if other
				hooks invoked by the related handler are calling it again.

				I call this the "Fiber deadlock barrier".

				!!Note that the assembler code does not invoke UnmanagedHookOutro() in this case!!
			*/

			goto DONT_INTERCEPT;
		}

		Info->Handle = InHandle->hThis;
		Info->Current = Runtime;

		/*
			Now we will negotiate thread access based on global and local ACL...
			As there is no way to determine a fiber ID like "GetCurrentFiberId()" 
			we will always negotiate access based on a per thread basis.
		*/
		Runtime->IsExecuting = IsThreadIntercepted(&InHandle->LocalACL, GetCurrentThreadId());

		if(!Runtime->IsExecuting)
			goto DONT_INTERCEPT;

		// save some context specific information
		Runtime->RetAddress = InRetAddr;
		Runtime->InitialRSP = InRSP;

		ReleaseSelfProtection();
		
		return true;

DONT_INTERCEPT:
		/*  !!Note that the assembler code does not invoke UnmanagedHookOutro() in this case!! */

		if(Info != NULL)
		{
			Info->Current = NULL;
			Info->Handle = NULL;

			ReleaseSelfProtection();
		}

		return false;
	}

	/*
		Will just reset the "fiber deadlock barrier" for the current hook handler and provides
		some important integrity checks. 

		The hook handle is just passed through, because the assembler code has no chance to
		save it in any efficient manner at this point of execution...
	*/
	void* __stdcall UnmanagedHookOutro(HOOK_HANDLE_DATA* InHandle, void** InAddrOfRetAddr)
	{
		#ifdef _M_X64
			InHandle -= 1;
		#endif

		UNMANAGED_BUGCHECK(AcquireSelfProtection(), "Unable to self protect hook outro.")

		LPTHREAD_RUNTIME_INFO	Info;

		Unit.TLS.GetValue(&Info);

		UNMANAGED_BUGCHECK(Unit.TLS.GetValue(&Info) && (Info != NULL), "No thread runtime information available in umanaged outro.");

		RUNTIME_INFO*			Runtime = &Info->Entries[InHandle->HLSIndex];
	
		// leave handler context
		Info->Current = NULL;
		Info->Handle = NULL;

		UNMANAGED_BUGCHECK(Runtime != NULL, "The hook was removed while its handler is being executed.");

		UNMANAGED_BUGCHECK(Runtime->IsExecuting, "A NET outro was called on a hook that is currently not being executed.");

		Runtime->IsExecuting = false;

		UNMANAGED_BUGCHECK(!IsBadWritePtr(InAddrOfRetAddr, sizeof(void*)), "The address of return address is invalid. This usually indicates a stack corruption.");

		UNMANAGED_BUGCHECK(*InAddrOfRetAddr == NULL, "Space for return address contains an invalid value. This usually indicates a stack corruption.");

		*InAddrOfRetAddr = Runtime->RetAddress;

		ReleaseSelfProtection();

		return InHandle;

	}

	

};