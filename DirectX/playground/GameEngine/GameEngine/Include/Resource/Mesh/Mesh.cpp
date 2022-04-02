#include "Mesh.h"
#include "../../Device.h"

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecContainer[i]);
	}
}

bool CMesh::CreateMesh(void* vtxData, int size, int count, D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt)
{
	sMeshContainer* container = new sMeshContainer;

	container->VB.Size = size;
	container->VB.Count = count;
	container->Primitive = primitive;

	if (!CreateBuffer(Buffer_Type::Vertex, vtxData, size, count, usage, &container->VB.Buffer))
		return false;


	if (idxData != nullptr)
	{
		container->vecIB.resize(1);

		container->vecIB[0].Size = idxSize;
		container->vecIB[0].Count = idxCount;
		container->vecIB[0].Fmt = fmt;

		if (!CreateBuffer(Buffer_Type::Index, idxData, idxSize, idxCount, idxUsage, &container->vecIB[0].Buffer))
			return false;
	}

	m_vecContainer.push_back(container);

	return true;
}

bool CMesh::Init()
{
	return true;
}

bool CMesh::CreateBuffer(Buffer_Type type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer)
{
	// ���� ������ ����
	D3D11_BUFFER_DESC desc = {};

	// ������ �� ũ��
	desc.ByteWidth = size * count;

	// ������ ���뵵
	desc.Usage = usage;

	if (type == Buffer_Type::Vertex)
	{
		// �ش� ���۴� ���ؽ� ���۷� ���ε��Ѵ�.
		desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	}
	else
	{
		// �ش� ���۴� �ε��� ���۷� ���ε��Ѵ�.
		desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	}

	if (usage == D3D11_USAGE_DYNAMIC)
	{
		// ���̳����Ͻ� CPU�� �� ���ۿ� �� �� �ִ�.
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	else if (usage == D3D11_USAGE_STAGING)
	{
		// ������¡�� ��� �а� ���� �� �� ����
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
	}

	// ���۸� ������ �ʱ�ȭ�Ҷ� ���� ��
	D3D11_SUBRESOURCE_DATA bufferData = {};

	// �����͸� �ִ´�.
	bufferData.pSysMem = data;

	// �ش� ������ bufferData�� �ʱⰪ���� �Ͽ� buffer�� �����Ѵ�.
	if (FAILED(CDevice::GetInst()->GetDevice()->CreateBuffer(&desc, &bufferData, buffer)))
		return false;

	if (type == Buffer_Type::Vertex)
	{
		// char�� �ּ� ���� 1����Ʈ, ����
		// data�� 1����Ʈ�� ��ȸ�� ����������.
		char* vertexData = (char*)data;

		for (int i = 0; i < count; i++)
		{
			// ù 1����Ʈ ������������ 3����Ʈ�� Vector3Ÿ���� �����´�.
			Vector3* pos = (Vector3*)vertexData;
			// ������ ������ ���� �ش� vertexData�� ���ؽ� �ϳ��� ũ�⸸ŭ �ǳʶڴ�.
			vertexData += size;

			if (m_Min.x > pos->x)
				m_Min.x = pos->x;

			if (m_Min.y > pos->y)
				m_Min.y = pos->y;

			if (m_Min.z > pos->z)
				m_Min.z = pos->z;

			if (m_Max.x < pos->x)
				m_Max.x = pos->x;

			if (m_Max.y < pos->y)
				m_Max.y = pos->y;

			if (m_Max.z < pos->z)
				m_Max.z = pos->z;
		}
	}

	return true;
}

void CMesh::Render()
{
	// ���� �޽��� �ִ� ��� �޽� �����̳ʵ��� ��ȸ�Ѵ�.
	// �𵨿� ���� ������� �������� �������� �ٸ� ���׸����� ���� ���� �ֱ� ����
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		// ��Ʈ���̵�: ���۾ȿ� �ִ� �����Ͱ� �����Ʈ ������ �̷��������.
		unsigned int stride = m_vecContainer[i]->VB.Size;
		// ������: ������ġ�� �����Ѵ�.
		unsigned int offset = 0;

		// IA ���������� �ַ� ��ġ�� ���ۿ� ���� ������ �Ѵ�.
		// �� �޽��� �׸��� � ������� �׸����� ����
		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		// �޽������̳ʿ� �ִ� ������ ���´�.
		// ���ؽ� ���۴� �ϳ�
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, &m_vecContainer[i]->VB.Buffer, &stride, &offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		// �ε����� ������ ���
		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				// �ε��� ���۵� ���� �����ش�.
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				// �׸��� �ε����� ������� �׸� �� �ְ� �����Ѵ�.
				// �ε����� ������ ����� �ε����� ������ġ��
				// ���̽� ���ؽ� ������ ��ġ
				CDevice::GetInst()->GetContext()->DrawIndexed(m_vecContainer[i]->vecIB[j].Count, 0, 0);
			}
		}
		// �ε����� �������� ���� ���
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->Draw(m_vecContainer[i]->VB.Count, 0);
		}
	}
}

void CMesh::RenderInstancing(int count)
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		unsigned int stride = (unsigned int)m_vecContainer[i]->VB.Size;
		unsigned int offset = 0;

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 1, 
			&m_vecContainer[i]->VB.Buffer, &stride, &offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(m_vecContainer[i]->vecIB[j].Count, count, 0, 0, 0);
			}
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN,
				0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecContainer[i]->VB.Count, count, 0, 0);
		}
	}
}

void CMesh::RenderInstancing(ID3D11Buffer* instancingBuffer, unsigned int instanceSize, int count)
{
	size_t size = m_vecContainer.size();

	for (size_t i = 0; i < size; i++)
	{
		unsigned int stride[2] = { (unsigned int)m_vecContainer[i]->VB.Size, instanceSize };
		unsigned int offset[2] = {};

		ID3D11Buffer* buffer[2] = { m_vecContainer[i]->VB.Buffer, instancingBuffer };

		CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(m_vecContainer[i]->Primitive);
		CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 2, buffer, stride, offset);

		size_t idxCount = m_vecContainer[i]->vecIB.size();

		if (idxCount > 0)
		{
			for (size_t j = 0; j < idxCount; j++)
			{
				CDevice::GetInst()->GetContext()->IASetIndexBuffer(
					m_vecContainer[i]->vecIB[j].Buffer,
					m_vecContainer[i]->vecIB[j].Fmt, 0);
				CDevice::GetInst()->GetContext()->DrawIndexedInstanced(
					m_vecContainer[i]->vecIB[j].Count, count, 0, 0, 0
				);
			}
		}
		else
		{
			CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);
			CDevice::GetInst()->GetContext()->DrawInstanced(m_vecContainer[i]->VB.Count, count, 0, 0);
		}
	}
}
