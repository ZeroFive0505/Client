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
	// 먼저 애니메이션을 전부 삭제한다. 애니메이션 또한 텍스쳐에서 스프라이트 시트형태로
	// 만들어지는 것이기에 텍스쳐 전에 먼저 삭제한다.
	m_mapAnimationSequence.clear();

	// 그 다음 텍스쳐를 삭제한다.
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

	// System 초기화
	result = m_System->init(128, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	// Master channel Group을 얻어온다.
	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));


	// 각각 채널별로 그룹을 만들어 놓는다.
	// BGM 채널그룹에 묶인 채널들은 BGM채널 그룹의 볼륨이 줄어들면 모두 다같이 줄어들 것이다.
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

// 텍스쳐를 파일이름과 경로를 통해서 불러온다.
bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = FindTexture(Name);

	// 이미 해당 키로 텍스쳐가 존재한다면 반환한다.
	if (Texture)
		return true;

	// 새롭게 텍스쳐를 만든다.
	Texture = new CTexture;

	// 만약 텍스처 로드가 실패했을시에는 릴리즈
	if (!Texture->LoadTexture(Name, FileName, PathName))
	{
		SAFE_RELEASE(Texture);
		return false;
	}

	// 리소스매니저가 관리하는 새로운 텍스쳐로 추가한다.
	m_mapTexture.insert(std::make_pair(Name, Texture));

	return true;
}

// 전체 경로를 통해서 텍스쳐를 로딩한다.
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

// 여러장으로 되어있는 텍스쳐를 로딩한다.
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

// 텍스처의 칼라 키를 설정한다.
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

// 텍스쳐를 릴리즈한다.
void CResourceManager::ReleaseTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	// 만약 릴리즈하려는 텍스쳐의 레퍼런스 카운트가 1이라는 것은
	// 리소스매니저 이외에는 아무런 참조자가 없다는 뜻이다.
	// 따라서 삭제한다.
	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);
}

// 이름을 키로 텍스쳐를 찾아온다.
CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	auto iter = m_mapTexture.find(Name);

	if (iter == m_mapTexture.end())
		return nullptr;
	
	return iter->second;
}

// 애니메이션 시퀀스를 만든다.
bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	// 만약 이미 해당 키로 애니메이션이 있을 경우는 바로 반환한다.
	if (Sequence)
		return true;

	// 새로운 애니메이션 시퀀스를 만든다.
	Sequence = new CAnimationSequence;

	// 애니메이션 시퀀스를 만들기위해서 텍스쳐를 가져온다.
	CTexture* Texture = FindTexture(TextureName);

	// 시퀀스를 초기화하고
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	// 새롭게 등록한다.
	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// 애니메이션 시퀀스를 텍스쳐 이름과 파일 이름, 경로로 설정한다.
bool CResourceManager::CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName, const TCHAR* FileName, const std::string& PathName)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (Sequence)
		return true;

	// 먼저 텍스쳐를 불러오고
	if (!LoadTexture(TextureName, FileName, PathName))
		return false;

	Sequence = new CAnimationSequence;

	CTexture* Texture = FindTexture(TextureName);

	// 시퀀스를 설정한다.
	if (!Sequence->Init(SequenceName, Texture))
	{
		SAFE_RELEASE(Sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(SequenceName, Sequence));

	return true;
}

// 여러장으로 되어있는 이미지를 시퀀스로 설정한다.
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

// 해당 시퀀스의 시작위치와 사이즈를 지정한다.
void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2 Size)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(StartPos, Size);
}

// 해당 시퀀스의 시작위치와 사이즈를 지정한다.
void CResourceManager::AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY)
{
	CAnimationSequence* Sequence = FindAnimationSequence(SequenceName);

	if (!Sequence)
		return;

	Sequence->AddFrameData(PosX, PosY, SizeX, SizeY);
}

// 시퀀스를 릴리즈한다.
void CResourceManager::ReleaseAnimationSeequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	// 텍스쳐와 마찬가지로 현재 카운트가 1이라면 매니저이외에는 참조하고 있지 않다는 뜻이다.
	// 따라서 삭제
	if (iter->second->GetRefCount() == 1)
		m_mapAnimationSequence.erase(iter);
}

// 시퀀스를 찾는다.
CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& Name)
{
	auto iter = m_mapAnimationSequence.find(Name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

// 새로운 사운드를 만들어 그룹에 등록시킨다.
bool CResourceManager::LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileNmae, const std::string& PathName)
{
	CSound* Sound = FindSound(Name);

	// 이미 동일한 이름으로 존재하는 사운드라면 반환
	if (Sound)
		return true;
	

	// 채널 그룹을 찾아온다.
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	// 새로운 사운드를 만들고
	Sound = new CSound;

	// 새로운 사운드를 채널 그룹에 등록
	if (!Sound->LoadSound(m_System, Group, Loop, Name, FileNmae, PathName))
	{
		SAFE_RELEASE(Sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(Name, Sound));

	return true;
}


// 새로운 채널 그룹을 만든다.
bool CResourceManager::CreateSoundChannelGroup(const std::string& Name)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(Name);

	if (Group)
		return true;


	FMOD_RESULT result = m_System->createChannelGroup(Name.c_str(), &Group);

	if (result != FMOD_OK)
		return false;

	// 생성된 그룹을 마스터 그룹에 추가해준다.
	m_MasterGroup->addGroup(Group, false);

	m_mapChannelGroup.insert(std::make_pair(Name, Group));

	return true;
}

// 마스터 그룹에 볼륨을 조절해준다.
bool CResourceManager::SetVolume(int Volume)
{
	m_MasterGroup->setVolume(Volume / 100.0f);

	return true;
}

// 그룹의 볼륨을 조절한다.
bool CResourceManager::SetVolume(const std::string& GroupName, int Volume)
{
	FMOD::ChannelGroup* Group = FindSoundChannelGroup(GroupName);

	if (!Group)
		return false;

	Group->setVolume(Volume / 100.0f);

	return true;
}


// 사운드를 재생한다.
bool CResourceManager::SoundPlay(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Play();

	return true;
}


// 사운드를 멈춘다.
bool CResourceManager::SoundStop(const std::string& Name)
{
	CSound* Sound = FindSound(Name);

	if (!Sound)
		return false;

	Sound->Stop();

	return true;
}

// 사운드를 일시정지한다.
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
