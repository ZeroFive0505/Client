#include "MT.h"
#include "MTAnimation.h"
#include "../Widget/RCGEnemyHUD.h"
#include "MTIdle.h"
#include "MTPursue.h"

CMT::CMT() :
    m_State(nullptr),
    m_NextState(nullptr)
{
}

CMT::CMT(const CMT& obj) :
    CRCGEnemy(obj)
{
}

CMT::~CMT()
{
    SAFE_DELETE(m_State);
    SAFE_DELETE(m_NextState);
}

bool CMT::Init()
{
    if (!CRCGEnemy::Init())
        return false;

    m_Sprite->CreateAnimationInstance<CMTAnimation>();

    m_Bottom->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnGround);

    m_NavAgent->SetMoveSpeed(m_WalkSpeed);

    SetMoveSetInfo();

    m_HitBox->AddCollisionCallback<CMT>(Collision_State::Begin, this, &CMT::HitBoxCheck);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Crosspunch", this, &CMT::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Jumpbackkick", this, &CMT::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Dodgekick", this, &CMT::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Jumpkick", this, &CMT::JumpKickEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Snapkick", this, &CMT::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CMT>("Alert", this, &CMT::AlertFinish);

    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Crosspunch", "CrosspunchHitBoxOn", 2, this, &CRCGEnemy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Crosspunch", "CrosspunchHitBoxOff", 3, this, &CRCGEnemy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Dodgekick", "DodgeKickHitBoxOn", 10, this, &CRCGEnemy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Dodgekick", "DodgeKickHitBoxOff", 13, this, &CRCGEnemy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Jumpbackkick", "JumpBackKickHitBoxOn", 2, this, &CRCGEnemy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Jumpbackkick", "JumpBackKickHitBoxOff", 3, this, &CRCGEnemy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Jumpkick", "JumpKickHitBoxOn", 2, this, &CRCGEnemy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Jumpkick", "JumpKickHitBoxOff", 3, this, &CRCGEnemy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CMT>("Jumpbackkick", "JumpBackKickOffset", 2, this, &CMT::SpriteOffset);
    m_Sprite->GetAnimationInstance()->AddNotify<CMT>("Jumpbackkick", "JumpBackKickResetOffset", 5, this, &CMT::ResetOffset);

    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Snapkick", "SnapKickHitBoxOn", 2, this, &CRCGEnemy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CRCGEnemy>("Snapkick", "SnapKickHitBoxOff", 3, this, &CRCGEnemy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CMT>("Dodgekick", this, &CMT::DodgeStart);
    m_Sprite->GetAnimationInstance()->AddNotify<CMT>("Dodgekick", "DodgeKickPush", 8, this, &CMT::DodgeKickStart);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit1", this, &CRCGEnemy::HitStatusEnd);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit2", this, &CRCGEnemy::HitStatusEnd);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Hit3", this, &CRCGEnemy::HitStatusEnd);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpStart);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Getup", this, &CRCGEnemy::GetUpComplete);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_midair", this, &CRCGEnemy::ChangeToFall);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Blownback_midair", this, &CRCGEnemy::ChangeToFall);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_success", this, &CRCGEnemy::BlockSuccess);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Block_end", this, &CRCGEnemy::GuardOff);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GetGroundHit);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Groundhit", this, &CRCGEnemy::GroundHitFinish);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Knockdown_ground", this, &CRCGEnemy::ChangeToDown);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Heldrelease", this, &CRCGEnemy::GrabbedReleaseEnd);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Heldhit", this, &CRCGEnemy::GutPunchEnd);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CRCGEnemy>("Wallhit", this, &CRCGEnemy::WallHitStart);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CRCGEnemy>("Wallhit", this, &CRCGEnemy::WallHitEnd);
    
    m_Dodge = false;
    m_Kick = false;

    m_Grabbable = true;

    m_HUDWidget = CreateComponent<CWidgetComponent>("EnemyHUD");

    m_EnemyHUD = m_HUDWidget->CreateWidgetWindow<CRCGEnemyHUD>("EnemyHUDWidget");

    m_Transform->AddChild(m_HUDWidget);

    m_HUDWidget->SetRelativePos(-22.5f, 80.0f, 0.0f);


    return true;
}

void CMT::Update(float deltaTime)
{
    CRCGEnemy::Update(deltaTime);

    if (m_Spawning)
        return;

    if (m_CurrentState == (int)EEnemyState::NORMAL && !m_Physics)
        m_StunAccumulate -= deltaTime;

    if (m_StunAccumulate <= 0.0f)
        m_StunAccumulate = 0.0f;


    if ((m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Knockdown_fall") ||
        m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Knockdown")) && m_OnGround)
    {
        m_Sprite->ChangeAnimation("Knockdown_ground");
        PopStateEnd(EEnemyState::GETHIT);
        m_HitCount = 0;
        m_Physics = false;
        m_Velocity = Vector2(0.0f, 0.0f);
        m_AbsVel = Vector2(0.0f, 0.0f);
        PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
    }


    m_FinishedState = PopStateEnd(m_CurrentTime);

    if (m_FinishedState & (int)EEnemyState::DOWN)
    {
        if (m_HealthPoint > 0)
        {
            m_Invincible = true;
            m_Sprite->ChangeAnimation("Getup");
            m_Velocity = Vector2(0.0f, 0.0f);
            m_AbsVel = Vector2(0.0f, 0.0f);
        }
        else
        {
            m_BlinkTime = 0.0f;
            m_HalfOpacity = true;
            PushState(EEnemyState::DEFEADTED, m_CurrentTime + 3.0f);
        }
    }

    if (m_FinishedState & (int)EEnemyState::KNOCKDOWN)
    {
        if (!m_OnGround)
            m_Sprite->ChangeAnimation("Knockdown_midair");
        else
        {
            m_Sprite->ChangeAnimation("Knockdown_ground");
            m_Physics = false;
            m_Velocity = Vector2(0.0f, 0.0f);
            m_AbsVel = Vector2(0.0f, 0.0f);
            PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
        }
    }

    if (m_FinishedState & (int)EEnemyState::BLOWNBACK)
    {
        if (!m_OnGround)
            m_Sprite->ChangeAnimation("Blownback_midair");
        else
        {
            m_Sprite->ChangeAnimation("Knockdown_ground");
            m_Physics = false;
            m_Velocity = Vector2(0.0f, 0.0f);
            m_AbsVel = Vector2(0.0f, 0.0f);
            PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
        }
    }

    if (m_FinishedState & (int)EEnemyState::STUNNED)
    {
        m_Sprite->ChangeAnimation("Idle");
        PopStateEnd(EEnemyState::GETHIT);
        m_Stunned = false;
    }

    if (m_FinishedState & (int)EEnemyState::GRABBED)
    {
        m_Sprite->ChangeAnimation("Heldrelease");
        if (m_Player)
        {
            ((CRCGPlayer*)m_Player)->GrabbingFinish();
            m_Player = nullptr;
        }
    }

    if (m_FinishedState & (int)EEnemyState::PARALYSIS)
    {
        PopStateEnd(EEnemyState::ATTACK);
        m_Sprite->ChangeAnimation("Idle");
    }


    if (m_FinishedState & (int)EEnemyState::DEFEADTED)
        Destroy();

    if (m_HitBox->IsEnable())
        HitBoxUpdate();

    if (m_Push)
    {
        if (m_FacingRight && !m_RightWallCollision)
        {
            if (m_Dodge)
            {
                if(!m_LeftWallCollision)
                    AddWorldPos(GetWorldAxis(AXIS_X) * -250.0f * deltaTime);
            }

            if (m_Kick)
            {
                if (!m_RightWallCollision)
                    AddWorldPos(GetWorldAxis(AXIS_X) * 600.0f * deltaTime);
            }
        }
        else if (!m_FacingRight && !m_LeftWallCollision)
        {
            if (m_Dodge)
            {
                if (!m_RightWallCollision)
                    AddWorldPos(GetWorldAxis(AXIS_X) * 250.0f * deltaTime);
            }

            if (m_Kick)
            {
                if (!m_LeftWallCollision)
                    AddWorldPos(GetWorldAxis(AXIS_X) * -600.0f * deltaTime);
            }
        }
    }

    if (CheckState(EEnemyState::DEFEADTED))
    {
        if (m_HalfOpacity)
            m_Sprite->SetOpacity(0.5f);
        else
            m_Sprite->SetOpacity(1.0f);

        m_BlinkTime += deltaTime;

        if (m_BlinkTime >= 0.25f)
            m_HalfOpacity = !m_HalfOpacity;
    }

    if (m_CurrentState != (int)EEnemyState::NORMAL)
        return;

    if (m_State)
    {
        CState* state = m_State->Process();

        if (state->GetCurrentState() != m_State->GetCurrentState())
        {
            SAFE_DELETE(m_State);
            m_State = state;
        }
    }
}

void CMT::PostUpdate(float deltaTime)
{
    CRCGEnemy::PostUpdate(deltaTime);
}

CMT* CMT::Clone()
{
    return new CMT(*this);
}

void CMT::SetMoveSetInfo()
{
    sEnemyMove info = {};

    info.attackType = EAttackType::BLOWNBACK;
    info.forceDir = Vector2(1.0f, 0.15f);
    info.forceDir.Normalize();
    info.force = 16.0f;
    info.damage = 8;
    info.forceTime = 0.4f;

    m_mapMoveset[EMTMoveSet::CROSSPUNCH] = info;

    info = {};

    info.attackType = EAttackType::BLOWNBACK;
    info.forceDir = Vector2(1.0f, 0.05f);
    info.forceDir.Normalize();
    info.force = 16.0f;
    info.damage = 12;
    info.forceTime = 0.45f;

    m_mapMoveset[EMTMoveSet::DODGEKICK] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.3f, 0.8f);
    info.forceDir.Normalize();
    info.force = 10.0f;
    info.damage = 9;
    info.forceTime = 0.4f;

    m_mapMoveset[EMTMoveSet::JUMPBACKKICK] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.3f, 0.5f);
    info.forceDir.Normalize();
    info.force = 12.0f;
    info.damage = 5;
    info.forceTime = 0.45f;

    m_mapMoveset[EMTMoveSet::SNAPKICK] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.3f, 0.5f);
    info.forceDir.Normalize();
    info.force = 10.0f;
    info.damage = 3;
    info.forceTime = 0.35f;

    m_mapMoveset[EMTMoveSet::JUMPKICK] = info;
}

void CMT::HitBoxUpdate()
{
    Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
    m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());
   
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
}

void CMT::HitBoxCheck(const sCollisionResult& result)
{
    if (!m_HitBox->IsEnable())
        return;

    if (result.dest->GetCollisionProfile()->name == "Player")
    {
        std::unordered_map<CGameObject*, int> collisions;

        {
            auto iter = m_Body->GetPrevColliderList().cbegin();
            auto iterEnd = m_Body->GetPrevColliderList().cend();

            for (; iter != iterEnd; iter++)
            {
                if ((*iter)->GetCollisionProfile()->name == "Player")
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
                if ((*iter)->GetCollisionProfile()->name == "PlayerBottom")
                {
                    collisions[(*iter)->GetGameObject()]++;
                }
            }
        }


        if (!collisions.empty())
        {
            sEnemyMove move = m_mapMoveset[m_CurrentMove];

            auto iter = collisions.begin();
            auto iterEnd = collisions.end();

            for (; iter != iterEnd; iter++)
            {
                bool right;

                if (GetWorldPos().x < (*iter).first->GetWorldPos().x)
                    right = true;
                else
                    right = false;

                CRCGPlayer* player = ((CRCGPlayer*)(*iter).first);

                if (player->IsInvincible() || player->CheckState(EKyokoState::INVINCIBLE))
                    return;

                if ((*iter).second < 2)
                    continue;

                if (player->CheckState(EKyokoState::DOWN))
                    return;

                if (player->OnGuard())
                {
                    if (m_CurrentTime - player->GetGuardStartTime() <= 1.7f)
                    {
                        PushState(EEnemyState::PARALYSIS, m_CurrentTime + 1.0f);
                        m_Sprite->ChangeAnimation("Hit1");
                        CResourceManager::GetInst()->SoundPlay("Parry");
                        if (m_Offset.y != 0.0f)
                        {
                            ResetOffset();
                            m_Offset = Vector3(0.0f, 0.0f, 0.0f);
                        }
                        DeactivateHitBox();
                        player->Parry();
                        m_Push = false;
                    }
                    else
                    {
                        CResourceManager::GetInst()->SoundPlay("Block");
                        player->Block();
                    }
                }
                else
                {
                    player->GetHit(move.attackType, move.forceDir, move.damage, move.force, move.forceTime, right);
                    CResourceManager::GetInst()->SoundPlay("Knockdown");
                }
            }
        }
    }
}

void CMT::OnGround(const sCollisionResult& result)
{
    if (result.dest->GetGameObject() == this &&
        m_Velocity.y <= 0.0f)
    {
        m_OnGround = true;
        m_Jump = false;
        m_Physics = false;
        m_Push = false;
        m_Velocity = Vector2(0.0f, 0.0f);
        m_AbsVel = Vector2(0.0f, 0.0f);

        if (CheckState(EEnemyState::ATTACK))
            PopStateEnd(EEnemyState::ATTACK);

        if (!m_MoveStart)
            return;

        if (CheckState(EEnemyState::GETHIT))
        {
            m_Sprite->ChangeAnimation("Knockdown_ground");
            m_HitCount = 0;
            PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
        }
        else
            m_Sprite->ChangeAnimation("Idle");
    }
}

void CMT::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
    m_MoveStart = true;
    if (m_Offset.y != 0.0f)
    {
        ResetOffset();
        m_Offset = Vector3(0.0f, 0.0f, 0.0f);
    }
    CRCGEnemy::GetHit(type, dir, damage, force, forceTime, right);
}

void CMT::SetIdleState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CMTIdle(this, m_NextState);
}

void CMT::SetPursueState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CMTPursue(this, m_NextState);
}
