#pragma once

#include "../GameInfo.h"
#include "../Object/CollisionData.h"
#include "../Object/WallObject.h"
#include "../Object/Driver.h"

// 128 x 128 픽셀을 다시 8개의 구역으로 나눈다.
const int NUM_AREA = 8;

class CCollisionHash
{
private:
	CCollisionHash();
	~CCollisionHash();


private:
	static CCollisionHash* m_Inst;

private:
	// 총 64개의 영역이 생긴다.
	std::vector<CWallObject*> m_Static[NUM_AREA * NUM_AREA];
	std::vector<CWallObject*> m_Dynamic[NUM_AREA * NUM_AREA];

private:
	int Hash(const Vector2& pos);
	
	// 정적물체 충돌체 등록
	inline void PushStatic(const Vector2& pos, const CSharedPtr<CWallObject>& obj)
	{
		m_Static[Hash(pos)].push_back(obj);
	}

	// 동적물체 충돌체 등록
	inline void PushDynamic(const Vector2& pos, const CSharedPtr<CWallObject>& obj)
	{
		m_Dynamic[Hash(pos)].push_back(obj);
	}

public:
	bool Init();

	void RegisterStatic(const CSharedPtr<CWallObject>& obj);

	void RegisterDynamic(const CSharedPtr<CWallObject>& obj);

	void RegisterDriver(const CSharedPtr<CDriver>& obj);

	bool Collide(const CSharedPtr<CDriver>& obj, CollisionData& data);

	bool Evade(const CDriver* self, const Vector2& pos, float hitBoxRadius, Vector2& evadeVector);

	inline void ResetStatic()
	{
		for (int i = 0; i < NUM_AREA; i++)
		{
			for (int j = 0; j < NUM_AREA; j++)
			{
				m_Static[i * NUM_AREA + j].clear();
			}
		}
	}

	inline void ResetDynamic()
	{
		for (int i = 0; i < NUM_AREA; i++)
		{
			for (int j = 0; j < NUM_AREA; j++)
			{
				m_Dynamic[i * NUM_AREA + j].clear();
			}
		}
	}

public:

	inline static CCollisionHash* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CCollisionHash;

		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

