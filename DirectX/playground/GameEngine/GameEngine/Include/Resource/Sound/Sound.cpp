#include "Sound.h"
#include "../../PathManager.h"

CSound::CSound() :
    m_System(nullptr),
    m_Sound(nullptr),
    m_Group(nullptr),
    m_Channel(nullptr),
    m_Play(false),
    m_Loop(false)
{
}

CSound::~CSound()
{
    if (m_Sound)
        m_Sound->release();
}

bool CSound::LoadSound(FMOD::System* System, FMOD::ChannelGroup* group, 
    bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
    SetName(name);
    m_System = System;
    m_Group = group;
    m_Loop = loop;

    char fullPath[MAX_PATH] = {};

    const sPathInfo* info = CPathManager::GetInst()->FindPath(SOUND_PATH);

    if (info)
        strcpy_s(fullPath, info->pathMultiByte);

    strcat_s(fullPath, fileName);

    FMOD_MODE mode = FMOD_DEFAULT;

    if (m_Loop)
        mode = FMOD_LOOP_NORMAL;

    if (m_System->createSound(fullPath, mode, nullptr, &m_Sound) != FMOD_OK)
        return false;

    return true;
}

void CSound::Play()
{
    m_System->playSound(m_Sound, m_Group, false, &m_Channel);
    m_Play = true;
}

void CSound::Stop()
{
    if (m_Channel)
    {
        bool playing = false;

        m_Channel->isPlaying(&playing);

        if (playing)
        {
            m_Channel->stop();
            m_Channel = nullptr;

            m_Play = false;
        }
    }
}

void CSound::Pause()
{
    if (m_Channel)
    {
        bool playing = false;

        m_Channel->isPlaying(&playing);

        if (playing)
            m_Channel->setPaused(true);
        m_Play = false;
    }
}

void CSound::Resume()
{
    if (m_Channel)
    {
        bool playing = false;

        m_Channel->isPlaying(&playing);

        if (!playing)
            m_Channel->setPaused(false);

        m_Play = true;
    }
}
