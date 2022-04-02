#pragma once

#include "Sound.h"

class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();


private:
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

public:
	bool Init();
	void Update();

	bool LoadSound(const std::string& channelGroupName, bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& name);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& channelGroupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);

	class CSound* FindSound(const std::string& name);
	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	void ReleaseSound(const std::string& name);
};

