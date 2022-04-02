#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/StructuredBuffer.h"

CParticleComponent::CParticleComponent() :
	m_SpawnTime(0.0f),
	m_SpawnTimeMax(0.01f),
	m_Info{},
	m_InfoShared{},
	m_CBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;

	m_LayerName = "Particle";
}

CParticleComponent::CParticleComponent(const CParticleComponent& com) :
	CSceneComponent(com)
{
	m_SpawnTime = 0.0f;
	m_SpawnTimeMax = com.m_SpawnTimeMax;

	m_Particle = com.m_Particle;

	size_t bufferCount = m_vecStructedBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		SAFE_DELETE(m_vecStructedBuffer[i]);
	}

	m_vecStructedBuffer.clear();

	if (m_Particle)
	{
		m_Particle->CloneStructedBuffer(m_vecStructedBuffer);

		m_UpdateShader = m_Particle->GetUpdateShader();

		SAFE_DELETE(m_CBuffer);

		m_CBuffer = m_Particle->CloneConstantBuffer();
	}
}

CParticleComponent::~CParticleComponent()
{
	size_t bufferCount = m_vecStructedBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		SAFE_DELETE(m_vecStructedBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

void CParticleComponent::SetParticle(const std::string& name)
{
	if (m_Scene)
		SetParticle(m_Scene->GetSceneResource()->FindParticle(name));
	else
		SetParticle(CResourceManager::GetInst()->FindParticle(name));
}

void CParticleComponent::SetParticle(CParticle* particle)
{
	m_Particle = particle;

	m_Material = m_Particle->CloneMaterial();

	size_t bufferCount = m_vecStructedBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		SAFE_DELETE(m_vecStructedBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);

	m_Particle->CloneStructedBuffer(m_vecStructedBuffer);

	m_UpdateShader = m_Particle->GetUpdateShader();

	m_CBuffer = m_Particle->CloneConstantBuffer();

	m_SpawnTimeMax = m_Particle->GetSpawnTime();
}

void CParticleComponent::SetSpawnTime(float spawnTime)
{
	m_SpawnTimeMax = spawnTime;

	// m_Particle->SetSpawnTime(m_SpawnTimeMax);
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	if (m_Scene)
		m_Mesh = m_Scene->GetSceneResource()->FindMesh("ParticlePointMesh");
	else
		m_Mesh = CResourceManager::GetInst()->FindMesh("ParticlePointMesh");

	return true;
}

void CParticleComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);

	m_SpawnTime += deltaTime;

	// ��ƼŬ�� ������ �ð��� ���.
	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;
		m_CBuffer->SetSpawnEnable(1);
	}
	else
		m_CBuffer->SetSpawnEnable(0);
}

void CParticleComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);

	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	// Update Shader�� ����
	Vector3 startMin, startMax;

	startMin = GetWorldPos() + CBuffer->GetStartMin();
	startMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(startMin);
	m_CBuffer->SetStartMax(startMax);

	m_CBuffer->UpdateCBuffer();

	size_t bufferCount = m_vecStructedBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		m_vecStructedBuffer[i]->SetShader();
	}

	// UpdateShader�� Thread�� 64, 1, 1�� ����ϰ� �ִ�.
	// �����Ǿ��� ��ƼŬ�� ��ü ������ 64���� ������. ���� 64���� �ִ� ��ƼŬ ���� �������־��ٸ�
	// �ʿ��� �׷��� ���� 1���̴�. ������ 64�� �̸��̶�� ���� ��� 0�� �����Ƿ� ���⿡ 1�� ������� �Ѵ�.
	// 100���� ��� �׷��� ���� 2���� �Ǹ�, �̶� ������� 128���� �ǹǷ� 100���� ������ ������ 28���� ó���� �ȵǰ�
	// �����ָ� �ȴ�.
	int groupCount = m_Particle->GetSpawnCountMax() / 64 + 1;
	m_UpdateShader->Execute(groupCount, 1, 1);

	// ��ǻƮ���̴��� ������� m_vecStructedBuffer�� ��������� ���̴�.
	for (size_t i = 0; i < bufferCount; i++)
	{
		m_vecStructedBuffer[i]->ResetShader();
	}
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();

	size_t bufferCount = m_vecStructedBuffer.size();

	for (size_t i = 0; i < bufferCount; i++)
	{
		// �˸��� �������� ��ȣ�� ���̴��� �����Ѵ�.
		m_vecStructedBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Render();

	// �ν��Ͻ��� �̿��ؼ� �׸���.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());

	for (size_t i = 0; i < bufferCount; i++)
	{
		m_vecStructedBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

void CParticleComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);
}

void CParticleComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);
}
