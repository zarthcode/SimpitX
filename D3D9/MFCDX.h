#ifndef _MFCDX_H
#define _MFCDX_H

#include <Windows.h>
#include <d3d9.h>
#include "d3d9tex.h"
#include <set>
#include <boost/utility.hpp>


class MFCDX
{
public:

	// EndScene event
	void OnEndScene();

	// Increments/Decrements the Left MFCD texture
	void LeftMFCD(bool increment = true);

	// Increments/Decrements the Left MFCD texture
	void RightMFCD(bool increment = true);

	// Switches the MFCDs
	void SwapMFCDs();

	// RWR

	// CDU

	// Other displays
	
	// Configuration

	// Threading

	// TCP/IP

	// Shared memory

	// DirectX Device

	static std::set<IDirect3DSurface9*> s_RenderSurfaces;
	static std::set<hkIDirect3DTexture9*> s_RenderTextures;

	static std::set<hkIDirect3DTexture9*>::iterator itLeftMFCD;
//	static std::set<hkIDirect3DTexture9*>::iterator itRightMFCD;


	static void saveSurfaceToDisk(IDirect3DSurface9* pSurfaceOfInterest);
	static bool saveTextureToDisk(IDirect3DBaseTexture9* pTextureOfInterest);
	static void pollSurfaces();
	static void pollTextures();


protected:
private:

	static bool bEnable;
	static int PollingSpeed;
	// Utility function
	template <typename Iter, typename Cont>
	static bool is_last(Iter iter, const Cont& cont)
	{
		return (iter != cont.end()) && (next(iter) == cont.end());
	}

};


#endif