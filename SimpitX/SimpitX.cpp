#include "SimpitX.h"
#include <iostream>

EXTERN_DLL_EXPORT IPC::Server* CreateServer( const std::string connectionName )
{
	// Create a new IPC server object.
	IPC::Server* pServer = nullptr;

	try
	{
		pServer = new IPC::Server();
		pServer->create(connectionName);
	}
	catch(...)
	{
		if (pServer)
		{
			delete pServer;
			pServer = nullptr;
			std::cerr << "Exception thrown during creation of Server(\"" << connectionName << "\"" << std::endl;
		}
	}

	return pServer;
}

EXTERN_DLL_EXPORT IPC::Client* CreateClient( std::string connectionName )
{

	IPC::Client* pClient = nullptr;

	try
	{
		pClient = new IPC::Client(connectionName);
	}
	catch(...)
	{
		// Exception thrown during construction.
		std::cerr << "Exception thrown during creation of Client(\"" << connectionName << "\"" << std::endl;
	}

	return pClient;

}
