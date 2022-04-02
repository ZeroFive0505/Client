#pragma once

#include "GameInfo.h"

class CDevice
{
private:
	// Device, DeviceContext는 물리적인 그래픽 장치 하드웨어에 대한 소프트웨어 제어기 라고 생각하면 된다.
	// Device의 경우 기능 지원 점검과 자원 할당에 쓰인다.
	ID3D11Device* m_Device;
	// Context는 렌더 대상을 설정하고 자원을 그래픽 파이프라인에 묶고 GPU가 렌더링 명령들을 지시하는데 쓰인다.
	ID3D11DeviceContext* m_Context;
	// 백 버퍼 플립핑을 위해 사용된다.
	IDXGISwapChain* m_SwapChain;


	// BackBuffer에 픽셀을 쓰기 위해 RenderTargetView를 만들고
	// BackBuffer를 이 TargetView에 연결해준다.
	ID3D11RenderTargetView* m_TargetView;

	// 깊이, 스텐슬 값을 쓰기위해서 DepthStencilView를 만들고 연결
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

