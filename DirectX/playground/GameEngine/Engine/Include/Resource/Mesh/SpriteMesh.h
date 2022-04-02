#pragma once

#include "Mesh.h"

/// <summary>
/// 2D 텍스쳐 출력 전용 스프라이트 메쉬
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

