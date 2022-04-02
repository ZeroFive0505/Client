#include "Input.h"

CInput* CInput::m_Inst = nullptr;

CInput::CInput()
{
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; i++)
	{
		m_vecKeyState[i].key = i;
	}

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;
}

CInput::~CInput()
{
	auto iter = m_mapInfo.begin();
	auto iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CInput::CreateKey(const std::string& name, int key)
{
	KeyInfo* info = FindKeyInfo(name);

	if (info)
		return false;

	info = new KeyInfo;

	info->name = name;
	info->state.key = key;

	m_mapInfo.insert(std::make_pair(name, info));

	bool added = false;

	size_t size = m_vecAddedKey.size();

	for (size_t i = 0; i < size; i++)
	{
		if (m_vecAddedKey[i] == key)
		{
			added = true;
			break;
		}
	}

	if (!added)
		m_vecAddedKey.push_back(key);

	return true;
}

bool CInput::SetCtrlKey(const std::string& name, bool state)
{
	KeyInfo* info = FindKeyInfo(name);

	if (!info)
		return false;

	info->ctrl = state;

	return true;
}

bool CInput::SetAltKey(const std::string& name, bool state)
{
	KeyInfo* info = FindKeyInfo(name);

	if (!info)
		return false;

	info->alt = state;

	return true;
}

bool CInput::SetShiftKey(const std::string& name, bool state)
{
	KeyInfo* info = FindKeyInfo(name);

	if (!info)
		return false;

	info->shift = state;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& name)
{
	auto iter = m_mapInfo.find(name);

	if (iter == m_mapInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	return true;
}

void CInput::Update(float DeltaTime)
{
	UpdateKeyState();

	UpdateMouse(DeltaTime);

	UpdateKeyInfo(DeltaTime);
}

void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;

	GetCursorPos(&ptMouse);

	ScreenToClient(m_hWnd, &ptMouse);

	Vector2 Pos;

	Pos.x = (float)ptMouse.x;
	Pos.y = (float)ptMouse.y;

	m_MouseMove = Pos - m_MousePos;

	m_MousePos = Pos;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_MouseDown && !m_MousePush)
		{
			m_MouseDown = true;
			m_MousePush = true;
		}
		else
			m_MouseDown = false;
	}
	else if (m_MousePush)
	{
		m_MouseDown = false;
		m_MousePush = false;
		m_MouseUp = true;
	}
	else if (m_MouseUp)
	{
		m_MouseUp;
	}
}

void CInput::UpdateKeyState()
{
	size_t size = m_vecAddedKey.size();

	for (size_t i = 0; i < size; i++)
	{
		int key = m_vecAddedKey[i];

		if (GetAsyncKeyState(key) & 0x8000)
		{
			if (!m_vecKeyState[key].state[KeyState_Down] &&
				!m_vecKeyState[key].state[KeyState_Hold])
			{
				m_vecKeyState[key].state[KeyState_Down] = true;
				m_vecKeyState[key].state[KeyState_Hold] = true;
			}
			else
				m_vecKeyState[key].state[KeyState_Down] = false;
		}
		else if (m_vecKeyState[key].state[KeyState_Hold])
		{
			m_vecKeyState[key].state[KeyState_Down] = false;
			m_vecKeyState[key].state[KeyState_Hold] = false;
			m_vecKeyState[key].state[KeyState_Up] = true;
		}
		else if (m_vecKeyState[key].state[KeyState_Up])
			m_vecKeyState[key].state[KeyState_Up] = false;
	}

	if (GetAsyncKeyState(VK_CONTROL) & 0x8000)
		m_Ctrl = true;
	else
		m_Ctrl = false;

	if (GetAsyncKeyState(VK_MENU) & 0x8000)
		m_Alt = true;
	else
		m_Alt = false;

	if (GetAsyncKeyState(VK_SHIFT) & 0x8000)
		m_Shift = true;
	else
		m_Shift = false;
}

void CInput::UpdateKeyInfo(float DeltaTime)
{
	auto iter = m_mapInfo.begin();
	auto iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		int key = iter->second->state.key;

		if (m_vecKeyState[key].state[KeyState_Down] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Down])
				iter->second->callBack[KeyState_Down](DeltaTime);
		}

		if (m_vecKeyState[key].state[KeyState_Hold] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Hold])
				iter->second->callBack[KeyState_Hold](DeltaTime);
		}

		if (m_vecKeyState[key].state[KeyState_Up] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Up])
				iter->second->callBack[KeyState_Up](DeltaTime);
		}
	}
}

void CInput::ClearCallback()
{
	auto iter = m_mapInfo.begin();
	auto iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		for (int i = 0; i < KeyState_Max; i++)
		{
			iter->second->callBack[i] = nullptr;
		}
			
		SAFE_DELETE(iter->second);
	}

	m_mapInfo.clear();
	m_vecAddedKey.clear();
}

void CInput::SetEditorKeySet()
{
	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey("MoveRight", 'D');
	CreateKey("Editor", VK_F1);
	CreateKey("MouseLButton", VK_LBUTTON);
	CreateKey("MouseRButton", VK_RBUTTON);
	CreateKey("BackToTitle", VK_ESCAPE);

	CreateKey("FixX", 'Z');
	CreateKey("FixY", 'X');

	CreateKey("Instantiate", VK_SPACE);
	CreateKey("DeleteObject", 'Q');
	CreateKey("Hide", 'H');
}

void CInput::SetPlayModeKeySet()
{
	CreateKey("TurnLeft", VK_LEFT);
	CreateKey("TurnRight", VK_RIGHT);
	CreateKey("ItemUseForward", VK_UP);
	CreateKey("ItemUseBackward", VK_DOWN);
	CreateKey("LookBack", 'V');
	CreateKey("ToggleAuto", VK_F2);
	CreateKey("Drift", 'C');
	CreateKey("Accel", 'X');
	CreateKey("Brake", 'Z');
	CreateKey("BackToTitle", VK_ESCAPE);
	CreateKey("Feather", VK_F1);
}

void CInput::SetMenuModeKeySet()
{
	CreateKey("Up", VK_UP);
	CreateKey("Down", VK_DOWN);
	CreateKey("Left", VK_LEFT);
	CreateKey("Right", VK_RIGHT);
	CreateKey("Next", VK_RETURN);
	CreateKey("Back", VK_ESCAPE);
}

void CInput::SetMode7DemoKeySet()
{
	CreateKey("Up", 'W');
	CreateKey("Down", 'S');
	CreateKey("Left", 'A');
	CreateKey("Right", 'D');
	CreateKey("Increase far", 'E');
	CreateKey("Decrease far", 'Q');
	CreateKey("Increase near", 'C');
	CreateKey("Decrease near", 'Z');
	CreateKey("Increase FOV", 'U');
	CreateKey("Decrease FOV", 'J');
	CreateKey("BackToTitle", VK_ESCAPE);
	CreateKey("Toggle perspective", 'P');
}
