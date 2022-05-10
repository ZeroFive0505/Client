#include "MaterialManager.h"
#include "../ResourceManager.h"
#include "../Shader/MaterialConstantBuffer.h"

CMaterialManager::CMaterialManager() :
	m_CBuffer(nullptr)
{
}

CMaterialManager::~CMaterialManager()
{
	SAFE_DELETE(m_CBuffer);
}

bool CMaterialManager::Init()
{
	m_CBuffer = new CMaterialConstantBuffer;

	m_CBuffer->Init();

	CreateMaterial<CMaterial>("Color");

	CSharedPtr<CMaterial> mat = FindMaterial("Color");

	// 기본 쉐이더는 쉐이더매니저가 초기화되면서 생성되는 쉐이더 이용.
	mat->SetShader("ColorMeshShader");

	CreateMaterial<CMaterial>("BaseTexture");

	mat = FindMaterial("BaseTexture");

	mat->SetShader("Mesh2DShader");

	CTexture* texture = CResourceManager::GetInst()->FindTexture("DefaultTexture");

	mat->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DefaultTexture", texture);

	CreateMaterial<CMaterial>("SkyMaterial");

	mat = FindMaterial("SkyMaterial");

	mat->SetShader("SkyShader");

	// 20번에 연결
	mat->AddTexture(20, (int)Buffer_Shader_Type::Pixel, "DefaultSly", TEXT("Sky/Sky.dds"));

	return true;
}

CMaterial* CMaterialManager::FindMaterial(const std::string& name)
{
	auto iter = m_mapMaterial.find(name);

	if (iter == m_mapMaterial.end())
		return nullptr;

	return iter->second;
}

void CMaterialManager::ReleaseMaterial(const std::string& name)
{
	auto iter = m_mapMaterial.find(name);

	if (iter != m_mapMaterial.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapMaterial.erase(iter);
	}
}
