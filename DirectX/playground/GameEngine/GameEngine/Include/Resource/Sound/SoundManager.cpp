#include "SoundManager.h"

CSoundManager::CSoundManager() :
    m_System(nullptr),
    m_MasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
    auto iter = m_mapGroup.begin();
    auto iterEnd = m_mapGroup.end();

    for (; iter != iterEnd; iter++)
    {
        iter->second->release();
    }

    m_mapSound.clear();

    if (m_System)
    {
        m_System->close();
        m_System->release();
    }
}

bool CSoundManager::Init()
{
    FMOD_RESULT	result = FMOD::System_Create(&m_System);

    if (result != FMOD_OK)
        return false;

    result = m_System->init(4093, FMOD_INIT_NORMAL, nullptr);

    if (result != FMOD_OK)
        return false;

    // MasterGroup을 얻어온다.
    result = m_System->getMasterChannelGroup(&m_MasterGroup);

    if (result != FMOD_OK)
        return false;

    m_mapGroup.insert(std::make_pair("Master", m_MasterGroup));

    return true;
}

void CSoundManager::Update()
{
    m_System->update();
}

bool CSoundManager::LoadSound(const std::string& channelGroupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
    CSound* sound = FindSound(name);

    if (sound)
        return true;

    FMOD::ChannelGroup* group = FindChannelGroup(channelGroupName);

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

bool CSoundManager::CreateSoundChannelGroup(const std::string& name)
{
    FMOD::ChannelGroup* group = FindChannelGroup(name);

    if (group)
        return true;

    FMOD_RESULT result = m_System->createChannelGroup(name.c_str(), &group);

    if (result != FMOD_OK)
        return false;

    m_MasterGroup->addGroup(group);

    m_mapGroup.insert(std::make_pair(name, group));

    return true;
}

bool CSoundManager::SetVolume(int volume)
{
    FMOD_RESULT result = m_MasterGroup->setVolume(volume / 100.0f);

    return result == FMOD_OK;
}

bool CSoundManager::SetVolume(const std::string& channelGroupName, int volume)
{
    FMOD::ChannelGroup* group = FindChannelGroup(channelGroupName);

    if (!group)
        return false;

    FMOD_RESULT result = group->setVolume(volume / 100.0f);

    return result == FMOD_OK;
}

bool CSoundManager::SoundPlay(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Play();

    return true;
}

bool CSoundManager::SoundStop(const std::string& name)
{
    CSound* sound = FindSound(name);


    if (!sound)
        return false;

    sound->Stop();

    return true;
}

bool CSoundManager::SoundPause(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Pause();

    return true;
}

bool CSoundManager::SoundResume(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Resume();

    return true;
}

CSound* CSoundManager::FindSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter == m_mapSound.end())
        return nullptr;

    return iter->second;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& name)
{
    auto iter = m_mapGroup.find(name);

    if (iter == m_mapGroup.end())
        return nullptr;

    return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter != m_mapSound.end())
    {
        if (iter->second->GetRefCount() == 1)
            m_mapSound.erase(iter);
    }
}
