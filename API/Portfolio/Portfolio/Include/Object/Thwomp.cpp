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
	// �ʱ⿡�� UP ����
	m_State = EThwmopState::UP;
}

// �� ������Ʈ ���� ���� �ð��� �����Ѵ�.
void CThwomp::Update(float deltaTime)
{
	// �ð��� �����Ѵ�.
	m_Time += deltaTime;

	switch (m_State)
	{
	// ���� UP ���°� ������ �ð��� 0�ʺ��� ũ�ų� ���ٸ�
	case CThwomp::EThwmopState::UP:
		if (m_Time >= 0.0f)
		{
			// �������� ���·� �ٲ۴�.
			m_State = EThwmopState::GOING_DONW;
			m_Time = 0.0f;
		}
		break;
	// �� ������ ���¿�����
	case CThwomp::EThwmopState::DOWN:
		// ���� �ð��� ������
		if (m_Time >= 0.0f)
		{
			// �ٽ� �ö󰡴� ���·� �ٲ۴�.
			m_State = EThwmopState::GOING_UP;
			m_Time = 0.0f;
		}
		break;
	// �ö󰡴� ����
	case CThwomp::EThwmopState::GOING_UP:
		// 2.0�ʰ� �Ǵ� ����
		m_Time = fminf(m_Time, 2.0f);
		// ������ ���̴� �ִ� ���̾� �� ���̴�.
		m_Height = m_Time * THWOMP_MAXHEIGHT / 2.0f;
		if (m_Time == 2.0f)
		{
			// �ִ� ���̰� ������ UP ���·� �ٲ��
			m_State = EThwmopState::UP;

			// -2 ~ -6 ������ ������ ���� ��ȯ ��Ű�� �����Ѵ�.
			m_Time = ((rand() % 5) + 2) * -1;

			// ���� �ٸ� Ÿ���� �����̶��
			// �ӵ��� �� ��������.
			if (m_Type == EWallObjectType::SUPER_THWOMP)
			{
				m_Time *= 0.85f;
			}
		}
		break;
	// �Ʒ��� �������� ����
	case CThwomp::EThwmopState::GOING_DONW:
		// �������� �ð��� 0.15���̴�.
		m_Time = fminf(m_Time, 0.15f);

		// 0.15�ʰ� �Ǵ� ���� ���̴� 0�� �� ���̴�.
		m_Height = (0.15f - m_Time) * 4.0f * THWOMP_MAXHEIGHT;

		if (m_Time == 0.15f)
		{
			// ���̰� 0�� ���������� DOWN���·� �����.
			m_State = EThwmopState::DOWN;
			// -2 ~ -4�� ������ ��ȯ
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
	// �������� ���� ��� �Ѿ��.
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	// ���� ����� ���̰� ���� ���ߴٸ� �浹�� ���� ����
	if (m_Height > STOMP_HEIGHT)
		return false;

	float speed = otherSpeed.x * otherSpeed.x + otherSpeed.y * otherSpeed.y;

	// ���� �Ÿ��� ���� �ʰų� ���� ������ ���°� �������� ���°� �ƴ϶�� 
	// �׳� �Ϲ� �浹
	if (distance > m_HitboxRadius * m_HitboxRadius ||
		(m_State != EThwmopState::GOING_DONW && (m_State != EThwmopState::DOWN || speed > 3e-3f)))
	{
		return CWallObject::DefaultSolveCollision(collisionData, otherSpeed, otherPos, m_Position, distance);
	}

	// ���� �����̶�� �浹�� ����
	if (otherIsImmune)
	{
		collisionData = CollisionData(Vector2(0.0f, 0.0f), 1.0f, ECollisionType::NO_HIT);

		return true;
	}


	// �׿ܿ��� SMASH
	collisionData = CollisionData(Vector2(0.0f, 0.0f), 0.0f, ECollisionType::SMASH);

	return true;
}
