#pragma once

#include "../GameInfo.h"

const int MAP_ASSETS_WIDTH = 1024;
const int MAP_ASSETS_HEIGHT = 1024;
const int MAP_TILE_SIZE = 8;
const int MAP_TILES_WIDTH = MAP_ASSETS_WIDTH / MAP_TILE_SIZE;
const int MAP_TILES_HEIGHT = MAP_ASSETS_WIDTH / MAP_TILE_SIZE;
const int MAP_EDGES_SIZE = 8;


// 일반 트랙, 벽, 느려지는 타일까지는 타일 옵션으로 설정이 가능하다.
enum class ETileProperty
{
	TRACK,
	BLOCK,
	SLOW,
	SLOW_2,
	SPECIAL_13H,
	HAZARD,
	RAMP,
	OIL_SLICK,
	RAMP_HORIZONTAL,
	RAMP_VERTICAL,
	ACCEL_PANEL,
	OTHER,
	END
};

enum class ETileMaterial
{
	DIRT,
	GRASS,
	SPOOKY_WOOD,
	STONE,
	WATER,
	LAVA,
	EMPTY,
	SWAMP,
	RAINBOW,
	END
};

enum class EFloorObjectType
{
	ACCEL_PANEL,
	ITEM_BOX,
	OIL_SLICK,
	COIN,
	RAMP_HORIZONTAL,
	RAMP_VERTICAL,
	END
};

enum class EFloorObjectOrientation
{
	UP,
	RIGHT,
	DOWN,
	LEFT,
	END
};

enum class EFloorObjectState
{
	INITIAL,
	NONE,
	ACTIVE,
	INACTIVE,
	END
};

enum class EWallObjectType
{
	GREEN_PIPE,
	ORNAGE_PIPE,
	SUPER_THWOMP,
	THWOMP,
	FLOWER,
	END
};