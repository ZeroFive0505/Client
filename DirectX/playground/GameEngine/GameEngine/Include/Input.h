#pragma once

#include "GameInfo.h"

#define DIK_MOUSELBUTTON    0xfc
#define DIK_MOUSERBUTTON    0xfd
#define DIK_MOUSEWHEEL      0xfe

struct sKeyState
{
	unsigned char key;
	bool state[KeyState_Max];

	sKeyState() : state{}
	{

	}
};

struct sKeyInfo
{
	std::string name;
	sKeyState state;
	std::function<void(float)> callBack[KeyState_Max];

	bool ctrl;
	bool alt;
	bool shift;

	sKeyInfo() :
		ctrl(false),
		alt(false),
		shift(false)
	{}
};

class CInput
{
private:
	HINSTANCE m_hInst;
	HWND m_hWnd;
	IDirectInput8* m_Input;
	IDirectInputDevice8* m_Keyboard;
	IDirectInputDevice8* m_Mouse;
	unsigned char m_KeyArray[256];
	DIMOUSESTATE m_MouseState;
	Input_Type m_InputType;
	bool m_LButtonClick;
	bool m_RButtonClick;
	short m_WheelDir;

private:
	std::unordered_map<std::string, sKeyInfo*> m_mapKeyInfo;
	std::vector<sKeyState> m_vecKeyState;
	std::vector<unsigned char> m_vecAddedKey;
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;
	Vector2 m_MousePos;
	Vector2 m_MouseUIPos;
	Vector2 m_MouseWorldPos;
	Vector2 m_MouseMove;
	bool m_CollisionWidget;
	CSharedPtr<class CWidget> m_DragWidget;
	sRay m_Ray;

public:
	inline sRay GetRay() const
	{
		return m_Ray;
	}

	inline sRay GetRay(Matrix matTransform) const
	{
		sRay result;

		matTransform.Inverse();

		result.position = m_Ray.position.TransformCoord(matTransform);
		result.direction = m_Ray.direction.TransformNormal(matTransform);
		result.direction.Normalize();

		return result;
	}

	inline Vector2 GetMousePos() const
	{
		return m_MousePos;
	}

	inline Vector2 GetMouseWorld2DPos() const
	{
		return m_MouseWorldPos;
	}
	
	inline Vector2 GetMouseMove() const
	{
		return m_MouseMove;
	}

	inline bool GetMouseLButtonClick() const
	{
		return m_LButtonClick;
	}

	inline bool GetMouseRButtonClick() const
	{
		return m_RButtonClick;
	}

	inline bool GetCollisionWidget() const
	{
		return m_CollisionWidget;
	}

	class CWidget* GetDragWidget() const;

	inline short GetWheelDirection() const
	{
		return m_WheelDir;
	}

public:
	inline void SetWheelDirection(short dir)
	{
		m_WheelDir = dir;
	}

	inline void ClearWheelDirection()
	{
		m_WheelDir = 0;
	}

public:
	bool CreateKey(const std::string& name, unsigned char key);
	bool SetCtrlkey(const std::string& name, bool state);
	bool SetAltKey(const std::string& name, bool state);
	bool SetShiftKey(const std::string& name, bool state);

	void SetDragWidget(class CWidget* widget);

private:
	sKeyInfo* FindkeyInfo(const std::string& name);

private:
	bool InitWindow();
	bool InitDirectInput();

public:
	bool Init(HINSTANCE hInst, HWND hWnd);
	void Update(float deltaTime);

private:
	void ReadDirectInputKeyboard();
	void ReadDirectInputMouse();
	void UpdateMouse(float deltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float deltaTime);

public:
	void ClearCallback();
	template <typename T>
	void SetCallback(const std::string& name, Key_State state, T* obj, void(T::* func)(float))
	{
		sKeyInfo* info = FindkeyInfo(name);

		if (!info)
			return;

		info->callBack[state] = std::bind(func, obj, std::placeholders::_1);
	}

private:
	unsigned char ConvertKey(unsigned char key);

	DECLARE_SINGLE(CInput)
};

