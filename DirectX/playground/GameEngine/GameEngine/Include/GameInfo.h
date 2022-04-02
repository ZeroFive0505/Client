#pragma once

#define	DIRECTINPUT_VERSION	0x0800

#include <Windows.h>
#include <list>
#include <vector>
#include <stack>
#include <unordered_map>
#include <crtdbg.h>
#include <functional>
#include <string>
#include <random>
#include <math.h>
#include <algorithm>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <ShlObj.h>
#include <dwrite.h>
#include <dwrite_3.h>
#include <d2d1.h>
#include <process.h>
#include <time.h>


#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix.h"
#include "SharedPtr.h"
#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "fmod.hpp"

#include "Resource/Texture/DirectXTex.h"

#ifdef _DEBUG

#pragma comment(lib, "DirectXTex_Debug.lib")

#else

#pragma comment(lib, "DirectXTex.lib")

#endif // _DEBUG

#pragma comment(lib, "fmod64_vc.lib")

#define ROOT_PATH "Root"
#define SHADER_PATH "Shader"
#define TEXTURE_PATH "Texture"
#define FONT_PATH "Font"
#define ANIMATION_PATH "Animation"
#define SCENE_PATH "Scene"
#define SOUND_PATH "Sound"

#define SAFE_DELETE(p) if(p) { delete p; p = nullptr; }
#define SAFE_DELETE_ARRAY(p) if(p) { delete[] p; p = nullptr; }
#define SAFE_RELEASE(p) if(p) { p->Release(); p = nullptr; }

#define	DECLARE_SINGLE(Type)	\
private:\
	static Type*	m_Inst;\
public:\
	static Type* GetInst()\
	{\
		if(!m_Inst)\
			m_Inst = new Type;\
		return m_Inst;\
	}\
	static void DestroyInst()\
	{\
		SAFE_DELETE(m_Inst);\
	}\
private:\
	Type();\
	~Type();

#define	DEFINITION_SINGLE(Type)	Type* Type::m_Inst = nullptr;


struct sResolution
{
	unsigned int width;
	unsigned int height;
};

// ���ؽ��� Į�� ������ ���ÿ� ������ �ִ� ���ؽ� ������ ����ü
struct sVertexColor
{
	Vector3 Pos;
	Vector4 Color;

	sVertexColor() {}

	sVertexColor(const Vector3& _pos, const Vector4& _color) :
		Pos(_pos),
		Color(_color)
	{

	}
};


// ���ؽ� ����
struct sVertexBuffer
{
	// ����
	ID3D11Buffer* Buffer;
	// ���ؽ� �ϳ��� ������ �ִ� �������� ũ��
	int Size;
	// �� ��� �ִ���
	int Count;

	sVertexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0)
	{}

	~sVertexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};

// ���ؽ��� �׸��� � ������ �׸��� �ε����� �ʿ��ϴ�.
// ���� �Ʒ��� �ε��� ������ �����͸� ���� ����ü�� �����.
struct sIndexBuffer
{
	// ����
	ID3D11Buffer* Buffer;
	// ũ��
	int Size;
	// ����
	int Count;
	// ����
	DXGI_FORMAT Fmt;

	sIndexBuffer() :
		Buffer(nullptr),
		Size(0),
		Count(0),
		Fmt(DXGI_FORMAT_UNKNOWN)
	{

	}

	~sIndexBuffer()
	{
		SAFE_RELEASE(Buffer);
	}
};


// �޽����� ������ ��� �����̳�
struct sMeshContainer
{
	// ���ؽ� ���۴� �ϳ��� ����
	sVertexBuffer VB;
	// �ε��� ���۴� �������� ���� �� �ִ�.
	std::vector<sIndexBuffer> vecIB;
	// ������ �׸��� ����� ����
	D3D11_PRIMITIVE_TOPOLOGY Primitive;
};

// ������۸� �̿��Ͽ� ���̴��� ���������ؼ� ������� ����ü
// HLSL ShaderInfo.fx�� Transform�� ���� ������ ���������.
// 4����Ʈ ������ �������
struct sTransformCBuffer
{
	Matrix matWorld;

	Matrix matView;

	Matrix matProj;

	Matrix matWV;

	Matrix matWVP;

	Matrix matVP;

	Vector3 pivot;
	float empty1;

	Vector3 meshSize;
	float empty2;
};

struct sVertexUV
{
	Vector3 pos;
	Vector2 uv;

	sVertexUV()
	{

	}

	sVertexUV(const Vector3& _pos, const Vector2& _uv) :
		pos(_pos),
		uv(_uv)
	{

	}
};

struct sMaterialCBuffer
{
	Vector4 baseColor;
	float opacity;
	int paperBurnEnable;
	Vector2 empty;

	sMaterialCBuffer() : 
		baseColor(1.0f, 1.0f, 1.0f, 1.0f),
		opacity(1.0f), 
		paperBurnEnable(0),
		empty(1.0f, 1.0f)
	{}
};

// �ִϸ��̼��� ����
struct sAnimationFrameData
{
	Vector2 start;
	Vector2 size;
};

// �ִϸ��̼��� ������ġ
struct sAnimation2DCBuffer
{
	Vector2 Animation2DStartUV;
	Vector2 Animation2DEndUV;
	int Animation2DType;
	int AnimationHorizontalFlip;
	Vector2 AnimationSize;
};

// ���̴��� ���۸� �����Ҷ� �ִϸ��̼��� �� ���� ������ �����ϴ� ���ۿ� ����
struct sStandard2DCBuffer
{
	int AnimationEnable;
	Vector3 Empty;
};

// ���� ������Ʈ�� ��� ������Ʈ���� �����ö� ���Ǵ� ����ü
struct sFindComponentName
{
	std::string name;
	std::string parentName;
};

// �浹 �������� ����ü
struct sCollisionProfile
{
	// ���������� �̸�
	std::string name;
	// ���� �� ���������� � ä�ο� �����ִ���
	Collision_Channel channel;
	bool collisionEnable;

	// ��� �ٸ� ä�ΰ��� �浹 ���¸� ����
	std::vector<Collision_Interaction> vecInteraction;
};

// �浹��� �ҽ� ������Ʈ�� �浹 ������Ʈ �׸��� �浹����
struct sCollisionResult
{
	class CColliderComponent* src;
	class CColliderComponent* dest;

	Vector3 hitPoint;

	sCollisionResult() :
		src(nullptr),
		dest(nullptr)
	{

	}
};

struct sBox2DInfo
{
	// �簢���� �߽�
	Vector2 center;
	// �簢���� ���� ������ ��
	Vector2 axis[2];
	// �簢���� �ʺ�� ������ ��
	Vector2 length;
	// �� �簢���� �ּ������� �ִ� ����
	Vector2 Min;
	Vector2 Max;
};

struct sCircleInfo
{
	Vector2 center;
	float radius;
	Vector2 Min;
	Vector2 Max;
};

struct sPixelInfo
{
	unsigned char* pixels;
	unsigned int width;
	unsigned int height;
	PixelCollision_Type type;
	unsigned char colors[4];
	sBox2DInfo box;
	Vector2 Min;
	Vector2 Max;
	ID3D11ShaderResourceView* SRV;
	int refCount;

	sPixelInfo() :
		refCount(1),
		pixels(nullptr),
		SRV(nullptr),
		width(0),
		height(0),
		box{},
		colors{},
		type(PixelCollision_Type::Color_Confirm)
	{}
};

struct sColliderCBuffer
{
	Vector4 color;
	Matrix matWVP;
};

struct sWidgetCBuffer
{
	Vector4 tint;
	Matrix matWP;
	int useTexture;
	int widgetAnimType;
	Vector2 widgetAnimStartUV;
	Vector2 widgetAnimEndUV;
	int widgetAnimEnable;
	float opacity;
};

struct sProgressBarCBuffer
{
	float percent;
	int direction;
	Vector2 empty;
};

struct sParticleCBuffer
{
	unsigned int spawnEnable;
	Vector3 startMin;
	Vector3 startMax;
	unsigned int spawnCountMax;
	Vector3 scaleMin;
	float lifeTimeMin;
	Vector3 scaleMax;
	float lifeTimeMax;
	Vector4 colorMin;
	Vector4 colorMax;
	float speedMin;
	float speedMax;
	int isMoving;
	int gravityEnable;
	Vector3 moveDir;
	int is2D;
	Vector3 moveAngle;
	float empty;
};

struct sParticleInfo
{
	Vector3 worldPos;
	Vector3 dir;
	float speed;
	float lifeTime;
	float lifeTimeMax;
	int alive;
	float fallTime;
	float fallStartY;
};

struct sParticleInfoShared
{
	unsigned int spawnEnable;
	Vector3 scaleMin;
	Vector3 scaleMax;
	Vector4 colorMin;
	Vector4 colorMax;
	int gravityEnable;
};

struct sGlobalCBuffer
{
	float deltaTime;
	float accTime;
	Vector2 resolution;
	Vector2 noiseResolution;
	Vector2 empty;
};

struct sPaperBurnCBuffer
{
	Vector4 inlineColor;
	Vector4 outlineColor;
	Vector4 centerlineColor;
	float filter;
	int inverse;
	float inFilter;
	float outFilter;
	float centerFilter;
	Vector3 empty;
};

struct sTileMapCBuffer
{
	Vector2 imageSize;
	Vector2 size;
	Vector2 start;
	Vector2 end;
	Matrix matWVP;
	int wire;
	Vector3 empty;
};

struct sTileInfo
{
	Matrix matWVP;
	Vector2 tileStart;
	Vector2 tileEnd;
	Vector4 tileColor;
	float opacity;
	Vector3 empty;
};

struct sNavWorkData
{
	std::function<void(const std::list<Vector3>&)> callBack;
	Vector3 start;
	Vector3 end;
};

struct sNavResultData
{
	std::function<void(const std::list<Vector3>&)> callBack;
	std::list<Vector3> path;
};