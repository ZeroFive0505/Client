#pragma once

#include "Texture.h"

class CRenderTarget :
	public CTexture
{
	friend class CTextureManager;

protected:
	CRenderTarget();
	virtual ~CRenderTarget();

protected:
	ID3D11RenderTargetView* m_TargetView;
	ID3D11Texture2D* m_TargetTex;
	ID3D11RenderTargetView* m_PrevTargetView;
	ID3D11DepthStencilView* m_PrevDephView;
	IDXGISurface* m_Surface;
	float m_ClearColor[4];
	bool m_DebugRender;
	Matrix m_matDebugWVP;
	Vector3 m_Scale;
	Vector3 m_Pos;

public:
	inline IDXGISurface* GetSurface() const
	{
		return m_Surface;
	}

	inline ID3D11RenderTargetView* GetTargetView() const
	{
		return m_TargetView;
	}

	inline Matrix GetWVP() const
	{
		return m_matDebugWVP;
	}

	inline void SetDebugRender(bool render)
	{
		m_DebugRender = render;
	}

	inline void SetPos(const Vector3& pos)
	{
		m_Pos = pos;
	}

	inline void SetScale(const Vector3& scale)
	{
		m_Scale = scale;
	}

public:
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat);
	void ClearTarget();
	void SetTarget(ID3D11DepthStencilView* depthView);
	void ResetTarget();
	void SetTargetShader();
	void ResetTargetShader();
};

