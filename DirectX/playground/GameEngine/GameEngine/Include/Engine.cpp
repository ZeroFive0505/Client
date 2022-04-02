#include "Engine.h"
#include "Device.h"
#include "Resource/ResourceManager.h"
#include "PathManager.h"
#include "Timer.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "Collision/CollisionManager.h"
#include "Resource/Shader/GlobalConstantBuffer.h"

DEFINITION_SINGLE(CEngine)

bool CEngine::m_Loop = true;

CEngine::CEngine() :
	m_ClearColors{},
	m_Timer(nullptr),
	m_Start(false),
	m_Play(true),
	m_Slow(false),
	m_Space(Engine_Space::Space2D),
	m_ShowCursorCount(0),
	m_GlobalCBuffer(nullptr),
	m_GlobalAccTime(0.0f),
	m_NoiseSeed1D(nullptr),
	m_NoiseSeed2D(nullptr),
	m_PerlinNoise1D(nullptr),
	m_PerlinNoise2D(nullptr),
	m_SceneChangeStart(false),
	m_ElapsedTime(0.0f)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(326);

	//m_ClearColors[0] = 211 / (float)255;
	//m_ClearColors[1] = 211 / (float)255;
	//m_ClearColors[2] = 211 / (float)255;
	//m_ShowCursorCount = 0;

	m_ClearColors[0] = 0.0f;
	m_ClearColors[1] = 0.0f;
	m_ClearColors[2] = 0.0f;
	m_ShowCursorCount = 0;
}

CEngine::~CEngine()
{
	CSceneManager::DestroyInst();

	SAFE_DELETE(m_GlobalCBuffer);

	CInput::DestroyInst();

	CRenderManager::DestroyInst();

	CCollisionManager::DestroyInst();

	CPathManager::DestroyInst();

	CResourceManager::DestroyInst();

	CIMGUIManager::DestroyInst();

	CDevice::DestroyInst();

	SAFE_DELETE_ARRAY(m_PerlinNoise1D);
	SAFE_DELETE_ARRAY(m_PerlinNoise2D);
	SAFE_DELETE_ARRAY(m_NoiseSeed1D);
	SAFE_DELETE_ARRAY(m_NoiseSeed2D);

	SAFE_DELETE(m_Timer);
}

void CEngine::SetMouseState(Mouse_State state)
{
	if (m_MouseWidget[(int)m_MouseState])
	{

	}

	m_MouseState = state;
}

bool CEngine::Init(HINSTANCE hInst, const TCHAR* name, unsigned int width, const int height, int iconID, bool windowMode)
{
	m_hInst = hInst;

	m_RS.width = width;
	m_RS.height = height;

	m_OctaveCount = 9;

	m_ScalingBias = 2.0f;

	m_PerlinNoiseWidth = (int)m_RS.width;
	m_PerlinNoiseHeight = (int)m_RS.width;

	m_PerlinOutPutSize1D = (int)m_RS.width;

	m_NoiseSeed1D = new float[m_PerlinOutPutSize1D];
	m_NoiseSeed2D = new float[m_PerlinNoiseWidth * m_PerlinNoiseHeight];
	m_PerlinNoise1D = new float[m_PerlinOutPutSize1D];
	m_PerlinNoise2D = new float[m_PerlinNoiseWidth * m_PerlinNoiseHeight];


	for (int i = 0; i < m_PerlinOutPutSize1D; i++)
	{
		float num = rand() / (float)RAND_MAX;

		m_NoiseSeed1D[i] = num;
	}

	for (int i = 0; i < m_PerlinNoiseWidth * m_PerlinNoiseHeight; i++)
	{
		float num = rand() / (float)RAND_MAX;

		m_NoiseSeed2D[i] = num;
	}

	PerlinNoise1D();
	PerlinNoise2D();


	Register(name, iconID);
	Create(name);

	return Init(hInst, m_hWnd, width, height, windowMode);
}

bool CEngine::Init(HINSTANCE hInst, HWND hWnd, unsigned int width, unsigned int height, bool windowMode)
{

	m_Timer = new CTimer;

	if (!CDevice::GetInst()->Init(m_hWnd, width, height, windowMode))
		return false;

	if (!CPathManager::GetInst()->Init())
		return false;

	if (!CResourceManager::GetInst()->Init())
		return false;

	if (!CCollisionManager::GetInst()->Init())
		return false;

	if (!CInput::GetInst()->Init(hInst, hWnd))
		return false;

	if (!CIMGUIManager::GetInst()->Init(hWnd))
		return false;

	if (!CRenderManager::GetInst()->Init())
		return false;

	m_GlobalCBuffer = new CGlobalConstantBuffer;

	if (!m_GlobalCBuffer->Init())
		return false;

	m_GlobalCBuffer->SetResolution(m_RS);

	if (!CSceneManager::GetInst()->Init())
		return false;

	// Noise Texture
	CResourceManager::GetInst()->LoadTexture("GlobalNoiseTexture", TEXT("noise_01.png"));

	m_GlobalNoiseTexture = CResourceManager::GetInst()->FindTexture("GlobalNoiseTexture");

	// 텍스처 유닛은 100번 한장이기에 0번
	m_GlobalNoiseTexture->SetShader(100, (int)Buffer_Shader_Type::All, 0);

	m_GlobalCBuffer->SetNoiseTextureResolution((float)m_GlobalNoiseTexture->GetWidth(), (float)m_GlobalNoiseTexture->GetHeight());

	return true;
}

void CEngine::PerlinNoise1D()
{
	for (int x = 0; x < m_PerlinOutPutSize1D; x++)
	{
		float noise = 0.0f;
		float scaleAcc = 0.0f;
		float scale = 1.0f;

		for (int o = 0; o < m_OctaveCount; o++)
		{
			int pitch = m_PerlinOutPutSize1D >> o;
			int sample1 = (x / pitch) * pitch;
			int sample2 = (sample1 + pitch) % m_PerlinOutPutSize1D;

			float blend = (float)(x - sample1) / (float)pitch;

			float sample = (1.0f - blend) * m_NoiseSeed1D[sample1] + blend * m_NoiseSeed1D[sample2];

			scaleAcc += scale;
			noise += sample * scale;
			scale = scale / m_ScalingBias;
		}

		m_PerlinNoise1D[x] = noise / scaleAcc;
	}
}

void CEngine::PerlinNoise2D()
{
	for (int x = 0; x < m_PerlinNoiseWidth; x++)
	{
		for (int y = 0; y < m_PerlinNoiseHeight; y++)
		{
			float noise = 0.0f;
			float scaleAcc = 0.0f;
			float scale = 1.0f;

			for (int o = 0; o < m_OctaveCount; o++)
			{
				int pitch = m_PerlinNoiseWidth >> o;

				int sampleX1 = (x / pitch) * pitch;
				int sampleY1 = (y / pitch) * pitch;

				int sampleX2 = (sampleX1 + pitch) % m_PerlinNoiseWidth;
				int sampleY2 = (sampleY1 + pitch) % m_PerlinNoiseWidth;

				float blendX = (float)(x - sampleX1) / (float)pitch;
				float blendY = (float)(y - sampleY1) / (float)pitch;

				float sample1 = (1.0f - blendX) * m_NoiseSeed2D[sampleY1 * m_PerlinNoiseWidth + sampleX1] + 
					blendX * m_NoiseSeed2D[sampleY1 * m_PerlinNoiseWidth + sampleX2];
				float sample2 = (1.0f - blendX) * m_NoiseSeed2D[sampleY2 * m_PerlinNoiseWidth + sampleX1]+ 
					blendX * m_NoiseSeed2D[sampleY2 * m_PerlinNoiseWidth + sampleX2];

				scaleAcc += scale;
				noise += (blendY * (sample2 - sample1) + sample1) * scale;
				scale = scale / m_ScalingBias;
			}

			m_PerlinNoise2D[y * m_PerlinNoiseWidth + x] = noise / scaleAcc;
		}
	}
}

int CEngine::Run()
{
	MSG msg = {};

	while (m_Loop)
	{
		// GetMessage는 메세지가 없을 경우 다른일을 할 수 없다.
		// 메세지가 올때까지 대기하고 있는 시간을 윈도우의 데드타임이라고 한다.
		// 실제로 메세지가 있는 시간보다 없는 시간이 훨씬 길다.
		// 그래서 게임은 윈도우의 데드타임동안 게임이 동작될 수 있게 제작한다.
		// PeekMessage는 메세지큐에서 메세지를 얻어온다.
		// 그런데 만약 메세지가 없다면 false를 리턴하면서 바로 빠져나오고
		// 메세지가 있다면 true를 리턴하면서 해당 메세지를 꺼내온다.
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			// WM_KEYDOWN 이라는 메세지가 있다. 이는 키보드 키를 눌렀을때 발생되는 메세지
			// 이다. TranslateMessage 함수에서는 메세지를 전달해주면 WM_KEYDOWN 인지를
			// 판단해주고 눌려진 키가 문자 키인지를 판단해서 일반 무자 키라면 WM_CHAR라는
			// 메시지를 만들어서 메세지 큐에 추가해주게 된다.
			TranslateMessage(&msg);

			// DispatchMessage 함수는 메세지를 윈도우 프로시저로 보내준다.
			DispatchMessage(&msg);
		}

		// 윈도우가 데드타임일 경우 여기로 들어오게 된다.
		else
		{
			Logic();
		}
	}

	return (int)msg.wParam;
}

void CEngine::Logic()
{
	if (!m_Start)
	{
		m_Start = true;
		CSceneManager::GetInst()->Start();
	}

	m_Timer->Update();

	m_SlowTime += m_Timer->GetDeltaTime();
	m_ElapsedTime += m_Timer->GetDeltaTime();

	if (m_Slow && m_SlowTime >= m_SlowDuration)
	{
		m_Slow = false;
		m_SlowTime = 0.0f;
	}

	if (m_ElapsedTime < (1.0f / 60.0f))
		return;

	m_ElapsedTime = m_ElapsedTime - (1.0f / 60.0f);

	float deltaTime = 1.0f / 60.0f;

	if (!m_Play)
		deltaTime = 0.0f;

	if (m_Slow)
		deltaTime *= m_SlowScale;

	m_GlobalAccTime += deltaTime;

	m_GlobalCBuffer->SetDeltaTime(deltaTime);
	m_GlobalCBuffer->SetAccTime(m_GlobalAccTime);

	m_GlobalCBuffer->UpdateCBuffer();

	CIMGUIManager::GetInst()->Update(deltaTime);

	CInput::GetInst()->Update(deltaTime);

	if (Update(deltaTime))
		return;

	if (PostUpdate(deltaTime))
		return;

	Render(deltaTime);
}

bool CEngine::Update(float deltaTime)
{
	CResourceManager::GetInst()->Update();

	if (CSceneManager::GetInst()->Update(deltaTime))
		return true;

	if (m_MouseWidget[(int)m_MouseState])
	{
		// 마우스 위치를 얻어온다.
		Vector2 mousePos = CInput::GetInst()->GetMousePos();

		// 마우스가 위도우 창을 벗어났다면 보이게 한다.
		if (mousePos.x < 0.0f || mousePos.x > m_RS.width || mousePos.y < 0.0f || mousePos.y > m_RS.height)
		{
			if (m_ShowCursorCount < 0)
			{
				ShowCursor(true);
				m_ShowCursorCount = 0;
			}
		}
		else
		{
			if (m_ShowCursorCount == 0)
			{
				ShowCursor(false);
				m_ShowCursorCount--;
			}
		}

		mousePos.y -= m_MouseWidget[(int)m_MouseState]->GetWindowSize().y;


		m_MouseWidget[(int)m_MouseState]->SetPos(mousePos);
		m_MouseWidget[(int)m_MouseState]->Update(deltaTime);
	}

	return false;
}

bool CEngine::PostUpdate(float deltaTime)
{
	if (CSceneManager::GetInst()->PostUpdate(deltaTime))
		return true;

	if (m_MouseWidget[(int)m_MouseState])
	{
		m_MouseWidget[(int)m_MouseState]->PostUpdate(deltaTime);
	}

	return false;
}

bool CEngine::Render(float deltaTime)
{
	CDevice::GetInst()->RenderStart();
	CDevice::GetInst()->ClearRenderTarget(m_ClearColors);
	CDevice::GetInst()->ClearDepthStencil(1.0f, 0);

	//// 테스트
	//// 메쉬와 쉐이더를 가져온다.
	//CMesh* mesh = CResourceManager::GetInst()->FindMesh("SpriteMesh");
	//CShader* shader = CResourceManager::GetInst()->FindShader("ColorMeshShader");

	//// 항상 메쉬를 그리기전에 사용할 쉐이더를 설정한다.
	//shader->SetShader();

	//// 그리고 그린다.
	//mesh->Render();

	// Update와 PostUpdate를 통해서 업데이트된 월드 행렬을 기반으로 출력한다.
	CRenderManager::GetInst()->Render();

	CIMGUIManager::GetInst()->Render();

	CDevice::GetInst()->Flip();

	return true;
}

ATOM CEngine::Register(const TCHAR* name, int iconID)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	// 작업영역이 바뀔때 다시 그리게끔 해준다. 가로와 세로 모두 다시 그린다.
	wcex.style = CS_HREDRAW | CS_VREDRAW;

	// 메세지가 큐에 들어왔을때 해당 메세지를 꺼내오고 꺼내온 메세지를 인자로 전달해서
	// 호출해줄 함수를 지정한다.
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;

	// Window Instance를 지정한다.
	wcex.hInstance = m_hInst;

	// 실행파일이 만들어질때 사용할 아이콘 이미지를 지정한다.
	wcex.hIcon = LoadIcon(m_hInst, MAKEINTRESOURCE(iconID));


	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	// 유니코드 문자열을 사용할때 L"" 을 붙여서 사용을 한다.
	// TEXT 매크로는 "" 앞에 L 을 붙여서 L"" 로 만들어준다.
	// 유니코드일때는 이렇게 해주고 multibyte  일때는 그냥 "" 로 만들어준다.
	wcex.lpszClassName = name;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(iconID));

	return RegisterClassExW(&wcex);
}

BOOL CEngine::Create(const TCHAR* name)
{
	m_hWnd = CreateWindowW(name, name, WS_OVERLAPPEDWINDOW,
		0, 0, m_RS.width, m_RS.height, nullptr, nullptr, m_hInst, nullptr);

	if (!m_hWnd)
	{
		return FALSE;
	}
	
	// 클라이언트 영역을 1280, 720으로 만들어준다.
	// RECT : 사각형을 표현하는 구조체이다.
	// left, top, right, bottom 4개의 값으로 구성되어 있다.
	RECT    rc = { 0, 0, (LONG)m_RS.width, (LONG)m_RS.height };

	// 1번인자 : 클라이언트 영역의 크기를 전달한다.
	// 이 함수가 완료되면 rc에는 실제 클라이언트 영역이 원하는 크기 될 수 있는
	// 전체 윈도우의 left, top, right, bottom 정보가 들어오게 된다.
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	// 위에서 얻어온 Rect를 이용해서 윈도우 크기를 지정한다.
	SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, rc.right - rc.left,
		rc.bottom - rc.top,
		SWP_NOZORDER);

	// SW_SHOW : 윈도우창을 화면에 보여준다.
	// SW_HIDE : 창을 만들긴 하지만 화면에 보여주는건 아니다.
	ShowWindow(m_hWnd, SW_SHOW);

	// UpdateWindow 함수는 갱신할 부분이 있다면 갱신하라는 명령을 내려주는 함수이다.
	UpdateWindow(m_hWnd);

	return TRUE;
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CEngine::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return 0;

	switch (message)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;

		// HDC : 화면에 출력하기 위한 그리기 도구이다.
		HDC hdc = BeginPaint(hWnd, &ps);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		m_Loop = false;
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
