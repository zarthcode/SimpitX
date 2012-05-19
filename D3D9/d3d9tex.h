#ifndef _D3D9TEX_H
#define _D3D9TEX_H

#include "d3d9.h"
#include <map>
#include <set>
DWORD QuickChecksum(DWORD *pData, int size);

interface hkIDirect3DTexture9 : public IDirect3DTexture9
{

	static hkIDirect3DTexture9* FindTextureObj(IDirect3DTexture9 *ppTex);
	static hkIDirect3DTexture9* FindTextureObj(IDirect3DBaseTexture9 *ppBaseTex);

	// Returns a pointer to a new object.
	static hkIDirect3DTexture9* Factory(IDirect3DTexture9 **ppTex, IDirect3DDevice9 *pIDirect3DDevice9, UINT Width, UINT Height, D3DFORMAT Format);

	// Stores a reference to each texture object.
	static std::map<IDirect3DBaseTexture9*, hkIDirect3DTexture9*> s_TextureObjects;
	static std::set<hkIDirect3DTexture9*> s_TextureSet;

	UINT m_Width, m_Height;
	D3DFORMAT m_Format;

	// callback interface
	IDirect3DTexture9 *m_D3Dtex;
	IDirect3DDevice9 *m_D3Ddev;
	
	// original interface
	STDMETHOD(QueryInterface)(REFIID riid, void** ppvObj);
	STDMETHOD_(ULONG,AddRef)();
	STDMETHOD_(ULONG,Release)();
	STDMETHOD(GetDevice)(IDirect3DDevice9** ppDevice);
	STDMETHOD(SetPrivateData)(REFGUID refguid,CONST void* pData,DWORD SizeOfData,DWORD Flags);
	STDMETHOD(GetPrivateData)(REFGUID refguid,void* pData,DWORD* pSizeOfData);
	STDMETHOD(FreePrivateData)(REFGUID refguid);
	STDMETHOD_(DWORD, SetPriority)(DWORD PriorityNew);
	STDMETHOD_(DWORD, GetPriority)();
	STDMETHOD_(void, PreLoad)();
	STDMETHOD_(D3DRESOURCETYPE, GetType)();
	STDMETHOD_(DWORD, SetLOD)(DWORD LODNew);
	STDMETHOD_(DWORD, GetLOD)();
	STDMETHOD_(DWORD, GetLevelCount)();
	STDMETHOD(SetAutoGenFilterType)(D3DTEXTUREFILTERTYPE FilterType);
	STDMETHOD_(D3DTEXTUREFILTERTYPE, GetAutoGenFilterType)();
	STDMETHOD_(void, GenerateMipSubLevels)();
	STDMETHOD(GetLevelDesc)(UINT Level,D3DSURFACE_DESC *pDesc);
	STDMETHOD(GetSurfaceLevel)(UINT Level,IDirect3DSurface9** ppSurfaceLevel);
	STDMETHOD(LockRect)(UINT Level,D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags);
	STDMETHOD(UnlockRect)(UINT Level);
	STDMETHOD(AddDirtyRect)(CONST RECT* pDirtyRect);

private:

	hkIDirect3DTexture9(IDirect3DTexture9 **ppTex, IDirect3DDevice9 *pIDirect3DDevice9, UINT Width, UINT Height, D3DFORMAT Format)
	{

		m_D3Dtex = *ppTex;
		m_D3Ddev = pIDirect3DDevice9;
		m_Width = Width;
		m_Height = Height;
		m_Format = Format;

		// Store a reference to this object.
		s_TextureObjects.insert(std::make_pair(m_D3Dtex, this));
		s_TextureSet.insert(this);

	}

	// Private, due to the ability to commit suicide.
	virtual ~hkIDirect3DTexture9()
	{
		s_TextureObjects.erase(m_D3Dtex);
		s_TextureSet.erase(this);
	}

};

#endif