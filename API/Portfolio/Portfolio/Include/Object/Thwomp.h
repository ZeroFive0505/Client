#pragma once

#include "../Resource/Texture.h"
#include "../Map/MapInfo.h"
#include "WallObject.h"

const float THWOMP_MAXHEIGHT = 20.0f;
const float STOMP_HEIGHT = 4.0f;

/// <summary>
/// ����
/// </summary>
class CThwomp :
	public CWallObject
{
private:
	// ������ ���� ����
	enum class EThwmopState
	{
		UP,
		DOWN, 
		GOING_UP,
		GOING_DONW
	};

	EThwmopState m_State;
	// ���¸� �ٲٱ� ���� �ð� ����
	float m_Time;
	EWallObjectType m_Type;
	CSharedPtr<CTexture> m_Texture;

public:
	CThwomp(const Vector2& pos, const EWallObjectType type, CRaceScene* scene, const std::string& name);

	void Update(float deltaTime) override;

	CTexture* GetTexture();

	virtual bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance);
};

