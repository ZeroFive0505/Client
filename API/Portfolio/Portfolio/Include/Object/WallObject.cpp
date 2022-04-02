#include "WallObject.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CWallObject::CWallObject(const Vector2& position,
	const float visualRadius, const float hitBoxRadius,
	const float height, const int mapWidth, const int mapHeight, CRaceScene* scene, const std::string& name) :
	m_Start(false)
{
	SetName(name);
	m_Scene = scene;
	m_VisualRadius = visualRadius / mapWidth;
	m_HitboxRadius = hitBoxRadius / mapWidth;
	m_Height = height;
	float scaleX = 1.0f / mapWidth;
	float scaleY = 1.0f / mapHeight;

	m_Position = Vector2(position.x * scaleX, position.y * scaleY);

	m_Scene->GetSceneResoruce()->LoadTexture("Shadow", TEXT("ETC/Shadow.bmp"));
	m_Shadow = m_Scene->GetSceneResoruce()->FindTexture("Shadow");
	m_Shadow->SetColorKey(255, 0, 255);
	m_Shadow->SetPivot((float)m_Shadow->GetWidth() / 2.0f, (float)m_Shadow->GetHeight());
}

CWallObject::~CWallObject()
{
}

void CWallObject::Init()
{
}

void CWallObject::Start()
{
}

void CWallObject::Update(float deltaTime)
{
}

// 벽 오브젝트간의 높이차를 체크한다.
bool CWallObject::CheckHeightDifference(const float height, const float otherHeight)
{
	return fabsf(height - otherHeight) > 2.5f;
}

// 기본 충돌 해결 함수
bool CWallObject::DefaultSolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const Vector2& myPos, const float distance)
{
	// 다른 충돌체의 속도를 구한다.
	float otherSpeedModule =
		sqrtf(otherSpeed.x * otherSpeed.x + otherSpeed.y * otherSpeed.y) + 0.05f;
	
	// 충돌체로 향하는 벡터를 만들고 속도를 곱해준다.
	Vector2 momentum = (otherPos - myPos) * otherSpeedModule / (35.0f * sqrtf(distance));
	

	// 충돌 데이터를 만든다.
	collisionData = CollisionData(std::move(momentum), 0.4f);

	return true;
}

// 충돌 체크
bool CWallObject::SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight, const float otherHeight, const bool otherIsImmune, const float distance)
{
	// 만약 높이차가 있다면 충돌하지 않는다.
	if (CWallObject::CheckHeightDifference(m_Height, otherHeight))
		return false;

	return CWallObject::DefaultSolveCollision(collisionData, otherSpeed, otherPos, m_Position, distance);
}
