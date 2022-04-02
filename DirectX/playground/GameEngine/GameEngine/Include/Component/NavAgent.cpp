#include "NavAgent.h"
#include "../Scene/Scene.h"
#include "../Scene/NavigationManager.h"
#include "SceneComponent.h"
#include "../GameObject/GameObject.h"

CNavAgent::CNavAgent() :
	m_MoveSpeed(300.0f),
	m_Interrupted(false)
{
	SetTypeID<CNavAgent>();
}

CNavAgent::CNavAgent(const CNavAgent& com) :
	CObjectComponent(com)
{
	m_MoveSpeed = com.m_MoveSpeed;
}

CNavAgent::~CNavAgent()
{
}

bool CNavAgent::Move(const Vector3& pos)
{
	if (!m_UpdateComponent)
		return false;

	return m_Scene->GetNavigationManager()->FindPath<CNavAgent>(this, &CNavAgent::PathResult, m_UpdateComponent->GetWorldPos(), pos);
}

void CNavAgent::Start()
{
	if (!m_UpdateComponent)
		m_UpdateComponent = m_Object->GetRootComponent();
}

bool CNavAgent::Init()
{
	return true;
}

void CNavAgent::Update(float deltaTime)
{
	if (m_UpdateComponent)
	{
		if (!m_Interrupted && !m_Path.empty())
		{
			m_IsMoving = true;
			Vector3 target = m_Path.front();
			Vector3 pos = m_UpdateComponent->GetWorldPos();
			
			// 거리를 구한다.
			float targetPos = pos.SqrDistance(target);

			Vector3 dir = target - pos;

			dir.Normalize();

			// 미래의 위치를 구한다.
			float nextPos = m_MoveSpeed * deltaTime;

			// 만약 미래의 위치가 현재 정해진 거리보다 커진다면
			// 다음 노드로 타겟을 바꾼다.
			if (targetPos <= nextPos * nextPos)
			{
				m_Path.pop_front();
				nextPos = targetPos;
			}
			
			m_UpdateComponent->AddWorldPos(dir * nextPos);
		}
		else if (m_Interrupted)
		{
			m_Path.clear();
			m_Interrupted = false;
		}
	}
}

void CNavAgent::PostUpdate(float deltaTime)
{
}

void CNavAgent::PrevRender()
{
}

void CNavAgent::Render()
{
}

void CNavAgent::PostRender()
{
}

CNavAgent* CNavAgent::Clone()
{
	return new CNavAgent(*this);
}

void CNavAgent::PathResult(const std::list<Vector3>& path)
{
	m_Path.clear();
	m_IsMoving = false;
	m_Path = path;
}
