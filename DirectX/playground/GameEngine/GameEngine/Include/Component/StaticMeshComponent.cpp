#include "StaticMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();
	m_Render = true;

	m_Transform->SetTransformState(Transform_State::Ground);
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)
{
	m_Mesh = com.m_Mesh;


	m_vecMaterialSlot.clear();

	for (size_t i = 0; i < com.m_vecMaterialSlot.size(); i++)
	{
		m_vecMaterialSlot.push_back(com.m_vecMaterialSlot[i]->Clone());
	}
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::SetMesh(const std::string& name)
{
	m_Mesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh(name);

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* materialSlots = m_Mesh->GetMaterialSlots();

	auto iter = materialSlots->begin();
	auto iterEnd = materialSlots->end();

	for (; iter != iterEnd; iter++)
	{
		m_vecMaterialSlot.push_back((*iter)->Clone());

		m_vecMaterialSlot.back()->SetScene(m_Scene);
	}

	// 메쉬를 설정하는 순간 메쉬의 크기를 설정한다.
	SetMeshSize(m_Mesh->GetMax() - m_Mesh->GetMin());

	m_CulligSphere.center = (m_Mesh->GetMax() + m_Mesh->GetMin()) / 2.0f;
}

void CStaticMeshComponent::SetMesh(CStaticMesh* mesh)
{
	m_Mesh = mesh;

	m_vecMaterialSlot.clear();

	const std::vector<CSharedPtr<CMaterial>>* materialSlots = m_Mesh->GetMaterialSlots();

	auto iter = materialSlots->begin();
	auto iterEnd = materialSlots->end();

	for (; iter != iterEnd; iter++)
	{
		m_vecMaterialSlot.push_back((*iter)->Clone());

		m_vecMaterialSlot.back()->SetScene(m_Scene);
	}

	SetMeshSize(m_Mesh->GetMax() - m_Mesh->GetMin());

	m_CulligSphere.center = (m_Mesh->GetMax() + m_Mesh->GetMin()) / 2.0f;
}

void CStaticMeshComponent::SetMaterial(CMaterial* material, int index)
{
	m_vecMaterialSlot[index] = material->Clone();

	m_vecMaterialSlot[index]->SetScene(m_Scene);
}

void CStaticMeshComponent::AddMaterial(CMaterial* material)
{
	m_vecMaterialSlot.push_back(material->Clone());

	m_vecMaterialSlot.back()->SetScene(m_Scene);
}

void CStaticMeshComponent::SetBaseColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetBaseColor(color);
}

void CStaticMeshComponent::SetBaseColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::SetAmbientColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetAmbientColor(color);
}

void CStaticMeshComponent::SetAmbientColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetAmbientColor(r, g, b, a);
}

void CStaticMeshComponent::SetSpecularColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetSpecularColor(color);
}

void CStaticMeshComponent::SetSpecularColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetSpecularColor(r, g, b, a);
}

void CStaticMeshComponent::SetSpecularPower(float power, int index)
{
	m_vecMaterialSlot[index]->SetSpecularPower(power);
}

void CStaticMeshComponent::SetEmissiveColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetEmissiveColor(color);
}

void CStaticMeshComponent::SetEmissiveColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetEmissiveColor(r, g, b, a);
}

void CStaticMeshComponent::SetRenderState(CRenderState* state, int index)
{
	m_vecMaterialSlot[index]->SetRenderState(state);
}

void CStaticMeshComponent::SetRenderState(const std::string& name, int index)
{
	m_vecMaterialSlot[index]->SetRenderState(name);
}

void CStaticMeshComponent::SetTransparency(bool enable, int index)
{
	m_vecMaterialSlot[index]->SetTransparency(enable);
}

void CStaticMeshComponent::SetOpacity(float opacity, int index)
{
	m_vecMaterialSlot[index]->SetOpacity(opacity);
}

void CStaticMeshComponent::AddOpacity(float opacity, int index)
{
	m_vecMaterialSlot[index]->AddOpacity(opacity);
}

void CStaticMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, texture);
}

void CStaticMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::AddTextureFullPath(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_vecMaterialSlot[materialIndex]->AddTextureFullPath(registerNum, shaderType, name, fullPath);
}

void CStaticMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, vecFileName, pathName);
}

void CStaticMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, texture);
}

void CStaticMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::SetTextureFullPath(int materialIndex, int index, int registerNum,
	int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_vecMaterialSlot[materialIndex]->SetTextureFullPath(index, registerNum, shaderType, name, fullPath);
}

void CStaticMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, vecFileName, pathName);
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	// SetMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));
	AddMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));

	return true;
}

void CStaticMeshComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	m_CulligSphere.center = (m_Mesh->GetMax() + m_Mesh->GetMin()) / 2.0f;
}

void CStaticMeshComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CStaticMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CStaticMeshComponent::Render()
{
	CSceneComponent::Render();

	if (!m_Mesh)
		return;

	size_t size = m_vecMaterialSlot.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecMaterialSlot[i]->Render();

		m_Mesh->Render((int)i);

		m_vecMaterialSlot[i]->Reset();
	}
}

void CStaticMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CStaticMeshComponent* CStaticMeshComponent::Clone()
{
	return new CStaticMeshComponent(*this);
}

void CStaticMeshComponent::Save(FILE* pFile)
{
	// 현재 메쉬의 이름을 저장한다.
	std::string meshName = m_Mesh->GetName();

	int length = (int)meshName.length();

	// 기록한다.
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(meshName.c_str(), sizeof(char), length, pFile);

	int materialSlotCount = (int)m_vecMaterialSlot.size();

	fwrite(&materialSlotCount, sizeof(int), 1, pFile);

	for (int i = 0; i < materialSlotCount; i++)
	{
		m_vecMaterialSlot[i]->Save(pFile);
	}

	CSceneComponent::Save(pFile);
}

void CStaticMeshComponent::Load(FILE* pFile)
{
	char meshName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(meshName, sizeof(char), length, pFile);

	SetMesh(meshName);

	m_Name = meshName;

	int materialSlotCount = 0;

	fread(&materialSlotCount, sizeof(int), 1, pFile);

	m_vecMaterialSlot.clear();

	m_vecMaterialSlot.resize(materialSlotCount);

	for (int i = 0; i < materialSlotCount; i++)
	{
		m_vecMaterialSlot[i] = new CMaterial;

		m_vecMaterialSlot[i]->Load(pFile);
	}

	CSceneComponent::Load(pFile);
}
