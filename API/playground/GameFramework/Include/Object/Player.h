#pragma once
#include "Character.h"
#include "Bullet.h"
/// <summary>
/// �÷��̾� Ŭ������ ���������� �⺻ ĳ���͸� ��ӹ޴´�.
/// �÷��̾��� �߰� ����� ���⿡ ������ �Ǿ��ִ�.
/// </summary>
class CPlayer :
    public CCharacter
{
	friend class CScene;

protected:
	CPlayer();
	CPlayer(const CPlayer& obj);
	virtual ~CPlayer();

protected:
	bool m_Skill1Enable;
	float m_Skill1Time;
	std::list<CSharedPtr<CBullet>> m_Skill1BulletList;
	CSharedPtr<CWidgetComponent> m_HPBarWidget;
	CSharedPtr<CWidgetComponent> m_NameWidget;
	CSharedPtr<CWidgetComponent> m_MPBarWidget;

public:
	virtual void SetAttackSpeed(float Speed)
	{
		CCharacter::SetAttackSpeed(Speed);

		SetAnimationPlayScale("PlayerRightAttack", Speed);
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CPlayer* Clone();
	virtual float SetDamage(float Damage);
	virtual float SetMP(float MP);

	void MPRegen();

private:
	// InputŬ������ ����� �ݹ��Լ����̴�.
	void MoveUp(float DeltaTime);
	void MoveDown(float DeltaTime);
	void MoveLeft(float DeltaTime);
	void MoveRight(float DeltaTime);
	void BulletFire(float DeltaTime);
	void Pause(float DeltaTime);
	void Resume(float DeltaTime);
	void Skill1(float DeltaTime);
	void JumpKey(float DeltaTime);

private:
	// �ִϸ��̼ǰ� ����Ǵ� �Լ���
	void AttackEnd();
	void Fire();

	void Skill1End();
	void Skill1Enable();

	float fElapsedTime;
};

