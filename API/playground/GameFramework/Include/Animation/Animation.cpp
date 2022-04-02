#include "Animation.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"

CAnimation::CAnimation() : m_CurrentAnimation(nullptr)
{
}

// 애니메이션을 복사 생성 호출한다.
CAnimation::CAnimation(const CAnimation& Anim)
{
    // 모든 애니메이션을 순회하면서 복사한다.
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

// 삭제시에는 모든 애니메이션을 순회하면서 삭제한다.
CAnimation::~CAnimation()
{
    auto iter = m_mapAnimation.begin();
    auto iterEnd = m_mapAnimation.end();

    for (; iter != iterEnd; iter++)
    {
        SAFE_DELETE(iter->second);
    }
}

// 애니메이션을 추가한다.
void CAnimation::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
    // 현재 씬 리소스에서 애니메이션을 찾는다.
    CAnimationSequence* Sequence = m_Scene->GetSceneResource()->FindAnimationSequence(SequenceName);

    // 있으면 반환
    if (!Sequence)
        return;

    // 새로운 애니메이션을 만든다.
    AnimationInfo* Info = new AnimationInfo;

    Info->Sequence = Sequence;
    // 루프인지
    Info->Loop = Loop;
    // 재생시간
    Info->PlayTime = PlayTime;
    // 재생배속
    Info->PlayScale = PlayScale;
    // 역재생
    Info->Reverse = Reverse;
    // 한 프레임당 재생 시간
    Info->FrameTime = PlayTime / Sequence->GetFrameCount();

    // 처음 추가되는 애니메이션일 경우 이 애니메이션을 기본 애니메이션으로 설정
    if (m_mapAnimation.empty())
        m_CurrentAnimation = Info;

    m_mapAnimation.insert(std::make_pair(SequenceName, Info));
}

// 재생 시간을 설정한다.
void CAnimation::SetPlayTime(const std::string& Name, float PlayTime)
{
    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->PlayTime = PlayTime;
}

// 재생 배율을 설정한다.
void CAnimation::SetPlayScale(const std::string& Name, float PlayScale)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->PlayScale = PlayScale;
}

// 현재 애니메이션을 설정한다.
void CAnimation::SetCurrentAnimation(const std::string& Name)
{
    m_CurrentAnimation = FindInfo(Name);

    // 애니메이션을 변경했으니 프레임과 시간을 초기화시킨다.
    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

    size_t Size = m_CurrentAnimation->vecNotify.size();

    // 콜백함수는 전부 부른적이 없다고 초기화
    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->Call = false;
    }
}

// 애니메이션을 바꾼다.
void CAnimation::ChangeAnimation(const std::string& Name)
{
    // 만약 이미 재생되고 있는 애니메이션으로 설정한다면 그냥 반환.
    if (m_CurrentAnimation->Sequence->GetName() == Name)
        return;

    // 초기화
    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

    size_t Size = m_CurrentAnimation->vecNotify.size();

    // 전부 초기화.
    for (size_t i = 0; i < Size; i++)
    {
        m_CurrentAnimation->vecNotify[i]->Call = false;
    }

    // 교체
    m_CurrentAnimation = FindInfo(Name);

    m_CurrentAnimation->Frame = 0;
    m_CurrentAnimation->Time = 0.0f;

}

bool CAnimation::CheckCurrentAnimation(const std::string& Name)
{
    return m_CurrentAnimation->Sequence->GetName() == Name;
}

// 역재생 설정
void CAnimation::SetReverse(const std::string& Name, bool Reverse)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->Reverse = Reverse;
}

// 반복 설정
void CAnimation::SetLoop(const std::string& Name, bool Loop)
{

    AnimationInfo* Info = FindInfo(Name);

    if (!Info)
        return;

    Info->Loop = Loop;
}

// 애니메이션을 업데이트
void CAnimation::Update(float DeltaTime)
{
    // 시간을 업데이트
    m_CurrentAnimation->Time += DeltaTime * m_CurrentAnimation->PlayScale;

      
    bool AnimEnd = false;

    // 만약 누적시간이 현재 애니메이션의 재생시간을 넘었다면
    if (m_CurrentAnimation->Time >= m_CurrentAnimation->FrameTime)
    {
        // 감소시킨다.
        m_CurrentAnimation->Time -= m_CurrentAnimation->FrameTime;

        // 역재생일 시에는
        if (m_CurrentAnimation->Reverse)
        {
            // 프레임을 감소시킨다.
            m_CurrentAnimation->Frame--;

            // 만약 0보다 작다면 끝에 도달
            if (m_CurrentAnimation < 0)
                AnimEnd = true;
        }
        // 순방향일시에는
        else
        {
            // 프레임을 증가시킨다.
            m_CurrentAnimation->Frame++;

            // 만약 총 프레임수보다 같거나 크다면
            // 끝
            if (m_CurrentAnimation->Frame >= m_CurrentAnimation->Sequence->GetFrameCount())
                AnimEnd = true;
        }
    }

    size_t Size = m_CurrentAnimation->vecNotify.size();

    for (size_t i = 0; i < Size; i++)
    {
        // 콜백함수가 호출된적이 없고 만약 현재 등록된 프레임이 현재 프레임과 같다면 콜백함수를 호출한다.
        if (!m_CurrentAnimation->vecNotify[i]->Call && 
            m_CurrentAnimation->vecNotify[i]->Frame == m_CurrentAnimation->Frame)
        {
            m_CurrentAnimation->vecNotify[i]->Call = true;
            m_CurrentAnimation->vecNotify[i]->Function();
        }
    }

    if (AnimEnd)
    {
        // 역재생시 거꾸로
         if (m_CurrentAnimation->Reverse)
            m_CurrentAnimation->Frame = m_CurrentAnimation->Sequence->GetFrameCount() - 1;
        else
            m_CurrentAnimation->Frame = 0;


        // 만약 애니메이션이 끝났을시 호출될 함수가 있다면
        if (m_CurrentAnimation->EndFunction)
            m_CurrentAnimation->EndFunction();

        // 애니메이션이 반복되어야한다면 콜백함수도 전부 초기화
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
