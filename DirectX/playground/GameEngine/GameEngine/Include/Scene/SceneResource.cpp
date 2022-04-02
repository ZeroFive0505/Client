#include "SceneResource.h"

CSceneResource::CSceneResource() :
    m_Scene(nullptr)
{
}

CSceneResource::~CSceneResource()
{
    {
        auto iter = m_mapMesh.begin();
        auto iterEnd = m_mapMesh.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapMesh.erase(iter);
        }
    }

    {
        auto iter = m_mapShader.begin();
        auto iterEnd = m_mapShader.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapShader.erase(iter);
        }
    }

    {
        auto iter = m_mapTexture.begin();
        auto iterEnd = m_mapTexture.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapTexture.erase(iter);
        }
    }

    {
        auto iter = m_mapMaterial.begin();
        auto iterEnd = m_mapMaterial.end();

        for (; iter != iterEnd;)
        {
            std::string name = iter->first;

            iter = m_mapMaterial.erase(iter);
        }
    }

    {
        auto iter = m_mapSequence2D.begin();
        auto iterEnd = m_mapSequence2D.end();

        for (; iter != iterEnd; iter++)
        {
            std::string name = iter->first;
        }
    }

    {
        auto iter = m_mapParticle.begin();
        auto iterEnd = m_mapParticle.end();

        for (; iter != iterEnd; iter++)
        {
            std::string name = iter->first;
        }
    }
}

CMesh* CSceneResource::FindMesh(const std::string& name)
{
    auto iter = m_mapMesh.find(name);

    // ���� �����ҽ��� ���ٸ�
    if (iter == m_mapMesh.end())
    {
        // ���ҽ��Ŵ����� Ȯ���Ѵ�.
        CMesh* mesh = CResourceManager::GetInst()->FindMesh(name);

        if (!mesh)
            return nullptr;

        m_mapMesh.insert(std::make_pair(name, mesh));

        return mesh;
    }

    return iter->second;
}

CShader* CSceneResource::FindShader(const std::string& name)
{
    auto iter = m_mapShader.find(name);

    if (iter == m_mapShader.end())
    {
        CShader* shader = CResourceManager::GetInst()->FindShader(name);

        if (!shader)
            return nullptr;

        m_mapShader.insert(std::make_pair(name, shader));

        return shader;
    }

    return iter->second;
}

CMaterial* CSceneResource::FindMaterial(const std::string& name)
{
    auto iter = m_mapMaterial.find(name);

    if (iter == m_mapMaterial.end())
    {
        CMaterial* mat = CResourceManager::GetInst()->FindMaterial(name);

        if (!mat)
            return nullptr;

        m_mapMaterial.insert(std::make_pair(name, mat));

        return mat;
    }

    return iter->second;
}

bool CSceneResource::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(name, fileName, pathName))
        return false;

    m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));

    return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTextureFullPath(name, fullPath))
        return false;

    m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));

    return true;
}

bool CSceneResource::LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTexture(name, vecFileName, pathName))
        return false;

    m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));

    return true;
}

bool CSceneResource::LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
    if (FindTexture(name))
        return true;

    if (!CResourceManager::GetInst()->LoadTextureFullPath(name, vecFullPath))
        return false;

    m_mapTexture.insert(std::make_pair(name, CResourceManager::GetInst()->FindTexture(name)));

    return true;
}

CTexture* CSceneResource::FindTexture(const std::string& name)
{
    auto iter = m_mapTexture.find(name);

    if (iter == m_mapTexture.end())
    {
        CTexture* texture = CResourceManager::GetInst()->FindTexture(name);

        if (!texture)
            return nullptr;

        m_mapTexture.insert(std::make_pair(name, texture));

        return texture;
    }

    return iter->second;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
    if (FindAnimationSequence2D(name))
        return true;

    if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, textureName, fileName, pathName))
        return false;

    m_mapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));

    return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, CTexture* texture)
{
    if (FindAnimationSequence2D(name))
        return true;

    if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, texture))
        return false;

    m_mapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));

    return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
    if (FindAnimationSequence2D(name))
        return true;

    if (!CResourceManager::GetInst()->CreateAnimationSequence2D(name, textureName, vecFileName))
        return false;

    m_mapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));

    return true;
}

bool CSceneResource::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFullPath)
{
    if (FindAnimationSequence2D(name))
        return true;

    if (!CResourceManager::GetInst()->CreateAnimationSequence2DFullPath(name, textureName, vecFullPath))   
        return false;

    m_mapSequence2D.insert(std::make_pair(name, CResourceManager::GetInst()->FindAnimationSequence2D(name)));

    return true;
}

void CSceneResource::AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
    CAnimationSequence2D* anim = FindAnimationSequence2D(name);

    if (!anim)
        return;

    anim->AddFrame(start, size);
}

void CSceneResource::AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float width, float height)
{
    CAnimationSequence2D* anim = FindAnimationSequence2D(name);

    if (!anim)
        return;

    anim->AddFrame(startX, startY, width, height);
}

CAnimationSequence2D* CSceneResource::FindAnimationSequence2D(const std::string& name)
{
    auto iter = m_mapSequence2D.find(name);

    if (iter == m_mapSequence2D.end())
    {
        CAnimationSequence2D* anim = CResourceManager::GetInst()->FindAnimationSequence2D(name);

        if (!anim)
            return nullptr;

        m_mapSequence2D.insert(std::make_pair(name, anim));

        return anim;
    }

    return iter->second;
}

CAnimation2DConstantBuffer* CSceneResource::GetAnimation2DCBuffer() const
{
    return CResourceManager::GetInst()->GetAnimation2DCBuffer();
}

bool CSceneResource::SaveSequence2DFullPath(const std::string& name, const char* fullPath)
{
    return CResourceManager::GetInst()->SaveSequence2DFullPath(name, fullPath);
}

bool CSceneResource::LoadSequence2DFullPath(const char* fullPath)
{
    std::string sequenceName;

    if (!CResourceManager::GetInst()->LoadSequence2DFullPath(sequenceName, fullPath, m_Scene))
        return false;

    if (FindAnimationSequence2D(sequenceName))
        return true;

    CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);

    m_mapSequence2D.insert(std::make_pair(sequenceName, sequence));

    return true;
}

bool CSceneResource::LoadSequence2DFullPath(std::string& resultName, const char* fullPath)
{
    if (!CResourceManager::GetInst()->LoadSequence2DFullPath(resultName, fullPath, m_Scene))
        return false;

    if (FindAnimationSequence2D(resultName))
        return true;

    CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(resultName);

    m_mapSequence2D.insert(std::make_pair(resultName, sequence));

    return true;
}

bool CSceneResource::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
    return CResourceManager::GetInst()->SaveSequence2D(name, fileName, pathName);
}

bool CSceneResource::LoadSequence2D(const char* fileName, const std::string& pathName)
{
    std::string sequenceName;

    if (!CResourceManager::GetInst()->LoadSequence2D(sequenceName, fileName, pathName, m_Scene))
        return false;

    if (FindAnimationSequence2D(sequenceName))
        return true;

    CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(sequenceName);

    m_mapSequence2D.insert(std::make_pair(sequenceName, sequence));

    return true;
}

bool CSceneResource::LoadSequence2D(std::string& resultName, const char* fileName, const std::string& pathName)
{
    if (!CResourceManager::GetInst()->LoadSequence2D(resultName, fileName, pathName, m_Scene))
        return false;

    if (FindAnimationSequence2D(resultName))
        return true;

    CAnimationSequence2D* sequence = CResourceManager::GetInst()->FindAnimationSequence2D(resultName);

    m_mapSequence2D.insert(std::make_pair(resultName, sequence));

    return true;
}

bool CSceneResource::LoadSound(const std::string& channelGroupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
    if (FindSound(name))
        return true;

    if (!CResourceManager::GetInst()->LoadSound(channelGroupName, loop, name, fileName, pathName))
        return false;

    m_mapSound.insert(std::make_pair(name, CResourceManager::GetInst()->FindSound(name)));

    return true;
}

bool CSceneResource::CreateSoundChannelGroup(const std::string& name)
{
    return CResourceManager::GetInst()->CreateSoundChannelGroup(name);
}

bool CSceneResource::SetVolume(int volume)
{
    return CResourceManager::GetInst()->SetVolume(volume);
}

bool CSceneResource::SetVolume(const std::string& channelGroupName, int volume)
{
    return CResourceManager::GetInst()->SetVolume(channelGroupName, volume);
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

CSound* CSceneResource::FindSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter == m_mapSound.end())
    {
        CSound* sound = CResourceManager::GetInst()->FindSound(name);

        if (!sound)
            return nullptr;

        m_mapSound.insert(std::make_pair(name, sound));

        return sound;
    }

    return iter->second;
}

FMOD::ChannelGroup* CSceneResource::FindChannelGroup(const std::string& name)
{
    return CResourceManager::GetInst()->FindChannelGroup(name);
}

bool CSceneResource::CreateParticle(const std::string& name)
{
    if (FindParticle(name))
        return true;

    if (!CResourceManager::GetInst()->CreateParticle(name))
        return false;

    m_mapParticle.insert(std::make_pair(name, CResourceManager::GetInst()->FindParticle(name)));

    return true;
}

CParticle* CSceneResource::FindParticle(const std::string& name)
{
    auto iter = m_mapParticle.find(name);

    if (iter == m_mapParticle.end())
    {
        CParticle* particle = CResourceManager::GetInst()->FindParticle(name);

        if (!particle)
            return nullptr;

        m_mapParticle.insert(std::make_pair(name, particle));

        return particle;
    }

    return iter->second;
}