#pragma once

#include "Mesh/MeshManager.h"
#include "Shader/ShaderManager.h"
#include "Material/MaterialManager.h"
#include "Texture/TextureManager.h"
#include "Animation/AnimationManager.h"
#include "Animation/AnimationManager3D.h"
#include "Sound/SoundManager.h"
#include "Font/FontManager.h"
#include "Particle/ParticleManager.h"

/// <summary>
/// 리소스 매니저는 중계의 역활을 한다.
/// </summary>
class CResourceManager
{
private:
	CMeshManager* m_MeshManager;
	CShaderManager* m_ShaderManager;
	CMaterialManager* m_MaterialManager;
	CTextureManager* m_TextureManger;
	CAnimationManager* m_AnimationManager;
	CAnimationManager3D* m_AnimationManager3D;
	CSoundManager* m_SoundManager;
	CFontManager* m_FontManager;
	CParticleManager* m_ParticleManager;

public:
	bool Init();
	void Update();

public: // =========================== Mesh ===========================
	class CMesh* FindMesh(const std::string& name);

	bool CreateMesh(Mesh_Type type, const std::string& name, void* vtxData, int size, int count, D3D11_USAGE usage,
		D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN, class CScene* scene = nullptr);
	bool LoadMesh(Mesh_Type type, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = MESH_PATH, class CScene* scene = nullptr);
	bool LoadMeshFullPath(Mesh_Type type, const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadMeshMultibyte(Mesh_Type type, const std::string& name, const char* fileName,
		const std::string& pathName = MESH_PATH, class CScene* scene = nullptr);
	bool LoadMeshFullPathMultibyte(Mesh_Type type, const std::string& name, const char* fullPath, class CScene* scene = nullptr);
	bool SetMeshSkeleton(const std::string& name, const std::string& skeletonName);
	bool SetMeshSkeleton(const std::string& name, class CSkeleton* skeleton);


	void ReleaseMesh(const std::string& name);


public: // =========================== Shader ===========================
	template <typename T>
	bool CreateShader(const std::string& name)
	{
		return m_ShaderManager->CreateShader<T>(name);
	}

	class CShader* FindShader(const std::string& name);

	void ReleaseShader(const std::string& name);

	bool CreateConstantBuffer(const std::string& name, int size, int
		registerNum, int constantBufferShaderType = (int)Buffer_Shader_Type::All);

	class CConstantBuffer* FindConstantBuffer(const std::string& name);

public: // =========================== Material ===========================
	CMaterial* FindMaterial(const std::string& name);

	void ReleaseMaterial(const std::string& name);

public:
	template <typename T>
	bool CreateMaterial(const std::string& name)
	{
		return m_MaterialManager->CreateMaterial<T>(name);
	}

	template <typename T>
	T* CreateMaterialEmpty()
	{
		return m_MaterialManager->CreateMaterialEmpty<T>();
	}


public: // =========================== Texture ===========================
	bool LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);

	bool LoadTextureFullPath(const std::string& name, const TCHAR* fullPath);

	bool LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);

	bool LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath);

	class CTexture* FindTexture(const std::string& name);

	void ReleaseTexture(const std::string& name);

public: // =========================== Render Target ===========================
	bool CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat);
	void RenderTarget();

public: // =========================== Animation Sequence2D ===========================
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string name, class CTexture* texture);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName,
		const std::vector<TCHAR*>& vecFullPath);
	void AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float width, float height);
	CAnimationSequence2D* FindAnimationSequence2D(const std::string& name);
	void ReleaseAnimationSequnce2D(const std::string& name);
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const;

public:
	bool SaveSequence2DFullPath(const std::string& name, const char* fullPath);
	bool LoadSequence2DFullPath(std::string& resultName, const char* fullPath, class CScene* scene = nullptr);
	bool LoadSequence2DFullPath(const char* fullPath, class CScene* scene = nullptr);

	bool SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName = ANIMATION_PATH);
	bool LoadSequence2D(const char* fileName, const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSequence2D(std::string& resultName, const char* fileName, const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);


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
	void ReleaseSound(const std::string& name);

public:  // =========================== Font ===========================
	bool CreateFontFile(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = FONT_PATH);

	bool LoadFont(const std::string& name, const TCHAR* fontName, int weight, float fontSize,
		const TCHAR* localName, int stretch = DWRITE_FONT_STRETCH_NORMAL);

	const TCHAR* GetFontFaceName(const std::string& name);
	const char* GetFontFaceNameMultibyte(const std::string& name);

	bool CreateFontColor(float r, float g, float b, float a);
	bool CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	bool CreateFontColor(const Vector4& color);
	bool CreateFontColor(unsigned int color);

	ID2D1SolidColorBrush* FindFontColor(float r, float g, float b, float a);
	ID2D1SolidColorBrush* FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	ID2D1SolidColorBrush* FindFontColor(const Vector4& color);
	ID2D1SolidColorBrush* FindFontColor(unsigned int color);

	unsigned int CreateFontColorKey(float r, float g, float b, float a);
	unsigned int CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
	unsigned int CreateFontColorKey(const Vector4& color);

	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, float width, float height);
	IDWriteTextLayout* CreateTextLayout(const TCHAR* text, const std::string& fontName, float width, float height);

	IDWriteFontCollection1* FindFontFile(const std::string& name);
	IDWriteTextFormat* FindFont(const std::string& name);

	
public: // =========================== Particle ===========================
	bool CreateParticle(const std::string& name);
	CParticle* FindParticle(const std::string& name);
	void ReleaseParticle(const std::string& name);

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
	void ReleaseAnimationSequence3D(const std::string& name);

	bool LoadSkeleton(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadSkeletonMulitybyte(const std::string& name, const char* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSkeletonFullPathMultibyte(const std::string& name, const char* fullPath,
		class CScene* scene = nullptr);

	CSkeleton* FindSkeleton(const std::string& name);
	void ReleaseSkeleton(const std::string& name);

	DECLARE_SINGLE(CResourceManager)
};

