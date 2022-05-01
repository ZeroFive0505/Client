#include "TextureManager.h"
#include "RenderTarget.h"
#include "../../Device.h"

CTextureManager::CTextureManager()
{
}

CTextureManager::~CTextureManager()
{
	auto iter = m_mapSampler.begin();
	auto iterEnd = m_mapSampler.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_RELEASE(iter->second);
	}
}

bool CTextureManager::Init()
{
	// 엔진에서 Texture지정이 안되었을 경우 기본으로 사용되는 Texture를 로딩한다.
	LoadTexture("DefaultTexture", TEXT("awesomeface.png"));

	float borderColor[4] = {};

	borderColor[0] = 1.0f;
	borderColor[1] = 1.0f;
	borderColor[2] = 1.0f;
	borderColor[3] = 1.0f;


	if (!CreateSampler("Point", D3D11_FILTER_MIN_MAG_MIP_POINT,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		borderColor))
		return false;

	if (!CreateSampler("Linear", D3D11_FILTER_MIN_MAG_MIP_LINEAR,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		borderColor))
		return false;

	if (!CreateSampler("Anisotropic", D3D11_FILTER_ANISOTROPIC,
		D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP,
		borderColor))
		return false;

	// 쉐이더 코드의 알맞은 레지스터 번호에 샘플러를 설정한다.
	SetSampler("Point", 0);
	SetSampler("Linear", 1);
	SetSampler("Anisotropic", 2);

	// Base Sampler
	SetSampler("Point", 3);

	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTexture(name, fileName, pathName))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

CTexture* CTextureManager::FindTexture(const std::string& name)
{
	auto iter = m_mapTexture.find(name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

bool CTextureManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTextureFullPath(name, fullPath))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

bool CTextureManager::LoadTexture(const std::string& name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTexture(name, vecFileName, pathName))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

bool CTextureManager::LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTextureFullPath(name, vecFullPath))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

void CTextureManager::ReleaseTexture(const std::string& name)
{
	auto iter = m_mapTexture.find(name);

	if (iter != m_mapTexture.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapTexture.erase(iter);
	}
}

bool CTextureManager::CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat)
{
	CRenderTarget* texture = (CRenderTarget*)FindTexture(name);

	if (texture)
		return true;

	texture = new CRenderTarget;

	if (!texture->CreateTarget(name, width, height, pixelFormat))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));
	
	return true;
}

bool CTextureManager::CreateSampler(const std::string name, D3D11_FILTER Filter, D3D11_TEXTURE_ADDRESS_MODE AddressU, D3D11_TEXTURE_ADDRESS_MODE AddressV, D3D11_TEXTURE_ADDRESS_MODE AddressW, float BorderColor[4])
{
	ID3D11SamplerState* sampler = FindSampler(name);

	if (sampler)
		return true;

	D3D11_SAMPLER_DESC desc = {};

	desc.Filter = Filter;
	desc.AddressU = AddressU;
	desc.AddressV = AddressV;
	desc.AddressW = AddressW;
	desc.MipLODBias = 0.0f;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	desc.MinLOD = -FLT_MAX;
	desc.MaxLOD = FLT_MAX;
	desc.BorderColor[0] = BorderColor[0];
	desc.BorderColor[1] = BorderColor[1];
	desc.BorderColor[2] = BorderColor[2];
	desc.BorderColor[3] = BorderColor[3];

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateSamplerState(&desc, &sampler)))
		return false;

	m_mapSampler.insert(std::make_pair(name, sampler));

	return true;
}

ID3D11SamplerState* CTextureManager::FindSampler(const std::string& name)
{
	auto iter = m_mapSampler.find(name);

	if (iter == m_mapSampler.end())
		return nullptr;

	return iter->second;
}

void CTextureManager::SetSampler(const std::string& name, int registerNum, int shaderType)
{
	ID3D11SamplerState* sampler = FindSampler(name);

	if (!sampler)
		return;


	if (shaderType & (int)Buffer_Shader_Type::Vertex)
		CDevice::GetInst()->GetContext()->VSSetSamplers(registerNum, 1, &sampler);

	if (shaderType & (int)Buffer_Shader_Type::Pixel)
		CDevice::GetInst()->GetContext()->PSSetSamplers(registerNum, 1, &sampler);

	if (shaderType & (int)Buffer_Shader_Type::Domain)
		CDevice::GetInst()->GetContext()->DSSetSamplers(registerNum, 1, &sampler);

	if (shaderType & (int)Buffer_Shader_Type::Hull)
		CDevice::GetInst()->GetContext()->HSSetSamplers(registerNum, 1, &sampler);

	if (shaderType & (int)Buffer_Shader_Type::Geometry)
		CDevice::GetInst()->GetContext()->GSSetSamplers(registerNum, 1, &sampler);

	if (shaderType & (int)Buffer_Shader_Type::Compute)
		CDevice::GetInst()->GetContext()->CSSetSamplers(registerNum, 1, &sampler);
}
