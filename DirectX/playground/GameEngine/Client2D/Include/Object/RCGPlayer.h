#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/SceneComponent.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/NavAgent.h"
#include "CameraPoint.h"
#include "AfterImage.h"
#include "../Client.h"

struct sPlayerMove
{
	int keyType;
	int moveFlag;
	bool bAir;
	bool bCancel;
	bool bGuard;
	int hitFlag;
	EAttackType attackType;
	Vector2 forceDir;
	int damage;
	int getSP;
	int sp;
	float force;
	float forceTime;
	std::string animName;
	std::string soundName;

	sPlayerMove(int _key, int _hitFlag, bool _air, bool _cancel, bool _guard) :
		keyType(_key), hitFlag(_hitFlag), moveFlag(0), bAir(_air), bCancel(_cancel), bGuard(_guard), animName(), soundName(),
		attackType(EAttackType::HIT), forceDir{}, force(0.0f), forceTime(0.0f)
	{

	}

	sPlayerMove() {}
};

class CRCGPlayer :
	public CGameObject
{
	friend class CScene;
protected:
	CRCGPlayer();
	CRCGPlayer(const CRCGPlayer& obj);
	virtual ~CRCGPlayer();

protected:
	CSharedPtr<CSceneComponent> m_Transform;
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CSpriteComponent> m_Shadow;
	CSharedPtr<CColliderBox2D> m_Body;
	CSharedPtr<CColliderBox2D> m_Bottom;
	CSharedPtr<CColliderBox2D> m_ContactBox;
	CSharedPtr<CCameraComponent> m_Camera;
	CSharedPtr<CColliderBox2D> m_HitBox;

	CCameraPoint* m_CameraPoint;

	float m_Opacity;
	float m_DoubleTapCheckTime;
	float m_CurrentTime;
	float m_RunTapLastTime;
	float m_DodgeTapLastTime;
	float m_JumpTime;
	float m_WallJumpTime;
	float m_WalkSpeed;
	float m_RunSpeed;
	float m_JumpSpeed;
	float m_WallJumpSpeed;
	float m_Gravity;
	float m_BlinkTime;
	bool m_BattleStart;

	int m_HitCount;
	const int m_HitAnimCnt = 3;

	bool m_FacingRight;
	bool m_OnGround;
	bool m_AbleToMove;
	bool m_AbleToAttack;
	bool m_Invincible;
	bool m_Physics;
	bool m_HalfOpacity;

	bool m_RunTap;
	bool m_DodgeTap;

	bool m_Guard;

	bool m_IsRunning;
	bool m_IsMovingLeft;
	bool m_IsMovingRight;
	bool m_IsMovingUp;
	bool m_IsMovingDown;

	bool m_LeftWallCollision;
	bool m_RightWallCollision;
	bool m_TopWallCollision;
	bool m_BottomWallCollision;

	bool m_Dodge;
	bool m_WallJump;
	bool m_Jump;

	bool m_GravityEnable;
	bool m_Stunned;

	int m_HitEnemyFlag;

	EMoveDirection m_DodgeDirection;
	int m_CurrentState;
	int m_CurrentMoveFlag;
	int m_CurrentAttackInputFlag;
	float m_StateEnd[(int)EKyokoState::MAX];
	float m_GuardStartTime;


	Vector2 m_Acceleration;
	Vector2 m_Velocity;

	Vector2 m_AbsAccel;
	Vector2 m_AbsVel;

	Vector2 m_KnockbackForce;

	std::list<class CColliderComponent*> m_ContactEnemies;

	class CGameObject* m_Enemy;
	class CRCGMainWidget* m_MainWidget;

	int m_MaxHealth;
	int m_Health;
	int m_SpecialGauge;

	CSharedPtr<CAfterImage> m_AfterImage;

private: // NavAgent Test
	CSharedPtr<CNavAgent> m_NavAgent;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CRCGPlayer* Clone();
	
public:
	inline void SetMainWidget(class CRCGMainWidget* widget)
	{
		m_MainWidget = widget;
	}

	inline void SetCameraPoint(CCameraPoint* cam)
	{
		m_CameraPoint = cam;
	}

	inline CCameraPoint* GetCameraPoint() const
	{
		return m_CameraPoint;
	}

	inline CColliderBox2D* GetBottom() const
	{
		return m_Bottom;
	}

public:
	void ApplyForce(const Vector2& force, bool isRelative = false);
	void ApplyForce(float x, float y, bool isRelative = false);
	void ApplyAbsForce(const Vector2& force);
	void ApplyAbsForce(float x, float y);

public:
	virtual int PopStateEnd(const float currentTime);
	virtual void PopStateEnd(EKyokoState state);
	virtual void PushState(EKyokoState state, float duration);
	virtual void PushState(EKyokoState state);
	inline bool CheckState(EKyokoState state)
	{
		return m_CurrentState & (int)state;
	}

	inline void ActivateHitBox()
	{
		m_HitBox->Enable(true);
	}

	inline void DeactivateHitBox()
	{
		m_HitBox->Enable(false);
	}

	inline bool IsInvincible() const
	{
		return m_Invincible;
	}

	inline bool IsPhysicsOn() const
	{
		return m_Physics;
	}

	inline bool IsJumping() const
	{
		return m_Jump;
	}

	inline bool OnGuard() const
	{
		return m_Guard;
	}

	inline bool GetTopWallCollideInfo() const
	{
		return m_TopWallCollision;
	}

	inline bool GetBottomWallCollideInfo() const
	{
		return m_BottomWallCollision;
	}

	inline bool GetLeftWallCollideInfo() const
	{
		return m_LeftWallCollision;
	}

	inline bool GetRightWallCollideInfo() const
	{
		return m_RightWallCollision;
	}

	inline float GetGuardStartTime() const
	{
		return m_GuardStartTime;
	}

	inline void BattleStartBegin()
	{
		m_ContactBox->Enable(false);
		m_AbleToMove = false;
		m_Sprite->ChangeAnimation("BattleStart");
		m_BattleStart = true;
	}

	inline void BattleStartEnd()
	{
		m_AbleToMove = true;
		m_BattleStart = false;
		m_CameraPoint->StartFollow();
	}

	inline void SetBodyYPos(float y)
	{
		Vector3 pos = m_Transform->GetWorldPos();
		m_Sprite->SetWorldPos(pos.x, y, pos.z);
		m_Body->SetWorldPos(pos.x, y, pos.z);
	}

public:
	virtual void OnGround(const sCollisionResult& result);
	virtual void OnWallCollision(const sCollisionResult& result);
	virtual void OnWallCollisionExit(const sCollisionResult& result);

public:
	void MoveUp(float deltaTime);
	void MoveUpEnd(float deltaTime);
	void MoveDown(float deltaTime);
	void MoveDownEnd(float deltaTime);
	void MoveLeft(float deltaTime);
	void MoveLeftEnd(float deltaTime);
	void MoveRight(float deltaTime);
	void MoveRightEnd(float deltaTime);
	void JumpStart(float deltaTime);
	void JumpEnd(float deltaTime);
	void Run(float deltaTime);
	void DodgeUp(float deltaTime);
	void DodgeDown(float deltaTime);
	void DodgeEnd();
	virtual void Guard(float deltaTime);
	virtual void GuardEnd(float deltaTime);

public:
	inline void GrabbingFinish()
	{
		m_Sprite->ChangeAnimation("Grabrelease");
		m_AbleToAttack = false;
	}

	inline void GrabbingReleaseComplete()
	{
		PopStateEnd(EKyokoState::GRABBING);
		PopStateEnd(EKyokoState::ATTACK);
		m_Sprite->ChangeAnimation("Idle");
		m_AbleToAttack = true;
	}

	inline void WallJumpStart()
	{
		m_GravityEnable = false;
		m_Invincible = true;
		m_AbleToMove = false;
		m_AbleToAttack = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
	}

	void WallJump();

	inline void SetStun()
	{
		m_Stunned = true;
	}

	inline void WallHitStart()
	{
		m_Invincible = true;
		m_Physics = false;
		m_GravityEnable = false;
		PopStateEnd(EKyokoState::KNOCKDOWN);
		PopStateEnd(EKyokoState::BLOWNBACK);
		m_Scene->GetCameraManager()->GetCurrentCamera()->ActiveCameraShake();
		m_AbsVel = Vector2(0.0f, 0.0f);
		m_Velocity = Vector2(0.0f, 0.0f);
	}

	inline virtual void WallHitEnd()
	{
		m_Invincible = false;
		m_Physics = true;
		m_OnGround = false;
		m_GravityEnable = true;
		m_Velocity.y = 0.05f;
		m_KnockbackForce.x *= -0.5f;
		m_LeftWallCollision = false;
		m_RightWallCollision = false;
		PushState(EKyokoState::KNOCKDOWN, m_CurrentTime + 0.25f);
		PushState(EKyokoState::GETHIT);
		m_KnockbackForce.y = fabs(m_KnockbackForce.y) + 1.2f;
		m_Sprite->ChangeAnimation("Knockdown");
	}

	inline void GetUpStart()
	{
		m_Invincible = true;
	}

	inline void GetUpComplete()
	{
		if (m_Stunned)
		{
			m_Sprite->ChangeAnimation("Stunned");
			CResourceManager::GetInst()->SoundPlay("Dizzy");
			m_Stunned = false;
			PushState(EKyokoState::STUNNED, m_CurrentTime + 3.0f);
		}
		else if(!m_Stunned)
		{
			m_Sprite->ChangeAnimation("Idle");
			m_BlinkTime = 0.0f;
			m_HalfOpacity = true;
			PushState(EKyokoState::INVINCIBLE, m_CurrentTime + 2.0f);
		}

		PopStateEnd(EKyokoState::GETHIT);
		m_AbleToMove = true;
		m_Invincible = false;
	}

	inline void ChangeToFall()
	{
		m_Sprite->ChangeAnimation("Knockdown_fall");
	}

	inline void ChangeToDown()
	{
		m_Sprite->ChangeAnimation("Down");
	}

	inline void Block()
	{
		m_Sprite->ChangeAnimation("Block_success");
		PushState(EKyokoState::GETHIT);
	}

	inline void BlockEnd()
	{
		if (m_Guard)
			m_Sprite->ChangeAnimation("Block");
		else
			m_Sprite->ChangeAnimation("Idle");

		PopStateEnd(EKyokoState::GETHIT);
	}

	inline void Parry()
	{
		m_Sprite->ChangeAnimation("Parry");
		PushState(EKyokoState::GETHIT);
	}

	virtual inline void ParryEnd()
	{
		PopStateEnd(EKyokoState::GETHIT);
		m_Guard = false;
	}


	virtual void GutPunch();

	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);
	void HitStatusEnd();

public:
	void AddWallCollisionCallback(CColliderBox2D* collider);
	void DeleteWallCollisionCallback(CColliderBox2D* collider);

	inline void DeleteContactEnemy(CColliderComponent* collider)
	{
		auto iter = std::find(m_ContactEnemies.begin(), m_ContactEnemies.end(), collider);

		if (iter != m_ContactEnemies.end())
			m_ContactEnemies.erase(iter);
	}
private:
	virtual void OnEnemyContact(const sCollisionResult& result);
	virtual void OnEnemyContactExit(const sCollisionResult& result);


public:
	void SetHP();
	void SetSP();

	void Defeated();
	void GameOver();

public:
	inline int GetHealth() const
	{
		return m_Health;
	}

	inline int GetSP() const
	{
		return m_SpecialGauge;
	}

	inline void CollidersRenderToggle()
	{
		m_Body->RenderToggle();
		m_Bottom->RenderToggle();
		m_HitBox->RenderToggle();
		m_ContactBox->RenderToggle();
	}

private: // Navigation test
	void MouseRDown(float deltaTime);
};