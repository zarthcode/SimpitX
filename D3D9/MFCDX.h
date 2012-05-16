#ifndef _MFCDX_H
#define _MFCDX_H

#include <Windows.h>
#include <d3d9.h>

#include <set>



class MFCDX
{
public:

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


protected:
private:

	static std::set<IDirect3DSurface9*> s_RenderSurfaces;
	static std::set<IDirect3DBaseTexture9*> s_RenderTextures;


	static void saveSurfaceToDisk(IDirect3DSurface9* pSurfaceOfInterest);
	static void saveTextureToDisk(IDirect3DBaseTexture9* pTextureOfInterest);
	static void pollSurfaces();
	static void pollTextures();

};


#endif