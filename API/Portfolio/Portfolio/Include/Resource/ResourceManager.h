#pragma once

#include "Texture.h"
#include "AnimationSequence.h"
#include "Sound.h"
#include "Font.h"
#include "../GameInfo.h"
#include "../Map/MapInfo.h"

class CResourceManager
{
private:
	CResourceManager();
	~CResourceManager();

private:
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*> m_mapChannelGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

private:
	std::list<std::wstring> m_FontLoadList;
	std::unordered_map<std::string, CSharedPtr<CFont>> m_mapFont;

private:
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapSpriteFont;

private:
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapAnimationSequence;

public:
	bool Init();
	void Update();

public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<std::wstring> &vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTextureColorKey(const std::string& name, const unsigned char r, const unsigned char g, const unsigned char b, int index = 0);

	void ReleaseTexture(const std::string& name);

	CTexture* FindTexture(const std::string& name);
	CTexture* FindSpriteFont(const std::string& name);

public:
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName);
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName,
		const std::vector<std::wstring>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& sequenceName, const Vector2& startPos, const Vector2& size);
	void AddAnimationFrameData(const std::string& sequenceName, float posX, float posY, float sizeX, float sizeY);

	void ReleaseAnimationSequence(const std::string& name);

	CAnimationSequence* FindAnimationSequence(const std::string& name);

public:
	bool LoadSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);
	bool Load3DSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);
	bool CreateSoundChannelGroup(const std::string& name);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);

	bool IsPlaying(const std::string& name);

	void StopAllSFX();
	void StopAllMusic();

	void UpdateEngineSound(const std::string& name, bool IsPlayer, const Vector2& pos, const float height, const float speedForward,
		const float speedTurn);

	void UpdateListener(const Vector2& pos, float angle, float height);

	void PlayEngine(const std::string& name, bool IsPlayer);

	void ReleaseSound(const std::string& name);

	FMOD::ChannelGroup* FindSoundChannelGroup(const std::string& name);
	CSound* FindSound(const std::string& name);

public:
	bool LoadOtherFont(const TCHAR* fileName, const std::string& pathName = FONT_PATH);
	bool LoadFont(const std::string& name, const TCHAR* fontName, int width = 0, int height = 0);

	CFont* FindFont(const std::string& name);

private:
	static CResourceManager* m_Inst;

public:
	inline static CResourceManager* GetInst()
	{
		if (!m_Inst)
			m_Inst = new CResourceManager;

		return m_Inst;
	}

	inline static void DestroyInst()
	{
		SAFE_DELETE(m_Inst);
	}
};
