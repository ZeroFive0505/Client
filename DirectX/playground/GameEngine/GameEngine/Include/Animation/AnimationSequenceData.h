#pragma once

#include "../GameInfo.h"

struct AnimationNotify
{
	std::string name;
	int frame;
	bool call;

	std::function<void()> function;

	AnimationNotify() :
		frame(0),
		call(false)
	{}
};

class CAnimationSequenceData
{
	friend class CAnimationSequenceInstance;

private:
	CAnimationSequenceData();
	~CAnimationSequenceData();

private:
	std::string m_Name;
	std::string m_SequenceName;
	CSharedPtr<class CAnimationSequence> m_Sequence;

	float m_Time;	// �ִϸ��̼� ���۽ð�
	float m_FrameTime;	// 1�����Ӵ� �ð�
	float m_PlayTime;
	float m_PlayScale;
	bool m_Loop;
	std::function<void()> m_EndFunction;
	std::vector<AnimationNotify*> m_vecNotify;

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	const std::string& GetName() const
	{
		return m_Name;
	}

	float GetAnimationTime() const
	{
		return m_Time;
	}

	class CAnimationSequence* GetAnimationSequence() const
	{
		return m_Sequence;
	}

public:
	template <typename T>
	void SetEndFunction(T* obj, void(T::* func)())
	{
		m_EndFunction = std::bind(func, obj);
	}

	template <typename T>
	void AddNotify(const std::string& name, int frame, T* obj, void(T::* func)())
	{
		AnimationNotify* notify = new AnimationNotify;

		notify->name = name;
		notify->frame = frame;
		notify->function = std::bind(func, obj);
	}
};

