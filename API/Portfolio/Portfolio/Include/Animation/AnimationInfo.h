#pragma once

#include "../Resource/AnimationSequence.h"

struct AnimationNotify
{
	int frame;
	bool call;
	std::function<void()> function;

	AnimationNotify() : frame(0), call(false)
	{

	}
};

struct AnimationInfo
{
	CSharedPtr<CAnimationSequence> sequence;
	int frame;
	float time;
	float frameTime;
	float playTime;
	float playScale;
	bool loop;
	bool reverse;

	std::function<void()> endFucntion;
	std::vector<AnimationNotify*> vecNotify;

	AnimationInfo() :
		frame(0),
		time(0.0f),
		frameTime(0.0f),
		playTime(0.0f),
		playScale(1.0f),
		loop(false),
		reverse(false)
	{

	}

	~AnimationInfo()
	{
		size_t size = vecNotify.size();

		for (size_t i = 0; i < size; i++)
		{
			SAFE_DELETE(vecNotify[i]);
		}
	}
};