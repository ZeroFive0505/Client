#pragma once

#include "../Map/MapInfo.h"
#include "Driver.h"
#include "Item.h"

const int GREENSHELL_NUM_LIVES = 6;
const int GREENSHELL_NUM_MARCHES_UPDATE = 3;
const int GREENSHELL_NUM_MARCHES_HIT = 20;
const float GREENSHELL_SPEED = 80.0f;
const float GREENSHELL_HITBOX_RADIUS = 5.0f;
const float GREENSHELL_JUMP_SPEED = 150.0f;
const float GREENSHELL_GRAVITY = -9.8f * 100.0f;
const float GREENSHELL_FLIGHTTIME = 1.0f;

class CGreenShell :
	public CItem
{
private:
	Vector2 m_Speed;
	float m_VerticalSpeed;
	int m_Lives;

public:
	CGreenShell(const Vector2& pos, const float forwardAngle, const bool forwardThrow, const float userHeight, class CRaceScene* scene, const std::string& name);

	void Update(float deltaTime) override;

	void MarchingUpdate(float deltaTime);

	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

	void Init();

	CTexture* GetTexture() override;

	inline bool RegisterCollisions() const override
	{
		return true;
	}
};

