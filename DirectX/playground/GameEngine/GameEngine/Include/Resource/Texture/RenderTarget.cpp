#include "RenderTarget.h"
#include "../../Device.h"

CRenderTarget::CRenderTarget() :
	m_TargetView(nullptr),
	m_TargetTex(nullptr),
	m_PrevTargetView(nullptr),
	m_PrevDephView(nullptr),
	m_Surface(nullptr),
	m_ClearColor{},
	m_DebugRender(false)
{
}

CRenderTarget::~CRenderTarget()
{
	SAFE_RELEASE(m_Surface);
	SAFE_RELEASE(m_PrevDephView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_TargetTex);
	SAFE_RELEASE(m_TargetView);
}

bool CRenderTarget::CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat)
{
	SetName(name);

	// Target¿ë Texture »ý¼º
	D3D11_TEXTURE2D_DESC desc = {};

	desc.Width = width;
	desc.Height = height;
	desc.ArraySize = 1;
	desc.MipLevels = 1;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	desc.Format = pixelFormat;
	desc.Usage = D3D11_USAGE_DEFAULT;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateTexture2D(&desc, nullptr, &m_TargetTex)))
		return false;

	sTextureResourceInfo* info = new sTextureResourceInfo;

	info->width = width;
	info->height = height;
	m_vecTextureInfo.push_back(info);

	m_TargetTex->QueryInterface(__uuidof(IDXGISurface), (void**)&m_Surface);

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateShaderResourceView(m_TargetTex, nullptr, &info->SRV)))
		return false;

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateRenderTargetView(m_TargetTex, nullptr, &m_TargetView)))
		return false;

	return true;
}

void CRenderTarget::ClearTarget()
{
	CDevice::GetInst()->GetContext()->ClearRenderTargetView(m_TargetView, m_ClearColor);
}

void CRenderTarget::SetTarget(ID3D11DepthStencilView* depthView)
{
	CDevice::GetInst()->GetContext()->OMGetRenderTargets(1, &m_PrevTargetView, &m_PrevDephView);

	ID3D11DepthStencilView* depth = m_PrevDephView;

	if (depthView)
		depth = depthView;

	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_TargetView, depth);
}

void CRenderTarget::ResetTarget()
{
	CDevice::GetInst()->GetContext()->OMSetRenderTargets(1, &m_PrevTargetView, m_PrevDephView);
	SAFE_RELEASE(m_PrevTargetView);
	SAFE_RELEASE(m_PrevDephView);
}

void CRenderTarget::SetTargetShader()
{
}

void CRenderTarget::ResetTargetShader()
{
}
