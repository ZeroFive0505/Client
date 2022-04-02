#pragma once

#include "GameInfo.h"

struct PathInfo
{
	TCHAR unicodePath[MAX_PATH];
	char multiBytePath[MAX_PATH];

	PathInfo() :
		unicodePath{},
		multiBytePath{}
	{

	}
};

class CPathManager
{
private:
	CPathManager();
	~CPathManager();

private:
	std::unordered_map<std::string, PathInfo*> m_mapPath;

public:
	bool Init();
	bool AddPath(const std::string& name, const TCHAR* path, const std::string& baseName = ROOT_PATH);
	const PathInfo* FindPath(const std::string& name);

private:
	static CPathManager* m_Inst;

public:
	inline static CPathManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CPathManager;

		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};

