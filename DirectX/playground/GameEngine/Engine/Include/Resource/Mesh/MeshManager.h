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
	class CMesh* FindMesh(const std::string& name);
	void ReleaseMesh(const std::string& name);
};

