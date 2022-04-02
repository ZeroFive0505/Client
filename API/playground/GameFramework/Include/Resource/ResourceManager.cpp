#include "ResourceManager.h"
#include "../PathManager.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

CResourceManager::CResourceManager() :
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	// ���� �ִϸ��̼��� ���� �����Ѵ�. �ִϸ��̼� ���� �ؽ��Ŀ��� ��������Ʈ ��Ʈ���·�
	// ��������� ���̱⿡ �ؽ��� ���� ���� �����Ѵ�.
	m_mapAnimationSequence.clear();

	// �� ���� �ؽ��ĸ� �����Ѵ�.
	m_mapTexture.clear();

	{
		auto iter = m_mapChannelGroup.begin();
		auto iterEnd = m_mapChannelGroup.end();

		for (; iter != iterEnd; iter++)
		{
			iter->second->release();
		}

		m_mapChannelGroup.clear();
	}


	if (m_System)
	{
		m_System->close();
		m_System->release();
	}

	{
		auto iter = m_FontLoadList.begin();
		auto iterEnd = m_FontLoadList.end();

		for (; iter != iterEnd; iter++)
		{
			RemoveFontResource((*iter).c_str());
		}
	}
}

bool CResourceManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&m_System);

	if (result != FMOD_OK)
		return false;

	// System �ʱ�ȭ
	result = m_System->init(128, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	// Master channel Group�� ���´�.
	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));


	// ���� ä�κ��� �׷��� ����� ���´�.
	// BGM ä�α׷쿡 ���� ä�ε��� BGMä�� �׷��� ������ �پ��� ��� �ٰ��� �پ�� ���̴�.
	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("Effect");
	CreateSoundChannelGroup("UI");

	LoadOtherFont(TEXT("NotoSansKR-Black.otf"));
	LoadOtherFont(TEXT("NotoSansKR-Bold.otf"));
	LoadOtherFont(TEXT("NotoSansKR-Regular.otf"));

	LoadFont("DefaultFont", TEXT("NotoSansKR-Regular"));

	return true;
}

void CResourceManager::Update()
{
	m_System->update();
}

// �ؽ��ĸ� �����̸��� ��θ� ���ؼ� �ҷ��´�.
bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	// �̹� �ش� Ű�� �ؽ��İ� �����Ѵٸ� ��ȯ�Ѵ�.
	if (Texture)
		return true;

	// ���Ӱ� �ؽ��ĸ� �����.
	Texture = new CTexture;

	// ���� �ؽ�ó �ε尡 ���������ÿ��� ������
	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	// ���ҽ��Ŵ����� �����ϴ� ���ο� �ؽ��ķ� �߰��Ѵ�.
	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// ��ü ��θ� ���ؼ� �ؽ��ĸ� �ε��Ѵ�.
bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTextureFullPath(Name, FullPath))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// ���������� �Ǿ��ִ� �ؽ��ĸ� �ε��Ѵ�.
bool CResourceManager::LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	if (Texture)
		return true;

	Texture = new CTexture;

	if (!Texture->LoadTexture(Name, vecFileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// �ؽ�ó�� Į�� Ű�� �����Ѵ�.
void CResourceManager::SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index)
{
	CTexture* Texture = FindTexture(Name);

	if (!Texture)
		return;

	Texture->SetColorKey(r, g, b, Index);
}

void CResourceManager::SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index)
{
	CTexture* Texture = FindTexture(Name);

	if(!Texture)
		return;

	Texture->SetColorKey(ColorKey, Index);
}

// �ؽ��ĸ� �������Ѵ�.
void CResourceManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	// ���� �������Ϸ��� �ؽ����� ���۷��� ī��Ʈ�� 1�̶�� ����
	// ���ҽ��Ŵ��� �̿ܿ��� �ƹ��� �����ڰ� ���ٴ� ���̴�.
	// ���� �����Ѵ�.
	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);
}

// �̸��� Ű�� �ؽ��ĸ� ã�ƿ´�.
CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;
	
	return iter->second;
}

// �ִϸ��̼� �������� �����.
bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	// ���� �̹� �ش� Ű�� �ִϸ��̼��� ���� ���� �ٷ� ��ȯ�Ѵ�.
	if (Sequence)
		return true;

	// ���ο� �ִϸ��̼� �������� �����.
	Sequence = new CAnimationSequence;

	// �ִϸ��̼� �������� ��������ؼ� �ؽ��ĸ� �����´�.
	CTexture* Texture = FindTexture(TextureName);

	// �������� �ʱ�ȭ�ϰ�
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	// ���Ӱ� ����Ѵ�.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// �ִϸ��̼� �������� �ؽ��� �̸��� ���� �̸�, ��η� �����Ѵ�.
bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (Sequence)
		return true;

	// ���� �ؽ��ĸ� �ҷ�����
	if (!LoadTexture(TextureName, FileName, PathName))
		return false;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	// �������� �����Ѵ�.
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// ���������� �Ǿ��ִ� �̹����� �������� �����Ѵ�.
bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const std::vector<std::wstring>& vecFileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);
	
	if (Sequence)
		return true;

	if (!LoadTexture(TextureName, vecFileName, PathName))
		return false;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// �ش� �������� ������ġ�� ����� �����Ѵ�.
void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2 Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

// �ش� �������� ������ġ�� ����� �����Ѵ�.
void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

// �������� �������Ѵ�.
void CResourceManager::ReleaseAnimationSeequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	// �ؽ��Ŀ� ���������� ���� ī��Ʈ�� 1�̶�� �Ŵ����̿ܿ��� �����ϰ� ���� �ʴٴ� ���̴�.
	// ���� ����
	if (iter->second->GetRefCount() == 1)
		m_mapAnimationSequence.erase(iter);
}

// �������� ã�´�.
CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

// ���ο� ���带 ����� �׷쿡 ��Ͻ�Ų��.
bool CResourceManager::LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileNmae, const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	// �̹� ������ �̸����� �����ϴ� ������ ��ȯ
	if (Sound)
		return true;
	

	// ä�� �׷��� ã�ƿ´�.
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	// ���ο� ���带 �����
	Sound = new CSound;

	// ���ο� ���带 ä�� �׷쿡 ���
	if (!Sound->LoadSound(m_System, Group, Loop, Name, FileNmae, PathName))
	{
		SAFE_RELEASE(Sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}


// ���ο� ä�� �׷��� �����.
bool CResourceManager::CreateSoundChannelGroup(const std::string& Name)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(Name);

	if (Group)
		return true;


	FMOD_RESULT result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	// ������ �׷��� ������ �׷쿡 �߰����ش�.
	m_MasterGroup->addGroup(Group, false);

	m_mapChannelGroup.insert(std::make_pair(Name, Group));

	return true;
}

// ������ �׷쿡 ������ �������ش�.
bool CResourceManager::SetVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.0f);

	return true;
}

// �׷��� ������ �����Ѵ�.
bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.0f);

	return true;
}


// ���带 ����Ѵ�.
bool CResourceManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}


// ���带 �����.
bool CResourceManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

// ���带 �Ͻ������Ѵ�.
bool CResourceManager::SoundPause(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Pause();

	return true;
}

bool CResourceManager::SoundResume(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Resume();

	return true;
}

void CResourceManager::ReleaseSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);
}

FMOD::ChannelGroup* CResourceManager::FindSoundChannelGroup(const std::string& Name)
{
	auto iter = m_mapChannelGroup.find(Name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CResourceManager::FindSound(const std::string& Name)
{
	auto iter = m_mapSound.find(Name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadOtherFont(const TCHAR* FileName, const std::string& PathName)
{
	TCHAR FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		lstrcpy(FullPath, Info->Path);

	lstrcat(FullPath, FileName);

	AddFontResource(FullPath);


	m_FontLoadList.push_back(FullPath);

	return true;
}

bool CResourceManager::LoadFont(const std::string& Name, const TCHAR* FontName, int Width, int Height)
{
	CFont* Font = FindFont(Name);

	if (Font)
		return true;

	Font = new CFont;

	if (!Font->LoadFont(Name, FontName, Width, Height))
	{
		SAFE_DELETE(Font);
		return false;
	}


	m_mapFont.insert(std::make_pair(Name, Font));


	return true;
}

CFont* CResourceManager::FindFont(const std::string& Name)
{
	auto iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
