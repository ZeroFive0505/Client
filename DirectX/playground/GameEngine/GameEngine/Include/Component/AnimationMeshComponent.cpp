#include "AnimationMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Animation/AnimationSequenceInstance.h"

CAnimationMeshComponent::CAnimationMeshComponent()
{
	SetTypeID<CAnimationMeshComponent>();
	m_Render = true;
	m_Animation = nullptr;

	m_Transform->SetTransformState(Transform_State::Ground);
}

CAnimationMeshComponent::CAnimationMeshComponent(const CAnimationMeshComponent& com)
	: CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	m_vecMaterialSlot.clear();

	for (size_t i = 0; i < com.m_vecMaterialSlot.size(); i++)
	{
		m_vecMaterialSlot.push_back(com.m_vecMaterialSlot[i]->Clone());
	}

	m_Skeleton = com.m_Skeleton;

	if (com.m_Animation)
		m_Animation = com.m_Animation->Clone();
}

CAnimationMeshComponent::~CAnimationMeshComponent()
{
	SAFE_DELETE(m_Animation);
}

void CAnimationMeshComponent::SetMesh(const std::string& name)
{
	m_Mesh = (CAnimationMesh*)m_Scene->GetSceneResource()->FindMesh(name);

	// 같은 본 형태를 이용한다.
	m_Skeleton = m_Mesh->GetSkeleton();

	if (m_Animation)
		m_Animation->SetSkeleton(m_Skeleton);

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

void CAnimationMeshComponent::SetMesh(CAnimationMesh* mesh)
{
	m_Mesh = mesh;

	m_Skeleton = m_Mesh->GetSkeleton();

	if (m_Animation)
		m_Animation->SetSkeleton(m_Skeleton);

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

void CAnimationMeshComponent::SetMaterial(CMaterial* material, int index)
{
	m_vecMaterialSlot[index] = material->Clone();

	m_vecMaterialSlot[index]->SetScene(m_Scene);
}

void CAnimationMeshComponent::AddMaterial(CMaterial* material)
{
	m_vecMaterialSlot.push_back(material->Clone());

	m_vecMaterialSlot.back()->SetScene(m_Scene);
}

void CAnimationMeshComponent::SetBaseColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetBaseColor(color);
}

void CAnimationMeshComponent::SetBaseColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetBaseColor(r, g, b, a);
}

void CAnimationMeshComponent::SetAmbientColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetAmbientColor(color);
}

void CAnimationMeshComponent::SetAmbientColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetAmbientColor(r, g, b, a);
}

void CAnimationMeshComponent::SetSpecularColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetSpecularColor(color);
}

void CAnimationMeshComponent::SetSpecularColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetSpecularColor(r, g, b, a);
}

void CAnimationMeshComponent::SetSpecularPower(float power, int index)
{
	m_vecMaterialSlot[index]->SetSpecularPower(power);
}

void CAnimationMeshComponent::SetEmissiveColor(const Vector4& color, int index)
{
	m_vecMaterialSlot[index]->SetEmissiveColor(color);
}

void CAnimationMeshComponent::SetEmissiveColor(float r, float g, float b, float a, int index)
{
	m_vecMaterialSlot[index]->SetEmissiveColor(r, g, b, a);
}

void CAnimationMeshComponent::SetRenderState(CRenderState* state, int index)
{
	m_vecMaterialSlot[index]->SetRenderState(state);
}

void CAnimationMeshComponent::SetRenderState(const std::string& name, int index)
{
	m_vecMaterialSlot[index]->SetRenderState(name);
}

void CAnimationMeshComponent::SetTransparency(bool enable, int index)
{
	m_vecMaterialSlot[index]->SetTransparency(enable);
}

void CAnimationMeshComponent::SetOpacity(float opacity, int index)
{
	m_vecMaterialSlot[index]->SetOpacity(opacity);
}

void CAnimationMeshComponent::AddOpacity(float opacity, int index)
{
	m_vecMaterialSlot[index]->AddOpacity(opacity);
}

void CAnimationMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, texture);
}

void CAnimationMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, fileName, pathName);
}

void CAnimationMeshComponent::AddTextureFullPath(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_vecMaterialSlot[materialIndex]->AddTextureFullPath(registerNum, shaderType, name, fullPath);
}

void CAnimationMeshComponent::AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->AddTexture(registerNum, shaderType, name, vecFileName, pathName);
}

void CAnimationMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, texture);
}

void CAnimationMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, fileName, pathName);
}

void CAnimationMeshComponent::SetTextureFullPath(int materialIndex, int index, int registerNum,
	int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_vecMaterialSlot[materialIndex]->SetTextureFullPath(index, registerNum, shaderType, name, fullPath);
}

void CAnimationMeshComponent::SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_vecMaterialSlot[materialIndex]->SetTexture(index, registerNum, shaderType, name, vecFileName, pathName);
}

void CAnimationMeshComponent::AddChild(CSceneComponent* child, const std::string& socketName)
{
	CSceneComponent::AddChild(child, socketName);

	if (m_Skeleton && (int)socketName.length() != 0)
	{
		m_Socket = m_Skeleton->GetSocket(socketName);

		child->GetTransform()->SetSocket(m_Socket);
	}
}

void CAnimationMeshComponent::AddChild(CGameObject* child, const std::string& socketName)
{
	CSceneComponent::AddChild(child, socketName);

	if (m_Skeleton && (int)socketName.length() != 0)
	{
		m_Socket = m_Skeleton->GetSocket(socketName);

		CSceneComponent* childComponent = child->GetRootComponent();

		childComponent->GetTransform()->SetSocket(m_Socket);
	}
}

void CAnimationMeshComponent::Start()
{
	CSceneComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CAnimationMeshComponent::Init()
{
	// SetMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));
	AddMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));

	return true;
}

void CAnimationMeshComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (m_Animation)
		m_Animation->Update(deltaTime);
}

void CAnimationMeshComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CAnimationMeshComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CAnimationMeshComponent::Render()
{
	CSceneComponent::Render();

	if (!m_Mesh)
		return;

	if (m_Animation)
		m_Animation->SetShader();

	size_t size = m_vecMaterialSlot.size();

	for (size_t i = 0; i < size; i++)
	{
		m_vecMaterialSlot[i]->Render();

		m_Mesh->Render((int)i);

		m_vecMaterialSlot[i]->Reset();
	}


	if (m_Animation)
		m_Animation->ResetShader();
}

void CAnimationMeshComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CAnimationMeshComponent* CAnimationMeshComponent::Clone()
{
	return new CAnimationMeshComponent(*this);
}

void CAnimationMeshComponent::Save(FILE* pFile)
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

void CAnimationMeshComponent::Load(FILE* pFile)
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
