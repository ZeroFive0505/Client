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

	// 이 충돌체가 소멸될때 이 충돌체와 충돌했었던 충돌체들을 순회하면서 이 충돌체를 제거해주면서
	// 콜백함수를 호출한다.
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
	// 충돌체 매니저를 통해서 프로파일을 가져온다.
	m_Profile = CCollisionManager::GetInst()->FindProfile(name);
}

void CColliderComponent::CheckPrevColliderSection()
{
	auto iter = m_PrevCollisionList.begin();
	auto iterEnd = m_PrevCollisionList.end();

	// 이전 프레임에 충돌되었던 충돌체들을 반복하며 영역이 겹치는지 판단한다.
	// 만약 이전 프레임에 충돌이 되었는데 현재 서로 다른 영역에 속한다면 해당 충돌체와는 충돌 가능성이
	// 없으므로 충돌되었다가 떨어지는 것으로 판단한다.

	for (; iter != iterEnd;)
	{
		bool check = false;

		// 충돌 영역을 체크한다.
		size_t size = m_vecSectionIndex.size();

		for (size_t i = 0; i < size; i++)
		{
			size_t destSize = (*iter)->m_vecSectionIndex.size();

			for (size_t j = 0; j < destSize; j++)
			{
				// 다시 충돌영역이 겹쳤다.
				if (m_vecSectionIndex[i] == (*iter)->m_vecSectionIndex[j])
				{
					check = true;
					break;
				}
			}

			if (check)
				break;
		}

		// 서로 겹치는 충돌영역이 없으므로 붙어있다가 떨어진 경우
		if (!check)
		{
			CallCollisionCallback(Collision_State::End);
			(*iter)->CallCollisionCallback(Collision_State::End);

			// 서로 이전 충돌체목록에서 제거한다.
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
	// 만약 현재 충돌체 목록에 없다면 추가
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

	// 현재 씬에 이 충돌체를 등록시킨다.
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
