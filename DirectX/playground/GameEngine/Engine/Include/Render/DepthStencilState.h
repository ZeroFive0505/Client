#pragma once

#include "RenderState.h"

/// <summary>
/// DepthStencil 렌더스테이트를 정의한다.
/// 기본값으로는 Depth만 설정되어있다.
/// </summary>
class CDepthStencilState :
	public CRenderState
{
	friend class CRenderStateManager;

protected:
	CDepthStencilState();
	virtual ~CDepthStencilState();

protected:
	UINT m_StencilRef;
	UINT m_PrevStencilRef;

public:
	bool CreateState(bool DepthEnable = true,
		D3D11_DEPTH_WRITE_MASK DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL,
		D3D11_COMPARISON_FUNC DepthFunc = D3D11_COMPARISON_LESS,
		bool StencilEnable = false,
		UINT8 StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK,
		UINT8 StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_DEPTH_STENCILOP_DESC FrontFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS },
		D3D11_DEPTH_STENCILOP_DESC BackFace = { D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP , D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS });

public:
	virtual void SetState();
	virtual void ResetState();
};

