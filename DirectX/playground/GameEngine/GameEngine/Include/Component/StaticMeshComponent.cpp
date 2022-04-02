#include "StaticMeshComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"

CStaticMeshComponent::CStaticMeshComponent()
{
	SetTypeID<CStaticMeshComponent>();
	m_Render = true;
}

CStaticMeshComponent::CStaticMeshComponent(const CStaticMeshComponent& com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_Material)
		m_Material = com.m_Material->Clone();
}

CStaticMeshComponent::~CStaticMeshComponent()
{
}

void CStaticMeshComponent::SetMesh(const std::string& name)
{
	m_Mesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh(name);
}

void CStaticMeshComponent::SetMesh(CStaticMesh* mesh)
{
	m_Mesh = mesh;
}

void CStaticMeshComponent::SetMaterial(CMaterial* material)
{
	m_Material = material->Clone();

	m_Material->SetScene(m_Scene);
}

void CStaticMeshComponent::SetBaseColor(const Vector4& color)
{
	m_Material->SetBaseColor(color);
}

void CStaticMeshComponent::SetBaseColor(float r, float g, float b, float a)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CStaticMeshComponent::SetRenderState(CRenderState* state)
{
	m_Material->SetRenderState(state);
}

void CStaticMeshComponent::SetRenderState(const std::string& name)
{
	m_Material->SetRenderState(name);
}

void CStaticMeshComponent::SetTransparency(bool enable)
{
	m_Material->SetTransparency(enable);
}

void CStaticMeshComponent::SetOpacity(float opacity)
{
	m_Material->SetOpacity(opacity);
}

void CStaticMeshComponent::AddOpacity(float opacity)
{
	m_Material->AddOpacity(opacity);
}

void CStaticMeshComponent::AddTexture(int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_Material->AddTexture(registerNum, shaderType, name, texture);
}

void CStaticMeshComponent::AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_Material->AddTexture(registerNum, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::AddTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_Material->AddTextureFullPath(registerNum, shaderType, name, fullPath);
}

void CStaticMeshComponent::AddTexture(int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_Material->AddTexture(registerNum, shaderType, name, vecFileName, pathName);
}

void CStaticMeshComponent::SetTexture(int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
{
	m_Material->SetTexture(index, registerNum, shaderType, name, texture);
}

void CStaticMeshComponent::SetTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	m_Material->SetTexture(index, registerNum, shaderType, name, fileName, pathName);
}

void CStaticMeshComponent::SetTextureFullPath(int index, int registerNum,
	int shaderType, const std::string& name, const TCHAR* fullPath)
{
	m_Material->SetTextureFullPath(index, registerNum, shaderType, name, fullPath);
}

void CStaticMeshComponent::SetTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	m_Material->SetTexture(index, registerNum, shaderType, name, vecFileName, pathName);
}

void CStaticMeshComponent::Start()
{
	CSceneComponent::Start();
}

bool CStaticMeshComponent::Init()
{
	SetMaterial(m_Scene->GetSceneResource()->FindMaterial("Color"));

	return true;
}

void CStaticMeshComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
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

	if (!m_Mesh || !m_Material)
		return;

	m_Material->Render();

	m_Mesh->Render();

	m_Material->Reset();
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

	m_Material->Save(pFile);

	CSceneComponent::Save(pFile);
}

void CStaticMeshComponent::Load(FILE* pFile)
{
	char meshName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(meshName, sizeof(char), length, pFile);

	m_Mesh = (CStaticMesh*)m_Scene->GetSceneResource()->FindMesh(meshName);

	m_Material->Load(pFile);

	CSceneComponent::Load(pFile);
}
