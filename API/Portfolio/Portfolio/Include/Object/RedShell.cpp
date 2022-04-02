#include "RedShell.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"
#include "../Map/TileMap.h"
#include "../AI/AIGradientDescent.h"

unsigned int CRedShell::m_NumRedShellsOnPlayer = 0;

CRedShell::CRedShell(const Vector2& pos, const CDriver* target, const float forwardAngle, const bool forwardThrow,
    const float userHeight, CRaceScene* scene, const std::string& name)
    : CItem(Vector2(0.0f, 0.0f), 0.05f, REDSHELL_HITBOX_RADIUS, userHeight, scene, name)
{
    m_InactiveFrames = 10;
    m_Target = target;
    float angle = forwardThrow ? forwardAngle : forwardAngle + (float)M_PI;

    Vector2 forward = Vector2(cosf(angle), sinf(angle)) * ((REDSHELL_HITBOX_RADIUS + DRIVER_HITBOX_RADIUS + 0.1f) / MAP_ASSETS_WIDTH);

    m_Position = pos + forward;

    m_Speed = forward * REDSHELL_SPEED;

    if (!forwardThrow)
        m_Target = nullptr;


    if (userHeight <= 0.0f)
    {
        m_RemainingFlightTime = 0.0f;
        m_LastDirection = Vector2(0.0f, 0.0f);
        m_GradientWhenRamp = -1;
    }
    else
    {
        m_RemainingFlightTime = REDSHELL_FLIGHTTIME;
        m_LastDirection = Vector2(cosf(angle), sinf(angle)) * 0.3f;
        m_GradientWhenRamp = CAIGradientDescent::GetPositionValue(m_Position);
    }

    m_FollowingPlayer = m_Target != nullptr && m_Target->m_ControlType == EDriverControlType::PLAYER;

    if (m_FollowingPlayer)
    {
        m_NumRedShellsOnPlayer++;
        // 빨간 등껍질 추격음 재생
        m_Scene->GetSceneResoruce()->SoundPlay("RedShell");
    }

    m_Texture = m_Scene->GetSceneResoruce()->FindTexture("RedShell");
    m_Texture->SetPivot((float)m_Texture->GetWidth() / 2.0f, (float)m_Texture->GetHeight());
}

CRedShell::~CRedShell()
{
    if (m_FollowingPlayer && --m_NumRedShellsOnPlayer == 0)
    {
        // 추격음 멈춤
        m_Scene->GetSceneResoruce()->SoundStop("RedShell");
    }
}

void CRedShell::Update(float deltaTime)
{
    if (m_InactiveFrames > 0)
        m_InactiveFrames--;

    // 타겟이 없을 경우
    if (!m_Target)
        m_Position += m_Speed * deltaTime;
    else
    {
        Vector2 direction = m_Target->GetPos() - m_Position;

        float distance = sqrtf(fmaxf(1e-12f, direction.x * direction.x + direction.y * direction.y));

        // 일정거리 미만일 경우 그냥 타겟 방향으로 움직인다.
        if (distance < 0.08f)
            m_Position += direction * 0.65f;

        // 아닐 경우
        else
        {
            direction = Vector2(0.0f, 0.0f);

            // 체공 시간이 끝났을 경우 타일의 영향을 받는다.
            if (m_RemainingFlightTime <= 0.0f)
            {
                CTileMap* tileMap = m_Scene->GetTileMap();

                if (tileMap)
                {
                    int tileXCount = tileMap->GetTileCountX();

                    for (int i = 0; i < 3; i++)
                    {

                        CTile* tile = tileMap->GetTile(int(m_Position.y * MAP_TILES_HEIGHT) * tileXCount + int(m_Position.x * MAP_TILES_WIDTH));
                        ETileProperty tileProp = tile->GetTileProperty();

                        if (!tile)
                        {
                            m_Used = true;
                            return;
                        }

                        // 트랙이 아닐 경우는 그냥 빠져나온다.
                        if (i > 0 && tileProp != ETileProperty::TRACK)
                            break;

                        // AI 경사 하강법을 통해 갱신
                        direction += CAIGradientDescent::GetNextDirection(m_Position + direction);
                    }

                    direction /= sqrtf(fmaxf(1e-12f, direction.x * direction.x + direction.y * direction.y));
                    direction *= 0.3f;
                    m_LastDirection = direction;
                }

            }
            else
            {
                direction = m_LastDirection;
            }

            m_Position += direction * 1.4f * deltaTime;
        }
    }

    // 만약 체공시간이 남았을 경우
    if (m_RemainingFlightTime > 0.0f)
    {
        int currentGradient = CAIGradientDescent::GetPositionValue(m_Position);

        if ((currentGradient >= 0.0f && currentGradient <= m_GradientWhenRamp) ||
            m_RemainingFlightTime > REDSHELL_FLIGHTTIME / 2.0f)
        {
            m_RemainingFlightTime -= deltaTime;
        }
    }


    CTileMap* tileMap = m_Scene->GetTileMap();

    if (tileMap)
    {
        int tileXCount = tileMap->GetTileCountX();

        CTile* tile = tileMap->GetTile(int(m_Position.y * MAP_TILES_HEIGHT) * tileXCount + int(m_Position.x * MAP_TILES_WIDTH));

        if (!tile)
        {
            m_Used = true;
            return;
        }

        ETileProperty tileProp = tile->GetTileProperty();


        if (tileProp == ETileProperty::BLOCK)
        {
            if (m_RemainingFlightTime <= 0.0f)
            {
                m_Used = true;

                // 파괴효과
            }
        }
        else if (tileProp == ETileProperty::RAMP_HORIZONTAL ||
            tileProp == ETileProperty::RAMP_VERTICAL)
        {
            if (m_RemainingFlightTime <= 0.0f)
            {
                m_RemainingFlightTime = REDSHELL_FLIGHTTIME;

                // AI 코드
                m_GradientWhenRamp = CAIGradientDescent::GetPositionValue(m_Position);

                if (tileProp == ETileProperty::RAMP_HORIZONTAL)
                {
                    m_LastDirection.x = 0.0f;
                    m_LastDirection.y = m_LastDirection.y < 0.0f ? -0.3f : 0.3f;
                }
                else
                {
                    m_LastDirection.x = m_LastDirection.x < 0.0f ? -0.3f : 0.3f;
                    m_LastDirection.y = 0.0f;
                }
            }
        }
        else if (tileProp == ETileProperty::HAZARD)
        {
            if (m_RemainingFlightTime <= 0.0f)
            {
                m_Used = true;

                // 빠지는 효과 생성
            }
        }
        else if (m_Position.x < 0.0f || m_Position.x > 1.0f || m_Position.y < 0.0f || m_Position.y >1.0f)
        {
            m_Used = true;
        }
    }

    if (m_RemainingFlightTime > 0.0f)
    {
        m_Height = REDSHELL_MAXHEIGHT * (0.5f - fabs(m_RemainingFlightTime / REDSHELL_FLIGHTTIME - 0.5f));
    }
}

bool CRedShell::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
    if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
        return false;

    if (m_Used || m_InactiveFrames > 0)
        return false;

    if (otherIsImmune)
    {
        m_Used = true;

        // 파괴 효과 생성

        collisionData = CollisionData(Vector2(0.0f, 0.0f), 1.0f, ECollisionType::NO_HIT);

        return true;
    }

    collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.4f, ECollisionType::HIT);

    m_Used = true;

    // 파괴 효과 생성

    if (m_Target && m_Target->m_ControlType == EDriverControlType::PLAYER && m_Used)
    {
        // 소리 정지
        m_Scene->GetSceneResoruce()->SoundStop("RedShell");
    }

    return true;
}
