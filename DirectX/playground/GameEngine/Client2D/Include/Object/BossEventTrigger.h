#pragma once

#include "GameObject/GameObject.h"
#include "Component/ColliderBox2D.h"
#include "../Widget/RCGBossWidget.h"
#include "../Object/Misuzu.h"

class CBossEventTrigger :
	public CGameObject
{
	friend class CScene;

protected:
	CBossEventTrigger();
	CBossEventTrigger(const CBossEventTrigger& obj);
	virtual ~CBossEventTrigger();

protected:
	CSharedPtr<CColliderBox2D> m_Trigger;
	CSharedPtr<CRCGBossWidget> m_RCGBossWidget;
	CSharedPtr<CMisuzu> m_Boss;
	std::vector<class CRCGPilar*> m_vecPilar;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBossEventTrigger* Clone();

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

	inline void AddPilar(class CRCGPilar* pilar)
	{
		m_vecPilar.push_back(pilar);
	}

	inline void SetHealthBar(float percent)
	{
		m_RCGBossWidget->SetBossHPMeter(percent);
	}

	void DeletePilar(class CRCGPilar* pilar);

public:
	inline const std::vector<class CRCGPilar*>& GetPilars() const
	{
		return m_vecPilar;
	}

public:
	void OnPlayerTrigger(const sCollisionResult& result);
};

