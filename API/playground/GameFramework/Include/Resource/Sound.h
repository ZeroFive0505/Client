#pragma once

#include "../Ref.h"

/// <summary>
/// FMOD 시스템의 등록되는 사운드
/// </summary>
class CSound :
	public CRef
{
	friend class CResourceManager;

private:
	CSound();
	~CSound();

private:
	// FMOD는 유니코드를 이용하지않고 멀티 바이트 문자열을 이용한다.
	std::string m_Name;
	FMOD::System* m_System;
	// 사운드 파일
	FMOD::Sound* m_Sound;
	// 어느 채널 그룹에 속해있는지
	FMOD::ChannelGroup* m_Group;
	// 무슨 채널인지
	FMOD::Channel* m_Channel;

	bool m_Play;
	bool m_Loop;

public:
	bool IsPlay() const
	{
		return m_Play;
	}

	bool IsLoop() const
	{
		return m_Loop;
	}

public:
	// 사운드를 불러온다.
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const std::string& Name, const char* FileName,
		const std::string PathName = SOUND_PATH);
	// 사운드 재생
	void Play();
	// 사운드 정지
	void Stop();
	// 사운드 일시 정지
	void Pause();
	// 사운드 재생 재개
	void Resume();
};

