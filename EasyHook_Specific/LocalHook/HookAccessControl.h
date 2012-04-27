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
#ifndef _HOOKACCESSCONTROL_H_
#define _HOOKACCESSCONTROL_H_

#include "stdafx.h"

namespace EasyHook
{

	typedef struct _HOOK_ACL_
	{
		DWORD Count;
		BOOL IsExclusive;
		DWORD Entries[MAX_ACE_COUNT];
	}HOOK_ACL;

	#if (MAX_ACE_COUNT > 1024)
		#error "An ACL shall not be larger than one native memory page."
	#endif

	/*///////////////// HookAccessControl

	DESCRIPTION:

		Provides a way to enable or disable threads and processes for hook interception.
		Depending on the configuration this allows a caller to explicitly specify which
		threads or processes should be intercepted by a given hook. This can be done on
		a global level and also on a per-hook level. The latter will adjust the global 
		configuration for a specific hook.
	*/
	public ref class HookAccessControl : IHookAccessControl
	{
	internal:
		cli::array<Int32>^		Entries;
		Boolean					m_IsExclusive;
		HOOK_ACL*				NativeHandle;

		void SetACL(
			Boolean InIsExclusive,
			cli::array<Int32>^ InACL);

		HookAccessControl(
			HOOK_ACL* InACL,
			Boolean InIsExclusive);

	public:
		virtual property Boolean IsExclusive { Boolean get() { return m_IsExclusive; } }
		virtual property Boolean IsInclusive { Boolean get() { return m_IsExclusive; } }

		virtual void SetInclusiveACL(cli::array<Int32>^ InACL) { SetACL(false, InACL); }

		virtual void SetExclusiveACL(cli::array<Int32>^ InACL) { SetACL(true, InACL); }

		virtual cli::array<Int32>^ GetACL();
	};
};

#endif