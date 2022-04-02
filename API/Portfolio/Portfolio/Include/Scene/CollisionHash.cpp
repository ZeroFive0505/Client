
#include "CollisionHash.h"
#include "../Object/Item.h"

CCollisionHash* CCollisionHash::m_Inst = nullptr;

CCollisionHash::CCollisionHash()
{
}

CCollisionHash::~CCollisionHash()
{
}

// ��ġ�� ������� �ؽ��Ѵ�.
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
	// ���� ������ü�� ��ġ�� �����´�.
	Vector2 pos = obj->GetPos();

	// ��Ʈ�ڽ��� ũ�⸦ �����´�.
	float radius = obj->GetHitBoxRadius();

	// �� 8 �������� �浹ü�� ��Ͻ�Ų��.
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
	
	// ����̹��� ��� 4�������θ� �浹ü ������Ѵ�.
	PushDynamic(pos + Vector2(radius, 0.0f), (CWallObject*)obj);
	PushDynamic(pos + Vector2(radius * -1.0f, 0.0f), (CWallObject*)obj);
	PushDynamic(pos + Vector2(0.0f, radius), (CWallObject*)obj);
	PushDynamic(pos + Vector2(0.0f, radius * -1.0f), (CWallObject*)obj);
}

// �浹 �׽�Ʈ
bool CCollisionHash::Collide(const CSharedPtr<CDriver>& obj, CollisionData& data)
{
	// ���� ��ġ
	Vector2 objPos = obj->GetPos();
	// �浹 ����
	float objRad = obj->GetHitBoxRadius();

	// ����
	Vector2 direction = Vector2(cosf(obj->GetAngle()), sinf(obj->GetAngle()));

	// �ӵ� ����
	Vector2 speedVector = direction * obj->m_SpeedForward;


	// �ӵ� ��������� �������� �����ʴ´�.
	const auto GetSquaredDist = [&objPos](const CWallObject* candidate)
	{
		Vector2 candPos = candidate->GetPos();

		float dx = objPos.x - candPos.x;
		float dy = objPos.y - candPos.y;

		return dx * dx + dy * dy;
	};

	// �ش� ��ü�� �ؽ� ��ġ���� �浹ü Ž�����Ѵ�.
	for (const auto& candidate : m_Dynamic[Hash(objPos)])
	{
		// ���� �ڱ� �ڽ��̶�� �ѱ��.
		if (candidate == obj)
			continue;

		// ��Ʈ�ڽ�
		float sum = objRad + candidate->GetHitBoxRadius();
		// �Ÿ��� ����
		float dist = GetSquaredDist(candidate);


		// �Ÿ��� ������ �Ǿ����� ��Ʈ�ڽ��� ũ�⵵ ������Ų��.
		// ���� �Ÿ��� �� ũ�ٸ� �浹������ ����.
		if (dist > sum * sum)
			continue;

		// �浹�� �Ǿ����� ���� �ӵ� ���Ϳ� ��ġ ������ ���� ���� �������� �Ÿ� ���� �����Ѵ�.
		if (candidate->SolveCollision(data, speedVector, objPos, obj->m_Kart->m_Weight, obj->GetHeight(), obj->IsImmune(), dist))
			return true;
	}

	// �̹����� ������ü�� �浹�� ó��
	for (const auto& candidate : m_Static[Hash(objPos)])
	{
		// ���� �ڱ��ڽ��̶�� �ѱ��.
		if (candidate == obj)
			continue;

		float sum = objRad + candidate->GetHitBoxRadius();
		float dist = GetSquaredDist(candidate);
	
		// ���� �Ÿ��� ��Ʈ�ڽ����� �۴ٸ� �浹
		if (dist < sum * sum && candidate->SolveCollision(data, speedVector, objPos, obj->m_Kart->m_Weight,
			obj->GetHeight(), obj->IsImmune(), dist))
			return true;
	}


	return false;
}



// ȸ�� ���͸� ������ִ� �Լ�
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
		// �ڱ��ڽ��̶�� ��ŵ
		if (candidate == self)
			continue;

		float sum = hitBoxRadius + candidate->GetHitBoxRadius();
		float dist = GetSquaredDist(candidate);

		if (dist > sum * sum)
			continue;

		// �������� ���
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
