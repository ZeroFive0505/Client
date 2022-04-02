#pragma once

#include "GameInfo.h"

// 경로를 저장할 구조체
struct sPathInfo
{
	// 유니코드 경로
	TCHAR path[MAX_PATH];
	// 멀티바이트 경로
	char pathMultiByte[MAX_PATH];

	sPathInfo() :
		path{},
		pathMultiByte{}
	{

	}
};

/// <summary>
/// 경로를 관리하는 클래스
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

