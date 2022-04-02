#pragma once

#include "GameInfo.h"

struct KeyState
{
	int key;
	bool state[KeyState_Max];

	KeyState() :
		key(0),
		state{}
	{

	}
};

struct KeyInfo
{
	std::string name;
	KeyState state;

	std::function<void(float)> callBack[KeyState_Max];
	bool ctrl;
	bool alt;
	bool shift;

	KeyInfo() :
		ctrl(false),
		alt(false),
		shift(false)
	{

	}
};

class CInput
{
private:
	CInput();
	~CInput();

private:
	std::unordered_map<std::string, KeyInfo*> m_mapInfo;
	std::vector<KeyState> m_vecKeyState;
	std::vector<int> m_vecAddedKey;

	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	Vector2 m_MousePos;
	Vector2 m_MouseMove;
	HWND m_hWnd;

	bool m_MouseDown;
	bool m_MousePush;
	bool m_MouseUp;

public:
	inline bool GetMouseDown() const
	{
		return m_MouseDown;
	}

	inline bool GetMousePush() const
	{
		return m_MousePush;
	}

	inline bool GetMouseUp() const
	{
		return m_MouseUp;
	}

	inline Vector2 GetMousePos() const
	{
		return m_MousePos;
	}

	inline Vector2 GetMouseMove() const
	{
		return m_MouseMove;
	}
	
public:
	bool CreateKey(const std::string& name, int key);
	bool SetCtrlKey(const std::string& name, bool state);
	bool SetAltKey(const std::string& name, bool state);
	bool SetShiftKey(const std::string& name, bool state);

private:
	KeyInfo* FindKeyInfo(const std::string& name);

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);


private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);

public:
	inline bool GetKeyState(int key, EKeyState state)
	{
		if (m_vecKeyState[key].state[state])
			return true;
		else
			return false;
	}

public:
	void ClearCallback();

public:
	template <typename T>
	void SetCallback(const std::string& name, EKeyState state, T* Obj, void(T::* Func)(float))
	{
		KeyInfo* Info = FindKeyInfo(name);

		Info->callBack[state] = std::bind(Func, Obj, std::placeholders::_1);
	}

private:
	static CInput* m_Inst;

public:
	inline static CInput* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CInput;
		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}

	
public:
	void SetEditorKeySet();

	void SetPlayModeKeySet();

	void SetMenuModeKeySet();

	void SetMode7DemoKeySet();
};

