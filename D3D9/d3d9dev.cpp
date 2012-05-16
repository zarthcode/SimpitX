/*	Direct3D9 Device */

#include <windows.h>
#include "misc.h"
#include "d3d9.h"
#include "MFCDX.h"
#include "d3d9debugutils.h"

#define D3DHOOK_TEXTURES //comment this to disable texture hooking

HRESULT CD3DManager::Initialize()
{
	/*
	initialize Resources such as textures 
	(managed and unmanaged [D3DPOOL]), 
	vertex buffers, and other D3D rendering resources
	...
	m_pD3Ddev->CreateTexture(..., ..., &m_pD3Dtexture);
	*/
	return S_OK;
}

HRESULT CD3DManager::PreReset()
{
	/*
	release all UNMANAGED [D3DPOOL_DEFAULT] 
	textures, vertex buffers, and other 
	volatile resources
	...
	_SAFE_RELEASE(m_pD3Dtexture);
	*/
	return S_OK;
}

HRESULT CD3DManager::PostReset()
{
	/*
	re-initialize all UNMANAGED [D3DPOOL_DEFAULT]
	textures, vertex buffers, and other volatile 
	resources
	...
	m_pD3Ddev->CreateTexture(..., ..., &m_pD3Dtexture);
	*/
	return S_OK;
}

HRESULT CD3DManager::Release()
{
	/*
	Release all textures, vertex buffers, and 
	other resources
	...
	_SAFE_RELEASE(m_pD3Dtexture);
	*/
	return S_OK;
}

//-----------------------------------------------------------------------------

HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid, LPVOID *ppvObj) 
{
	LPOLESTR wbuff[128];
	INC_CALL_LOG();


	if(::StringFromCLSID(riid, wbuff) != S_OK)
	{
		// Could not convert. Log and clear buffer.
		detail_log("REFIID conversion failed.");
	}

	std::wstring tempstr;
	tempstr.copy((const LPOLESTR)wbuff, wcslen((const LPOLESTR)wbuff));

	//	call_log("HRESULT APIENTRY hkIDirect3DDevice9::QueryInterface(REFIID riid = %s, LPVOID *ppvObj = 0x%x)", narrow(tempstr).c_str(), ppvObj );


	HRESULT result = m_pD3Ddev->QueryInterface(riid, ppvObj);


	RESULT_LOG();
	return result;

}

ULONG APIENTRY hkIDirect3DDevice9::AddRef() 
{
	INC_CALL_LOG();
	;


	m_refCount++;

	return m_pD3Ddev->AddRef();


}

HRESULT APIENTRY hkIDirect3DDevice9::BeginScene() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->BeginScene();

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::BeginStateBlock() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->BeginStateBlock();

// Squelched for now. -Zarthrag
//	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::Clear(DWORD Count, CONST D3DRECT *pRects, DWORD Flags, D3DCOLOR Color, float Z, DWORD Stencil) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->Clear(Count, pRects, Flags, Color, Z, Stencil);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect, D3DCOLOR color) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->ColorFill(pSurface,pRect,color);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS *pPresentationParameters, IDirect3DSwapChain9 **ppSwapChain) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateAdditionalSwapChain(pPresentationParameters, ppSwapChain);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle) 
{
	static bool bFirstCall = false;

	INC_CALL_LOG();

	if (!bFirstCall)
	{
		bFirstCall = true;
		
		detail_log("first call.");
	}

	

	HRESULT result = m_pD3Ddev->CreateCubeTexture(EdgeLength, Levels, Usage, Format, Pool, ppCubeTexture,pSharedHandle);

#ifdef D3DHOOK_TEXTURES
	//	hkIDirect3DTexture9* texturenew =  new hkIDirect3DTexture9((IDirect3DBaseTexture9**)ppCubeTexture, m_pD3Ddev, Width, Height, Format);
	//	*ppTexture = (IDirect3DTexture9*)texturenew;
#endif
	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateDepthStencilSurface(Width, Height, Format, MultiSample, MultisampleQuality,Discard,ppSurface, pSharedHandle);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer,pSharedHandle);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateOffscreenPlainSurface(Width,Height,Format,Pool,ppSurface,pSharedHandle);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		// Log
		

		// Store a pointer to the surface (TODO: create a wrapper for this class)
		if (MFCDX::s_RenderSurfaces.find(*ppSurface) == MFCDX::s_RenderSurfaces.end())
		{
			detail_log("IDirect3D9Surface created at address 0x%x", *ppSurface);
			MFCDX::s_RenderSurfaces.insert(*ppSurface);
		}
		else
		{
			// No need.
		}
	}

	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader) 
{
	INC_CALL_LOG();

	HRESULT result = m_pD3Ddev->CreatePixelShader(pFunction, ppShader);


	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateQuery(Type,ppQuery);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle) 
{
	INC_CALL_LOG();



	detail_log("CreateRenderTarget(UINT Width = %lu, UINT Height = %lu, D3DFORMAT %i, D3DMULTISAMPLE_TYPE %i, DWORD multisamplequality = %lu, BOOL Lockable = %i, IDirect3DSurface9** ppSurface, HANDLE* pSharedHandle);", Width, Height, Format, MultiSample, MultisampleQuality, Lockable);

	HRESULT result = m_pD3Ddev->CreateRenderTarget(Width, Height, Format, MultiSample,MultisampleQuality, Lockable, ppSurface,pSharedHandle);

	if (pSharedHandle != NULL)
	{
		detail_log("NOTE: pSharedHandle is NOT NULL (0x%x).  Resource sharing cannot be inserted here, easily.", pSharedHandle);
	}

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(*ppSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (render target) created at address 0x%x", *ppSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(*ppSurface);
		}
		else
		{
			// Move along.
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateStateBlock(Type, ppSB);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) 
{
	static bool bFirstCall = false;

	INC_CALL_LOG();

	if (!bFirstCall)
	{
		bFirstCall = true;
		
		detail_log("first call.");
	}

	// We're interested in D3DUSAGE_RENDERTARGET
	

	//	call_log("CreateTexture(%lu, %lu, %lu, %lu, D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle) ", Width, Height, Levels, Usage);
	HRESULT ret = m_pD3Ddev->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);

	if(ret == D3D_OK)
	{
		if(Usage & D3DUSAGE_RENDERTARGET)
		{
			detail_log("CreateTexture(UINT Width = %lu,\nUINT Height = %lu,\nUINT Levels = %lu,\nDWORD Usage = %s,\nD3DFORMAT Format = %s,\nD3DPOOL Pool = %s, \nIDirect3DTexture9** ppTexture = 0x%x, \nHANDLE* pSharedHandle = 0x%x) ", Width, Height, Levels, D3DUSAGEToString(Usage).c_str(), ToString(Format).c_str(), ToString(Pool).c_str(), ppTexture, pSharedHandle);
			// Squirrel away some interest in this one.			
			MFCDX::s_RenderTextures.insert(*ppTexture);
		
#ifdef D3DHOOK_TEXTURES
			hkIDirect3DTexture9* texturenew =  new hkIDirect3DTexture9(ppTexture, m_pD3Ddev, Width, Height, Format);
			*ppTexture = (IDirect3DTexture9*)texturenew;
#endif
		}
		
	}
	else
	{
		
		detail_log("HRESULT = %s", ToString(ret).c_str());

	}


	return ret;
}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer,pSharedHandle);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateVertexDeclaration(pVertexElements,ppDecl);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->CreateVertexShader(pFunction, ppShader);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle) 
{
	static bool bFirstCall = false;

	INC_CALL_LOG();

	if (!bFirstCall)
	{
		bFirstCall = true;
		detail_log("first call.");
	}

	HRESULT result = m_pD3Ddev->CreateVolumeTexture(Width, Height, Depth, Levels, Usage, Format, Pool, ppVolumeTexture,pSharedHandle);


	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DeletePatch(UINT Handle) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DeletePatch(Handle);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{	
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::EndScene()
{
	INC_CALL_LOG();
	static int lockOut[] = {0,0,0,0};
#define lockOutPeriod 150;
	for (int i = 0; i < 4; i++)
	{
		if (lockOut[i] > 0)
		{
			lockOut[i]--;
		}
	}

	HRESULT result = m_pD3Ddev->EndScene();

	if (result == D3D_OK)
	{
		if(GetAsyncKeyState(VK_OEM_4) & 0x8000)	// '[{' key
		{
			// Left MFCD
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (lockOut[0] == 0))
			{
				detail_log("Left MFCD Increment");
				lockOut[0] = lockOutPeriod;
				
				lockOut[1] = lockOutPeriod;	// Slave buttons
			}
			else if (lockOut[1] == 0)
			{
				detail_log("Left MFCD Decrement");
				lockOut[1] = lockOutPeriod;
				lockOut[0] = lockOutPeriod;
			}

			
			
		}
		else
		{
			lockOut[0] = lockOut[1] = 0;
		}

		if(GetAsyncKeyState(VK_OEM_6) & 0x8000) // ']}' key
		{
			// Next Right MFCD
			if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (lockOut[2] == 0))
			{
				detail_log("Right MFCD Increment");
				lockOut[2] = lockOutPeriod;
				lockOut[3] = lockOutPeriod; // Slave buttons
			}
			else if (lockOut[3] == 0)
			{
				detail_log("Right MFCD Decrement");
				lockOut[3] = lockOutPeriod;
				lockOut[2] = lockOutPeriod; // Slave buttons
			}
		}
		else
		{
			lockOut[2] = lockOut[3] = 0;
		}

		
	}

	//	MFCDX::pollSurfaces();

// Squelched - ED is...a bit naughty. -Zarthrag
// 	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::EndStateBlock(IDirect3DStateBlock9** ppSB) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->EndStateBlock(ppSB);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::EvictManagedResources() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->EvictManagedResources();

	RESULT_LOG();
	return result;


}

UINT APIENTRY hkIDirect3DDevice9::GetAvailableTextureMem() 
{
	INC_CALL_LOG();



	return m_pD3Ddev->GetAvailableTextureMem();

}

HRESULT APIENTRY hkIDirect3DDevice9::GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetBackBuffer(iSwapChain,iBackBuffer, Type, ppBackBuffer);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(*ppBackBuffer) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (backbuffer) found at address 0x%x", *ppBackBuffer);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(*ppBackBuffer);
		}
		else
		{
			// Move along.
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;



}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipPlane(DWORD Index, float *pPlane) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetClipPlane(Index, pPlane);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetClipStatus(D3DCLIPSTATUS9 *pClipStatus) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetClipStatus(pClipStatus);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetCreationParameters(D3DDEVICE_CREATION_PARAMETERS *pParameters) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetCreationParameters(pParameters);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetCurrentTexturePalette(UINT *pPaletteNumber)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetCurrentTexturePalette(pPaletteNumber);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetDepthStencilSurface(IDirect3DSurface9 **ppZStencilSurface) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->GetDepthStencilSurface(ppZStencilSurface);

	if (result == D3D_OK)
	{
		//		if (MFCDX::s_RenderSurfaces.find(*ppZStencilSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			//			
			//			detail_log("IDirect3D9Surface (depth stencil) retrieved from address 0x%x", *ppZStencilSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			//			MFCDX::s_RenderSurfaces.insert(*ppZStencilSurface);
		}
		//		else
		{
			// Move along.
		}

	}
	else //(result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetDeviceCaps(D3DCAPS9 *pCaps) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->GetDeviceCaps(pCaps);

	RESULT_LOG();
	return result;

}

HRESULT APIENTRY hkIDirect3DDevice9::GetDirect3D(IDirect3D9 **ppD3D9) 
{
	INC_CALL_LOG();



	HRESULT hRet = m_pD3Ddev->GetDirect3D(ppD3D9);
	if( SUCCEEDED(hRet) )
		*ppD3D9 = m_pD3Dint;
	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::GetDisplayMode(UINT iSwapChain,D3DDISPLAYMODE* pMode) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetDisplayMode(iSwapChain,pMode);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->GetFrontBufferData(iSwapChain,pDestSurface);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(pDestSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (front buffer) retrieved from address 0x%x", pDestSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(pDestSurface);
		}
		else
		{
			// Move along.
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetFVF(DWORD* pFVF) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->GetFVF(pFVF);

	RESULT_LOG();
	return result;


}

void APIENTRY hkIDirect3DDevice9::GetGammaRamp(UINT iSwapChain,D3DGAMMARAMP* pRamp) 
{
	INC_CALL_LOG();



	m_pD3Ddev->GetGammaRamp(iSwapChain,pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::GetIndices(IDirect3DIndexBuffer9** ppIndexData) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetIndices(ppIndexData);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetLight(DWORD Index, D3DLIGHT9 *pLight) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetLight(Index, pLight);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetLightEnable(DWORD Index, BOOL *pEnable) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetLightEnable(Index, pEnable);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetMaterial(D3DMATERIAL9 *pMaterial) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetMaterial(pMaterial);

	RESULT_LOG();
	return result;


}

float APIENTRY hkIDirect3DDevice9::GetNPatchMode() 
{
	INC_CALL_LOG();



	return m_pD3Ddev->GetNPatchMode();


}

unsigned int APIENTRY hkIDirect3DDevice9::GetNumberOfSwapChains() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetNumberOfSwapChains();

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetPaletteEntries(UINT PaletteNumber, PALETTEENTRY *pEntries)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetPaletteEntries(PaletteNumber, pEntries);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShader(IDirect3DPixelShader9** ppShader) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetPixelShader(ppShader);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetPixelShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetRasterStatus(UINT iSwapChain,D3DRASTER_STATUS* pRasterStatus) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetRasterStatus(iSwapChain,pRasterStatus);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderState(D3DRENDERSTATETYPE State, DWORD *pValue) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetRenderState(State, pValue);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget) 
{
	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->GetRenderTarget(RenderTargetIndex,ppRenderTarget);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(*ppRenderTarget) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (render target) found at address 0x%x", *ppRenderTarget);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(*ppRenderTarget);
		}
		else
		{
			// Move along.
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetRenderTargetData(pRenderTarget,pDestSurface);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD* pValue) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetSamplerState(Sampler,Type,pValue);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetScissorRect(RECT* pRect) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetScissorRect(pRect);

	RESULT_LOG();
	return result;


}

BOOL APIENTRY hkIDirect3DDevice9::GetSoftwareVertexProcessing() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetSoftwareVertexProcessing();

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* OffsetInBytes,UINT* pStride) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetStreamSource(StreamNumber, ppStreamData,OffsetInBytes, pStride);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetStreamSourceFreq(UINT StreamNumber,UINT* Divider) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetStreamSourceFreq(StreamNumber,Divider);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetSwapChain(iSwapChain,pSwapChain);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetTexture(DWORD Stage, IDirect3DBaseTexture9 **ppTexture) 
{
	static bool bFirstCall = false;

	INC_CALL_LOG();

	if (!bFirstCall)
	{
		bFirstCall = true;
		
		detail_log("first call.");
	}


	HRESULT result = m_pD3Ddev->GetTexture(Stage, ppTexture);

	/*
	#ifdef D3DHOOK_TEXTURES
	// Lookup the texture.
	hkIDirect3DTexture9* hookedTexture = hkIDirect3DTexture9::FindTexture(ppTexture);

	if (hookedTexture == nullptr)
	{
	
	detail_log("hookedTexture is null.  This is dubious.");
	// Create wrapped texture
	// hookedTexture = new hkIDirect3DTexture9(ppTexture, m_pD3Ddev);

	}
	else
	{
	*ppTexture = hookedTexture;
	}

	#endif
	*/


	RESULT_LOG();
	return result;

}

HRESULT APIENTRY hkIDirect3DDevice9::GetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD *pValue) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetTextureStageState(Stage, Type, pValue);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetTransform(D3DTRANSFORMSTATETYPE State, D3DMATRIX *pMatrix) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetTransform(State, pMatrix);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetVertexDeclaration(ppDecl);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShader(IDirect3DVertexShader9** ppShader) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetVertexShader(ppShader);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantB(UINT StartRegister,BOOL* pConstantData,UINT BoolCount)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantF(UINT StartRegister,float* pConstantData,UINT Vector4fCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetVertexShaderConstantI(UINT StartRegister,int* pConstantData,UINT Vector4iCount)
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::GetViewport(D3DVIEWPORT9 *pViewport) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->GetViewport(pViewport);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::LightEnable(DWORD LightIndex, BOOL bEnable) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->LightEnable(LightIndex, bEnable);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::MultiplyTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->MultiplyTransform(State, pMatrix);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::Present(CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion) 
{	
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->ProcessVertices(SrcStartIndex, DestIndex, VertexCount, pDestBuffer,pVertexDecl, Flags);

	RESULT_LOG();
	return result;

}

ULONG APIENTRY hkIDirect3DDevice9::Release() 
{
	INC_CALL_LOG();



	if( --m_refCount == 0 )
		m_pManager->Release();

	return m_pD3Ddev->Release();
}

HRESULT APIENTRY hkIDirect3DDevice9::Reset(D3DPRESENT_PARAMETERS *pPresentationParameters) 
{
	INC_CALL_LOG();
	


	m_pManager->PreReset();

	HRESULT hRet = m_pD3Ddev->Reset(pPresentationParameters);

	if( SUCCEEDED(hRet) )
	{
		m_PresentParam = *pPresentationParameters;
		m_pManager->PostReset();
	}

	return hRet;
}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipPlane(DWORD Index, CONST float *pPlane) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetClipPlane(Index, pPlane);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetClipStatus(CONST D3DCLIPSTATUS9 *pClipStatus) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetClipStatus(pClipStatus);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetCurrentTexturePalette(UINT PaletteNumber) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetCurrentTexturePalette(PaletteNumber);

	RESULT_LOG();
	return result;


}

void APIENTRY hkIDirect3DDevice9::SetCursorPosition(int X, int Y, DWORD Flags) 
{
	INC_CALL_LOG();



	m_pD3Ddev->SetCursorPosition(X, Y, Flags);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetCursorProperties(UINT XHotSpot, UINT YHotSpot, IDirect3DSurface9 *pCursorBitmap) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetCursorProperties(XHotSpot, YHotSpot, pCursorBitmap);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetDepthStencilSurface(pNewZStencil);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetDialogBoxMode(BOOL bEnableDialogs) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetDialogBoxMode(bEnableDialogs);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetFVF(DWORD FVF) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetFVF(FVF);

	RESULT_LOG();
	return result;


}

void APIENTRY hkIDirect3DDevice9::SetGammaRamp(UINT iSwapChain,DWORD Flags,CONST D3DGAMMARAMP* pRamp)
{
	INC_CALL_LOG();



	m_pD3Ddev->SetGammaRamp(iSwapChain,Flags, pRamp);
}

HRESULT APIENTRY hkIDirect3DDevice9::SetIndices(IDirect3DIndexBuffer9* pIndexData) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetIndices(pIndexData);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetLight(DWORD Index, CONST D3DLIGHT9 *pLight) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetLight(Index, pLight);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetMaterial(CONST D3DMATERIAL9 *pMaterial) 
{	
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetMaterial(pMaterial);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetNPatchMode(float nSegments) 
{	
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetNPatchMode(nSegments);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetPaletteEntries(UINT PaletteNumber, CONST PALETTEENTRY *pEntries) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetPaletteEntries(PaletteNumber, pEntries);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShader(IDirect3DPixelShader9* pShader) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetPixelShader(pShader);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetPixelShaderConstantB(StartRegister,pConstantData,BoolCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetPixelShaderConstantF(StartRegister,pConstantData,Vector4fCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetPixelShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetPixelShaderConstantI(StartRegister,pConstantData,Vector4iCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderState(D3DRENDERSTATETYPE State, DWORD Value) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetRenderState(State, Value);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetRenderTarget(DWORD RenderTargetIndex, IDirect3DSurface9* pRenderTarget) 
{
	INC_CALL_LOG();



	//	call_log("SetRenderTarget(%lu, %lu);", RenderTargetIndex, QuickChecksum((DWORD*) pRenderTarget, sizeof(IDirect3DSurface9)));
	HRESULT result = m_pD3Ddev->SetRenderTarget(RenderTargetIndex,pRenderTarget);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(pRenderTarget) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (render target) found at address 0x%x", pRenderTarget);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(pRenderTarget);
		}
		else
		{
			// Move along.
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetSamplerState(DWORD Sampler,D3DSAMPLERSTATETYPE Type,DWORD Value) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetSamplerState(Sampler,Type,Value);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetScissorRect(CONST RECT* pRect) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetScissorRect(pRect);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetSoftwareVertexProcessing(BOOL bSoftware) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetSoftwareVertexProcessing(bSoftware);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetStreamSource(StreamNumber, pStreamData,OffsetInBytes, Stride);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetStreamSourceFreq(UINT StreamNumber,UINT Divider)
{	
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetStreamSourceFreq(StreamNumber,Divider);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetTexture(DWORD Stage, IDirect3DBaseTexture9 *pTexture) 
{
	static bool bFirstCall = false;

	INC_CALL_LOG();

	if (!bFirstCall)
	{
		bFirstCall = true;
		
		detail_log("first call.");
	}




	HRESULT result = D3DERR_INVALIDCALL;

	try
	{

#ifdef D3DHOOK_TEXTURES

		hkIDirect3DTexture9* phkTextureObj = hkIDirect3DTexture9::FindTextureObj(pTexture);	// Optimize this: short-circuit into the next test	

		if((pTexture != NULL) && (phkTextureObj != nullptr))
		{
			IDirect3DDevice9 *dev = NULL;
			{

				HRESULT res =  phkTextureObj->GetDevice(&dev);

				if (res != D3D_OK)
				{
					detail_log("((hkIDirect3DTexture9*)(pTexture))->GetDevice(&dev) returned HRESULT = %s", ToString(res).c_str());
				} 
				
			}

			if(dev == m_pD3Ddev)
			{
				result = m_pD3Ddev->SetTexture(Stage, phkTextureObj->m_D3Dtex); 

				RESULT_LOG();
				return result;

			}
			else
			{
				add_log("SetTexture() encountered a device mismatch.");
				detail_log("Device mismatch");
			}
		}
		else
#endif
		{
		result = m_pD3Ddev->SetTexture(Stage, pTexture); 
		}

	}
	catch (std::bad_cast* e)
	{
		detail_log("cast threw an exception! %s", e->what());
	}


	RESULT_LOG();
	return result;



}

HRESULT APIENTRY hkIDirect3DDevice9::SetTextureStageState(DWORD Stage, D3DTEXTURESTAGESTATETYPE Type, DWORD Value) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetTextureStageState(Stage, Type, Value);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetTransform(D3DTRANSFORMSTATETYPE State, CONST D3DMATRIX *pMatrix) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetTransform(State, pMatrix);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetVertexDeclaration(pDecl);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShader(IDirect3DVertexShader9* pShader) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetVertexShader(pShader);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetVertexShaderConstantB(StartRegister,pConstantData,BoolCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetVertexShaderConstantF(StartRegister,pConstantData,Vector4fCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetVertexShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->SetVertexShaderConstantI(StartRegister,pConstantData,Vector4iCount);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::SetViewport(CONST D3DVIEWPORT9 *pViewport) 
{
	INC_CALL_LOG();



	//	call_log("SetViewport(checksum: %lu);", QuickChecksum((DWORD*) pViewport, sizeof(D3DVIEWPORT9)));
	HRESULT result = m_pD3Ddev->SetViewport(pViewport);

	RESULT_LOG();
	return result;


}

BOOL APIENTRY hkIDirect3DDevice9::ShowCursor(BOOL bShow) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->ShowCursor(bShow);

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter) 
{

	INC_CALL_LOG();


	HRESULT result = m_pD3Ddev->StretchRect(pSourceSurface,pSourceRect,pDestSurface,pDestRect,Filter);

	// Store a pointer/handle to the IDirect3DSurface9 surface just created.
	if (result == D3D_OK)
	{

		

		if (MFCDX::s_RenderSurfaces.find(pSourceSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log

			detail_log("IDirect3D9Surface (stretching rectangle, source) found at address 0x%x", pSourceSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			//	MFCDX::s_RenderSurfaces.insert(pSourceSurface);
		}

		if (MFCDX::s_RenderSurfaces.find(pDestSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			detail_log("IDirect3D9Surface (stretching rectangle, dest) found at address 0x%x", pDestSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(pDestSurface);
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::TestCooperativeLevel() 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->TestCooperativeLevel();

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->UpdateSurface(pSourceSurface,pSourceRect,pDestinationSurface,pDestPoint);

	if (result == D3D_OK)
	{
		if (MFCDX::s_RenderSurfaces.find(pSourceSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (update surface, source) found at address 0x%x", pSourceSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(pSourceSurface);
		}

		if (MFCDX::s_RenderSurfaces.find(pDestinationSurface) == MFCDX::s_RenderSurfaces.end())
		{

			// Log
			
			detail_log("IDirect3D9Surface (update surface, dest) found at address 0x%x", pDestinationSurface);

			// Store a pointer to the surface (TODO: create a wrapper for this class)
			MFCDX::s_RenderSurfaces.insert(pDestinationSurface);
		}

	}
	else // (result != D3D_OK)
	{
		
		detail_log("HRESULT = %s", ToString(result).c_str());
	}

	RESULT_LOG();
	return result;


}

HRESULT APIENTRY hkIDirect3DDevice9::UpdateTexture(IDirect3DBaseTexture9 *pSourceTexture, IDirect3DBaseTexture9 *pDestinationTexture) 
{
	INC_CALL_LOG();

#ifdef D3DHOOK_TEXTURES
	
	hkIDirect3DTexture9* phkSourceTexture = hkIDirect3DTexture9::FindTextureObj(pSourceTexture);
	hkIDirect3DTexture9* phkDestTexture = hkIDirect3DTexture9::FindTextureObj(pDestinationTexture);

	if(phkSourceTexture != nullptr)
	{
		detail_log("Source texture is hooked texture object.");
		pSourceTexture = phkSourceTexture->m_D3Dtex;
	}

	if(phkDestTexture != nullptr)
	{
		detail_log("Destination texture is hooked texture object.");
		pDestinationTexture = phkDestTexture->m_D3Dtex;
	}
	
#endif
	
	HRESULT result = m_pD3Ddev->UpdateTexture(pSourceTexture,pDestinationTexture);

	RESULT_LOG();
	return result;
	
}

HRESULT APIENTRY hkIDirect3DDevice9::ValidateDevice(DWORD *pNumPasses) 
{
	INC_CALL_LOG();



	HRESULT result = m_pD3Ddev->ValidateDevice(pNumPasses);


	RESULT_LOG();
	return result;

}
