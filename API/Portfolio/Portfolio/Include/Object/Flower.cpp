#include "Flower.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CFlower::CFlower(const Vector2& pos, const EWallObjectType type, CRaceScene* scene, const std::string& name) :
	CWallObject(pos, 2.5f, 2.0f, 0.0f, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, scene, name)
{
	m_Type = type;


	scene->GetSceneResoruce()->LoadTexture("Flower_1", TEXT("WallObjects/Flower_1.bmp"));

	m_Textures[0] = scene->GetSceneResoruce()->FindTexture("Flower_1");

	scene->GetSceneResoruce()->LoadTexture("Flower_2", TEXT("WallObjects/Flower_2.bmp"));

	m_Textures[1] = scene->GetSceneResoruce()->FindTexture("Flower_2");

	m_CurrentTexture = m_Textures[0];
}

// ������Ʈ ȣ�� �� �ð��� �����ǰ� ���� �ð��� �����Ǹ� 
// �ִϸ��̼��� �ٲ��.
void CFlower::Update(float deltaTime)
{
	m_fElasedTime += deltaTime;

	if (m_fElasedTime >= m_FlowerAnimTime)
	{
		m_TexIndex = (m_TexIndex + 1) % 2;

		m_CurrentTexture = m_Textures[m_TexIndex];

		m_fElasedTime = 0.0f;
	}
}

CTexture* CFlower::GetTexture()
{
	CTexture* cloneTexture = m_CurrentTexture->Clone();

	cloneTexture->SetColorKey(255, 0, 255);
	cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

	return cloneTexture;
}

// �ö���� ��� �浹ó���� �޶�����.
bool CFlower::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
	// ����üũ ��
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	// �ӵ��� ���ϰ�
	float otherSpeedModule =
		sqrtf(otherSpeed.x * otherSpeed.x + otherSpeed.y * otherSpeed.y) + 0.05f;

	// ���� �ǰ� �����ȿ� �ִٸ�
	if (distance < m_HitboxRadius * m_HitboxRadius)
	{
		// ������ �浹�� ���������� �浹Ÿ���� �ǰ�
		Vector2 momentum = (otherPos - m_Position) * otherSpeedModule / (35.0f * sqrtf(distance));
		collisionData = CollisionData(std::move(momentum), 0.4f, ECollisionType::HIT);
		return true;
	}

	return false;
}
