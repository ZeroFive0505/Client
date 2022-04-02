#include "Input.h"
#include "Resource/ResourceManager.h"

CInput* CInput::m_Inst = nullptr;

// 256���� Ű�� �⺻������ �����Ѵ�.
CInput::CInput()
{
	m_vecKeyState.resize(KEYCOUNT_MAX);

	for (int i = 0; i < KEYCOUNT_MAX; i++)
	{
		m_vecKeyState[i].Key = i;
	}

	m_Ctrl = false;
	m_Alt = false;
	m_Shift = false;

	m_MouseType = Mouse_Default;
}

// �Ҹ�� �ؽ����� ��ȸ�ϸ� ��� KeyInfo ������ Ÿ���� �����Ѵ�.
CInput::~CInput()
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

// Name�� Ű�� ���Ӱ� Ű�� �����.
bool CInput::CreateKey(const std::string& Name, int Key)
{
	KeyInfo* Info = FindKeyInfo(Name);

	// ���� �̹� ��ϵǾ��ִ� Ű��� ���� ��ȯ.
	if (Info)
		return false;

	// ���ο� Ű�� �����Ѵ�.
	Info = new KeyInfo;

	// Ű�� �̸��� ���¸� ����
	Info->Name = Name;
	Info->State.Key = Key;

	// �ؽ��ʿ� �ִ´�.
	m_mapInfo.insert(std::make_pair(Name, Info));

	bool Add = false;

	// Ű ���͵��� ��ȸ�ϸ鼭 �̹� Ű�� �ִ��� Ȯ���Ѵ�.
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecAddKey[i] == Key)
		{
			// �̹� ���� Ű�� �߰��� �Ǿ��ִ� ����.
			Add = true;
			break;
		}
	}

	// �ƴ϶�� Ű�� ���
	if (!Add)
		m_vecAddKey.push_back(Key);

	return true;
}

bool CInput::SetCtrlKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Ctrl = State;

	return true;
}

bool CInput::SetAltKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Alt = State;

	return true;
}

bool CInput::SetShiftKey(const std::string& Name, bool State)
{
	KeyInfo* Info = FindKeyInfo(Name);

	if (!Info)
		return false;

	Info->Shift = State;

	return true;
}

KeyInfo* CInput::FindKeyInfo(const std::string& Name)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.find(Name);

	if (iter == m_mapInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::Init(HWND hWnd)
{
	m_hWnd = hWnd;

	m_MouseDown = false;
	m_MousePush = false;
	m_MouseUp = false;

	CreateKey("MoveUp", 'W');
	CreateKey("MoveDown", 'S');
	CreateKey("MoveLeft", 'A');
	CreateKey("MoveRight", 'D');
	CreateKey("Jump", VK_SPACE);
	CreateKey("Fire", VK_CONTROL);

	CreateKey("Pause", 'P');
	SetCtrlKey("Pause", true);
	CreateKey("Resume", 'R');
	SetCtrlKey("Resume", true);

	CreateKey("Skill1", '1');

	CreateKey("Editor", VK_F1);
	CreateKey("MouseLButton", VK_LBUTTON);
	CreateKey("MouseRButton", VK_RBUTTON);

	ShowCursor(FALSE);

	CUIImage* MouseDefault = new CUIImage;

	MouseDefault->SetSize(32.0f, 31.0f);

	std::vector<std::wstring> vecFileName;

	for (int i = 0; i <= 10; i++)
	{
		TCHAR FileName[MAX_PATH] = {};
		wsprintf(FileName, TEXT("Mouse/%d.bmp"), i);

		vecFileName.push_back(FileName);
	}


	CResourceManager::GetInst()->LoadTexture("MouseDefault", vecFileName);

	CTexture* Texture = CResourceManager::GetInst()->FindTexture("MouseDefault");

	for (int i = 0; i <= 10; i++)
	{
		Texture->SetColorKey(255, 0, 255, i);
	}

	MouseDefault->SetTexture(Texture);

	for (int i = 0; i <= 10; i++)
	{
		MouseDefault->AddFrameData(Vector2(0.0f, 0.0f), Vector2(32.0f, 31.0f));
	}

	m_vecMouseImage.push_back(MouseDefault);

	return true;
}

void CInput::Update(float DeltaTime)
{
	RECT rc;
	GetClientRect(m_hWnd, &rc);

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	if (rc.left <= ptMouse.x && ptMouse.x <= rc.right &&
		rc.top <= ptMouse.y && ptMouse.y <= rc.bottom)
	{
		if (IsShowCursor())
		{
			SetShowCursor(false);
			ShowCursor(FALSE);
		}
	}
	else
	{
		if (!IsShowCursor())
		{
			SetShowCursor(true);
			ShowCursor(TRUE);
		}
	}


	// Ű ���¸� ������Ʈ �Ѵ�.
	UpdateKeyState();

	// ���콺 ������Ʈ
	UpdateMouse(DeltaTime);

	// ������ ������Ʈ�� ���� ���·� ���� Ű ������ ������Ʈ�Ѵ�.
	UpdateKeyInfo(DeltaTime);

	m_vecMouseImage[m_MouseType]->Update(DeltaTime);
}

void CInput::Render(HDC hDC)
{
	m_vecMouseImage[m_MouseType]->Render(m_MousePos, hDC);
}

// ���콺 ������ �� ������ ������Ʈ
void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;

	// �� �Լ��� ������ ��ũ����ǥ �������� �����ش�.
	GetCursorPos(&ptMouse);

	// ��ũ�� ��ǥ�� Ŭ���̾�Ʈ ��ǥ�� �ٲ۴�.
	ScreenToClient(m_hWnd, &ptMouse);

	Vector2 Pos;
	Pos.x = (float)ptMouse.x;
	Pos.y = (float)ptMouse.y;

	m_MouseMove = Pos - m_MouseMove;

	m_MousePos = Pos;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		if (!m_MouseDown && !m_MousePush)
		{
			m_MouseDown = true;
			m_MousePush = true;
		}
		else
		{
			m_MouseDown = false;
		}
	}
	else if (m_MousePush)
	{
		m_MouseDown = false;
		m_MousePush = false;
		m_MouseUp = true;
	}
	else if (m_MouseUp)
	{
		m_MouseUp = false;
	}
}

/// <summary>
/// Ű�� ���¸� ������Ʈ�Ѵ�.
/// �������� ����� ���� �ִ�.
/// ���� Push�� Down�� �����̰� ���� �� Ű�� �����ٸ�
/// �ΰ��� ������ ������ �����.
/// �׸��� Ű�� �������¿��� Push�� ���� ���̶��
/// Down�� �������� �����.
/// 
/// ���� Ű�� �������°� �ƴ����� Push�� ���̶��
/// Push�� �������� ����� Up�� ������ �����.
/// �׸��� ���������� ���� Up�� ���̶�� �������� ������ش�.
/// </summary>
void CInput::UpdateKeyState()
{
	// ��ϵ� Ű�� ����ŭ �ݺ��Ѵ�.
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
	{
		int Key = m_vecAddKey[i];

		if (GetAsyncKeyState(Key) & 0x8000)
		{
			// ���� �� �������� �ƴ� ���� �����Ӻ��� �������� �Ǵ�.
			// ���� ���� �����ӿ��� ������ �ʾҰ� ���� �����ӿ��� ������ �����Ÿ�
			if (!m_vecKeyState[Key].State[KeyState_Down] && !m_vecKeyState[Key].State[KeyState_Push])
			{
				// Ǫ�ÿ� �ٿ��� ������ �ٲ۴�.
				m_vecKeyState[Key].State[KeyState_Down] = true;
				m_vecKeyState[Key].State[KeyState_Push] = true;
			}
			else
			{
				// �������� Ű�� ������ ���絵 Ű�� ��������
				m_vecKeyState[Key].State[KeyState_Down] = false;
			}
		}
		else if (m_vecKeyState[Key].State[KeyState_Push])
		{
			m_vecKeyState[Key].State[KeyState_Push] = false;
			m_vecKeyState[Key].State[KeyState_Down] = false;
			m_vecKeyState[Key].State[KeyState_Up] = true;
		}
		else if (m_vecKeyState[Key].State[KeyState_Up])
		{
			m_vecKeyState[Key].State[KeyState_Up] = false;
		}
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

/// <summary>
/// ������Ʈ�� Ű�� ���·� �ؽ����� ��ȸ�ϸ鼭 üũ�Ѵ�.
/// ���� �ش� Ű�� ���¿� ������ Ctrl, Alt, Shift�� ���̶�� �ݹ��Լ��� �� �Ѵ�.
/// </summary>
/// <param name="DeltaTime">DeltaTime�� �ݹ��Լ��� ���� �Լ��� ���ڷ� �����ش�.</param>
void CInput::UpdateKeyInfo(float DeltaTime)
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		int Key = iter->second->State.Key;

		if (m_vecKeyState[Key].State[KeyState_Down] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Down])
				iter->second->Callback[KeyState_Down](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Push] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Push])
				iter->second->Callback[KeyState_Push](DeltaTime);
		}

		if (m_vecKeyState[Key].State[KeyState_Up] &&
			iter->second->Ctrl == m_Ctrl &&
			iter->second->Alt == m_Alt &&
			iter->second->Shift == m_Shift)
		{
			if (iter->second->Callback[KeyState_Up])
				iter->second->Callback[KeyState_Up](DeltaTime);
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
			iter->second->Callback[i] = nullptr;
		}
	}
}