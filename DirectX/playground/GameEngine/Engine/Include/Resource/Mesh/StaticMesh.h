#pragma once

#include "Mesh.h"

// 에디터 모드에서 사용될 정적 메쉬 클래스
class CStaticMesh :
	public CMesh
{
	friend class CMeshManager;

protected:
	CStaticMesh();
	virtual ~CStaticMesh();

public:
	virtual bool Init();
	virtual bool LoadMeshFullPathMultibyte(const char* fullPath);

public:
	virtual bool ConvertFBX(class CFBXLoader* loader, const char* fullPath);
	virtual bool SaveMesh(FILE* pFile);
	virtual bool LoadMesh(FILE* pFile);
};

