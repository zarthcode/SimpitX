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
	#ifdef _M_X64
		extern "C" int __stdcall GetInstructionLength_x64(void* InPtr, int InType);
	#else
		extern "C" int __stdcall GetInstructionLength_x86(void* InPtr, int InType);
	#endif

	typedef BOOL __stdcall MPROC(DWORD InValue);

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::GetProcAddress
	///////////////////////////////////////////////////////////////////////////////////
	IntPtr LocalHook_Specific::GetProcAddress(
			String^ InModule,
			String^ InSymbolName)
	{
		wchar_t*		pMod = NULL;
		char*			pSymbol = NULL;
		
		try
		{
			pMod = (wchar_t*)Marshal::StringToCoTaskMemUni(InModule).ToPointer();
			pSymbol = (char*)Marshal::StringToCoTaskMemAnsi(InSymbolName).ToPointer();

			if((pMod == NULL) || (pSymbol == NULL))
				THROW(System::OutOfMemoryException, "Not enough unmanaged memory available to complete the call.");

			HMODULE			hMod = GetModuleHandleW(pMod);

			if(hMod == NULL)
				THROW(System::DllNotFoundException, "The given module was not loaded into the current process. It makes no sense to hook any of its methods!");

			void*			hSymbol = ::GetProcAddress(hMod, pSymbol);

			if(hSymbol == NULL)
				THROW(System::MissingMethodException, "The given method \"{1}\" couldn't be found in \"{0}\"", InModule, InSymbolName);

			return (IntPtr)hSymbol;
		}
		finally
		{
			if(pMod != NULL)
				Marshal::FreeCoTaskMem((IntPtr)pMod);

			if(pSymbol != NULL)
				Marshal::FreeCoTaskMem((IntPtr)pSymbol);
		}
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::GetProcDelegate
	///////////////////////////////////////////////////////////////////////////////////
	generic<typename T>
	T LocalHook_Specific::GetProcDelegate(
			String^ InModule,
			String^ InSymbolName)
	{
		IntPtr			pSymbol = GetProcAddress(InModule, InSymbolName);

		return (T)Marshal::GetDelegateForFunctionPointer(pSymbol, T::typeid);
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::GetInstructionLength
	///////////////////////////////////////////////////////////////////////////////////
	Int32 LocalHook_Specific::GetInstructionLength(void* InPtr)
	{
		Int32			Length = -1;
		Boolean			HasLength = false;

		if(IsBadCodePtr((FARPROC)InPtr))
			THROW(System::ApplicationException, "The given pointer does not map to a valid machine code sequence.");

		if(Debugger_Specific::CanUse)
		{
			try
			{
				Length = (Int32)(Debugger_Specific::GetNearInstruction((Int64)InPtr, 1) - (Int64)InPtr);

				HasLength = true;
			}
			catch(...){}
		}
		
		if(!HasLength)
		{
			// might return wrong results for exotic instructions, leading to unknown application behavior...
		#ifdef _M_X64
			Length = GetInstructionLength_x64(InPtr, 64);
		#else
			Length = GetInstructionLength_x86(InPtr, 0);
		#endif
		}

		if(Length > 0)
			return Length;	
		else
			THROW(System::ArgumentException, "The given pointer does map to an invalid machine code instruction.");
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::HookRoundToNextInstruction
	///////////////////////////////////////////////////////////////////////////////////
	Int32 LocalHook_Specific::HookRoundToNextInstruction(
				void* InCodePtr,
				Int32 InCodeSize)
	{
		BYTE*				Ptr = (BYTE*)InCodePtr;
		BYTE*				BasePtr = Ptr;

		if(IsBadCodePtr((FARPROC)InCodePtr))
			THROW(System::ApplicationException, "The given code pointer 0x{0:X16} is invalid.", (Int64)InCodePtr);

		while(BasePtr + InCodeSize > Ptr)
		{
			int Len = GetInstructionLength(Ptr);

			Ptr += Len;
		}

		return (Int32)(Ptr - BasePtr);
	}


	/*////////////////////// GetTrampolineSize

	DESCRIPTION:

		Will dynamically detect the size in bytes of the assembler code stored
		in "HookSpecifix_Xxx.asm".
	*/
	static Int32 ___TrampolineSize = 0;

	#ifdef _M_X64
		extern "C" void __stdcall Trampoline_ASM_x64();
	#else
		extern "C" void __stdcall Trampoline_ASM_x86();
	#endif

	BYTE* GetTrampolinePtr()
	{
	// bypass possible Visual Studio debug jump table
	#ifdef _M_X64
		BYTE* Ptr = (BYTE*)Trampoline_ASM_x64;
	#else
		BYTE* Ptr = (BYTE*)Trampoline_ASM_x86;
	#endif

		if(*Ptr == 0xE9)
			Ptr += *((int*)(Ptr + 1)) + 5;

	#ifdef _M_X64
		return Ptr + 5 * 8;
	#else
		return Ptr;
	#endif
	}

	Int32 GetTrampolineSize()
	{
		if(___TrampolineSize != 0)
			return ___TrampolineSize;
		
		// search for signature
		BYTE*		Ptr = GetTrampolinePtr();
		BYTE*		BasePtr = Ptr;

		for(int i = 0; i < 2000 /* some always large enough value*/; i++)
		{
			Int32 Signature = *((Int32*)Ptr);

			if(Signature == 0x12345678)	
			{
				___TrampolineSize = (Int32)(Ptr - BasePtr);

				return ___TrampolineSize;
			}

			Ptr++;
		}

		THROW(System::ApplicationException, "Unable to determine trampoline size.");
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::RelocateEntryPoint
	///////////////////////////////////////////////////////////////////////////////////
	bool LocalHook_Specific::RelocateEntryPoint(
					BYTE* InEntryPoint,
					Int32 InEPSize,
					BYTE* Buffer,
					Int32* OutRelocSize)
	{
	#ifdef _M_X64
		#define POINTER_TYPE __int64
	#elif defined(_M_IX86)
		#define POINTER_TYPE __int32
	#endif
		
		BYTE*				pRes = Buffer;
		BYTE*				pOld = InEntryPoint;

		BUGCHECK(InEPSize < 20, "The entry point is too large.");

		while(pOld < InEntryPoint + InEPSize)
		{
			BYTE			b1 = *(pOld);
			BYTE			b2 = *(pOld + 1);
			Int32			OpcodeLen = 0;
			POINTER_TYPE	AbsAddr = 0;
			BOOLEAN			a16 = false;
			BOOLEAN			IsRIPRelative = false;

			// check for prefixes
			switch(b1)
			{
			case 0x67: a16 = true; continue;
			}

			/////////////////////////////////////////////////////////
			// get relative address value
			switch(b1)
			{
				case 0xE9: // jmp imm16/imm32
				{
					/* only allowed as first instruction and only if the trampoline can be planted 
					   within a 32-bit boundary around the original entrypoint. So the jumper will 
					   be only 5 bytes and whereever the underlying code returns it will always
					   be in a solid state. But this can only be guaranteed if the jump is the first
					   instruction... */
					if(pOld != InEntryPoint)
						THROW(System::NotSupportedException, "The entry point contains a jump that is not the first instruction.");
					
					// ATTENTION: will continue in "case 0xE8"
				}
			case 0xE8: // call imm16/imm32
				{
					if(a16)
					{
						AbsAddr = *((__int16*)(pOld + 1));
						OpcodeLen = 3;
					}
					else
					{
						AbsAddr = *((__int32*)(pOld + 1));
						OpcodeLen = 5;
					}
				}break;

			/*
				The problem with (conditional) jumps is that there will be no return into the relocated entry point.
				So the execution will be proceeded in the original method and this will cause the whole
				application to remain in an unstable state. Only near jumps with 32-bit offset are allowed as
				first instruction (see above)...
			*/
			case 0xEB: // jmp imm8
			case 0xE3: // jcxz imm8
				{
					THROW(System::NotSupportedException, "The entry point contains short (conditional) jumps.");
				}break;
			case 0x0F:
				{
					if((b2 & 0xF0) == 0x80) // jcc imm16/imm32
						THROW(System::NotSupportedException, "The entry point contains conditional jumps.");
				}break;
			}

			if((b1 & 0xF0) == 0x70) // jcc imm8
				THROW(System::NotSupportedException, "The entry point contains short conditional jumps.");

			/////////////////////////////////////////////////////////
			// convert to: mov eax, AbsAddr

			if(OpcodeLen > 0)
			{
				AbsAddr += (POINTER_TYPE)(pOld + OpcodeLen);

	#ifdef _M_X64
				*(pRes++) = 0x48; // REX.W-Prefix
	#endif
				*(pRes++) = 0xB8;

				*((POINTER_TYPE*)pRes) = AbsAddr;

				pRes += sizeof(POINTER_TYPE);

				// points into entry point?
				if((AbsAddr >= (__int64)InEntryPoint) && (AbsAddr < (__int64)InEntryPoint + InEPSize))
					/* is not really unhookable but not worth the effort... */
					THROW(System::NotSupportedException, "The entry point contains jumps or calls into itself.");

				/////////////////////////////////////////////////////////
				// insert alternate code
				switch(b1)
				{
				case 0xE8: // call eax
					{
						*(pRes++) = 0xFF;
						*(pRes++) = 0xD0;
					}break;
				case 0xE9: // jmp eax
					{
						*(pRes++) = 0xFF;
						*(pRes++) = 0xE0;
					}break;
				}

				/* such conversions shouldnt be necessary in general...
				   maybe the method was already hooked or uses some hook protection or is just
				   bad programmed. In all cases a hook shouldnt be placed here if the user doesnt
				   know exactly what he is doing. EasyHook is capable of hooking the same method
				   many times simultanously. Even if other (unknown) hook libraries are hooking methods that
				   are already hooked by EasyHook. Only if EasyHook hooks methods that are already
				   hooked with other libraries there can be problems if the other libraries are not
				   capable of such a "bad" circumstance.
				*/

				*OutRelocSize = (Int32)(pRes - Buffer);

				return false;
			}
			else
			{
				if(Debugger_Specific::CanUse)
				{
					try
					{
				#ifdef _M_X64
						/* This will recognize and relocate RIP-relative addressing using a hacky and short approach :-) */ 
						IsRIPRelative = Debugger_Specific::RelocateRIPRelative(
							(Int64)pOld,
							(Int64)pRes);

				#endif
					}
					catch(...)
					{
						IsRIPRelative = false;
					}
				}
			}

			// find next instruction
			int InstrLen = GetInstructionLength(pOld);

			if(OpcodeLen == 0)
			{
				// just copy the instruction
				if(!IsRIPRelative)
					memcpy(pRes, pOld, InstrLen);

				pRes += InstrLen;
			}

			pOld += InstrLen;
			IsRIPRelative = false;
		}

		*OutRelocSize = (Int32)(pRes - Buffer);

		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::AllocateMemory
	///////////////////////////////////////////////////////////////////////////////////
	void* LocalHook_Specific::AllocateMemory(void* InEntryPoint, Int32% OutSize)
	{
		BYTE*			Res = NULL;

		// get page size
		SYSTEM_INFO		SysInfo;

		GetSystemInfo(&SysInfo);

#if (MAX_ACE_COUNT > 128)
		SysInfo.dwPageSize *= 2;
#endif

		// reserve page with execution privileges
#ifdef _M_X64
		Int64			iStart = ((Int64)InEntryPoint) - ((Int64)0x7FFFFF00);
		Int64			iEnd = ((Int64)InEntryPoint) + ((Int64)0x7FFFFF00);

		if(iStart < (Int64)SysInfo.lpMinimumApplicationAddress)
			iStart = (Int64)SysInfo.lpMinimumApplicationAddress; // shall not be null, because then VirtualAlloc() will not work as expected

		if(iEnd > (Int64)SysInfo.lpMaximumApplicationAddress)
			iEnd = (Int64)SysInfo.lpMaximumApplicationAddress;

		// we are trying to get memory as near as possible to relocate most RIP-relative addressings
		for(Int64 i = (iEnd - iStart) / 2; i < iEnd; i += SysInfo.dwPageSize)
		{
			if((Res = (BYTE*)VirtualAlloc((void*)i, SysInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) != NULL)
				break;
		}

		if(Res == NULL)
		{
			for(Int64 i = (iEnd - iStart) / 2; i > iStart; i -= SysInfo.dwPageSize)
			{
				if((Res = (BYTE*)VirtualAlloc((void*)i, SysInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE)) != NULL)
					break;
			}

			if(Res == NULL)
				THROW(System::OutOfMemoryException, "Unable to allocate unmanaged trampoline.");
		}

#else
		// in 32-bit mode the trampoline will always be reachable
		Res = (BYTE*)VirtualAlloc(NULL, SysInfo.dwPageSize, MEM_COMMIT, PAGE_EXECUTE_READWRITE);

		if(Res == NULL)
		{
			Res = (BYTE*)VirtualAlloc(NULL, SysInfo.dwPageSize, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);

			if(Res == NULL)
				THROW(System::OutOfMemoryException, "Unable to allocate unmanaged memory for trampoline.");
		}
#endif

		RtlZeroMemory(Res, SysInfo.dwPageSize);

		OutSize = SysInfo.dwPageSize;

		return Res;
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::ReleaseMemory
	///////////////////////////////////////////////////////////////////////////////////
	void LocalHook_Specific::ReleaseMemory(void* InMemory)
	{
		if(InMemory == NULL)
			return;

		if(!VirtualFree(InMemory, 0, MEM_RELEASE))
			THROW(System::ApplicationException, "Unable to release unmanaged virtual memory.");

		InMemory = NULL;
	}

	///////////////////////////////////////////////////////////////////////////////////
	/////////////////////// LocalHook_Specific::IsCodeRegionWriteable
	///////////////////////////////////////////////////////////////////////////////////
	Boolean LocalHook_Specific::IsCodeRegionWriteable(void* InPtr, Int32 InSize)
	{
		if(!AreThreadsSuspended)
			return false;

		if(IsBadCodePtr((FARPROC)InPtr))
			THROW(System::ApplicationException, "An invalid code pointer was specified.");

		// traverse thread list
		for(int i = 0; i < ThreadList->Count; i++)
		{
			ThreadEntry^		Entry = ThreadList[i];

			if(((BYTE*)Entry->RIP >= (BYTE*)InPtr) && ((BYTE*)Entry->RIP <= (BYTE*)InPtr + InSize) )
				return false;
		}	

		return true;
	}
};