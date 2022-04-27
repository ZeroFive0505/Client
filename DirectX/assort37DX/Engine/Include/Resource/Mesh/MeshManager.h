#pragma once

#include "../../GameInfo.h"

class CMeshManager
{
	friend class CResourceManager;

private:
	CMeshManager();
	~CMeshManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>>	m_mapMesh;

public:
	bool Init();
	bool LoadMesh(Mesh_Type Type, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = MESH_PATH, class CScene* Scene = nullptr);
	bool LoadMeshFullPath(Mesh_Type Type, const std::string& Name,
		const TCHAR* FullPath, class CScene* Scene = nullptr);
	bool LoadMeshMultibyte(Mesh_Type Type, const std::string& Name, const char* FileName,
		const std::string& PathName = MESH_PATH, class CScene* Scene = nullptr);
	bool LoadMeshFullPathMultibyte(Mesh_Type Type, const std::string& Name,
		const char* FullPath, class CScene* Scene = nullptr);
	bool SetMeshSkeleton(const std::string& Name, class CSkeleton* Skeleton);
	class CMesh* FindMesh(const std::string& Name);
	void ReleaseMesh(const std::string& Name);
};

