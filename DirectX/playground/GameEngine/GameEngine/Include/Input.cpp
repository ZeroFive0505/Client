#include "Input.h"
#include "Device.h"
#include "Engine.h"
#include "Scene/SceneManager.h"
#include "Component/CameraComponent.h"
#include "Widget/Widget.h"

DEFINITION_SINGLE(CInput)

CInput::CInput() : 
	m_hInst(0),
	m_hWnd(0),
	m_Input(nullptr),
	m_Keyboard(nullptr),
	m_Mouse(nullptr),
	m_KeyArray{},
	m_LButtonClick(false),
	m_RButtonClick(false),
	m_CollisionWidget(false)
{
	m_vecKeyState.resize(256);

	for (int i = 0; i < 256; i++)
	{
		m_vecKeyState[i].key = (unsigned char)i;
	}
}

CInput::~CInput()
{
	auto iter = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}

	SAFE_RELEASE(m_Keyboard);
	SAFE_RELEASE(m_Mouse);
	SAFE_RELEASE(m_Input);
}

CWidget* CInput::GetDragWidget() const
{
	return m_DragWidget;
}

bool CInput::CreateKey(const std::string& name, unsigned char key)
{
	sKeyInfo* info = FindkeyInfo(name);

	if (info)
		return false;

	info = new sKeyInfo;

	info->name = name;

	unsigned char convertedKey = ConvertKey(key);

	info->state.key = convertedKey;

	m_mapKeyInfo.insert(std::make_pair(name, info));

	bool add = false;

	size_t size = m_vecAddedKey.size();

	for (size_t i = 0; i < size; i++)
	{
		if (m_vecAddedKey[i] == convertedKey)
		{
			add = true;
			break;
		}
	}

	if (!add)
		m_vecAddedKey.push_back(convertedKey);

	return true;
}

bool CInput::SetCtrlkey(const std::string& name, bool state)
{
	sKeyInfo* info = FindkeyInfo(name);

	if (!info)
		return false;

	info->ctrl = state;

	return true;
}

bool CInput::SetAltKey(const std::string& name, bool state)
{
	sKeyInfo* info = FindkeyInfo(name);

	if (!info)
		return false;

	info->alt = state;

	return true;
}

bool CInput::SetShiftKey(const std::string& name, bool state)
{
	sKeyInfo* info = FindkeyInfo(name);

	if (!info)
		return false;

	info->shift = state;

	return true;
}

void CInput::SetDragWidget(CWidget* widget)
{
	m_DragWidget = widget;
}

sKeyInfo* CInput::FindkeyInfo(const std::string& name)
{
	auto iter = m_mapKeyInfo.find(name);

	if (iter == m_mapKeyInfo.end())
		return nullptr;

	return iter->second;
}

bool CInput::InitWindow()
{
	return true;
}

bool CInput::InitDirectInput()
{
	if (FAILED(m_Input->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr)))
		return false;

	if (FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)))
		return false;

	if (FAILED(m_Input->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr)))
		return false;

	if (FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)))
		return false;

	return true;
}

bool CInput::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hInst = hInst;
	m_hWnd = hWnd;

	m_InputType = Input_Type::Direct;

	HRESULT result = DirectInput8Create(m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&m_Input, nullptr);

	if (FAILED(result))
		m_InputType = Input_Type::Window;

	switch (m_InputType)
	{
	case Input_Type::Direct:
		if (!InitDirectInput())
			return false;
		break;
	case Input_Type::Window:
		if (!InitWindow())
			return false;
		break;
	}

	return true;
}

void CInput::Update(float deltaTime)
{
	if (m_InputType == Input_Type::Direct)
	{
		ReadDirectInputKeyboard();
		ReadDirectInputMouse();
	}

	// 마우스 입력처리를 한다. (먼저 마우스와 UI의 충돌을 처리해서 마우스 충돌시에 마우스 입력을 막는다.)
	UpdateMouse(deltaTime);

	// UI vs 마우스
	m_CollisionWidget = CSceneManager::GetInst()->GetScene()->GetCollision()->CollisionWidget(deltaTime);

	// 키 상태 업데이트, (다운, 푸쉬, 업)
	UpdateKeyState();

	// 키보드 키 입력처리를 한다.
	UpdateKeyInfo(deltaTime);
}

void CInput::ReadDirectInputKeyboard()
{
	HRESULT	result = m_Keyboard->GetDeviceState(256, m_KeyArray);

	if (FAILED(result))
	{	
		// 다시 장치를 얻어온다.
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_Keyboard->Acquire();
	}
}

void CInput::ReadDirectInputMouse()
{
	HRESULT	result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);

	if (FAILED(result))
	{
		if (result == DIERR_INPUTLOST || result == DIERR_NOTACQUIRED)
			m_Mouse->Acquire();
	}
}

void CInput::UpdateMouse(float deltaTime)
{
	POINT mouseWindowPos;

	// 마우스의 위치를 가져온다.
	GetCursorPos(&mouseWindowPos);
	// 클라이언트 창에서의 마우스 위치를 가져온다.
	ScreenToClient(m_hWnd, &mouseWindowPos);

	// 현재 바뀐 해상도의 비율을 구해온다.
	Vector2 ratio = CDevice::GetInst()->GetViewportAspectRatio();

	// 마웃의 위치는 현재 위치에서 비율만큼 곱해준다.
	// 스크린 좌표에서는 왼쪽 상단이 0, 0
	// 월드에서는 반대로 왼쪽 하단이 0, 0 따라서 뒤집는다.
	Vector2 mousePos = Vector2(mouseWindowPos.x * ratio.x, mouseWindowPos.y * ratio.y);

	mousePos.y = CDevice::GetInst()->GetResolution().height - mousePos.y;

	// 이동량 갱신
	m_MouseMove = mousePos - m_MousePos;

	m_MouseWorldPos = mousePos;


	// 새로운 위치 갱신
	m_MousePos = mousePos;

	// 2D일때는 월드공간에서의 마우스 좌표를 구해온다.
	// 카메라를 일단 얻어온다.
	if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
	{
		CCameraComponent* camera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetCurrentCamera();

		// 마우스의 위치는 그만큼 더해준다.
		m_MouseWorldPos += camera->GetLeftBottom();
	}
}

void CInput::UpdateKeyState()
{
	switch (m_InputType)
	{
	case Input_Type::Direct:
		if (m_KeyArray[DIK_LCONTROL] & 0x80)
			m_Ctrl = true;
		else
			m_Ctrl = false;

		if (m_KeyArray[DIK_LALT] & 0x80)
			m_Alt = true;
		else
			m_Alt = false;

		if (m_KeyArray[DIK_LSHIFT] & 0x80)
			m_Shift = true;
		else
			m_Shift = false;

		if (m_MouseState.rgbButtons[0] & 0x80)
			m_LButtonClick = true;
		else
			m_LButtonClick = false;

		if (m_MouseState.rgbButtons[1] & 0x80)
			m_RButtonClick = true;
		else
			m_RButtonClick = false;

		break;
	case Input_Type::Window:
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

		if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			m_LButtonClick = true;
		else
			m_LButtonClick = false;

		if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
			m_RButtonClick = true;
		else
			m_RButtonClick = false;

		break;
	}

	size_t size = m_vecAddedKey.size();

	// 등록된 키들을 순회한다.
	for (size_t i = 0; i < size; i++)
	{
		unsigned char key = m_vecAddedKey[i];

		bool keyInput = false;

		switch (m_InputType)
		{
		case Input_Type::Direct:
			switch (key)
			{
			case DIK_MOUSELBUTTON:
				if (m_MouseState.rgbButtons[0] & 0x80 && !m_CollisionWidget)
					keyInput = true;
				break;
			case DIK_MOUSERBUTTON:
				if (m_MouseState.rgbButtons[1] & 0x80 && !m_CollisionWidget)
					keyInput = true;
				break;
			case DIK_MOUSEWHEEL:
				break;
			default:
				// 키보드 입력
				if (m_KeyArray[key] & 0x80)
				{
					keyInput = true;
				}
			}
			break;
		case Input_Type::Window:
			if (GetAsyncKeyState(key) & 0x800)
				keyInput = true;
			break;
		}

		// 키 입력이 있을 경우
		if (keyInput)
		{
			// 만약 누른적이 없을 경우
			if (!m_vecKeyState[key].state[KeyState_Down] &&
				!m_vecKeyState[key].state[KeyState_Push])
			{
				m_vecKeyState[key].state[KeyState_Down] = true;
				m_vecKeyState[key].state[KeyState_Push] = true;
			}
			// 키 입력이 있고 전에도 누른 키라면
			else
			{
				m_vecKeyState[key].state[KeyState_Down] = false;
			}
		}
		// 만약 키입력은 없지만 전에 쭉 키를 눌렀던 상태라면
		else if (m_vecKeyState[key].state[KeyState_Push])
		{
			m_vecKeyState[key].state[KeyState_Push] = false;
			m_vecKeyState[key].state[KeyState_Down] = false;
			m_vecKeyState[key].state[KeyState_Up] = true;
		}
		else if (m_vecKeyState[key].state[KeyState_Up])
		{
			m_vecKeyState[key].state[KeyState_Up] = false;
		}
	}
}

void CInput::UpdateKeyInfo(float deltaTime)
{
	auto iter = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; iter++)
	{
		unsigned char key = iter->second->state.key;

		if (m_vecKeyState[key].state[KeyState_Down] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Down])
			{
				if (!ImGui::GetIO().WantCaptureMouse)
					iter->second->callBack[KeyState_Down](deltaTime);
			}
		}


		if (m_vecKeyState[key].state[KeyState_Push] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Push])
			{
				if (!ImGui::GetIO().WantCaptureMouse)
					iter->second->callBack[KeyState_Push](deltaTime);
			}
		}


		if (m_vecKeyState[key].state[KeyState_Up] &&
			iter->second->ctrl == m_Ctrl &&
			iter->second->alt == m_Alt &&
			iter->second->shift == m_Shift)
		{
			if (iter->second->callBack[KeyState_Up])
			{
				if(!ImGui::GetIO().WantCaptureMouse)
					iter->second->callBack[KeyState_Up](deltaTime);
			}
		}
	}
}

void CInput::ClearCallback()
{
	auto iter = m_mapKeyInfo.begin();
	auto iterEnd = m_mapKeyInfo.end();

	for (; iter != iterEnd; iter++)
	{
		for (int i = 0; i < KeyState_Max; i++)
		{
			iter->second->callBack[i] = nullptr;
		}
	}
}

unsigned char CInput::ConvertKey(unsigned char key)
{
	if (m_InputType == Input_Type::Direct)
	{
		switch (key)
		{
		case VK_LBUTTON:
			return DIK_MOUSELBUTTON;
		case VK_RBUTTON:
			return DIK_MOUSERBUTTON;
		case VK_BACK:
			return DIK_BACK;
		case VK_TAB:
			return DIK_TAB;
		case VK_RETURN:
			return DIK_RETURN;
		case VK_LCONTROL:
			return DIK_LCONTROL;
		case VK_RCONTROL:
			return DIK_RCONTROL;
		case VK_LMENU:
			return DIK_LALT;
		case VK_RMENU:
			return DIK_RALT;
		case VK_LSHIFT:
			return DIK_LSHIFT;
		case VK_RSHIFT:
			return DIK_RSHIFT;
		case VK_PAUSE:
			return DIK_PAUSE;
		case VK_CAPITAL:
			return DIK_CAPSLOCK;
		case VK_ESCAPE:
			return DIK_ESCAPE;
		case VK_SPACE:
			return DIK_SPACE;
		case VK_NEXT:
			return DIK_PGDN;
		case VK_PRIOR:
			return DIK_PGUP;
		case VK_END:
			return DIK_END;
		case VK_HOME:
			return DIK_HOME;
		case VK_LEFT:
			return DIK_LEFT;
		case VK_UP:
			return DIK_UP;
		case VK_RIGHT:
			return DIK_RIGHT;
		case VK_DOWN:
			return DIK_DOWN;
		case VK_PRINT:
			return DIK_SYSRQ;
		case VK_INSERT:
			return DIK_INSERT;
		case VK_DELETE:
			return DIK_DELETE;
		case VK_HELP:
			return 0;
		case '0':
			return DIK_0;
		case '1':
			return DIK_1;
		case '2':
			return DIK_2;
		case '3':
			return DIK_3;
		case '4':
			return DIK_4;
		case '5':
			return DIK_5;
		case '6':
			return DIK_6;
		case '7':
			return DIK_7;
		case '8':
			return DIK_8;
		case '9':
			return DIK_9;
		case 'A':
			return DIK_A;
		case 'B':
			return DIK_B;
		case 'C':
			return DIK_C;
		case 'D':
			return DIK_D;
		case 'E':
			return DIK_E;
		case 'F':
			return DIK_F;
		case 'G':
			return DIK_G;
		case 'H':
			return DIK_H;
		case 'I':
			return DIK_I;
		case 'J':
			return DIK_J;
		case 'K':
			return DIK_K;
		case 'L':
			return DIK_L;
		case 'M':
			return DIK_M;
		case 'N':
			return DIK_N;
		case 'O':
			return DIK_O;
		case 'P':
			return DIK_P;
		case 'Q':
			return DIK_Q;
		case 'R':
			return DIK_R;
		case 'S':
			return DIK_S;
		case 'T':
			return DIK_T;
		case 'U':
			return DIK_U;
		case 'V':
			return DIK_V;
		case 'W':
			return DIK_W;
		case 'X':
			return DIK_X;
		case 'Y':
			return DIK_Y;
		case 'Z':
			return DIK_Z;
		case VK_OEM_3:
			return DIK_GRAVE;
		case VK_OEM_MINUS:
			return DIK_MINUS;
		case VK_OEM_NEC_EQUAL:
			return DIK_EQUALS;
		case VK_OEM_4:
			return DIK_LBRACKET;
		case VK_OEM_6:
			return DIK_RBRACKET;
		case VK_OEM_5:
			return DIK_BACKSLASH;
		case VK_OEM_1:
			return DIK_SEMICOLON;
		case VK_OEM_7:
			return DIK_APOSTROPHE;
		case VK_OEM_COMMA:
			return DIK_COMMA;
		case VK_OEM_PERIOD:
			return DIK_PERIOD;
		case VK_OEM_2:
			return DIK_SLASH;
		case VK_NUMPAD0:
			return DIK_NUMPAD0;
		case VK_NUMPAD1:
			return DIK_NUMPAD1;
		case VK_NUMPAD2:
			return DIK_NUMPAD2;
		case VK_NUMPAD3:
			return DIK_NUMPAD3;
		case VK_NUMPAD4:
			return DIK_NUMPAD4;
		case VK_NUMPAD5:
			return DIK_NUMPAD5;
		case VK_NUMPAD6:
			return DIK_NUMPAD6;
		case VK_NUMPAD7:
			return DIK_NUMPAD7;
		case VK_NUMPAD8:
			return DIK_NUMPAD8;
		case VK_NUMPAD9:
			return DIK_NUMPAD9;
		case VK_MULTIPLY:
			return DIK_MULTIPLY;
		case VK_ADD:
			return DIK_ADD;
		case VK_SEPARATOR:
			return DIK_NUMPADCOMMA;
		case VK_SUBTRACT:
			return DIK_SUBTRACT;
		case VK_DECIMAL:
			return DIK_DECIMAL;
		case VK_DIVIDE:
			return DIK_DIVIDE;
			//case VK_RETURN:		
			return DIK_NUMPADENTER;
		case VK_F1:
			return DIK_F1;
		case VK_F2:
			return DIK_F2;
		case VK_F3:
			return DIK_F3;
		case VK_F4:
			return DIK_F4;
		case VK_F5:
			return DIK_F5;
		case VK_F6:
			return DIK_F6;
		case VK_F7:
			return DIK_F7;
		case VK_F8:
			return DIK_F8;
		case VK_F9:
			return DIK_F9;
		case VK_F10:
			return DIK_F10;
		case VK_F11:
			return DIK_F11;
		case VK_F12:
			return DIK_F12;
		case VK_F13:
			return DIK_F13;
		case VK_F14:
			return DIK_F14;
		case VK_F15:
			return DIK_F15;
		case VK_F16:
		case VK_F17:
		case VK_F18:
		case VK_F19:
		case VK_F20:
		case VK_F21:
		case VK_F22:
		case VK_F23:
		case VK_F24:
			return 0;
		case VK_NUMLOCK:
			return DIK_NUMLOCK;
		case VK_SCROLL:
			return DIK_SCROLL;
		case VK_LWIN:
			return DIK_LWIN;
		case VK_RWIN:
			return DIK_RWIN;
		case VK_APPS:
			return DIK_APPS;
		case VK_OEM_102:
			return DIK_OEM_102;
		}

		return 0xff;
	}

	return key;
}
