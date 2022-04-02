#pragma once

#include "GameObject.h"

class CCharacter :
	public CGameObject
{
	friend class CScene;

protected:
	CCharacter();
	CCharacter(const CCharacter& obj);
	virtual ~CCharacter();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(HDC hDC);
	virtual CCharacter* Clone();
};
