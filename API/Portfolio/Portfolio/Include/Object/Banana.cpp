#include "Banana.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"
#include "../Map/TileMap.h"
#include "../Map/Tile.h"

CBanana::CBanana(const Vector2& pos, const float forwardAngle, const bool forwardThrow, const float userHeight, CRaceScene* scene, const std::string& name) :
    CItem(Vector2(0.0f, 0.0f), 0.05f, BANANA_HITBOX_RADIUS, userHeight + 1.0f, scene, name)
{
    Vector2 forward = Vector2(cosf(forwardAngle), sinf(forwardAngle)) * 
        ((BANANA_HITBOX_RADIUS + DRIVER_HITBOX_RADIUS + 0.1f) / MAP_ASSETS_WIDTH);

    forward *= forwardThrow ? 1.0f : -1.0f;

    m_Position = pos + forward;

    if (forwardThrow)
    {
        m_Speed.x = forward.x * BANANA_SPEED * 2.5f;
        m_Speed.y = forward.y * BANANA_SPEED * 2.5f;
        m_Speed.z = BANANA_SPEED * 4.0f;
    }
    else
    {
        m_Speed.x = forward.x;
        m_Speed.y = forward.y;
        m_Speed.z = 0.0f;
    }

    m_Texture = m_Scene->GetSceneResoruce()->FindTexture("Banana");
    m_Texture->SetPivot((float)m_Texture->GetWidth() / 2.0f, (float)m_Texture->GetHeight());
}

void CBanana::Update(float deltaTime)
{
    if (m_Height <= 0.0f)
    {
        m_Speed.x = 0.0f;
        m_Speed.y = 0.0f;
        m_Speed.z = 0.0f;

        return;
    }

    m_Position += Vector2(m_Speed.x, m_Speed.y) * deltaTime;
    m_Height = std::fmaxf(m_Height + m_Speed.z * deltaTime, 0.0f);
    m_Speed.z += BANANA_GRAVITY * deltaTime;

    CTileMap* tileMap = m_Scene->GetTileMap();

    if (tileMap)
    {
        int tileCountX = tileMap->GetTileCountX();
        if (m_Height <= 0.0f)
        {
            CTile* tile = tileMap->GetTile(int(m_Position.y * MAP_TILES_HEIGHT) * tileCountX + int(m_Position.x * MAP_TILES_WIDTH));
            if (tile && tile->GetTileProperty() == ETileProperty::HAZARD)
            {
                // 잠기는 효과 생성
                m_Used = true;
            }
        }
        else if (m_Position.x < 0.0f || m_Position.x > 1.0f || m_Position.y < 0.0f || m_Position.y > 1.0f)
        {
            m_Used = true;
        }
    }
}

bool CBanana::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
    if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
        return false;

    if (m_Used)
        return false;

    if (otherIsImmune)
    {
        m_Used = true;

        // 파괴효과 생성

        collisionData = CollisionData(Vector2(0.0f, 0.0f), 1.0f, ECollisionType::NO_HIT);

        return true;
    }

    collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.4f, ECollisionType::HIT);

    m_Used = true;

    // 파괴 효과 생성

    return true;
}
