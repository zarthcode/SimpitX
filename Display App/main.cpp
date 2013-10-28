// file name:	main.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#include"main.h"
#include <iostream>
#include <string>
#include <boost/tokenizer.hpp>
#include <vector>

//globals
CApplication	g_App;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{
	// Process the command line arguments
	std::string cmdLine(lpcmdline);
	boost::tokenizer<> tok(cmdLine);
	std::vector<std::string> vCmdLine;

	for (boost::tokenizer<>::iterator beg=tok.begin(); beg != tok.end(); ++beg)
	{
		vCmdLine.push_back(*beg);
	}

	// Configuration XML
	

		// 



	// Connect to the texture stream



	// Create the system tray icon


	// Enter Program loop
	while (!g_App.Exit())
	{
		// Initialize Wait for a simulator to connect


		// Startup direct3d windows


		// Enter D3D display loop


		// Destroy D3D 

	}


	// Exit application
	return 0;

}