// file name:	main.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef main_h
#define main_h

//includes
#include<windows.h>
#include<commctrl.h>
#include<d3d9.h>
#include<d3dx9.h>
#include<fstream>

using namespace std;

#include"capplication.h"
#include"d3ddefs.h"

//constants
#define TITLE				"D3D Tut 16: Render-To-Texture"
#define WINDOW_X			350
#define WINDOW_Y			320

//Button ID's
#define ID_START			1
#define ID_CANCEL			2

//globals
extern CApplication			g_App;

//functions
LRESULT CALLBACK			WindowProcedure(HWND,UINT,WPARAM,LPARAM);

#endif