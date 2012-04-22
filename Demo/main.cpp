// file name:	main.cpp
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#include"main.h"

//globals
CApplication	g_App;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPreviousInstance,LPSTR lpcmdline,int nCmdShow)
{
MSG Message;
LPDIRECT3DVERTEXBUFFER9 pTriangleVB = NULL;
LPDIRECT3DVERTEXBUFFER9 pQuadVB = NULL;
LPDIRECT3DTEXTURE9 pPyramideTexture = NULL,pRenderTexture = NULL;
LPDIRECT3DSURFACE9 pRenderSurface = NULL,pBackBuffer = NULL;
D3DXMATRIX matRotationY,matTranslation,matProjection,matOldProjection;
VOID* pData;
float fRotation = 0.0f;
D3DVERTEX aTriangle[] = {{0.0f,1.0f,0.0f,0.5f,0.0f},{-1.0f,-1.0f,-1.0f,0.0f,1.0f},{1.0f,-1.0f,-1.0f,1.0f,1.0f},
						{0.0f,1.0f,0.0f,0.5f,0.0f},{-1.0f,-1.0f,1.0f,0.0f,1.0f},{-1.0f,-1.0f,-1.0f,1.0f,1.0f},
						{0.0f,1.0f,0.0f,0.5f,0.0f},{1.0f,-1.0f,1.0f,0.0f,1.0f},{-1.0f,-1.0f,1.0f,1.0f,1.0f},
						{0.0f,1.0f,0.0f,0.5f,0.0f},{1.0f,-1.0f,-1.0f,0.0f,1.0f},{1.0f,-1.0f,1.0f,1.0f,1.0f}};

D3DVERTEX aQuad[] = {{-1.0f,1.0f,0.0f,0.0f,0.0f},
					{-1.0f,-1.0f,0.0f,0.0f,5.0f},
					{1.0f,1.0f,0.0f,5.0f,0.0f},
					{1.0f,-1.0f,0.0f,5.0f,5.0f}};

//window loop
while(g_App.GetWindowStatus())
 {
 if(GetMessage(&Message,NULL,0,0))
  {
  if(!IsDialogMessage(g_App.GetWindowHandle(),&Message))
   {
   TranslateMessage(&Message);
   DispatchMessage(&Message);
   }
  }
 }

//switch to D3D loop or quit
if(!g_App.GetD3DStatus())
 return 0;

g_App.GetDevice()->CreateVertexBuffer(sizeof(aTriangle),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEX,D3DPOOL_MANAGED,&pTriangleVB,NULL);
g_App.GetDevice()->CreateVertexBuffer(sizeof(aQuad),D3DUSAGE_WRITEONLY,D3DFVF_CUSTOMVERTEX,D3DPOOL_MANAGED,&pQuadVB,NULL);

pTriangleVB->Lock(0,sizeof(pData),(void**)&pData,0);
memcpy(pData,aTriangle,sizeof(aTriangle));
pTriangleVB->Unlock();

pQuadVB->Lock(0,sizeof(pData),(void**)&pData,0);
memcpy(pData,aQuad,sizeof(aQuad));
pQuadVB->Unlock();

D3DXCreateTextureFromFile(g_App.GetDevice(),"texture.png",&pPyramideTexture);

//create render-to-texture objects
g_App.GetDevice()->CreateTexture(256,256,1,D3DUSAGE_RENDERTARGET,D3DFMT_R5G6B5,D3DPOOL_DEFAULT,&pRenderTexture,NULL);
pRenderTexture->GetSurfaceLevel(0,&pRenderSurface);

D3DXMatrixPerspectiveFovLH(&matProjection,D3DX_PI / 4.0f,1,1,100);
g_App.GetDevice()->GetTransform(D3DTS_PROJECTION,&matOldProjection);
g_App.GetDevice()->GetRenderTarget(0,&pBackBuffer);

//D3D loop
while(g_App.GetD3DStatus())
 {
 if(PeekMessage(&Message,NULL,0,0,PM_REMOVE))
  {
  TranslateMessage(&Message);
  DispatchMessage(&Message);
  }

 if(g_App.CheckDevice())
  {
  //render-to-texture
  g_App.GetDevice()->SetRenderTarget(0,pRenderSurface);
  g_App.GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(100,100,100),1.0f,0);
  g_App.GetDevice()->BeginScene();

  g_App.GetDevice()->SetTexture(0,pPyramideTexture);

  D3DXMatrixRotationY(&matRotationY,fRotation);
  D3DXMatrixTranslation(&matTranslation,0.0f,0.0f,5.0f);
  g_App.GetDevice()->SetTransform(D3DTS_WORLD,&(matRotationY * matTranslation));
  g_App.GetDevice()->SetTransform(D3DTS_PROJECTION,&matProjection);

  g_App.GetDevice()->SetStreamSource(0,pTriangleVB,0,sizeof(D3DVERTEX));
  g_App.GetDevice()->DrawPrimitive(D3DPT_TRIANGLELIST,0,4);

  g_App.GetDevice()->EndScene();

  //render scene with texture
  g_App.GetDevice()->SetRenderTarget(0,pBackBuffer);
  g_App.GetDevice()->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,D3DCOLOR_XRGB(0,0,0),1.0f,0);
  g_App.GetDevice()->BeginScene();

  g_App.GetDevice()->SetTexture(0,pRenderTexture);

  g_App.GetDevice()->SetTransform(D3DTS_WORLD,&matTranslation);
  g_App.GetDevice()->SetTransform(D3DTS_PROJECTION,&matOldProjection);

  g_App.GetDevice()->SetStreamSource(0,pQuadVB,0,sizeof(D3DVERTEX));
  g_App.GetDevice()->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

  g_App.GetDevice()->EndScene();
  g_App.GetDevice()->Present(NULL,NULL,NULL,NULL);

  fRotation += 0.001f;
  }
 }

pRenderSurface->Release();
pRenderSurface = NULL;

pBackBuffer->Release();
pBackBuffer = NULL;

return 0;
}//WinMain