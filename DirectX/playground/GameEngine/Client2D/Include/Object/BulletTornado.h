#pragma once

#include "GameObject/GameObject.h"

class CBulletTornado :
	public CGameObject
{
	friend class CScene;

protected:
	CBulletTornado();
	CBulletTornado(const CBulletTornado& obj);
	virtual ~CBulletTornado();

private:
	CSharedPtr<class CSceneComponent> m_Root;
	CSharedPtr<class CSceneComponent> m_Root1;
	CSharedPtr<class CSpriteComponent> m_Sprite;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CBulletTornado* Clone();
};

