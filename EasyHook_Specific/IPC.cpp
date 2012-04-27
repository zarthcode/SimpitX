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
	/*
		Each IPC channel consists of a channel name, which can be user defined,
		and a provider name for which you can define well known objects. But each
		object can only be bound to one provider and so the prov name has to stay unchanged
		for each channel. 
		EasyHook will managed those provider names internally, so that a user of EasyHook 
		only have to deal with channel names even through application boundaries, what is 
		not common.
	*/

	generic<class TObject> where TObject : MarshalByRefObject
	TObject RemoteHooking_Specific::_IpcConnectClient(
		String^ InChannelName)
	{
		// connect to bypass service
		TObject Interface = (TObject)Activator::GetObject(
			TObject::typeid,
            "ipc://" + InChannelName + "/" + InChannelName);

        if (Interface == nullptr)
			THROW(System::ArgumentException, "Unable to create remote interface.");

		return Interface;
	}

	generic<class TObject> where TObject : MarshalByRefObject
	IpcServerChannel^ RemoteHooking_Specific::_IpcCreateServer(
		String^% RefChannelName,
		WellKnownObjectMode InObjectMode,
		...
		cli::array<WellKnownSidType>^ InAllowedClientSIDs)
	{
		String^				ChannelName = RefChannelName;

		if(ChannelName == nullptr)
		{
			// randomly generate name
			HelperService::GenerateServiceName(ChannelName);
		}

		///////////////////////////////////////////////////////////////////
		// create security descriptor for IpcChannel...
		System::Collections::IDictionary^	Properties = gcnew System::Collections::Hashtable();

		Properties["name"] = ChannelName;
		Properties["portName"] = ChannelName;

		DiscretionaryAcl^					DACL = gcnew DiscretionaryAcl(false, false, 1);

		if(InAllowedClientSIDs->Length == 0)
		{
			if(RefChannelName != nullptr)
				THROW(System::Security::HostProtectionException, "If no random channel name is being used, you shall specify all allowed SIDs.");

			// allow access from all users... Channel is protected by random path name!
			DACL->AddAccess(AccessControlType::Allow, gcnew SecurityIdentifier(WellKnownSidType::WorldSid, nullptr),
				-1, InheritanceFlags::None, PropagationFlags::None);
		}
		else
		{
			for(int i = 0; i < InAllowedClientSIDs->Length; i++)
			{
				DACL->AddAccess(AccessControlType::Allow, gcnew SecurityIdentifier(InAllowedClientSIDs[i], nullptr),
					-1, InheritanceFlags::None, PropagationFlags::None);
			}
		}

		CommonSecurityDescriptor^		SecDescr = gcnew CommonSecurityDescriptor(false, false,
			ControlFlags::GroupDefaulted |
			ControlFlags::OwnerDefaulted |
			ControlFlags::DiscretionaryAclPresent,
			nullptr, nullptr, nullptr, DACL);

		//////////////////////////////////////////////////////////
		// create IpcChannel...
		BinaryServerFormatterSinkProvider^ BinaryProv = gcnew BinaryServerFormatterSinkProvider();
		BinaryProv->TypeFilterLevel = TypeFilterLevel::Full;

		m_HookingChannel = gcnew IpcServerChannel(Properties, BinaryProv, SecDescr);

		ChannelServices::RegisterChannel(m_HookingChannel, false);

		RemotingConfiguration::RegisterWellKnownServiceType(
			TObject::typeid,
			ChannelName,
			InObjectMode);

		RefChannelName = ChannelName;

		return m_HookingChannel;
	}
};