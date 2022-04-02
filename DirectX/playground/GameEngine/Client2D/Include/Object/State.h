#pragma once

#include "RCGEnemy.h"
#include "RCGPlayer.h"
#include "Scene/Scene.h"
#include "Scene/SceneMode.h"
#include "Scene/SceneManager.h"

enum class STATE
{
	IDLE,
	PURSUE,
	FALLBACK,
	BLOCK,
	ATTACK
};

enum class EVENT
{
	ENTER,
	UPDATE,
	EXIT
};

class CState
{
	friend class CRCGEnemy;

public:
	CState(CRCGEnemy* npc, CState* nextState);
	virtual ~CState();

protected:
	STATE m_State;
	EVENT m_Stage;
	CState* m_NextState;
	CRCGEnemy* m_NPC;

public:
	inline STATE GetCurrentState() const
	{
		return m_State;
	}

public:
	virtual void Enter()
	{
		m_Stage = EVENT::UPDATE;
	}

	virtual void Update()
	{
		m_Stage = EVENT::UPDATE;
	}

	virtual void Exit()
	{
		m_Stage = EVENT::EXIT;
	}

	CState* Process()
	{
		if (m_Stage == EVENT::ENTER)
			Enter();

		if (m_Stage == EVENT::UPDATE)
			Update();

		if (m_Stage == EVENT::EXIT)
		{
			Exit();
			return m_NextState;
		}

		return this;
	}
};

