#include "Thwomp.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CThwomp::CThwomp(const Vector2& pos, const EWallObjectType type, CRaceScene* scene, const std::string& name) :
	CWallObject(pos, 2.5f, 1.5f, THWOMP_MAXHEIGHT, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, scene, name)
{
	m_Type = type;

	switch (type)
	{
	case EWallObjectType::THWOMP:
	{
		m_Scene->GetSceneResoruce()->LoadTexture("Thwomp", TEXT("WallObjects/Thwomp.bmp"));
		m_Texture = m_Scene->GetSceneResoruce()->FindTexture("Thwomp");
		m_Texture->SetPivot(m_Texture->GetWidth() / 2.0f, m_Texture->GetHeight());
		m_Texture->SetColorKey(255, 0, 255);
	}
		break;
	case EWallObjectType::SUPER_THWOMP:
	{
		m_Scene->GetSceneResoruce()->LoadTexture("Super Thwomp", TEXT("WallObjects/SuperThwomp.bmp"));
		m_Texture = m_Scene->GetSceneResoruce()->FindTexture("Super Thwomp");
		m_Texture->SetPivot(m_Texture->GetWidth() / 2.0f, m_Texture->GetHeight());
		m_Texture->SetColorKey(255, 0, 255);
	}
		break;
	}

	m_Time = 0.0f;
	// 초기에는 UP 상태
	m_State = EThwmopState::UP;
}

// 매 업데이트 순간 마다 시간을 누적한다.
void CThwomp::Update(float deltaTime)
{
	// 시간을 누적한다.
	m_Time += deltaTime;

	switch (m_State)
	{
	// 만약 UP 상태고 누적된 시간이 0초보다 크거나 같다면
	case CThwomp::EThwmopState::UP:
		if (m_Time >= 0.0f)
		{
			// 내려오는 상태로 바꾼다.
			m_State = EThwmopState::GOING_DONW;
			m_Time = 0.0f;
		}
		break;
	// 다 내려온 상태에서는
	case CThwomp::EThwmopState::DOWN:
		// 일정 시간이 지나면
		if (m_Time >= 0.0f)
		{
			// 다시 올라가는 상태로 바꾼다.
			m_State = EThwmopState::GOING_UP;
			m_Time = 0.0f;
		}
		break;
	// 올라가는 상태
	case CThwomp::EThwmopState::GOING_UP:
		// 2.0초가 되는 순간
		m_Time = fminf(m_Time, 2.0f);
		// 현재의 높이는 최대 높이악 될 것이다.
		m_Height = m_Time * THWOMP_MAXHEIGHT / 2.0f;
		if (m_Time == 2.0f)
		{
			// 최대 높이가 됬으니 UP 상태로 바뀌고
			m_State = EThwmopState::UP;

			// -2 ~ -6 까지의 랜덤한 수로 변환 시키고 누적한다.
			m_Time = ((rand() % 5) + 2) * -1;

			// 만약 다른 타입의 쿵쿵이라면
			// 속도는 더 빨라진다.
			if (m_Type == EWallObjectType::SUPER_THWOMP)
			{
				m_Time *= 0.85f;
			}
		}
		break;
	// 아래로 내려오는 상태
	case CThwomp::EThwmopState::GOING_DONW:
		// 내려오는 시간은 0.15초이다.
		m_Time = fminf(m_Time, 0.15f);

		// 0.15초가 되는 순간 높이는 0이 될 것이다.
		m_Height = (0.15f - m_Time) * 4.0f * THWOMP_MAXHEIGHT;

		if (m_Time == 0.15f)
		{
			// 높이가 0에 도달했으니 DOWN상태로 만든다.
			m_State = EThwmopState::DOWN;
			// -2 ~ -4의 값으로 변환
			m_Time = ((rand() % 3) + 2) * -1;
		}

		break;
	}
}

CTexture* CThwomp::GetTexture()
{
	CTexture* cloneTexture = m_Texture->Clone();

	cloneTexture->SetColorKey(255, 0, 255);
	cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

	return cloneTexture;
}

bool CThwomp::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
	// 높이차가 생길 경우 넘어간다.
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	// 만약 충분한 높이가 되지 못했다면 충돌은 하지 못함
	if (m_Height > STOMP_HEIGHT)
		return false;

	float speed = otherSpeed.x * otherSpeed.x + otherSpeed.y * otherSpeed.y;

	// 만약 거리가 되지 않거나 현재 쿵쿵의 상태가 내려오는 상태가 아니라면 
	// 그냥 일반 충돌
	if (distance > m_HitboxRadius * m_HitboxRadius ||
		(m_State != EThwmopState::GOING_DONW && (m_State != EThwmopState::DOWN || speed > 3e-3f)))
	{
		return CWallObject::DefaultSolveCollision(collisionData, otherSpeed, otherPos, m_Position, distance);
	}

	// 만약 무적이라면 충돌은 무시
	if (otherIsImmune)
	{
		collisionData = CollisionData(Vector2(0.0f, 0.0f), 1.0f, ECollisionType::NO_HIT);

		return true;
	}


	// 그외에는 SMASH
	collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.0f, ECollisionType::SMASH);

	return true;
}
