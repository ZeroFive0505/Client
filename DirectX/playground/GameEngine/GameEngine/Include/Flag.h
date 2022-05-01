#pragma once

enum class Engine_Space
{
	Space2D,
	Space3D
};

enum AXIS
{
	AXIS_X,
	AXIS_Y,
	AXIS_Z,
	AXIS_MAX
};

enum class Buffer_Type
{
	Vertex,
	Index
};

enum class Shader_Type
{
	Graphic,
	Compute
};

// 상수버퍼의 타입을 지정하기 위한
// 플래그 변수들이다.
// 1을 왼쪽으로 비트 쉬프트한 값들로 이뤄져있어 간단하게 | 연산으로 온이 가능하다.
enum class Buffer_Shader_Type
{
	Vertex = 0x1,
	Pixel = 0x2,
	Domain = 0x4,
	Hull = 0x8,
	Geometry = 0x10,
	Compute = 0x20,
	Graphic = Vertex | Pixel | Domain | Hull | Geometry,
	All = Vertex | Pixel | Domain | Hull | Geometry | Compute 
};

enum class Component_Type
{
	SceneComponent,
	ObjectComponent
};

enum class Input_Type
{
	Direct,
	Window
};

enum Key_State
{
	KeyState_Down,
	KeyState_Push,
	KeyState_Up,
	KeyState_Max
};

enum class Image_Type
{
	Atlas,
	Frame,
	Array
};


// 텍스처를 스케일링 샘플링 방법
enum class Sampler_Type
{
	Point, // 품질은 최하, 성능은 최대
	Linear, // 품질 성능 둘다 중간
	Anisotropic // 품질은 최고 성능은 최하
};

enum class RenderState_Type
{
	Blend,
	Resterizer,
	DepthStencil,
	MAX
};

enum class SceneComponent_Type
{
	Sprite,
	StaticMesh,
	Box2DCollider,
	CircleCollider,
	PixelCollider,
	Camera,
	Widget,
	Particle,
	TileMap,
	MAX
};

enum class Collision_Channel
{
	Object,
	Player,
	Enemy,
	PlayerAttack,
	EnemyAttack,
	PlayerBottom,
	EnemyBottom,
	PlayerContactBox,
	EnemyContactBox,
	Wall,
	SideWall,
	Custom7,
	Custom8,
	Custom9,
	Custom10,
	Custom11,
	Custom12,
	Custom13,
	Custom14,
	Custom15,
	Custom16,
	MAX
};

enum class Collision_Interaction
{
	Ignore,
	Collision
};

enum class Collision_State
{
	Begin,
	End,
	Max
};

enum class Collider_Type
{
	Box2D,
	Circle,
	Pixel
};

enum class Camera_Type
{
	Camera2D,
	Camera3D,
	CameraUI
};

enum class PixelCollision_Type
{
	Color_Ignore,
	Color_Confirm,
	Alpha_Ignore,
	Alpha_Confirm
};

enum class Button_State
{
	Normal, 
	MouseOn,
	Click,
	Disable,
	Max
};

enum class Button_Sound_State
{
	MouseOn,
	Click,
	Max
};

enum class WidgetComponent_Space
{
	Screen,
	World
};

enum class ProgressBar_Direction
{
	RightToLeft,
	LeftToRight,
	TopToBottom,
	BottomToTop
};

enum class Mouse_State
{
	Normal,
	State1,
	State2,
	State3,
	State4,
	State5,
	State6,
	State7,
	State8,
	State9,
	State10,
	MAX
};

enum class Tile_Shape
{
	Rect,
	Rhombus,
	RectWire,
	End
};

enum class Tile_Type
{
	Normal,
	Wall,
	SideWall,
	Pilar,
	End
};

enum class Nav_Node_Type
{
	None,
	Open,
	Close
};

enum class Mesh_Type
{
	Sprite,
	Static,
	Animation
};

enum class Light_Type
{
	Directional,
	Point,
	Spot,
	End
};