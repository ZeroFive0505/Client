#pragma once

#include "GameObject/GameObject.h"

class CSpriteEditObject :
	public CGameObject
{
	friend class CScene;

protected:
	CSpriteEditObject();
	CSpriteEditObject(const CSpriteEditObject& obj);
	virtual ~CSpriteEditObject();

private:
	CSharedPtr<class CSpriteComponent> m_Sprite;
	

public:
	inline class CSpriteComponent* GetSpriteComponent() const
	{
		return m_Sprite;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CSpriteEditObject* Clone();
};

