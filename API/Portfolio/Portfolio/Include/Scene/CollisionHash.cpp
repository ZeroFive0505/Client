
#include "CollisionHash.h"
#include "../Object/Item.h"

CCollisionHash* CCollisionHash::m_Inst = nullptr;

CCollisionHash::CCollisionHash()
{
}

CCollisionHash::~CCollisionHash()
{
}

// 위치를 기반으로 해싱한다.
int CCollisionHash::Hash(const Vector2& pos)
{
	unsigned int x = min((unsigned int)(pos.x * NUM_AREA), NUM_AREA - 1);
	unsigned int y = min((unsigned int)(pos.y * NUM_AREA), NUM_AREA - 1);

	return y * NUM_AREA + x;
}

bool CCollisionHash::Init()
{
	for (int i = 0; i < NUM_AREA; i++)
	{
		for (int j = 0; j < NUM_AREA; j++)
		{
			m_Static[i * NUM_AREA + j].clear();

			m_Dynamic[i * NUM_AREA + j].clear();
		}
	}

	return true;
}

void CCollisionHash::RegisterStatic(const CSharedPtr<CWallObject>& obj)
{
	// 현재 정적물체의 위치를 가져온다.
	Vector2 pos = obj->GetPos();

	// 히트박스의 크기를 가져온다.
	float radius = obj->GetHitBoxRadius();

	// 총 8 방향으로 충돌체를 등록시킨다.
	PushStatic(pos + Vector2(radius, 0.0f), obj);
	PushStatic(pos + Vector2(radius * -1.0f, 0.0f), obj);
	PushStatic(pos + Vector2(0.0f, radius), obj);
	PushStatic(pos + Vector2(0.0f, radius * -1.0f), obj);

	PushStatic(pos + Vector2(radius, radius), obj);
	PushStatic(pos + Vector2(radius * -1.0f, radius), obj);
	PushStatic(pos + Vector2(radius * -1.0f, radius * -1.0f), obj);
	PushStatic(pos + Vector2(radius, radius * -1.0f), obj);
}

void CCollisionHash::RegisterDynamic(const CSharedPtr<CWallObject>& obj)
{
	Vector2 pos = obj->GetPos();

	float radius = obj->GetHitBoxRadius();
		
	PushDynamic(pos + Vector2(radius, 0.0f), obj);
	PushDynamic(pos + Vector2(radius * -1.0f, 0.0f), obj);
	PushDynamic(pos + Vector2(0.0f, radius), obj);
	PushDynamic(pos + Vector2(0.0f, radius * -1.0f), obj);

	PushDynamic(pos + Vector2(radius, radius), obj);
	PushDynamic(pos + Vector2(radius * -1.0f, radius), obj);
	PushDynamic(pos + Vector2(radius * -1.0f, radius * -1.0f), obj);
	PushDynamic(pos + Vector2(radius, radius * -1.0f), obj);
}

void CCollisionHash::RegisterDriver(const CSharedPtr<CDriver>& obj)
{
	Vector2 pos = obj->GetPos();

	float radius = obj->GetHitBoxRadius();
	
	// 드라이버의 경우 4방향으로만 충돌체 등록을한다.
	PushDynamic(pos + Vector2(radius, 0.0f), (CWallObject*)obj);
	PushDynamic(pos + Vector2(radius * -1.0f, 0.0f), (CWallObject*)obj);
	PushDynamic(pos + Vector2(0.0f, radius), (CWallObject*)obj);
	PushDynamic(pos + Vector2(0.0f, radius * -1.0f), (CWallObject*)obj);
}

// 충돌 테스트
bool CCollisionHash::Collide(const CSharedPtr<CDriver>& obj, CollisionData& data)
{
	// 현재 위치
	Vector2 objPos = obj->GetPos();
	// 충돌 범위
	float objRad = obj->GetHitBoxRadius();

	// 방향
	Vector2 direction = Vector2(cosf(obj->GetAngle()), sinf(obj->GetAngle()));

	// 속도 벡터
	Vector2 speedVector = direction * obj->m_SpeedForward;


	// 속도 향상을위해 제곱근을 쓰지않는다.
	const auto GetSquaredDist = [&objPos](const CWallObject* candidate)
	{
		Vector2 candPos = candidate->GetPos();

		float dx = objPos.x - candPos.x;
		float dy = objPos.y - candPos.y;

		return dx * dx + dy * dy;
	};

	// 해당 물체의 해싱 위치에서 충돌체 탐색을한다.
	for (const auto& candidate : m_Dynamic[Hash(objPos)])
	{
		// 만약 자기 자신이라면 넘긴다.
		if (candidate == obj)
			continue;

		// 히트박스
		float sum = objRad + candidate->GetHitBoxRadius();
		// 거리의 제곱
		float dist = GetSquaredDist(candidate);


		// 거리는 제곱이 되었으니 히트박스의 크기도 제곱시킨다.
		// 만약 거리가 더 크다면 충돌은하지 않음.
		if (dist > sum * sum)
			continue;

		// 충돌이 되었으며 현재 속도 벡터와 위치 차량의 무게 높이 무적여부 거리 등을 전달한다.
		if (candidate->SolveCollision(data, speedVector, objPos, obj->m_Kart->m_Weight, obj->GetHeight(), obj->IsImmune(), dist))
			return true;
	}

	// 이번에는 정적물체와 충돌을 처리
	for (const auto& candidate : m_Static[Hash(objPos)])
	{
		// 만약 자기자신이라면 넘긴다.
		if (candidate == obj)
			continue;

		float sum = objRad + candidate->GetHitBoxRadius();
		float dist = GetSquaredDist(candidate);
	
		// 만약 거리가 히트박스보다 작다면 충돌
		if (dist < sum * sum && candidate->SolveCollision(data, speedVector, objPos, obj->m_Kart->m_Weight,
			obj->GetHeight(), obj->IsImmune(), dist))
			return true;
	}


	return false;
}



// 회피 벡터를 계산해주는 함수
bool CCollisionHash::Evade(const CDriver* self, const Vector2& pos, float hitBoxRadius, Vector2& evadeVector)
{
	const auto GetSquaredDist = [&pos](const CWallObject* candidate)
	{
		Vector2 candPos = candidate->GetPos();
		float dx = pos.x - candPos.x;
		float dy = pos.y - candPos.y;

		return dx * dx + dy * dy;
	};

	for (const CWallObject* candidate : m_Dynamic[Hash(pos)])
	{
		// 자기자신이라면 스킵
		if (candidate == self)
			continue;

		float sum = hitBoxRadius + candidate->GetHitBoxRadius();
		float dist = GetSquaredDist(candidate);

		if (dist > sum * sum)
			continue;

		// 아이템일 경우
		const CItem* itemCandidate = dynamic_cast<const CItem*>(candidate);
		if (itemCandidate && !itemCandidate->RegisterCollisions())
			continue;
			
		CollisionData data;
		evadeVector = pos - candidate->GetPos();

		return true;
	}

	for (const auto& candidate : m_Static[Hash(pos)])
	{
		if (candidate == self)
			continue;

		float sum = hitBoxRadius + candidate->GetHitBoxRadius();
		float dist = GetSquaredDist(candidate);

		if (dist > sum * sum)
			continue;

		CollisionData data;
		evadeVector = pos - candidate->GetPos();

		return true;
	}

	return false;
}
