#pragma once

#include "../Resource/AnimationSequence.h"

// 애니메이션의 프레임과 콜백함수를 가지고있는 구조체.
struct AnimationNotify
{
	// 몇 프레임인지
	int Frame;
	// 콜백함수가 호출됬는지
	bool Call;

	// 콜백함수
	std::function<void()> Function;

	AnimationNotify() :
		Frame(0),
		Call(false)
	{

	}
};


// 애니메이션의 정보를 담는 구조체
struct AnimationInfo
{
	// 애니메이션
	CSharedPtr<CAnimationSequence> Sequence;
	int Frame;
	float Time;
	float FrameTime;
	float PlayTime;
	float PlayScale;
	bool Loop;
	bool Reverse;
	// 애니메이션이 끝났을시 불리는 함수
	std::function<void()> EndFunction;
	std::vector<AnimationNotify*> vecNotify;

	AnimationInfo() :
		Frame(0),
		Time(0.0f),
		FrameTime(0.0f),
		PlayTime(0.0f),
		PlayScale(1.0f),
		Loop(false),
		Reverse(false)
	{}

	~AnimationInfo()
	{
		size_t Size = vecNotify.size();

		for (size_t i = 0; i < Size; i++)
		{
			SAFE_DELETE(vecNotify[i]);
		}
	}
};