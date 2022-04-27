#pragma once

#include "../../GameInfo.h"

/// <summary>
/// 리소스 매니저를 통해서 메쉬 매니저에 접근해서 메쉬를 관리한다.
/// </summary>
class CMeshManager
{
	friend class CResourceManager;

private:
	CMeshManager();
	~CMeshManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CMesh>> m_mapMesh;

public:
	bool Init();
	bool LoadMesh(Mesh_Type type, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = MESH_PATH, class CScene* scene = nullptr);
	bool LoadMeshFullPath(Mesh_Type type, const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadMeshMultibyte(Mesh_Type type, const std::string& name, const char* fileName,
		const std::string& pathName = MESH_PATH, class CScene* scene = nullptr);
	bool LoadMeshFullPathMultibyte(Mesh_Type type, const std::string& name, const char* fullPath, class CScene* scene = nullptr);
	bool SetMeshSkeleton(const std::string& name, class CSkeleton* skeleton);
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);
};

