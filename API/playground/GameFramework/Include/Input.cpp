#include "Input.h"
#include "Resource/ResourceManager.h"

CInput* CInput::m_Inst = nullptr;

// 256개의 키를 기본적으로 설정한다.
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

// 소멸시 해쉬맵을 순회하며 모든 KeyInfo 포인터 타입을 제거한다.
CInput::~CInput()
{
	std::unordered_map<std::string, KeyInfo*>::iterator iter = m_mapInfo.begin();
	std::unordered_map<std::string, KeyInfo*>::iterator iterEnd = m_mapInfo.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

// Name을 키로 새롭게 키를 만든다.
bool CInput::CreateKey(const std::string& Name, int Key)
{
	KeyInfo* Info = FindKeyInfo(Name);

	// 만약 이미 등록되어있는 키라면 거짓 반환.
	if (Info)
		return false;

	// 새로운 키를 생성한다.
	Info = new KeyInfo;

	// 키의 이름과 상태를 저장
	Info->Name = Name;
	Info->State.Key = Key;

	// 해쉬맵에 넣는다.
	m_mapInfo.insert(std::make_pair(Name, Info));

	bool Add = false;

	// 키 벡터들을 순회하면서 이미 키가 있는지 확인한다.
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
	{
		if (m_vecAddKey[i] == Key)
		{
			// 이미 같은 키가 추가가 되어있는 경우다.
			Add = true;
			break;
		}
	}

	// 아니라면 키를 등록
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


	// 키 상태를 업데이트 한다.
	UpdateKeyState();

	// 마우스 업데이트
	UpdateMouse(DeltaTime);

	// 위에서 업데이트를 해준 상태로 실제 키 정보를 업데이트한다.
	UpdateKeyInfo(DeltaTime);

	m_vecMouseImage[m_MouseType]->Update(DeltaTime);
}

void CInput::Render(HDC hDC)
{
	m_vecMouseImage[m_MouseType]->Render(m_MousePos, hDC);
}

// 마우스 정보를 매 프레임 업데이트
void CInput::UpdateMouse(float DeltaTime)
{
	POINT ptMouse;

	// 이 함수는 윈도우 스크린좌표 기준으로 구해준다.
	GetCursorPos(&ptMouse);

	// 스크린 좌표를 클라이언트 좌표로 바꾼다.
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
/// 키의 상태를 업데이트한다.
/// 세가지의 경우의 수가 있다.
/// 만약 Push와 Down이 거짓이고 지금 막 키를 눌렀다면
/// 두개의 변수를 참으로 만든다.
/// 그리고 키를 누른상태에서 Push가 지금 참이라면
/// Down을 거짓으로 만든다.
/// 
/// 지금 키를 누른상태가 아니지만 Push가 참이라면
/// Push를 거짓으로 만들고 Up을 참으로 만든다.
/// 그리고 마지막으로 현재 Up이 참이라면 거짓으로 만들어준다.
/// </summary>
void CInput::UpdateKeyState()
{
	// 등록된 키의 수만큼 반복한다.
	size_t Size = m_vecAddKey.size();

	for (size_t i = 0; i < Size; i++)
	{
		int Key = m_vecAddKey[i];

		if (GetAsyncKeyState(Key) & 0x8000)
		{
			// 지금 막 눌렀는지 아님 이전 프레임부터 눌렀는지 판단.
			// 만약 이전 프레임에도 누르지 않았고 지금 프레임에도 누르지 않은거면
			if (!m_vecKeyState[Key].State[KeyState_Down] && !m_vecKeyState[Key].State[KeyState_Push])
			{
				// 푸시와 다운을 참으로 바꾼다.
				m_vecKeyState[Key].State[KeyState_Down] = true;
				m_vecKeyState[Key].State[KeyState_Push] = true;
			}
			else
			{
				// 이전에도 키를 눌렀고 현재도 키를 누른상태
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
/// 업데이트된 키의 상태로 해쉬맵을 순회하면서 체크한다.
/// 만약 해당 키의 상태와 정해진 Ctrl, Alt, Shift가 참이라면 콜백함수를 콜 한다.
/// </summary>
/// <param name="DeltaTime">DeltaTime은 콜백함수로 묶인 함수의 인자로 보내준다.</param>
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
