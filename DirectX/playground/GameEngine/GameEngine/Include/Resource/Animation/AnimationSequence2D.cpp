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
	// ���� �ҼӾ��� �������
	if (m_Scene)
	{
		// ���� ���� ���ؼ� �ε��Ѵ�.
		if (!m_Scene->GetSceneResource()->LoadTexture(name, fileName, pathName))
			return false;

		m_Texture = m_Scene->GetSceneResource()->FindTexture(name);
	}
	else
	{
		// ���� ���� ��� ���� ���ҽ� �Ŵ����� �����ؼ� �����´�.
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

	// ���� �ؽ�ó�� �̸��� ���̿� �̸��� ����
	int length = (int)m_Name.length();
	fwrite(&length, sizeof(int), 1, pFile);
	fwrite(m_Name.c_str(), sizeof(char), length, pFile);

	bool hasTexture = false;

	if (m_Texture)
		hasTexture = true;

	// �ؽ�ó �������� ����
	fwrite(&hasTexture, sizeof(bool), 1, pFile);

	// �ؽ�ó�� �������ִٸ� �ؽ�ó���� �����ؾ���
	if (m_Texture)
		m_Texture->Save(pFile);

	// ���� �������� �������ִ� ��� �����ӵ鵵 �����Ѵ�.
	int frameCount = (int)m_vecFrameData.size();

	// ��� ������ ���� ����
	fwrite(&frameCount, sizeof(int), 1, pFile);

	// ������ �����ּҺ��� �����Ӽ���ŭ �����Ѵ�.
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

	// ���� �ؽ�ó�� �̸��� ���̿� �̸��� �޾ƿ´�.
	int length = 0;
	fread(&length, sizeof(int), 1, pFile);

	char name[256] = {};
	fread(name, sizeof(char), length, pFile);
	m_Name = name;

	bool hasTexture = false;

	// �ؽ�ó ���� ���θ� �����´�.
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

		// �ϳ��� ���������� ���� �ؽ�ó�� �̿�� �� �ִ� ���� ���ͷ� ���� �� �����Ѵ�.
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
				// ���� �Ҽӵ� ���� �ִٸ�
				if (m_Scene)
				{
					// ������ �ؽ��ĸ� �ε��Ѵ�.
					m_Scene->GetSceneResource()->LoadTexture(textureName, vecFileName[0].c_str(), pathName);
				}
				else
				{
					// �ƴҰ�� ���ҽ��Ŵ������� �ε�
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
