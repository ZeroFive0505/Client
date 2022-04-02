#include "MapBase.h"
#include "../PathManager.h"

CMapBase::CMapBase() :
	m_Scene(nullptr),
	m_ZOrder(0),
	m_Enable(true),
	m_Active(true)
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

void CMapBase::Update(float DeltaTime)
{
}

void CMapBase::PostUpdate(float DeltaTime)
{
}

void CMapBase::PrevRender()
{
}

void CMapBase::Render(HDC hDC)
{
}

void CMapBase::SaveFile(const char* FileName, const std::string& PathName)
{
	char FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if(Info)
		strcpy_s(FullPath, Info->PathMultibyte);
	strcat_s(FullPath, FileName);

	SaveFullPath(FullPath);
}

void CMapBase::SaveFullPath(const char* FullPath)
{
	// 파일포인터를 만든다.
	FILE* pFile = nullptr;

	// 경로에 파일 바이너리 형태로 생성한다.
	fopen_s(&pFile, FullPath, "wb");

	if (!pFile)
		return;

	// 저장함수를 호출한다.
	// 이때 SaveFullPath, LoadFullPath 함수의 경우 오버라이딩이 되어있지 않고
	// Save, Load 함수는 오버라이딩이 되어있다.
	// 따라서 아래의 함수는 자식의 Save 함수가 호출된다.
	Save(pFile);

	fclose(pFile);
}

void CMapBase::Save(FILE* pFile)
{
	int Length = (int)m_Name.length();

	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_Size, sizeof(Vector2), 1, pFile);

	fwrite(&m_ZOrder, sizeof(int), 1, pFile);
	fwrite(&m_Enable, sizeof(bool), 1, pFile);
	fwrite(&m_Active, sizeof(bool), 1, pFile);
}

void CMapBase::LoadFile(const char* FileName, const std::string& PathName)
{
	char FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	LoadFullPath(FullPath);
}

void CMapBase::LoadFullPath(const char* FullPath)
{
	FILE* pFile = nullptr;

	// 파일을 연다.
	fopen_s(&pFile, FullPath, "rb");

	if (!pFile)
		return;
	
	// 재정의된 로드 함수를 호출한다.
	Load(pFile);

	fclose(pFile);
}

void CMapBase::Load(FILE* pFile)
{
	// 맵의 이름을 불러온다.
	int Length = 0;

	fread(&Length, sizeof(int), 1, pFile);

	char Name[256] = {};

	fread(Name, sizeof(char), Length, pFile);

	m_Name = Name;

	// 베이스 맵의 기본 설정들을 불러온다.
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_Size, sizeof(Vector2), 1, pFile);

	fread(&m_ZOrder, sizeof(int), 1, pFile);
	fread(&m_Enable, sizeof(bool), 1, pFile);
	fread(&m_Active, sizeof(bool), 1, pFile);
}
