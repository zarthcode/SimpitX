#include "d3d9debugutils.h"
#include <sstream>


std::string ToString( D3DFORMAT format )
{
	switch(format)
	{
	case D3DFMT_UNKNOWN:
		return "D3DFMT_UNKNOWN";
		break;

	case D3DFMT_R8G8B8:
		return "D3DFMT_R8G8B8";
		break;
	case D3DFMT_A8R8G8B8:
		return "D3DFMT_A8R8G8B8";
		break;

	case D3DFMT_X8R8G8B8:
		return "D3DFMT_X8R8G8B8";
		break;


	case D3DFMT_R5G6B5:
		return "D3DFMT_R5G6B5";
		break;
	case D3DFMT_X1R5G5B5:
		return "D3DFMT_X1R5G5B5";
		break;
	case D3DFMT_A1R5G5B5:
		return "D3DFMT_A1R5G5B5";
		break;
	case D3DFMT_A4R4G4B4:
		return "D3DFMT_A4R4G4B4";
		break;
	case D3DFMT_R3G3B2:
		return "D3DFMT_R3G3B2";
		break;
	case D3DFMT_A8:
		return "D3DFMT_A8";
		break;
	case D3DFMT_A8R3G3B2:
		return "D3DFMT_A8R3G3B2";
		break;
	case D3DFMT_X4R4G4B4:
		return "D3DFMT_X4R4G4B4";
		break;
	case D3DFMT_A2B10G10R10:
		return "D3DFMT_A2B10G10R10";
		break;
	case D3DFMT_A8B8G8R8:
		return "D3DFMT_A8B8G8R8";
		break;
	case D3DFMT_X8B8G8R8:
		return "D3DFMT_X8B8G8R8";
		break;
	case D3DFMT_G16R16:
		return "D3DFMT_G16R16";
		break;
	case D3DFMT_A2R10G10B10:
		return "D3DFMT_A2R10G10B10";
		break;
	case D3DFMT_A16B16G16R16:
		return "D3DFMT_A16B16G16R16";
		break;

	case D3DFMT_A8P8:
		return "D3DFMT_A8P8";
		break;
	case D3DFMT_P8:
		return "D3DFMT_P8";
		break;

	case D3DFMT_L8:
		return "D3DFMT_L8";
		break;
	case D3DFMT_A8L8:
		return "D3DFMT_A8L8";
		break;
	case D3DFMT_A4L4:
		return "D3DFMT_A4L4";
		break;

	case D3DFMT_V8U8:
		return "D3DFMT_V8U8";
		break;
	case D3DFMT_L6V5U5:
		return "D3DFMT_L6V5U5";
		break;
	case D3DFMT_X8L8V8U8:
		return "D3DFMT_X8L8V8U8";
		break;
	case D3DFMT_Q8W8V8U8:
		return "D3DFMT_Q8W8V8U8";
		break;
	case D3DFMT_V16U16:
		return "D3DFMT_V16U16";
		break;
	case D3DFMT_A2W10V10U10:
		return "D3DFMT_A2W10V10U10";
		break;

	case D3DFMT_UYVY:
		return "D3DFMT_UYVY";
		break;
	case D3DFMT_R8G8_B8G8:
		return "D3DFMT_R8G8_B8G8";
		break;
	case D3DFMT_YUY2:
		return "D3DFMT_YUY2";
		break;
	case D3DFMT_G8R8_G8B8:
		return "D3DFMT_G8R8_G8B8";
		break;
	case D3DFMT_DXT1:
		return "D3DFMT_DXT1";
		break;
	case D3DFMT_DXT2:
		return "D3DFMT_DXT2";
		break;
	case D3DFMT_DXT3:
		return "D3DFMT_DXT3";
		break;
	case D3DFMT_DXT4:
		return "D3DFMT_DXT4";
		break;
	case D3DFMT_DXT5:
		return "D3DFMT_DXT5";
		break;

	case D3DFMT_D16_LOCKABLE:
		return "D3DFMT_D16_LOCKABLE";
		break;
	case D3DFMT_D32:
		return "D3DFMT_D32";
		break;
	case D3DFMT_D15S1:
		return "D3DFMT_D15S1";
		break;
	case D3DFMT_D24S8:
		return "D3DFMT_D24S8";
		break;
	case D3DFMT_D24X8:
		return "D3DFMT_D24X8";
		break;
	case D3DFMT_D24X4S4:
		return "D3DFMT_D24X4S4";
		break;
	case D3DFMT_D16:
		return "D3DFMT_D16";
		break;

	case D3DFMT_D32F_LOCKABLE:
		return "D3DFMT_D32F_LOCKABLE";
		break;
	case D3DFMT_D24FS8:
		return "D3DFMT_D24FS8";
		break;

#if !defined(D3D_DISABLE_9EX)
	case D3DFMT_D32_LOCKABLE:
		return "D3DFMT_D32_LOCKABLE";
		break;
	case D3DFMT_S8_LOCKABLE:
		return "D3DFMT_S8_LOCKABLE";
		break;
#endif // !D3D_DISABLE_9EX

	case D3DFMT_L16:
		return "D3DFMT_L16";
		break;

	case D3DFMT_VERTEXDATA:
		return "D3DFMT_VERTEXDATA";
		break;
	case D3DFMT_INDEX16:
		return "D3DFMT_INDEX16";
		break;
	case D3DFMT_INDEX32:
		return "D3DFMT_INDEX32";
		break;

	case D3DFMT_Q16W16V16U16:
		return "D3DFMT_Q16W16V16U16";
		break;

	case D3DFMT_MULTI2_ARGB8:
		return "D3DFMT_MULTI2_ARGB8";
		break;

	case D3DFMT_R16F:
		return "D3DFMT_R16F";
		break;
	case D3DFMT_G16R16F:
		return "D3DFMT_G16R16F";
		break;
	case D3DFMT_A16B16G16R16F:
		return "D3DFMT_A16B16G16R16F";
		break;

	case D3DFMT_R32F:
		return "D3DFMT_R32F";
		break;
	case D3DFMT_G32R32F:
		return "D3DFMT_G32R32F";
		break;
	case D3DFMT_A32B32G32R32F:
		return "D3DFMT_A32B32G32R32F";
		break;

	case D3DFMT_CxV8U8:
		return "D3DFMT_CxV8U8";
		break;

#if !defined(D3D_DISABLE_9EX)
	case D3DFMT_A1:
		return "D3DFMT_A1";
		break;
	case D3DFMT_A2B10G10R10_XR_BIAS:
		return "D3DFMT_A2B10G10R10_XR_BIAS";
		break;
	case D3DFMT_BINARYBUFFER:
		return "D3DFMT_BINARYBUFFER";
		break;
#endif // !D3D_DISABLE_9EX

	case D3DFMT_FORCE_DWORD:
		return "D3DFMT_FORCE_DWORD";
		break;
	default:
		return "";
		break;
	};
}

std::string ToString( D3DPOOL pool )
{

	switch(pool)
	{
	case D3DPOOL_DEFAULT:
		return "D3DPOOL_DEFAULT";
		break;
	case D3DPOOL_MANAGED:
		return "D3DPOOL_MANAGED";
		break;
	case D3DPOOL_SYSTEMMEM:
		return "D3DPOOL_SYSTEMMEM";
		break;
	case D3DPOOL_SCRATCH:
		return "D3DPOOL_SCRATCH";
		break;
	case D3DPOOL_FORCE_DWORD:
		return "D3DPOOL_FORCE_DWORD";
		break;
	default:
		std::stringstream tempstr;
		tempstr << "(Unknown pool type: " << pool << ")";
		return tempstr.str();
	}
}

std::string ToString( HRESULT result )
{
	switch(result)
	{
	//case S_OK:
	case D3D_OK:
		return "D3D_OK";
		break;
	
	case E_NOINTERFACE:
		return "E_NOINTERFACE";
		break;
	case E_OUTOFMEMORY:
		return "E_OUTOFMEMORY";
		break;
	case D3DERR_WRONGTEXTUREFORMAT:
		return "D3DERR_WRONGTEXTUREFORMAT";
		break;
	case D3DERR_UNSUPPORTEDCOLOROPERATION:
		return "D3DERR_UNSUPPORTEDCOLOROPERATION";
		break;
	case D3DERR_UNSUPPORTEDCOLORARG:
		return "D3DERR_UNSUPPORTEDCOLORARG";
		break;
	case D3DERR_UNSUPPORTEDALPHAOPERATION:
		return "D3DERR_UNSUPPORTEDALPHAOPERATION";
		break;
	case D3DERR_UNSUPPORTEDALPHAARG:
		return "D3DERR_UNSUPPORTEDALPHAARG";
		break;
	case D3DERR_TOOMANYOPERATIONS:
		return "D3DERR_TOOMANYOPERATIONS";
		break;
	case D3DERR_CONFLICTINGTEXTUREFILTER:
		return "D3DERR_CONFLICTINGTEXTUREFILTER";
		break;
	case D3DERR_UNSUPPORTEDFACTORVALUE:
		return "D3DERR_UNSUPPORTEDFACTORVALUE";
		break;
	case D3DERR_CONFLICTINGRENDERSTATE:
		return "D3DERR_CONFLICTINGRENDERSTATE";
		break;
	case D3DERR_UNSUPPORTEDTEXTUREFILTER:
		return "D3DERR_UNSUPPORTEDTEXTUREFILTER";
		break;
	case D3DERR_CONFLICTINGTEXTUREPALETTE:
		return "D3DERR_CONFLICTINGTEXTUREPALETTE";
		break;
	case D3DERR_DRIVERINTERNALERROR:
		return "D3DERR_DRIVERINTERNALERROR";
		break;

	case D3DERR_NOTFOUND:
		return "D3DERR_NOTFOUND";
		break;
	case D3DERR_MOREDATA:
		return "D3DERR_MOREDATA";
		break;
	case D3DERR_DEVICELOST:
		return "D3DERR_DEVICELOST";
		break;
	case D3DERR_DEVICENOTRESET:
		return "D3DERR_DEVICENOTRESET";
		break;
	case D3DERR_NOTAVAILABLE:
		return "D3DERR_NOTAVAILABLE";
		break;
	case D3DERR_OUTOFVIDEOMEMORY:
		return "D3DERR_OUTOFVIDEOMEMORY";
		break;
	case D3DERR_INVALIDDEVICE:
		return "D3DERR_INVALIDDEVICE";
		break;
	case D3DERR_INVALIDCALL:
		return "D3DERR_INVALIDCALL";
		break;
	case D3DERR_DRIVERINVALIDCALL:
		return "D3DERR_DRIVERINVALIDCALL";
		break;
	case D3DERR_WASSTILLDRAWING:
		return "D3DERR_WASSTILLDRAWING";
		break;
	case D3DOK_NOAUTOGEN:
		return "D3DOK_NOAUTOGEN";
		break;

		/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
	case D3DERR_DEVICEREMOVED:
		return "D3DERR_DEVICEREMOVED";
		break;
	case S_NOT_RESIDENT:
		return "S_NOT_RESIDENT";
		break;
	case S_RESIDENT_IN_SHARED_MEMORY:
		return "S_RESIDENT_IN_SHARED_MEMORY";
		break;
	case S_PRESENT_MODE_CHANGED:
		return "S_PRESENT_MODE_CHANGED";
		break;
	case S_PRESENT_OCCLUDED:
		return "S_PRESENT_OCCLUDED";
		break;
	case D3DERR_DEVICEHUNG:
		return "D3DERR_DEVICEHUNG";
		break;
	case D3DERR_UNSUPPORTEDOVERLAY:
		return "D3DERR_UNSUPPORTEDOVERLAY";
		break;
	case D3DERR_UNSUPPORTEDOVERLAYFORMAT:
		return "D3DERR_UNSUPPORTEDOVERLAYFORMAT";
		break;
	case D3DERR_CANNOTPROTECTCONTENT:
		return "D3DERR_CANNOTPROTECTCONTENT";
		break;
	case D3DERR_UNSUPPORTEDCRYPTO:
		return "D3DERR_UNSUPPORTEDCRYPTO";
		break;
	case D3DERR_PRESENT_STATISTICS_DISJOINT:
		return "D3DERR_PRESENT_STATISTICS_DISJOINT";
		break;
#endif
	default:
		std::stringstream tempstr;
		tempstr << "(Unknown HRESULT: " << result << ")";
		return tempstr.str();


	}
}

std::string D3DUSAGEToString( DWORD usage )
{
	std::stringstream outputStream;
	bool OrFlag = false;

	if(usage & D3DUSAGE_AUTOGENMIPMAP)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_AUTOGENMIPMAP";
		OrFlag = true;
	}

	if(usage & D3DUSAGE_DEPTHSTENCIL)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_DEPTHSTENCIL";
		OrFlag = true;
	}
	
	if(usage & D3DUSAGE_DMAP)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_DMAP";
		OrFlag = true;
	}

	if(usage & D3DUSAGE_DONOTCLIP)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_DONOTCLIP";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_DYNAMIC)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_DYNAMIC";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_NONSECURE)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_NONSECURE";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_NPATCHES)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_NPATCHES";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_POINTS)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_POINTS";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_RENDERTARGET)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_RENDERTARGET";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_RTPATCHES)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_RTPATCHES";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_SOFTWAREPROCESSING)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_SOFTWAREPROCESSING";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_TEXTAPI)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_TEXTAPI";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_WRITEONLY)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_WRITEONLY";
		OrFlag = true;
	}
	/* D3D9Ex only -- */
#if !defined(D3D_DISABLE_9EX)
	if(usage & D3DUSAGE_RESTRICTED_CONTENT)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_RESTRICTED_CONTENT";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_RESTRICT_SHARED_RESOURCE)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_RESTRICT_SHARED_RESOURCE";
		OrFlag = true;
	}
	if(usage & D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER)
	{
		outputStream << (OrFlag ? "|" : "") << "D3DUSAGE_RESTRICT_SHARED_RESOURCE_DRIVER";
		OrFlag = true;
	}
#endif

	return outputStream.str();

}
