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

	// �� ������ ���̴� ���α׷��� : �ڿ� �ٴ� �̸��� �ǹ��Ѵ�.
	desc.SemanticName = name;
	// �� ������ �������� ������ �ǹ��Ѵ� : POSITION0���� 0��
	desc.SemanticIndex = index;
	// �����ʹ� ��� ���·� �̷��� �ִ���
	desc.Format = fmt;
	desc.InputSlot = slot;
	desc.InputSlotClass = slotClass;
	desc.InstanceDataStepRate = instanceDataStep;

	// ���� m_InputSize = 0�̴� 0��° ���� �����Ѵ�.
	desc.AlignedByteOffset = m_InputSize;

	// ũ�⸸ŭ �����ش� ���� ������ AddInputDesc�� ȣ��� ��Ȯ�� ��ġ���� AlignedByteOffset�� �����ȴ�.
	m_InputSize += size;

	m_vecInputDesc.push_back(desc);
}

bool CGraphicShader::CreateInputLayout()
{
	// ��ǲ���̾ƿ��� 0������ �����Ͽ� ��ü ũ�⸸ŭ�� ���ؽ� ������ m_InputLayout�� �����Ѵ�.
	// GPU�� ��� �����ͱ����� � ������������ �𸣱⿡ �̷��� ���̾ƿ��� �����ؾ���
	// ���ؽ� ���̴��� ���� ó������ ����Ǵ� ���̴��̱⿡ ���ؽ� ���̴��� ���̾ƿ��� �����Ѵ�.
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
	// ���� �����ϰ� �����ִ� �����͸� ���������ش�.
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
		// �����Ͻ� ������ �߻�������
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// �����ϵ� �ڵ�� Shader�� ������.
	if(FAILED(CDevice::GetInst()->GetDevice()->CreateVertexShader(m_VSBlob->GetBufferPointer(), m_VSBlob->GetBufferSize(), nullptr,
		&m_VS)))
		return false;

	return true;
}

bool CGraphicShader::LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// ���� �����ϰ� �����ִ� �����͸� ���������ش�.
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
		// �����Ͻ� ������ �߻�������
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// �����ϵ� �ڵ�� Shader�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreatePixelShader(m_PSBlob->GetBufferPointer(), m_PSBlob->GetBufferSize(), nullptr,
		&m_PS)))
		return false;

	return true;
}

bool CGraphicShader::LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// ���� �����ϰ� �����ִ� �����͸� ���������ش�.
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
		// �����Ͻ� ������ �߻�������
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// �����ϵ� �ڵ�� Shader�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateHullShader(m_HSBlob->GetBufferPointer(), m_HSBlob->GetBufferSize(), nullptr,
		&m_HS)))
		return false;

	return true;
}

bool CGraphicShader::LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// ���� �����ϰ� �����ִ� �����͸� ���������ش�.
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
		// �����Ͻ� ������ �߻�������
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// �����ϵ� �ڵ�� Shader�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateDomainShader(m_DSBlob->GetBufferPointer(), m_DSBlob->GetBufferSize(), nullptr,
		&m_DS)))
		return false;

	return true;
}

bool CGraphicShader::LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	// ���� �����ϰ� �����ִ� �����͸� ���������ش�.
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
		// �����Ͻ� ������ �߻�������
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	// �����ϵ� �ڵ�� Shader�� ������.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateGeometryShader(m_GSBlob->GetBufferPointer(), m_GSBlob->GetBufferSize(), nullptr,
		&m_GS)))
		return false;

	return true;
}
