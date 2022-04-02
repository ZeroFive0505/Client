#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"
#include "RCGEnemy.h"

class CLockEventTrigger :
	public CGameObject
{
	friend class CScene;

protected:
	CLockEventTrigger();
	CLockEventTrigger(const CLockEventTrigger& obj);
	virtual ~CLockEventTrigger();

protected:
	CSharedPtr<CColliderBox2D> m_Trigger;
	bool m_EventOn;
	bool m_EventEnd;


	float m_TimeToSpawn;
	float m_ElapsedTime;
	std::vector<class CChainLockWall*> m_vecChainWall;
	std::list<CRCGEnemy*> m_Enemies;
	int m_SpawnCnt;
	int m_EnemiesToSpawn;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CLockEventTrigger* Clone();

public:
	inline void SetTriggerExtent(const Vector2& size)
	{
		if (m_Trigger)
			m_Trigger->SetExtent(size.x, size.y);
	}

	inline void SetTriggerExtent(float x, float y)
	{
		if (m_Trigger)
			m_Trigger->SetExtent(x, y);
	}

public:
	inline const std::vector<class CChainLockWall*>& GetChainWalls() const
	{
		return m_vecChainWall;
	}

public:
	inline bool IsDuringEvent() const
	{
		return m_EventOn;
	}

	inline void DeleteEnemy(const CRCGEnemy* enemy)
	{
		auto iter = std::find(m_Enemies.begin(), m_Enemies.end(), enemy);

		if (iter != m_Enemies.end())
			m_Enemies.erase(iter);
	}

public:
	void OnPlayerTrigger(const sCollisionResult& result);
};

