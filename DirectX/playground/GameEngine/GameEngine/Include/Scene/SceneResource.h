#pragma once

#include "../GameInfo.h"
#include "../Resource/Mesh/Mesh.h"
#include "../Resource/Shader/Shader.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Texture/Texture.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Resource/Animation/AnimationSequence.h"
#include "../Resource/ResourceManager.h"
#include "../Resource/Sound/Sound.h"
#include "../Resource/Particle/Particle.h"
#include "../Resource/Animation/Skeleton.h"

/// <summary>
/// 씬 리소스를 통해서 리소스매니저의 중계를 받아
/// 각 리소스 매니저에 접근한다.
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
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapSequence;
	std::unordered_map<std::string, CSharedPtr<CSound>> m_mapSound;
	std::unordered_map<std::string, CSharedPtr<CParticle>> m_mapParticle;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>> m_mapSkeleton;

public: // =========================== Mesh ===========================
	bool CreateMesh(Mesh_Type type, const std::string& name, void* vtxData, int size, int count,
		D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(Mesh_Type type, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = MESH_PATH);
	bool LoadMeshFullPath(Mesh_Type type, const std::string& name, const TCHAR* fullPath);
	bool LoadMeshMultibyte(Mesh_Type type, const std::string& name, const char* fileName,
		const std::string& pathName = MESH_PATH);
	bool LoadMeshFullPathMultibyte(Mesh_Type type, const std::string& name, const char* fullPath);
	bool SetMeshSkeleton(const std::string& name, const std::string& skeletonName);
	bool SetMeshSkeleton(const std::string& name, class CSkeleton* skeleton);

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

public:
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat);

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

public: // =========================== Animation3D ===========================
	bool LoadAnimationSequence(const std::string& name, bool loop,
		struct _tagFbxAnimationClip* clip, class CScene* scene = nullptr);
	bool LoadAnimationSequence(const std::string& name, bool loop,
		int startFrame, int endFrame, float playTime,
		const std::vector<BoneKeyFrame*>& vecFrame, class CScene* scene = nullptr);
	bool LoadAnimationSequence(bool loop, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadAnimationSequenceFullPath(bool loop, const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadAnimationSequenceMultibyte(bool loop, const std::string& name, const char* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadAnimationSequenceFullPathMultibyte(bool loop, const std::string& name, const char* fullPath, class CScene* scene = nullptr);
	CAnimationSequence* FindAnimationSequence(const std::string& name);


	bool LoadSkeleton(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = ANIMATION_PATH);
	bool LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath);
	bool LoadSkeletonMultibyte(const std::string& name, const char* fileName,
		const std::string& pathName);
	bool LoadSkeletonFullPathMultibyte(const std::string& name, const char* fullPath);
	CSkeleton* FindSkeleton(const std::string& name);
};

