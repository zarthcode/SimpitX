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
#ifndef _FIXEDLIST_H_
#define _FIXEDLIST_H_

#include "stdafx.H"

namespace EasyHook
{
	generic<class T>
	ref class FastStack
	{
	private:
		List<T>^ Entries;
		Int32 m_Count;
	public:

		void Push(T InValue)
		{
			LOCK(Entries)
			{
				if(m_Count < Entries->Count)
				{
					Entries[m_Count++] = InValue;
				}
				else
				{
					Entries->Add(InValue);
				}
			}
			UNLOCK(Entries);
		}

		property Int32 Count { Int32 get() { return m_Count; } }



		T Peek()
		{
			LOCK(Entries)
			{
				if(m_Count == 0)
					THROW(System::InvalidOperationException, "No element to pop.");

				return Entries[m_Count - 1];
			}
			UNLOCK(Entries);
		}

		T Pop()
		{
			T Result;

			LOCK(Entries)
			{
				if(m_Count == 0)
					THROW(System::InvalidOperationException, "No element to pop.");

				Result = Peek();

				m_Count--;
			}
			UNLOCK(Entries);

			return Result;
		}

		property T default[Int32] {
			T get(Int32 InIndex) 
			{
				LOCK(Entries)
				{
					if((InIndex < 0) || (InIndex >= m_Count))
						THROW(System::ArgumentOutOfRangeException, "The given index is not valid.");

					return Entries[InIndex];
				}
				UNLOCK(Entries);
			}
			void set(Int32 InIndex, T InValue) 
			{
				LOCK(Entries)
				{
					if((InIndex < 0) || (InIndex >= m_Count))
						THROW(System::ArgumentOutOfRangeException, "The given index is not valid.");

					Entries[InIndex] = InValue;
				}
				UNLOCK(Entries);
			}
	   }
	};

	ref class FixedHandleList
	{
	private:
		ref class Entry
		{
		public:
			GCHandle^ Handle;

			Entry(GCHandle^ InValue)
			{
				if((InValue == nullptr) || !InValue->IsAllocated)
					THROW(System::ArgumentNullException, "The given GCHandle is not allocated.");

				Handle = InValue;
			}
		};

		List<Entry^>^ Entries;
		List<Int32>^ FreeEntries;
	public:

		FixedHandleList()
		{
			Entries = gcnew List<Entry^>();
			FreeEntries = gcnew List<Int32>();
		}

		Int32 Add(GCHandle^ InValue)
		{
			LOCK(Entries)
			{
				if(FreeEntries->Count > 0)
				{
					Int32 Index = FreeEntries[FreeEntries->Count - 1];

					FreeEntries->RemoveAt(FreeEntries->Count - 1);

					Entries[Index] = gcnew Entry(InValue);

					return Index;
				}
				else
				{
					Entries->Add(gcnew Entry(InValue));

					return Entries->Count - 1;
				}
			}
			UNLOCK(Entries);
		}

		void RemoveAt(Int32 InIndex)
		{
			LOCK(Entries)
			{
				if((InIndex < 0) || (InIndex >= Entries->Count))
					THROW(System::ArgumentOutOfRangeException, "The given index is not valid.");

	#ifdef _DEBUG
				for(int i = 0; i < FreeEntries->Count; i++)
				{
					if(FreeEntries[i] == InIndex)
						THROW(System::InvalidOperationException, "The given element is already removed.");
				}
	#endif
				FreeEntries->Add(InIndex);

				Entries[InIndex] = nullptr;
			}
			UNLOCK(Entries);
		}

		property GCHandle^ default[Int32] {
			GCHandle^ get(Int32 InIndex) 
			{
				LOCK(Entries)
				{
					if((InIndex < 0) || (InIndex >= Entries->Count))
						THROW(System::ArgumentOutOfRangeException, "The given index is not valid.");

		#ifdef _DEBUG
					for(int i = 0; i < FreeEntries->Count; i++)
					{
						if(FreeEntries[i] == InIndex)
							THROW(System::InvalidOperationException, "The given element is already removed.");
					}
		#endif
					return Entries[InIndex]->Handle;
				}
				UNLOCK(Entries);
			}
			void set(Int32 InIndex, GCHandle^ InValue) 
			{
				LOCK(Entries)
				{
					if((InIndex < 0) || (InIndex >= Entries->Count))
						THROW(System::ArgumentOutOfRangeException, "The given index is not valid.");

		#ifdef _DEBUG
					for(int i = 0; i < FreeEntries->Count; i++)
					{
						if(FreeEntries[i] == InIndex)
							THROW(System::InvalidOperationException, "The given element is already removed.");
					}
		#endif
					Entries[InIndex] = gcnew Entry(InValue);
				}
				UNLOCK(Entries);
			}
	   }
	};
}

#endif