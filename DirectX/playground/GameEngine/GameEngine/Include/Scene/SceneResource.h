#pragma once

#include "../GameInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Particle/Particle.h"

/// <summary>
/// �� ���ҽ��� ���ؼ� ���ҽ��Ŵ����� �߰踦 �޾�
/// �� ���ҽ� �Ŵ����� �����Ѵ�.
/// </summary>
class CSceneResource
{
	friend class CScene;

private:
	CSceneResource();
	~CSceneResource();

private:
	class CScene* m_Scene;

private:
	std::unordered_map<std::string, CSharedPtr<CMesh>> m_mapMesh;
	std::unordered_map<std::string, CSharedPtr<CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<CMaterial>> m_mapMaterial;
	std::unordered_map<std::string, CSharedPtr<CTexture>> m_mapTexture;
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CParticle>> m_mapParticle;

public: // =========================== Mesh ===========================
	CMesh* FindMesh(const std::string& name);

public: // =========================== Shader ===========================
	CShader* FindShader(const std::string& name);

	template <typename T>
	bool CreateShader(const std::string& name)
	{
		if (FindShader(name))
			return true;

		CResourceManager::GetInst()->CreateShader<T>(name);

		m_mapShader.insert(std::make_pair(name, CResourceManager::GetInst()->FindShader(name)));

		return true;
	}

public: // =========================== Shader ===========================
	CMaterial* FindMaterial(const std::string& name);

	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		if (FindMaterial(name))
			return true;

		CResourceManager::GetInst()->CreateMaterial<T>(name);

		m_mapMaterial.insert(std::make_pair(name, CResourceManager::GetInst()->FindMaterial(name)));

		return true;
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		return CResourceManager::GetInst()->CreateMaterialEmpty<T>();
	}

public: // =========================== Texture ===========================
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);
	class CTexture* FindTexture(const std::string& name);

public:  // =========================== Animation Sequence2D ===========================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFullPath);
	void AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float width, float height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;

public:
	bool SaveSequence2DFullPath(const std::string& name, const char* fullPath);
	bool LoadSequence2DFullPath(const char* fullPath);
	bool LoadSequence2DFullPath(std::string& resultName, const char* fullPath);

	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName = ANIMATION_PATH);
	bool LoadSequence2D(const char* fileName, const std::string& pathName = ANIMATION_PATH);
	bool LoadSequence2D(std::string& resultName, const char* fileName, const std::string& pathName = ANIMATION_PATH);

public: // =========================== Sound ===========================
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

public: // =========================== Particle ===========================
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
};

