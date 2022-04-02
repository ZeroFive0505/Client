
#pragma once


/*
stl : standard template library이다.
배열, 더블링크드리스트 등 자료구조들을 미리 구현해놓은 라이브러리이다.
*/


#include <Windows.h>
#include <list>		// 더블링크드리스트 템플릿 자료구조
#include <vector>	// 동적배열 템플릿 자료구조
#include <unordered_map>
#include <stack>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <algorithm>
#include "resource.h"


#include "SharedPtr.h"
#include "Math.h"
#include "Flag.h"
#include "fmod.hpp"

#pragma comment(lib, "msimg32.lib")

#pragma comment(lib, "fmod64_vc.lib")

#define KEYCOUNT_MAX 256

#define GRAVITY 9.8f

#define ROOT_PATH "RootPath"
#define TEXTURE_PATH "TexturePath"
#define SOUND_PATH "SoundPath"
#define FONT_PATH "FontPath"
#define MAP_PATH "MapPath"

#define	SAFE_DELETE(p)	if(p)	{ delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p)	{ p->Release(); p = nullptr; }

struct Resolution
{
	int	Width;
	int	Height;
};

struct RectInfo
{
	float	Left;
	float	Top;
	float	Right;
	float	Bottom;
};

struct AnimationFrameData
{
	Vector2	StartPos;
	Vector2	Size;
};

struct SphereInfo
{
	Vector2 Center;

	float Radius;
};

struct CharacterInfo
{
	int	Attack;
	int	Armor;
	int	HP;
	int	HPMax;
	int	MP;
	int	MPMax;
	int	Level;
	int	Exp;
	int	Gold;
	float	AttackSpeed;
	float	AttackDistance;
};

/// <summary>
/// 충돌 설정을 위한 프로파일 정보 구조체이다.
/// 이 구조체에는 프로파일의 이름과 어떤 채널을 이용하는지와
/// 다른 채널과의 충돌의 유무를 설정한다.
/// </summary>
struct CollisionProfile
{
	std::string Name;
	ECollision_Channel Channel;
	bool CollisionEnable;

	// Channel 수만큼을 저장한다. 현재 이 프로파일이 다른 프로파일과 충돌해야 할지 말지를
	// 비교할때 해당 프로파일이 사용하는 채널과 Ignore인지 Collision인지를
	// 판단할때 사용한다.
	std::vector<ECollision_State> vecState;
};