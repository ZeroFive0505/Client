#include "Device.h"

DEFINITION_SINGLE(CDevice)

CDevice::CDevice() :
	m_Device(nullptr),
	m_Context(nullptr),
	m_SwapChain(nullptr),
	m_TargetView(nullptr),
	m_DepthView(nullptr),
	m_2DTarget(nullptr),
	m_2DTargetWorld(nullptr),
	m_2DFactory(nullptr)
{

}

CDevice::~CDevice()
{
	SAFE_RELEASE(m_2DTarget);
	SAFE_RELEASE(m_2DTargetWorld);
	SAFE_RELEASE(m_2DFactory);

	SAFE_RELEASE(m_TargetView);
	SAFE_RELEASE(m_DepthView);

	SAFE_RELEASE(m_SwapChain);

	if (m_Context)
		m_Context->ClearState();

	SAFE_RELEASE(m_Context);
	SAFE_RELEASE(m_Device);
}

bool CDevice::Init(HWND hWnd, unsigned int width, unsigned int height, bool windowMode)
{
	m_hWnd = hWnd;
	m_RS.width = width;
	m_RS.height = height;

	unsigned int Flag = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#ifdef _DEBUG
	Flag |= D3D11_CREATE_DEVICE_DEBUG;
#endif // _DEBUG


	// 지원 버전
	D3D_FEATURE_LEVEL FLevel = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL FLevel1 = D3D_FEATURE_LEVEL_11_0;

	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, Flag,
		&FLevel, 1, D3D11_SDK_VERSION, &m_Device, &FLevel1, &m_Context)))
		return false;

	int sampleCount = 4;

	UINT check = 0;

	m_Device->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &check);

	if (check < 1)
		sampleCount = 1;

	DXGI_SWAP_CHAIN_DESC swapDesc = {};

	swapDesc.BufferDesc.Width = width;
	swapDesc.BufferDesc.Height = height;
	// Red 8, Green 8, Blue 8 Alpha 8 비트이며 0 ~ 1 사이의 정규화된 값을 가지는 포맷
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// 백퍼의 갯수
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	// 안티 앨리어싱
	swapDesc.SampleDesc.Count = sampleCount;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = windowMode;
	// 플립핑시 백버퍼의 내용은 버린다.
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	IDXGIDevice* DXGIDevice = nullptr;
	m_Device->QueryInterface(__uuidof(IDXGIDevice), (void**)&DXGIDevice);

	IDXGIAdapter* Adapter = nullptr;
	DXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&Adapter);

	IDXGIFactory* Factory = nullptr;
	Adapter->GetParent(__uuidof(IDXGIFactory), (void**)&Factory);

	if (FAILED(Factory->CreateSwapChain(m_Device, &swapDesc, &m_SwapChain)))
	{
		SAFE_RELEASE(DXGIDevice);
		SAFE_RELEASE(Adapter);
		SAFE_RELEASE(Factory);

		return false;
	}
	
	SAFE_RELEASE(DXGIDevice);
	SAFE_RELEASE(Adapter);
	SAFE_RELEASE(Factory);


	// SwapChain이 가지고 있는 백버퍼를 얻어온다.
	ID3D11Texture2D* BackBuffer = nullptr;

	// 백버퍼를 가져온다. 백버퍼의 용도 텍스쳐2D
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// BackBuffer와 연결된 RenderTargetView를 생성한다.
	// 두번째 인자로는 초기화 값을 집어넣을 수 있다.
	m_Device->CreateRenderTargetView(BackBuffer, nullptr, &m_TargetView);

	SAFE_RELEASE(BackBuffer);

	// 깊이 버퍼를 만든다.
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.ArraySize = 1;

	// 깊이 24비트 스텐실 8비트
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// 총 4가지 타입이 존재
	/*
		D3D11_USAGE_DEFAULT	= 0,
        D3D11_USAGE_IMMUTABLE	= 1,
        D3D11_USAGE_DYNAMIC	= 2,
        D3D11_USAGE_STAGING	= 3

		DEFAULT: GPU의 읽기/쓰기 허용 CPU는 접근이 불가능
		IMMUTABLE: GPU의 읽기 허용, CPU는 접근이 불가능하며 초기화시 값을 넣어줘야함
		DYNAMIC: GPU의 읽기 허용, CPU에서의 쓰기 허용(Map함수를 이용하여 LOCK, UNLOCK) 속도는 느리다.
		STAGING: GPU 메모리에서 CPU메모리로 복사를 허용 속도는 제일 느리다.
	*/
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = sampleCount;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.MipLevels = 1;

	ID3D11Texture2D* depthBuffer;

	m_Device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);

	m_Device->CreateDepthStencilView(depthBuffer, nullptr, &m_DepthView);

	SAFE_RELEASE(depthBuffer);

	// 뷰포트를 설정
	D3D11_VIEWPORT VP = {};

	// 해상도는 동일
	VP.Width = (float)width;
	VP.Height = (float)height;
	VP.MaxDepth = 1.0f;

	// 1은 현재 뷰포트의 갯수
	m_Context->RSSetViewports(1, &VP);

	// 2D Factory 생성
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
		return false;

	// 3D BackBuffer의 Surface를 얻어온다.
	// 텍스쳐나 픽셀의 값을 가지고 있는 데이터는 서피스를 가지고 있다.
	IDXGISurface* backSurface = nullptr;

	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backSurface));

	// 2D용 렌더타겟을 만들어준다.
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(backSurface, props, &m_2DTarget)))
		return false;

	// 참조 횟수를 줄여준다.
	SAFE_RELEASE(backSurface);

	return true;
}

void CDevice::ClearRenderTarget(float clearColors[4])
{
	m_Context->ClearRenderTargetView(m_TargetView, clearColors);
}

void CDevice::ClearDepthStencil(float depth, unsigned char stencil)
{
	m_Context->ClearDepthStencilView(m_DepthView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, depth, stencil);
}

void CDevice::RenderStart()
{
	// OM 최종 출력
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	// 페이지 플립핑
	m_SwapChain->Present(0, 0);
}

Vector2 CDevice::GetViewportAspectRatio() const
{
	RECT clientRect = {};

	GetClientRect(m_hWnd, &clientRect);

	// 창 크기를 바꿨을 경우 새롭게 크기를 구한다.
	float width = (float)(clientRect.right - clientRect.left);
	float height = (float)(clientRect.bottom - clientRect.top);

	// 바뀐 해상도의 비율을 구해서 반환한다.
	return Vector2(m_RS.width / width, m_RS.height / height);
}

