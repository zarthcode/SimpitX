// file name:	capplication.h
// author:		Victor Saar, Ingmar Rötzler
// e-mail:		vsaar@web.de, ingmar.r@web.de
// web site:	http://www.two-kings.de

#ifndef capplication_h
#define capplication_h

#include"main.h"

class CApplication
{
public:
								CApplication(void);
								~CApplication(void);

	void						InitWindow(void);
	void						InitD3D(void);
	void						InitScene(void);
	void						CheckDeviceCaps(void);
	void						SaveSettings(void);
	void						LoadSettings(void);

	bool						CheckDevice(void);

	void						KillWindow(void);
	void						KillD3D(void);

	inline bool					GetWindowStatus(void)						{ return m_bRunningWindow; }
	inline bool					GetD3DStatus(void)							{ return m_bRunningD3D; }
	inline LPDIRECT3DDEVICE9	GetDevice(void)								{ return m_pDirect3DDevice; }
	inline HWND					GetWindowHandle(void)						{ return m_hWindow; }
	inline DWORD				GetWidth(void)								{ return m_dwWidth; }
	inline DWORD				GetHeight(void)								{ return m_dwHeight; }

	inline void					SetWindowStatus(bool bRunningWindow)		{ m_bRunningWindow = bRunningWindow; }
	inline void					SetD3DStatus(bool bRunningD3D)				{ m_bRunningD3D = bRunningD3D; }

private:
	bool						m_bRunningWindow,
								m_bRunningD3D;

	HWND						m_hWindow,
								m_hBtnStart,
								m_hBtnCancel,
								m_hLblResolution,
								m_hCbResolution,
								m_hLblBackBuffer,
								m_hCbBackBuffer,
								m_hLblDepthStencil,
								m_hCbDepthStencil,
								m_hLblVertexProcessing,
								m_hCbVertexProcessing,
								m_hLblMultiSampling,
								m_hCbMultiSampling,
								m_hLblAnisotropy,
								m_hCbAnisotropy;

	LPDIRECT3D9					m_pDirect3DObject;
	LPDIRECT3DDEVICE9			m_pDirect3DDevice;
	D3DPRESENT_PARAMETERS		m_PresentParameters;
	D3DCAPS9					m_DeviceCaps;

	DWORD						m_dwWidth,
								m_dwHeight,
								m_dwVertexProcessing,
								m_dwAnisotropy;
	D3DFORMAT					m_ColorFormat,
								m_DepthStencilFormat;
	D3DMULTISAMPLE_TYPE			m_MultiSampling;

	D3DXMATRIX					m_matProjection;
	float						m_fAspectRatio,
								m_fFieldOfView,
								m_fNearPlane,
								m_fFarPlane;
};

#endif