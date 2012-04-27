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
#ifndef _HOOKRUNTIMEINFO_H_
#define _HOOKRUNTIMEINFO_H_

#include "stdafx.h"

namespace EasyHook
{
	///	<summary>
	///		This class provides access to the internal hook local storage API of EasyHook.
	///	</summary>
	public ref class HookRuntimeInfo_Specific : IHookRuntimeInfo
	{
	public:
		static HookRuntimeInfo_Specific^ Constructor(){ return gcnew HookRuntimeInfo_Specific(); }

		virtual /*static*/ property Boolean				IsHandlerContext { Boolean get(); }
		virtual /*static*/ property IntPtr				ReturnAddress { IntPtr get(); }
		virtual /*static*/ property IntPtr				InitialRSP { IntPtr get(); }
		virtual /*static*/ property Object^				Callback { Object^ get(); }
		virtual /*static*/ property ILocalHook^			Handle { ILocalHook^ get(); }
	};
};

#endif