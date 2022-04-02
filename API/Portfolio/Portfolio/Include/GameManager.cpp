#include "GameManager.h"
#include "Timer.h"
#include "Input.h"
#include "PathManager.h"
#include "Scene/SceneManager.h"
#include "Resource/ResourceManager.h"
#include "Scene/RaceScene.h"
#include "Scene/TitleScene.h"
#include "Scene/EditorScene.h"
#include "Scene/CollisionHash.h"

CGameManager* CGameManager::m_Inst = nullptr;
bool CGameManager::m_Loop = true;

CGameManager::CGameManager() :
	m_Timer(nullptr),
	m_TimeScale(1.0f)
{
}

CGameManager::~CGameManager()
{
	CSceneManager::DestroyInst();

	CInput::DestroyInst();

	CPathManager::DestroyInst();

	CCollisionHash::DestroyInst();

	CResourceManager::DestroyInst();

	SAFE_DELETE(m_Timer);

	SelectObject(m_hBackDC, m_hPrevBackBmp);

	DeleteObject(m_hBackBmp);

	DeleteDC(m_hBackDC);

	DeleteObject(m_hGreenBrush);
	DeleteObject(m_hYellowBrush);
	DeleteObject(m_hDarkerYellowBrush);
	DeleteObject(m_hRedBrush);
	DeleteObject(m_hBlackBrush);
	DeleteObject(m_hOrangeBrush);

	DeleteObject(m_hGreenPen);
	DeleteObject(m_hRedPen);

	ReleaseDC(m_hWnd, m_hDC);
}

float CGameManager::GetDeltaTime() const
{
	return m_Timer->GetDeltaTime() * m_TimeScale;
}

void CGameManager::Exit()
{
	DestroyWindow(m_hWnd);
}

bool CGameManager::Init(HINSTANCE hInst)
{

	m_hInst = hInst;

	m_RS.width = 512;
	m_RS.height = 448;

	Register();
	Create();

	m_hDC = GetDC(m_hWnd);

	if (!CPathManager::GetInst()->Init())
		return false;

	if (!CResourceManager::GetInst()->Init())
		return false;

	if (!CCollisionHash::GetInst()->Init())
		return false;

	if (!CInput::GetInst()->Init(m_hWnd))
		return false;

	if (!CSceneManager::GetInst()->Init())
		return false;

	// CSceneManager::GetInst()->CreateScene<CRaceScene>();
	// CSceneManager::GetInst()->CreateScene<CEditorScene>();
	CSceneManager::GetInst()->CreateScene<CTitleScene>();

	m_Timer = new CTimer;

	m_hBackDC = CreateCompatibleDC(m_hDC);

	m_hBackBmp = CreateCompatibleBitmap(m_hDC, m_RS.width, m_RS.height);

	m_hPrevBackBmp = (HBITMAP)SelectObject(m_hBackDC, m_hBackBmp);


	m_hGreenBrush = CreateSolidBrush(RGB(0, 255, 0));
	m_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
	m_hYellowBrush = CreateSolidBrush(RGB(255, 255, 0));
	m_hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
	m_hOrangeBrush = CreateSolidBrush(RGB(255, 154, 0));
	m_hDarkerYellowBrush = CreateSolidBrush(RGB(232, 163, 23));

	m_hGreenPen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	m_hRedPen = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));


	return true;
}

int CGameManager::Run()
{
	MSG msg;

	while (m_Loop)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);

			DispatchMessage(&msg);
		}
		else
			Logic();
	}

	return (int)msg.wParam;
}

void CGameManager::Logic()
{
	float deltaTime = m_Timer->Update() * m_TimeScale;

	CResourceManager::GetInst()->Update();

	CInput::GetInst()->Update(deltaTime);

	if (Update(deltaTime))
		return;

	if (PostUpdate(deltaTime))
		return;

	if (Collision(deltaTime))
		return;

	Render(deltaTime);
}

bool CGameManager::Update(float deltaTime)
{
	return CSceneManager::GetInst()->Update(deltaTime);
}

bool CGameManager::PostUpdate(float deltaTime)
{
	return CSceneManager::GetInst()->PostUpdate(deltaTime);
}

bool CGameManager::Collision(float deltaTime)
{
	return CSceneManager::GetInst()->Collision(deltaTime);
}

void CGameManager::Render(float deltaTime)
{
	Rectangle(m_hBackDC, -1, -1, m_RS.width + 1, m_RS.height + 1);

	CSceneManager::GetInst()->Render(m_hBackDC);

	BitBlt(m_hDC, 0, 0, m_RS.width, m_RS.height, m_hBackDC, 0, 0, SRCCOPY);
}

ATOM CGameManager::Register()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;

	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	wcex.hInstance = m_hInst;

	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(IDI_ICON1));

	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;

	wcex.lpszClassName = TEXT("Portfolio");
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassExW(&wcex);
}

BOOL CGameManager::Create()
{
	m_hWnd = CreateWindowW(TEXT("Portfolio"), TEXT("Portfolio"), WS_OVERLAPPEDWINDOW,
		100, 100, m_RS.width, m_RS.height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
		return FALSE;

	RECT rc = { 0, 0, m_RS.width, m_RS.height };

	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	SetWindowPos(m_hWnd, HWND_TOPMOST, 100, 100, rc.right - rc.left, rc.bottom - rc.top, SWP_NOZORDER);

	ShowWindow(m_hWnd, SW_SHOW);

	UpdateWindow(m_hWnd);

	return TRUE;
}

LRESULT CGameManager::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
