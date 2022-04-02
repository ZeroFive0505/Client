#pragma once

#include "../Resource/Texture.h"
#include "../Animation/AnimationInfo.h"
#include "../Resource/AnimationSequence.h"
#include "../Resource/Sound.h"

/// <summary>
/// ��� ���ҽ��� ��鿡 �����ִ� ���ҽ����� �����Ѵ�.
/// ���� ���ҽ��� �߰��ϰ��ʹٸ� �� ���ҽ��� ���ؼ� �߰��ؾ��Ѵ�.
/// ������ ������ �ڱ⸸�� �� ���ҽ��� ������ ���� �� �ִ�.
/// </summary>
class CSceneResource
{
	// ���� �� ���ҽ��� ������ �� �־���Ѵ�. ���� ������ ����
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	// ���� �����ִ� �ؽ��ĸ� �������ִ´�.
	std::unordered_map<std::string, CSharedPtr<CTexture>>	m_mapTexture;
	// ���� �ִ� �ִϸ��̼� �������� ���� ������ �������ִ´�.
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapAnimationSequence;
	// ���� �ִ� ���� ������ �������ִ´�.
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;

public:
	//  �ؽ��� ��ƿ��Ƽ �Լ�
	bool LoadTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	bool LoadTexture(const std::string& Name, const std::vector<std::wstring>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTextureColorKey(const std::string& Name, const unsigned char r, const unsigned char g, const unsigned char b, int Index = 0);
	void SetTextureColorKey(const std::string& Name, unsigned int ColorKey, int Index = 0);

	CTexture* FindTexture(const std::string& Name);

public:
	// �ִϸ��̼� ��ƿ��Ƽ �Լ�
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
	// ���� ���� ��ƿ��Ƽ �Լ���
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
