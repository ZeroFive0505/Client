#pragma once

#include "../GameInfo.h"
#include "../Object/CollisionData.h"
#include "../Object/WallObject.h"
#include "../Object/Driver.h"

// 128 x 128 �ȼ��� �ٽ� 8���� �������� ������.
const int NUM_AREA = 8;

class CCollisionHash
{
private:
	CCollisionHash();
	~CCollisionHash();


private:
	static CCollisionHash* m_Inst;

private:
	// �� 64���� ������ �����.
	std::vector<CWallObject*> m_Static[NUM_AREA * NUM_AREA];
	std::vector<CWallObject*> m_Dynamic[NUM_AREA * NUM_AREA];

private:
	int Hash(const Vector2& pos);
	
	// ������ü �浹ü ���
	inline void PushStatic(const Vector2& pos, const CSharedPtr<CWallObject>& obj)
	{
		m_Static[Hash(pos)].push_back(obj);
	}

	// ������ü �浹ü ���
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

