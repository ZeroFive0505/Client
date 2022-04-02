#include "AnimationManager.h"
#include "../Shader/Animation2DConstantBuffer.h"

CAnimationManager::CAnimationManager() :
	m_Animation2DCBuffer(nullptr)
{
}

CAnimationManager::~CAnimationManager()
{
	SAFE_DELETE(m_Animation2DCBuffer);
}

bool CAnimationManager::Init()
{
	m_Animation2DCBuffer = new CAnimation2DConstantBuffer;

	m_Animation2DCBuffer->Init();

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
		return true;

	sequence = new CAnimationSequence2D;

	sequence->SetName(name);

	if (!sequence->Init(textureName, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
		return true;

	sequence = new CAnimationSequence2D;

	sequence->SetName(name);

	if (!sequence->Init(texture))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2D(const std::string& name, 
	const std::string& textureName, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
		return true;

	sequence = new CAnimationSequence2D;

	sequence->SetName(name);

	if (!sequence->Init(textureName, vecFileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName,
	const std::vector<TCHAR*>& vecFullPath)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (sequence)
		return true;

	sequence = new CAnimationSequence2D;

	sequence->SetName(name);

	if (!sequence->InitFullPath(textureName, vecFullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	m_mapSequence2D.insert(std::make_pair(name, sequence));

	return true;
}

void CAnimationManager::AddFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
		return;

	sequence->AddFrame(start, size);
}

void CAnimationManager::AddFrame(const std::string& name, float startX, float startY, float width, float height)
{
	CAnimationSequence2D* sequnce = FindSequence(name);

	if (!sequnce)
		return;

	sequnce->AddFrame(startX, startY, width, height);
}

bool CAnimationManager::SaveSequenceFullPath(const std::string& name, const char* fullPath)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	// 저장시에는 시퀀스가 존재하지않으면 안된다.
	if (!sequence)
		return false;

	sequence->SaveFullPath(fullPath);

	return true;
}

bool CAnimationManager::LoadSequenceFullPath(std::string& resultName, const char* fullPath, CScene* scene)
{
	CAnimationSequence2D* sequence = new CAnimationSequence2D;

	sequence->SetScene(scene);

	if (!sequence->LoadFullPath(fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	resultName = sequence->GetName();

	if (FindSequence(resultName))
	{
		SAFE_DELETE(sequence);
		return true;
	}
	
	m_mapSequence2D.insert(std::make_pair(resultName, sequence));


	return true;
}

bool CAnimationManager::SaveSequence(const std::string& name, const char* fileName, const std::string& pathName)
{
	CAnimationSequence2D* sequence = FindSequence(name);

	if (!sequence)
		return false;

	sequence->Save(fileName, pathName);

	return true;
}

bool CAnimationManager::LoadSequence(std::string& resultName, const char* fileName, const std::string& pathName, CScene* scene)
{
	CAnimationSequence2D* sequence = new CAnimationSequence2D;

	sequence->SetScene(scene);

	if (!sequence->Load(fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	resultName = sequence->GetName();

	if (FindSequence(resultName))
	{
		SAFE_DELETE(sequence);
		return true;
	}

	m_mapSequence2D.insert(std::make_pair(resultName, sequence));

	return true;
}

CAnimationSequence2D* CAnimationManager::FindSequence(const std::string& name)
{
	auto iter = m_mapSequence2D.find(name);

	if (iter == m_mapSequence2D.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager::ReleaseSequence(const std::string& name)
{
	auto iter = m_mapSequence2D.find(name);

	if (iter != m_mapSequence2D.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence2D.erase(iter);
	}
}
