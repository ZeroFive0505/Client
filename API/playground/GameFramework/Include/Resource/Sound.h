#pragma once

#include "../Ref.h"

/// <summary>
/// FMOD �ý����� ��ϵǴ� ����
/// </summary>
class CSound :
	public CRef
{
	friend class CResourceManager;

private:
	CSound();
	~CSound();

private:
	// FMOD�� �����ڵ带 �̿������ʰ� ��Ƽ ����Ʈ ���ڿ��� �̿��Ѵ�.
	std::string m_Name;
	FMOD::System* m_System;
	// ���� ����
	FMOD::Sound* m_Sound;
	// ��� ä�� �׷쿡 �����ִ���
	FMOD::ChannelGroup* m_Group;
	// ���� ä������
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
	// ���带 �ҷ��´�.
	bool LoadSound(FMOD::System* System, FMOD::ChannelGroup* Group, bool Loop, const std::string& Name, const char* FileName,
		const std::string PathName = SOUND_PATH);
	// ���� ���
	void Play();
	// ���� ����
	void Stop();
	// ���� �Ͻ� ����
	void Pause();
	// ���� ��� �簳
	void Resume();
};

