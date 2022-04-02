#include "Cheerleader.h"
#include "CheerleaderAnimation.h"
#include "../Widget/RCGEnemyHUD.h"
#include "CheerleaderIdle.h"
#include "CheerleaderPursue.h"

CCheerleader::CCheerleader() :
    m_State(nullptr),
    m_NextState(nullptr)
{
}

CCheerleader::CCheerleader(const CCheerleader& obj) :
    CRCGEnemy(obj)
{
}

CCheerleader::~CCheerleader()
{
    SAFE_DELETE(m_State);
    SAFE_DELETE(m_NextState);
}

void CCheerleader::Start()
{
    CRCGEnemy::Start();
}

bool CCheerleader::Init()
{
    if (!CRCGEnemy::Init())
        return false;

    m_Sprite->CreateAnimationInstance<CCheerleaderAnimation>();

    m_Bottom->AddCollisionCallback<CCheerleader>(Collision_State::Begin, this, &CCheerleader::OnGround);

    m_NavAgent->SetMoveSpeed(m_WalkSpeed);

    SetMoveSetInfo();

    m_HitBox->AddCollisionCallback<CCheerleader>(Collision_State::Begin, this, &CCheerleader::HitBoxCheck);

    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Backflip", this, &CCheerleader::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Cartwheel", this, &CCheerleader::CartWheelEnd);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Doublepom", this, &CCheerleader::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Jab", this, &CCheerleader::GroundAttackEndCallback);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Alert", this, &CCheerleader::AlertFinish);

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

    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Backflip", "BackflipHitBoxOn", 7, this, &CCheerleader::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Backflip", "BackflipHitBoxOff", 8, this, &CCheerleader::DeactivateHitBox);

    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Backflip", "BackflipOffset", 15, this, &CCheerleader::SpriteOffset);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Backflip", "BackflipResetOffset", 19, this, &CCheerleader::ResetOffset);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CCheerleader>("Divekick_start", this, &CCheerleader::GravityOff);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Divekick_start", this, &CCheerleader::DiveKickBegin);

    m_Sprite->GetAnimationInstance()->SetStartFunction<CCheerleader>("Divekick_middle", this, &CCheerleader::DiveKickFall);

    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Divekick_end", "DiveKickHitBoxOff", 2, this, &CCheerleader::DeactivateHitBox);
    m_Sprite->GetAnimationInstance()->SetEndFunction<CCheerleader>("Divekick_end", this, &CCheerleader::DiveKickEnd);


    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Doublepom", "DoublepomHitBoxOn", 3, this, &CCheerleader::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Doublepom", "DoublepomHitBoxOff", 4, this, &CCheerleader::DeactivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Doublepom", "DoublepomSetOffset", 4, this, &CCheerleader::SpriteOffset);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Doublepom", "DoublepomResetOffset", 8, this, &CCheerleader::ResetOffset);

    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Jab", "JabHitBoxOn", 3, this, &CCheerleader::ActivateHitBox);
    m_Sprite->GetAnimationInstance()->AddNotify<CCheerleader>("Jab", "JabHitBoxOff", 4, this, &CCheerleader::DeactivateHitBox);

    m_HUDWidget = CreateComponent<CWidgetComponent>("EnemyHUD");

    m_EnemyHUD = m_HUDWidget->CreateWidgetWindow<CRCGEnemyHUD>("EnemyHUDWidget");

    m_Transform->AddChild(m_HUDWidget);

    m_HUDWidget->SetRelativePos(-22.5f, 80.0f, 0.0f);

    m_MoveStart = false;
    
    m_Grabbable = true;

    return true;
}

void CCheerleader::Update(float deltaTime)
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
        if (m_CartWheelPush)
        {
            if (!m_RightWallCollision && m_CartWheelDirection == 0)
                AddWorldPos(GetWorldAxis(AXIS_X) * 400.0f * deltaTime);
            else if(!m_LeftWallCollision && m_CartWheelDirection == 1)
                AddWorldPos(GetWorldAxis(AXIS_X) * -400.0f * deltaTime);
        }
        else
        {
            if (m_FacingRight && !m_RightWallCollision)
                AddWorldPos(GetWorldAxis(AXIS_X) * 650.0f * deltaTime);
            else if (!m_FacingRight && !m_LeftWallCollision)
                AddWorldPos(GetWorldAxis(AXIS_X) * -650.0f * deltaTime);
        }
    }

    if (m_Sprite->GetAnimationInstance()->CheckCurrentAnimation("Jump") && !m_DiveKickStart && m_Velocity.y >= -0.01f && m_Velocity.y <= 0.01f)
    {
        m_Sprite->ChangeAnimation("Divekick_start");
        m_DiveKickStart = true;
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

void CCheerleader::PostUpdate(float deltaTime)
{
    CRCGEnemy::PostUpdate(deltaTime);
}

CCheerleader* CCheerleader::Clone()
{
    return new CCheerleader(*this);
}

void CCheerleader::SetMoveSetInfo()
{
    sEnemyMove info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(0.15f, 1.0f);
    info.forceDir.Normalize();
    info.force = 10.0f;
    info.damage = 5;
    info.forceTime = 0.4f;

    m_mapMoveset[ECheerleaderMoveSet::BACKFLIP] = info;

    info = {};

    info.attackType = EAttackType::HIT;
    info.forceDir = Vector2(0.1f, 0.1f);
    info.forceDir.Normalize();
    info.force = 1.0f;
    info.damage = 0;
    info.forceTime = 0.1f;

    m_mapMoveset[ECheerleaderMoveSet::CARTWHEEL] = info;

    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(1.0f, 0.35f);
    info.forceDir.Normalize();
    info.force = 10.0f;
    info.damage = 10;
    info.forceTime = 0.45f;

    m_mapMoveset[ECheerleaderMoveSet::DIVEKICK] = info;


    info = {};

    info.attackType = EAttackType::KNOCKDOWN;
    info.forceDir = Vector2(1.0f, 0.25f);
    info.forceDir.Normalize();
    info.force = 9.0f;
    info.damage = 7;
    info.forceTime = 0.4f;

    m_mapMoveset[ECheerleaderMoveSet::DOUBLEPOM] = info;

    info = {};

    info.attackType = EAttackType::HIT;
    info.forceDir = Vector2(1.0f, 0.1f);
    info.forceDir.Normalize();
    info.force = 2.4f;
    info.damage = 3;
    info.forceTime = 0.2f;

    m_mapMoveset[ECheerleaderMoveSet::JAB] = info;
}

void CCheerleader::OnGround(const sCollisionResult& result)
{
    if (result.dest->GetGameObject() == this &&
        m_Velocity.y <= 0.0f)
    {
        m_OnGround = true;
        m_Jump = false;
        m_Physics = false;
        m_Push = false;
        m_DiveKickStart = false;
        m_Velocity = Vector2(0.0f, 0.0f);
        m_AbsVel = Vector2(0.0f, 0.0f);


        if (!m_MoveStart)
        {
            m_Sprite->GetAnimationInstance()->GetCurrentAnimation()->SetCurrentFrame(0);
            return;
        }

        if (CheckState(EEnemyState::GETHIT))
        {
            m_Sprite->ChangeAnimation("Knockdown_ground");
            m_HitCount = 0;
            PushState(EEnemyState::DOWN, m_CurrentTime + 3.0f);
        }
        else if (m_CurrentMove == ECheerleaderMoveSet::DIVEKICK)
        {
            m_Sprite->ChangeAnimation("Divekick_end");
            m_Gravity = -5.0f;
        }
        else
            m_Sprite->ChangeAnimation("Idle");
    }
}

void CCheerleader::GetHit(EAttackType type, const Vector2& dir, int damage, float force, float forceTime, bool right)
{
    m_MoveStart = true;
    m_DiveKickStart = false;
    if (m_Offset.y != 0.0f)
    {
        ResetOffset();
        m_Offset = Vector3(0.0f, 0.0f, 0.0f);
    }
    CRCGEnemy::GetHit(type, dir, damage, force, forceTime, right);
}

void CCheerleader::HitBoxUpdate()
{
    Vector3 size = m_Sprite->GetRelativeScale() * 0.5f;
    m_HitBox->SetWorldPos(m_Body->GetWorldPos() + m_Body->GetOffset());

    switch (m_CurrentMove)
    {
    case ECheerleaderMoveSet::JAB:
    case ECheerleaderMoveSet::BACKFLIP:
    case ECheerleaderMoveSet::DOUBLEPOM:
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
    case ECheerleaderMoveSet::DIVEKICK:
    {
        if (m_FacingRight)
        {
            m_HitBox->SetExtent(size.x , size.y * 0.75f);
            m_HitBox->SetOffset(size.x * 0.5f, 0.0f, 0.0f);
        }
        else
        {
            m_HitBox->SetExtent(size.x, size.y * 0.75f);
            m_HitBox->SetOffset(-size.x * 0.5f, 0.0f, 0.0f);
        }
    }
    break;
    }
}

void CCheerleader::HitBoxCheck(const sCollisionResult& result)
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
                        if (m_OnGround)
                        {
                            PushState(EEnemyState::PARALYSIS, m_CurrentTime + 1.0f);
                            m_Sprite->ChangeAnimation("Hit1");
                            DeactivateHitBox();
                            if (m_Offset.y != 0.0f)
                            {
                                ResetOffset();
                                m_Offset = Vector3(0.0f, 0.0f, 0.0f);
                            }
                        }   
                        CResourceManager::GetInst()->SoundPlay("Parry");
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
                    CResourceManager::GetInst()->SoundPlay("Knockdown");
                }
            }
        }
    }
}

void CCheerleader::SetIdleState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CCheerleaderIdle(this, m_NextState);
}

void CCheerleader::SetPursueState()
{
    SAFE_DELETE(m_State);

    m_State = (CState*)new CCheerleaderPursue(this, m_NextState);
}
