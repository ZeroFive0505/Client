#pragma once

#include "Mesh.h"

/// <summary>
/// 2D �ؽ��� ��� ���� ��������Ʈ �޽�
/// </summary>
class CSpriteMesh :
	public CMesh
{
	friend class CMeshManager;

protected:
	CSpriteMesh();
	virtual ~CSpriteMesh();

public:
	virtual bool Init();
};

