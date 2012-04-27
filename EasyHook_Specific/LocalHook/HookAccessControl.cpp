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
	void __stdcall SetGlobalACL(
		DWORD* InACL,
		DWORD InACECount,
		BOOL InIsExclusive);

	bool __stdcall IsThreadIntercepted(
		HOOK_ACL* LocalACL, 
		DWORD InThreadID);

	HookAccessControl::HookAccessControl(
		HOOK_ACL* InACL,
		Boolean InIsExclusive)
	{
		NativeHandle = InACL;

		// defaultly no thread or process will be accepted... so every hook starts virtually disabled
		SetACL(InIsExclusive, nullptr);
	}

	/*/////////////////// HookAccessControl::SetACL

		It is assumed that only managed code will modify an ACL.
	*/
	void HookAccessControl::SetACL(Boolean InIsExclusive, cli::array<Int32>^ InACL)	
	{

		LOCK(this)
		{
			// import ACL
			cli::array<Int32>^			NewList;
			DWORD						NativeBuffer[MAX_ACE_COUNT];

			if(InACL == nullptr)
				NewList = gcnew cli::array<Int32>(0);
			else
				NewList = (cli::array<Int32>^)InACL->Clone();

			if(NewList->Length > MAX_ACE_COUNT)
				THROW(System::InvalidOperationException, "Too many access control entries (limit is {0}).", MAX_ACE_COUNT);


			for(int i = 0; i < NewList->Length; i++)
			{
				if(NewList[i] == 0)
					NewList[i] = GetCurrentThreadId();

				NativeBuffer[i] = NewList[i];
			}

			m_IsExclusive = InIsExclusive;
		    Entries = NewList;

			/*
				Now we just update the underlying native ACL...
			*/
			if(NativeHandle == NULL)
			{
				SetGlobalACL(NativeBuffer, NewList->Length, InIsExclusive);
			}
			else
			{
				NativeHandle->Count = NewList->Length;
				NativeHandle->IsExclusive = InIsExclusive;

				memcpy(NativeHandle->Entries, NativeBuffer, NewList->Length * sizeof(DWORD));
			}
		}
		UNLOCK(this);
	}

	/// <summary>
	///	A copy of the internal ACL is returned a recreated on each call!
	///	</summary>
	cli::array<Int32>^ HookAccessControl::GetACL()
	{
		return (cli::array<Int32>^)Entries->Clone();
	}

	bool LocalHook_Specific::IsThreadIntercepted(Int32 InThreadID)
	{
		if(!IsInstalled)
			THROW(System::InvalidOperationException, "The underlying hook is not installed or already disposed.");

		return EasyHook::IsThreadIntercepted(&Handle->LocalACL, InThreadID);
	}
};