#pragma once

#include "../Ref.h"

class CSound :
	public CRef
{
	friend class CResourceManager;

private:
	CSound();
	~CSound();

private:
	std::string m_Name;
	FMOD::System* m_System;
	FMOD::Sound* m_Sound;
	FMOD::ChannelGroup* m_Group;

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
	bool LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const std::string& name, const char* fileName,
		const std::string PathName = SOUND_PATH);
	bool Load3DSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const std::string& name, const char* fileName,
		const std::string PathName = SOUND_PATH);

	void Play();

	void Stop();

	void Pause();

	void Resume();
};

