#include "Kyoko.h"
#include "KyokoAnimation2D.h"
#include "Input.h"
#include "RCGEnemy.h"
#include "Scene/SceneManager.h"
#include "Engine.h"
#include "../Widget/RCGMainWidget.h"
#include "HitEffect.h"

CKyoko::CKyoko() :
	m_CurrentMove(EKyokoMoveSet::GROUND_DEFAULT_STATE)
{
}

CKyoko::CKyoko(const CKyoko& kyoko) :
	CRCGPlayer(kyoko),
	m_CurrentMove(EKyokoMoveSet::GROUND_DEFAULT_STATE)
{
}

CKyoko::~CKyoko()
{
}

bool CKyoko::Init()
{
	CRCGPlayer::Init();

	m_Bottom->AddCollisionCallback<CKyoko>(Collision_State::Begin, this, &CKyoko::OnGround);

	m_ContactBox->AddCollisionCallback<CKyoko>(Collision_State::Begin, this, &CKyoko::OnEnemyContact);
	m_ContactBox->AddCollisionCallback<CKyoko>(Collision_State::End, this, &CKyoko::OnEnemyContactExit);

	m_Sprite->CreateAnimationInstance<CKyokoAnimation2D>();

	CInput::GetInst()->SetCallback<CKyoko>("Guard", KeyState_Down, this, &CKyoko::Guard);
	CInput::GetInst()->SetCallback<CKyoko>("Guard", KeyState_Up, this, &CKyoko::GuardEnd);

	CreateChainComboSets();

	m_HitEnemyFlag = (int)EHitFlag::NONE;

	m_AfterImage = m_Scene->CreateGameObject<CAfterImage>("KyokoAfter Image");

	m_AfterImage->SetAnimationInstance<CKyokoAnimation2D>();

	m_LastAttackInputTime = 0.0f;

	m_AirBoostJump = false;

	m_Bounce = false;

	m_DragonFeetBlowback = false;

	SetMoveSetInfo();

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Hit1", this, &CRCGPlayer::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Hit2", this, &CRCGPlayer::HitStatusEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Hit3", this, &CRCGPlayer::HitStatusEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Knockdown_midair", this, &CRCGPlayer::ChangeToFall);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Blownback_midair", this, &CRCGPlayer::ChangeToFall);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Knockdown_ground", this, &CRCGPlayer::ChangeToDown);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("BattleStartGround", this, &CRCGPlayer::BattleStartEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGPlayer>("Getup", this, &CRCGPlayer::GetUpStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Getup", this, &CRCGPlayer::GetUpComplete);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGPlayer>("Wallhit", this, &CRCGPlayer::WallHitStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Wallhit", this, &CRCGPlayer::WallHitEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGPlayer>("Walljump", this, &CRCGPlayer::WallJumpStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Walljump", this, &CRCGPlayer::WallJump);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Block_success", this, &CRCGPlayer::BlockEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Parry", this, &CRCGPlayer::ParryEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Dodge", this, &CRCGPlayer::DodgeEnd);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("QC1_Chop", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("QC2_Snapkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("QC3_Cresentkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("QC4_Hookkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("QC5_Backkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Backelbow", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Dive", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Dropkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Axekick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Volleyset", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Spinkick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Dab", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Hurricanekick", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Newcheerdrill", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Stomp", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Donkeykick", this, &CKyoko::DonkeyKickEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Blockend", this, &CKyoko::BlockEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Cheerdrill", this, &CKyoko::AirCheerDrillBeginCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Cheerdrill", this, &CKyoko::AirCheerDrillEndCallback);


	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Buttbump", this, &CKyoko::AirAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Airstep", this, &CKyoko::AirAttackEndCallback);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Volleyspike", this, &CKyoko::AirAttackEndCallback);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Dragonfeet", this, &CKyoko::GroundAttackEndCallback);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC1_Chop", "QC1HitBoxOn", 2, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC1_Chop", "QC1HitBoxOff", 3, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC2_Snapkick", "QC2HitBoxOn", 2, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC2_Snapkick", "QC2HitBoxOff", 3, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC3_Cresentkick", "QC3HitBoxOn", 3, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC3_Cresentkick", "QC3HitBoxOff", 4, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC4_Hookkick", "QC4HitBoxOn", 3, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC4_Hookkick", "QC4HitBoxOff", 4, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC5_Backkick", "QC5HitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("QC5_Backkick", "QC5HitBoxOff", 5, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Backelbow", "ApplySlide", 1, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Backelbow", "EndSlide", 3, this, &CKyoko::EndPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Backelbow", "BackelbowHitBoxOn", 2, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Backelbow", "BackelbowHitBoxOff", 3, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Dive", this, &CKyoko::DiveBegin);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dive", "DiveHitBoxOn", 2, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dive", "ApplyPush", 2, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dive", "EndPush", 15, this, &CKyoko::EndPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dive", "DiveHitBoxOff", 7, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Dropkick", this, &CKyoko::DropKickBegin);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dropkick", "DropkickHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dropkick", "ApplyPush", 2, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dropkick", "EndPush", 5, this, &CKyoko::EndPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dropkick", "DropkickHitBoxOff", 6, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "AxekickOffsetBegin", 1, this, &CKyoko::SpriteOffset);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "ApplySlide", 3, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "EndSlide", 5, this, &CKyoko::EndPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "AxekickHitBoxOn", 5, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "AxekickHitBoxoff", 6, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Axekick", "AxekickOffsetEnd", 6, this, &CKyoko::ResetSpriteOffset);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Spinkick", "SpinkickOffset", 3, this, &CKyoko::SpriteOffset);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Spinkick", "SpinkickHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Spinkick", "SpinkickHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Spinkick", "SpinkickOffset", 8, this, &CKyoko::ResetSpriteOffset);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Buttbump", "ButtbumpHitBoxOn", 3, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Buttbump", "ButtbumpHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Airstep", "AirStepHitBoxOn", 5, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Airstep", "AirStepHitBoxOff", 6, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Volleyspike", "VolleySpikeHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Volleyspike", "VolleySpikeHitboxOff", 5, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetAfterImage", 4, this, &CKyoko::DragonFeetAfterImage);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 6, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOff", 7, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 8, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOff", 9, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 10, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOff", 11, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 12, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOff", 13, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetBlowback", 13, this, &CKyoko::DragonFeetBlowBack);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetHitBoxOn", 14, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dragonfeet", "DragonfeetBlowbackOff", 16, this, &CKyoko::DragonFeetBlowBackEnd);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillOffset", 3, this, &CKyoko::SpriteOffset);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillPush", 3, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillOffset", 9, this, &CKyoko::ResetSpriteOffset);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillPushEnd", 9, this, &CKyoko::EndPush);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Hurricanekick", "HurricanekickPush", 5, this, &CKyoko::ApplyPush);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Hurricanekick", "HurricanekickPush", 17, this, &CKyoko::EndPush);

	for (int i = 5; i < 18; i++)
	{
		if (i % 2 == 1)
			m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Hurricanekick", "HurricanekickHitBoxOn", i, this, &CKyoko::ActivateHitBox);
		else
			m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Hurricanekick", "HurricanekickHitBoxOn", i, this, &CKyoko::DeactivateHitBox);

	}


	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dab", "DabHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dab", "DabAfterImageStart", 4, this, &CKyoko::DabAfterImage);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dab", "DabHitBoxOff", 6, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Dab", "DabAfterImageEnd", 22, this, &CKyoko::DabAfterImageEnd);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Acrocircus", this, &CKyoko::StartAcrocircus);

	for (int i = 0; i < 8; i++)
	{
		if (i % 2 == 0)
			m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Acrocircus_loop", "AcrocircusHitBoxOn", i, this, &CKyoko::ActivateHitBox);
		else
			m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Acrocircus_loop", "AcrocircusHitBoxOn", i, this, &CKyoko::DeactivateHitBox);

		if (i == 4)
		{
			m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Acrocircus_loop", "AcrocircusHitBoxOn", i, this, &CKyoko::EndUpwardForce);
		}
	}

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOn", 6, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOff", 7, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOn", 8, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Cheerdrill", "CheerDrillHitBoxOff", 9, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillHitBoxOn", 6, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Newcheerdrill", "NewcheerdrillHitBoxOff", 7, this, &CKyoko::DeactivateHitBox);


	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Donkeykick", "DonkeykickHitBoxOn", 4, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Donkeykick", "DonkeykickHitBoxOff", 5, this, &CKyoko::DeactivateHitBox);

	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Acrocircus_land", this, &CKyoko::GroundAttackEndCallback);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Acrocircus_land", "BounceEnd", 5, this, &CKyoko::BounceOff);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Stomp", "StompDamage", 4, this, &CKyoko::Stomp);
	
	m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGPlayer>("Grabrelease", this, &CRCGPlayer::GrabbingReleaseComplete);

	m_Sprite->GetAnimationInstance()->AddNotify<CRCGPlayer>("Gutpunch", "GutpunchBegin", 1, this, &CRCGPlayer::GutPunch);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Gutpunch", this, &CKyoko::GutPunchEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Kicktoss", this, &CKyoko::KickTossBegin);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Kicktoss", "Throw", 3, this, &CKyoko::KickToss);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Kicktoss", this, &CKyoko::KickTossEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Franken", this, &CKyoko::FrankenBegin);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Franken", "Throw", 7, this, &CKyoko::Franken);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Franken", "ResetOffset", 11, this, &CKyoko::ResetSpriteOffset);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Franken", this, &CKyoko::FrankenEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Pogokick", this, &CKyoko::PogoKickBegin);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Pogokick", "PogokickOffset", 1, this, &CKyoko::PogoKickOffset);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Pogokick", "Pogokick", 5, this, &CKyoko::PogoKick);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Pogokick", "PhysicsOn", 10, this, &CKyoko::PogoKickPhysicsOn);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Pogokick", this, &CKyoko::PogoKickEnd);

	m_Sprite->GetAnimationInstance()->SetStartFunction<CKyoko>("Volleyspike", this, &CKyoko::VolleySpikeStart);
	m_Sprite->GetAnimationInstance()->SetEndFunction<CKyoko>("Volleyspike", this, &CKyoko::VolleySpikeEnd);

	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Volleyset", "VolleysetHitBoxOn", 5, this, &CKyoko::ActivateHitBox);
	m_Sprite->GetAnimationInstance()->AddNotify<CKyoko>("Volleyset", "VolleysetHitBoxOff", 7, this, &CKyoko::DeactivateHitBox);

	m_HitBox->AddCollisionCallback<CKyoko>(Collision_State::Begin, this, &CKyoko::HitBoxCheck);

	CInput::GetInst()->SetCallback<CKyoko>("WeakAttack", KeyState_Down, this, &CKyoko::WeakAttack);
	CInput::GetInst()->SetCallback<CKyoko>("HeavyAttack", KeyState_Down, this, &CKyoko::HeavyAttack);
	CInput::GetInst()->SetCallback<CKyoko>("Special", KeyState_Down, this, &CKyoko::SpecialAttack);

	return true;
}


void CKyoko::Update(float deltaTime)
{
	CRCGPlayer::Update(deltaTime);

	if (!m_OnGround && m_GravityEnable)
	{
		if (m_Jump)
		{
			if (m_CurrentTime - m_JumpTime <= 0.17f)
			{
				ApplyForce(0.0f, m_JumpSpeed * deltaTime, true);

				if (m_Velocity.y >= 15.0f)
					m_Velocity.y = 15.0f;
			}
			else
				m_Jump = false;
		}
		else if (CheckState(EKyokoState::KNOCKDOWN) || CheckState(EKyokoState::BLOWNBACK)
			|| CheckState(EKyokoState::POGOKICK))
		{
			ApplyForce(0.0f, m_KnockbackForce.y * deltaTime, true);
			ApplyForce(0.0f, m_KnockbackForce.y * -0.01f * deltaTime, true);

			if (m_Velocity.y >= 10.0f)
				m_Velocity.y = 10.0f;
		}

		if (m_WallJump)
		{
			if (m_CurrentTime - m_WallJumpTime <= 0.3f)
			{
				ApplyAbsForce(m_WallJumpSpeed * deltaTime, 0.0f);

				if (fabsf(m_AbsVel.x) >= 10.0f)
					m_AbsVel.x = 10.0f;
			}
			else
				ApplyAbsForce(m_WallJumpSpeed * deltaTime * -0.1f, 0.0f);
		}

		if (!CheckState(EKyokoState::KNOCKDOWN) && !CheckState(EKyokoState::BLOWNBACK))
		{
			ApplyForce(0.0f, m_Gravity * deltaTime, true);

			if (m_Velocity.y <= -10.0f)
				m_Velocity.y = -10.0f;

		}

		if (m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Jump"))
		{
			if (m_Velocity.y > 0.0f)
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
			else if (m_Velocity.y < 0.0f && m_Velocity.y > -0.5f)
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(1);
			else
				m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(2);
		}
	}

	if (m_BattleStart)
		return;

	if (CheckState(EKyokoState::INVINCIBLE))
	{
		m_BlinkTime += deltaTime;

		if (m_HalfOpacity)
			m_Sprite->SetOpacity(0.5f);
		else
			m_Sprite->SetOpacity(1.0f);

		if (m_BlinkTime >= 0.1f)
		{
			m_HalfOpacity = !m_HalfOpacity;
			m_BlinkTime = 0.0f;
		}
	}

	if (m_OnGround && !m_IsRunning && !m_Guard && !CheckState(EKyokoState::ATTACK) &&
		!CheckState(EKyokoState::GRABBING) && !CheckState(EKyokoState::GRABBED))
		m_CurrentMove = EKyokoMoveSet::GROUND_DEFAULT_STATE;
	else if (m_OnGround && m_IsRunning && !m_Guard && !CheckState(EKyokoState::ATTACK) &&
		!CheckState(EKyokoState::GRABBING) && !CheckState(EKyokoState::GRABBED))
		m_CurrentMove = EKyokoMoveSet::DASH_DEFAULT_STATE;
	else if (!m_OnGround && !CheckState(EKyokoState::ATTACK) &&
		!CheckState(EKyokoState::GRABBING) && !CheckState(EKyokoState::GRABBED))
		m_CurrentMove = EKyokoMoveSet::AIR_DEFAULT;


	if (CheckState(EKyokoState::KNOCKDOWN) || CheckState(EKyokoState::BLOWNBACK)
		|| CheckState(EKyokoState::POGOKICK))
	{
		if (m_OnGround)
		{
			if (!m_LeftWallCollision && m_KnockbackForce.x < 0.0f)
			{
				ApplyForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else if (!m_RightWallCollision && m_KnockbackForce.x > 0.0f)
			{
				ApplyForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else
				m_Velocity.x = 0.0f;
		}
		else
		{
			if (m_KnockbackForce.x < 0.0f && !m_LeftWallCollision)
			{
				ApplyAbsForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyAbsForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else if (m_KnockbackForce.x > 0.0f && !m_RightWallCollision)
			{
				ApplyAbsForce(m_KnockbackForce.x * deltaTime, 0.0f);
				ApplyAbsForce(m_KnockbackForce.x * -0.01f * deltaTime, 0.0f);
			}
			else
				m_AbsVel.x = 0.0f;
		}
	}
	else if (m_Physics)
	{
		Vector2 v = m_Velocity;

		v *= -1.0f;

		v *= 0.1f;

		ApplyForce(v * deltaTime, true);
		

		v = m_AbsVel;

		v *= -1.0f;

		v *= 0.1f;

		if (!m_LeftWallCollision && v.x < 0.0f)
			ApplyAbsForce(v * deltaTime);
		else if (!m_RightWallCollision && v.x > 0.0f)
			ApplyAbsForce(v * deltaTime);
	}


	if (m_CurrentAttackInputFlag != (int)EPlayerKeySet::NONE)
	{
		Action();

		if (m_CurrentTime - m_LastAttackInputTime >= 0.15f)
		{
			m_LastAttackInputTime = 0.0f;
			m_CurrentAttackInputFlag = 0;
		}
	}

	if (m_HitBox->IsEnable())
		HitBoxUpdate();

	if (m_AirBoostJump)
	{
		if (m_CurrentTime - m_AirBoostTime <= 0.2f)
		{
			ApplyForce(0.0f, 60.0f * deltaTime, true);
		}
		else
			m_AirBoostJump = false;
	}

	if (m_Push)
	{
		if (!m_OnGround)
		{
			if (m_PushForce < 0.0f && !m_LeftWallCollision)
				ApplyAbsForce(m_PushForce * deltaTime, 0.0f);
			else if (m_PushForce > 0.0 && !m_RightWallCollision)
				ApplyAbsForce(m_PushForce * deltaTime, 0.0f);
			else
				m_AbsVel.x = 0.0f;
		}
		else
		{
			if (m_PushForce < 0.0f && !m_LeftWallCollision)
				ApplyForce(m_PushForce * deltaTime, 0.0f);
			else if (m_PushForce > 0.0f && !m_RightWallCollision)
				ApplyForce(m_PushForce * deltaTime, 0.0f);
			else
				m_Velocity.x = 0.0f;
		}
	}

	if (m_Upward)
		ApplyForce(0.0f, m_UpwardForce * deltaTime, true);

	if (m_AfterImageWithInterval)
	{
		m_Interval += deltaTime;

		if (m_Interval >= m_AfterImageInterval)
		{
			m_Interval = 0.0f;
			m_AfterImage->SetFrame(m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame());
			m_AfterImage->SetPos(m_Sprite->GetWorldPos());
			m_AfterImage->IncreaseIndex();
		}
	}
	
	if (m_AfterImagePrevFrame)
	{
		int prevFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame() - 1;

		m_AfterImage->SetFrame(prevFrame);
		Vector3 pos = m_Sprite->GetWorldPos();

		if (m_FacingRight)
			m_AfterImage->SetPos(Vector3(pos.x - 10.0f, pos.y, pos.z));
		else
			m_AfterImage->SetPos(Vector3(pos.x + 10.0f, pos.y, pos.z));
	}

	if (m_Dab)
	{
		int currentFrame = m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

		m_AfterImage->SetFrame(currentFrame);

		Vector4 tintColor = m_AfterImage->GetTintColor();

		tintColor.x = 0.5f * cosf(m_CurrentTime * 10.0f) + 0.5f;

		tintColor.y = 0.5f * sinf(m_CurrentTime * 5.0f) + 0.5f;

		tintColor.z = 0.5f * sinf(cosf(m_CurrentTime * 1.5f)) + 0.5f;


		m_AfterImage->SetTintColor(tintColor);

		m_AfterImage->SetPos(m_Sprite->GetWorldPos());

		m_AfterImage->IncreaseIndex();
	}

	if (!m_OnGround && m_Velocity.y <= 0.0f && !m_Body->GetPrevColliderList().empty())
	{
		auto iter = m_Body->GetPrevColliderList().begin();
		auto iterEnd = m_Body->GetPrevColliderList().end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->GetGameObject() == this)
			{
				m_OnGround = true;
				m_Physics = false;
				m_WallJump = false;
				m_Jump = false;
				m_JumpSpeed = 70.0f;

				if (m_Offset.y != 0.0f)
				{
					ResetSpriteOffset();
					m_Offset = Vector3(0.0f, 0.0f, 0.0f);
				}

				m_Velocity = Vector2(0.0f, 0.0f);
				m_AbsVel = Vector2(0.0f, 0.0f);
				m_AfterImageWithInterval = false;
				m_AfterImage->SetEffect(false);
				m_AfterImageInterval = 0.0f;
			}
		}
	}
}

void CKyoko::PostUpdate(float deltaTime)
{
	CRCGPlayer::PostUpdate(deltaTime);
}

CKyoko* CKyoko::Clone()
{
	return new CKyoko(*this);
}

void CKyoko::CreateChainComboSets()
{
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::GROUND_WEAK_QC1_CHOP);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::GROUND_HEAVY_AXEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::GROUND_HEAVY_SPINKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::SPECIAL_ACROCIRCUS);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::SPECIAL_CHEERDRILL);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::SPECIAL_DRAGONFEET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::SPECIAL_HURRICANEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_DEFAULT_STATE].push_back(EKyokoMoveSet::GROUND_HEAVY_DAB);

	m_mapChainMoveSets[EKyokoMoveSet::AIR_DEFAULT].push_back(EKyokoMoveSet::AIR_WEAK_BUTTBUMP);
	m_mapChainMoveSets[EKyokoMoveSet::AIR_DEFAULT].push_back(EKyokoMoveSet::AIR_HEAVY_AIRSTEP);
	m_mapChainMoveSets[EKyokoMoveSet::AIR_DEFAULT].push_back(EKyokoMoveSet::AIR_HEAVY_SPECIAL_VOLLEYSPIKE);
	m_mapChainMoveSets[EKyokoMoveSet::AIR_DEFAULT].push_back(EKyokoMoveSet::AIR_SPECIAL_CHEERDRILL);

	m_mapChainMoveSets[EKyokoMoveSet::GRAB_FRONT].push_back(EKyokoMoveSet::GRAB_GUTPUNCH);
	m_mapChainMoveSets[EKyokoMoveSet::GRAB_FRONT].push_back(EKyokoMoveSet::GRAB_POGOKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GRAB_BEHIND].push_back(EKyokoMoveSet::GRAB_FRAKENSTEINER);
	m_mapChainMoveSets[EKyokoMoveSet::GRAB_BEHIND].push_back(EKyokoMoveSet::GRAB_GUTPUNCH);
	m_mapChainMoveSets[EKyokoMoveSet::GRAB_FRONT].push_back(EKyokoMoveSet::GRAB_KICKTOSS);

	m_mapChainMoveSets[EKyokoMoveSet::GUARD_DEFAULT].push_back(EKyokoMoveSet::GUARD_HEAVY_DONKEYKICK);

	m_mapChainMoveSets[EKyokoMoveSet::DASH_DEFAULT_STATE].push_back(EKyokoMoveSet::DASH_WEAK_BACK_ELBOW);
	m_mapChainMoveSets[EKyokoMoveSet::DASH_DEFAULT_STATE].push_back(EKyokoMoveSet::DASH_HEAVY_DIVE);
	m_mapChainMoveSets[EKyokoMoveSet::DASH_DEFAULT_STATE].push_back(EKyokoMoveSet::DASH_SPECIAL_DROPKICK);

	m_mapChainMoveSets[EKyokoMoveSet::DASH_WEAK_BACK_ELBOW].push_back(EKyokoMoveSet::GROUND_WEAK_QC1_CHOP);

	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC1_CHOP].push_back(EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK);

	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_AXEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_SPINKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_DAB);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::SPECIAL_ACROCIRCUS);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::SPECIAL_CHEERDRILL);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::SPECIAL_DRAGONFEET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK].push_back(EKyokoMoveSet::SPECIAL_HURRICANEKICK);


	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_AXEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_SPINKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_DAB);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::SPECIAL_ACROCIRCUS);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::SPECIAL_CHEERDRILL);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::SPECIAL_DRAGONFEET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK].push_back(EKyokoMoveSet::SPECIAL_HURRICANEKICK);

	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_AXEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_SPINKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_DAB);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::SPECIAL_ACROCIRCUS);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::SPECIAL_CHEERDRILL);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::SPECIAL_DRAGONFEET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK].push_back(EKyokoMoveSet::SPECIAL_HURRICANEKICK);

	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_AXEKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_SPINKICK);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::GROUND_HEAVY_DAB);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::SPECIAL_ACROCIRCUS);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::SPECIAL_CHEERDRILL);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::SPECIAL_DRAGONFEET);
	m_mapChainMoveSets[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK].push_back(EKyokoMoveSet::SPECIAL_HURRICANEKICK);

	m_mapChainMoveSets[EKyokoMoveSet::SPECIAL_ACROCIRCUS].push_back(EKyokoMoveSet::AIR_SPECIAL_CHEERDRILL);
}

void CKyoko::SetMoveSetInfo()
{
	sPlayerMove info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP |
		(int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT |
		(int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(0.2f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.25f;
	info.force = 2.0f;
	info.sp = 0;
	info.getSP = 2;
	info.damage = 4;
	info.animName = "QC1_Chop";
	info.soundName = "Chop";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_WEAK_QC1_CHOP] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP |
		(int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT |
		(int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::HIT;
	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(0.2f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.25f;
	info.force = 2.0f;
	info.sp = 0;
	info.getSP = 2;
	info.damage = 4;
	info.animName = "QC2_Snapkick";
	info.soundName = "Snapkick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_WEAK_QC2_SNAPKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP |
		(int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT |
		(int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::HIT;
	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(0.2f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.25f;
	info.force = 2.0f;
	info.sp = 0;
	info.getSP = 2;
	info.damage = 4;
	info.animName = "QC3_Cresentkick";
	info.soundName = "Crescentkick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_WEAK_QC3_CRESENTKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP |
		(int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT |
		(int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::HIT;
	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(0.2f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.25f;
	info.force = 2.0f;
	info.sp = 0;
	info.getSP = 2;
	info.damage = 4;
	info.animName = "QC4_Hookkick";
	info.soundName = "Hookkick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_WEAK_QC4_HOOKKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP |
		(int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT |
		(int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::HIT;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.1f, 1.0f);
	info.forceDir.Normalize();
	info.force = 7.5f;
	info.forceTime = 0.25f;
	info.sp = 0;
	info.getSP = 5;
	info.damage = 8;
	info.animName = "QC5_Backkick";
	info.soundName = "Backkick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_WEAK_QC5_BACKKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.1f, -1.0f);
	info.forceDir.Normalize();
	info.force = 12.0f;
	info.forceTime = 0.4f;
	info.sp = 0;
	info.getSP = 7;
	info.damage = 12;
	info.animName = "Axekick";
	info.soundName = "Axekick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_HEAVY_AXEKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.8f, 1.0f);
	info.forceDir.Normalize();
	info.force = 15.0f;
	info.forceTime = 0.35f;
	info.sp = 0;
	info.getSP = 7;
	info.damage = 12;
	info.animName = "Spinkick";
	info.soundName = "Spinkick";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_HEAVY_SPINKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.2f, 1.0f);
	info.forceDir.Normalize();
	info.force = 15.0f;
	info.forceTime = 0.3f;
	info.sp = 0;
	info.getSP = 5;
	info.damage = 12;
	info.animName = "Volleyset";
	info.soundName = "Volleyset";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::BLOWNBACK;
	info.forceDir = Vector2(1.0f, 0.05f);
	info.forceDir.Normalize();
	info.force = 30.0f;
	info.forceTime = 0.3f;
	info.sp = 0;
	info.getSP = 7;
	info.damage = 20;
	info.animName = "Dab";
	info.soundName = "Dab";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_HEAVY_DAB] = info;

	info = {};

	info.bAir = true;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::UP | (int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::HIT;
	info.forceDir = Vector2(0.2f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.2f;
	info.force = 4.0f;
	info.sp = 0;
	info.getSP = 3;
	info.damage = 4;
	info.animName = "Buttbump";
	info.soundName = "Buttbump";

	m_mapMoveSetInfo[EKyokoMoveSet::AIR_WEAK_BUTTBUMP] = info;

	info = {};

	info.bAir = true;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::UP | (int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.2f, 0.1f);
	info.forceDir.Normalize();
	info.forceTime = 0.2f;
	info.force = 5.0f;
	info.sp = 0;
	info.getSP = 2;
	info.damage = 6;
	info.animName = "Airstep";
	info.soundName = "Airstep";

	m_mapMoveSetInfo[EKyokoMoveSet::AIR_HEAVY_AIRSTEP] = info;

	info = {};

	info.bAir = true;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::UP | (int)EMoveDirection::DOWN |
		(int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK | (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.5f, -0.7f);
	info.forceDir.Normalize();
	info.forceTime = 0.4f;
	info.force = 22.0f;
	info.sp = 15;
	info.getSP = 5;
	info.damage = 24;
	info.animName = "Volleyspike";
	info.soundName = "Volleyspike";

	m_mapMoveSetInfo[EKyokoMoveSet::AIR_HEAVY_SPECIAL_VOLLEYSPIKE] = info;

	info = {};

	info.bAir = true;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.5f, 0.25f);
	info.forceDir.Normalize();
	info.forceTime = 0.15f;
	info.force = 10.0f;
	info.getSP = 0;
	info.sp = 20;
	info.damage = 14;
	info.animName = "Cheerdrill";
	info.soundName = "Cheerdrill";

	m_mapMoveSetInfo[EKyokoMoveSet::AIR_SPECIAL_CHEERDRILL] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN | (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::HIT;
	info.getSP = 3;
	info.sp = 0;
	info.damage = 4;
	info.animName = "Stomp";
	info.soundName = "Stomp";

	m_mapMoveSetInfo[EKyokoMoveSet::GROUND_HEAVY_STOMP] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::UP | (int)EMoveDirection::DOWN | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::HIT;
	info.getSP = 2;
	info.sp = 0;
	info.damage = 4;
	info.animName = "Backelbow";
	info.soundName = "Backelbow";

	m_mapMoveSetInfo[EKyokoMoveSet::DASH_WEAK_BACK_ELBOW] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.01f, 1.0f);
	info.forceDir.Normalize();
	info.force = 16.0f;
	info.forceTime = 0.35f;
	info.getSP = 5;
	info.sp = 0;
	info.damage = 14;
	info.animName = "Dive";
	info.soundName = "Dive";

	m_mapMoveSetInfo[EKyokoMoveSet::DASH_HEAVY_DIVE] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::BLOWNBACK;
	info.forceDir = Vector2(3.0f, 0.15f);
	info.forceDir.Normalize();
	info.force = 30.0f;
	info.forceTime = 0.5f;
	info.sp = 15;
	info.getSP = 0;
	info.damage = 25;
	info.animName = "Dropkick";
	info.soundName = "Dropkick";

	m_mapMoveSetInfo[EKyokoMoveSet::DASH_SPECIAL_DROPKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::NONE;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::NONE | (int)EHitFlag::HIT;
	info.attackType = EAttackType::HIT;
	info.sp = 20;
	info.getSP = 0;
	info.damage = 4;
	info.animName = "Dragonfeet";
	info.soundName = "Dragonfeet";

	m_mapMoveSetInfo[EKyokoMoveSet::SPECIAL_DRAGONFEET] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::HIT | (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.5f, 1.0f);
	info.forceDir.Normalize();
	info.forceTime = 0.35f;
	info.force = 10.0f;
	info.sp = 20;
	info.getSP = 0;
	info.damage = 16;
	info.animName = "Newcheerdrill";
	info.soundName = "Cheerdrill";

	m_mapMoveSetInfo[EKyokoMoveSet::SPECIAL_CHEERDRILL] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::HIT | (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(0.1f, 1.0f);
	info.forceDir.Normalize();
	info.forceTime = 0.2f;
	info.force = 10.0f;
	info.sp = 35;
	info.getSP = 0;
	info.damage = 2;
	info.animName = "Hurricanekick";
	info.soundName = "Hurricanekick";

	m_mapMoveSetInfo[EKyokoMoveSet::SPECIAL_HURRICANEKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = true;
	info.moveFlag = (int)EMoveDirection::UP;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::HIT | (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(1.2f, 0.9f);
	info.forceDir.Normalize();
	info.forceTime = 0.2f;
	info.force = 10.0f;
	info.sp = 25;
	info.getSP = 0;
	info.damage = 7;
	info.animName = "Acrocircus";
	info.soundName = "Acrocircus";

	m_mapMoveSetInfo[EKyokoMoveSet::SPECIAL_ACROCIRCUS] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::NONE | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN;
	info.bGuard = true;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(-1.0f, 0.5f);
	info.forceDir.Normalize();
	info.force = 20.0f;
	info.forceTime = 0.35f;
	info.getSP = 5;
	info.sp = 0;
	info.damage = 6;
	info.animName = "Donkeykick";
	info.soundName = "Donkeykick";

	m_mapMoveSetInfo[EKyokoMoveSet::GUARD_HEAVY_DONKEYKICK] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::NONE | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::WEAKATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::HIT;
	info.getSP = 2;
	info.sp = 0;
	info.damage = 4;
	info.animName = "Gutpunch";
	info.soundName = "Punch";

	m_mapMoveSetInfo[EKyokoMoveSet::GRAB_GUTPUNCH] = info;

	info = {};

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::NONE | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(-0.5f, 1.0f);
	info.forceDir.Normalize();
	info.forceTime = 0.5f;
	info.force = 10.0f;
	info.getSP = 5;
	info.sp = 0;
	info.damage = 12;
	info.animName = "Kicktoss";
	info.soundName = "Kicktoss";

	m_mapMoveSetInfo[EKyokoMoveSet::GRAB_KICKTOSS] = info;

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::NONE | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::SPECIAL;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(-0.01f, 0.005f);
	info.forceDir.Normalize();
	info.forceTime = 0.5f;
	info.force = 10.0f;
	info.sp = 15;
	info.getSP = 0;
	info.damage = 14;
	info.animName = "Pogokick";
	info.soundName = "Pogokick";

	m_mapMoveSetInfo[EKyokoMoveSet::GRAB_POGOKICK] = info;

	info.bAir = false;
	info.bCancel = false;
	info.moveFlag = (int)EMoveDirection::NONE | (int)EMoveDirection::LEFT | (int)EMoveDirection::RIGHT |
		(int)EMoveDirection::UP | (int)EMoveDirection::DOWN;
	info.bGuard = false;
	info.keyType = (int)EPlayerKeySet::HEAVYATTACK;
	info.hitFlag = (int)EHitFlag::NONE;
	info.attackType = EAttackType::KNOCKDOWN;
	info.forceDir = Vector2(-0.7f, 1.0f);
	info.forceDir.Normalize();
	info.forceTime = 0.5f;
	info.force = 10.0f;
	info.getSP = 10;
	info.sp = 0;
	info.damage = 16;
	info.animName = "Franken";
	info.soundName = "Franken";

	m_mapMoveSetInfo[EKyokoMoveSet::GRAB_FRAKENSTEINER] = info;
}

void CKyoko::WeakAttack(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) ||
		CheckState(EKyokoState::STUNNED) ||
		CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED))
		return;

	if (m_Dodge)
		return;

	if (!m_AbleToAttack)
		return;

	m_CurrentAttackInputFlag |= (int)EPlayerKeySet::WEAKATTACK;

	if (m_LastAttackInputTime == 0.0f)
		m_LastAttackInputTime = m_CurrentTime;
}

void CKyoko::HeavyAttack(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) ||
		CheckState(EKyokoState::STUNNED) ||
		CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED))
		return;

	if (m_Dodge)
		return;

	if (!m_AbleToAttack)
		return;

	bool groundAttack = false;

	if (!m_ContactEnemies.empty() && m_OnGround)
	{
		auto iter = m_ContactEnemies.begin();
		auto iterEnd = m_ContactEnemies.end();

		if (!CheckState(EKyokoState::GRABBING))
		{
			for (; iter != iterEnd; iter++)
			{
				CRCGEnemy* enemy = (CRCGEnemy*)(*iter)->GetGameObject();

				if (enemy->IsInvincible() ||
					enemy->CheckState(EEnemyState::DEFEADTED) ||
					enemy->GetCurrentHP() <= 0)
					continue;

				if (enemy->CheckState(EEnemyState::DOWN))
				{
					groundAttack = true;
				}
			}
		}
	}

	if (groundAttack && m_CurrentMoveFlag & (int)EMoveDirection::NONE)
	{
		m_CurrentMove = EKyokoMoveSet::GROUND_HEAVY_STOMP;
		m_Sprite->ChangeAnimation("Stomp");
		CResourceManager::GetInst()->SoundPlay("Stomp");
		PushState(EKyokoState::ATTACK);
		m_CurrentAttackInputFlag = 0;
		m_LastAttackInputTime = 0.0f;
		return;
	}

	m_CurrentAttackInputFlag |= (int)EPlayerKeySet::HEAVYATTACK;

	if (m_LastAttackInputTime == 0.0f)
		m_LastAttackInputTime = m_CurrentTime;
}

void CKyoko::SpecialAttack(float deltaTime)
{
	if (CheckState(EKyokoState::DOWN) ||
		CheckState(EKyokoState::STUNNED) ||
		CheckState(EKyokoState::GETHIT) ||
		CheckState(EKyokoState::GRABBED))
		return;

	if (m_Dodge)
		return;

	if (!m_AbleToAttack)
		return;

	m_CurrentAttackInputFlag |= (int)EPlayerKeySet::SPECIAL;

	if (m_LastAttackInputTime == 0.0f)
		m_LastAttackInputTime = m_CurrentTime;
}

void CKyoko::Action()
{
	std::vector<EKyokoMoveSet> availableMoves = m_mapChainMoveSets[m_CurrentMove];

	size_t chainSize = availableMoves.size();

	for (size_t i = 0; i < chainSize; i++)
	{
		EKyokoMoveSet candidateMove = availableMoves[i];

		sPlayerMove moveSetInfo = m_mapMoveSetInfo[candidateMove];

		if (m_SpecialGauge < moveSetInfo.sp)
			continue;

		if (CheckState(EKyokoState::ATTACK))
		{
			if (!moveSetInfo.bCancel)
				continue;

			if (moveSetInfo.bAir == (!m_OnGround) &&
				moveSetInfo.bGuard == m_Guard && 
				moveSetInfo.hitFlag & m_HitEnemyFlag)
			{
				if (moveSetInfo.keyType == m_CurrentAttackInputFlag)
				{
					if (m_CurrentMoveFlag & moveSetInfo.moveFlag)
					{
						m_CurrentMove = candidateMove;
						m_Sprite->ChangeAnimation(moveSetInfo.animName);
						DeactivateHitBox();
						PushState(EKyokoState::ATTACK);
						m_CurrentAttackInputFlag = 0;
						m_LastAttackInputTime = 0.0f;
						m_SpecialGauge -= moveSetInfo.sp;
						CResourceManager::GetInst()->SoundPlay(moveSetInfo.soundName);
						m_MainWidget->SetSpecialPercent(m_SpecialGauge / 100.0f);


						if (m_CurrentMove == EKyokoMoveSet::SPECIAL_DRAGONFEET)
						{
							int randomSoundIndex = (rand() % 5) + 1;

							std::string name = "KyokoCombo" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}
						else if (m_CurrentMove == EKyokoMoveSet::SPECIAL_HURRICANEKICK)
						{
							int randomSoundIndex = (rand() % 3) + 1;

							std::string name = "KyokoEffort_buildup" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}
						else
						{
							int randomSoundIndex = (rand() % 20) + 1;

							std::string name = "KyokoEffort" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}

						return;
					}
				}
			}
		}
		else
		{
			if (moveSetInfo.bAir == (!m_OnGround) &&
				moveSetInfo.bGuard == m_Guard &&
				moveSetInfo.hitFlag & m_HitEnemyFlag)
			{
				if (moveSetInfo.keyType == m_CurrentAttackInputFlag)
				{
					if (m_CurrentMoveFlag & moveSetInfo.moveFlag)
					{
						m_CurrentMove = candidateMove;
						m_Sprite->ChangeAnimation(moveSetInfo.animName);
						DeactivateHitBox();
						PushState(EKyokoState::ATTACK);
						m_CurrentAttackInputFlag = 0;
						m_LastAttackInputTime = 0.0f;
						m_SpecialGauge -= moveSetInfo.sp;
						CResourceManager::GetInst()->SoundPlay(moveSetInfo.soundName);
						m_MainWidget->SetSpecialPercent(m_SpecialGauge / 100.0f);

						if (m_CurrentMove == EKyokoMoveSet::SPECIAL_DRAGONFEET)
						{
							int randomSoundIndex = (rand() % 5) + 1;

							std::string name = "KyokoCombo" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}
						else if (m_CurrentMove == EKyokoMoveSet::SPECIAL_HURRICANEKICK)
						{
							int randomSoundIndex = (rand() % 3) + 1;

							std::string name = "KyokoEffort_buildup" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}
						else
						{
							int randomSoundIndex = (rand() % 20) + 1;

							std::string name = "KyokoEffort" + std::to_string(randomSoundIndex);

							CResourceManager::GetInst()->SoundPlay(name);
						}
						return;
					}
				}
			}
		}
	}
}

void CKyoko::ThrowEnemy()
{
	sPlayerMove info = m_mapMoveSetInfo[m_CurrentMove];
	m_Invincible = true;
	((CRCGEnemy*)m_Enemy)->PopStateEnd(EEnemyState::STUNNED);
	((CRCGEnemy*)m_Enemy)->PopStateEnd(EEnemyState::GRABBED);
	CResourceManager::GetInst()->SoundPlay(info.soundName);
	((CRCGEnemy*)m_Enemy)->GetHit(info.attackType, info.forceDir, info.damage, info.force, info.forceTime, m_FacingRight);
}

void CKyoko::HitBoxUpdate()
{
	Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
	m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());
	switch (m_CurrentMove)
	{
	case EKyokoMoveSet::GROUND_HEAVY_DAB:
	{	
		m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());
		m_HitBox->SetExtent(size.x * 1.5f, size.y);
	}
	break;
	case EKyokoMoveSet::GUARD_HEAVY_DONKEYKICK:
	{
		if (m_FacingRight)
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
		}
		else
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
		}
	}
	break;
	case EKyokoMoveSet::SPECIAL_ACROCIRCUS:
		m_HitBox->SetExtent(size.x, size.y);
		m_HitBox->SetOffset(0.0f, 0.0f, 0.0f);
		break;
	case EKyokoMoveSet::SPECIAL_HURRICANEKICK:
		m_HitBox->SetExtent(size.x, size.y);
		break;
	case EKyokoMoveSet::AIR_HEAVY_AIRSTEP:
		m_HitBox->SetExtent(size.x, size.y);
		m_HitBox->SetOffset(0.0f, -size.y * 0.5f, 0.0f);
		break;
	case EKyokoMoveSet::GROUND_HEAVY_VOLLEYSET:
		if (m_FacingRight)
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
		}
		else
		{
			m_HitBox->SetExtent(size.x * 0.5f, size.y);
			m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
		}
	default:
	{
		if (m_FacingRight)
		{
			m_HitBox->SetExtent(size.x / 2.0f, size.y);
			m_HitBox->SetOffset(size.x / 2.0f, 0.0f, 0.0f);
		}
		else
		{
			m_HitBox->SetExtent(size.x / 2.0f, size.y);
			m_HitBox->SetOffset(-size.x / 2.0f, 0.0f, 0.0f);
		}
	}
	break;
	}
}


void CKyoko::HitBoxCheck(const sCollisionResult& result)
{
	if (!m_HitBox->IsEnable())
		return;

	if (result.dest->GetCollisionProfile()->name == "Enemy")
	{
		std::unordered_map<CGameObject*, int> collisions;
		{
			auto iter = m_Body->GetPrevColliderList().cbegin();
			auto iterEnd = m_Body->GetPrevColliderList().cend();


			for (; iter != iterEnd; iter++)
			{
				if ((*iter)->GetCollisionProfile()->name == "Enemy")
				{
					collisions[(*iter)->GetGameObject()]++;
				}
			}
		}

		{
			auto iter = m_Bottom->GetPrevColliderList().cbegin();
			auto iterEnd = m_Bottom->GetPrevColliderList().cend();

			for (; iter != iterEnd; iter++)
			{
				if ((*iter)->GetCollisionProfile()->name == "EnemyBottom")
				{
					collisions[(*iter)->GetGameObject()]++;
				}
			}
		}



		if (!collisions.empty())
		{
			if (m_mapMoveSetInfo.count(m_CurrentMove))
			{

				sPlayerMove& moveSet = m_mapMoveSetInfo[m_CurrentMove];

				auto iter = collisions.begin();
				auto iterEnd = collisions.end();
				
				for (; iter != iterEnd; iter++)
				{
					bool right;

					if (GetWorldPos().x < (*iter).first->GetWorldPos().x)
						right = true;
					else
						right = false;

					if (m_CurrentMove == EKyokoMoveSet::GUARD_HEAVY_DONKEYKICK)
						right = !right;

					CRCGEnemy* enemy = (CRCGEnemy*)(*iter).first;

					if (enemy->IsInvincible())
						continue;

					if ((*iter).second < 2)
						continue;

					if (enemy->CheckState(EEnemyState::DEFEADTED))
						continue;

					if (enemy->CheckState(EEnemyState::DOWN))
						continue;

					if (enemy->GetCurrentHP() <= 0)
						continue;

					if (enemy->OnGuard())
					{
						enemy->Block();
						CResourceManager::GetInst()->SoundPlay("Block");
					}
					else
					{
						//CHitEffect* effect = m_Scene->CreateGameObject<CHitEffect>("Effect");
						//effect->SetTintColor(Vector4(1.0f, 1.0f, 0.0f, 0.5f));
						//effect->SetOpacity(0.5f);
						//effect->SetWorldPos(Vector3(m_HitBox->GetInfo().center.x, m_HitBox->GetInfo().center.y, 0.0f));

						if (m_CurrentMove == EKyokoMoveSet::SPECIAL_DRAGONFEET && m_DragonFeetBlowback)
						{
							Vector2 dir = Vector2(1.0f, 0.2f);
							dir.Normalize();
							float force = 20.0f;
							float forceTime = 0.35f;

							enemy->GetHit(EAttackType::BLOWNBACK, dir, moveSet.damage * 2, force, forceTime, right);
						}
						else
						{
							if (m_CurrentMove == EKyokoMoveSet::AIR_HEAVY_AIRSTEP)
							{
								m_AirBoostTime = m_CurrentTime;
								m_CurrentMove = EKyokoMoveSet::AIR_DEFAULT;
								m_Velocity.y = 0.0f;
								m_Sprite->ChangeAnimation("Jump");
								PopStateEnd(EKyokoState::ATTACK);
								m_AirBoostJump = true;
							}
							else if (m_CurrentMove == EKyokoMoveSet::DASH_WEAK_BACK_ELBOW)
							{
								m_Push = false;
								m_Velocity = Vector2(0.0f, 0.0f);
								m_AbsVel = Vector2(0.0f, 0.0f);
							}

							enemy->GetHit(moveSet.attackType, moveSet.forceDir, moveSet.damage, moveSet.force,
								moveSet.forceTime, right);
						}

						m_SpecialGauge += moveSet.getSP;
						if (m_SpecialGauge > 100)
							m_SpecialGauge = 100;

						m_MainWidget->SetSpecialPercent(m_SpecialGauge / 100.0f);
					}

					CResourceManager::GetInst()->SoundPlay("Knockdown");
						
					m_HitEnemyFlag |= (int)EHitFlag::HIT;
				}

			}
		}
	}
	else
		m_HitEnemyFlag &= ~(int)EHitFlag::HIT;

}

void CKyoko::ApplyUpwardForce()
{
	m_Upward = true;
	m_Velocity = Vector2(m_Velocity.x, 0.0f);
	m_AbsVel = Vector2(m_AbsVel.x, 0.0f);
}


void CKyoko::StartAcrocircus()
{
	m_Sprite->ChangeAnimation("Acrocircus_loop");
	m_Upward = true;
	m_Push = true;
	m_Bounce = false;
	m_Velocity.y = 0.005f;
	m_OnGround = false;
	m_AbleToMove = false;
	m_Velocity = Vector2(0.0f, 0.0f);
	m_AbsVel = Vector2(0.0f, 0.0f);
	PushState(EKyokoState::ATTACK);

	int randomSoundIndex = (rand() % 3) + 1;

	std::string name = "KyokoEffort_buildup" + std::to_string(randomSoundIndex);

	CResourceManager::GetInst()->SoundPlay(name);

	m_UpwardForce = 70.0f;

	m_AfterImage->SetEffect(true);
	m_AfterImage->SetAnimation("Acrocircus_loop");
	m_AfterImage->SetTintColor(Vector4(0.0f, 0.0f, 0.8f, 0.5f));
	m_AfterImage->SetOffsetMode(false);
	if (m_FacingRight)
		m_AfterImage->FlipImage(false);
	else
		m_AfterImage->FlipImage(true);
	m_AfterImage->Prepare();
	m_AfterImageWithInterval = true;

	m_AfterImageInterval = 0.05f;
	m_Interval = 0.0f;

	ApplyPush();
}

void CKyoko::Guard(float deltaTime)
{
	if (m_Dodge || !m_OnGround || m_Jump || !m_AbleToMove)
		return;

	if (m_CurrentState != (int)EKyokoState::NORMAL)
		return;

	m_GuardStartTime = CEngine::GetInst()->GetCurrentPlayTime();

	m_Guard = true;

	m_IsRunning = false;
	m_IsMovingUp = false;
	m_IsMovingDown = false;
	m_IsMovingLeft = false;
	m_IsMovingRight = false;

	m_CurrentMove = EKyokoMoveSet::GUARD_DEFAULT;

	m_Sprite->ChangeAnimation("Block");
}

void CKyoko::GuardEnd(float deltaTime)
{
	m_Guard = false;
}

void CKyoko::EndUpwardForce()
{
	if (m_Upward)
	{
		m_Upward = false;
		m_UpwardForce = 0.0f;
		m_Velocity = Vector2(m_Velocity.x, 0.0f);
		m_AbsVel = Vector2(m_AbsVel.x, 0.0f);
	}
}

void CKyoko::GroundAttackEndCallback()
{
	PopStateEnd(EKyokoState::ATTACK);
	m_CurrentAttackInputFlag = 0;
	m_LastAttackInputTime = 0.0f;
	DeactivateHitBox();
	m_AbleToMove = true;
	m_PushForce = 0.0f;
	m_HitEnemyFlag &= ~(int)EHitFlag::HIT;

	if (m_AfterImagePrevFrame)
		m_AfterImagePrevFrame = false;
}

void CKyoko::ApplyPush()
{
	m_Push = true;
	m_Velocity = Vector2(0.0f, m_Velocity.y);

	switch (m_CurrentMove)
	{
	case EKyokoMoveSet::GROUND_HEAVY_AXEKICK:
	case EKyokoMoveSet::GROUND_HEAVY_SPINKICK:
	case EKyokoMoveSet::DASH_WEAK_BACK_ELBOW:
		if (m_FacingRight)
		{
			m_PushForce = 48.0f;
		}
		else
		{
			m_PushForce = -48.0f;
		}
		break;
	case EKyokoMoveSet::DASH_HEAVY_DIVE:
		if (m_FacingRight)
		{
			m_PushForce = 60.0f;
		}
		else
		{
			m_PushForce = -60.0f;
		}
		break;
	case EKyokoMoveSet::DASH_SPECIAL_DROPKICK:
		if (m_FacingRight)
		{
			m_PushForce = 100.0f;
		}
		else
		{
			m_PushForce = -100.0f;
		}
		break;
	case EKyokoMoveSet::SPECIAL_ACROCIRCUS:
		if (m_FacingRight)
		{
			m_PushForce = 14.0f;
		}
		else
		{
			m_PushForce = -14.0f;
		}
		break;
	case EKyokoMoveSet::SPECIAL_CHEERDRILL:
		if (m_FacingRight)
		{
			m_PushForce = 30.0f;
		}
		else
		{
			m_PushForce = -30.0f;
		}
		break;
	case EKyokoMoveSet::SPECIAL_HURRICANEKICK:
		if (m_FacingRight)
		{
			m_PushForce = 2.4f;
		}
		else
		{
			m_PushForce = -2.4f;
		}
		break;
	}
}

void CKyoko::KickToss()
{
	if (CheckState(EKyokoState::GRABBING) && m_Enemy)
	{
		ThrowEnemy();
	}
}

void CKyoko::KickTossEnd()
{
	PopStateEnd(EKyokoState::ATTACK);
	PopStateEnd(EKyokoState::GRABBING);
	ResetSpriteOffset();
	m_Enemy = nullptr;
	m_Invincible = false;
	m_AbleToMove = true;
}

void CKyoko::KickTossBegin()
{
	((CRCGEnemy*)m_Enemy)->PushState(EEnemyState::GRABBED, m_CurrentTime + 50.0f);
	PushState(EKyokoState::ATTACK);
	KickTossOffset();
	m_Invincible = true;
	m_AbleToMove = false;
}

void CKyoko::FrankenBegin()
{
	((CRCGEnemy*)m_Enemy)->PushState(EEnemyState::GRABBED, m_CurrentTime + 50.0f);
	PushState(EKyokoState::ATTACK);
	FrankenOffset();
	m_Invincible = true;
	m_AbleToMove = false;
}


void CKyoko::Franken()
{
	if (CheckState(EKyokoState::GRABBING) && m_Enemy)
	{
		ThrowEnemy();
	}
}

void CKyoko::FrankenOffset()
{
	m_Offset = Vector3(0.0f, 50.0f, 0.0f);

	if (m_Enemy)
	{
		if (m_FacingRight)
			m_Offset.x = 50.0f;
		else
			m_Offset.x = -50.0f;
	}

	Vector3 offset = m_Sprite->GetWorldPos() + m_Offset;
	m_Sprite->SetWorldPos(offset);
}

void CKyoko::FrankenEnd()
{
	PopStateEnd(EKyokoState::ATTACK);
	PopStateEnd(EKyokoState::GRABBING);
	m_Enemy = nullptr;
	m_Invincible = false;
	m_AbleToMove = true;
}

void CKyoko::PogoKickBegin()
{
	((CRCGEnemy*)m_Enemy)->PushState(EEnemyState::GRABBED, m_CurrentTime + 50.0f);
	PushState(EKyokoState::ATTACK);
	m_Invincible = true;
	m_AbleToMove = false;
}

void CKyoko::PogoKickOffset()
{
	m_Offset = Vector3(0.0f, 60.0f, 0.0f);

	if (m_Enemy)
	{
		if (m_FacingRight)
			m_Offset.x = 50.0f;
		else
			m_Offset.x = -50.0f;
	}

	Vector3 offset = m_Sprite->GetWorldPos() + m_Offset;
	m_Sprite->SetWorldPos(offset);
}

void CKyoko::PogoKick()
{
	m_OnGround = false;

	m_Velocity = Vector2(0.0f, 0.0f);
	m_AbsVel = Vector2(0.0f, 0.0f);

	m_Velocity.y = 0.001f;

	if (m_FacingRight)
		m_KnockbackForce = Vector2(14.0f, 40.0f);
	else
		m_KnockbackForce = Vector2(-14.0f, 40.0f);

	PushState(EKyokoState::POGOKICK, m_CurrentTime + 0.25f);

	m_Physics = true;

	if (CheckState(EKyokoState::GRABBING) && m_Enemy)
	{
		ThrowEnemy();
	}
}

void CKyoko::PogoKickEnd()
{
	PopStateEnd(EKyokoState::ATTACK);
	PopStateEnd(EKyokoState::GRABBING);
	ResetSpriteOffset();
	m_Sprite->ChangeAnimation("Jump");
	m_Enemy = nullptr;
	m_AbleToMove = true;
	m_Invincible = false;
}

void CKyoko::KickTossOffset()
{
	m_Offset = Vector3(0.0f, 0.0f, 0.0f);

	if (m_Enemy)
	{
		if (m_FacingRight)
			m_Offset.x = 10.0f;
		else
			m_Offset.x = -10.0f;
	}

	Vector3 offset = m_Sprite->GetWorldPos() + m_Offset;
	m_Sprite->SetWorldPos(offset);
}

void CKyoko::AirAttackEndCallback()
{
	m_CurrentAttackInputFlag = 0;
	m_LastAttackInputTime = 0.0f;
	DeactivateHitBox();
}

void CKyoko::AirCheerDrillEndCallback()
{
	m_CurrentAttackInputFlag = 0;
	PopStateEnd(EKyokoState::ATTACK);
	m_Sprite->ChangeAnimation("Jump");
	m_LastAttackInputTime = 0.0f;
	m_GravityEnable = true;
	m_Push = false;
	m_AbleToMove = true;
	DeactivateHitBox();
}

void CKyoko::OnEnemyContactExit(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
	{
		CColliderComponent* comp = result.dest;

		auto iter = std::find(m_ContactEnemies.begin(), m_ContactEnemies.end(), comp);

		if (iter != m_ContactEnemies.end())
			return;

		m_ContactEnemies.erase(iter);
	}
}

void CKyoko::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
	m_Push = false;
	m_Upward = false;
		
	m_Dab = false;
	m_AfterImageInterval = false;
	m_AfterImagePrevFrame = false;

	if (m_Offset.y != 0.0f)
	{
		ResetSpriteOffset();
		m_Offset = Vector3(0.0f, 0.0f, 0.0f);
	}

	if (CheckState(EKyokoState::GRABBING))
	{
		if (m_Enemy)
		{
			CRCGEnemy* enemy = (CRCGEnemy*)m_Enemy;
			enemy->SetPlayer(nullptr);
			enemy->PopStateEnd(EEnemyState::GRABBED);
			enemy->GetSpriteComponent()->ChangeAnimation("Idle");
			PopStateEnd(EKyokoState::GRABBING);
			m_CurrentMove = EKyokoMoveSet::GROUND_DEFAULT_STATE;
			m_AbleToAttack = true;
			m_Enemy = nullptr;
		}
	}

	switch (type)
	{
	case EAttackType::HIT:
	{
		int randomSoundIndex = (rand() % 2) + 1;

		std::string name = "KyokoGethit_light" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}
		break;
	case EAttackType::KNOCKDOWN:
	{
		int randomSoundIndex = (rand() % 7) + 1;

		std::string name = "KyokoGethit" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}
		break;
	case EAttackType::BLOWNBACK:
	{
		int randomSoundIndex = (rand() % 2) + 1;

		std::string name = "KyokoGethit_pain" + std::to_string(randomSoundIndex);

		CResourceManager::GetInst()->SoundPlay(name);
	}
		break;
	}

	CRCGPlayer::GetHit(type, dir, damage, force, forceTime, right);
}

void CKyoko::AirCheerDrillBeginCallback()
{
	m_GravityEnable = false;
	m_Push = true;
	m_Upward = false;
	if (m_FacingRight)
		m_PushForce = 30.0f;
	else
		m_PushForce = -30.0f;
	m_Velocity = Vector2(0.0f, 0.0f);
	m_AbsVel = Vector2(0.0f, 0.0f);
}

void CKyoko::OnGround(const sCollisionResult& result)
{
	if (result.dest->GetGameObject() == this &&
		m_Velocity.y < 0.0f)
	{
		m_OnGround = true;
		m_Physics = false;
		m_WallJump = false;
		m_Jump = false;
		m_JumpSpeed = 70.0f;

		if (m_Offset.y != 0.0f)
		{
			ResetSpriteOffset();
			m_Offset = Vector3(0.0f, 0.0f, 0.0f);
		}
		
		if (m_BattleStart)
		{
			m_Sprite->ChangeAnimation("BattleStartGround");
			CResourceManager::GetInst()->SoundPlay("Battlestart");
			m_MainWidget->EnableBattleIntro();
			m_MainWidget->BattleIntroStart();
			m_ContactBox->Enable(true);
		}

		if (m_CurrentMove == EKyokoMoveSet::SPECIAL_ACROCIRCUS)
		{
			if(!m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Acrocircus_land"))
				m_Sprite->ChangeAnimation("Acrocircus_land");
			m_Upward = false;
			if (!m_Bounce)
			{
				m_Bounce = true;
				m_BounceCount = 0;
			}
			m_AbsVel = Vector2(0.0f, 0.0f);
		}
		else if (m_CurrentMove == EKyokoMoveSet::AIR_SPECIAL_CHEERDRILL)
		{
			m_Push = false;
			m_Upward = false;
		}
		else if (CheckState(EKyokoState::GETHIT))
		{
			m_Sprite->ChangeAnimation("Knockdown_ground");
			PopStateEnd(EKyokoState::ATTACK);
			DeactivateHitBox();
			m_HitCount = 0;
			PushState(EKyokoState::DOWN, m_CurrentTime + 3.0f);
		}
		else if (m_CurrentMove != EKyokoMoveSet::DASH_SPECIAL_DROPKICK && m_CurrentMove != EKyokoMoveSet::DASH_HEAVY_DIVE)
		{
			if (CheckState(EKyokoState::ATTACK))
				PopStateEnd(EKyokoState::ATTACK);

			CResourceManager::GetInst()->SoundPlay("Land");
		}

		if (m_CurrentMove == EKyokoMoveSet::DASH_HEAVY_DIVE)
		{
			CResourceManager::GetInst()->SoundPlay("Diveslide");
			m_PushForce *= 0.5f;
		}

		m_CurrentAttackInputFlag = 0;
		m_LastAttackInputTime = 0.0f;

		if (m_Bounce && m_BounceCount < 1)
		{
			m_OnGround = false;
			m_PushForce *= 0.25f;
			m_Velocity *= -0.5f;
			m_BounceCount++;
		}
		else
		{
			m_Velocity = Vector2(0.0f, 0.0f);
			m_AbsVel = Vector2(0.0f, 0.0f);
			m_AfterImageWithInterval = false;
			m_AfterImage->SetEffect(false);
			m_AfterImageInterval = 0.0f;
		}

		//CSmokeParticle* smokeParticle = m_Scene->CreateGameObject<CSmokeParticle>("SmokeParticle");
		//Vector3 pos = Vector3(m_Transform->GetWorldPos().x - m_Sprite->GetWorldScale().x / 2.0f,
		//    m_Transform->GetWorldPos().y - m_Sprite->GetWorldScale().y / 2.0f, 0.0f);
		//smokeParticle->SetRelativePos(pos);
		//smokeParticle->SetLifeSpan(0.5f);
	}
}

void CKyoko::Stomp()
{
	sPlayerMove move = m_mapMoveSetInfo[m_CurrentMove];

	for (const auto& collider : m_ContactEnemies)
	{
		CRCGEnemy* enemy = (CRCGEnemy*)collider->GetGameObject();

		if (enemy->CheckState(EEnemyState::DOWN))
		{

			if (enemy->CheckState(EEnemyState::DEFEADTED) ||
				enemy->GetCurrentHP() <= 0)
				continue;
			bool right;

			if (GetWorldPos().x > enemy->GetWorldPos().x)
				right = true;
			else
				right = false;

			enemy->GetHit(move.attackType, move.forceDir, move.damage, move.force, move.forceTime, right);
		}
	}
}

void CKyoko::DabAfterImage()
{
	m_Dab = true;
	if(m_FacingRight)
		m_DabOffset = Vector3(-10.0f, 0.0f, 0.0f);
	else
		m_DabOffset = Vector3(10.0f, 0.0f, 0.0f);

	m_AfterImage->SetEffect(true);
	m_AfterImage->Prepare();
	m_AfterImage->SetAnimation("Dab");
	m_AfterImage->SetTintColor(Vector4(0.5f, 0.5f, 1.0f, 1.0f));
	m_AfterImage->SetOffset(m_DabOffset);
	m_AfterImage->SetOffsetMode(true);

	if (m_FacingRight)
		m_AfterImage->FlipImage(false);
	else
		m_AfterImage->FlipImage(true);
}

void CKyoko::DabAfterImageEnd()
{
	m_Dab = false;
	m_AfterImage->SetEffect(false);
}

void CKyoko::OnEnemyContact(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
	{
		CColliderComponent* comp = result.dest;

		auto iter = std::find(m_ContactEnemies.begin(), m_ContactEnemies.end(), comp);

		if (iter != m_ContactEnemies.end())
			return;

		m_ContactEnemies.push_back(comp);
	}

	if (m_Invincible)
		return;

	if (CheckState(EKyokoState::INVINCIBLE))
		return;

	if (m_OnGround && m_CurrentState == (int)EKyokoState::NORMAL &&
		!m_ContactEnemies.empty())
	{
		m_ContactEnemies.sort([&](const CColliderComponent* src, const CColliderComponent* dest) {
			return m_Transform->GetWorldPos().SqrDistance(src->GetGameObject()->GetWorldPos())
				< m_Transform->GetWorldPos().SqrDistance(dest->GetGameObject()->GetWorldPos());
		});

		for (const auto& collider : m_ContactEnemies)
		{
			CRCGEnemy* enemy = (CRCGEnemy*)collider->GetGameObject();

			if (enemy->CheckState(EEnemyState::STUNNED) && 
				enemy->IsGrabbable() &&
				fabs(m_Transform->GetWorldPos().y - enemy->GetWorldPos().y) <= 10.0f)
			{
				enemy->SetPlayer(this);
				m_Enemy = enemy;
				enemy->PopStateEnd(EEnemyState::STUNNED);
				enemy->PushState(EEnemyState::GRABBED, m_CurrentTime + 3.0f);

				if (enemy->IsFacingRight() ^ m_FacingRight)
					m_CurrentMove = EKyokoMoveSet::GRAB_FRONT;
				else
					m_CurrentMove = EKyokoMoveSet::GRAB_BEHIND;

				((CRCGEnemy*)m_Enemy)->Grabbed();

				PushState(EKyokoState::GRABBING);

				m_Sprite->GetAnimationInstance()->ChangeAnimation("Grabbing");
				CResourceManager::GetInst()->SoundPlay("Grab_enemy");

				break;
			}
		}
	}
}

void CKyoko::DragonFeetAfterImage()
{
	m_AfterImage->SetEffect(true);
	m_AfterImage->SetAnimation("Dragonfeet");
	m_AfterImage->SetTintColor(Vector4(0.0f, 0.0f, 0.8f, 0.5f));
	m_AfterImage->SetOffsetMode(false);
	m_AfterImagePrevFrame = true;
	m_AfterImage->Prepare();

	if (m_FacingRight)
		m_AfterImage->FlipImage(false);
	else
		m_AfterImage->FlipImage(true);
}

void CKyoko::DropKickBegin()
{
	m_OnGround = false;
	m_AbleToMove = false;
	Vector3 offset = m_Sprite->GetWorldPos() + Vector3(0.0f, 50.0f, 0.0f);
	m_Velocity.y = 0.5f;
	m_Velocity.x = 0.0f;
	m_AbsVel = Vector2(0.0f, 0.0f);
	m_Sprite->SetWorldPos(offset);
	m_Body->SetWorldPos(offset);
}

void CKyoko::DiveBegin()
{
	m_OnGround = false;
	m_AbleToMove = false;
	Vector3 offset = m_Sprite->GetWorldPos() + Vector3(0.0f, 50.0f, 0.0f);
	m_Velocity.y = 0.5f;
	m_Velocity.x = 0.0f;
	m_AbsVel = Vector2(0.0f, 0.0f);
	m_Sprite->SetWorldPos(offset);
	m_Body->SetWorldPos(offset);
}

void CKyoko::SpriteOffset()
{
	m_Offset = Vector3(0.0f, 35.0f, 0.0f);
	Vector3 offset = m_Sprite->GetWorldPos() + m_Offset;
	m_Sprite->SetWorldPos(offset);
}

void CKyoko::ResetSpriteOffset()
{
	m_Sprite->SetWorldPos(m_Sprite->GetWorldPos() - m_Offset);
	m_Offset = Vector3(0.0f, 0.0f, 0.0f);
}

void CKyoko::GutPunch()
{
	PushState(EKyokoState::ATTACK);
	int randomSoundIndex = (rand() % 3) + 1;

	std::string name = "Punch" + std::to_string(randomSoundIndex);

	CResourceManager::GetInst()->SoundPlay(name);

	CRCGEnemy* enemy = (CRCGEnemy*)m_Enemy;

	enemy->GetHit(EAttackType::HIT, Vector2(0.0f, 0.0f), 4, 0.0f, 0.0f, false);

	if (enemy->GetCurrentHP() <= 0)
	{
		PopStateEnd(EKyokoState::GRABBING);
		enemy->SetPlayer(nullptr);
		enemy->PopStateEnd(EEnemyState::GRABBED);
		m_CurrentMove = EKyokoMoveSet::GROUND_DEFAULT_STATE;
		m_Sprite->ChangeAnimation("Idle");
		m_AbleToAttack = true;
		m_Enemy = nullptr;
	}
}

void CKyoko::GutPunchEnd()
{
	m_Sprite->ChangeAnimation("Grabbing");
	PopStateEnd(EKyokoState::ATTACK);

	if (m_Enemy)
	{
		if (m_FacingRight ^ ((CRCGEnemy*)m_Enemy)->IsFacingRight())
			m_CurrentMove = EKyokoMoveSet::GRAB_FRONT;
		else
			m_CurrentMove = EKyokoMoveSet::GRAB_BEHIND;
	}
}

void CKyoko::EndPush()
{
	m_Push = false;
	m_Velocity = Vector2(0.0f, m_Velocity.y);
	m_AbsVel = Vector2(0.0f, m_AbsVel.y);
}
