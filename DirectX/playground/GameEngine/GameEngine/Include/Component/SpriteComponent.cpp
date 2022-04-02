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
	// 초기화하면서 가장 기초적인 메쉬를 가져온다.
	// 이 메쉬는 메쉬매니저가 초기화되면서 자동으로 생성되는 기본 메쉬
	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh("SpriteMesh");
	SetMaterial(m_Scene->GetSceneResource()->FindMaterial("BaseTexture"));

	// 재질은 마테리얼 매니저가 초기화되면서 기본으로 생성되는 마테리얼을 가져온다.
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

	// 렌더시 만약 애니메이션이 있다면
	if (m_Animation)
	{
		// 상수버퍼를 통해 애니메이션을 이용하겠다고 업데이트한다.
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
	// 현재 스프라이트가 적용되어있는 메쉬의 이름을 가져온다.
	std::string meshName = m_Mesh->GetName();

	int length = (int)meshName.length();

	// 메쉬의 이름을 저장한다.
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(meshName.c_str(), sizeof(char), length, pFile);

	// 마테리얼 저장
	m_Material->Save(pFile);

	// 애니메이션 여부를 저장
	bool hasAnim = false;

	if (m_Animation)
		hasAnim = true;

	fwrite(&hasAnim, sizeof(bool), 1, pFile);

	// 만약 애니메이션이 존재할 경우
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

	// 불러온 메쉬 이름으로 메쉬를 찾아온다.
	m_Mesh = (CSpriteMesh*)m_Scene->GetSceneResource()->FindMesh(meshName);

	// 먼저 빈 마테리얼을 만들어온다.
	m_Material = m_Scene->GetSceneResource()->CreateMaterialEmpty<CMaterial>();

	// 그 이후 로드해서 정보를 채워 넣는다.
	m_Material->Load(pFile);

	bool hasAnim = false;

	fread(&hasAnim, sizeof(bool), 1, pFile);

	// 애니메이션이 존재할 경우 애니메션의 정보를 불러온다.
	if (hasAnim)
	{
		size_t typeID = 0;
		fread(&typeID, sizeof(size_t), 1, pFile);

		CSceneManager::GetInst()->CallCreateAnimInstance(this, typeID);

		m_Animation->Load(pFile);
	}

	CSceneComponent::Load(pFile);
}
