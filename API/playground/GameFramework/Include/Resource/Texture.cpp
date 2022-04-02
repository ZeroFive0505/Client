
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

// �ؽ��ĸ� �����̸��� ��θ� ���ؼ� �ҷ��´�.
bool CTexture::LoadTexture(const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	// ���� ��θ� ������ش�.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	TCHAR	FullPath[MAX_PATH] = {};

	if (Info)
		lstrcpy(FullPath, Info->Path);
	lstrcat(FullPath, FileName);

	return LoadTextureFullPath(Name, FullPath);
}

// �ؽ��ĸ� ���� ��θ� ���ؼ� �ҷ��´�.
bool CTexture::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	SetName(Name);

	// FileName �κи��� ���´�.
	TCHAR FileName[MAX_PATH] = {};

	char FullPathMultiByte[MAX_PATH] = {};

#ifdef UNICODE
	int ConvertLenght = WideCharToMultiByte(CP_ACP, 0, FullPath, -1, nullptr, 0, 0, 0);

	WideCharToMultiByte(CP_ACP, 0, FullPath, -1, FullPathMultiByte, ConvertLenght, 0, 0);

#else
	strcpy_s(FullPathMultiByte, FullPath);
#endif // UNICODE

	// Ȥ�ó� �𸣴� ���ϱ� ���ϰ� ��� �빮�ڷ� �ٲ۴�.
	_strupr_s(FullPathMultiByte);

	int PathLength = (int)strlen(FullPathMultiByte);

	// ���� ��� �̷��� ������ D:/Procjet/Bin/Texture/Player/Test.bmp
	for (int i = PathLength - 1; i >= 0; i--)
	{
		// ��� �� �ڿ������� ���ϸ鼭 �������ø� ã�´�.
		if (FullPathMultiByte[i] == '\\')
		{
			char FolderName[9] = {};
			// �ڿ������� ���ϱ⿡ �Ųٷ� �־��ش�.
			strcpy_s(FolderName, "ERUTXET\\");

			// ���� ��Ī�Ǵ����� ã�´�.
			// Texture �����ȿ� �ٷ� �ؽ��� ������ �������� ������
			// ���� �ȿ� ���� ������ ������� .../Texture������ �ɷ����� �� �ڷ� Player/Test.bmp�� �����´�.
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

	// �޸�DC�� �����.
	Info->hDC = CreateCompatibleDC(CGameManager::GetInst()->GetWindowDC());
	Info->hBmp = (HBITMAP)LoadImage(CGameManager::GetInst()->GetWindowInstance(),
		FullPath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// ������ ������ �޸� DC�� �ҷ��� ��Ʈ���� �������ش�.
	// SelectObject�� �Ʒ�ó�� HGDIOBJ�� �����ϸ� ���� ������ �ִ�
	// HGDIOBJ�� ��ȯ���ش�.
	// �׷��� �� DC�� �����ٶ����� �ݵ�� ���� HGDIOBJ�� ������ �صΰ�
	// ������� �Ѵ�.
	Info->hPrev = (HBITMAP)SelectObject(Info->hDC, Info->hBmp);

	// ��Ʈ�� ������ ���ͼ� �����صд�.
	GetObject(Info->hBmp, sizeof(BITMAP), &Info->BmpInfo);

	m_vecTextureInfo.push_back(Info);

	if (m_vecTextureInfo.size() == 1)
		m_TextureType = ETexture_Type::Atlas;

	else
		m_TextureType = ETexture_Type::Frame;

	return true;
}

// ���������� ������� �ؽ��ĸ� �ε��Ѵ�.
bool CTexture::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	// ���� ��θ� ������ش�.
	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	// �̹��� �� ��ŭ ��ȸ�ϸ鼭 �ݺ��Ѵ�.
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

// �ؽ�ó�� �׸���. Į��Ű�� �����Ǿ��� ���� ������ �� �Ǿ����� ��� �ٸ��� �׸���.
void CTexture::Render(HDC hDC, const Vector2& WindowPos, const Vector2& ImgPos, const Vector2& Size, int Index)
{
	if (m_vecTextureInfo[Index]->ColorKeyEnable)
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		// 9������ : ����� �̹��� �󿡼��� ����ũ��
		// 10������ : ����� �̹��� �󿡼��� ����ũ��
		// 11������ : ColorKey ����
		TransparentBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y,
			(int)Size.x, (int)Size.y, m_vecTextureInfo[Index]->ColorKey);
	}

	else
	{
		// 1������ : ����� DC�̴�.
		// 2������ : ����� ������ �󿡼��� x��ǥ
		// 3������ : ����� ������ �󿡼��� y��ǥ
		// 4������ : ����� ������ �󿡼��� xũ��
		// 5������ : ����� ������ �󿡼��� yũ��
		// 6������ : ����� �̹����� DC�̴�.
		// 7������ : ����� �̹��� �󿡼��� ���� ��ǥ x
		// 8������ : ����� �̹��� �󿡼��� ���� ��ǥ y
		BitBlt(hDC, (int)WindowPos.x, (int)WindowPos.y, (int)Size.x, (int)Size.y,
			m_vecTextureInfo[Index]->hDC, (int)ImgPos.x, (int)ImgPos.y, SRCCOPY);
	}
}

void CTexture::Save(FILE* pFile)
{
	// ���� �ؽ����� �̸��� �ҷ�����
	int Length = (int)m_Name.length();

	// �̸��� �����Ѵ�.
	fwrite(&Length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), Length, pFile);

	// �ؽ����� Ÿ���� �����Ѵ�.
	fwrite(&m_TextureType, sizeof(ETexture_Type), 1, pFile);

	// ���� �������� �� �ؽ����� ��� �װ͵��� ���δ� �����Ѵ�.
	int Count = (int)m_vecTextureInfo.size();
	fwrite(&Count, sizeof(int), 1, pFile);

	// ������ŭ ��ȸ�ϸ鼭 ������ �̸��� Į��Ű�� ������ �����Ѵ�.
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
