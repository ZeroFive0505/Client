
#pragma once


/*
stl : standard template library�̴�.
�迭, ����ũ�帮��Ʈ �� �ڷᱸ������ �̸� �����س��� ���̺귯���̴�.
*/


#include <Windows.h>
#include <list>		// ����ũ�帮��Ʈ ���ø� �ڷᱸ��
#include <vector>	// �����迭 ���ø� �ڷᱸ��
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
/// �浹 ������ ���� �������� ���� ����ü�̴�.
/// �� ����ü���� ���������� �̸��� � ä���� �̿��ϴ�����
/// �ٸ� ä�ΰ��� �浹�� ������ �����Ѵ�.
/// </summary>
struct CollisionProfile
{
	std::string Name;
	ECollision_Channel Channel;
	bool CollisionEnable;

	// Channel ����ŭ�� �����Ѵ�. ���� �� ���������� �ٸ� �������ϰ� �浹�ؾ� ���� ������
	// ���Ҷ� �ش� ���������� ����ϴ� ä�ΰ� Ignore���� Collision������
	// �Ǵ��Ҷ� ����Ѵ�.
	std::vector<ECollision_State> vecState;
};