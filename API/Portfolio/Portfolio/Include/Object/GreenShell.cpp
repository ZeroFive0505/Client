#include "GreenShell.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/Scene.h"
#include "../Map/TileMap.h"

CGreenShell::CGreenShell(const Vector2& pos, const float forwardAngle, const bool forwardThrow, const float userHeight, CRaceScene* scene, const std::string& name) :
    CItem(Vector2(0.0f, 0.0f), 0.05f, GREENSHELL_HITBOX_RADIUS, userHeight, scene, name)
{
    m_Lives = GREENSHELL_NUM_LIVES;
    
    float angle = forwardThrow ? forwardAngle : forwardAngle + (float)M_PI;

    Vector2 forward = Vector2(cosf(angle), sinf(angle)) * ((GREENSHELL_HITBOX_RADIUS + DRIVER_HITBOX_RADIUS + 0.1f) / MAP_ASSETS_WIDTH);


    m_Texture = m_Scene->GetSceneResoruce()->FindTexture("GreenShell");

    m_Position = pos + forward;

    m_Speed = forward * GREENSHELL_SPEED;

    m_Texture->SetPivot((float)m_Texture->GetWidth() / 2.0f, (float)m_Texture->GetHeight());
}

void CGreenShell::Update(float deltaTime)
{
    for (int i = 0; i < GREENSHELL_NUM_MARCHES_UPDATE; i++)
    {
        MarchingUpdate(deltaTime / (float)GREENSHELL_NUM_MARCHES_UPDATE);
    }
}

void CGreenShell::MarchingUpdate(float deltaTime)
{
    Vector2 prevPos = m_Position;
    m_Position += m_Speed * deltaTime;
    m_VerticalSpeed += GREENSHELL_GRAVITY * deltaTime;
    m_Height = fmaxf(0.0f, m_Height + m_VerticalSpeed * deltaTime);

    if (m_Position.x < 0.0f || m_Position.x > 1.0f || m_Position.y < 0.0f ||
        m_Position.y > 1.0f)
    {
        // 파괴
        m_Used = true;
        return;
    }
    else if (m_Height > 0.0f)
        return;

    CTileMap* tileMap = m_Scene->GetTileMap();

    if (tileMap)
    {
        int y = int(m_Position.y * MAP_TILES_HEIGHT);
        int x = int(m_Position.x * MAP_TILES_WIDTH);
        int tileCountX = tileMap->GetTileCountX();
        ETileProperty tile = tileMap->GetTile(y * tileCountX + x)->GetTileProperty();

        if (tile == ETileProperty::BLOCK && m_Lives <= 0)
        {
            m_Used = true;

            // 파괴효과 생성
        }
        else if (tile == ETileProperty::BLOCK)
        {
            Vector2 delta = (m_Position - prevPos) / (float)GREENSHELL_NUM_MARCHES_HIT;
            Vector2 block = prevPos + delta;

            while (tileMap->GetTile(int(block.y * MAP_TILES_HEIGHT) * tileCountX + int(block.x * MAP_TILES_WIDTH))->GetTileProperty()
                != ETileProperty::BLOCK)
            {
                block += delta;
            }

            Vector2 blockCenter = Vector2((int)(block.x * MAP_TILES_WIDTH), (int)(block.y * MAP_TILES_HEIGHT));

            blockCenter = Vector2((blockCenter.x + 0.5f) / MAP_TILES_WIDTH, (blockCenter.y + 0.5f) / MAP_TILES_HEIGHT);

            if (fabsf(blockCenter.x - prevPos.x) > fabsf(blockCenter.y - prevPos.y))
                m_Speed.x *= -1.0f;
            else
                m_Speed.y *= -1.0f;

            m_Position = prevPos;
            m_Lives--;
        }
        else if (tile == ETileProperty::RAMP_HORIZONTAL || tile == ETileProperty::RAMP_VERTICAL)
            m_VerticalSpeed = GREENSHELL_JUMP_SPEED;
        else if (tile == ETileProperty::HAZARD)
        {
            m_Used = true;
            // 빠지는 이펙트 생성
        }
    }
}

bool CGreenShell::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
    if(CWallObject::CheckHeightDifference(m_Height, otherHeight))
        return false;

    if (m_Used)
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

    // 파괴효과 생성

    return true;
}

void CGreenShell::Init()
{

}

CTexture* CGreenShell::GetTexture()
{
    CTexture* cloneTexture = m_Texture->Clone();
    cloneTexture->SetColorKey(255, 0, 255);
    cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

    return cloneTexture;
}
