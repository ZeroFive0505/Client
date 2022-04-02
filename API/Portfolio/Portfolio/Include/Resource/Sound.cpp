#include "Sound.h"
#include "../PathManager.h"

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

bool CSound::LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const std::string& name, const char* fileName, const std::string PathName)
{
    m_Name = name;
    m_System = system;
    m_Group = group;
    m_Loop = loop;

    char fullPath[MAX_PATH] = {};

    const PathInfo* pathInfo = CPathManager::GetInst()->FindPath(PathName);

    if(pathInfo)
        strcpy_s(fullPath, pathInfo->multiBytePath);
    strcat_s(fullPath, fileName);

    FMOD_MODE mode = FMOD_DEFAULT;
    
    if (loop)
        mode = FMOD_LOOP_NORMAL;

    if (m_System->createSound(fullPath, mode, nullptr, &m_Sound) != FMOD_OK)
        return false;
    
    return true;
}

bool CSound::Load3DSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const std::string& name, const char* fileName, const std::string PathName)
{
    m_Name = name;
    m_System = system;
    m_Group = group;
    m_Loop = loop;

    char fullPath[MAX_PATH] = {};

    const PathInfo* pathInfo = CPathManager::GetInst()->FindPath(PathName);

    if (pathInfo)
        strcpy_s(fullPath, pathInfo->multiBytePath);
    strcat_s(fullPath, fileName);

    FMOD_MODE mode = FMOD_DEFAULT;

    if (loop)
        mode = FMOD_LOOP_NORMAL;

    if (m_System->createSound(fullPath, (mode | FMOD_3D), nullptr, &m_Sound) != FMOD_OK)
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
    if (!m_Channel)
        return;

    bool playing = false;

    m_Channel->isPlaying(&playing);

    if (playing)
        m_Channel->setPaused(true);
    
    m_Play = false;
}

void CSound::Resume()
{
    if (!m_Channel)
        return;

    bool playing = false;

    m_Channel->isPlaying(&playing);

    if (!playing)
        m_Channel->setPaused(false);

    m_Play = true;
}
