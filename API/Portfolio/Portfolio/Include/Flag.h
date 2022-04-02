#pragma once

enum EKeyState
{
	KeyState_Down,
	KeyState_Hold,
	KeyState_Up,
	KeyState_Max
};

enum class ETexture_Type
{
	Atlas,
	Frame
};

enum class ETileEditMode
{
	PROPERTY,
	WALL,
	FLOOR,
	END
};