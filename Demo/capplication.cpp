// file name:	capplication.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#include"main.h"

/************************************************************************/
/* Name:		CApplication											*/
/* Description:	define member variables and calls init functions		*/
/************************************************************************/

CApplication::CApplication(void)
{
//define member variables
m_pDirect3DObject = NULL;
m_pDirect3DDevice = NULL;

m_dwWidth = 800;
m_dwHeight = 600;
m_ColorFormat = D3DFMT_R5G6B5;
m_DepthStencilFormat = D3DFMT_D16; 
m_dwVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
m_MultiSampling = D3DMULTISAMPLE_NONE;
m_dwAnisotropy = 0;

m_fFieldOfView = 0.785398f;
m_fNearPlane = 1.0f;
m_fFarPlane = 1000.0f;
m_fAspectRatio = (float)m_dwWidth / (float)m_dwHeight;

m_bRunningWindow = true;
m_bRunningD3D = false;

//create window
InitWindow();
}//CApplication

/************************************************************************/
/* Name:		~CApplication											*/
/* Description:	call kill functions										*/
/************************************************************************/

CApplication::~CApplication(void)
{
KillD3D();
KillWindow();
}//~CApplication

/************************************************************************/
/* Name:		InitWindow												*/
/* Description:	create	window class, window and child elements			*/
/************************************************************************/

void CApplication::InitWindow(void)
{
WNDCLASSEX WindowClass;
RECT DesktopSize;

InitCommonControls();

//Get desktop resolution
GetClientRect(GetDesktopWindow(),&DesktopSize);

//fill window class and register class
WindowClass.cbSize = sizeof(WNDCLASSEX);
WindowClass.style = CS_HREDRAW | CS_VREDRAW;
WindowClass.lpfnWndProc = WindowProcedure;
WindowClass.cbClsExtra = 0;
WindowClass.cbWndExtra = 0;
WindowClass.hInstance = GetModuleHandle(NULL);
WindowClass.hIcon = NULL;
WindowClass.hCursor = NULL;
WindowClass.hbrBackground = GetSysColorBrush(COLOR_BTNFACE);
WindowClass.lpszMenuName = NULL;
WindowClass.lpszClassName = "ClassName";
WindowClass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

RegisterClassEx(&WindowClass);

//create window
m_hWindow = CreateWindowEx(WS_EX_CONTROLPARENT,"ClassName",TITLE,WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_VISIBLE,(DesktopSize.right - WINDOW_X) / 2,(DesktopSize.bottom - WINDOW_Y) / 2,WINDOW_X,WINDOW_Y,NULL,NULL,GetModuleHandle(NULL),NULL);

//create window elements
m_hLblResolution = CreateWindow("static","Resolution:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,24,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbResolution = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,20,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hLblBackBuffer = CreateWindow("static","Backbuffer Format:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,54,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbBackBuffer = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,50,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hLblDepthStencil = CreateWindow("static","DepthStencil Format:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,84,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbDepthStencil = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,80,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hLblVertexProcessing = CreateWindow("static","Vertex Processing:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,144,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbVertexProcessing = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,140,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hLblMultiSampling = CreateWindow("static","MultiSampling:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,174,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbMultiSampling = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,170,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hLblAnisotropy = CreateWindow("static","Anisotropy:",WS_CHILD | WS_VISIBLE | SS_LEFT,20,204,200,18,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hCbAnisotropy = CreateWindow("combobox", "", WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,160,200,160,100,m_hWindow,NULL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hBtnStart = CreateWindow("button","Start",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,65,250,80,24,m_hWindow,(HMENU)ID_START,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);
m_hBtnCancel = CreateWindow("button","Cancel",WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | WS_TABSTOP,175,250,80,24,m_hWindow,(HMENU)ID_CANCEL,(HINSTANCE)GetWindowLongPtr(m_hWindow,GWLP_HINSTANCE),NULL);

//fill combo boxes
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"640 x 480");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"800 x 600");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"1024 x 768");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"1152 x 864");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"1280 x 960");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"1280 x 1024");
SendMessage(m_hCbResolution,CB_ADDSTRING,0,(long)"1600 x 1200");

SendMessage(m_hCbBackBuffer,CB_ADDSTRING,0,(long)"D3DFMT_R5G6B5");
SendMessage(m_hCbBackBuffer,CB_ADDSTRING,0,(long)"D3DFMT_A1R5G5B5");
SendMessage(m_hCbBackBuffer,CB_ADDSTRING,0,(long)"D3DFMT_X8R8G8B8");
SendMessage(m_hCbBackBuffer,CB_ADDSTRING,0,(long)"D3DFMT_A8R8G8B8");

SendMessage(m_hCbDepthStencil,CB_ADDSTRING,0,(long)"D3DFMT_D16");
SendMessage(m_hCbDepthStencil,CB_ADDSTRING,0,(long)"D3DFMT_D24S8");
SendMessage(m_hCbDepthStencil,CB_ADDSTRING,0,(long)"D3DFMT_D32");

SendMessage(m_hCbVertexProcessing,CB_ADDSTRING,0,(long)"Software");
SendMessage(m_hCbVertexProcessing,CB_ADDSTRING,0,(long)"Hardware");

SendMessage(m_hCbMultiSampling,CB_ADDSTRING,0,(long)"No FSAA");
SendMessage(m_hCbMultiSampling,CB_ADDSTRING,0,(long)"2x FSAA");
SendMessage(m_hCbMultiSampling,CB_ADDSTRING,0,(long)"4x FSAA");

SendMessage(m_hCbAnisotropy,CB_ADDSTRING,0,(long)"No AF");
SendMessage(m_hCbAnisotropy,CB_ADDSTRING,0,(long)"2x AF");
SendMessage(m_hCbAnisotropy,CB_ADDSTRING,0,(long)"4x AF");
SendMessage(m_hCbAnisotropy,CB_ADDSTRING,0,(long)"8x AF");
SendMessage(m_hCbAnisotropy,CB_ADDSTRING,0,(long)"16x AF");

//load settings from file
LoadSettings();
}//InitWindow

/************************************************************************/
/* Name:		InitD3D													*/
/* Description:	create Direct3D object and device						*/
/************************************************************************/

void CApplication::InitD3D(void)
{
//create Direct3D object
if((m_pDirect3DObject = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
 {
 MessageBox(m_hWindow,"Direct3DCreate9() failed!","InitD3D()",MB_OK);
 m_bRunningD3D = false;
 }

ZeroMemory(&m_PresentParameters,sizeof(m_PresentParameters));
m_PresentParameters.Windowed = true;
m_PresentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
m_PresentParameters.EnableAutoDepthStencil = true;
m_PresentParameters.AutoDepthStencilFormat = m_DepthStencilFormat;
m_PresentParameters.hDeviceWindow = m_hWindow;
m_PresentParameters.BackBufferWidth = m_dwWidth;
m_PresentParameters.BackBufferHeight = m_dwHeight;
m_PresentParameters.BackBufferFormat = m_ColorFormat;
m_PresentParameters.MultiSampleType = m_MultiSampling;

if(FAILED(m_pDirect3DObject->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,m_hWindow,m_dwVertexProcessing,&m_PresentParameters,&m_pDirect3DDevice)))
 {
 MessageBox(m_hWindow,"CreateDevice() failed!","InitD3D()",MB_OK);
 m_bRunningD3D = false;
 }

ShowCursor(false);

InitScene();
CheckDeviceCaps();
}//InitD3D

/************************************************************************/
/* Name:		InitScene												*/
/* Description:	set projection matrix, render states and texture stage	*/
/*				states													*/
/************************************************************************/

void CApplication::InitScene(void)
{
D3DXMatrixPerspectiveFovLH(&m_matProjection,m_fFieldOfView,m_fAspectRatio,m_fNearPlane,m_fFarPlane);
m_pDirect3DDevice->SetTransform(D3DTS_PROJECTION,&m_matProjection);

m_pDirect3DDevice->SetRenderState(D3DRS_AMBIENT,RGB(255,255,255));
m_pDirect3DDevice->SetRenderState(D3DRS_LIGHTING,false);
m_pDirect3DDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
m_pDirect3DDevice->SetRenderState(D3DRS_ZENABLE,D3DZB_TRUE);

for(unsigned i = 0;i < 8;++i)
 {
 m_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MINFILTER,D3DTEXF_LINEAR);
 m_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MAGFILTER,D3DTEXF_LINEAR);
 m_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MIPFILTER,D3DTEXF_ANISOTROPIC);
 m_pDirect3DDevice->SetSamplerState(i,D3DSAMP_MAXANISOTROPY,m_dwAnisotropy);
 }

m_pDirect3DDevice->SetFVF(D3DFVF_CUSTOMVERTEX);
}//InitScene

/************************************************************************/
/* Name:		CheckDeviceCaps											*/
/* Description:	check for necessary device caps							*/
/************************************************************************/

void CApplication::CheckDeviceCaps(void)
{
}//CheckDeviceCaps

/************************************************************************/
/* Name:		LoadSettings											*/
/* Description:	load settings and fill combo boxes						*/
/************************************************************************/

void CApplication::LoadSettings(void)
{
ifstream File("settings.cfg");
int iColorFormat,iDepthStencilFormat,iMultiSampling;

//load settings from file
File >> m_dwWidth;
File >> m_dwHeight;
File >> iColorFormat;
File >> iDepthStencilFormat;
File >> m_dwVertexProcessing;
File >> iMultiSampling;
File >> m_dwAnisotropy;

//fill D3DENUMERATED_TYPES
switch(iColorFormat)
 {
 case D3DFMT_R5G6B5: m_ColorFormat = D3DFMT_R5G6B5; break;
 case D3DFMT_A1R5G5B5: m_ColorFormat = D3DFMT_A1R5G5B5; break;
 case D3DFMT_X8R8G8B8: m_ColorFormat = D3DFMT_X8R8G8B8; break;
 case D3DFMT_A8R8G8B8: m_ColorFormat = D3DFMT_A8R8G8B8; break;
 }

switch(iDepthStencilFormat)
 {
 case D3DFMT_D16: m_DepthStencilFormat = D3DFMT_D16; break;
 case D3DFMT_D24S8: m_DepthStencilFormat = D3DFMT_D24S8; break;
 case D3DFMT_D32: m_DepthStencilFormat = D3DFMT_D32; break;
 }

switch(iMultiSampling)
 {
 case D3DMULTISAMPLE_NONE: m_MultiSampling = D3DMULTISAMPLE_NONE; break;
 case D3DMULTISAMPLE_2_SAMPLES: m_MultiSampling = D3DMULTISAMPLE_2_SAMPLES; break;
 case D3DMULTISAMPLE_4_SAMPLES: m_MultiSampling = D3DMULTISAMPLE_4_SAMPLES; break;
 }

//select loaded settings in combo boxes
switch(m_dwHeight)
 {
 case 480: SendMessage(m_hCbResolution,CB_SETCURSEL,0,0); break;
 case 600: SendMessage(m_hCbResolution,CB_SETCURSEL,1,0); break;
 case 768: SendMessage(m_hCbResolution,CB_SETCURSEL,2,0); break;
 case 864: SendMessage(m_hCbResolution,CB_SETCURSEL,3,0); break;
 case 960: SendMessage(m_hCbResolution,CB_SETCURSEL,4,0); break;
 case 1024: SendMessage(m_hCbResolution,CB_SETCURSEL,5,0); break;
 case 1200: SendMessage(m_hCbResolution,CB_SETCURSEL,6,0); break;
 }

switch(m_ColorFormat)
 {
 case D3DFMT_R5G6B5: SendMessage(m_hCbBackBuffer,CB_SETCURSEL,0,0); break;
 case D3DFMT_A1R5G5B5: SendMessage(m_hCbBackBuffer,CB_SETCURSEL,1,0); break;
 case D3DFMT_X8R8G8B8: SendMessage(m_hCbBackBuffer,CB_SETCURSEL,2,0); break;
 case D3DFMT_A8R8G8B8: SendMessage(m_hCbBackBuffer,CB_SETCURSEL,3,0); break;
 }

switch(m_DepthStencilFormat)
 {
 case D3DFMT_D16: SendMessage(m_hCbDepthStencil,CB_SETCURSEL,0,0); break;
 case D3DFMT_D24S8: SendMessage(m_hCbDepthStencil,CB_SETCURSEL,1,0); break;
 case D3DFMT_D32: SendMessage(m_hCbDepthStencil,CB_SETCURSEL,2,0); break;
 }

switch(m_dwVertexProcessing)
 {
 case D3DCREATE_SOFTWARE_VERTEXPROCESSING: SendMessage(m_hCbVertexProcessing,CB_SETCURSEL,0,0); break;
 case D3DCREATE_HARDWARE_VERTEXPROCESSING: SendMessage(m_hCbVertexProcessing,CB_SETCURSEL,1,0); break;
 }

switch(m_MultiSampling)
 {
 case D3DMULTISAMPLE_NONE: SendMessage(m_hCbMultiSampling,CB_SETCURSEL,0,0); break;
 case D3DMULTISAMPLE_2_SAMPLES: SendMessage(m_hCbMultiSampling,CB_SETCURSEL,1,0); break;
 case D3DMULTISAMPLE_4_SAMPLES: SendMessage(m_hCbMultiSampling,CB_SETCURSEL,2,0); break;
 }

switch(m_dwAnisotropy)
 {
 case 1: SendMessage(m_hCbAnisotropy,CB_SETCURSEL,0,0); break;
 case 2: SendMessage(m_hCbAnisotropy,CB_SETCURSEL,1,0); break;
 case 4: SendMessage(m_hCbAnisotropy,CB_SETCURSEL,2,0); break;
 case 8: SendMessage(m_hCbAnisotropy,CB_SETCURSEL,3,0); break;
 case 16: SendMessage(m_hCbAnisotropy,CB_SETCURSEL,4,0); break;
 }

File.close();
}//LoadSettings

/************************************************************************/
/* Name:		SaveSettings											*/
/* Description:	retrieve settings from window and save					*/
/************************************************************************/

void CApplication::SaveSettings(void)
{
ofstream File("settings.cfg",ios::trunc);

//get selected settings from combo boxes
switch(SendMessage(m_hCbResolution,CB_GETCURSEL,0,0))
 {
 case 0: m_dwWidth = 640; m_dwHeight = 480; break;
 case 1: m_dwWidth = 800; m_dwHeight = 600; break;
 case 2: m_dwWidth = 1024; m_dwHeight = 768; break;
 case 3: m_dwWidth = 1152; m_dwHeight = 864; break;
 case 4: m_dwWidth = 1280; m_dwHeight = 960; break;
 case 5: m_dwWidth = 1280; m_dwHeight = 1024; break;
 case 6: m_dwWidth = 1600; m_dwHeight = 1200; break;
 }

switch(SendMessage(m_hCbBackBuffer,CB_GETCURSEL,0,0))
 {
 case 0: m_ColorFormat = D3DFMT_R5G6B5; break;
 case 1: m_ColorFormat = D3DFMT_A1R5G5B5; break;
 case 2: m_ColorFormat = D3DFMT_X8R8G8B8; break;
 case 3: m_ColorFormat = D3DFMT_A8R8G8B8; break;
 }

switch(SendMessage(m_hCbDepthStencil,CB_GETCURSEL,0,0))
 {
 case 0: m_DepthStencilFormat = D3DFMT_D16; break;
 case 1: m_DepthStencilFormat = D3DFMT_D24S8; break;
 case 2: m_DepthStencilFormat = D3DFMT_D32; break;
 }

switch(SendMessage(m_hCbVertexProcessing,CB_GETCURSEL,0,0))
 {
 case 0: m_dwVertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING; break;
 case 1: m_dwVertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING; break;
 }

switch(SendMessage(m_hCbMultiSampling,CB_GETCURSEL,0,0))
 {
 case 0: m_MultiSampling = D3DMULTISAMPLE_NONE; break;
 case 1: m_MultiSampling = D3DMULTISAMPLE_2_SAMPLES; break;
 case 2: m_MultiSampling = D3DMULTISAMPLE_4_SAMPLES; break;
 }

switch(SendMessage(m_hCbAnisotropy,CB_GETCURSEL,0,0))
 {
 case 0: m_dwAnisotropy = 1; break;
 case 1: m_dwAnisotropy = 2; break;
 case 2: m_dwAnisotropy = 4; break;
 case 3: m_dwAnisotropy = 8; break;
 case 4: m_dwAnisotropy = 16; break;
 }

//save settings to file
File << m_dwWidth << endl;
File << m_dwHeight << endl;
File << m_ColorFormat << endl;
File << m_DepthStencilFormat << endl;
File << m_dwVertexProcessing << endl;
File << m_MultiSampling << endl;
File << m_dwAnisotropy;

File.close();
}//SaveSettings

/************************************************************************/
/* Name:		CheckDevice												*/
/* Description:	check for lost device									*/
/************************************************************************/

bool CApplication::CheckDevice(void)
{
switch(m_pDirect3DDevice->TestCooperativeLevel())
 {
 case D3DERR_DEVICELOST: return false;
 case D3DERR_DEVICENOTRESET:
  {
  if(FAILED(m_pDirect3DDevice->Reset(&m_PresentParameters)))
   {
   MessageBox(m_hWindow,"Reset() failed!","CheckDevice()",MB_OK);
   return false;
   }

  InitScene();

  return true;
  }
 default: return true;
 }
}//CheckDevice

/************************************************************************/
/* Name:		KillWindow												*/
/* Description:	unregister window class									*/
/************************************************************************/

void CApplication::KillWindow(void)
{
UnregisterClass("ClassName",GetModuleHandle(NULL));
}//KillWindow

/************************************************************************/
/* Name:		KillD3D													*/
/* Description:	release memory for Direct3D device and Direct3D object	*/
/************************************************************************/

void CApplication::KillD3D(void)
{
if(m_pDirect3DDevice != NULL)
 {
 m_pDirect3DDevice->Release();
 m_pDirect3DDevice = NULL;
 }

if(m_pDirect3DObject != NULL)
 {
 m_pDirect3DObject->Release();
 m_pDirect3DObject = NULL;
 }
}//KillD3D