#include "SpriteMesh.h"

CSpriteMesh::CSpriteMesh()
{
	SetTypeID<CSpriteMesh>();
}

CSpriteMesh::~CSpriteMesh()
{
}

bool CSpriteMesh::Init()
{
	sMeshContainer* container = new sMeshContainer;

	// 버텍스 버퍼의 버텍스 하나당 크기를 정한다.
	container->VB.Size = sizeof(sVertexUV);

	// 버텍스의 갯수는 4개
	container->VB.Count = 4;

	// 그릴때 방법을 정한다.
	container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//// 4개의 정점과 색깔 정보를 가지고 있는 버텍스 데이터를 만든다.
	//sVertexColor vertices[4] =
	//{
	//	sVertexColor(Vector3(0.0f, 1.0f, 0.0f), Vector4::Red), // 왼쪽 위
	//	sVertexColor(Vector3(1.0f, 1.0f, 0.0f), Vector4::Green), // 오른쪽 위
	//	sVertexColor(Vector3(0.0f, 0.0f, 0.0f), Vector4::Blue), // 왼쪽 아래
	//	sVertexColor(Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f)) // 오른쪽 아래
	//};

	// 스프라이트 메쉬는 이제 텍스쳐를 가진다 따라서 위의 칼라 메쉬와 달리
	// UV 좌표를 전달한다.
	// 가운데가 0.0, 0.0, 0.0 이다.
	sVertexUV vertices[4] =
	{
		sVertexUV(Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)), // 왼쪽 위
		sVertexUV(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)), // 오른쪽 위
		sVertexUV(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)), // 왼쪽 하단
		sVertexUV(Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)) // 오른쪽 하단
	};

	if (!CreateBuffer(Buffer_Type::Vertex, vertices, sizeof(sVertexUV), 4, D3D11_USAGE_IMMUTABLE,
		&container->VB.Buffer))
		return false;

	container->vecIB.resize(1);

	// 인덱스 버퍼의 사이즈를 정한다.
	// 여기서는 2바이트로 설정한다 따라서 최대 2^16개의 인덱스를 가질 수 있다.
	container->vecIB[0].Size = 2;

	// 인덱스의 갯수
	container->vecIB[0].Count = 6;

	// 인덱스 버퍼의 포맷은 2바이트 언사인드 타입
	container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	unsigned short indices[6] = { 0, 1, 3, 0, 3, 2 };

	// Buffer에 해당 인덱스 배열의 데이터를 집어넣어 생성한다.
	if (!CreateBuffer(Buffer_Type::Index, indices, sizeof(unsigned short), 6, D3D11_USAGE_IMMUTABLE, &container->vecIB[0].Buffer))
		return false;

	m_vecContainer.push_back(container);

	return true;
}
