#include "ResourceManager.h"
#include "../PathManager.h"

CResourceManager* CResourceManager::m_Inst = nullptr;

float logFunc(const float value)
{
	float ret = -log10f(powf(1 - value * 0.9f, 1.0f));

	if (ret > 1.0f)
		ret = 1.0f;

	return ret;
}

CResourceManager::CResourceManager() :
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	m_mapSound.clear();
	m_mapAnimationSequence.clear();
	m_mapTexture.clear();
	m_mapSpriteFont.clear();
	m_mapFont.clear();

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

	result = m_System->init(128, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapChannelGroup.insert(std::make_pair("Master", m_MasterGroup));

	CreateSoundChannelGroup("BGM");
	CreateSoundChannelGroup("Effect");
	CreateSoundChannelGroup("UI");
	CreateSoundChannelGroup("Engine");

	LoadOtherFont(TEXT("Gameplay.ttf"));

	LoadFont("DefaultFont", TEXT("Gameplay"), 0, 15);

	for (int i = 0; i < 26; i++)
	{
		std::string name;
		name = 'A' + i;
		TCHAR filePath[MAX_PATH] = {};
		wsprintf(filePath, TEXT("UI/Letters/%c.bmp"), 'A' + i);
		LoadTexture(name, filePath);
		CTexture* temp = FindTexture(name);
		temp->SetColorKey(255, 0, 255);
		temp->SetScale(1.5f, 1.5f);
		m_mapSpriteFont.insert(std::make_pair(name, temp));
	}

	for (int i = 0; i < 10; i++)
	{
		std::string name;
		name = "Small_" + std::to_string(i);
		TCHAR filePath[MAX_PATH] = {};
		wsprintf(filePath, TEXT("UI/Letters/%d.bmp"), i);
		LoadTexture(name, filePath);
		CTexture* temp = FindTexture(name);
		temp->SetColorKey(255, 0, 255);
		temp->SetScale(1.5f, 1.5f);
		m_mapSpriteFont.insert(std::make_pair(name, temp));
	}
	
	CTexture* temp;

	LoadTexture("(", TEXT("UI/Letters/(.bmp"));

	temp = FindTexture("(");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("(", temp));

	LoadTexture(")", TEXT("UI/Letters/).bmp"));

	temp = FindTexture(")");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair(")", FindTexture(")")));

	LoadTexture("SmallBar", TEXT("UI/Letters/bar.bmp"));

	temp = FindTexture("SmallBar");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("SmallBar", FindTexture("SmallBar")));

	LoadTexture("SmallSingle", TEXT("UI/Letters/single.bmp"));

	temp = FindTexture("SmallSingle");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("SmallSingle", FindTexture("SmallSingle")));

	LoadTexture("SmallDouble", TEXT("UI/Letters/double.bmp"));

	temp = FindTexture("SmallDouble");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("SmallDouble", FindTexture("SmallDouble")));

	LoadTexture("Question", TEXT("UI/Letters/question.bmp"));

	temp = FindTexture("Question");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("Question", FindTexture("Question")));

	LoadTexture("Exclamantion", TEXT("UI/Letters/exclamantion.bmp"));

	temp = FindTexture("Exclamantion");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("Exclamantion", FindTexture("Exclamantion")));

	LoadTexture("Left", TEXT("UI/Letters/left.bmp"));

	temp = FindTexture("Left");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("Left", FindTexture("Left")));

	LoadTexture("Right", TEXT("UI/Letters/right.bmp"));

	temp = FindTexture("Right");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("Right", FindTexture("Right")));

	LoadTexture("Space", TEXT("UI/Letters/space.bmp"));

	temp = FindTexture("Space");

	temp->SetColorKey(255, 0, 255);

	temp->SetScale(1.5f, 1.5f);

	m_mapSpriteFont.insert(std::make_pair("Space", temp));

	return true;
}

void CResourceManager::Update()
{
	m_System->update();
}

bool CResourceManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTexture(name, fileName, pathName))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTextureFullPath(name, fullPath))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));
	
	return true;
}

bool CResourceManager::LoadTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
	CTexture* texture = FindTexture(name);

	if (texture)
		return true;

	texture = new CTexture;

	if (!texture->LoadTexture(name, vecFileName, pathName))
	{
		SAFE_RELEASE(texture);
		return false;
	}

	m_mapTexture.insert(std::make_pair(name, texture));

	return true;
}

void CResourceManager::SetTextureColorKey(const std::string& name, const unsigned char r, const unsigned char g, const unsigned char b, int index)
{
	CTexture* texture = FindTexture(name);

	if (!texture)
		return;

	texture->SetColorKey(r, g, b, index);
}

void CResourceManager::ReleaseTexture(const std::string& name)
{
	auto iter = m_mapTexture.find(name);

	if (iter->second->GetRefCount() == 1)
		m_mapTexture.erase(iter);
}

CTexture* CResourceManager::FindTexture(const std::string& name)
{
	auto iter = m_mapTexture.find(name);

	if (iter == m_mapTexture.end())
		return nullptr;

	return iter->second;
}

CTexture* CResourceManager::FindSpriteFont(const std::string& name)
{
	auto iter = m_mapSpriteFont.find(name);

	if (iter == m_mapSpriteFont.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName)
{
	CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

	if (sequence)
		return true;

	sequence = new CAnimationSequence;

	CTexture* texture = FindTexture(textureName);

	if (!sequence->Init(sequenceName, texture))
	{
		SAFE_RELEASE(sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

	return true;
}

bool CResourceManager::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

	if (sequence)
		return true;

	if (!LoadTexture(textureName, fileName, pathName))
		return false;

	sequence = new CAnimationSequence;

	CTexture* texture = FindTexture(textureName);

	if (!sequence->Init(sequenceName, texture))
	{
		SAFE_RELEASE(sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

	return true;
}

bool CResourceManager::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
	CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

	if (sequence)
		return true;

	if (!LoadTexture(textureName, vecFileName, pathName))
		return false;

	sequence = new CAnimationSequence;

	CTexture* texture = FindTexture(textureName);

	if (!sequence->Init(sequenceName, texture))
	{
		SAFE_RELEASE(sequence);
		return false;
	}

	m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

	return true;
}

void CResourceManager::AddAnimationFrameData(const std::string& sequenceName, const Vector2& startPos, const Vector2& size)
{
	CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

	if (!sequence)
		return;

	sequence->AddFrameData(startPos, size);
}

void CResourceManager::AddAnimationFrameData(const std::string& sequenceName, float posX, float posY, float sizeX, float sizeY)
{
	CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

	if (!sequence)
		return;

	sequence->AddFrameData(posX, posY, sizeX, sizeY);
}

void CResourceManager::ReleaseAnimationSequence(const std::string& name)
{
	auto iter = m_mapAnimationSequence.find(name);

	if (iter->second->GetRefCount() == 1)
		m_mapAnimationSequence.erase(iter);
}

CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& name)
{
	auto iter = m_mapAnimationSequence.find(name);

	if (iter == m_mapAnimationSequence.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
	CSound* sound = FindSound(name);

	if (sound)
		return true;

	FMOD::ChannelGroup* group = FindSoundChannelGroup(groupName);

	if (!group)
		return false;

	sound = new CSound;

	if (!sound->LoadSound(m_System, group, loop, name, fileName, pathName))
	{
		SAFE_RELEASE(sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(name, sound));

	return true;
}

bool CResourceManager::Load3DSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
	CSound* sound = FindSound(name);

	if (sound)
		return true;

	FMOD::ChannelGroup* group = FindSoundChannelGroup(groupName);

	if (!group)
		return false;

	FMOD_RESULT result;

	result = group->setMode(FMOD_3D);

	sound = new CSound;

	if (!sound->Load3DSound(m_System, group, loop, name, fileName, pathName))
	{
		SAFE_RELEASE(sound);
		return false;
	}

	m_mapSound.insert(std::make_pair(name, sound));

	return true;
}


bool CResourceManager::CreateSoundChannelGroup(const std::string& name)
{
	FMOD::ChannelGroup* group = FindSoundChannelGroup(name);

	if (group)
		return true;

	FMOD_RESULT result = m_System->createChannelGroup(name.c_str(), &group);

	if (result != FMOD_OK)
		return false;

	m_MasterGroup->addGroup(group, false);

	m_mapChannelGroup.insert(std::make_pair(name, group));

	return true;
}

bool CResourceManager::SetVolume(int volume)
{
	m_MasterGroup->setVolume(volume / 100.0f);

	return true;
}

bool CResourceManager::SetVolume(const std::string& groupName, int volume)
{
	FMOD::ChannelGroup* group = FindSoundChannelGroup(groupName);

	if (!group)
		return false;

	group->setVolume(volume / 100.0f);

	return true;
}

bool CResourceManager::SoundPlay(const std::string& name)
{
	CSound* sound = FindSound(name);

	if (!sound)
		return false;

	sound->Play();

	return true;
}

bool CResourceManager::SoundStop(const std::string& name)
{
	CSound* sound = FindSound(name);

	if (!sound)
		return false;

	sound->Stop();

	return true;
}

bool CResourceManager::SoundPause(const std::string& name)
{
	CSound* sound = FindSound(name);

	if (!sound)
		return false;

	sound->Pause();

	return true;
}

bool CResourceManager::SoundResume(const std::string& name)
{
	CSound* sound = FindSound(name);

	if (!sound)
		return false;

	sound->Resume();

	return true;
}

bool CResourceManager::IsPlaying(const std::string& name)
{
	CSound* sound = FindSound(name);

	if (sound->IsPlay())
		return true;
	else
		return false;
}

void CResourceManager::StopAllSFX()
{
	FMOD::ChannelGroup* group = FindSoundChannelGroup("Effect");

	group->stop();
}

void CResourceManager::StopAllMusic()
{
	auto iter = m_mapSound.begin();
	auto iterEnd = m_mapSound.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter).first == "Engine")
			continue;
		SoundStop((*iter).first);
	}
}

void CResourceManager::UpdateEngineSound(const std::string& name, bool IsPlayer, const Vector2& pos, const float height, const float speedForward,
	const float speedTurn)
{
	CSound* sound = FindSound(name);

	float maxLinearSeped = 0.4992f / 2.0f;
	float maxSpeedTurn = 3.6f;
	float pitch = 1.0f;


	pitch += speedForward / maxLinearSeped;
	pitch -= fabs(speedTurn) / maxSpeedTurn * 0.65f;

	if (height > 0.0f)
		pitch += 0.35f;

	pitch = fmin(pitch, 2.0f);

	FMOD_RESULT result;


	FMOD_VECTOR v;
	v.x = pos.x;
	v.y = pos.y;
	v.z = height / 80.0f;
	result = sound->m_Group->set3DAttributes(&v, nullptr);

	result = sound->m_Group->setPitch(pitch);
}

void CResourceManager::UpdateListener(const Vector2& pos, float angle, float height)
{
	FMOD_VECTOR v;
	v.x = pos.x;
	v.y = pos.y;
	v.z = height / 80.0f;

	FMOD_VECTOR dir;

	dir.x = -cosf(angle);
	dir.y = -sinf(angle);
	dir.z = 0.0f;

	FMOD_VECTOR up;

	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;

	FMOD_RESULT result;

	result = m_System->set3DListenerAttributes(0, &v, nullptr, &dir, &up);
}

void CResourceManager::PlayEngine(const std::string& name, bool IsPlayer)
{
	CSound* sound = FindSound(name);

	FMOD_RESULT result;

		sound->Play();

		sound->m_Group->setVolume(75.0f / 100.0f);
		result = sound->m_Sound->set3DMinMaxDistance(1.0f / MAP_ASSETS_WIDTH * 3.0f, 1.0f);
	
}

void CResourceManager::ReleaseSound(const std::string& name)
{
	auto iter = m_mapSound.find(name);

	if (iter->second->GetRefCount() == 1)
		m_mapSound.erase(iter);
}

FMOD::ChannelGroup* CResourceManager::FindSoundChannelGroup(const std::string& name)
{
	auto iter = m_mapChannelGroup.find(name);

	if (iter == m_mapChannelGroup.end())
		return nullptr;

	return iter->second;
}

CSound* CResourceManager::FindSound(const std::string& name)
{
	auto iter = m_mapSound.find(name);

	if (iter == m_mapSound.end())
		return nullptr;

	return iter->second;
}

bool CResourceManager::LoadOtherFont(const TCHAR* fileName, const std::string& pathName)
{
	TCHAR fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->unicodePath);

	lstrcat(fullPath, fileName);

	AddFontResource(fullPath);

	m_FontLoadList.push_back(fullPath);

	return true;
}

bool CResourceManager::LoadFont(const std::string& name, const TCHAR* fontName, int width, int height)
{
	CFont* font = FindFont(name);

	if (font)
		return true;

	font = new CFont;

	if (!font->LoadFont(name, fontName, width, height))
	{
		SAFE_DELETE(font);
		return false;
	}


	m_mapFont.insert(std::make_pair(name, font));

	return true;
}

CFont* CResourceManager::FindFont(const std::string& name)
{
	auto iter = m_mapFont.find(name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
