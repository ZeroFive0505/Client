#pragma once
#include "GameObject.h"

/// <summary>
/// 캐릭터 클래스는 게임 오브젝트를 상속받으며
/// 가장 기본이 되는 캐릭터 클래스이다. 캐릭터 클래스를 상속받는 것은
/// Player, Monster 등이 있다.
/// </summary>
class CCharacter :
    public CGameObject
{
	// 씬은 해당 캐릭터를 자유롭게 생성이 가능해야한다. 따라서 프렌드
	friend class CScene;

protected:
	CCharacter();
	CCharacter(const CCharacter& obj);
	virtual ~CCharacter();

protected:
	CharacterInfo m_CharacterInfo;

public:
	void SetCharacterInfo(int Attack, int Armor, int HP, int MP, int Level,
		int Exp, int Gold, float AttackSpeed,
		float AttackDistance)
	{
		m_CharacterInfo.Attack = Attack;
		m_CharacterInfo.Armor = Armor;
		m_CharacterInfo.HP = HP;
		m_CharacterInfo.HPMax = HP;
		m_CharacterInfo.MP = MP;
		m_CharacterInfo.MPMax = MP;
		m_CharacterInfo.Level = Level;
		m_CharacterInfo.Exp = Exp;
		m_CharacterInfo.Gold = Gold;
		m_CharacterInfo.AttackSpeed = AttackSpeed;
		m_CharacterInfo.AttackDistance = AttackDistance;
	}

	virtual void SetAttackSpeed(float Speed)
	{
		m_CharacterInfo.AttackSpeed = Speed;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Collision(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCharacter* Clone();
	virtual float SetDamage(float Damage);
	virtual float SetMP(float MP);
};

