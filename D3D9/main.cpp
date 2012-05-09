/*
	Project : Direct3D StarterKit v3.0
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

//Globals
ofstream ofile;	
char dlldir[320];

bool WINAPI DllMain(HMODULE hDll, DWORD dwReason, PVOID pvReserved)
{

	if(dwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hDll);

		GetModuleFileName(hDll, dlldir, 512);
		for(int i = strlen(dlldir); i > 0; i--) { if(dlldir[i] == '\\') { dlldir[i+1] = 0; break; } }
		ofile.open(GetDirectoryFile("d3d9hook.log"), ios::trunc);

		add_log("\n---------------------\nD3D9Hook Started...\n---------------------");

		MessageBox(NULL, GetDirectoryFile("d3d9hook.log"), "Hook started successfully", MB_OK);

		char sysd3d[320];
		GetSystemDirectory(sysd3d, 320);
		strcat(sysd3d, "\\d3d9.dll");

		add_log("LoadLibrary(\"%s\")", sysd3d);
		HMODULE hMod = LoadLibrary(sysd3d);	
		add_log("\t...result 0x%x", hMod);

		add_log("Registering D3D9 Detour.");
		
		
		
		// oDirect3DCreate9 = (tDirect3DCreate9)DETOUR(GetProcAddress(hMod, "Direct3DCreate9"),	hkDirect3DCreate9, 5);
		// Initialize MinHook.
		if (MH_Initialize() != MH_OK)
		{
			add_log("MinHook Init failed.");
			return false;
		}

		// Create a hook for MessageBoxW, in disabled state.
		if (MH_CreateHook(GetProcAddress(hMod, "Direct3DCreate9"), hkDirect3DCreate9, 
			reinterpret_cast<void**>(&oDirect3DCreate9)) != MH_OK)
		{
			add_log("MinHook Create failed.");
			return false;
		}

		// Enable the hook for MessageBoxW.
		if (MH_EnableHook(GetProcAddress(hMod, "Direct3DCreate9")) != MH_OK)
		{
			add_log("MinHook Enable failed.");
			return false;
		}

		add_log("Detour created (0x%x)", oDirect3DCreate9);
		return true;
	}

	else if(dwReason == DLL_PROCESS_DETACH)
	{
		add_log("---------------------\nD3D9Hook Exiting...\n---------------------\n");
		if(ofile) { ofile.close(); }
	}

	return false;
}


char *GetDirectoryFile(char *filename)
{
	static char path[320];
	strcpy(path, dlldir);
	strcat(path, filename);
	return path;
}

void __cdecl add_log (const char *fmt, ...)
{
	if(ofile != NULL)
	{
		if(!fmt) { return; }

		char buff[20];
		time_t now = time(NULL);
		strftime(buff, 20, "%Y-%m-%d %H:%M:%S", localtime(&now));

		va_list va_alist;
		char logbuf[256] = {0};

		va_start (va_alist, fmt);
		_vsnprintf (logbuf+strlen(logbuf), sizeof(logbuf) - strlen(logbuf), fmt, va_alist);
		va_end (va_alist);

		ofile << buff << " - " << logbuf << endl;
	}
}
