#pragma once

#include "../../GameInfo.h"

/// <summary>
/// ���ҽ� �Ŵ����� ���ؼ� �޽� �Ŵ����� �����ؼ� �޽��� �����Ѵ�.
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

