#include "ColorMeshShader.h"

CColorMeshShader::CColorMeshShader()
{
	SetTypeID<CColorMeshShader>();
}

CColorMeshShader::~CColorMeshShader()
{
}

bool CColorMeshShader::Init()
{
	// 버텍스 쉐이더의 진입점과 파일 이름을 전달
	if (!LoadVertexShader("ColorMeshVS", TEXT("ColorMesh.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("ColorMeshPS", TEXT("ColorMesh.fx"), SHADER_PATH))
		return false;

	// 시멘틱 이름과 슬롯번호와 파일의 포맷 그리고 크기를 전달해준다.
	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);
	AddInputDesc("COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}
