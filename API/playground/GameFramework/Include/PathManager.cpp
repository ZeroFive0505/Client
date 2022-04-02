#include "PathManager.h"

CPathManager* CPathManager::m_Inst = nullptr;

CPathManager::CPathManager()
{
}

CPathManager::~CPathManager()
{
    // 소멸시에는 순회하면서 PathInfo를 제거한다.
    auto    iter = m_mapPath.begin();
    auto    iterEnd = m_mapPath.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

bool CPathManager::Init()
{
    // RootPath의 경로를 얻어온다.
    // RootPath는 Bin폴더 까지의 전체 경로를 말한다.
    TCHAR   Path[MAX_PATH] = {};

    // GetModuleFileName : 현재 실행파일이 있는 경로 + 실행파일이름.exe 로
    // 전체경로를 얻어오게 된다.
    // 여기에서 실행파일 이름까지를 빼서 Bin폴더 까지만 얻어와서
    // Root폴더로 사용한다.
    GetModuleFileName(0, Path, MAX_PATH);

    // 얻어온 문자열을 뒤에서부터 차례로 접근하여 \나 / 가 있는 문자를 찾는다.
    int Length = lstrlen(Path);

    // D:\Lecture\37th\API\GameFramework\GameFramework\Bin\GameFramework_Debug.exe
    for (int i = Length - 1; i > 0; --i)
    {
        if (Path[i] == '\\')
        {
            memset(&Path[i + 1], 0, sizeof(TCHAR) * (Length - i - 1));
            break;
        }
    }

    PathInfo* Info = new PathInfo;

    lstrcpy(Info->Path, Path);

#ifdef UNICODE
    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, Path, -1,
        Info->PathMultibyte, ConvertLength, 0, 0);
#else
    strcpy_s(Info->PathMultibyte, Path);
#endif // UNICODE

    m_mapPath.insert(std::make_pair(ROOT_PATH, Info));

    AddPath(TEXTURE_PATH, TEXT("Texture\\"));
    AddPath(SOUND_PATH, TEXT("Sound\\"));
    AddPath(FONT_PATH, TEXT("Font\\"));
    AddPath(MAP_PATH, TEXT("Map\\"));

    return true;
}

// 경로를 추가한다.
bool CPathManager::AddPath(const std::string& Name, const TCHAR* Path,
    const std::string& BaseName)
{
    // 해당 이름을 키로 가지는 경로가 없다면 반환.
    if (FindPath(Name))
        return false;

    // 먼저 베이스 경로를 찾아온다.
    const PathInfo* BaseInfo = FindPath(BaseName);

    if (!BaseInfo)
        return false;

    // 새로운 경로를 만든다.
    PathInfo* Info = new PathInfo;

    // 먼저 기본 경로를 복사하고
    lstrcpy(Info->Path, BaseInfo->Path);
    // 새로운 그 뒤에 덧붙인다.
    lstrcat(Info->Path, Path);

#ifdef UNICODE
    // 유니코드 문자열을 멀티바이트 문자열로 변환한다.
    int ConvertLength = WideCharToMultiByte(CP_ACP, 0, Info->Path, -1, nullptr, 0, 0, 0);

    WideCharToMultiByte(CP_ACP, 0, Info->Path, -1,
        Info->PathMultibyte, ConvertLength, 0, 0);
#else
    strcpy_s(Info->PathMultibyte, Info->Path);
#endif // UNICODE

    // 경로의 이름을 키로 새로운 경로 정보를 저장한다.
    m_mapPath.insert(std::make_pair(Name, Info));

    return true;
}

const PathInfo* CPathManager::FindPath(const std::string& Name)
{
    std::unordered_map<std::string, PathInfo*>::iterator iter = m_mapPath.find(Name);

    if (iter == m_mapPath.end())
        return nullptr;

    return iter->second;
}
