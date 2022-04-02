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
            std::string name = iter->first;

            iter = m_mapAnimationSequence.erase(iter);

            CResourceManager::GetInst()->ReleaseAnimationSequence(name);
        }
    }


    {
        auto iter = m_mapTexture.begin();
        auto iterEnd = m_mapTexture.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapTexture.erase(iter);

            CResourceManager::GetInst()->ReleaseTexture(name);
        }
    }

    {
        auto iter = m_mapSound.begin();
        auto iterEnd = m_mapSound.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapSound.erase(iter);

            CResourceManager::GetInst()->ReleaseSound(name);
        }
    }
}

bool CSceneResource::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
        return false;

    CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

    m_mapTexture.insert(std::make_pair(name, texture));

    return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
        return false;

    CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

    m_mapTexture.insert(std::make_pair(name, texture));
    
    return true;
}

bool CSceneResource::LoadTexture(const std::string& name, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
        return false;

    CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

    m_mapTexture.insert(std::make_pair(name, texture));

    return true;
}

bool CSceneResource::ReleaseTexture(const std::string& name)
{
    if (FindTexture(name))
    {
        auto iter = m_mapTexture.find(name);
        m_mapTexture.erase(iter);
        CResourceManager::GetInst()->ReleaseTexture(name);

        return true;
    }
    else
        return false;
}


void CSceneResource::SetTextureColorKey(const std::string& name, const unsigned char r, const unsigned char g, const unsigned char b, int index)
{
    CResourceManager::GetInst()->SetTextureColorKey(name, r, g, b, index);
}

CTexture* CSceneResource::FindSpriteFont(const std::string& name)
{
    return CResourceManager::GetInst()->FindSpriteFont(name);
}

CTexture* CSceneResource::FindTexture(const std::string& name)
{
    auto iter = m_mapTexture.find(name);

    if (iter == m_mapTexture.end())
        return nullptr;

    return iter->second;
}

bool CSceneResource::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName)
{
    if (FindAnimationSequence(sequenceName))
        return true;

    if (!CResourceManager::GetInst()->CreateAnimationSequence(sequenceName, textureName))
        return false;

    CAnimationSequence* sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);

    m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

    return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
    if (FindAnimationSequence(sequenceName))
        return true;

    LoadTexture(textureName, fileName, pathName);

    if (!CResourceManager::GetInst()->CreateAnimationSequence(sequenceName, textureName))
        return false;

    CAnimationSequence* sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);

    m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

    return true;
}

bool CSceneResource::CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName, const std::vector<std::wstring>& vecFileName, const std::string& pathName)
{
    if (FindAnimationSequence(sequenceName))
        return true;

    LoadTexture(textureName, vecFileName, pathName);

    if (!CResourceManager::GetInst()->CreateAnimationSequence(sequenceName, textureName))
        return false;

    CAnimationSequence* sequence = CResourceManager::GetInst()->FindAnimationSequence(sequenceName);

    m_mapAnimationSequence.insert(std::make_pair(sequenceName, sequence));

    return true;
}

void CSceneResource::AddAnimationFrameData(const std::string& sequenceName, const Vector2& startPos, const Vector2& size)
{
    CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

    if (!sequence)
        return;

    sequence->AddFrameData(startPos, size);
}

void CSceneResource::AddAnimationFrameData(const std::string& sequenceName, float posX, float posY, float sizeX, float sizeY)
{
    CAnimationSequence* sequence = FindAnimationSequence(sequenceName);

    if (!sequence)
        return;

    sequence->AddFrameData(posX, posY, sizeX, sizeY);
}

CAnimationSequence* CSceneResource::FindAnimationSequence(const std::string& name)
{
    auto iter = m_mapAnimationSequence.find(name);

    if (iter == m_mapAnimationSequence.end())
        return nullptr;

    return iter->second;
}

bool CSceneResource::LoadSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
    if (FindSound(name))
        return true;

    if (!CResourceManager::GetInst()->LoadSound(groupName, loop, name, fileName, pathName))
        return false;

    CSound* sound = CResourceManager::GetInst()->FindSound(name);

    m_mapSound.insert(std::make_pair(name, sound));

    return true;
}

bool CSceneResource::Load3DSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
    if (FindSound(name))
        return true;

    if (!CResourceManager::GetInst()->Load3DSound(groupName, loop, name, fileName, pathName))
        return false;

    CSound* sound = CResourceManager::GetInst()->FindSound(name);

    m_mapSound.insert(std::make_pair(name, sound));

    return true;
}


bool CSceneResource::SetVolume(int volume)
{
    return CResourceManager::GetInst()->SetVolume(volume);
}

bool CSceneResource::SetVolume(const std::string& groupName, int volume)
{
    return CResourceManager::GetInst()->SetVolume(groupName, volume);
}

bool CSceneResource::SoundPlay(const std::string& name)
{
    return CResourceManager::GetInst()->SoundPlay(name);
}

bool CSceneResource::SoundStop(const std::string& name)
{
    return CResourceManager::GetInst()->SoundStop(name);
}

bool CSceneResource::SoundPause(const std::string& name)
{
    return CResourceManager::GetInst()->SoundPause(name);
}

bool CSceneResource::SoundResume(const std::string& name)
{
    return CResourceManager::GetInst()->SoundResume(name);
}

bool CSceneResource::IsPlaying(const std::string& name)
{
    return CResourceManager::GetInst()->IsPlaying(name);
}

void CSceneResource::UpdateEngineSound(const std::string& name, bool IsPlayer, const Vector2& pos, const float height, const float speedForward, const float speedTurn)
{
    CResourceManager::GetInst()->UpdateEngineSound(name, IsPlayer, pos, height, speedForward, speedTurn);
}

void CSceneResource::UpdateListener(const Vector2& pos, float angle, float height)
{
    CResourceManager::GetInst()->UpdateListener(pos, angle, height);
}

void CSceneResource::PlayEngine(const std::string& name, bool IsPlayer)
{
    CResourceManager::GetInst()->PlayEngine(name, IsPlayer);
}

void CSceneResource::StopAllMusic()
{
    CResourceManager::GetInst()->StopAllMusic();
}

void CSceneResource::StopAllSFX()
{
    CResourceManager::GetInst()->StopAllSFX();
}

CSound* CSceneResource::FindSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter == m_mapSound.end())
        return nullptr;

    return iter->second;
}
