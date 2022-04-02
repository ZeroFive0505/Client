#pragma once

#include "../GameInfo.h"

enum class ECollisionType : int
{
	NO_HIT,
	HIT,
	SMASH
};

struct CollisionData
{
	Vector2 m_Momentum;
	float m_SpeedFactor;

	ECollisionType type;

	CollisionData() {}
	CollisionData(const Vector2&& momentum, const float speedFactor,
		const ECollisionType _type = ECollisionType::NO_HIT) 
		: m_Momentum(momentum), m_SpeedFactor(speedFactor), type(_type)
	{}
};