#include "MapBase.h"
#include "../PathManager.h"

CMapBase::CMapBase() :
    m_Scene(nullptr),
    m_ZOrder(0),
    m_Enable(1),
    m_Active(1)
{
}

CMapBase::~CMapBase()
{
}

void CMapBase::Start()
{
}

bool CMapBase::Init()
{
    return false;
}

void CMapBase::Update(float deltaTime)
{
}

void CMapBase::PostUpdate(float detlaTime)
{
}

void CMapBase::Render(HDC hDC)
{
}

void CMapBase::SaveFile(const char* fileName, const std::string& pathName)
{
    char fullPath[MAX_PATH] = {};

    const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

    if (info)
        strcpy_s(fullPath, info->multiBytePath);
    strcat_s(fullPath, fileName);

    SaveFullPath(fullPath);
}

void CMapBase::SaveFullPath(const char* fullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, fullPath, "wb");

    if (!pFile)
        return;

    Save(pFile);

    fclose(pFile);
}

void CMapBase::Save(FILE* pFile)
{
    int length = m_Name.length();
    
    fwrite(&length, sizeof(int), 1, pFile);
    fwrite(m_Name.c_str(), sizeof(char), length, pFile);

    fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
    fwrite(&m_Size, sizeof(Vector2), 1, pFile);

    fwrite(&m_ZOrder, sizeof(int), 1, pFile);
    fwrite(&m_Enable, sizeof(int), 1, pFile);
    fwrite(&m_Active, sizeof(int), 1, pFile);
}

void CMapBase::LoadFile(const char* fileName, const std::string& pathName)
{
    char fullPath[MAX_PATH] = {};

    const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

    if (info)
        strcpy_s(fullPath, info->multiBytePath);

    strcat_s(fullPath, fileName);

    LoadFullPath(fullPath);
}

void CMapBase::LoadFullPath(const char* fullPath)
{
    FILE* pFile = nullptr;

    fopen_s(&pFile, fullPath, "rb");

    if (!pFile)
        return;

    Load(pFile);

    fclose(pFile);
}

void CMapBase::Load(FILE* pFile)
{
    int length = 0;

    fread(&length, sizeof(int), 1, pFile);

    char name[256] = {};

    fread(name, sizeof(char), length, pFile);

    m_Name = name;

    fread(&m_Pos, sizeof(Vector2), 1, pFile);
    fread(&m_Size, sizeof(Vector2), 1, pFile);

    fread(&m_ZOrder, sizeof(int), 1, pFile);
    fread(&m_Enable, sizeof(int), 1, pFile);
    fread(&m_Active, sizeof(int), 1, pFile);
}
