#include "Material.h"
#include "../../Scene/Scene.h"
#include "../ResourceManager.h"
#include "../../Scene/SceneResource.h"
#include "../../Render/RenderManager.h"
#include "../../Render/RenderState.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterial::CMaterial() :
	m_BaseColor(Vector4::White),
	m_AmbientColor(Vector4(0.2f, 0.2f, 0.2f, 1.0f)),
	m_SpecularColor(Vector4::White),
	m_EmissiveColor(Vector4::Black),
	m_Animation3D(false),
	m_SpecularTex(false),
	m_EmissiveTex(false),
	m_Bump(false),
	m_Scene(nullptr),
	m_CBuffer(nullptr),
	m_Opacity(1.0f),
	m_RenderStateArray{}
{
	SetTypeID<CMaterial>();
}

CMaterial::CMaterial(const CMaterial& material)
{
	// 얕은 복사를 이용한다.
	*this = material;

	// 새롭게 복사생성된 마테리얼이므로 참조수는 0
	m_RefCount = 0;

	m_CBuffer = material.m_CBuffer->Clone();

	m_Scene = nullptr;

	m_RenderCallback.clear();
}

CMaterial::~CMaterial()
{
	auto iter = m_RenderCallback.begin();
	auto iterEnd = m_RenderCallback.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	SAFE_DELETE(m_CBuffer);
}

void CMaterial::CreateConstantBuffer()
{
	SAFE_DELETE(m_CBuffer);

	m_CBuffer = new CMaterialConstantBuffer;

	m_CBuffer->Init();
}

void CMaterial::EnableBump()
{
	m_Bump = true;

	m_CBuffer->SetBump(true);
}

void CMaterial::EnableAnimation3D()
{
	m_Animation3D = true;

	m_CBuffer->SetAnimation3D(true);
}

void CMaterial::EnableSpecularTex()
{
	m_SpecularTex = true;

	m_CBuffer->SetSpecularTex(true);
}

void CMaterial::EnableEmissiveTex()
{
	m_EmissiveTex = true;

	m_CBuffer->SetEmissiveTex(true);
}

void CMaterial::SetRenderState(CRenderState* state)
{
	m_RenderStateArray[(int)state->GetType()] = state;
}

void CMaterial::SetRenderState(const std::string& name)
{
	CRenderState* state = CRenderManager::GetInst()->FindRenderState(name);
	m_RenderStateArray[(int)state->GetType()] = state;
}

void CMaterial::SetTransparency(bool enable)
{
	if (enable)
		m_RenderStateArray[(int)RenderState_Type::Blend] = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

void CMaterial::SetOpacity(float opacity)
{
	m_Opacity = opacity;

	if (m_Opacity < 0.0f)
		m_Opacity = 0.0f;
	else if (m_Opacity > 1.0f)
		m_Opacity = 1.0f;
}

void CMaterial::AddOpacity(float opacity)
{
	m_Opacity += opacity;

	if (m_Opacity < 0.0f)
		m_Opacity = 0.0f;
	else if (m_Opacity > 1.0f)
		m_Opacity = 1.0f;
}

void CMaterial::SetBaseColor(const Vector4& color)
{
	m_BaseColor = color;
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);
}

void CMaterial::SetAmbientColor(const Vector4& color)
{
	m_AmbientColor = color;
}

void CMaterial::SetAmbientColor(float r, float g, float b, float a)
{
	m_AmbientColor = Vector4(r, g, b, a);
}

void CMaterial::SetSpecularColor(const Vector4& color)
{
	m_SpecularColor = color;
}

void CMaterial::SetSpecularColor(float r, float g, float b, float a)
{
	m_SpecularColor = Vector4(r, g, b, a);
}

void CMaterial::SetEmissiveColor(const Vector4& color)
{
	m_EmissiveColor = color;
}

void CMaterial::SetEmissiveColor(float r, float g, float b, float a)
{
	m_EmissiveColor = Vector4(r, g, b, a);
}

void CMaterial::SetSpecularPower(float power)
{
	m_SpecularColor.w = power;
}

void CMaterial::AddTexture(int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	int index = (int)m_vecTextureInfo.size() - 1;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	// 어차피 같은 텍스처를 공유한다.
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	// 만약 씬에 소속되어 있지 않다면 리소스 매니저에 직접 접근해서 가져온다.
	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	// 만약 씬에 소속되어있다면
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	int index = (int)m_vecTextureInfo.size() - 1;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::AddTextureFullPath(int registerNum, int shaderType, 
	const std::string& name, const TCHAR* fullPath)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(name, fullPath))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	int index = (int)m_vecTextureInfo.size() - 1;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::AddTexture(int registerNum, int shaderType, const std::string& name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(name, vecFileName, pathName))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	int index = (int)m_vecTextureInfo.size() - 1;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::AddTextureArrayFullPath(int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTextureArrayFullPath(name, vecFullPath))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	m_vecTextureInfo.back().registerNum = registerNum;
	m_vecTextureInfo.back().name = name;
	m_vecTextureInfo.back().texture = texture;
	m_vecTextureInfo.back().shaderType = shaderType;
}

void CMaterial::AddTextureArray(int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTextureArray(name, vecFileName, pathName))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.push_back(sMaterialTextureInfo());

	m_vecTextureInfo.back().registerNum = registerNum;
	m_vecTextureInfo.back().name = name;
	m_vecTextureInfo.back().texture = texture;
	m_vecTextureInfo.back().shaderType = shaderType;
}

void CMaterial::SetTexture(int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::SetTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::SetTextureFullPath(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
		return;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = CResourceManager::GetInst()->FindTexture(name);
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::SetTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	/*if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
		return;

	m_vecTextureInfo[index].registerNum = registerNum;
	m_vecTextureInfo[index].name = name;
	m_vecTextureInfo[index].texture = CResourceManager::GetInst()->FindTexture(name);
	m_vecTextureInfo[index].shaderType = shaderType;*/
}

void CMaterial::SetTextureArrayFullPath(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTextureArrayFullPath(name, vecFullPath))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTextureArrayFullPath(name, vecFullPath))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.back().registerNum = registerNum;
	m_vecTextureInfo.back().name = name;
	m_vecTextureInfo.back().texture = CResourceManager::GetInst()->FindTexture(name);
	m_vecTextureInfo.back().shaderType = shaderType;
}

void CMaterial::SetTextureArray(int index, int registerNum, int shaderType, 
	const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTextureArray(name, vecFileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	else
	{
		if (!m_Scene->GetSceneResource()->LoadTextureArray(name, vecFileName, pathName))
			return;

		texture = m_Scene->GetSceneResource()->FindTexture(name);
	}

	m_vecTextureInfo.back().registerNum = registerNum;
	m_vecTextureInfo.back().name = name;
	m_vecTextureInfo.back().texture = CResourceManager::GetInst()->FindTexture(name);
	m_vecTextureInfo.back().shaderType = shaderType;
}

void CMaterial::SetPaperBurn(bool enable)
{
	m_CBuffer->SetPaperBurn(enable);
}

void CMaterial::SetShader(const std::string& name)
{
	// 리소스매니저의 중계를 받아 쉐이더를 가져온다.
	m_Shader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(name);
}

// 렌더시 쉐이더가 존재한다면 이용해서 그려낸다.
void CMaterial::Render()
{
	if (m_Shader)
		m_Shader->SetShader();

	// 만약 상수버퍼가 존재한다면 상수버퍼를 통해서 색깔을 업데이트한다.
	if (m_CBuffer)
	{
		m_CBuffer->SetBaseColor(m_BaseColor);
		m_CBuffer->SetAmbientColor(m_AmbientColor);
		m_CBuffer->SetSpecularColor(m_SpecularColor);
		m_CBuffer->SetEmissiveColor(m_EmissiveColor);
		m_CBuffer->SetOpacity(m_Opacity);

		m_CBuffer->UpdateCBuffer();
	}

	for (int i = 0; i < (int)RenderState_Type::MAX; i++)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->SetState();
	}

	// 모든 텍스처를 순회하면서 같이 쉐이더를 적용해 그린다.
	size_t size = m_vecTextureInfo.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecTextureInfo[i].texture->SetShader(m_vecTextureInfo[i].registerNum, m_vecTextureInfo[i].shaderType, 0);
	}

	auto iter = m_RenderCallback.begin();
	auto iterEnd = m_RenderCallback.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->func();
	}
}

void CMaterial::Reset()
{
	size_t size = m_vecTextureInfo.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecTextureInfo[i].texture->ResetShader(m_vecTextureInfo[i].registerNum, m_vecTextureInfo[i].shaderType, 0);
	}

	for (int i = 0; i < (int)RenderState_Type::MAX; i++)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->ResetState();
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}

void CMaterial::Save(FILE* pFile)
{
	// 쉐이더는 이름으로 저장한다.
	std::string shaderName = m_Shader->GetName();

	int length = (int)shaderName.length();

	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(shaderName.c_str(), sizeof(char), length, pFile);

	fwrite(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_AmbientColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_SpecularColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_EmissiveColor, sizeof(Vector4), 1, pFile);
	fwrite(&m_Opacity, sizeof(float), 1, pFile);
	fwrite(&m_Animation3D, sizeof(bool), 1, pFile);
	fwrite(&m_SpecularTex, sizeof(bool), 1, pFile);
	fwrite(&m_EmissiveTex, sizeof(bool), 1, pFile);
	fwrite(&m_Bump, sizeof(bool), 1, pFile);

	// 렌더 스테이트도 이름으로 저장한다.
	for (int i = 0; i < (int)RenderState_Type::MAX; i++)
	{
		bool stateEnable = false;

		if (m_RenderStateArray[i])
			stateEnable = true;

		fwrite(&stateEnable, sizeof(bool), 1, pFile);

		if (m_RenderStateArray[i])
		{
			std::string stateName = m_RenderStateArray[i]->GetName();

			length = (int)stateName.length();

			fwrite(&length, sizeof(int), 1, pFile);
			fwrite(stateName.c_str(), sizeof(char), length, pFile);
		}
	}

	// 현재 마테리얼이 가지고있는 텍스쳐 정보들을 모두 저장한다.
	int textureCount = (int)m_vecTextureInfo.size();

	fwrite(&textureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < textureCount; i++)
	{
		length = (int)m_vecTextureInfo[i].name.length();

		// 텍스쳐의 이름을 저장한다.
		fwrite(&length, sizeof(int), 1, pFile);
		fwrite(m_vecTextureInfo[i].name.c_str(), sizeof(char), length, pFile);

		// 텍스처의 레지스터번호와 타입들을 저정한다.
		fwrite(&m_vecTextureInfo[i].samplerType, sizeof(Sampler_Type), 1, pFile);
		fwrite(&m_vecTextureInfo[i].registerNum, sizeof(int), 1, pFile);
		fwrite(&m_vecTextureInfo[i].shaderType, sizeof(int), 1, pFile);

		m_vecTextureInfo[i].texture->Save(pFile);
	}
}

void CMaterial::Load(FILE* pFile)
{
	CreateConstantBuffer();

	// 쉐이더 이름을 받아온다.
	char shaderName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(shaderName, sizeof(char), length, pFile);

	// 쉐이더는 이름으로 찾아와서 초기화한다.
	m_Shader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(shaderName);

	fread(&m_BaseColor, sizeof(Vector4), 1, pFile);
	fread(&m_AmbientColor, sizeof(Vector4), 1, pFile);
	fread(&m_SpecularColor, sizeof(Vector4), 1, pFile);
	fread(&m_EmissiveColor, sizeof(Vector4), 1, pFile);
	fread(&m_Opacity, sizeof(float), 1, pFile);
	fread(&m_Animation3D, sizeof(bool), 1, pFile);
	fread(&m_SpecularTex, sizeof(bool), 1, pFile);
	fread(&m_EmissiveTex, sizeof(bool), 1, pFile);
	fread(&m_Bump, sizeof(bool), 1, pFile);

	m_CBuffer->SetAnimation3D(m_Animation3D);
	m_CBuffer->SetBump(m_Bump);
	m_CBuffer->SetSpecularTex(m_SpecularTex);
	m_CBuffer->SetEmissiveTex(m_EmissiveTex);

	// 렌더스테이트 정보를 불러온다.
	for (int i = 0; i < (int)RenderState_Type::MAX; i++)
	{
		bool stateEnable = false;

		// 만약 렌더스테이트가 활성화되어 있으면
		fread(&stateEnable, sizeof(bool), 1, pFile);

		if (stateEnable)
		{
			// 스테이트의 이름을 가져온다.
			char stateName[256] = {};
			length = 0;

			fread(&length, sizeof(int), 1, pFile);
			fread(stateName, sizeof(char), length, pFile);

			// 이름으로 렌더 스테이트를 찾아오고 초기화시킨다.
			m_RenderStateArray[i] = CRenderManager::GetInst()->FindRenderState(stateName);
		}
	}

	// 모든 텍스쳐의 정보를 가져온다.
	int textureCount = 0;

	fread(&textureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < textureCount; i++)
	{
		m_vecTextureInfo.push_back(sMaterialTextureInfo());

		length = 0;

		char textureName[256] = {};

		// 텍스쳐의 이름을 저장한다.
		fread(&length, sizeof(int), 1, pFile);
		fread(textureName, sizeof(char), length, pFile);

		m_vecTextureInfo[i].name = textureName;

		// 저장되어있던 텍스쳐의 세부 정보를 가져온다.
		fread(&m_vecTextureInfo[i].samplerType, sizeof(Sampler_Type), 1, pFile);
		fread(&m_vecTextureInfo[i].registerNum, sizeof(int), 1, pFile);
		fread(&m_vecTextureInfo[i].shaderType, sizeof(int), 1, pFile);

		// 텍스처의 이름을 가져온다.
		int texNameLength = 0;
		char texName[256] = {};

		fread(&texNameLength, sizeof(int), 1, pFile);
		fread(texName, sizeof(char), texNameLength, pFile);

		// 이미지의 타입을 가져온다.
		Image_Type imageType;
		fread(&imageType, sizeof(Image_Type), 1, pFile);

		// 현재 텍스처 정보가 들어있는 수
		int infoCount = 0;

		fread(&infoCount, sizeof(int), 1, pFile);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string pathName;

		// 수만큼 반복하면서 모든 텍스처의 완전경로, 이름등을 받아온다.
		for (int i = 0; i < infoCount; i++)
		{
			int pathSize = 0;

			fread(&pathSize, sizeof(int), 1, pFile);

			TCHAR fullPath[MAX_PATH] = {};
			fread(fullPath, sizeof(TCHAR), pathSize, pFile);
			vecFullPath.push_back(fullPath);

			fread(&pathSize, sizeof(int), 1, pFile);

			TCHAR texFileName[MAX_PATH] = {};
			fread(texFileName, sizeof(TCHAR), pathSize, pFile);
			vecFileName.push_back(texFileName);

			fread(&pathSize, sizeof(int), 1, pFile);

			char texPathName[MAX_PATH] = {};
			fread(texPathName, sizeof(char), pathSize, pFile);

			pathName = texPathName;
		}

		switch (imageType)
		{
		case Image_Type::Atlas:
		{
			// 파일의 이름이 하나만 존재할경우
			// 텍스처는 하나만 있다는 뜻
			if (vecFileName.size() == 1)
			{
				if (m_Scene)
				{
					m_Scene->GetSceneResource()->LoadTexture(texName, vecFileName[0].c_str(), pathName);
				}
				else
				{
					CResourceManager::GetInst()->LoadTexture(texName, vecFileName[0].c_str(), pathName);
				}
			}
		}
			break;
		case Image_Type::Frame:
			if (vecFileName.size() == 1)
			{

			}
			else
			{

			}
			break;
		case Image_Type::Array:
			if (vecFileName.size() == 1)
			{

			}
			else
			{

			}
			break;
		}

		// 위에서 로드한 텍스쳐로 설정한다.
		if (m_Scene)
		{
			m_vecTextureInfo[i].texture = m_Scene->GetSceneResource()->FindTexture(texName);
		}
		else
		{
			m_vecTextureInfo[i].texture = CResourceManager::GetInst()->FindTexture(texName);
		}
	}
}
