#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation() :
    m_CurrentAnimation(nullptr)
{
}

CAnimation::CAnimation(const CAnimation& anim)
{
    auto iter = anim.m_mapAnimation.begin();
    auto iterEnd = anim.m_mapAnimation.end();

    for (; iter != iterEnd; iter++)
    {
        AnimationInfo* info = new AnimationInfo;

        info->sequence = iter->second->sequence;
        info->frame = iter->second->frame;
        info->playTime = iter->second->playTime;
        info->playScale = iter->second->playScale;
        info->loop = iter->second->loop;
        info->reverse = iter->second->reverse;

        if (anim.m_CurrentAnimation->sequence->GetName() == info->sequence->GetName())
            m_CurrentAnimation = info;

        m_mapAnimation.insert(std::make_pair(iter->first, info));
    }
}

CAnimation::~CAnimation()
{
    auto iter = m_mapAnimation.begin();
    auto iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; iter++)
    {
        SAFE_DELETE(iter->second);
    }
}

void CAnimation::AddAnimation(const std::string& sequenceName, bool loop, float playTime, float playScale, bool reverse)
{
    CAnimationSequence* sequenece = m_Scene->GetSceneResoruce()->FindAnimationSequence(sequenceName);

    if (!sequenece)
        return;

    AnimationInfo* info = new AnimationInfo;

    info->sequence = sequenece;
    info->loop = loop;
    info->playTime = playTime;
    info->playScale = playScale;
    info->reverse = reverse;
    info->frameTime = playTime / sequenece->GetFrameCount();

    if (m_mapAnimation.empty())
        m_CurrentAnimation = info;

    m_mapAnimation.insert(std::make_pair(sequenceName, info));
}

void CAnimation::SetPlayTime(const std::string& name, float playTime)
{
    AnimationInfo* info = FindInfo(name);

    if (!info)
        return;

    info->playTime = playTime;
}

void CAnimation::SetPlayScale(const std::string& name, float playScale)
{
    AnimationInfo* info = FindInfo(name);

    if (!info)
        return;

    info->playScale = playScale;
}

void CAnimation::SetCurrentAnimation(const std::string& name)
{
    m_CurrentAnimation = FindInfo(name);

    m_CurrentAnimation->frame = 0;
    m_CurrentAnimation->time = 0.0f;

    size_t size = m_CurrentAnimation->vecNotify.size();

    for (size_t i = 0; i < size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->call = false;
    }
}

void CAnimation::ChangeAnimation(const std::string& name)
{
    if (m_CurrentAnimation->sequence->GetName() == name)
        return;

    m_CurrentAnimation->frame = 0;
    m_CurrentAnimation->time = 0.0f;

    size_t size = m_CurrentAnimation->vecNotify.size();

    for (size_t i = 0; i < size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->call = false;
    }

    m_CurrentAnimation = FindInfo(name);
    m_CurrentAnimation->frame = 0;
    m_CurrentAnimation->time = 0.0f;
}

bool CAnimation::CheckCurrentAnimation(const std::string& name)
{
    return m_CurrentAnimation->sequence->GetName() == name;
}

void CAnimation::SetReverse(const std::string& name, bool reverse)
{
    AnimationInfo* info = FindInfo(name);

    if (!info)
        return;

    info->reverse = reverse;
}

void CAnimation::SetLoop(const std::string& name, bool loop)
{
    AnimationInfo* info = FindInfo(name);

    if (!info)
        return;

    info->loop = loop;
}

void CAnimation::Update(float deltaTime)
{
    m_CurrentAnimation->time += deltaTime * m_CurrentAnimation->playScale;

    bool animEnd = false;

    if (m_CurrentAnimation->time >= m_CurrentAnimation->frameTime)
    {
        m_CurrentAnimation->time -= m_CurrentAnimation->frameTime;

        if (m_CurrentAnimation->reverse)
        {
            m_CurrentAnimation->frame--;

            if (m_CurrentAnimation->frame < 0)
                animEnd = true;
        }
        else
        {
            m_CurrentAnimation->frame++;

            if (m_CurrentAnimation->frame == m_CurrentAnimation->sequence->GetFrameCount())
                animEnd = true;
        }
    }

    size_t size = m_CurrentAnimation->vecNotify.size();

    for (size_t i = 0; i < size; i++)
    {
        if (!m_CurrentAnimation->vecNotify[i]->call &&
            m_CurrentAnimation->vecNotify[i]->frame == m_CurrentAnimation->frame)
        {
            m_CurrentAnimation->vecNotify[i]->call = true;

            m_CurrentAnimation->vecNotify[i]->function();
        }
    }

    if (animEnd)
    {
        if (m_CurrentAnimation->loop)
        {
            if (m_CurrentAnimation->reverse)
                m_CurrentAnimation->frame = m_CurrentAnimation->sequence->GetFrameCount() - 1;
            else
                m_CurrentAnimation->frame = 0;
        }

        else
        {
            if (m_CurrentAnimation->reverse)
                m_CurrentAnimation->frame = 0;
            else
                m_CurrentAnimation->frame = m_CurrentAnimation->sequence->GetFrameCount() - 1;
        }

        if (m_CurrentAnimation->endFucntion)
            m_CurrentAnimation->endFucntion();

        if (m_CurrentAnimation->loop)
        {
            size_t size = m_CurrentAnimation->vecNotify.size();

            for (size_t i = 0; i < size; i++)
            {
                m_CurrentAnimation->vecNotify[i]->call = false;
            }
        }
    }
}

CAnimation* CAnimation::Clone()
{
    return new CAnimation(*this);
}

AnimationInfo* CAnimation::FindInfo(const std::string& name)
{
    auto iter = m_mapAnimation.find(name);

    if (iter == m_mapAnimation.end())
        return nullptr;

    return iter->second;
}
