#pragma once

#include "../Resource/Texture.h"

#include "Item.h"

	
const float COIN_JUMP_SPEED = 110.0f;
	
const float COIN_GRAVITY = -9.8f * 35.0f;
	
const int NUM_COIN_TEXTURES = 3;

const float COIN_TIME_BETWEEN_FRAMES = 0.05f;

class CCoinEffect :
	public CItem
{
private:

	const CDriver* m_Driver;

	float m_RelativeHeight;

	float m_Delay;

	bool m_Positive;

	float m_VerticalSpeed;

	float m_SpeedForward;
	float m_Angle;

	float m_CurrentTime;

	int m_CurrentFrame;

	bool m_Once;

	inline void SetCurrentCoinTexture(int frame);

public:
	CCoinEffect(const CDriver* driver, class CRaceScene* scene, const std::string& name, const float delay, const bool positive);

	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;


	virtual void Init() override;
	virtual void Update(float deltaTime) override;
};

