#pragma once

#include "../../Ref.h"

/// <summary>
/// 메쉬 기본 클래스
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

	// 현재 메쉬의 x, y, z 최소점을 저장
	Vector3 m_Min;
	// 현재 메쉬의 x, y, z 최대점을 저장
	Vector3 m_Max;

public:
	// 버텍스와 인덱스 버퍼를 전달해서 메쉬를 만들어내는 함수
	virtual bool CreateMesh(void* vtxData, int size, int count, D3D11_USAGE usage,
		D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	virtual bool Init();
	virtual void Render();

	// 인스턴싱을 이용한 렌더링
	virtual void RenderInstancing(int count);
	virtual void RenderInstancing(ID3D11Buffer* instancingBuffer, unsigned int instanceSize, int count);

protected:
	// 버퍼를 생성한다.
	// 데이터는 void* 로 받아서 어떠한 데이터도 받을 수 있게 한다.
	bool CreateBuffer(Buffer_Type type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer);

};

