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


	// ���� ����
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
	// Red 8, Green 8, Blue 8 Alpha 8 ��Ʈ�̸� 0 ~ 1 ������ ����ȭ�� ���� ������ ����
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 1;
	swapDesc.BufferDesc.RefreshRate.Denominator = 60;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	// ������ ����
	swapDesc.BufferCount = 1;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = hWnd;
	// ��Ƽ �ٸ����
	swapDesc.SampleDesc.Count = sampleCount;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.Windowed = windowMode;
	// �ø��ν� ������� ������ ������.
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


	// SwapChain�� ������ �ִ� ����۸� ���´�.
	ID3D11Texture2D* BackBuffer = nullptr;

	// ����۸� �����´�. ������� �뵵 �ؽ���2D
	m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&BackBuffer);

	// BackBuffer�� ����� RenderTargetView�� �����Ѵ�.
	// �ι�° ���ڷδ� �ʱ�ȭ ���� ������� �� �ִ�.
	m_Device->CreateRenderTargetView(BackBuffer, nullptr, &m_TargetView);

	SAFE_RELEASE(BackBuffer);

	// ���� ���۸� �����.
	D3D11_TEXTURE2D_DESC depthDesc = {};

	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.ArraySize = 1;

	// ���� 24��Ʈ ���ٽ� 8��Ʈ
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	// �� 4���� Ÿ���� ����
	/*
		D3D11_USAGE_DEFAULT	= 0,
        D3D11_USAGE_IMMUTABLE	= 1,
        D3D11_USAGE_DYNAMIC	= 2,
        D3D11_USAGE_STAGING	= 3

		DEFAULT: GPU�� �б�/���� ��� CPU�� ������ �Ұ���
		IMMUTABLE: GPU�� �б� ���, CPU�� ������ �Ұ����ϸ� �ʱ�ȭ�� ���� �־������
		DYNAMIC: GPU�� �б� ���, CPU������ ���� ���(Map�Լ��� �̿��Ͽ� LOCK, UNLOCK) �ӵ��� ������.
		STAGING: GPU �޸𸮿��� CPU�޸𸮷� ���縦 ��� �ӵ��� ���� ������.
	*/
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.SampleDesc.Count = sampleCount;
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.MipLevels = 1;

	ID3D11Texture2D* depthBuffer;

	m_Device->CreateTexture2D(&depthDesc, nullptr, &depthBuffer);

	m_Device->CreateDepthStencilView(depthBuffer, nullptr, &m_DepthView);

	SAFE_RELEASE(depthBuffer);

	// ����Ʈ�� ����
	D3D11_VIEWPORT VP = {};

	// �ػ󵵴� ����
	VP.Width = (float)width;
	VP.Height = (float)height;
	VP.MaxDepth = 1.0f;

	// 1�� ���� ����Ʈ�� ����
	m_Context->RSSetViewports(1, &VP);

	// 2D Factory ����
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &m_2DFactory)))
		return false;

	// 3D BackBuffer�� Surface�� ���´�.
	// �ؽ��ĳ� �ȼ��� ���� ������ �ִ� �����ʹ� ���ǽ��� ������ �ִ�.
	IDXGISurface* backSurface = nullptr;

	m_SwapChain->GetBuffer(0, IID_PPV_ARGS(&backSurface));

	// 2D�� ����Ÿ���� ������ش�.
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_HARDWARE,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));

	if (FAILED(m_2DFactory->CreateDxgiSurfaceRenderTarget(backSurface, props, &m_2DTarget)))
		return false;

	// ���� Ƚ���� �ٿ��ش�.
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
	// OM ���� ���
	m_Context->OMSetRenderTargets(1, &m_TargetView, m_DepthView);
}

void CDevice::Flip()
{
	// ������ �ø���
	m_SwapChain->Present(0, 0);
}

Vector2 CDevice::GetViewportAspectRatio() const
{
	RECT clientRect = {};

	GetClientRect(m_hWnd, &clientRect);

	// â ũ�⸦ �ٲ��� ��� ���Ӱ� ũ�⸦ ���Ѵ�.
	float width = (float)(clientRect.right - clientRect.left);
	float height = (float)(clientRect.bottom - clientRect.top);

	// �ٲ� �ػ��� ������ ���ؼ� ��ȯ�Ѵ�.
	return Vector2(m_RS.width / width, m_RS.height / height);
}

