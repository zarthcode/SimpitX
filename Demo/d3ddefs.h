// file name:	d3ddefs.h
// author:		Victor Saar
// e-mail:		vsaar@web.de
// web site:	http://www.two-kings.de

#ifndef d3ddefs_h
#define d3ddefs_h

#include"main.h"

//constants
#define D3DFVF_CUSTOMVERTEX	(D3DFVF_XYZ | D3DFVF_TEX1)

//structures
struct D3DVERTEX
{
float	fX,
		fY,
		fZ;
float	fU,
		fV;
};

#endif