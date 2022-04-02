#include "GraphicShader.h"
#include "../../Device.h"
#include "../../PathManager.h"


CGraphicShader::CGraphicShader() :
	m_InputLayout(nullptr),
	m_InputSize(0),
	m_VS(nullptr),
	m_PS(nullptr),
	m_HS(nullptr),
	m_DS(nullptr),
	m_GS(nullptr),
	m_VSBlob(nullptr),
	m_PSBlob(nullptr),
	m_HSBlob(nullptr),
	m_DSBlob(nullptr),
	m_GSBlob(nullptr)
{
	m_Type = Shader_Type::Graphic;
}

CGraphicShader::CGraphicShader(const CGraphicShader& shader)
{
	m_InputLayout = nullptr;
	m_InputSize = shader.m_InputSize;
	m_VS = nullptr;
	m_PS = nullptr;
	m_HS = nullptr;
	m_DS = nullptr;
	m_GS = nullptr;
	m_VSBlob = nullptr;
	m_PSBlob = nullptr;
	m_HSBlob = nullptr;
	m_DSBlob = nullptr;
	m_GSBlob = nullptr;
}

CGraphicShader::~CGraphicShader()
{
	SAFE_RELEASE(m_InputLayout);

	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_GS);

	SAFE_RELEASE(m_VSBlob);
	SAFE_RELEASE(m_PSBlob);
	SAFE_RELEASE(m_HSBlob);
	SAFE_RELEASE(m_DSBlob);
	SAFE_RELEASE(m_GSBlob);
}

void CGraphicShader::AddInputDesc(const char* name, unsigned int index, DXGI_FORMAT fmt, unsigned int slot, unsigned int size, D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStep)
{
	D3D11_INPUT_ELEMENT_DESC desc = {};

	// 이 변수가 쉐이더 프로그램에 : 뒤에 붙는 이름을 의미한다.
	desc.SemanticName = name;
	// 이 변수는 레지스터 슬롯을 의미한다 : POSITION0에서 0번
	desc.SemanticIndex = index;
	// 데이터는 어떠한 형태로 이뤄져 있는지
	desc.Format = fmt;
	desc.InputSlot = slot;
	desc.InputSlotClass = slotClass;
	desc.InstanceDataStepRate = instanceDataStep;

	// 현재 m_InputSize = 0이다 0번째 부터 시작한다.
	desc.AlignedByteOffset = m_InputSize;

	// 크기만큼 더해준다 따라서 다음에 AddInputDesc를 호출시 정확한 위치에서 AlignedByteOffset이 설정된다.
	m_InputSize += size;

	m_vecInputDesc.push_back(desc);
}

bool CGraphicShader::CreateInputLayout()
{
	// 인풋레이아웃의 0번부터 시작하여 전체 크기만큼의 버텍스 정보를 m_InputLayout에 저장한다.
	// GPU는 어디서 어디부터까지가 어떤 데이터인지를 모르기에 이러한 레이아웃을 설정해야함
	// 버텍스 쉐이더가 가장 처음으로 적용되는 쉐이더이기에 버텍스 쉐이더로 레이아웃을 설정한다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateInputLayout(&m_vecInputDesc[0],
		(UINT)m_vecInputDesc.size(), m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(),
		&m_InputLayout)))
		return false;

	return true;
}

void CGraphicShader::SetShader()
{
	CDevice::GetInst()->GetContext()->VSSetShader(m_VS, nullptr, 0);
	CDevice::GetInst()->GetContext()->PSSetShader(m_PS, nullptr, 0);
	CDevice::GetInst()->GetContext()->HSSetShader(m_HS, nullptr, 0);
	CDevice::GetInst()->GetContext()->DSSetShader(m_DS, nullptr, 0);
	CDevice::GetInst()->GetContext()->GSSetShader(m_GS, nullptr, 0);

	CDevice::GetInst()->GetContext()->IASetInputLayout(m_InputLayout);
}

CGraphicShader* CGraphicShader::Clone()
{
	return nullptr;
}

bool CGraphicShader::LoadVertexShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// 먼저 안전하게 묶여있는 데이터를 릴리즈해준다.
	SAFE_RELEASE(m_VS);
	SAFE_RELEASE(m_VSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "vs_5_0", flag, 0, &m_VSBlob,
		&error)))
	{
		// 컴파일시 에러가 발생했을시
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 컴파일된 코드로 Shader를 만들어낸다.
	if(FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr,
		&m_VS)))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// 먼저 안전하게 묶여있는 데이터를 릴리즈해준다.
	SAFE_RELEASE(m_PS);
	SAFE_RELEASE(m_PSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "ps_5_0", flag, 0, &m_PSBlob,
		&error)))
	{
		// 컴파일시 에러가 발생했을시
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 컴파일된 코드로 Shader를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr,
		&m_PS)))
		return false;

	return true;
}

bool CGraphicShader::LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// 먼저 안전하게 묶여있는 데이터를 릴리즈해준다.
	SAFE_RELEASE(m_HS);
	SAFE_RELEASE(m_HSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "hs_5_0", flag, 0, &m_HSBlob,
		&error)))
	{
		// 컴파일시 에러가 발생했을시
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 컴파일된 코드로 Shader를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(), nullptr,
		&m_HS)))
		return false;

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// 먼저 안전하게 묶여있는 데이터를 릴리즈해준다.
	SAFE_RELEASE(m_DS);
	SAFE_RELEASE(m_DSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "ds_5_0", flag, 0, &m_DSBlob,
		&error)))
	{
		// 컴파일시 에러가 발생했을시
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 컴파일된 코드로 Shader를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(), nullptr,
		&m_DS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// 먼저 안전하게 묶여있는 데이터를 릴리즈해준다.
	SAFE_RELEASE(m_GS);
	SAFE_RELEASE(m_GSBlob);

	unsigned int flag = 0;

#ifdef _DEBUG
	flag = D3DCOMPILE_DEBUG;
#endif // _DEBUG

	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	ID3DBlob* error = nullptr;

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "gs_5_0", flag, 0, &m_GSBlob,
		&error)))
	{
		// 컴파일시 에러가 발생했을시
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// 컴파일된 코드로 Shader를 만들어낸다.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr,
		&m_GS)))
		return false;

	return true;
}
