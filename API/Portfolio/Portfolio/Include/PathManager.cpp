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

	// ���� ���������� �ִ� ��� + �������� �̸�.exe�� ��ü ��θ� ���´�.
	// ���⿡�� ���������� �̸������� ���� Bin���������� ���ͼ�
	// �� ��θ� Root������ �̿��Ѵ�.
	GetModuleFileName(0, path, MAX_PATH);

	int length = lstrlen(path);

	for (int i = length - 1; i > 0; i--)
	{
		// ù �������� ���´ٸ� ������� ���������� �̸�.
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


	// ��Ʈ ��η� �߰����ش�.
	m_mapPath.insert(std::make_pair(ROOT_PATH, info));

	// ��Ʈ ��� �ʱ�ȭ ���� �⺻ �ּ� ��ε� �߰��Ѵ�.
	AddPath(TEXTURE_PATH, TEXT("Texture\\"));
	AddPath(SOUND_PATH, TEXT("Sound\\"));
	AddPath(FONT_PATH, TEXT("Font\\"));
	AddPath(MAP_PATH, TEXT("Map\\"));

	return true;
}

bool CPathManager::AddPath(const std::string& name, const TCHAR* path, const std::string& baseName)
{
	// �̹� �����ϴ� ��ζ��
	if (FindPath(name))
		return false;

	const PathInfo* baseInfo = FindPath(baseName);

	// �Ǵ� �������� �ʴ� ��Ʈ �н����
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
