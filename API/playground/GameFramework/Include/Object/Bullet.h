#pragma once
#include "GameObject.h"

/// <summary>
/// 총알 클래스는 게임 오브젝트를 기본으로 하여 추가적인 속성을 가지고 있는다.
/// </summary>
class CBullet :
    public CGameObject
{
	friend class CScene;

protected:
	CBullet();
	CBullet(const CBullet& obj);
	virtual ~CBullet();

protected:
	Vector2		m_Dir;
	float		m_Distance;
	float m_Damage;

public:
	void SetBulletDamage(float Damage)
	{
		m_Damage = Damage;
	}

	void SetDir(float x, float y)
	{
		m_Dir.x = x;
		m_Dir.y = y;
	}

	void SetDir(float Angle)
	{
		m_Dir.x = cosf(DegreeToRadian(Angle));
		m_Dir.y = sinf(DegreeToRadian(Angle));
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CBullet* Clone();

private:
	void CollisionBegin(CCollider* Src, CCollider* Dest, float DeltaTime);
};

