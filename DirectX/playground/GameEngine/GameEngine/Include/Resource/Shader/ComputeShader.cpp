#include "ComputeShader.h"
#include "../../PathManager.h"
#include "../../Device.h"

CComputeShader::CComputeShader() :
	m_CS(nullptr),
	m_CSBlob(nullptr),
	m_EntryName{},
	m_FileName{}
{
	m_Type = Shader_Type::Compute;
}

CComputeShader::CComputeShader(const CComputeShader& shader) :
	CShader(shader)
{
	m_CS = nullptr;
	m_CSBlob = nullptr;

	memcpy(m_EntryName, shader.m_EntryName, 64);
	memcpy(m_FileName, shader.m_FileName, MAX_PATH);
	m_PathName = shader.m_PathName;

	LoadComputeShader(shader.m_EntryName, shader.m_FileName, shader.m_PathName);
}

CComputeShader::~CComputeShader()
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);
}

void CComputeShader::SetShader()
{
	CDevice::GetInst()->GetContext()->CSSetShader(m_CS, nullptr, 0);
}

CComputeShader* CComputeShader::Clone()
{
	return nullptr;
}

bool CComputeShader::LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName)
{
	SAFE_RELEASE(m_CS);
	SAFE_RELEASE(m_CSBlob);

	strcpy_s(m_EntryName, entryName);
	lstrcpy(m_FileName, fileName);
	m_PathName = pathName;

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

	if (FAILED(D3DCompileFromFile(fullPath, nullptr, 
		D3D_COMPILE_STANDARD_FILE_INCLUDE, entryName, "cs_5_0", flag, 0, &m_CSBlob, &error)))
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		OutputDebugStringA("\n");

		return false;
	}

	if (FAILED(CDevice::GetInst()->GetDevice()->CreateComputeShader(m_CSBlob->GetBufferPointer(),
		m_CSBlob->GetBufferSize(), nullptr, &m_CS)))
		return false;

	return true;

}

void CComputeShader::Execute(unsigned int x, unsigned int y, unsigned int z)
{
	// ComputeShader ½ÇÇà
	SetShader();
	CDevice::GetInst()->GetContext()->Dispatch(x, y, z);
	CDevice::GetInst()->GetContext()->CSSetShader(nullptr, nullptr, 0);
}
