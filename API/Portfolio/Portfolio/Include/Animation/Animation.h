#pragma once

#include "AnimationInfo.h"

enum class EPlayerState
{
	GO_RIGHT,
	GO_LEFT,
	GO_FORWARD,
	GO_BACK,
	FALLING,
	HIT
};

class CAnimation
{
	friend class CGameObject;

private:
	CAnimation();
	CAnimation(const CAnimation& anim);
	~CAnimation();

private:
	class CGameObject* m_Owner;
	class CScene* m_Scene;
	std::unordered_map<std::string, AnimationInfo*> m_mapAnimation;
	AnimationInfo* m_CurrentAnimation;
	EPlayerState m_State;
	bool m_Drifting = false;

public:
	void AddAnimation(const std::string& sequenceName, bool loop = true, float playTime = 1.0f, float playScale = 1.0f,
		bool reverse = false);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);
	void SetReverse(const std::string& name, bool reverse);
	void SetLoop(const std::string& name, bool loop);
	void Update(float deltaTime);
	CAnimation* Clone();

private:
	AnimationInfo* FindInfo(const std::string& name);

public:
	template <typename T>
	void SetEndNotify(const std::string& sequenceName, T* obj, void(T::* func)())
	{
		AnimationInfo* info = FindInfo(sequenceName);

		if (!info)
			return;

		info->endFucntion = std::bind(func, obj);
	}

	template <typename T>
	void AddNotify(const std::string& sequenceName, int frame, T* obj, void(T::* func)())
	{
		AnimationInfo* info = FindInfo(sequenceName);

		if (!info)
			return;

		AnimationNotify* notify = new AnimationNotify;

		notify->frame = frame;
		notify->function = std::bind(func, obj);

		info->vecNotify.push_back(notify);
	}
};

