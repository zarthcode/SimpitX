#include "MFCDX.h"
#include "misc.h"
#include <d3dx9tex.h>
#include <sstream>
#include <map>

#pragma comment(lib,"D3dx9.lib")

std::set<IDirect3DSurface9*> MFCDX::s_RenderSurfaces;
std::set<IDirect3DBaseTexture9*> MFCDX::s_RenderTextures;


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


void MFCDX::saveTextureToDisk( IDirect3DBaseTexture9* pTextureOfInterest )
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

	add_log("Saving render surface to image file: %s", filenamestream.str().c_str());

	HRESULT result = D3DXSaveTextureToFile(filenamestream.str().c_str(), D3DXIFF_JPG, pTextureOfInterest, NULL);

	if (result != D3D_OK)
	{
		add_log("\tSave failed.");
	}
	else
	{
		add_log("\tSave Successful");
	}

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

	if (iFrameCount++ < 40)
	{
		return;
	}

	iFrameCount = 0;
	std::set<IDirect3DBaseTexture9*>::iterator itRenderTextures = s_RenderTextures.begin();

	for (;itRenderTextures != s_RenderTextures.end(); itRenderTextures++)
	{

		saveTextureToDisk(*itRenderTextures);
		s_RenderTextures.erase(*itRenderTextures);
		return;

	}

}
