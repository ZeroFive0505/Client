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
/// ���� Ű�� ���¸� �����Ѵ�.
/// Ű�� ���´� Down, Push, Up���� ������ ����� �����ִ�.
/// Key���� ���� ����Ű�� �ƽ�Ű�ڵ尪�� ����.
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
/// Name���� ����Ű�� �̸��� ���Ѵ�. ��) MoveUp, MoveLeft ���..
/// State ������ ���� Ű�� ���� ��� ��������, �� ������ �־������� ��� �ô����� �˻��Ѵ�.
/// �ݹ��Լ��� ������ ���� �� �ְ��ؼ� Ŭ���̾�Ʈ�ʿ��� ���ϴ� �Լ��� ȣ�� �� �� �ֵ����Ѵ�.
/// �߰������� Ctrl, Alt, Shift���� Ű�� bool ������ �������־ ���� �������� �۵��� �Ǵ� Ű�� ��¡�� �����ϵ��� �Ѵ�.
/// </summary>
struct KeyInfo
{
	std::string Name;
	KeyState State;
	// return type�� void�̰� �Լ��� ���ڴ� floatŸ�� �ϳ��� �޴�
	// �Լ��� �ּҸ� ������ �� �ְ� �Ѵ�.
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
/// Input�� �̱������� ���� �Ŵ����� ȣ��ǰ� �ʱ�ȭ������ ���� �ʱ�ȭ�ȴ�.
/// </summary>
class CInput
{
private:
	CInput();
	~CInput();

private:
	// �ؽ� ������ ȣ���� �̸��� Ű�� ���¸� ���� ��� �����Ѵ�.
	std::unordered_map<std::string, KeyInfo*> m_mapInfo;
	// ���� Ű���� ���¸� �����Ѵ�.
	std::vector<KeyState> m_vecKeyState;
	// �߰��� Ű�� �ƽ�Ű�ڵ� ���� ���⿡ �����Ѵ�.
	std::vector<int> m_vecAddKey;

	// ���� �÷��̾ Ctrl, Alt, ShiftŰ�� ��������
	bool m_Ctrl;
	bool m_Alt;
	bool m_Shift;

	// ���콺
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
	// Ű�� �����Ѵ�. Name�� �ؽ����� Ű�� ����.
	bool CreateKey(const std::string& Name, int Key);
	bool SetCtrlKey(const std::string& Name, bool State);
	bool SetAltKey(const std::string& Name, bool State);
	bool SetShiftKey(const std::string& Name, bool State);

private:
	// �ؽ����� �̿��ؼ� ���� Name���� ��ϵ� Ű�� �ִ��� ã����.
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
	/// Ű�� �ݹ��Լ��� �����ִ� �Լ��̴�. ���ø��� �̿��Ͽ� ȣ������ Ÿ���� ���� �������ֵ��� �Ѵ�.
	/// </summary>
	/// <typeparam name="T">ȣ������ Ÿ��</typeparam>
	/// <param name="Name">Ű�� �̸�</param>
	/// <param name="State">Ű�� ����</param>
	/// <param name="Obj">ȣ������ ��ü</param>
	/// <param name="Func">ȣ������ �ɹ��Լ� ������</param>
	template <typename T>
	void SetCallback(const std::string& Name, EKeyState State, T* Obj, void(T::* Func)(float))
	{
		KeyInfo* Info = FindKeyInfo(Name);

		// std::bind : �Լ������͸� �̿��Ͽ� function ��ü�� ������ش�.
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

