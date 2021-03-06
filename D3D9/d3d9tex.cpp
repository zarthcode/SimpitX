/*	Direct3D9 Texture */

#include <windows.h>
#include "main.h"
#include "d3d9.h"
#include "misc.h"

HRESULT APIENTRY hkIDirect3DTexture9::QueryInterface(REFIID riid, void** ppvObj)
{
	INC_CALL_LOG();

	return m_D3Dtex->QueryInterface(riid, ppvObj);
}

ULONG APIENTRY hkIDirect3DTexture9::AddRef()
{
	INC_CALL_LOG();

	return m_D3Dtex->AddRef();
}

ULONG APIENTRY hkIDirect3DTexture9::Release()
{
	INC_CALL_LOG();

	// this texture is no longer interesting if the refcount is zero.
	ULONG refcount = m_D3Dtex->Release();

	// When you're rejected...
	if (refcount == 0)
	{
		
		// and forgotten...
		s_TextureObjects.erase(m_D3Dtex);
		s_TextureSet.erase(this);
		
		// ...Try Seppuku! it's the only way!
		delete this;
	}

	return refcount;
}

HRESULT APIENTRY hkIDirect3DTexture9::GetDevice(IDirect3DDevice9** ppDevice)
{
//	INC_CALL_LOG();

	*ppDevice = m_D3Ddev;
	return D3D_OK;
}

HRESULT APIENTRY hkIDirect3DTexture9::SetPrivateData(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags)
{
	INC_CALL_LOG();

	return m_D3Dtex->SetPrivateData(refguid, pData, SizeOfData, Flags);
}

HRESULT APIENTRY hkIDirect3DTexture9::GetPrivateData(REFGUID refguid,void* pData,DWORD* pSizeOfData)
{
	INC_CALL_LOG();

	return m_D3Dtex->GetPrivateData(refguid, pData, pSizeOfData);
}

HRESULT APIENTRY hkIDirect3DTexture9::FreePrivateData(REFGUID refguid)
{
	INC_CALL_LOG();

	return m_D3Dtex->FreePrivateData(refguid);
}

DWORD APIENTRY hkIDirect3DTexture9::SetPriority(DWORD PriorityNew)
{
	INC_CALL_LOG();

	return m_D3Dtex->SetPriority(PriorityNew);
}

DWORD APIENTRY hkIDirect3DTexture9::GetPriority()
{
	INC_CALL_LOG();

	return m_D3Dtex->GetPriority();
}

void APIENTRY hkIDirect3DTexture9::PreLoad()
{
	INC_CALL_LOG();

	m_D3Dtex->PreLoad();
}

D3DRESOURCETYPE APIENTRY hkIDirect3DTexture9::GetType()
{
	INC_CALL_LOG();

	return m_D3Dtex->GetType();
}

DWORD APIENTRY hkIDirect3DTexture9::SetLOD(DWORD LODNew)
{
	INC_CALL_LOG();

	return m_D3Dtex->SetLOD(LODNew);
}

DWORD APIENTRY hkIDirect3DTexture9::GetLOD()
{
	INC_CALL_LOG();

	return m_D3Dtex->GetLOD();
}

DWORD APIENTRY hkIDirect3DTexture9::GetLevelCount()
{
	INC_CALL_LOG();

	return m_D3Dtex->GetLevelCount();
}

HRESULT APIENTRY hkIDirect3DTexture9::SetAutoGenFilterType(D3DTEXTUREFILTERTYPE FilterType)
{
	INC_CALL_LOG();

	return m_D3Dtex->SetAutoGenFilterType(FilterType);
}

D3DTEXTUREFILTERTYPE APIENTRY hkIDirect3DTexture9::GetAutoGenFilterType()
{
	INC_CALL_LOG();

	return m_D3Dtex->GetAutoGenFilterType();
}

void APIENTRY hkIDirect3DTexture9::GenerateMipSubLevels()
{
	INC_CALL_LOG();

	m_D3Dtex->GenerateMipSubLevels();
}

HRESULT APIENTRY hkIDirect3DTexture9::GetLevelDesc(UINT Level,D3DSURFACE_DESC *pDesc)
{
	INC_CALL_LOG();

	return m_D3Dtex->GetLevelDesc(Level, pDesc);
}

HRESULT APIENTRY hkIDirect3DTexture9::GetSurfaceLevel(UINT Level,IDirect3DSurface9** ppSurfaceLevel)
{
	INC_CALL_LOG();

	return m_D3Dtex->GetSurfaceLevel(Level, ppSurfaceLevel);
}

HRESULT APIENTRY hkIDirect3DTexture9::LockRect(UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	INC_CALL_LOG();

	return m_D3Dtex->LockRect(Level, pLockedRect, pRect, Flags);
}

HRESULT APIENTRY hkIDirect3DTexture9::UnlockRect(UINT Level)
{
	INC_CALL_LOG();

	return m_D3Dtex->UnlockRect(Level);
}

HRESULT APIENTRY hkIDirect3DTexture9::AddDirtyRect(CONST RECT* pDirtyRect)
{
	INC_CALL_LOG();

	return m_D3Dtex->AddDirtyRect(pDirtyRect);
}

hkIDirect3DTexture9* hkIDirect3DTexture9::FindTextureObj( IDirect3DTexture9 *pTexture )
{
	if (pTexture == nullptr)
	{
		return nullptr;
	}

	std::set<hkIDirect3DTexture9*>::iterator itTextureObj = hkIDirect3DTexture9::s_TextureSet.find(reinterpret_cast<hkIDirect3DTexture9*>(pTexture));

	if( itTextureObj == hkIDirect3DTexture9::s_TextureSet.end())
	{
		return nullptr;
	}

	return *itTextureObj;
	
}

hkIDirect3DTexture9* hkIDirect3DTexture9::FindTextureObj( IDirect3DBaseTexture9 *pBaseTex )
{
	if (pBaseTex == nullptr)
	{
		return nullptr;
	}

	// Search for the texture
	std::set<hkIDirect3DTexture9*>::iterator itTextureObj = hkIDirect3DTexture9::s_TextureSet.find(reinterpret_cast<hkIDirect3DTexture9*>(pBaseTex));

	if( itTextureObj == hkIDirect3DTexture9::s_TextureSet.end())
	{
		return nullptr;
	}

	return *itTextureObj;
}

hkIDirect3DTexture9* hkIDirect3DTexture9::Factory( IDirect3DTexture9 **ppTex, IDirect3DDevice9 *pIDirect3DDevice9, UINT Width, UINT Height, D3DFORMAT Format )
{
	return new hkIDirect3DTexture9( ppTex, pIDirect3DDevice9, Width, Height, Format );
}

std::set<hkIDirect3DTexture9*> hkIDirect3DTexture9::s_TextureSet;

std::map<IDirect3DBaseTexture9*, hkIDirect3DTexture9*> hkIDirect3DTexture9::s_TextureObjects;

//////////////////////////////
// Useful Functions
//////////////////////////////
DWORD QuickChecksum(DWORD *pData, int size)
{
	if(!pData) { return 0x0; }

	DWORD sum;
	DWORD tmp;
	sum = *pData;

	for(int i = 1; i < (size/4); i++)
	{
		tmp = pData[i];
		tmp = (DWORD)(sum >> 29) + tmp;
		tmp = (DWORD)(sum >> 17) + tmp;
		sum = (DWORD)(sum << 3)  ^ tmp;
	}

	return sum;
}
