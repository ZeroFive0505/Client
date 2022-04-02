
#include "SceneResource.h"
#include "../Resource/ResourceManager.h"

CSceneResource::CSceneResource()
{
}

CSceneResource::~CSceneResource()
{

	{
		auto iter = m_mapAnimationSequence.begin();
		auto iterEnd = m_mapAnimationSequence.end();

		for (; iter != iterEnd;)
		{

			std::string Name = iter->first;

			iter = m_mapAnimationSequence.erase(iter);

			CResourceManager::GetInst()->ReleaseAnimationSeequence(Name);
		}
	}

	{
		auto	iter = m_mapTexture.begin();
		auto	iterEnd = m_mapTexture.end();

		// �� ���ҽ��� �ᱹ ���ҽ� �Ŵ����� �ִ� ���ҽ��� �����ϰ����� ���̴�. ���� ���ҽ� �Ŵ������� �����Ѵ�.
		for (; iter != iterEnd;)
		{

			std::string Name = iter->first;

			iter = m_mapTexture.erase(iter);

			CResourceManager::GetInst()->ReleaseTexture(Name);
		}
	}



	{
		auto iter = m_mapSound.begin();
		auto iterEnd = m_mapSound.end();

		for (; iter != iterEnd;)
		{
			std::string Name = iter->first;

			// earse�� �ϸ� SharedPtr�̹Ƿ� �ڵ����� ��ü�� ���ŵǸ� ī��Ʈ��
			// 1 �����Ѵ�. erase�� ���� ���� iterator�� ��ȯ�ϹǷ� ++�� ��������
			iter = m_mapSound.erase(iter);

			CResourceManager::GetInst()->ReleaseSound(Name);
		}
	}
}

// �ؽ��ĸ� �̸��� �����̸� �׸��� ��η� �ҷ��´�.
bool CSceneResource::LoadTexture(const std::string& Name,
	const TCHAR* FileName, const std::string& PathName)
{
	// ���� �̹� �����ϴ� �̸��̶�� ��ȯ.
	if (FindTexture(Name))
		return true;

	// �� ���ҽ��� ���ҽ��� �߰��Ҷ��� �Ŵ����� ���ؼ� �߰��Ѵ�.
	if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
		return false;

	// �ؽ��ĸ� ������ �ͼ�
	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	// ���� �ʿ� ���Ӱ� �߰��Ѵ�.
	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// �ؽ��ĸ� �̸��� �Բ� ���� ��η� �����´�.
bool CSceneResource::LoadTextureFullPath(const std::string& Name,
	const TCHAR* FullPath)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// �������� �� �ؽ��ĵ��� �����´�.
bool CSceneResource::LoadTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	if (FindTexture(Name))
		return true;

	if (!CResourceManager::GetInst()->LoadTexture(Name, vecFileName, PathName))
		return false;

	CTexture* Texture = CResourceManager::GetInst()->FindTexture(Name);

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// �ؽ����� Į��Ű�� �����Ѵ�. (�ش� ���� ��ŵ���)
void CSceneResource::SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, r, g, b, Index);
}

void CSceneResource::SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
	CResourceManager::GetInst()->SetTextureColorKey(Name, ColorKey, Index);
}

// �ؽ��ĸ� ã�ƿ´�.
CTexture* CSceneResource::FindTexture(const std::string& Name)
{
	auto	iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

// �ִϸ��̼� �������� �ؽ����� �̸��� ���޹޾Ƽ� �����.
bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
	// ���� �̹� �����ϴ� ��������� ��ȯ.
	if (FindAnimationSequence(SequenceName))
		return true;

	// ���ҽ��Ŵ����� ���ؼ� ���ο� �������� �����.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// �ش� �������� ���������
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// �ؽ��ʿ� ���
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// �ִϸ��̼� �������� �̸��� �ؽ����̸� �����̸� ��α��� �����ؼ� �ҷ��´�.
bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;

	// �ؽ��ĸ� �ҷ��´�.
	LoadTexture(TextureName, FileName, PathName);

	// ���ҽ��Ŵ����� ���� �������� �����.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// �ش� �������� ã�ƿ���
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// �ʿ� ��Ͻ�Ų��.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	if (FindAnimationSequence(SequenceName))
		return true;
		
	// �������� �и��� ��� �ؽ��ĵ��� �����´�.
	LoadTexture(TextureName, vecFileName, PathName);

	// �ִϸ��̼� �������� ����Ѵ�.
	if (!CResourceManager::GetInst()->CreateAnimationSequence(SequenceName, TextureName))
		return false;

	// �ִϸ��̼� �������� ��������
	CAnimationSequence* Sequence = CResourceManager::GetInst()->FindAnimationSequence(SequenceName);

	// �ʿ� �ִ´�.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// �������� �������� ������ġ�� ũ�⸦ ���ؼ� �ִ´�.
void CSceneResource::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

// �������� �������� ������ġ�� ũ�⸦ ���ؼ� �ִ´�.
void CSceneResource::AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

// ���� ��ϵ� �������� ã�´�.
CAnimationSequence* CSceneResource::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

// ��µ带 �ε��Ѵ�.
bool CSceneResource::LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileName, const std::string& PathName)
{
	// �̹� ������ ��� �ε��� �ʿ�� ����.
	if (FindSound(Name))
		return true;

	// ���ҽ��Ŵ����� ȣ���� �ε��Ѵ�.
	if (!CResourceManager::GetInst()->LoadSound(
		GroupName, Loop, Name, FileName, PathName
	))
		return false;

	CSound* Sound = CResourceManager::GetInst()->FindSound(Name);

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}

bool CSceneResource::SetVolume(int Volume)
{
	return CResourceManager::GetInst()->SetVolume(Volume);
}

bool CSceneResource::SetVolume(const std::string& GroupName, int Volume)
{
	return CResourceManager::GetInst()->SetVolume(GroupName, Volume);
}

bool CSceneResource::SoundPlay(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPlay(Name);
}

bool CSceneResource::SoundStop(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundStop(Name);
}

bool CSceneResource::SoundPause(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundPause(Name);
}

bool CSceneResource::SoundResume(const std::string& Name)
{
	return CResourceManager::GetInst()->SoundResume(Name);
}

CSound* CSceneResource::FindSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}
