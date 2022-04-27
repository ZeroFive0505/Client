#include "ColliderComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneCollision.h"
#include "../Collision/CollisionManager.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

CColliderComponent::CColliderComponent()
{
	SetTypeID<CColliderComponent>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_CurrentSectionCheck = false;
	m_Profile = nullptr;
	m_Mesh = nullptr;
	m_CBuffer = nullptr;
	m_MouseCollision = false;
	m_Shader = nullptr;
}

CColliderComponent::CColliderComponent(const CColliderComponent& com) :
	CSceneComponent(com)
{
	m_MouseCollision = false;
	m_CurrentSectionCheck = false;
	m_Render = com.m_Render;
	m_Profile = com.m_Profile;
	m_CBuffer = com.m_CBuffer->Clone();
	m_Mesh = com.m_Mesh;
	m_Shader = com.m_Shader;
}

CColliderComponent::~CColliderComponent()
{
	SAFE_DELETE(m_CBuffer);

	// �� �浹ü�� �Ҹ�ɶ� �� �浹ü�� �浹�߾��� �浹ü���� ��ȸ�ϸ鼭 �� �浹ü�� �������ָ鼭
	// �ݹ��Լ��� ȣ���Ѵ�.
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->DeletePrevCollision(this);
		(*iter)->CallCollisionCallback(Collision_State::End);
		CallCollisionCallback(Collision_State::End);
	}
}

void CColliderComponent::SetCollisionProfile(const std::string& name)
{
	// �浹ü �Ŵ����� ���ؼ� ���������� �����´�.
	m_Profile = CCollisionManager::GetInst()->FindProfile(name);
}

void CColliderComponent::CheckPrevColliderSection()
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	// ���� �����ӿ� �浹�Ǿ��� �浹ü���� �ݺ��ϸ� ������ ��ġ���� �Ǵ��Ѵ�.
	// ���� ���� �����ӿ� �浹�� �Ǿ��µ� ���� ���� �ٸ� ������ ���Ѵٸ� �ش� �浹ü�ʹ� �浹 ���ɼ���
	// �����Ƿ� �浹�Ǿ��ٰ� �������� ������ �Ǵ��Ѵ�.

	for (; iter != iterEnd;)
	{
		bool check = false;

		// �浹 ������ üũ�Ѵ�.
		size_t size = m_vecSectionIndex.size();

		for (size_t i = 0; i < size; i++)
		{
			size_t destSize = (*iter)->m_vecSectionIndex.size();

			for (size_t j = 0; j < destSize; j++)
			{
				// �ٽ� �浹������ ���ƴ�.
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					check = true;
					break;
				}
			}

			if (check)
				break;
		}

		// ���� ��ġ�� �浹������ �����Ƿ� �پ��ִٰ� ������ ���
		if (!check)
		{
			CallCollisionCallback(Collision_State::End);
			(*iter)->CallCollisionCallback(Collision_State::End);

			// ���� ���� �浹ü��Ͽ��� �����Ѵ�.
			(*iter)->DeletePrevCollision(this);
			
			iter = m_PrevCollisionList.erase(iter);
			iterEnd = m_PrevCollisionList.end();
			continue;
		}

		iter++;
	}

}

void CColliderComponent::AddPrevCollision(CColliderComponent* collider)
{
	m_PrevCollisionList.push_back(collider);
}

void CColliderComponent::DeletePrevCollision(CColliderComponent* collider)
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == collider)
		{
			m_PrevCollisionList.erase(iter);
			return;
		}
	}
}

bool CColliderComponent::EmptyPrevCollision()
{
	return m_PrevCollisionList.empty();
}

bool CColliderComponent::CheckPrevCollision(CColliderComponent* collider)
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == collider)
			return true;
	}

	return false;
}

bool CColliderComponent::CheckCurrentFrameCollision(CColliderComponent* collider)
{
	auto iter = m_CurrentCollisionList.begin();
	auto iterEnd = m_CurrentCollisionList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == collider)
			return true;
	}

	return false;
}

void CColliderComponent::AddCurrentFrameCollision(CColliderComponent* collider)
{
	// ���� ���� �浹ü ��Ͽ� ���ٸ� �߰�
	if (!CheckCurrentFrameCollision(collider))
		m_CurrentCollisionList.push_back(collider);
}

void CColliderComponent::CallCollisionCallback(Collision_State state)
{
	auto iter = m_CollisionCallback[(int)state].begin();
	auto iterEnd = m_CollisionCallback[(int)state].end();

	for (; iter != iterEnd; iter++)
	{
		(*iter).first(m_Result);
	}
}

void CColliderComponent::CallCollisionMouseCallback(Collision_State state)
{
	if (state == Collision_State::End)
		m_MouseCollision = false;

	auto iter = m_CollisionMouseCallback[(int)state].begin();
	auto iterEnd = m_CollisionMouseCallback[(int)state].end();

	for (; iter != iterEnd; iter++)
	{
		if((*iter))
			(*iter)(m_MouseResult);
	}
}

void CColliderComponent::DeleteCollisionCallback(CGameObject* obj)
{
	m_CollisionCallback[(int)Collision_State::Begin].erase(std::remove_if(m_CollisionCallback[(int)Collision_State::Begin].begin(), m_CollisionCallback[(int)Collision_State::Begin].end(),
		[&](std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>& item) { return item.second == obj; }));

	m_CollisionCallback[(int)Collision_State::End].erase(std::remove_if(m_CollisionCallback[(int)Collision_State::End].begin(), m_CollisionCallback[(int)Collision_State::End].end(),
		[&](std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>& item) { return item.second == obj; }));
}

void CColliderComponent::ClearFrame()
{
	m_vecSectionIndex.clear();
	m_CurrentCollisionList.clear();
	m_CurrentSectionCheck = false;
}

void CColliderComponent::Start()
{
	CSceneComponent::Start();

	// ���� ���� �� �浹ü�� ��Ͻ�Ų��.
	m_Scene->GetCollision()->AddCollider(this);
}

bool CColliderComponent::Init()
{
	if (!CSceneComponent::Init())
		return false;

	SetCollisionProfile("Object");

	m_CBuffer = new CColliderConstantBuffer;

	m_CBuffer->Init();

	m_CBuffer->SetColliderColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));

	m_Shader = CResourceManager::GetInst()->FindShader("ColliderShader");

	return true;
}

void CColliderComponent::Update(float deltaTime)
{
	CSceneComponent::Update(deltaTime);
}

void CColliderComponent::PostUpdate(float deltaTime)
{
	CSceneComponent::PostUpdate(deltaTime);
}

void CColliderComponent::CheckCollision()
{
	CSceneComponent::CheckCollision();
}

void CColliderComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CColliderComponent::Render()
{
	CSceneComponent::Render();
}

void CColliderComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CColliderComponent* CColliderComponent::Clone()
{
	return nullptr;
}

void CColliderComponent::Save(FILE* pFile)
{
	CSceneComponent::Save(pFile);

	fwrite(&m_ColliderType, sizeof(Collider_Type), 1, pFile);
	fwrite(&m_Offset, sizeof(Vector3), 1, pFile);
	fwrite(&m_Min, sizeof(Vector3), 1, pFile);
	fwrite(&m_Max, sizeof(Vector3), 1, pFile);

	int len = (int)m_Profile->name.length();
	fwrite(&len, sizeof(int), 1, pFile);
	fwrite(m_Profile->name.c_str(), sizeof(char), len, pFile);
}

void CColliderComponent::Load(FILE* pFile)
{
	CSceneComponent::Load(pFile);

	fread(&m_ColliderType, sizeof(Collider_Type), 1, pFile);
	fread(&m_Offset, sizeof(Vector3), 1, pFile);
	fread(&m_Min, sizeof(Vector3), 1, pFile);
	fread(&m_Max, sizeof(Vector3), 1, pFile);

	int len = 0;
	fread(&len, sizeof(int), 1, pFile);
	char profileName[256] = {};
	fread(profileName, sizeof(char), len, pFile);

	SetCollisionProfile(profileName);
}
