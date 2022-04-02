#pragma once

#include "GameInfo.h"
#include "UI/UIImage.h"

enum EMouse_Type
{
	Mouse_Default,
	Mouse_Battle,
	Mouse_End
};

/// <summary>
/// 현재 키의 상태를 저장한다.
/// 키의 상태는 Down, Push, Up으로 세가지 경우의 수가있다.
/// Key에는 현재 누른키의 아스키코드값이 들어간다.
/// </summary>
struct KeyState
{
	int Key;
	bool State[KeyState_Max];

	KeyState() :
		Key(0),
		State{}
	{

	}
};

/// <summary>
/// Name으로 현재키의 이름을 정한다. 예) MoveUp, MoveLeft 등등..
/// State 변수는 현재 키의 상태 방금 눌렀는지, 쭉 누르고 있었는지와 방금 뗐는지를 검사한다.
/// 콜백함수를 변수로 가질 수 있게해서 클라이언트쪽에서 원하는 함수를 호출 할 수 있도록한다.
/// 추가적으로 Ctrl, Alt, Shift등의 키를 bool 변수로 가지고있어서 같이 눌러여만 작동이 되는 키도 설징이 가능하도록 한다.
/// </summary>
struct KeyInfo
{
	std::string Name;
	KeyState State;
	// return type은 void이고 함수의 인자는 float타입 하나를 받는
	// 함수의 주소를 저장할 수 있게 한다.
	std::function<void(float)> Callback[KeyState_Max];
	bool Ctrl;
	bool Alt;
	bool Shift;

	KeyInfo() :
		Ctrl(false),
		Alt(false),
		Shift(false)
	{

	}
};

/// <summary>
/// Input은 싱글톤으로 게임 매니저가 호출되고 초기화됬을때 같이 초기화된다.
/// </summary>
class CInput
{
private:
	CInput();
	~CInput();

private:
	// 해쉬 맵으로 호출할 이름과 키의 상태를 같이 묶어서 저장한다.
	std::unordered_map<std::string, KeyInfo*> m_mapInfo;
	// 현재 키들의 상태를 저장한다.
	std::vector<KeyState> m_vecKeyState;
	// 추가된 키의 아스키코드 값을 여기에 저장한다.
	std::vector<int> m_vecAddKey;

	// 현재 플레이어가 Ctrl, Alt, Shift키를 눌렀는지
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	// 마우스
	Vector2 m_MousePos;
	Vector2 m_MouseMove;
	HWND m_hWnd;


	bool m_MouseDown;
	bool m_MousePush;
	bool m_MouseUp;

	bool m_ShowCursor;

	std::vector<CSharedPtr<CUIImage>> m_vecMouseImage;
	EMouse_Type m_MouseType;

public:
	bool IsShowCursor() const
	{
		return m_ShowCursor;
	}

	bool GetMouseDown()
	{
		return m_MouseDown;
	}

	bool GetMousePush()
	{
		return m_MousePush;
	}

	bool GetMouseUp()
	{
		return m_MouseUp;
	}

	Vector2 GetMousePos()
	{
		return m_MousePos;
	}

	Vector2 GetMouseMove()
	{
		return m_MouseMove;
	}

	void ChangeMouse(EMouse_Type Type)
	{
		m_MouseType = Type;
	}

	void SetShowCursor(bool Cursor)
	{
		m_ShowCursor = Cursor;
	}

public:
	// 키를 생성한다. Name은 해쉬맵의 키로 들어간다.
	bool CreateKey(const std::string& Name, int Key);
	bool SetCtrlKey(const std::string& Name, bool State);
	bool SetAltKey(const std::string& Name, bool State);
	bool SetShiftKey(const std::string& Name, bool State);

private:
	// 해쉬맵을 이용해서 현재 Name으로 등록된 키가 있는지 찾느다.
	KeyInfo* FindKeyInfo(const std::string& Name);

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void Render(HDC hDC);

private:
	void UpdateMouse(float DeltaTime);
	void UpdateKeyState();
	void UpdateKeyInfo(float DeltaTime);

public:
	void ClearCallback();

public:
	/// <summary>
	/// 키의 콜백함수를 정해주는 함수이다. 탬플릿을 이용하여 호출자의 타입을 같이 전달해주도록 한다.
	/// </summary>
	/// <typeparam name="T">호출자의 타입</typeparam>
	/// <param name="Name">키의 이름</param>
	/// <param name="State">키의 상태</param>
	/// <param name="Obj">호출자의 객체</param>
	/// <param name="Func">호출자의 맴버함수 포인터</param>
	template <typename T>
	void SetCallback(const std::string& Name, EKeyState State, T* Obj, void(T::* Func)(float))
	{
		KeyInfo* Info = FindKeyInfo(Name);

		// std::bind : 함수포인터를 이용하여 function 객체를 만들어준다.
		Info->Callback[State] = std::bind(Func, Obj, std::placeholders::_1);
	}

private:
	static CInput* m_Inst;

public:
	static CInput* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CInput;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

