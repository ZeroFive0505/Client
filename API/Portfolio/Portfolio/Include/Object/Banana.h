#pragma once

#include "Driver.h"
#include "Item.h"
#include "../Map/TileMap.h"


const float BANANA_SPEED = 60.0f;
const float BANANA_HITBOX_RADIUS = 0.8f;
const float BANANA_GRAVITY = -9.8f * BANANA_SPEED * 1.5f;

struct Vector3
{
	float x;
	float y;
	float z;
};

class CBanana :
	public CItem
{
private:
	Vector3 m_Speed;

public:
	CBanana(const Vector2& pos, const float forwardAngle, const bool forwardThrow, const float userHeight, CRaceScene* scene, const std::string& name);

	void Update(float deltaTime) override;

	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

	bool RegisterCollisions() const override
	{
		return true;
	}
};

