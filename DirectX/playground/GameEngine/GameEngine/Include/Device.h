#pragma once

#include "GameInfo.h"

class CDevice
{
private:
	// Device, DeviceContext�� �������� �׷��� ��ġ �ϵ��� ���� ����Ʈ���� ����� ��� �����ϸ� �ȴ�.
	// Device�� ��� ��� ���� ���˰� �ڿ� �Ҵ翡 ���δ�.
	ID3D11Device* m_Device;
	// Context�� ���� ����� �����ϰ� �ڿ��� �׷��� ���������ο� ���� GPU�� ������ ��ɵ��� �����ϴµ� ���δ�.
	ID3D11DeviceContext* m_Context;
	// �� ���� �ø����� ���� ���ȴ�.
	IDXGISwapChain* m_SwapChain;


	// BackBuffer�� �ȼ��� ���� ���� RenderTargetView�� �����
	// BackBuffer�� �� TargetView�� �������ش�.
	ID3D11RenderTargetView* m_TargetView;

	// ����, ���ٽ� ���� �������ؼ� DepthStencilView�� ����� ����
	ID3D11DepthStencilView* m_DepthView;

	HWND m_hWnd;
	sResolution m_RS;

private:
	ID2D1RenderTarget* m_2DTarget;
	ID2D1RenderTarget* m_2DTargetWorld;
	ID2D1Factory* m_2DFactory;

public:
	inline ID2D1RenderTarget* Get2DRenderTarget() const
	{
		return m_2DTarget;
	}

	inline ID2D1RenderTarget* Get2DWorldRenderTarget() const
	{
		return m_2DTargetWorld;
	}

	inline ID2D1Factory* Get2DFactory() const
	{
		return m_2DFactory;
	}


	inline ID3D11Device* GetDevice() const
	{
		return m_Device;
	}

	inline ID3D11DeviceContext* GetContext() const
	{
		return m_Context;
	}

	inline IDXGISwapChain* GetSwapChain() const
	{
		return m_SwapChain;
	}

	inline sResolution GetResolution() const
	{
		return m_RS;
	}

	Vector2 GetViewportAspectRatio() const;

public:
	bool Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode);
	void ClearRenderTarget(float clearColors[4]);
	void ClearDepthStencil(float depth, unsigned char stencil);
	void RenderStart();
	void Flip();

	DECLARE_SINGLE(CDevice)
};

