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

	typedef DWORD WINAPI FlsAlloc_PROC(PFLS_CALLBACK_FUNCTION lpCallback);
	typedef PVOID WINAPI FlsGetValue_PROC(DWORD dwFlsIndex);
	typedef BOOL WINAPI FlsSetValue_PROC(DWORD dwFlsIndex, PVOID lpFlsData);
	typedef BOOL WINAPI FlsFree_PROC(DWORD dwFlsIndex);

	typedef struct _RUNTIME_INFO_
	{
		// "true" if the current thread is within the related hook handler
		bool IsExecuting;
		// the hook this information entry belongs to... This allows a per thread and hook storage!
		DWORD HLSIdent;
		// the return address of the current thread's hook handler...
		void* RetAddress;
		// the RSP value before interception...
		void* InitialRSP;
	}RUNTIME_INFO;

	typedef struct _THREAD_RUNTIME_INFO_
	{
		RUNTIME_INFO		Entries[MAX_HOOK_COUNT];
		RUNTIME_INFO*		Current;
		void*				Handle;
		bool				IsProtected;
	}THREAD_RUNTIME_INFO, *LPTHREAD_RUNTIME_INFO;

	bool AcquireSelfProtection();

	static class UNIT
	{
	public:
		HOOK_ACL				GlobalACL;
		HANDLE					RuntimeHeap;
		bool					IsInitialized;

		DWORD					TlsCurrent;
		DWORD					TlsList;
		DWORD					TlsSelfProtect;
		BOOL					HasFiberSupport;
		FlsAlloc_PROC*			FlsAlloc;
		FlsFree_PROC*			FlsFree;
		FlsSetValue_PROC*		FlsSetValue;
		FlsGetValue_PROC*		FlsGetValue;

		void HLSFree(DWORD InIndex)
		{
			if(HasFiberSupport)
				UNIT::FlsFree(InIndex);
			else
				TlsFree(InIndex);
		}

		void* HLSGetValue(DWORD InIndex)
		{
			if(HasFiberSupport)
				return UNIT::FlsGetValue(InIndex);
			else
				return TlsGetValue(InIndex);
		}

		bool HLSSetValue(DWORD InIndex, void* InValue)
		{
			if(HasFiberSupport)
				return UNIT::FlsSetValue(InIndex, InValue);
			else
				return TlsSetValue(InIndex, InValue);
		}

		/*
			Releases related HLS ports on fiber removal...
		*/
		static void __stdcall FlsCallback(void* InList)
		{
			if(InList != NULL)
			{
				HeapFree(Unit.RuntimeHeap, 0, InList);
			}
		}

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
		if((BOOL)Unit.HLSGetValue(Unit.TlsSelfProtect))
			return false;

		return Unit.HLSSetValue(Unit.TlsSelfProtect, (void*)true);
	}

	void ReleaseSelfProtection()
	{
		UNMANAGED_BUGCHECK((BOOL)Unit.HLSGetValue(Unit.TlsSelfProtect), 
			"Attempt to release self protection without acquiring it.");

		Unit.HLSSetValue(Unit.TlsSelfProtect, (void*)false);
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
		LPTHREAD_RUNTIME_INFO Runtime = (LPTHREAD_RUNTIME_INFO)Unit.HLSGetValue(Unit.TlsList);

		if(Runtime != NULL)
			return Runtime->Handle;
		else
			return NULL;
	}

	void* __stdcall GetCurrentReturnAddress()
	{
		LPTHREAD_RUNTIME_INFO Runtime = (LPTHREAD_RUNTIME_INFO)Unit.HLSGetValue(Unit.TlsList);

		if((Runtime != NULL) && (Runtime->Current != NULL))
			return Runtime->Current->RetAddress;
		else
			return NULL;
	}

	void* __stdcall GetInitialRSP()
	{
		LPTHREAD_RUNTIME_INFO Runtime = (LPTHREAD_RUNTIME_INFO)Unit.HLSGetValue(Unit.TlsList);

		if((Runtime != NULL) && (Runtime->Current != NULL))
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
				if(!Unit.HasFiberSupport)
					Unit.FlsCallback(Unit.HLSGetValue(Unit.TlsList));
			}break;
		case DLL_PROCESS_ATTACH:
			{
				memset(&Unit, 0, sizeof(Unit));

				Unit.RuntimeHeap = NULL;

				// globally accept all threads...
				Unit.GlobalACL.IsExclusive = true;

				// allocate private heap
				Unit.RuntimeHeap = HeapCreate(0, 0, 0);

				// check for fiber support
				HMODULE hKernel = GetModuleHandleA("kernel32.dll");

				Unit.HasFiberSupport = (GetProcAddress(hKernel, "FlsAlloc") != NULL);
				Unit.TlsCurrent = TLS_OUT_OF_INDEXES;
				Unit.TlsList = TLS_OUT_OF_INDEXES;
				Unit.TlsSelfProtect = TLS_OUT_OF_INDEXES;

				if(Unit.HasFiberSupport)
				{
					/*
						We have to load them dynamically to use one library for all
						supported windows versions...
					*/
					Unit.FlsAlloc = (FlsAlloc_PROC*)GetProcAddress(hKernel, "FlsAlloc");
					Unit.FlsFree = (FlsFree_PROC*)GetProcAddress(hKernel, "FlsFree");
					Unit.FlsSetValue = (FlsSetValue_PROC*)GetProcAddress(hKernel, "FlsSetValue");
					Unit.FlsGetValue = (FlsGetValue_PROC*)GetProcAddress(hKernel, "FlsGetValue");

					Unit.TlsCurrent = Unit.FlsAlloc(NULL);
					Unit.TlsList = Unit.FlsAlloc(Unit.FlsCallback);
					Unit.TlsSelfProtect = Unit.FlsAlloc(NULL);
				}
				else
				{
					Unit.TlsCurrent = TlsAlloc();
					Unit.TlsList = TlsAlloc();
					Unit.TlsSelfProtect = TlsAlloc();
				}

				Unit.IsInitialized = AuxUlibInitialize() && (Unit.RuntimeHeap != NULL) &&
					(Unit.TlsCurrent != TLS_OUT_OF_INDEXES) && (Unit.TlsList != TLS_OUT_OF_INDEXES) &&
					(Unit.TlsSelfProtect != TLS_OUT_OF_INDEXES);
			}break;

		case DLL_PROCESS_DETACH:
			{
				Unit.HLSFree(Unit.TlsCurrent);
				Unit.HLSFree(Unit.TlsList);
				Unit.HLSFree(Unit.TlsSelfProtect);

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

		// open pointer table
		UNMANAGED_BUGCHECK(InHandle->HLSIndex < MAX_HOOK_COUNT, "Fatal error: More than %d hooks installed.", MAX_HOOK_COUNT);

		THREAD_RUNTIME_INFO*		Info = (THREAD_RUNTIME_INFO*)Unit.HLSGetValue(Unit.TlsList);

		if(Info == NULL)
		{
			Info = (THREAD_RUNTIME_INFO*)HeapAlloc(Unit.RuntimeHeap, HEAP_ZERO_MEMORY, sizeof(THREAD_RUNTIME_INFO));

			if(Info == NULL)
				goto DONT_INTERCEPT;

			if(!Unit.HLSSetValue(Unit.TlsList, Info))
			{
				HeapFree(Unit.RuntimeHeap, 0, Info);

				goto DONT_INTERCEPT;
			}
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
		//Runtime->InitialRSP = InRSP;

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

		LPTHREAD_RUNTIME_INFO	Info = (LPTHREAD_RUNTIME_INFO)Unit.HLSGetValue(Unit.TlsList);

		UNMANAGED_BUGCHECK((Info != NULL), "No thread runtime information available in umanaged outro.");

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