#pragma once

#include "Mesh.h"

// ������ ��忡�� ���� ���� �޽� Ŭ����
class CStaticMesh :
	public CMesh
{
	friend class CMeshManager;

protected:
	CStaticMesh();
	virtual ~CStaticMesh();

public:
	virtual bool Init();
};

