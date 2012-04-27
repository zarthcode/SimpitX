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
	HelperServiceInterface^ RemoteHooking_Specific::x86ServiceInterface::get()
	{
#ifdef _M_X64
		THROW(System::ApplicationException, "It is not supported to use a 32-Bit helper service in a 64-Bit environment!");
#endif
		LOCK(m_ThreadSafe)
		{
			if(m_x86ServiceInterface != nullptr)
				return m_x86ServiceInterface;

			if(m_x86Service == nullptr)
				m_x86Service = gcnew HelperService();

			return m_x86Service->Interface;
		}
		UNLOCK(m_ThreadSafe);
	}

	HelperServiceInterface^ RemoteHooking_Specific::x64ServiceInterface::get()
	{
#ifndef _M_X64
		THROW(System::ApplicationException, "It is not supported to use a 64-Bit helper service in a 32-Bit environment!");
#endif

		LOCK(m_ThreadSafe)
		{
			if(m_x64ServiceInterface != nullptr)
				return m_x64ServiceInterface;

			if(m_x64Service == nullptr)
				m_x64Service = gcnew HelperService();

			return m_x64Service->Interface;
		}
		UNLOCK(m_ThreadSafe);
	}

	#undef GetTempPath

	void HelperService::GenerateServiceName(String^% OutName)
	{
		RNGCryptoServiceProvider^ Rnd = gcnew RNGCryptoServiceProvider();
		cli::array<Byte>^ Data = gcnew cli::array<Byte>(30);
		StringBuilder^ Builder = gcnew StringBuilder();

		Rnd->GetBytes(Data);

		for(int i = 0; i < (20 + (Data[0] % 10)); i++)
		{
			Byte b = Data[i] % 62;

			if((b >= 0) && (b <= 9))
				Builder->Append((Char)('0' + b));
			if((b >= 10) && (b <= 35))
				Builder->Append((Char)('A' + (b - 10)));
			if((b >= 36) && (b <= 61))
				Builder->Append((Char)('a' + (b - 36)));
		}

		OutName = Builder->ToString();
	}


	/*
		Will use a system service to upgrade to LOCAL-SYSTEM...
	*/
	HelperService::HelperService()
	{
		if(Config::Paths == nullptr)
			THROW(System::InvalidOperationException, "You have to call Config.Install() first.");

		// check privileges
		if(!RemoteHooking_Specific::_IsAdministrator)
			THROW(System::AccessViolationException, "This is an administrative task. Please contact your system administrator for help.");

		wchar_t*			pServiceName = NULL;
		wchar_t*			pServicePath = NULL;
		SC_HANDLE			hSCManager = NULL;
		SC_HANDLE			hService = NULL;
		String^				AsmDir = Path::GetDirectoryName(Config::Paths->EasyHook);
#ifdef _M_X64
		String^				ServicePath = Config::Paths->EasyHookSvc64;
#else
		String^				ServicePath = Config::Paths->EasyHookSvc32;
#endif	
		String^				ServiceName = Path::GetFileNameWithoutExtension(ServicePath);
		String^				ChannelName;
		Boolean				HasInstalled = false;

		GenerateServiceName(ChannelName);
		/*
			Extract service files...
		*/
		try
		{
			try
			{
				pServiceName = (wchar_t*)Marshal::StringToCoTaskMemUni(ServiceName).ToPointer();
				pServicePath = (wchar_t*)Marshal::StringToCoTaskMemUni(ServicePath).ToPointer();

				if((hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS)) == NULL)
					THROW(System::ApplicationException, "Unable to open service control manager.");


				// does service exist?
				if((hService = OpenService(hSCManager, pServiceName, SERVICE_ALL_ACCESS)) == NULL)
				{
					if(GetLastError() != ERROR_SERVICE_DOES_NOT_EXIST)
						THROW(System::InvalidOperationException, "The service \"{0}\" is already registered.", ServiceName);
				}
				else
				{
					CloseHandle(hService);

					THROW(System::InvalidOperationException, "The service \"{0}\" is already registered.", ServiceName);
				}

				// prepare startup parameters
				wchar_t ParamArray[2][MAX_PATH+1];

				memset(ParamArray, 0, sizeof(ParamArray));

				Marshal::Copy(AsmDir->ToCharArray(), 0, (IntPtr)ParamArray[0], AsmDir->Length);
				Marshal::Copy(ChannelName->ToCharArray(), 0, (IntPtr)ParamArray[1], ChannelName->Length);

				// Create the service
				if((hService = CreateServiceW(
						hSCManager,              
						pServiceName,            
						pServiceName,           
						SERVICE_ALL_ACCESS,        
						SERVICE_WIN32_OWN_PROCESS,
						SERVICE_DEMAND_START,    
						SERVICE_ERROR_NORMAL,     
						pServicePath,            
						NULL, NULL, NULL, NULL, NULL)) == NULL)
					THROW(System::ApplicationException, "Unable to install service as system process.");

				HasInstalled = true;

				// create sync objects
				EventWaitHandle^		Listening = gcnew EventWaitHandle(
					false, 
					EventResetMode::ManualReset, 
					"Global\\Event_" + ChannelName);

				TermMutex = gcnew Mutex(true, "Global\\Mutex_" + ChannelName);

				// start and connect service...
				LPCWSTR		StartParams[2] = {ParamArray[0], ParamArray[1]};

				if(!StartServiceW(hService, 2, (LPCWSTR*)StartParams))
					THROW(System::ApplicationException, "Unable to start service.");

				if(!Listening->WaitOne(5000, true))
					THROW(System::ApplicationException, "Unable to wait for service startup.");

				m_Interface = RemoteHooking_Specific::_IpcConnectClient<HelperServiceInterface^>(ChannelName);

				m_Interface->Ping();
			}
			finally
			{
				if(pServiceName != NULL)
					Marshal::FreeCoTaskMem((IntPtr)pServiceName);

				if(pServicePath != NULL)
					Marshal::FreeCoTaskMem((IntPtr)pServicePath);

				if(hService != NULL)
				{
					if(HasInstalled)
						DeleteService(hService);

					CloseServiceHandle(hService);
				}

				if(hSCManager != NULL)
					CloseServiceHandle(hSCManager);
			}
		}
		catch(Exception^ e)
		{
			// this will terminate the service...
			TermMutex->ReleaseMutex();

			m_Interface = nullptr;
			TermMutex = nullptr;

			throw e;
		}
	}
};