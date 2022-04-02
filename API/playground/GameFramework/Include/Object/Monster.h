#pragma once
#include "Character.h"
/// <summary>
/// ���� Ŭ������ ĳ���͸� ��ӹ޴� Ŭ������
/// ĳ���Ϳ� �⺻������ ���� ������. �߰������� ����� �ӵ����� ���ڸ� �������ִ�.
/// </summary>
class CMonster :
    public CCharacter
{
	friend class CScene;

protected:
	CMonster();
	CMonster(const CMonster& obj);
	virtual ~CMonster();

private:
	Vector2	m_Dir;
	float m_ElpasedTime;
	float m_TimeToShoot;
	int m_Count;
	float m_DetectDistance;
	float m_AttackDistance;
	bool m_AttackEnable;
	bool m_Skill1Enable;
	EMonsterAI m_AI;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CMonster* Clone();
	virtual float SetDamage(float Damage);


protected:
	void AIIdle(float DeltaTime);
	void AITrace(float DeltaTime);
	void AIAttack(float DeltaTime);
	void AIDeath(float DeltaTime);

	void AttackEnd();
	void Fire();
};

