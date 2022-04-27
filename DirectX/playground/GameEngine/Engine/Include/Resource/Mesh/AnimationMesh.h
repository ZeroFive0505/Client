#pragma once

#include "Mesh.h"

class CAnimationMesh :
	public CMesh
{
	friend class CMeshManager;

protected:
	CAnimationMesh();
	virtual ~CAnimationMesh();

private:
	CSharedPtr<class CSkeleton> m_Skeleton;

public:
	inline class CSkeleton* GetSkeleton() const
	{
		return m_Skeleton;
	}

public:
	virtual bool Init();
	void SetSkeleton(class CSkeleton* skeleton);
	void SetSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshFullPathMultibyte(const char* fullPath);

public:
	virtual bool ConvertFBX(class CFBXLoader* loader, const char* fullPath);
	virtual bool SaveMesh(FILE* pFile);
	virtual bool LoadMesh(FILE* pFile);
};

