#pragma once

#include "../Resource/AnimationSequence.h"

// �ִϸ��̼��� �����Ӱ� �ݹ��Լ��� �������ִ� ����ü.
struct AnimationNotify
{
	// �� ����������
	int Frame;
	// �ݹ��Լ��� ȣ������
	bool Call;

	// �ݹ��Լ�
	std::function<void()> Function;

	AnimationNotify() :
		Frame(0),
		Call(false)
	{

	}
};


// �ִϸ��̼��� ������ ��� ����ü
struct AnimationInfo
{
	// �ִϸ��̼�
	CSharedPtr<CAnimationSequence> Sequence;
	int Frame;
	float Time;
	float FrameTime;
	float PlayTime;
	float PlayScale;
	bool Loop;
	bool Reverse;
	// �ִϸ��̼��� �������� �Ҹ��� �Լ�
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