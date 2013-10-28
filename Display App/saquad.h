#pragma once
#include <d3d9.h>
#include <atlbase.h>

class ScreenAlignedQuad
{
public:

	ScreenAlignedQuad();
	~ScreenAlignedQuad();

	bool isValid()const { return m_isValid; };
	void Create(IDirect3DDevice9* pD3DDevice, int width, int height);
	void Render();
	void Release();
	
protected:
private:

	CComPtr<IDirect3DVertexBuffer9> m_pVertexBuffer;
	CComPtr<IDirect3DDevice9> m_pD3DDevice; 
	bool m_isValid;

};
