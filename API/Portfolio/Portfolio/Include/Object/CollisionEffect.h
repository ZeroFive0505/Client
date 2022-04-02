#pragma once

#include "../Resource/Texture.h"

#include "Item.h"

const int NUM_COLLISION_EFFECT = 4;
const float COLLISION_TIME_BETWEEN_FRAME = 0.04f;

class CCollisionEffect :
	public CItem
{
private:
	float m_CurrentTime;

	int m_CurrentFrame;

	inline void SetCurrentEffect(int frame);

public:
	CCollisionEffect(const Vector2& pos, class CRaceScene* scene, const std::string& name);

	virtual void Update(float deltaTime) override;

	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

};

