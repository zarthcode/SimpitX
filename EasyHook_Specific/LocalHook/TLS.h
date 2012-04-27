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
#ifndef _TLS_H_
#define _TLS_H_

#include "stdafx.h"

#pragma unmanaged

/*
	This is only provided to realize the whole HookRuntimeInformation.cpp
	without thread local storage. 
*/
namespace EasyHook
{
	template<class T, int COUNT>
	class ThreadLocalStorage
	{
	private:
		T						Entries[COUNT];
		DWORD					IdList[COUNT];
		CRITICAL_SECTION		ThreadSafe;
	public:

		void Create()
		{
			memset(this, 0, sizeof(ThreadLocalStorage<T, COUNT>));

			InitializeCriticalSection(&ThreadSafe);
		}

		void Destroy()
		{
			DeleteCriticalSection(&ThreadSafe);

			memset(this, 0, sizeof(ThreadLocalStorage<T, COUNT>));
		}

		bool AddCurrentThread()
		{
			EnterCriticalSection(&ThreadSafe);

			DWORD		CurrentId = GetCurrentThreadId();
			int			Index = -1;

			for(int i = 0; i < COUNT; i++)
			{
				if((IdList[i] == 0) && (Index == -1))
					Index = i;
				
				UNMANAGED_BUGCHECK(IdList[i] != CurrentId, "The given thread is already added.");
			}

			if(Index == -1)
			{
				LeaveCriticalSection(&ThreadSafe);

				return false;
			}

			IdList[Index] = CurrentId;
			memset(&Entries[Index], 0, sizeof(T));
			
			LeaveCriticalSection(&ThreadSafe);

			return true;
		}

		bool GetValue(T** OutValue)
		{
			DWORD		CurrentId = GetCurrentThreadId();

			for(int i = 0; i < COUNT; i++)
			{
				if(IdList[i] == CurrentId)
				{
					*OutValue = &Entries[i];

					return true;
				}
			}

			return false;
		}

		void RemoveCurrentThread()
		{

			EnterCriticalSection(&ThreadSafe);

			DWORD		CurrentId = GetCurrentThreadId();

			for(int i = 0; i < COUNT; i++)
			{
				if(IdList[i] == CurrentId)
				{
					IdList[i] = 0;

					memset(&Entries[i], 0, sizeof(T));
				}
			}

			LeaveCriticalSection(&ThreadSafe);
		}
	};
};

#pragma managed

#endif