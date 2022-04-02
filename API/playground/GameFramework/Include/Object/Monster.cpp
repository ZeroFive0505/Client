
#include "Monster.h"
#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Collision//ColliderBox.h"

CMonster::CMonster()
{
	m_Dir.x = 0.f;
	m_Dir.y = 1.f;
	m_TimeToShoot = 1.0f;
	m_ElpasedTime = 0;
	m_Count = 0;
	m_AI = EMonsterAI::Idle;
	m_DetectDistance = 500.0f;
	m_AttackDistance = 200.0f;
	m_AttackEnable = false;
	m_Skill1Enable = false;
}

CMonster::CMonster(const CMonster& obj) :
	CCharacter(obj)
{
	m_Dir = obj.m_Dir;
	m_TimeToShoot = obj.m_TimeToShoot;
	m_ElpasedTime = obj.m_ElpasedTime;
	m_Count = obj.m_Count;
}

CMonster::~CMonster()
{
}

void CMonster::Start()
{
	CCharacter::Start();
}

bool CMonster::Init()
{
	if (!CCharacter::Init())
		return false;

	SetPivot(0.5, 1.0f);

	CreateAnimation();
	AddAnimation("PlayerLeftIdle");
	AddAnimation("PlayerLeftWalk", true, 0.6f);
	AddAnimation("PlayerLeftAttack", true, 0.5f);

	AddAnimationNotify<CMonster>("PlayerLeftAttack", 2, this, &CMonster::Fire);
	SetAnimationEndNotify<CMonster>("PlayerLeftAttack", this, &CMonster::AttackEnd);

	CColliderBox* Body = AddCollider<CColliderBox>("Body");
	Body->SetExtent(82.0f, 73.0f);
	Body->SetOffset(0.0f, -36.5f);
	Body->SetCollisionProfile("Monster");

	m_CharacterInfo.HP = 100;
	m_CharacterInfo.HPMax = 100;

	m_MoveSpeed = 100.0f;

	return true;
}

// 몬스터의 업데이트
void CMonster::Update(float DeltaTime)
{
	// 기본 캐릭터의 업데이트 함수를 호출한다.
	CCharacter::Update(DeltaTime);

	CGameObject* Player = m_Scene->GetPlayer();

	float Dist = Distance(Player->GetPos(), m_Pos);

	if (Dist <= m_DetectDistance)
	{
		if (Dist <= m_AttackDistance)
		{
			m_AI = EMonsterAI::Attack;
			m_AttackEnable = true;
		}
		else
		{
			if (!m_AttackEnable)
				m_AI = EMonsterAI::Trace;
		}
	}
	else
	{
		m_AI = EMonsterAI::Idle;
	}

	if (m_CharacterInfo.HP <= 0)
		m_AI = EMonsterAI::Death;
	else if (m_CharacterInfo.HP / (float)m_CharacterInfo.HPMax < 0.5f &&
		!m_Skill1Enable)
	{
		m_Skill1Enable = true;
		m_AI = EMonsterAI::Skill1;
	}

	//m_Pos += m_Dir * 300.f * DeltaTime;

	//// 만약 높이보다 커지면 방향을 바꾼다.
	//if (m_Pos.y >= 720.f)
	//{
	//	m_Pos.y = 720.f;
	//	m_Dir.y = -1.f;
	//}

	//else if (m_Pos.y - m_Size.y <= 0.f)
	//{
	//	m_Pos.y = m_Size.y;
	//	m_Dir.y = 1.f;
	//}

	//m_ElpasedTime += DeltaTime;

	//// 시간이 되면 투사체를 발사한다.
	//if (m_ElpasedTime >= m_TimeToShoot)
	//{
	//	m_ElpasedTime = 0.0f;
	//	// m_Count++;
	//	CSharedPtr<CBullet> bullet = m_Scene->CreateObject<CBullet>("Enemy Bullet", "MonsterBullet",
	//		Vector2(m_Pos - Vector2(m_Size.x / 2.0f + 25.0f, m_Size.y / 2.0f)),
	//		Vector2(50.0f, 50.0f));

	//    /*	if(m_Count % 3 != 0)
	//		bullet->SetDir(-1.0f, 0.0f);
	//	else
	//	{
	//		CGameObject* Player = m_Scene->FindObject("Player");

	//		float Angle = GetAngleByAtan(bullet->GetPos(), Player->GetPos());

	//		bullet->SetDir(Angle);
	//	}*/

	//	CGameObject* Player = m_Scene->GetPlayer();

	//	float Angle = GetAngleByAtan(bullet->GetPos(), Player->GetPos());

	//	bullet->SetDir(Angle);
	//}

	switch (m_AI)
	{
	case EMonsterAI::Idle:
		AIIdle(DeltaTime);
		break;
	case EMonsterAI::Trace:
		AITrace(DeltaTime);
		break;
	case EMonsterAI::Attack:
		AIAttack(DeltaTime);
		break;
	case EMonsterAI::Death:
		AIDeath(DeltaTime);
		break;
	case EMonsterAI::Skill1:
		MessageBox(0, TEXT("Skill"), TEXT("Skill"), MB_OK);
		break;
	}
}

void CMonster::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);
}

void CMonster::Collision(float DeltaTime)
{
	CCharacter::Collision(DeltaTime);
}

void CMonster::Render(HDC hDC)
{
	CCharacter::Render(hDC);
}

CMonster* CMonster::Clone()
{
	return new CMonster(*this);
}

float CMonster::SetDamage(float Damage)
{
	Damage = CCharacter::SetDamage(Damage);

	return Damage;
}

void CMonster::AIIdle(float DeltaTime)
{
	ChangeAnimation("PlayerLeftIdle");
}

void CMonster::AITrace(float DeltaTime)
{
	ChangeAnimation("PlayerLeftWalk");

	CGameObject* Player = m_Scene->GetPlayer();

	Vector2 Dir = Player->GetPos() - m_Pos;

	Dir.Normalize();

	Move(Dir, m_MoveSpeed);
}

void CMonster::AIAttack(float DeltaTime)
{
	ChangeAnimation("PlayerLeftAttack");
}

void CMonster::AIDeath(float DeltaTime)
{
	ChangeAnimation("PlayerLeftIdle");
}

void CMonster::AttackEnd()
{
	m_AttackEnable = false;
}

void CMonster::Fire()
{
	CGameObject* Player = m_Scene->GetPlayer();

	CSharedPtr<CBullet> Bullet = m_Scene->CreateObject<CBullet>("Bullet", "MonsterBullet", Vector2(m_Pos - Vector2(m_Size.x / 2.0f + 25.0f,
		m_Size.y / 2.0f)), Vector2(50.0f, 50.0f));

	float Angle = GetAngleByAcos(Bullet->GetPos(), Player->GetPos());
	Bullet->SetDir(Angle);
}
