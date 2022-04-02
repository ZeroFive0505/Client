#pragma once

#include "../GameInfo.h"
#include "../Map/TileMap.h"
#include "../Map/MapInfo.h"

class CAIGradientDescent
{
public:
	static const int WALL_PENALTY_MAX = 4096;
	static const int WALL_PENALTY_FACTOR = 2;
	static const int WALL_PENALTY_ITERS = 3;

	static int GradientMatrix[MAP_TILES_HEIGHT][MAP_TILES_WIDTH];
	static int PositionMatrix[MAP_TILES_HEIGHT][MAP_TILES_WIDTH];

	static const std::array<Vector2, 8> EightNeibours;

	static int WeightTile(const ETileProperty tileType);

public:
	static int GRADIENT_LAP_CHECK;
	static int MAX_POSITION_MATRIX;

	static int MAX_DISTANCE_BEHIND[(int)ECCOption::END];
	static float MIN_PROB_BEHIND[(int)ECCOption::END];
	static int MAX_DISTANCE_AHEAD[(int)ECCOption::END];
	static float MIN_PROB_AHEAD[(int)ECCOption::END];

	static void SetGradient(const std::vector<class CTile*>& vecTiles, const Vector2& goalStartPos, const Vector2& goalSize, const std::string& positionsPath,
		const std::string& gradientPath);

	static int GetPositionValue(unsigned int col, unsigned int row);
	static int GetPositionValue(const Vector2& pos);

	static Vector2 GetNextDirection(const Vector2& pos);
};

