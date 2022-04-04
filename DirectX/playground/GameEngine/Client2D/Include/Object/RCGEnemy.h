#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/SceneComponent.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "Component/WidgetComponent.h"
#include "../Client.h"

struct sEnemyMove
{
	EAttackType attackType;
	Vector2 forceDir;
	int damage;
	float force;
	float forceTime;

	sEnemyMove(EAttackType type, Vector2 dir, int _damage, float _force, float _time) :
		attackType(type), forceDir(dir), damage(_damage), force(_force), forceTime(_time) {}

	sEnemyMove() {}
};

class CRCGEnemy :
	public CGameObject
{
	friend class CScene;

protected:
	CRCGEnemy();
	CRCGEnemy(const CRCGEnemy& obj);
	virtual ~CRCGEnemy();

protected:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderBox2D> m_Body;
	CSharedPtr<CSpriteComponent> m_Shadow;
	CSharedPtr<CColliderBox2D> m_ContactBox;
	CSharedPtr<CColliderBox2D> m_Bottom;
	CSharedPtr<CColliderBox2D> m_HitBox;

	int m_FinishedState;

	class CGameObject* m_Player;

	const int m_HitAnimCnt = 3;

	int m_HitCount;

	float m_Opacity;
	float m_WalkSpeed;
	float m_Gravity;
	float m_RunSpeed;
	float m_JumpSpeed;

	bool m_FacingRight;
	bool m_GravityEnable;
	bool m_OnGround;
	bool m_Guard;

	bool m_Jump;

	bool m_Grabbable;

	bool m_Physics;

	bool m_Stunned;

	bool m_Push;

	bool m_Spawning;
	float m_SpawnTime;

	bool m_Invincible;
	bool m_HalfOpacity;

	bool m_LeftWallCollision;
	bool m_RightWallCollision;
	bool m_TopWallCollision;
	bool m_BottomWallCollision;

	Vector2 m_Velocity;
	Vector2 m_Acceleration;

	Vector2 m_AbsVel;
	Vector2 m_AbsAccel;

	Vector2 m_KnockbackForce;


	float m_CurrentTime;
	float m_BlinkTime;
	int m_CurrentState;
	float m_StateEnd[(int)EEnemyState::MAX];

	float m_JumpTime;
	float m_MaxFallSpeed;

	float m_StunAccumulate;

	int m_HealthPoint;

	CSharedPtr<CNavAgent> m_NavAgent;

	CSharedPtr<CWidgetComponent> m_HUDWidget;

	class CRCGEnemyHUD* m_EnemyHUD;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CRCGEnemy* Clone();

public:
	void ApplyForce(const Vector2& force, bool isRelative = false);
	void ApplyForce(float x, float y, bool isRelative = false);
	void ApplyAbsForce(const Vector2& force);
	void ApplyAbsForce(float x, float y);

public:
	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime,  bool right);

public:
	void HitStatusEnd();

public:
	int PopStateEnd(float currentTime);
	void PopStateEnd(EEnemyState state);
	void PushState(EEnemyState state, float duration);
	void PushState(EEnemyState state);

	inline bool CheckState(EEnemyState state)
	{
		return m_CurrentState & (int)state;
	}

	inline void SetHealth(int health)
	{
		if (health <= 0)
			return;

		m_HealthPoint = health;
	}

	inline void GuardOn()
	{
		m_Guard = true;
	}

	inline void GuardOff()
	{
		m_Guard = false;
		m_Sprite->ChangeAnimation("Idle");
	}

	inline void Block()
	{
		m_Sprite->ChangeAnimation("Block_success");
		PushState(EEnemyState::GETHIT);
	}

	inline void BlockSuccess()
	{
		m_Sprite->ChangeAnimation("Block");
		PopStateEnd(EEnemyState::GETHIT);
	}

	inline void BlockEnd()
	{
		m_Sprite->ChangeAnimation("Block_end");
	}

	inline bool IsGrabbable() const
	{
		return m_Grabbable;
	}

public:
	inline bool IsInvincible() const
	{
		return m_Invincible;
	}

	inline bool IsFacingRight() const
	{
		return m_FacingRight;
	}

	inline bool IsJumping() const
	{
		return m_Jump;
	}

	inline CSpriteComponent* GetSpriteComponent() const
	{
		return m_Sprite;
	}

	inline bool OnGuard() const
	{
		return m_Guard;
	}

	inline bool IsOnGround() const
	{
		return m_OnGround;
	}

	inline void ChangeToFall()
	{
		m_Sprite->ChangeAnimation("Knockdown_fall");
	}

	inline void StartSpawn()
	{
		m_Sprite->SetOpacity(0.0f);
		m_Spawning = true;
	}

	inline bool IsSpawning() const
	{
		return m_Spawning;
	}

	inline void GetUpStart()
	{
		m_Invincible = true;
	}

	inline void WallHitStart()
	{
		m_Invincible = true;
		m_Physics = false;
		m_GravityEnable = false;
		PopStateEnd(EEnemyState::KNOCKDOWN);
		PopStateEnd(EEnemyState::BLOWNBACK);
		m_AbsVel = Vector2(0.0f, 0.0f);
		m_Velocity = Vector2(0.0f, 0.0f);
	}

	inline void WallHitEnd()
	{
		m_Invincible = false;
		m_Physics = true;
		m_OnGround = false;
		m_GravityEnable = true;
		m_Velocity.y = 0.05f;
		m_KnockbackForce.x *= -0.35f;
		PushState(EEnemyState::KNOCKDOWN, m_CurrentTime + 0.25f);
		m_KnockbackForce.y = fabs(m_KnockbackForce.y) + 1.2f;
		m_Sprite->ChangeAnimation("Knockdown");
	}

	inline CNavAgent* GetNavAgent() const
	{
		return m_NavAgent;
	}

	inline float GetWalkSpeed() const
	{
		return m_WalkSpeed;
	}

	inline float GetRunSpeed() const
	{
		return m_RunSpeed;
	}

	inline int GetCurrentHP() const
	{
		return m_HealthPoint;
	}

	inline CColliderBox2D* GetBodyCollider() const
	{
		return m_Body;
	}

	inline CColliderBox2D* GetBottomCollider() const
	{
		return m_Bottom;
	}

	inline bool GetTopWallCollideInfo() const
	{
		return m_TopWallCollision;
	}

	inline bool GetBottomWallCollideInfo() const
	{
		return m_BottomWallCollision;
	}

	inline bool GetRightWallCollideInfo() const
	{
		return m_RightWallCollision;
	}

	inline bool GetLeftWallCollideInfo() const
	{
		return m_LeftWallCollision;
	}

	inline virtual void GetUpComplete()
	{
		if (m_StunAccumulate >= 1.0f)
		{
			SetStun();
			m_StunAccumulate = 0.0f;
		}

		if (m_Stunned)
		{
			m_Sprite->ChangeAnimation("Stunned");
			PushState(EEnemyState::STUNNED, m_CurrentTime + 5.0f);
			m_Stunned = false;
		}
		else
			m_Sprite->ChangeAnimation("Idle");

		//m_Sprite->ChangeAnimation("Stunned");
		//PushState(EEnemyState::STUNNED, m_CurrentTime + 5.0f);

		PopStateEnd(EEnemyState::GETHIT);
		m_Invincible = false;
	}

	inline void GetGroundHit()
	{
		m_Invincible = true;
		PushState(EEnemyState::GETHIT);
	}

	inline void GroundHitFinish()
	{
		m_Invincible = false;
		ChangeToDown();
		PopStateEnd(EEnemyState::GETHIT);
	}

	inline int GetCurrentState() const
	{
		return m_CurrentState;
	}

	inline void ChangeToDown()
	{
		m_Sprite->ChangeAnimation("Down");
	}

	inline void GrabbedReleaseEnd()
	{
		m_Sprite->ChangeAnimation("Idle");
		PopStateEnd(EEnemyState::GETHIT);
	}

	inline void Grabbed()
	{
		m_Sprite->ChangeAnimation("Grabbed");
	}

	inline void GutPunchEnd()
	{
		m_Sprite->ChangeAnimation("Grabbed");
	}

	inline void SetStun()
	{
		m_Stunned = true;
	}

public:
	inline void SetPlayer(class CGameObject* player)
	{
		m_Player = player;
	}

	inline void SetFacingDirection(bool right)
	{
		m_FacingRight = right;
	}

public:
	inline void MoveLeft()
	{
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * -m_WalkSpeed * CEngine::GetInst()->GetDeltaTime());
	}

	inline void MoveRight()
	{
		m_Transform->AddWorldPos(GetWorldAxis(AXIS_X) * m_WalkSpeed * CEngine::GetInst()->GetDeltaTime());
	}

	void PlayerDetect();
	

	void JumpStart();

public:
	inline void ActivateHitBox()
	{
		m_HitBox->Enable(true);
	}

	inline void DeactivateHitBox()
	{
		m_HitBox->Enable(false);
	}

	inline void CollidersRenderToggle()
	{
		m_Body->RenderToggle();
		m_Bottom->RenderToggle();
		m_HitBox->RenderToggle();
		m_ContactBox->RenderToggle();
	}

public:
	void AddWallCollisionCallback(CColliderBox2D* collider);
	void DeleteWallCollisionCallback(CColliderBox2D* collider);

public:
	virtual void OnGround(const sCollisionResult& result);
	virtual void OnWallCollision(const sCollisionResult& result);
	virtual void OnWallCollisionExit(const sCollisionResult& result);
};

