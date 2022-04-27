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

    // 현재 실행파일이 있는 경로를 받아온다.
    // 또는 lib파일이 있는 경로를 받아온다.
    GetModuleFileName(0, path, MAX_PATH);

    int len = lstrlen(path);

    // Bin까지의 경로를 얻어온다.
    for (int i = len - 1; i > 0; i--)
    {
        if (path[i] == '\\')
        {
            memset(&path[i + 1], 0, sizeof(TCHAR) * (len - i - 1));
            break;
        }
    }

    sPathInfo* info = new sPathInfo;

    lstrcpy(info->path, path);

#ifdef UNICODE
    int convertLength = WideCharToMultiByte(CP_ACP, 0, path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, path, -1, info->pathMultiByte, convertLength, 0, 0);
#else
    strcpy_s(info->pathMultiByte, path);
#endif // UNICODE

    m_mapPath.insert(std::make_pair(ROOT_PATH, info));

    AddPath(SHADER_PATH, TEXT("Shader\\"));
    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(FONT_PATH, TEXT("Font\\"));
    AddPath(ANIMATION_PATH, TEXT("Animation\\"));
    AddPath(SCENE_PATH, TEXT("Scene\\"));
    AddPath(SOUND_PATH, TEXT("Sound\\"));
    AddPath(MESH_PATH, TEXT("Mesh\\"));

    return true;
}

bool CPathManager::AddPath(const std::string& name, const TCHAR* path, const std::string& baseName)
{
    // 이미 경로가 존재
    if (FindPath(name))
        return false;

    const sPathInfo* baseInfo = FindPath(baseName);

    if (!baseInfo)
        return false;

    sPathInfo* info = new sPathInfo;

    lstrcpy(info->path, baseInfo->path);
    lstrcat(info->path, path);

#ifdef UNICODE
    int convertLength = WideCharToMultiByte(CP_ACP, 0, info->path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, info->path, -1, info->pathMultiByte, convertLength, 0, 0);
#else
    strcpy_s(info->pathMultiByte, path);
#endif // UNICODE


    m_mapPath.insert(std::make_pair(name, info));

    return true;
}

const sPathInfo* CPathManager::FindPath(const std::string& name)
{
    auto iter = m_mapPath.find(name);

    if (iter == m_mapPath.end())
        return nullptr;

    return iter->second;
}
