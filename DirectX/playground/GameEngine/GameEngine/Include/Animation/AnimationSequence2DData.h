#pragma once

#include "../GameInfo.h"

// 알림 함수 구조체
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
/// 애니메이션 데이터를 담는 클래스
/// </summary>
class CAnimationSequence2DData
{
	friend class CAnimationSequence2DInstance;

private:
	CAnimationSequence2DData();
	~CAnimationSequence2DData();

private:
	// 애니메이션의 이름
	std::string m_Name;
	std::string m_SequenceName;
	// 애니메이션 시퀀스
	CSharedPtr<class CAnimationSequence2D> m_Sequence;
	// 현재 프레임
	int m_Frame;
	// 재생시간
	float m_Time;
	// 프레임당 시간
	float m_FrameTime;
	// 총 재생시간
	float m_PlayTime;
	// 재생 배율
	float m_PlayScale;
	// 반복
	bool m_Loop;
	// 역재생
	bool m_Reverse;
	// 수동으로 애니메이션을 조절할것인지
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

