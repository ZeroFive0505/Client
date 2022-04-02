#include "CollisionEffect.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

inline void CCollisionEffect::SetCurrentEffect(int frame)
{
    m_Texture = m_Scene->GetSceneResoruce()->FindTexture("Collision_" + std::to_string(frame + 1));
    m_Texture->SetPivot(m_Texture->GetWidth() / 2.0f, m_Texture->GetHeight() / 1.5f);
}

CCollisionEffect::CCollisionEffect(const Vector2& pos, CRaceScene* scene, const std::string& name) :
    CItem(Vector2(pos.x * MAP_ASSETS_WIDTH, pos.y * MAP_ASSETS_HEIGHT), 1.05f, 0.0f, 0.0f, scene, name),
    m_CurrentTime(0.0f),
    m_CurrentFrame(0)
{
    SetCurrentEffect(0);
}

void CCollisionEffect::Update(float deltaTime)
{
    m_CurrentTime += deltaTime;

    if (m_CurrentTime > COLLISION_TIME_BETWEEN_FRAME)
    {
        m_CurrentFrame++;
        m_CurrentTime = 0.0f;

        if (m_CurrentFrame >= NUM_COLLISION_EFFECT)
        {
            m_Used = true;
        }
        else
        {
            SetCurrentEffect(m_CurrentFrame);
        }
    }
}

bool CCollisionEffect::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
    return false;
}
