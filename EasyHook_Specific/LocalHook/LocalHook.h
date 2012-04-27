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
#ifndef _LOCALHOOKDATA_H_
#define _LOCALHOOKDATA_H_

#include "stdafx.h"

namespace EasyHook
{

	///////////////////////////////////////////////////////////////////////////////
	////////////////////// EasyHook::HookUpdateEntry
	/// <summary>
	///		Used by <see cref="LocalHook_Specific::EndUpdate"/>, <see cref="LocalHook_Specific::EndUpdate"/> and
	///		<see cref="LocalHook_Specific::EndUpdate"/> to realize lock levels.
	/// </summary>
	ref class HookUpdateEntry
	{
	internal:
		///	<summary>
		///		As far as there can be only one thread per time executing an update frame,
		///		this represents the current one.
		///	</summary>
		static HookUpdateEntry^			Current = nullptr;
		/// <summary> Used to serialize access to the update frame. </summary>
		static Object^					Lock = gcnew Object();
	public:
		///	<summary> All sublevels of the current level. </summary>
		List<HookUpdateEntry^>^			Childs;
		///	<summary> A list of hooks collected within this lock level. </summary>
		List<LocalHook_Specific^>^			Entries;
		/// <summary> The previous lock level. </summary>
		HookUpdateEntry^				Prev;
		/// <summary> Does the current lock level ignore warnings? </summary>
		Boolean							CanIgnoreWarnings;
	};

	typedef struct _HOOK_HANDLE_DATA_
	{
		Int32					HLSIndex;
		Int32					HLSIdent;
		void*					hThis;
		HOOK_ACL				LocalACL;

		void*					RandomValue; // fixed
		void*					UnmanagedIntro; // fixed
		BYTE*					OldProc; // fixed
		BYTE*					NewProc; // fixed
		void*					UnmanagedOutro; // fixed
		size_t*					IsExecutedPtr; // fixed
	}HOOK_HANDLE_DATA, *HOOK_HANDLE;


	public ref class LocalHook_Specific : CriticalFinalizerObject, ILocalHook
	{
	internal:
		/// <summary> Will hold all information about a suspended thread during <see cref="LocalHook_Specific::EndUpdate"/>. </summary>
		ref class ThreadEntry : CriticalFinalizerObject
		{
		public:
			/// <summary> The suspend recursion of the given thread. </summary>
			Int32						SuspendCount;
			/// <summary> The current RIP of the suspended thread. </summary>
			Int64						RIP;
			/// <summary> A handle for the suspended thread. </summary>
			HANDLE						Handle;
			Int32						ID;

			~ThreadEntry() { this->!ThreadEntry(); }
			!ThreadEntry() 
			{
				if(Handle != NULL)
					CloseHandle(Handle);

				Handle = NULL;
			}
		};

		static Int32					AreThreadsSuspended = 0;
		static Int32					UniqueIDCounter = 0x10000000;
		/// <summary> Will hold a list of all suspended threads during <see cref="LocalHook_Specific::EndUpdate"/>. </summary>
		static List<ThreadEntry^>^		ThreadList = gcnew List<ThreadEntry^>();
		/// <summary> Is the library currently within an updating frame? </summary>
		static property Boolean			IsUpdating { Boolean get() { return HookUpdateEntry::Current != nullptr; } }
		/// <summary> A list holding all hooks waiting for removal... </summary>
		static List<LocalHook_Specific^>^		GlobalRemovalList = gcnew List<LocalHook_Specific^>();
		/// <summary> The global access control list used by <see cref="LocalHook_Specific::IsThreadIntercepted"/> to evaluate access. </summary>
		static HookAccessControl^		m_GlobalThreadACL = gcnew HookAccessControl(NULL, true);

		static cli::array<Int32>^		GlobalSlotList = gcnew cli::array<Int32>(MAX_HOOK_COUNT);

		/// <summary> Will hold the runtime hooking information for the current thread and is only available within a hooking handler. </summary>
		static HookRuntimeInfo_Specific^			RuntimeInfo;

		static Timer^					CleanupTimer = gcnew Timer(gcnew TimerCallback(OnCleanup), nullptr, 30000, 30000);

		static void OnCleanup(Object^ InCallback);

		static StringBuilder^			Journal = gcnew StringBuilder();

	internal:

		// Represents the native structure of a hook.
		HOOK_HANDLE_DATA*			Handle;
		// The hook specific local thread ACL.
		HookAccessControl^			m_ThreadACL;
		// The native hook handle size.
		Int32						NativeSize;
		// The native hook handle size.
		BYTE*						TargetProc;
		// A copy of the initial entry point. 
		BYTE*						EntryBackup;
		// A copy of the modified entry point.
		BYTE*						EntryCopy;
		// The native target entry point size rounded up to next instruction boundary.
		Int32						EntrySize;
		// Pointer to the hook trampoline code. 
		BYTE*						Trampoline;
		// A weak handle to this instance. 
		GCHandle^					hThis;
		// a user defined callback
		Object^						m_Callback;

		/// <summary>  
		///		Is used by <see cref="LocalHook_Specific::Create"/> and <see cref="LocalHook_Specific::CreateUnmanaged"/> to store
		///		installation information. All other fields are filled by <see cref="Install"/> only.
		/// </summary>
		ref struct INSTALL_INFO
		{
			/// <summary> A validated method pointer to the entry point that will be hooked. </summary>
			IntPtr					TargetProc;
			/// <summary> A validated delegate representing a managed hook handler. Used only by <see cref="LocalHook_Specific::Create"/>. </summary>
			Delegate^				NewProcDelegate;
			/// <summary> A validated method pointer either user supplied or the unmanaged function pointer of the delegate. </summary>
			IntPtr					NewProc;
		}InstallInfo;

		// refer to definition for documentation
		static bool IsThreadSuspended(HANDLE hInThread);

		// refer to definition for documentation
		static Int64 ThreadGetCurrentRIP(HANDLE InThread);

		///////////////////// LocalHook_Specific::GetInstructionLength
		/// <summary>
		/// 	Determines the length of an instruction at the given code pointer.
		/// 	This will be done using either the microsoft debugging engine or
		/// 	the internal ILDs.
		/// </summary>
		static Int32 GetInstructionLength(void* InPtr);

		///////////////////// LocalHook_Specific::HookRoundToNextInstruction
		/// <summary>
		///		Rounds the given size to the next instruction boundary. For example if you have a piece of code
		///		and the first three instructions are eight bytes long in total and the last instruction starts
		///		at the fourth byte, then this method will return eight, if you pass a value greater than three
		///		but less than nine to it.
		/// </summary>
		static Int32 HookRoundToNextInstruction(
				void* InCodePtr,
				Int32 InCodeSize);

		///////////////////// LocalHook_Specific::RelocateEntryPoint
		/// <summary>
		///		Relocates a given pice of code to be executed at <paramref name="Buffer"/>. Relative 
		///		instructions will be converted to be absolute.
		/// </summary>
		static bool RelocateEntryPoint(
					BYTE* InEntryPoint,
					Int32 InEPSize,
					BYTE* Buffer,
					Int32* OutRelocSize);

		///////////////////// LocalHook_Specific::AllocateMemory
		/// <summary>
		///		This will allocate memory at an address that is reachable through a jumper at
		/// 	the given entry point. The memory is always one page large and at least 4 KB.
		/// </summary>
		static void* AllocateMemory(void* InEntryPoint, Int32% OutSize);

		///////////////////// LocalHook_Specific::ReleaseMemory
		/// <summary>
		///		Releases the memory allocated with <see cref="LocalHook_Specific::AllocateMemory"/>. 
		/// </summary>
		static void ReleaseMemory(void* InMemory);

		///////////////////// LocalHook_Specific::IsCodeRegionWriteable
		/// <summary>
		///		This will ensure that none of the suspended threads will be resumed within the
		///		given code section.
		/// </summary>
		static Boolean IsCodeRegionWriteable(void* InPtr, Int32 InSize);

		//////////////////////////////////////////////////////////////////////////////
		////////////////////// LocalHook_Specific::Install
		static void Install(
			Boolean InCanIgnoreWarnings,
			LocalHook_Specific^ InData);

		//////////////////////////////////////////////////////////////////////////////
		////////////////////// LocalHook_Specific::SuspendAllThreads
		///	<summary>
		///		Will increment the suspend count of all threads of the current process.
		///	</summary>
		static bool SuspendAllThreads();

		//////////////////////////////////////////////////////////////////////////////
		////////////////////// LocalHook_Specific::ResumeAllThreads
		///	<summary>
		///		Will decrement the suspend count of all threads previously suspended
		///		calling SuspendAllThreads(). 
		///	</summary>
		static void ResumeAllThreads();

		//////////////////////////////////////////////////////////////////////////////
		////////////////////// LocalHook_Specific::Destroy
		///	<summary>
		///		Detects if the underlying hook can be removed. If this is the case
		///		it will restore the entry point and release all unmanaged resources.
		///		In all cases the user defined handler will be released.
		///		This method is intended to be used in <see cref="LocalHookList::OnCleanup"/>
		///		only.
		///	</summary>
		bool Destroy();


		//////////////////////////////////////////////////////////////////////////////
		////////////////////// LocalHook_Specific::CollectUpdateEntries
		///	<summary>
		///		Recursively collects all update entries of the current update frame
		///		and adds them to the given TODO-List. It also removes the level
		///		linkage information. After this the update frame is subject to 
		///		be cleaned up.
		///	</summary>
		static void CollectUpdateEntries(
			Stack<HookUpdateEntry^>^ InTodoList,
			HookUpdateEntry^ InCurrent);

		static property LocalHook_Specific^	Current { LocalHook_Specific^ get(); }
		property static bool IsHandlerContext{ bool get(); }
		property Object^ Callback {Object^ get(){return m_Callback;} void set(Object^ InValue){m_Callback = InValue;}}

	public:		

		static ILocalHook^ Constructor(){ return gcnew LocalHook_Specific(); }

		virtual /*static*/String^ QueryJournal() { return Journal->ToString(); }

		virtual property IHookAccessControl^	ThreadACL 
		{ 
			IHookAccessControl^ get() 
			{ 
				if(!IsInstalled)
					THROW(System::InvalidOperationException, "The underlying hook is not installed or already disposed.");

				return m_ThreadACL; 
			} 
		}

		virtual property Boolean IsInstalled { Boolean get() { return ((Handle != NULL) && (Handle->NewProc != NULL)); } }

		virtual /*static*/property IHookAccessControl^	GlobalThreadACL { IHookAccessControl^ get() { return m_GlobalThreadACL; } }

		virtual /*static*/bool IsProtectedEntryPoint(IntPtr InEntryPoint);

		// refer to definition for documentation
		~LocalHook_Specific();

		// refer to definition for documentation
		!LocalHook_Specific();

		virtual /*static*/void BeginUpdate(Boolean InIgnoreWarnings);

		/*
			Will leave the current lock level entered in <see cref="LocalHook_Specific::BeginUpdate"/>.
			If this is the final call within the current update frame, also all collected
			will be installed.
		*/
		virtual /*static*/void EndUpdate();

		/*
			Clears the internal temporary list of hooks to install.
			A call to CancelUpdate() only applies to the current update level and does not
			affect parent ones. This allows you to group
			hook creation in subroutines and only cancel hooks added within such a subroutine
			and recursively its subroutines, if something goes wrong. 
		*/
		virtual /*static*/void CancelUpdate();

		/*
			Will install a managed hook. Such a hook supports <see cref="HookRuntimeInfo_Specific"/> to be used
			within its handler and uses the global and local thread ACLs to evaluate interception.
		*/
		virtual /*static*/ILocalHook^ Create(
			IntPtr InTargetProc,
			Delegate^ InNewProc,
			Object^ InCallback);

		/*
			Will install an unmanaged hook. <see cref="LocalHook_Specific::Create"/> is just a wrapper around
			this method so you will get all the convenience you would get with Create().
		*/
		virtual /*static*/ILocalHook^ CreateUnmanaged(
			IntPtr InTargetProc,
			IntPtr InNewProc);

		/*
			Will return the address for a given DLL export symbol. The specified
			module has to be loaded into the current process space and also export
			the given method.
		*/
		virtual /*static*/IntPtr GetProcAddress(
			String^ InModule,
			String^ InSymbolName);

		/*
			Will return a delegate for a given DLL export symbol. The specified
			module has to be loaded into the current process space and also export
			the given method.
		*/
		generic<typename T> 
		virtual /*static*/T GetProcDelegate(
				String^ InModule,
				String^ InSymbolName);


		/*
			Will check if the given thread ID will be intercepted by the hook.
		*/
		virtual bool IsThreadIntercepted(Int32 InThreadID);

		/*
			Raises a garbage collection which will release all orphaned hooks.
			After this the method tries to remove them. If all hook have been
			removed, "true" will be returned, "false" otherwise.
		*/
		virtual /*static*/bool Release();
	};
};

#endif