#include "MFCDX.h"
#include "misc.h"
#include "d3d9tex.h"
#include <d3dx9tex.h>
#include <sstream>
#include <map>
#include <boost/utility.hpp>

#pragma comment(lib,"D3dx9.lib")

std::set<IDirect3DSurface9*> MFCDX::s_RenderSurfaces;
std::set<hkIDirect3DTexture9*> MFCDX::s_RenderTextures;


void MFCDX::saveSurfaceToDisk( IDirect3DSurface9* pSurfaceOfInterest )
{
	static std::map<IDirect3DSurface9*, unsigned int>s_Counters;

	if(s_Counters.find(pSurfaceOfInterest) == s_Counters.end())
	{
		s_Counters.insert(std::make_pair<IDirect3DSurface9*,unsigned int>(pSurfaceOfInterest,0));
	}

	// Call save method
	char filenamebuffer[256];
	sprintf_s(filenamebuffer, 256, "Surface_0x%x_%i.jpg", pSurfaceOfInterest, s_Counters[pSurfaceOfInterest]++);

	std::stringstream filenamestream;

	filenamestream << GetDirectoryFile(filenamebuffer);

	add_log("Saving render surface to image file: %s", filenamestream.str().c_str());

	HRESULT result = D3DXSaveSurfaceToFile(filenamestream.str().c_str(), D3DXIFF_JPG, pSurfaceOfInterest, NULL, NULL);

	if (result != D3D_OK)
	{
		add_log("\tSave failed.");
	}
	else
	{
		add_log("\tSave Successful");
	}

}


bool MFCDX::saveTextureToDisk( IDirect3DBaseTexture9* pTextureOfInterest )
{
	static std::map<IDirect3DBaseTexture9*, unsigned int>s_Counters;

	if(s_Counters.find(pTextureOfInterest) == s_Counters.end())
	{
		s_Counters.insert(std::make_pair<IDirect3DBaseTexture9*,unsigned int>(pTextureOfInterest,0));
	}

	// Call save method
	char filenamebuffer[256];
	sprintf_s(filenamebuffer, 256, "Texture_0x%x_%i.jpg", pTextureOfInterest, s_Counters[pTextureOfInterest]++);

	std::stringstream filenamestream;

	filenamestream << GetDirectoryFile(filenamebuffer);

	add_log("Saving texture to image file: %s", filenamestream.str().c_str());

	HRESULT result = D3DXSaveTextureToFile(filenamestream.str().c_str(), D3DXIFF_JPG, pTextureOfInterest, NULL);

	if (result != D3D_OK)
	{
		add_log("\tSave failed.");
		return false;
	}
	
	add_log("\tSave Successful");
	return true;

}


bool MFCDX::saveTextureToMem( IDirect3DBaseTexture9* pTextureOfInterest )
{
	IDirect3DTexture9* pTexture = static_cast<IDirect3DTexture9*>(pTextureOfInterest);
	// Store the frame id number

	// Get dimensions

	// Copy texture data to shared memory
	D3DLOCKED_RECT *pLockedRect = nullptr;
	HRESULT result = pTexture->LockRect(0, pLockedRect, NULL, D3DLOCK_READONLY);

	if (result != D3D_OK)
	{
		add_log("\Texture readonly lock failed.");
		return false;
	}

	

	add_log("\tSave Successful");
	return true;

}

void MFCDX::pollSurfaces()
{
	static int iFrameCount = 0;

	if (iFrameCount++ < 40)
	{
		return;
	}

	iFrameCount = 0;
	std::set<IDirect3DSurface9*>::iterator itRenderSurfaces = s_RenderSurfaces.begin();

	for (;itRenderSurfaces != s_RenderSurfaces.end(); itRenderSurfaces++)
	{
		
		saveSurfaceToDisk(*itRenderSurfaces);
		s_RenderSurfaces.erase(*itRenderSurfaces);
		return;
		
	}

}

void MFCDX::pollTextures()
{
	static int iFrameCount = 0;
	static bool bInitFlag = false;

	if (iFrameCount++ < PollingSpeed)
	{
		return;
	}

	if(!bInitFlag)
	{
		bInitFlag = true;
		bEnable = false;
		PollingSpeed = 200;
		add_log("reinit left mfcd.");
		itLeftMFCD = s_RenderTextures.begin();
	}

	if(!bEnable)
		return;

	iFrameCount = 0;
	
	// Save the texture
	if (itLeftMFCD != s_RenderTextures.end())
	{
		add_log("attempting save.");
		if(!saveTextureToDisk((IDirect3DBaseTexture9*)((*itLeftMFCD)->m_D3Dtex)))
		{
			itLeftMFCD = s_RenderTextures.erase(itLeftMFCD);
		}

		add_log("successful.");
	}

	return;

}

void MFCDX::OnEndScene()
{

	static int lockOut[] = {0,0,0,0};

#define lockOutPeriod 75;

	// Key-repeating functionality.
	for (int i = 0; i < 4; i++)
	{
		if (lockOut[i] > 0)
		{
			lockOut[i]--;
		}
	}

	// Look for pressed keys
	if(GetAsyncKeyState(VK_OEM_4) & 0x8000)	// '[{' key
	{
		// Left MFCD
		if ((GetAsyncKeyState(VK_SHIFT) & 0x8000) && (lockOut[0] == 0))
		{
			detail_log("Left MFCD Increment");
			LeftMFCD();
			lockOut[0] = lockOutPeriod;
			lockOut[1] = lockOutPeriod;	// Slave buttons

		}
		else if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (lockOut[0] == 0))
		{
			detail_log("MFCD Swap");
			SwapMFCDs();
			lockOut[0] =  lockOutPeriod;
			lockOut[0] *= 5;
			lockOut[1] = lockOut[0];	// Slave buttons
		}
		else if (lockOut[1] == 0)
		{
			detail_log("Left MFCD Decrement");
			LeftMFCD(false);
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
			RightMFCD();
			lockOut[2] = lockOutPeriod;
			lockOut[3] = lockOutPeriod; // Slave buttons
		}
		else if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (lockOut[2] == 0))
		{
			detail_log("MFCD Swap");
			SwapMFCDs();
			lockOut[2] = lockOutPeriod;
			lockOut[2] *= 5;
			lockOut[3] = lockOut[2];	// Slave buttons
		}
		else if (lockOut[3] == 0)
		{
			detail_log("Right MFCD Decrement");
			RightMFCD(false);
			lockOut[3] = lockOutPeriod;
			lockOut[2] = lockOutPeriod; // Slave buttons
		}
	}
	else
	{
		lockOut[2] = lockOut[3] = 0;
	}
	
	pollTextures();

}

void MFCDX::LeftMFCD( bool increment /*= true*/ )
{
	if (increment)
	{

		if(!is_last(itLeftMFCD,s_RenderTextures))
		{
			itLeftMFCD++;
		}
	}	
	else
	{
		if(itLeftMFCD != s_RenderTextures.begin())
		{
			itLeftMFCD--;
		}
	}
}

void MFCDX::RightMFCD( bool increment /*= true*/ )
{
	if(increment)
	{
		PollingSpeed += 15;
	}
	else
	{
		PollingSpeed -= 15;
	}
}

void MFCDX::SwapMFCDs()
{
	bEnable = !bEnable;
}


int MFCDX::PollingSpeed;

bool MFCDX::bEnable;

std::set<hkIDirect3DTexture9*>::iterator MFCDX::itLeftMFCD;
