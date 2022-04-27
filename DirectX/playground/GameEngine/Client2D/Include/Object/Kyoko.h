#pragma once

#include "RCGPlayer.h"

enum class EKyokoMoveSet
{
	GROUND_DEFAULT_STATE,
	GROUND_WEAK_QC1_CHOP,
	GROUND_WEAK_QC2_SNAPKICK,
	GROUND_WEAK_QC3_CRESENTKICK,
	GROUND_WEAK_QC4_HOOKKICK,
	GROUND_WEAK_QC5_BACKKICK,
	GROUND_HEAVY_AXEKICK,
	GROUND_HEAVY_SPINKICK,
	GROUND_HEAVY_VOLLEYSET,
	GROUND_HEAVY_DAB,
	GROUND_HEAVY_STOMP,
	GUARD_DEFAULT,
	GUARD_HEAVY_DONKEYKICK,
	DASH_DEFAULT_STATE,
	DASH_WEAK_BACK_ELBOW,
	DASH_HEAVY_DIVE,
	DASH_SPECIAL_DROPKICK,
	SPECIAL_ACROCIRCUS,
	SPECIAL_CHEERDRILL,
	SPECIAL_DRAGONFEET,
	SPECIAL_HURRICANEKICK,
	AIR_DEFAULT,
	AIR_HEAVY_AIRSTEP,
	AIR_WEAK_BUTTBUMP,
	AIR_HEAVY_SPECIAL_VOLLEYSPIKE,
	AIR_SPECIAL_CHEERDRILL,
	GRAB_FRONT,
	GRAB_BEHIND,
	GRAB_FRAKENSTEINER,
	GRAB_KICKTOSS,
	GRAB_GUTPUNCH,
	GRAB_POGOKICK,
};

class CKyoko :
	public CRCGPlayer
{
	friend class CScene;

protected:
	CKyoko();
	CKyoko(const CKyoko& kyoko);
	virtual ~CKyoko();

private:
	EKyokoMoveSet m_CurrentMove;
	std::unordered_map<EKyokoMoveSet, std::vector<EKyokoMoveSet>> m_mapChainMoveSets;
	std::unordered_map<EKyokoMoveSet, sPlayerMove> m_mapMoveSetInfo;
	bool m_AirBoostJump;
	float m_AirBoostTime;
	float m_PushForce;
	float m_UpwardForce;

	float m_LastAttackInputTime;

	bool m_Push;
	bool m_Upward;
	bool m_DragonFeetBlowback;
	bool m_Bounce;

	int m_BounceCount;

	Vector3 m_Offset;

	bool m_AfterImageWithInterval;
	float m_AfterImageInterval;
	float m_Interval;
	bool m_AfterImagePrevFrame;

	bool m_Dab;
	Vector3 m_DabOffset;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CKyoko* Clone();

private:
	void CreateChainComboSets();
	void SetMoveSetInfo();

private:
	void WeakAttack(float deltaTime);
	void HeavyAttack(float deltaTime);
	void SpecialAttack(float deltaTime);
	void Action();
	void ThrowEnemy();
	void Guard(float deltaTime);
	void GuardEnd(float deltaTime);

public:
	void HitBoxUpdate();

public:
	void ApplyPush();
	void EndPush();
	void ApplyUpwardForce();
	void EndUpwardForce();

private:
	inline void BlockEnd()
	{
		m_Sprite->ChangeAnimation("Idle");
		m_Guard = false;
	}

	inline void DragonFeetBlowBack()
	{
		m_DragonFeetBlowback = true;
	}

	inline void DragonFeetBlowBackEnd()
	{
		m_DragonFeetBlowback = false;
		m_AfterImagePrevFrame = false;
		m_AfterImage->SetEffect(false);
	}

	inline void VolleySpikeStart()
	{
		m_GravityEnable = false;
		m_AbleToMove = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
	}

	inline void VolleySpikeEnd()
	{
		m_GravityEnable = true;
		m_AbleToMove = true;
		m_Sprite->ChangeAnimation("Jump");
	}

	void StartAcrocircus();
	void DropKickBegin();
	void DiveBegin();
	void SpriteOffset();
	void ResetSpriteOffset();

	void KickTossBegin();
	void KickTossOffset();
	void KickToss();
	void KickTossEnd();

	void FrankenBegin();
	void FrankenOffset();
	void Franken();
	void FrankenEnd();

	void PogoKickBegin();
	void PogoKickOffset();

	inline void PogoKickPhysicsOn()
	{
		m_Physics = true;
	}

	void PogoKick();
	void PogoKickEnd();

	inline void DonkeyKickEnd()
	{
		if (m_Guard)
		{
			m_Sprite->ChangeAnimation("Block");
			m_AbleToMove = true;
		}
		else
		{
			m_Sprite->ChangeAnimation("Blockend");
		}

		PopStateEnd(EKyokoState::ATTACK);
		m_CurrentAttackInputFlag = 0;
		m_LastAttackInputTime = 0.0f;
		DeactivateHitBox();
		m_HitEnemyFlag &= ~(int)EHitFlag::HIT;
	}


	virtual void GutPunch();
	void GutPunchEnd();


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

		int randomSoundIndex = (rand() % 7) + 1;

		std::string name = "KyokoGethit" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}


	inline void BounceOff()
	{
		m_Push = false;
		m_Upward = false;
		m_Velocity = Vector2(0.0f, 0.0f);
		m_AbsVel = Vector2(0.0f, 0.0f);
	}

	virtual inline void ParryEnd()
	{
		PopStateEnd(EKyokoState::GETHIT);
		m_CurrentMove = EKyokoMoveSet::GROUND_DEFAULT_STATE;
		m_Guard = false;
	}

	void Stomp();

	void DragonFeetAfterImage();

	void DabAfterImage();

	void DabAfterImageEnd();

private:
	virtual void OnEnemyContact(const sCollisionResult& result);
	virtual void OnEnemyContactExit(const sCollisionResult& result);
	void HitBoxCheck(const sCollisionResult& result);
	virtual void OnGround(const sCollisionResult& result);
	virtual void GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right);

private:
	void GroundAttackEndCallback();
	void AirAttackEndCallback();
	void AirCheerDrillBeginCallback();
	void AirCheerDrillEndCallback();
};

