#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CSprite2D :
	public CGameObject
{
	friend class CScene;

protected:
	CSprite2D();
	CSprite2D(const CSprite2D& obj);
	virtual ~CSprite2D();

private:
	CSharedPtr<CSpriteComponent> m_Sprite;

private:
	virtual bool Init();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSprite2D* Clone();
};

