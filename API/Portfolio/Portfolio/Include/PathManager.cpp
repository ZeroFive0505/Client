#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
	auto iter = m_mapPath.begin();
	auto iterEnd = m_mapPath.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE(iter->second);
	}
}

bool CPathManager::Init()
{
	TCHAR path[MAX_PATH] = {};

	// 현재 실행파일이 있는 경로 + 실행파일 이름.exe로 전체 경로를 얻어온다.
	// 여기에서 실행파일의 이름까지를 빼서 Bin폴더까지만 얻어와서
	// 이 경로를 Root폴더로 이용한다.
	GetModuleFileName(0, path, MAX_PATH);

	int length = lstrlen(path);

	for (int i = length - 1; i > 0; i--)
	{
		// 첫 슬래쉬가 나온다면 여기까지 실행파일의 이름.
		if (path[i] == '\\')
		{
			memset(&path[i + 1], 0, sizeof(TCHAR) * (length - i - 1));
			break;
		}
	}

	PathInfo* info = new PathInfo;

	lstrcpy(info->unicodePath, path);

#ifdef UNICODE
	
	int convertLength = WideCharToMultiByte(CP_ACP, 0, path, -1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, path, -1, info->multiBytePath, convertLength, 0, 0);

#else
	strcpy_s(info->multiBytePath, path);

#endif // UNICODE


	// 루트 경로로 추가해준다.
	m_mapPath.insert(std::make_pair(ROOT_PATH, info));

	// 루트 경로 초기화 이후 기본 애셋 경로도 추가한다.
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));
	AddPath(MAP_PATH, TEXT("Map\\"));

	return true;
}

bool CPathManager::AddPath(const std::string& name, const TCHAR* path, const std::string& baseName)
{
	// 이미 존재하는 경로라면
	if (FindPath(name))
		return false;

	const PathInfo* baseInfo = FindPath(baseName);

	// 또는 존재하지 않는 루트 패스라면
	if (!baseInfo)
		return false;

	PathInfo* info = new PathInfo;

	lstrcpy(info->unicodePath, baseInfo->unicodePath);
	lstrcat(info->unicodePath, path);

#ifdef UNICODE
	
	int convertLength = WideCharToMultiByte(CP_ACP, 0, info->unicodePath, -1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, info->unicodePath, -1, info->multiBytePath, convertLength, 0, 0);
#else
	strcpy_s(info->multiBytePath, info->path);

#endif // UNICODE


	m_mapPath.insert(std::make_pair(name, info));

	return true;
}

const PathInfo* CPathManager::FindPath(const std::string& name)
{
	auto iter = m_mapPath.find(name);

	if (iter == m_mapPath.end())
		return nullptr;

	return iter->second;
}
