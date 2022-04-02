#pragma once

#include "../Resource/Texture.h"
#include "../Resource/AnimationSequence.h"
#include "../Resource/Sound.h"

class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapAnimationSequence;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

public:
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<std::wstring>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool ReleaseTexture(const std::string& name);
	void SetTextureColorKey(const std::string& name, const unsigned char r, const unsigned char g, const unsigned char b, int index = 0);

	
	CTexture* FindSpriteFont(const std::string& name);
	CTexture* FindTexture(const std::string& name);

public:
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName);
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& sequenceName, const std::string& textureName,
		const std::vector<std::wstring>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& sequenceName, const Vector2& startPos, const Vector2& size);
	void AddAnimationFrameData(const std::string& sequenceName, float posX, float posY, float sizeX, float sizeY);

	CAnimationSequence* FindAnimationSequence(const std::string& name);

public:
	bool LoadSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);
	bool Load3DSound(const std::string& groupName, bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = SOUND_PATH);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);
	bool IsPlaying(const std::string& name);

	void UpdateEngineSound(const std::string& name, bool IsPlayer, const Vector2& pos, const float height, const float speedForward, const float speedTurn);

	void UpdateListener(const Vector2& pos, float angle, float height);

	void PlayEngine(const std::string& name, bool IsPlayer);

	void StopAllMusic();
	void StopAllSFX();


	CSound* FindSound(const std::string& name);
};

