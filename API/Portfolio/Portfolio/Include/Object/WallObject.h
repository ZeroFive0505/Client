#pragma once

#include "../GameInfo.h"
#include "CollisionData.h"
#include "../Resource/Texture.h"
#include "../Ref.h"

/// <summary>
/// �÷��̾ �� ��ֹ� ���ع� �������� ����� �Ǵ� Ŭ����
/// </summary>
class CWallObject :
	public CRef
{
	friend class CRaceScene;
protected:
	// �׸���
	CSharedPtr<CTexture> m_Shadow;

	// ���̴� ����
	float m_VisualRadius;

	// �ǰ� ������ �ʺ�
	float m_HitboxRadius;

	// ����
	float m_Height;

	// ���� ��ġ
	Vector2 m_Position;

	class CRaceScene* m_Scene;

	bool m_Start;

public:
	CWallObject(const Vector2& position, const float visualRadius, const float hitBoxRadius, const float height,
		const int mapWidth, const int mapHeight, class CRaceScene* scene, const std::string& name);
	virtual ~CWallObject();

public:

	virtual void Init();
	virtual void Start();
	virtual void Update(float deltaTime);

	virtual CTexture* GetTexture() = 0;

	// ���� �׸��ڸ� ��ȯ�Ѵ�.
	inline CTexture* GetShadow()
	{
		CTexture* cloneTexture = m_Shadow->Clone();
		cloneTexture->SetColorKey(255, 0, 255);
		cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

		cloneTexture->SetColorKey(255, 0, 255);

		return cloneTexture;
	}

	inline float GetVisualRadius() const
	{
		return m_VisualRadius;
	}

	inline Vector2 GetPos() const
	{
		return m_Position;
	}

	inline void SetPos(const Vector2& pos)
	{
		m_Position = pos;
	}

	inline float GetHitBoxRadius() const
	{
		return m_HitboxRadius;
	}

	inline float GetHeight() const
	{
		return m_Height;
	}

	inline class CRaceScene* GetScene() const
	{
		return m_Scene;
	}

	inline void SetScene(class CRaceScene* scene)
	{
		m_Scene = scene;
	}

	inline void SetHeight(float height)
	{
		m_Height = height;
	}


	bool CheckHeightDifference(const float height, const float otherHeight);

	bool DefaultSolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const Vector2& myPos,
		const float distance);

	virtual bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance);
};

