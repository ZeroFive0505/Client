#pragma once

#include "../Map/MapInfo.h"
#include "../GameInfo.h"
#include "WallObject.h"

/// <summary>
/// »µ²û»µ²û ÇÃ¶ó¿ö
/// </summary>
class CFlower : 
	public CWallObject
{
private:
	// ¾Ö´Ï¸ÞÀÌ¼ÇÀ» À§ÇÑ ÅØ½ºÃÄ
	CSharedPtr<CTexture> m_CurrentTexture;
	CSharedPtr<CTexture> m_Textures[2];
	EWallObjectType m_Type;

	// 1.5ÃÊ¸¶´Ù ¹Ù²ï´Ù.
	const float m_FlowerAnimTime = 1.5f;

	float m_fElasedTime;

	int m_TexIndex;

public:
	CFlower(const Vector2& pos, const EWallObjectType type, class CRaceScene* scene, const std::string& name);

	void Update(float deltaTime) override;

	CTexture* GetTexture();

	virtual bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float ohterWeight,
		const float otherHeight, const bool otherIsImmune, const float distance);
};

