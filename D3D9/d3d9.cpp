#include <windows.h>

#include "d3d9.h"
#include "HookManager.h"
#include "misc.h"

IDirect3D9 *APIENTRY hkDirect3DCreate9(UINT SDKVersion)
{
	// Obtain the original function pointer upon first run.
	static tDirect3DCreate9 oDirect3DCreate9 = D3DHookMgr::GetSingleton()->GetFunction<tDirect3DCreate9>("Direct3DCreate9");

	add_log("Create Device Started...");
	add_log("\tCalling oDirect3DCreate9(%u)...", SDKVersion);
	
	IDirect3D9 *d3dint = oDirect3DCreate9(SDKVersion);
	add_log("...Device created successfully!");

	if( d3dint != NULL )
	{
		hkIDirect3D9 *ret = new hkIDirect3D9(&d3dint);
		add_log("Direct3DCreate9(%i) succeeded...", SDKVersion);
		add_log("Hooked Direct3D9 interface: 0x%x -> 0x%x", ret->m_pD3Dint, ret);
	}
	else
		add_log("Direct3DCreate9(%i) failed...", SDKVersion);

	return d3dint;
}