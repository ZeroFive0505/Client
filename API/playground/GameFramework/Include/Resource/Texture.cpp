
#include "Texture.h"
#include "../PathManager.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
	size_t	Size = m_vecTextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecTextureInfo[i]);
	}
}

// 텍스쳐를 파일이름과 경로를 통해서 불러온다.
bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	// 최종 경로를 만들어준다.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(Name, FullPath);
}

// 텍스쳐를 절대 경로를 통해서 불러온다.
bool CTexture::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	SetName(Name);

	// FileName 부분만을 얻어온다.
	TCHAR FileName[MAX_PATH] = {};

	char FullPathMultiByte[MAX_PATH] = {};

#ifdef UNICODE
	int ConvertLenght = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultiByte, ConvertLenght, 0, 0);

#else
	strcpy_s(FullPathMultiByte, FullPath);
#endif // UNICODE

	// 혹시나 모르니 비교하기 편하게 모두 대문자로 바꾼다.
	_strupr_s(FullPathMultiByte);

	int PathLength = (int)strlen(FullPathMultiByte);

	// 예를 들어 이렇게 들어오면 D:/Procjet/Bin/Texture/Player/Test.bmp
	for (int i = PathLength - 1; i >= 0; i--)
	{
		// 경로 맨 뒤에서부터 비교하면서 역슬래시를 찾는다.
		if (FullPathMultiByte[i] == '\\')
		{
			char FolderName[9] = {};
			// 뒤에서부터 비교하기에 거꾸로 넣어준다.
			strcpy_s(FolderName, "ERUTXET\\");

			// 이제 매칭되는지를 찾는다.
			// Texture 폴더안에 바로 텍스쳐 파일이 있을수도 있지만
			// 따로 안에 서브 폴더가 있을경우 .../Texture까지를 걸러내고 그 뒤로 Player/Test.bmp를 가져온다.
			bool Find = true;

			for (int j = 1; j <= 8; j++)
			{
				if (FullPathMultiByte[i - j] != FolderName[j - 1])
				{
					Find = false;
					break;
				}
			}

			if (Find)
				lstrcpy(FileName, &FullPath[i + 1]);
		}
	}


	TextureInfo* Info = new TextureInfo;

	lstrcpy(Info->FileName, FileName);

	// 메모리DC를 만든다.
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 위에서 생성한 메모리 DC에 불러온 비트맵을 지정해준다.
	// SelectObject는 아래처럼 HGDIOBJ을 지정하면 원래 가지고 있던
	// HGDIOBJ를 반환해준다.
	// 그래서 이 DC를 지워줄때에는 반드시 원본 HGDIOBJ로 지정을 해두고
	// 지워줘야 한다.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// 비트맵 정보를 얻어와서 저장해둔다.
	GetObject(Info->hBmp, sizeof(BITMAP), &Info->BmpInfo);

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

// 여러장으로 만들어진 텍스쳐를 로딩한다.
bool CTexture::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	// 최종 경로를 만들어준다.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	// 이미지 수 만큼 순회하면서 반복한다.
	size_t	Size = vecFileName.size();

	for (size_t i = 0; i < Size; ++i)
	{
		TCHAR	FullPath[MAX_PATH] = {};

		if (Info)
			lstrcpy(FullPath, Info->Path);
		lstrcat(FullPath, vecFileName[i].c_str());

		if (!LoadTextureFullPath(Name, FullPath))
			return false;
	}

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;
	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

// 텍스처를 그린다. 칼라키가 설정되었을 경우와 설정이 안 되어있을 경우 다르게 그린다.
void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos, const Vector2& Size, int Index)
{
	if (m_vecTextureInfo[Index]->ColorKeyEnable)
	{
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		// 9번인자 : 출력할 이미지 상에서의 가로크기
		// 10번인자 : 출력할 이미지 상에서의 세로크기
		// 11번인자 : ColorKey 지정
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1번인자 : 출력할 DC이다.
		// 2번인자 : 출력할 윈도우 상에서의 x좌표
		// 3번인자 : 출력할 윈도우 상에서의 y좌표
		// 4번인자 : 출력할 윈도우 상에서의 x크기
		// 5번인자 : 출력할 윈도우 상에서의 y크기
		// 6번인자 : 출력할 이미지의 DC이다.
		// 7번인자 : 출력할 이미지 상에서의 시작 좌표 x
		// 8번인자 : 출력할 이미지 상에서의 시작 좌표 y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y, SRCCOPY);
	}
}

void CTexture::Save(FILE* pFile)
{
	// 현재 텍스쳐의 이름을 불러오고
	int Length = (int)m_Name.length();

	// 이름을 저장한다.
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	// 텍스쳐의 타입을 저장한다.
	fwrite(&m_TextureType, sizeof(ETexture_Type), 1, pFile);

	// 만약 낱장으로 된 텍스쳐일 경우 그것들을 전부다 저장한다.
	int Count = (int)m_vecTextureInfo.size();
	fwrite(&Count, sizeof(int), 1, pFile);

	// 개수만큼 순회하면서 파일의 이름과 칼라키의 정보를 저장한다.
	for (int i = 0; i < Count; i++)
	{
		fwrite(&m_vecTextureInfo[i]->ColorKeyEnable, sizeof(bool), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = lstrlen(m_vecTextureInfo[i]->FileName);
		fwrite(&NameCount, sizeof(int), 1, pFile);
		fwrite(&m_vecTextureInfo[i]->FileName, sizeof(TCHAR), NameCount, pFile);
	}
}

void CTexture::Load(FILE* pFile)
{
	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	m_Name = Name;

	fread(&m_TextureType, sizeof(ETexture_Type), 1, pFile);

	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);


	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};

		fread(FileName, sizeof(TCHAR), NameCount, pFile);


		LoadTexture(m_Name, FileName);

		if (ColorKeyEnable)
			SetColorKey(ColorKey);
	}
	else
	{
		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorKeyEnable;
		std::vector<unsigned int> vecColorKey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorKeyEnable.push_back(ColorKeyEnable);
			vecColorKey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};

			fread(FileName, sizeof(TCHAR), NameCount, pFile);

			vecFileName.push_back(FileName);
		}

		LoadTexture(m_Name, vecFileName);

		for (int i = 0; i < Count; i++)
		{
			if (!vecColorKeyEnable[i])
				continue;

			SetColorKey(vecColorKey[i], i);
		}
	}
}

CTexture* CTexture::LoadStatic(FILE* pFile, CScene* Scene)
{
	CTexture* Result = nullptr;

	int Length = 0;
	char Name[256] = {};

	fread(&Length, sizeof(int), 1, pFile);
	fread(Name, sizeof(char), Length, pFile);

	ETexture_Type TextureType;

	fread(&TextureType, sizeof(ETexture_Type), 1, pFile);

	int Count = 0;
	fread(&Count, sizeof(int), 1, pFile);

	if (Count == 1)
	{
		bool ColorKeyEnable = false;
		unsigned int ColorKey = 0;

		fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
		fread(&ColorKey, sizeof(unsigned int), 1, pFile);

		int NameCount = 0;
		fread(&NameCount, sizeof(int), 1, pFile);

		TCHAR FileName[MAX_PATH] = {};

		fread(FileName, sizeof(TCHAR), NameCount, pFile);

		Scene->GetSceneResource()->LoadTexture(Name, FileName);

		if (ColorKeyEnable)
			Scene->GetSceneResource()->SetTextureColorKey(Name, ColorKey);

		Result = Scene->GetSceneResource()->FindTexture(Name);
	}

	else
	{
		std::vector<std::wstring> vecFileName;
		std::vector<bool> vecColorKeyEnable;
		std::vector<unsigned int> vecColorKey;

		for (int i = 0; i < Count; i++)
		{
			bool ColorKeyEnable = false;
			unsigned int ColorKey = 0;

			fread(&ColorKeyEnable, sizeof(bool), 1, pFile);
			fread(&ColorKey, sizeof(unsigned int), 1, pFile);

			vecColorKeyEnable.push_back(ColorKeyEnable);
			vecColorKey.push_back(ColorKey);

			int NameCount = 0;
			fread(&NameCount, sizeof(int), 1, pFile);

			TCHAR FileName[MAX_PATH] = {};

			fread(FileName, sizeof(TCHAR), NameCount, pFile);

			vecFileName.push_back(FileName);
		}

		Scene->GetSceneResource()->LoadTexture(Name, vecFileName);

		for (int i = 0; i < Count; i++)
		{
			if (!vecColorKeyEnable[i])
				continue;

			Scene->GetSceneResource()->SetTextureColorKey(Name, vecColorKey[i]);
		}

		Result = Scene->GetSceneResource()->FindTexture(Name);
	}

	return Result;
}
