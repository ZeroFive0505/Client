#include "SchoolBoy.h"
#include "SchoolBoyAnimation2D.h"
#include "RCGPlayer.h"
#include "SchoolBoyIdle.h"
#include "SchoolBoyPursue.h"
#include "../Widget/RCGEnemyHUD.h"
#include "Engine.h"

CSchoolBoy::CSchoolBoy() :
    m_State(nullptr),
    m_NextState(nullptr)
{
}

CSchoolBoy::CSchoolBoy(const CSchoolBoy& obj) :
    CRCGEnemy(obj)
{
}

CSchoolBoy::~CSchoolBoy()
{
    SAFE_DELETE(m_State);
    SAFE_DELETE(m_NextState);
}

bool CSchoolBoy::Init()
{
    if (!CRCGEnemy::Init())
        return false;

    m_Sprite->CreateAnimationInstance<CSchoolBoyAnimation2D>();

    m_Bottom->AddCollisionCallback<CRCGEnemy>(Collision_State::Begin, this, &CRCGEnemy::OnGround);

    m_NavAgent->SetMoveSpeed(m_WalkSpeed);

    SetMoveSetInfo();

    m_HitBox->AddCollisionCallback<CSchoolBoy>(Collision_State::Begin, this, &CSchoolBoy::HitBoxCheck);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Axekick", this, &CSchoolBoy::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Elbowupper", this, &CSchoolBoy::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Sandtoss", this, &CSchoolBoy::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Rightstraight", this, &CSchoolBoy::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Sidekick", this, &CSchoolBoy::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CSchoolBoy>("Jumppunch", this, &CSchoolBoy::JumpPunchEndCallback);

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

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Axekick", "AxekickHitBoxOn", 3, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Axekick", "AxekickHitBoxOff", 5, this, &CSchoolBoy::ActivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Elbowupper", "ElbowUpperHitBoxOn", 2, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Elbowupper", "ElbowUpperHitBoxOff", 4, this, &CSchoolBoy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Jumppunch", "JumpPunchHitBoxOn", 2, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Jumppunch", "JumpPunchHitBoxOff", 4, this, &CSchoolBoy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Rightstraight", "RightstraightHitBoxOn", 2, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Rightstraight", "RightstraightHitBoxOff", 4, this, &CSchoolBoy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Sandtoss", "SandTossHitBoxOn", 5, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Sandtoss", "SandTossHitBoxOff", 6, this, &CSchoolBoy::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Sidekick", "SidekickHitBoxOn", 2, this, &CSchoolBoy::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CSchoolBoy>("Sidekick", "SidekickHitBoxOff", 4, this, &CSchoolBoy::DeactivateHitBox);

    m_HUDWidget = CreateComponent<CWidgetComponent>("EnemyHUD");

    m_EnemyHUD = m_HUDWidget->CreateWidgetWindow<CRCGEnemyHUD>("EnemyHUDWidget");

    m_Transform->AddChild(m_HUDWidget);

    m_HUDWidget->SetRelativePos(-22.5f, 80.0f, 0.0f);

    m_Grabbable = true;

    return true;
}

void CSchoolBoy::Update(float deltaTime)
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
    {
        Destroy();
    }

    if (m_HitBox->IsEnable())
        HitBoxUpdate();

    if (CheckState(EEnemyState::DEFEADTED))
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

void CSchoolBoy::PostUpdate(float deltaTime)
{
    CRCGEnemy::PostUpdate(deltaTime);
}

CSchoolBoy* CSchoolBoy::Clone()
{
    return new CSchoolBoy(*this);
}

void CSchoolBoy::SetMoveSetInfo()
{
    sEnemyMove info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.5f, 0.3f);
    info.forceDir.Normalize();
    info.forceTime = 0.35f;
    info.damage = 5;
    info.force = 12.0f;

    m_mapMoveset[ESchoolBoyMoveSet::AXEKICK] = info;

    info = {};

    info.attackType = EAttackType::BLOWNBACK;
    info.forceDir = Vector2(1.0f, 0.1f);
    info.forceDir.Normalize();
    info.forceTime = 0.5f;
    info.damage = 10;
    info.force = 20.0f;

    m_mapMoveset[ESchoolBoyMoveSet::SIDEKICK] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.2f, 1.0f);
    info.forceDir.Normalize();
    info.forceTime = 0.3f;
    info.damage = 4;
    info.force = 12.0f;
    

    m_mapMoveset[ESchoolBoyMoveSet::ELBOW_UPPERCUT] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.5f, -0.3f);
    info.forceDir.Normalize();
    info.forceTime = 0.25f;
    info.damage = 5;
    info.force = 10.0f;

    m_mapMoveset[ESchoolBoyMoveSet::JUMP_PUNCH] = info;

    info = {};

    info.attackType = EAttackType::BLOWNBACK;
    info.forceDir = Vector2(1.0f, 0.1f);
    info.forceDir.Normalize();
    info.forceTime = 0.35f;
    info.damage = 10;
    info.force = 20.0f;
    
    m_mapMoveset[ESchoolBoyMoveSet::RIGHTSTRAIGHT_PUNCH] = info;

    info = {};

    info.attackType = EAttackType::HIT;
    info.forceDir = Vector2(0.1f, 0.2f);
    info.forceDir.Normalize();
    info.forceTime = 0.1f;
    info.damage = 1;
    info.force = 1.2f;

    m_mapMoveset[ESchoolBoyMoveSet::SANDTOSS] = info;
}

void CSchoolBoy::HitBoxUpdate()
{
    Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
    m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());

    switch (m_CurrentMove)
    {
    case ESchoolBoyMoveSet::AXEKICK:
    case ESchoolBoyMoveSet::ELBOW_UPPERCUT:
    case ESchoolBoyMoveSet::JUMP_PUNCH:
    case ESchoolBoyMoveSet::RIGHTSTRAIGHT_PUNCH:
    case ESchoolBoyMoveSet::SIDEKICK:
    {
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
        break;
    case ESchoolBoyMoveSet::SANDTOSS:
    {
        if (m_FacingRight)
        {
            m_HitBox->SetExtent(size.x * 0.75f, size.y);
            m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
        }
        else
        {
            m_HitBox->SetExtent(size.x * 0.75f, size.y);
            m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
        }
    }
        break;
    }
}

void CSchoolBoy::HitBoxCheck(const sCollisionResult& result)
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
                    if (CEngine::GetInst()->GetCurrentPlayTime() - player->GetGuardStartTime() <= 0.1f)
                    {
                        PushState(EEnemyState::PARALYSIS, m_CurrentTime + 1.0f);
                        m_Sprite->ChangeAnimation("Hit1");
                        CResourceManager::GetInst()->SoundPlay("Parry");
                        DeactivateHitBox();
                        player->Parry();
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

                    if (m_CurrentMove == ESchoolBoyMoveSet::SANDTOSS)
                    {
                        if (!player->CheckState(EKyokoState::STUNNED))
                            player->SetStun();
                    }

                    CResourceManager::GetInst()->SoundPlay("Knockdown");
                }
            }
        }
    }
}

void CSchoolBoy::SetIdleState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CSchoolBoyIdle(this, m_NextState);
}

void CSchoolBoy::SetPursueState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CSchoolBoyPursue(this, m_NextState);
}
