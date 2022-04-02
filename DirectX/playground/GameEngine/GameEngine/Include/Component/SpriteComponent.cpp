#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"
#include "../Device.h"

CSpriteComponent::CSpriteComponent()
{
	SetTypeID<CSpriteComponent>();
	m_Render = true;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_Animation)
		m_Animation = com.m_Animation->Clone();

	if (com.m_Material)
		m_Material = com.m_Material->Clone();
}

CSpriteComponent::~CSpriteComponent()
{
	SAFE_DELETE(m_Animation);
}

void CSpriteComponent::ChangeAnimation(const std::string& name)
{
	m_Animation->ChangeAnimation(name);
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CSpriteComponent::Init()
{
	// �ʱ�ȭ�ϸ鼭 ���� �������� �޽��� �����´�.
	// �� �޽��� �޽��Ŵ����� �ʱ�ȭ�Ǹ鼭 �ڵ����� �����Ǵ� �⺻ �޽�
	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh("SpriteMesh");
	SetMaterial(m_Scene->GetSceneResource()->FindMaterial("BaseTexture"));

	// ������ ���׸��� �Ŵ����� �ʱ�ȭ�Ǹ鼭 �⺻���� �����Ǵ� ���׸����� �����´�.
	// m_Material = m_Scene->GetSceneResource()->FindMaterial("Color");
	SetMeshSize(1.0f, 1.0f, 0.0f);

	SetWorldScale((float)m_Material->GetTextureWidth(), (float)m_Material->GetTextureHeight(), 1.0f);

	return true;
}

void CSpriteComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	if (m_Animation)
		m_Animation->Update(deltaTime);
}

void CSpriteComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CSpriteComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CSpriteComponent::Render()
{
	CSceneComponent::Render();

	// ������ ���� �ִϸ��̼��� �ִٸ�
	if (m_Animation)
	{
		// ������۸� ���� �ִϸ��̼��� �̿��ϰڴٰ� ������Ʈ�Ѵ�.
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(m_Animation->GetAnimationCount() > 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();

		m_Animation->SetShader();
	}

	m_Material->Render();

	m_Mesh->Render();

	m_Material->Reset();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* pFile)
{
	// ���� ��������Ʈ�� ����Ǿ��ִ� �޽��� �̸��� �����´�.
	std::string meshName = m_Mesh->GetName();

	int length = (int)meshName.length();

	// �޽��� �̸��� �����Ѵ�.
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(meshName.c_str(), sizeof(char), length, pFile);

	// ���׸��� ����
	m_Material->Save(pFile);

	// �ִϸ��̼� ���θ� ����
	bool hasAnim = false;

	if (m_Animation)
		hasAnim = true;

	fwrite(&hasAnim, sizeof(bool), 1, pFile);

	// ���� �ִϸ��̼��� ������ ���
	if (m_Animation)
	{
		size_t typeID = m_Animation->GetTypeID();
		fwrite(&typeID, sizeof(size_t), 1, pFile);

		m_Animation->Save(pFile);
	}

	CSceneComponent::Save(pFile);
}

void CSpriteComponent::Load(FILE* pFile)
{
	char meshName[256] = {};

	int length = 0;

	fread(&length, sizeof(int), 1, pFile);
	fread(meshName, sizeof(char), length, pFile);

	// �ҷ��� �޽� �̸����� �޽��� ã�ƿ´�.
	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh(meshName);

	// ���� �� ���׸����� �����´�.
	m_Material = m_Scene->GetSceneResource()->CreateMaterialEmpty<CMaterial>();

	// �� ���� �ε��ؼ� ������ ä�� �ִ´�.
	m_Material->Load(pFile);

	bool hasAnim = false;

	fread(&hasAnim, sizeof(bool), 1, pFile);

	// �ִϸ��̼��� ������ ��� �ִϸ޼��� ������ �ҷ��´�.
	if (hasAnim)
	{
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, pFile);

		CSceneManager::GetInst()->CallCreateAnimInstance(this, typeID);

		m_Animation->Load(pFile);
	}

	CSceneComponent::Load(pFile);
}
