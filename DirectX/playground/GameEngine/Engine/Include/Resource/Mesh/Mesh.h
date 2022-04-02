#pragma once

#include "../../Ref.h"

/// <summary>
/// �޽� �⺻ Ŭ����
/// </summary>
class CMesh :
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	std::vector<sMeshContainer*> m_vecContainer;

	// ���� �޽��� x, y, z �ּ����� ����
	Vector3 m_Min;
	// ���� �޽��� x, y, z �ִ����� ����
	Vector3 m_Max;

public:
	// ���ؽ��� �ε��� ���۸� �����ؼ� �޽��� ������ �Լ�
	virtual bool CreateMesh(void* vtxData, int size, int count, D3D11_USAGE usage,
		D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	virtual bool Init();
	virtual void Render();

	// �ν��Ͻ��� �̿��� ������
	virtual void RenderInstancing(int count);
	virtual void RenderInstancing(ID3D11Buffer* instancingBuffer, unsigned int instanceSize, int count);

protected:
	// ���۸� �����Ѵ�.
	// �����ʹ� void* �� �޾Ƽ� ��� �����͵� ���� �� �ְ� �Ѵ�.
	bool CreateBuffer(Buffer_Type type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer);

};

