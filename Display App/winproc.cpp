// file name:	main.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#include"main.h"

/************************************************************************/
/* Name:		WindowProcedure											*/
/* Description:	handles windows messages								*/
/************************************************************************/

LRESULT CALLBACK WindowProcedure(HWND hWindow,UINT uMessage,WPARAM wparam,LPARAM lparam)
{
	switch(uMessage)
	{
		//user command on window
	case WM_COMMAND:
		{
			switch(LOWORD(wparam))
			{
				//start button pressed
			case ID_START:
				{
					//switch to D3D loop and init D3D with settings
					g_App.SaveSettings();
					g_App.SetWindowStatus(false);
					g_App.SetD3DStatus(true);
					SetFocus(hWindow);
					g_App.InitD3D();
					break;
				}
				//cancel button pressed
			case ID_CANCEL:
				{
					DestroyWindow(hWindow);
					break;
				}
			}
			return 0;
		}
	case WM_KEYDOWN:
		{
			switch(wparam)
			{
			case VK_ESCAPE:	
				{
					DestroyWindow(hWindow);
					break;
				}
			}
			return 0;
		}
	case WM_DESTROY:
		{
			g_App.SetWindowStatus(false);
			g_App.SetD3DStatus(false);
			break;
		}
	}

	return DefWindowProc(hWindow,uMessage,wparam,lparam);
}//WindowProcedure