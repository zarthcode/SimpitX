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


	HelperServiceInterface^ RemoteHooking_Specific::WOWBypassInterface::get()
	{
		LOCK(m_ThreadSafe)
		{
			if(m_WOWBypassInterface != nullptr)
				return m_WOWBypassInterface;

			if(m_WOWBypass == nullptr)
				m_WOWBypass = gcnew WOW64Bypass();

			return m_WOWBypass->Interface;
		}
		UNLOCK(m_ThreadSafe);
	}

	void RemoteHooking_Specific::x86ServiceInterface::set(HelperServiceInterface^ InValue)
	{
		if((m_x86Service != nullptr) || (m_x86ServiceInterface != nullptr))
			THROW(System::InvalidOperationException, "There is already an interface associated.");

		m_x86ServiceInterface = InValue;
	}

	void RemoteHooking_Specific::x64ServiceInterface::set(HelperServiceInterface^ InValue)
	{
		if((m_x64Service != nullptr) || (m_x64ServiceInterface != nullptr))
			THROW(System::InvalidOperationException, "There is already an interface associated.");

		m_x64ServiceInterface = InValue;
	}

	void RemoteHooking_Specific::WOWBypassInterface::set(HelperServiceInterface^ InValue)
	{
		if((m_WOWBypass != nullptr) || (m_WOWBypassInterface != nullptr))
			THROW(System::InvalidOperationException, "There is already an interface associated.");

		m_WOWBypassInterface = InValue;
	}

	WOW64Bypass::WOW64Bypass()
	{	
		String^				ChannelName;

		if(Config::Paths == nullptr)
			THROW(System::InvalidOperationException, "You have to call Config.Install() first.");
		
	#ifndef _M_X64
		if(!Config::Has64BitSupport)
			THROW(System::NotSupportedException, "Your license does not contain 64-bit support. Please visit www.easyhook.org for more information.");
	#endif

		HelperService::GenerateServiceName(ChannelName);
		
		Process^				Proc = gcnew Process();
		ProcessStartInfo^		StartInfo = gcnew ProcessStartInfo(
								#ifdef _M_X64
									Config::Paths->EasyHookSvc32,
								#else
									Config::Paths->EasyHookSvc64,
								#endif
									"\"" + Config::typeid->Assembly->Location + "\" " +
									"\"" + ChannelName + "\""
									);
		
		// create sync objects
		EventWaitHandle^		Listening = gcnew EventWaitHandle(false, EventResetMode::ManualReset, "Global\\Event_" + ChannelName);
		
		TermMutex = gcnew Mutex(true, "Global\\Mutex_" + ChannelName);
		
		// start and connect program
		Proc->StartInfo = StartInfo;
		
		Proc->Start();
		
		if(!Listening->WaitOne(5000, true))
			THROW(System::ApplicationException, "Unable to wait for service application due to timeout.");
		
		m_Interface = RemoteHooking_Specific::_IpcConnectClient<HelperServiceInterface^>(ChannelName);
		
		m_Interface->Ping();
	}
};