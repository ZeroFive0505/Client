#pragma once

#include "../GameInfo.h"

// �˸� �Լ� ����ü
struct sAnimation2DNotify
{
	std::string name;
	int frame;
	bool call;
	std::function<void()> callBack;

	sAnimation2DNotify() :
		frame(0),
		call(false)
	{
	}
};

/// <summary>
/// �ִϸ��̼� �����͸� ��� Ŭ����
/// </summary>
class CAnimationSequence2DData
{
	friend class CAnimationSequence2DInstance;

private:
	CAnimationSequence2DData();
	~CAnimationSequence2DData();

private:
	// �ִϸ��̼��� �̸�
	std::string m_Name;
	std::string m_SequenceName;
	// �ִϸ��̼� ������
	CSharedPtr<class CAnimationSequence2D> m_Sequence;
	// ���� ������
	int m_Frame;
	// ����ð�
	float m_Time;
	// �����Ӵ� �ð�
	float m_FrameTime;
	// �� ����ð�
	float m_PlayTime;
	// ��� ����
	float m_PlayScale;
	// �ݺ�
	bool m_Loop;
	// �����
	bool m_Reverse;
	// �������� �ִϸ��̼��� �����Ұ�����
	bool m_IsMenual;

	bool m_Start;

	std::function<void()> m_EndFunction;
	std::function<void()> m_StartFunction;
	std::vector<sAnimation2DNotify*> m_vecNotify;

public:
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	void SetCurrentFrame(int frame);
	
public:
	inline const std::string& GetName() const
	{
		return m_Name;
	}

	inline int GetCurrentFrame() const
	{
		return m_Frame;
	}

	class CAnimationSequence2D* GetAnimationSequence() const
	{
		return m_Sequence;
	}

	inline bool IsLoop() const
	{
		return m_Loop;
	}

	inline bool IsReverse() const
	{
		return m_Reverse;
	}

	inline float GetPlayScale() const
	{
		return m_PlayScale;
	}

	inline float GetPlayTime() const
	{
		return m_PlayTime;
	}

public:
	template <typename T>
	void SetEndFunction(T* obj, void(T::* func)())
	{
		m_EndFunction = std::bind(func, obj);
	}

	template <typename T>
	void SetStartFucntion(T* obj, void(T::* func)())
	{
		m_StartFunction = std::bind(func, obj);
	}

	template <typename T>
	void AddNotify(const std::string& name, int frame, T* obj, void(T::* func)())
	{
		sAnimation2DNotify* notify = new sAnimation2DNotify;

		notify->name = name;
		notify->frame = frame;
		notify->callBack = std::bind(func, obj);

		m_vecNotify.push_back(notify);
	}
};

