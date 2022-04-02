#pragma once

#include "../Resource/Texture.h"
#include "../Animation/AnimationInfo.h"
#include "../Resource/AnimationSequence.h"
#include "../Resource/Sound.h"

/// <summary>
/// 장면 리소스는 장면에 속해있는 리소스들을 관리한다.
/// 만약 리소스를 추가하고싶다면 씬 리소스를 통해서 추가해야한다.
/// 각각의 씬들은 자기만의 씬 리소스를 가지고 있을 수 있다.
/// </summary>
class CSceneResource
{
	// 씬은 씬 리소스를 생성할 수 있어야한다. 따라서 프렌드 선언
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	// 씬에 속해있는 텍스쳐를 가지고있는다.
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	// 씬에 있는 애니메이션 시퀀스에 관한 정보를 가지고있는다.
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapAnimationSequence;
	// 씬에 있는 사운드 정보를 가지고있는다.
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

public:
	//  텍스쳐 유틸리티 함수
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index = 0);

	CTexture* FindTexture(const std::string& Name);

public:
	// 애니메이션 유틸리티 함수
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
	bool CreateAnimationSequence(const std::string& SequenceName, const std::string& TextureName,
		const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void AddAnimationFrameData(const std::string& SequenceName, const Vector2& StartPos, const Vector2& Size);
	void AddAnimationFrameData(const std::string& SequenceName, float PosX, float PosY, float SizeX, float SizeY);

	CAnimationSequence* FindAnimationSequence(const std::string& Name);

public:
	// 사운드 관련 유틸리티 함수들
	bool LoadSound(const std::string& GroupName, bool Loop, const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);
	bool SetVolume(int Volume);
	bool SetVolume(const std::string& GroupName, int Volume);
	bool SoundPlay(const std::string& Name);
	bool SoundStop(const std::string& Name);
	bool SoundPause(const std::string& Name);
	bool SoundResume(const std::string& Name);

	CSound* FindSound(const std::string& Name);
};

