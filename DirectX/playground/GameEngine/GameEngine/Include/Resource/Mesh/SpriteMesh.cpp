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

	// ���ؽ� ������ ���ؽ� �ϳ��� ũ�⸦ ���Ѵ�.
	container->VB.Size = sizeof(sVertexUV);

	// ���ؽ��� ������ 4��
	container->VB.Count = 4;

	// �׸��� ����� ���Ѵ�.
	container->Primitive = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	//// 4���� ������ ���� ������ ������ �ִ� ���ؽ� �����͸� �����.
	//sVertexColor vertices[4] =
	//{
	//	sVertexColor(Vector3(0.0f, 1.0f, 0.0f), Vector4::Red), // ���� ��
	//	sVertexColor(Vector3(1.0f, 1.0f, 0.0f), Vector4::Green), // ������ ��
	//	sVertexColor(Vector3(0.0f, 0.0f, 0.0f), Vector4::Blue), // ���� �Ʒ�
	//	sVertexColor(Vector3(1.0f, 0.0f, 0.0f), Vector4(1.0f, 0.0f, 1.0f, 1.0f)) // ������ �Ʒ�
	//};

	// ��������Ʈ �޽��� ���� �ؽ��ĸ� ������ ���� ���� Į�� �޽��� �޸�
	// UV ��ǥ�� �����Ѵ�.
	// ����� 0.0, 0.0, 0.0 �̴�.
	sVertexUV vertices[4] =
	{
		sVertexUV(Vector3(0.0f, 1.0f, 0.0f), Vector2(0.0f, 0.0f)), // ���� ��
		sVertexUV(Vector3(1.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)), // ������ ��
		sVertexUV(Vector3(0.0f, 0.0f, 0.0f), Vector2(0.0f, 1.0f)), // ���� �ϴ�
		sVertexUV(Vector3(1.0f, 0.0f, 0.0f), Vector2(1.0f, 1.0f)) // ������ �ϴ�
	};

	if (!CreateBuffer(Buffer_Type::Vertex, vertices, sizeof(sVertexUV), 4, D3D11_USAGE_IMMUTABLE,
		&container->VB.Buffer))
		return false;

	container->vecIB.resize(1);

	// �ε��� ������ ����� ���Ѵ�.
	// ���⼭�� 2����Ʈ�� �����Ѵ� ���� �ִ� 2^16���� �ε����� ���� �� �ִ�.
	container->vecIB[0].Size = 2;

	// �ε����� ����
	container->vecIB[0].Count = 6;

	// �ε��� ������ ������ 2����Ʈ ����ε� Ÿ��
	container->vecIB[0].Fmt = DXGI_FORMAT_R16_UINT;

	unsigned short indices[6] = { 0, 1, 3, 0, 3, 2 };

	// Buffer�� �ش� �ε��� �迭�� �����͸� ����־� �����Ѵ�.
	if (!CreateBuffer(Buffer_Type::Index, indices, sizeof(unsigned short), 6, D3D11_USAGE_IMMUTABLE, &container->vecIB[0].Buffer))
		return false;

	m_vecContainer.push_back(container);

	return true;
}
