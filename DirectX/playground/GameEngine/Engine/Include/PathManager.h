#pragma once

#include "GameInfo.h"

// ��θ� ������ ����ü
struct sPathInfo
{
	// �����ڵ� ���
	TCHAR path[MAX_PATH];
	// ��Ƽ����Ʈ ���
	char pathMultiByte[MAX_PATH];

	sPathInfo() :
		path{},
		pathMultiByte{}
	{

	}
};

/// <summary>
/// ��θ� �����ϴ� Ŭ����
/// </summary>
class CPathManager
{
private:
	CPathManager();
	~CPathManager();

private:
	std::unordered_map<std::string, sPathInfo*> m_mapPath;

public:
	bool Init();
	bool AddPath(const std::string& name, const TCHAR* path, const std::string& baseName = ROOT_PATH);
	const sPathInfo* FindPath(const std::string& name);

private:
	static CPathManager* m_Inst;

public:
	static CPathManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CPathManager;

		return m_Inst;
	}

	static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

