#pragma once

#include "AnimationInfo.h"

/// <summary>
/// �ִϸ��̼� Ŭ������ �ܺο��� ������ �Ұ����ϰ�
/// ���� ������Ʈ���� �� �ִϸ��̼��� ��� ������ ���� �� �ִ�.
/// ���� ���� ������Ʈ���� �ִϸ��̼� ������ �����ϴ�.
/// </summary>
class CAnimation
{
	friend class CGameObject;

private:
	CAnimation();
	CAnimation(const CAnimation& Anim);
	~CAnimation();

private:
	// �� �ִϸ��̼��� �������ִ� ���� ������Ʈ�� ��������.
	class CGameObject* m_Owner;
	// �� �ִϸ��̼��� �����ִ� ��
	class CScene* m_Scene;
	// string�� Ű�� AnimationInfo�� ��ȸ�Ѵ�.
	std::unordered_map<std::string, AnimationInfo*> m_mapAnimation;
	// ���� �ִϸ��̼�
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
	// �ִϸ��̼��� �������� ȣ��ȴ�.
	template <typename T>
	void SetEndNotify(const std::string& SequenceName, T* Obj, void(T::* Func)())
	{
		AnimationInfo* Info = FindInfo(SequenceName);

		if (!Info)
			return;

		Info->EndFunction = std::bind(Func, Obj);
	}

	// �� �����ӿ� ȣ����� �Լ��� ����� �ִ´�.
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

