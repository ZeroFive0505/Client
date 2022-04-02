#pragma once

// 기본적으로 공유하는 헤더들을 포함하는 장소
#define _CRTDBG_MAP_ALLOC
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <list>
#include <vector>
#include <array>
#include <string>
#include <unordered_map>
#include <stdlib.h>
#include <crtdbg.h>
#include <functional>
#include <assert.h>
#include <time.h>
#include "resource.h"

#include "Flag.h"
#include "SharedPtr.h"
#include "Math.h"
#include "fmod.hpp"

#pragma comment(lib, "msimg32.lib")

#pragma comment(lib, "fmod64_vc.lib")

#define KEYCOUNT_MAX 256

#define ROOT_PATH "RootPath"
#define TEXTURE_PATH "TexturePath"
#define SOUND_PATH "SoundPath"
#define FONT_PATH "FontPath"
#define MAP_PATH "MapPath"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) {p->Release(); p = nullptr; }

struct Resolution
{
	int width;
	int height;
};

struct RectInfo
{
	float left;
	float top;
	float right;
	float bottom;

	RectInfo() :
		left(0.0f),
		top(0.0f),
		right(0.0f),
		bottom(0.0f)
	{

	}
};

struct AnimationFrameData
{
	Vector2 startPos;
	Vector2 size;
};

const float BASIC_WIDTH = 512.0f;
const float BASIC_HEIGHT = 448.0;

const int MAX_LAPS = 5;

enum class ECupSelect
{
	MUSHROOM,
	FLOWER,
	STAR,
	SEPCIAL
};

enum class ECCOption
{
	CC50,
	CC100,
	CC150,
	CC200,
	END
};