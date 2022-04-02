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

	// 파티클이 생성될 시간이 됬다.
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

	// Update Shader를 동작
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

	// UpdateShader의 Thread는 64, 1, 1을 사용하고 있다.
	// 생성되야할 파티클의 전체 수에서 64개를 나눈다. 만약 64개를 최대 파티클 수로 지정해주었다면
	// 필요한 그룹의 수는 1개이다. 하지만 64개 미만이라면 나눌 경우 0이 나오므로 여기에 1을 더해줘야 한다.
	// 100개일 경우 그룹의 수는 2개가 되며, 이때 스레드는 128개가 되므로 100개를 제외한 나머지 28개는 처리가 안되게
	// 막아주면 된다.
	int groupCount = m_Particle->GetSpawnCountMax() / 64 + 1;
	m_UpdateShader->Execute(groupCount, 1, 1);

	// 컴퓨트쉐이더의 계산결과가 m_vecStructedBuffer에 저장되있을 것이다.
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
		// 알맞은 레지스터 번호에 쉐이더를 설정한다.
		m_vecStructedBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Render();

	// 인스턴싱을 이용해서 그린다.
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
