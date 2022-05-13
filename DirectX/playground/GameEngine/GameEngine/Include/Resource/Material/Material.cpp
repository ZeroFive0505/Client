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
	// ���� ���縦 �̿��Ѵ�.
	*this = material;

	// ���Ӱ� ��������� ���׸����̹Ƿ� �������� 0
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
	// ������ ���� �ؽ�ó�� �����Ѵ�.
	m_vecTextureInfo[index].texture = texture;
	m_vecTextureInfo[index].shaderType = shaderType;
}

void CMaterial::AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = nullptr;

	// ���� ���� �ҼӵǾ� ���� �ʴٸ� ���ҽ� �Ŵ����� ���� �����ؼ� �����´�.
	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return;

		texture = CResourceManager::GetInst()->FindTexture(name);
	}
	// ���� ���� �ҼӵǾ��ִٸ�
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
	// ���ҽ��Ŵ����� �߰踦 �޾� ���̴��� �����´�.
	m_Shader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(name);
}

// ������ ���̴��� �����Ѵٸ� �̿��ؼ� �׷�����.
void CMaterial::Render()
{
	if (m_Shader)
		m_Shader->SetShader();

	// ���� ������۰� �����Ѵٸ� ������۸� ���ؼ� ������ ������Ʈ�Ѵ�.
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

	// ��� �ؽ�ó�� ��ȸ�ϸ鼭 ���� ���̴��� ������ �׸���.
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
	// ���̴��� �̸����� �����Ѵ�.
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

	// ���� ������Ʈ�� �̸����� �����Ѵ�.
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

	// ���� ���׸����� �������ִ� �ؽ��� �������� ��� �����Ѵ�.
	int textureCount = (int)m_vecTextureInfo.size();

	fwrite(&textureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < textureCount; i++)
	{
		length = (int)m_vecTextureInfo[i].name.length();

		// �ؽ����� �̸��� �����Ѵ�.
		fwrite(&length, sizeof(int), 1, pFile);
		fwrite(m_vecTextureInfo[i].name.c_str(), sizeof(char), length, pFile);

		// �ؽ�ó�� �������͹�ȣ�� Ÿ�Ե��� �����Ѵ�.
		fwrite(&m_vecTextureInfo[i].samplerType, sizeof(Sampler_Type), 1, pFile);
		fwrite(&m_vecTextureInfo[i].registerNum, sizeof(int), 1, pFile);
		fwrite(&m_vecTextureInfo[i].shaderType, sizeof(int), 1, pFile);

		m_vecTextureInfo[i].texture->Save(pFile);
	}
}

void CMaterial::Load(FILE* pFile)
{
	CreateConstantBuffer();

	// ���̴� �̸��� �޾ƿ´�.
	char shaderName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(shaderName, sizeof(char), length, pFile);

	// ���̴��� �̸����� ã�ƿͼ� �ʱ�ȭ�Ѵ�.
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

	// ����������Ʈ ������ �ҷ��´�.
	for (int i = 0; i < (int)RenderState_Type::MAX; i++)
	{
		bool stateEnable = false;

		// ���� ����������Ʈ�� Ȱ��ȭ�Ǿ� ������
		fread(&stateEnable, sizeof(bool), 1, pFile);

		if (stateEnable)
		{
			// ������Ʈ�� �̸��� �����´�.
			char stateName[256] = {};
			length = 0;

			fread(&length, sizeof(int), 1, pFile);
			fread(stateName, sizeof(char), length, pFile);

			// �̸����� ���� ������Ʈ�� ã�ƿ��� �ʱ�ȭ��Ų��.
			m_RenderStateArray[i] = CRenderManager::GetInst()->FindRenderState(stateName);
		}
	}

	// ��� �ؽ����� ������ �����´�.
	int textureCount = 0;

	fread(&textureCount, sizeof(int), 1, pFile);

	for (int i = 0; i < textureCount; i++)
	{
		m_vecTextureInfo.push_back(sMaterialTextureInfo());

		length = 0;

		char textureName[256] = {};

		// �ؽ����� �̸��� �����Ѵ�.
		fread(&length, sizeof(int), 1, pFile);
		fread(textureName, sizeof(char), length, pFile);

		m_vecTextureInfo[i].name = textureName;

		// ����Ǿ��ִ� �ؽ����� ���� ������ �����´�.
		fread(&m_vecTextureInfo[i].samplerType, sizeof(Sampler_Type), 1, pFile);
		fread(&m_vecTextureInfo[i].registerNum, sizeof(int), 1, pFile);
		fread(&m_vecTextureInfo[i].shaderType, sizeof(int), 1, pFile);

		// �ؽ�ó�� �̸��� �����´�.
		int texNameLength = 0;
		char texName[256] = {};

		fread(&texNameLength, sizeof(int), 1, pFile);
		fread(texName, sizeof(char), texNameLength, pFile);

		// �̹����� Ÿ���� �����´�.
		Image_Type imageType;
		fread(&imageType, sizeof(Image_Type), 1, pFile);

		// ���� �ؽ�ó ������ ����ִ� ��
		int infoCount = 0;

		fread(&infoCount, sizeof(int), 1, pFile);

		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string pathName;

		// ����ŭ �ݺ��ϸ鼭 ��� �ؽ�ó�� �������, �̸����� �޾ƿ´�.
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
			// ������ �̸��� �ϳ��� �����Ұ��
			// �ؽ�ó�� �ϳ��� �ִٴ� ��
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

		// ������ �ε��� �ؽ��ķ� �����Ѵ�.
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
