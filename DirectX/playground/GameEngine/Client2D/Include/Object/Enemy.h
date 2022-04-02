#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"

class CEnemy :
	public CGameObject
{
	friend class CScene;

protected:
	CEnemy();
	CEnemy(const CEnemy& obj);
	virtual ~CEnemy();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderCircle> m_Body;
	CSharedPtr<CPaperBurnComponent> m_PaperBurn;

	CSharedPtr<CWidgetComponent> m_SimpleHUDWidget;

	class CSimpleHUD* m_SimpleHUD;

	int m_MaxHitPoint;
	int m_HitPoint;

public:
	void DealDamage(int damage);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CEnemy* Clone();

private:
	void OnMouseEnter(const sCollisionResult& result);
	void OnMouseExit(const sCollisionResult& result);

private:
	void PaperBurnEnd();
};

