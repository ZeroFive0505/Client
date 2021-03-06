#pragma once

#include "../../Ref.h"

class CSound :
	public CRef
{
	friend class CSoundManager;

private:
	CSound();
	~CSound();

private:
	FMOD::System* m_System;
	FMOD::Sound* m_Sound;
	FMOD::ChannelGroup* m_Group;
	FMOD::Channel* m_Channel;
	bool m_Play;
	bool m_Loop;

public:
	inline bool IsPlay() const
	{
		return m_Play;
	}

	inline bool IsLoop() const
	{
		return m_Loop;
	}

public:
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* group, bool loop,
		const std::string& name, const char* fileName, const std::string& pathName = SOUND_PATH);
	void Play();
	void Stop();
	void Pause();
	void Resume();
};

