#pragma once

#include "../../GameInfo.h"

class CTextureManager
{
	friend class CResourceManager;

private:
	CTextureManager();
	~CTextureManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CTexture>> m_mapTexture;
	std::unordered_map<std::string, ID3D11SamplerState*> m_mapSampler;
	class CWidgetConstantBuffer* m_CBuffer;

public:
	bool Init();

	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

	class CTexture* FindTexture(const std::string& name);
	void ReleaseTexture(const std::string& name);

public:
	void RenderTarget(class CMesh* mesh, class CShader* shader);
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat);

public:
	bool CreateSampler(const std::string name, D3D11_FILTER Filter,
		D3D11_TEXTURE_ADDRESS_MODE AddressU,
		D3D11_TEXTURE_ADDRESS_MODE AddressV,
		D3D11_TEXTURE_ADDRESS_MODE AddressW,
		float BorderColor[4]);
	ID3D11SamplerState* FindSampler(const std::string& name);
	void SetSampler(const std::string& name, int registerNum, int shaderType = (int)Buffer_Shader_Type::All);
};

