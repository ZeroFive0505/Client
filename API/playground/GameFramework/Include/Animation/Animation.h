#pragma once

#include "AnimationInfo.h"

/// <summary>
/// 애니메이션 클래스는 외부에서 생성이 불가능하고
/// 게임 오브젝트만이 이 애니메이션을 멤버 변수로 가질 수 있다.
/// 따라서 게임 오브젝트만이 애니메이션 생성이 가능하다.
/// </summary>
class CAnimation
{
	friend class CGameObject;

private:
	CAnimation();
	CAnimation(const CAnimation& Anim);
	~CAnimation();

private:
	// 이 애니메이션을 가지고있는 게임 오브젝트가 누구인지.
	class CGameObject* m_Owner;
	// 이 애니메이션이 속해있는 씬
	class CScene* m_Scene;
	// string을 키로 AnimationInfo를 조회한다.
	std::unordered_map<std::string, AnimationInfo*> m_mapAnimation;
	// 현재 애니메이션
	AnimationInfo* m_CurrentAnimation;

public:
	void AddAnimation(const std::string& SequenceName, 
		bool Loop = true, float PlayTime = 1.0f, float PlayScale = 1.0f,
		bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetCurrentAnimation(const std::string& Name);
	void ChangeAnimation(const std::string& Name);
	bool CheckCurrentAnimation(const std::string& Name);
	void SetReverse(const std::string& Name, bool Reverse);
	void SetLoop(const std::string& Name, bool Loop);
	void Update(float DeltaTime);
	CAnimation* Clone();

private:
	AnimationInfo* FindInfo(const std::string& Name);

public:
	// 애니메이션이 끝났을시 호출된다.
	template <typename T>
	void SetEndNotify(const std::string& SequenceName, T* Obj, void(T::* Func)())
	{
		AnimationInfo* Info = FindInfo(SequenceName);

		if (!Info)
			return;

		Info->EndFunction = std::bind(Func, Obj);
	}

	// 몇 프레임에 호출될지 함수를 만들어 넣는다.
	template <typename T>
	void AddNotify(const std::string& SequenceName, int Frame, T* Obj, void(T::* Func)())
	{
		AnimationInfo* Info = FindInfo(SequenceName);

		if (!Info)
			return;

		AnimationNotify* Notify = new AnimationNotify;

		Notify->Frame = Frame;
		Notify->Function = std::bind(Func, Obj);

		Info->vecNotify.push_back(Notify);
	}
};

