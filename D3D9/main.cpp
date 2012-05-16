/*	Project : Direct3D StarterKit v3.0
	Author	: Matthew L (Azorbix)
	Date	: June 10th, 2005

	Credits :
		Tetsuo:			Help with hooking the device
		LanceVorgin:	His name is funny
		p47r!ck:		Hes German
		Hybrid:			Knows DirectX9
		Dom1n1k:		Helped with detourfunc
		Da_BuzZ:		Helped alot with d3d8
		Sparten:		Bugfix with GetDirect3D()

		Dockman:		A Game-Deception Head Honcho
		Absolution:		A Game-Deception Head Honcho
		UNI:			The Game-Deception Head Honcho Fries Supreme!
		Game-Deception
	
	Tools used:
		Microsoft Visual Studio .NET 2003
		DirectX9 SDK Update (summer 2004)

	Information:
		This StarterKit was developed for Game-Deception
		visit us at http://www.game-deception.com
*/

#include <windows.h>
#include <fstream>
#include <stdio.h>
#include <time.h>
using namespace std;

#include "main.h"
#include "d3d9.h"
#include "MinHook.h"
#include "misc.h"
#include "HookManager.h"
#include "MFCDX.h"


// Globals



bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved)
{

	if(dwReason == DLL_PROCESS_ATTACH)
	{
		char dlldir[320];

		DisableThreadLibraryCalls(hDll);

		GetModuleFileName(hDll, dlldir, 512);
		for(int i = strlen(dlldir); i > 0; i--) { if(dlldir[i] == '\\') { dlldir[i+1] = 0; break; } }
		
		open_log(dlldir);
		add_log("Log Ready");
		
		add_log("SimpitX™ - D3D9 Hook - ©2012, ZarthCode LLC");
		

		// Create hooks
		try
		{
			D3DHookMgr::GetSingleton()->HookD3D9();
		}
		catch (std::exception ex)
		{
			add_log("Exception thrown during D3DHookMgr::GetSingleton()->HookD3D9()");
			add_log(ex.what());
		}
		add_log("Exiting DllMain.");
		
		return true;
	}

	else if(dwReason == DLL_PROCESS_DETACH)
	{
		add_log("SimpitX™ D3D9Hook Detaching");


		add_log("%lu unique surfaces.", MFCDX::s_RenderSurfaces.size());
		add_log("%lu interesting textures.", MFCDX::s_RenderTextures.size());
		add_log("Current call (#%lu) %s", callID(), callInfo());

		close_log();
		return true;
	}

	return false;
}





