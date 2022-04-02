#include "AnimationSequence2D.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../ResourceManager.h"
#include "../../PathManager.h"

CAnimationSequence2D::CAnimationSequence2D() :
	m_Scene(nullptr)
{
}

CAnimationSequence2D::~CAnimationSequence2D()
{
}

void CAnimationSequence2D::SetTexture(CTexture* texture)
{
	m_Texture = texture;
}

bool CAnimationSequence2D::Init(CTexture* texture)
{
	m_Texture = texture;

	return true;
}

bool CAnimationSequence2D::Init(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	// 만약 소속씬이 있을경우
	if (m_Scene)
	{
		// 먼저 씬을 통해서 로드한다.
		if (!m_Scene->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return false;

		m_Texture = m_Scene->GetSceneResource()->FindTexture(name);
	}
	else
	{
		// 씬이 없을 경우 직접 리소스 매니저에 접근해서 가져온다.
		if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
			return false;

		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}

	return true;
}

bool CAnimationSequence2D::Init(const std::string& name, 
	const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	if (m_Scene)
	{
		if (!m_Scene->GetSceneResource()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_Texture = m_Scene->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
			return false;

		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}

	return true;
}

bool CAnimationSequence2D::InitFullPath(const std::string& name,
	const std::vector<TCHAR*>& vecFullPath)
{
	if (m_Scene)
	{
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_Texture = m_Scene->GetSceneResource()->FindTexture(name);
	}
	else
	{
		if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
			return false;

		m_Texture = CResourceManager::GetInst()->FindTexture(name);
	}

	return true;
}

void CAnimationSequence2D::AddFrame(const Vector2& startPos, const Vector2& size)
{
	sAnimationFrameData data = {};

	data.start = startPos;
	data.size = size;

	m_vecFrameData.push_back(data);
}

void CAnimationSequence2D::AddFrame(float startX, float startY, float width, float height)
{
	sAnimationFrameData data = {};

	data.start = Vector2(startX, startY);
	data.size = Vector2(width, height);

	m_vecFrameData.push_back(data);
}

void CAnimationSequence2D::SetFrameData(int index, const sAnimationFrameData& data)
{
	if (index >= m_vecFrameData.size())
		return;

	m_vecFrameData[index] = data;
}

void CAnimationSequence2D::DeleteFrame(int index)
{
	if (index >= m_vecFrameData.size() || m_vecFrameData.empty())
		return;

	auto iter = m_vecFrameData.begin() + index;

	m_vecFrameData.erase(iter);
}

void CAnimationSequence2D::ClearFrame()
{
	m_vecFrameData.clear();
}

bool CAnimationSequence2D::SaveFullPath(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "wb");

	if (!pFile)
		return false;

	// 현재 텍스처의 이름의 길이와 이름을 저장
	int length = (int)m_Name.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), length, pFile);

	bool hasTexture = false;

	if (m_Texture)
		hasTexture = true;

	// 텍스처 소유여부 저장
	fwrite(&hasTexture, sizeof(bool), 1, pFile);

	// 텍스처를 가지고있다면 텍스처또한 저장해야함
	if (m_Texture)
		m_Texture->Save(pFile);

	// 현재 시퀀스가 가지고있는 모든 프레임들도 저장한다.
	int frameCount = (int)m_vecFrameData.size();

	// 모든 프레임 수도 저장
	fwrite(&frameCount, sizeof(int), 1, pFile);

	// 벡터의 시작주소부터 프레임수만큼 저장한다.
	fwrite(&m_vecFrameData[0], sizeof(sAnimationFrameData), frameCount, pFile);

	fclose(pFile);

	return true;
}

bool CAnimationSequence2D::LoadFullPath(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "rb");

	if (!pFile)
		return false;

	// 현재 텍스처의 이름의 길이와 이름을 받아온다.
	int length = 0;
	fread(&length, sizeof(int), 1, pFile);

	char name[256] = {};
	fread(name, sizeof(char), length, pFile);
	m_Name = name;

	bool hasTexture = false;

	// 텍스처 소유 여부를 가져온다.
	fread(&hasTexture, sizeof(bool), 1, pFile);

	if (hasTexture)
	{
		int textureNameLength = 0;
		fread(&textureNameLength, sizeof(int), 1, pFile);
		char textureName[256] = {};
		fread(textureName, sizeof(char), textureNameLength, pFile);

		Image_Type imageType;
		fread(&imageType, sizeof(Image_Type), 1, pFile);

		int infoCount = 0;

		fread(&infoCount, sizeof(int), 1, pFile);

		// 하나의 시퀀스에는 여러 텍스처가 이용될 수 있다 따라서 벡터로 전부 다 저장한다.
		std::vector<std::wstring> vecFullPath;
		std::vector<std::wstring> vecFileName;
		std::string pathName;

		for (int i = 0; i < infoCount; i++)
		{
			int pathSize = 0;

			fread(&pathSize, sizeof(int), 1, pFile);

			TCHAR fullPath[MAX_PATH] = {};
			fread(fullPath, sizeof(TCHAR), pathSize, pFile);
			vecFullPath.push_back(fullPath);

			fread(&pathSize, sizeof(int), 1, pFile);

			TCHAR textureFileName[MAX_PATH] = {};
			fread(textureFileName, sizeof(TCHAR), pathSize, pFile);
			vecFileName.push_back(textureFileName);

			fread(&pathSize, sizeof(int), 1, pFile);

			char texturePathName[MAX_PATH] = {};
			fread(texturePathName, sizeof(char), pathSize, pFile);
			
			pathName = texturePathName;
		}

		switch (imageType)
		{
		case Image_Type::Atlas:
			if (vecFileName.size() == 1)
			{
				// 현재 소속된 씬이 있다면
				if (m_Scene)
				{
					// 씬에서 텍스쳐를 로드한다.
					m_Scene->GetSceneResource()->LoadTexture(textureName, vecFileName[0].c_str(), pathName);
				}
				else
				{
					// 아닐경우 리소스매니저에서 로드
					CResourceManager::GetInst()->LoadTexture(textureName, vecFileName[0].c_str(), pathName);
				}
			}
			break;
		case Image_Type::Frame:
			if (vecFileName.size() == 1)
			{

			}
			else
			{

			}
			break;
		case Image_Type::Array:
			if (vecFileName.size() == 1)
			{

			}
			else
			{

			}
			break;
		}

		if (m_Scene)
			m_Texture = m_Scene->GetSceneResource()->FindTexture(textureName);
		else
			m_Texture = CResourceManager::GetInst()->FindTexture(textureName);
	}

	int frameCount = 0;

	fread(&frameCount, sizeof(int), 1, pFile);

	m_vecFrameData.resize((const size_t)frameCount);

	fread(&m_vecFrameData[0], sizeof(sAnimationFrameData), frameCount, pFile);

	fclose(pFile);

	return true;
}

bool CAnimationSequence2D::Save(const char* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
		strcpy_s(fullPath, path->pathMultiByte);

	strcat_s(fullPath, fileName);

	return SaveFullPath(fullPath);
}

bool CAnimationSequence2D::Load(const char* fileName, const std::string& pathName)
{
	const sPathInfo* path = CPathManager::GetInst()->FindPath(pathName);

	char fullPath[MAX_PATH] = {};

	if (path)
		strcpy_s(fullPath, path->pathMultiByte);

	strcat_s(fullPath, fileName);

	return LoadFullPath(fullPath);
}
