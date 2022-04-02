#pragma once

#include "GameInfo.h"

struct sLoadingMessage
{
	bool complete;
	float percent;
};

enum class EMoveDirection : int
{
	UP = 1,
	DOWN = 2,
	LEFT = 4,
	RIGHT = 8,
	NONE = 16
};

enum class EKyokoState : int
{
	NORMAL = 0,
	GETHIT = 1,
	STUNNED = 2,
	KNOCKDOWN = 4,
	BLOWNBACK = 8,
	DOWN = 16,
	ATTACK = 32,
	GRABBED = 64,
	GRABBING = 128,
	POGOKICK = 256,
	INVINCIBLE = 512,
	MAX = 11
};

enum class EEnemyState : int
{
	NORMAL = 0,
	GETHIT = 1,
	STUNNED = 2,
	DOWN = 4,
	ATTACK = 8,
	KNOCKDOWN = 16,
	BLOWNBACK = 32,
	GRABBED = 64,
	GRABBING = 128,
	BEGGING = 256,
	DEFEADTED = 512,
	PARALYSIS = 1024,
	MAX = 12
};

enum class EHitFlag : int
{
	NONE = 1,
	HIT = 2
};

enum class EPlayerKeySet : int
{
	NONE = 0,
	WEAKATTACK = 1,
	HEAVYATTACK = 2,
	SPECIAL = 4
};

enum class EAttackType : int
{
	HIT = 0,
	KNOCKDOWN = 1,
	BLOWNBACK = 2
};