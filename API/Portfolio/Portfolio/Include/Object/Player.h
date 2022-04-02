#pragma once

#include "Character.h"

class CPlayer :
	public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Collision(float deltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer* Clone();

private:
	void Up(float deltaTime);
	void Down(float deltaTime);
	void Left(float deltaTime);
	void Right(float deltaTime);
};

