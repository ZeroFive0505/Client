#pragma once

#include "../Resource/Texture.h"
#include "Driver.h"
#include "Item.h"
#include "../Map/TileMap.h"

const float REDSHELL_SPEED = 75.0f;
const float REDSHELL_HITBOX_RADIUS = 4.0f;
const float REDSHELL_MAXHEIGHT = 8.0f;
const float REDSHELL_FLIGHTTIME = 0.5f;

class CRedShell :
	public CItem
{
private:
	static unsigned int m_NumRedShellsOnPlayer;
	bool m_FollowingPlayer;
	const CDriver* m_Target;
	int m_InactiveFrames;
	Vector2 m_Speed;
	float m_RemainingFlightTime;
	Vector2 m_LastDirection;

	int m_GradientWhenRamp;

public:
	CRedShell(const Vector2& pos, const CDriver* target, const float forwardAngle, const bool forwardThrow,
		const float userHeight, class CRaceScene* scene, const std::string& name);
	~CRedShell();

	void Update(float deltaTime) override;


	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

	bool RegisterCollisions() const override
	{
		return true;
	}
};

