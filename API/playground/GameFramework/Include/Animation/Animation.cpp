#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation() : m_CurrentAnimation(nullptr)
{
}

// �ִϸ��̼��� ���� ���� ȣ���Ѵ�.
CAnimation::CAnimation(const CAnimation& Anim)
{
    // ��� �ִϸ��̼��� ��ȸ�ϸ鼭 �����Ѵ�.
    std::unordered_map<std::string, AnimationInfo*>::const_iterator iter = Anim.m_mapAnimation.begin();
    std::unordered_map<std::string, AnimationInfo*>::const_iterator iterEnd = Anim.m_mapAnimation.end();

    for (; iter != iterEnd; iter++)
    {
        AnimationInfo* Info = new AnimationInfo;

        Info->Sequence = iter->second->Sequence;
        Info->FrameTime = iter->second->FrameTime;
        Info->PlayTime = iter->second->PlayTime;
        Info->PlayScale = iter->second->PlayScale;
        Info->Loop = iter->second->Loop;
        Info->Reverse = iter->second->Reverse;

        if (Anim.m_CurrentAnimation->Sequence->GetName() == Info->Sequence->GetName())
            m_CurrentAnimation = Info;

        m_mapAnimation.insert(std::make_pair(iter->first, Info));
    }
}

// �����ÿ��� ��� �ִϸ��̼��� ��ȸ�ϸ鼭 �����Ѵ�.
CAnimation::~CAnimation()
{
    auto iter = m_mapAnimation.begin();
    auto iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; iter++)
    {
        SAFE_DELETE(iter->second);
    }
}

// �ִϸ��̼��� �߰��Ѵ�.
void CAnimation::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
    // ���� �� ���ҽ����� �ִϸ��̼��� ã�´�.
    CAnimationSequence* Sequence = m_Scene->GetSceneResource()->FindAnimationSequence(SequenceName);

    // ������ ��ȯ
    if (!Sequence)
        return;

    // ���ο� �ִϸ��̼��� �����.
    AnimationInfo* Info = new AnimationInfo;

    Info->Sequence = Sequence;
    // ��������
    Info->Loop = Loop;
    // ����ð�
    Info->PlayTime = PlayTime;
    // ������
    Info->PlayScale = PlayScale;
    // �����
    Info->Reverse = Reverse;
    // �� �����Ӵ� ��� �ð�
    Info->FrameTime = PlayTime / Sequence->GetFrameCount();

    // ó�� �߰��Ǵ� �ִϸ��̼��� ��� �� �ִϸ��̼��� �⺻ �ִϸ��̼����� ����
    if (m_mapAnimation.empty())
        m_CurrentAnimation = Info;

    m_mapAnimation.insert(std::make_pair(SequenceName, Info));
}

// ��� �ð��� �����Ѵ�.
void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->PlayTime = PlayTime;
}

// ��� ������ �����Ѵ�.
void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->PlayScale = PlayScale;
}

// ���� �ִϸ��̼��� �����Ѵ�.
void CAnimation::SetCurrentAnimation(const std::string& Name)
{
    m_CurrentAnimation = FindInfo(Name);

    // �ִϸ��̼��� ���������� �����Ӱ� �ð��� �ʱ�ȭ��Ų��.
    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

    size_t Size = m_CurrentAnimation->vecNotify.size();

    // �ݹ��Լ��� ���� �θ����� ���ٰ� �ʱ�ȭ
    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->Call = false;
    }
}

// �ִϸ��̼��� �ٲ۴�.
void CAnimation::ChangeAnimation(const std::string& Name)
{
    // ���� �̹� ����ǰ� �ִ� �ִϸ��̼����� �����Ѵٸ� �׳� ��ȯ.
    if (m_CurrentAnimation->Sequence->GetName() == Name)
        return;

    // �ʱ�ȭ
    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

    size_t Size = m_CurrentAnimation->vecNotify.size();

    // ���� �ʱ�ȭ.
    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->Call = false;
    }

    // ��ü
    m_CurrentAnimation = FindInfo(Name);

    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
    return m_CurrentAnimation->Sequence->GetName() == Name;
}

// ����� ����
void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->Reverse = Reverse;
}

// �ݺ� ����
void CAnimation::SetLoop(const std::string& Name, bool Loop)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->Loop = Loop;
}

// �ִϸ��̼��� ������Ʈ
void CAnimation::Update(float DeltaTime)
{
    // �ð��� ������Ʈ
    m_CurrentAnimation->Time += DeltaTime * m_CurrentAnimation->PlayScale;

      
    bool AnimEnd = false;

    // ���� �����ð��� ���� �ִϸ��̼��� ����ð��� �Ѿ��ٸ�
    if (m_CurrentAnimation->Time >= m_CurrentAnimation->FrameTime)
    {
        // ���ҽ�Ų��.
        m_CurrentAnimation->Time -= m_CurrentAnimation->FrameTime;

        // ������� �ÿ���
        if (m_CurrentAnimation->Reverse)
        {
            // �������� ���ҽ�Ų��.
            m_CurrentAnimation->Frame--;

            // ���� 0���� �۴ٸ� ���� ����
            if (m_CurrentAnimation < 0)
                AnimEnd = true;
        }
        // �������Ͻÿ���
        else
        {
            // �������� ������Ų��.
            m_CurrentAnimation->Frame++;

            // ���� �� �����Ӽ����� ���ų� ũ�ٸ�
            // ��
            if (m_CurrentAnimation->Frame >= m_CurrentAnimation->Sequence->GetFrameCount())
                AnimEnd = true;
        }
    }

    size_t Size = m_CurrentAnimation->vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        // �ݹ��Լ��� ȣ������� ���� ���� ���� ��ϵ� �������� ���� �����Ӱ� ���ٸ� �ݹ��Լ��� ȣ���Ѵ�.
        if (!m_CurrentAnimation->vecNotify[i]->Call && 
            m_CurrentAnimation->vecNotify[i]->Frame == m_CurrentAnimation->Frame)
        {
            m_CurrentAnimation->vecNotify[i]->Call = true;
            m_CurrentAnimation->vecNotify[i]->Function();
        }
    }

    if (AnimEnd)
    {
        // ������� �Ųٷ�
         if (m_CurrentAnimation->Reverse)
            m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
        else
            m_CurrentAnimation->Frame = 0;


        // ���� �ִϸ��̼��� �������� ȣ��� �Լ��� �ִٸ�
        if (m_CurrentAnimation->EndFunction)
            m_CurrentAnimation->EndFunction();

        // �ִϸ��̼��� �ݺ��Ǿ���Ѵٸ� �ݹ��Լ��� ���� �ʱ�ȭ
        if (m_CurrentAnimation->Loop)
        {
            size_t Size = m_CurrentAnimation->vecNotify.size();

            for (size_t i = 0; i < Size; i++)
            {
                m_CurrentAnimation->vecNotify[i]->Call = false;
            }
        }

    }
}

CAnimation* CAnimation::Clone()
{
    return new CAnimation(*this);
}

AnimationInfo* CAnimation::FindInfo(const std::string& Name)
{
    auto iter = m_mapAnimation.find(Name);

    if (iter == m_mapAnimation.end())
        return nullptr;

    return iter->second;
}
