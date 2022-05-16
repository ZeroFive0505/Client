#include "ColliderShader3D.h"

CColliderShader3D::CColliderShader3D()
{
	SetTypeID<CColliderShader3D>();
}

CColliderShader3D::~CColliderShader3D()
{
}

bool CColliderShader3D::Init()
{
	if (!LoadVertexShader("Collider3DMeshVS", TEXT("Collider3D.fx"), SHADER_PATH))
		return false;

	if (!LoadPixelShader("Collider3DMeshPS", TEXT("Collider3D.fx"), SHADER_PATH))
		return false;

	AddInputDesc("POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0);

	if (!CreateInputLayout())
		return false;

	return true;
}
