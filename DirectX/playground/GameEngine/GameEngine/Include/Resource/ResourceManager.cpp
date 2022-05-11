#include "ResourceManager.h"


DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	m_MeshManager(nullptr),
	m_ShaderManager(nullptr),
	m_MaterialManager(nullptr),
	m_TextureManger(nullptr),
	m_AnimationManager(nullptr),
	m_SoundManager(nullptr),
	m_FontManager(nullptr),
	m_ParticleManager(nullptr),
	m_AnimationManager3D(nullptr)
{

}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(m_ParticleManager);
	SAFE_DELETE(m_FontManager);
	SAFE_DELETE(m_SoundManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_MaterialManager);
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MeshManager);
	SAFE_DELETE(m_TextureManger);
	SAFE_DELETE(m_AnimationManager3D);
}

bool CResourceManager::Init()
{
	// 리소스 매니저를 초기화하면서
	// 메쉬 매니저와 쉐이더 메니저도 같이 초기화 시킨다.
	m_MeshManager = new CMeshManager;

	if (!m_MeshManager->Init())
		return false;

	m_ShaderManager = new CShaderManager;

	if (!m_ShaderManager->Init())
		return false;

	m_TextureManger = new CTextureManager;

	if (!m_TextureManger->Init())
		return false;

	m_MaterialManager = new CMaterialManager;

	if (!m_MaterialManager->Init())
		return false;

	m_AnimationManager = new CAnimationManager;

	if (!m_AnimationManager->Init())
		return false;

	m_SoundManager = new CSoundManager;

	if (!m_SoundManager->Init())
		return false;

	m_FontManager = new CFontManager;

	if (!m_FontManager->Init())
		return false;

	m_ParticleManager = new CParticleManager;

	if (!m_ParticleManager->Init())
		return false;

	m_AnimationManager3D = new CAnimationManager3D;

	if (!m_AnimationManager3D->Init())
		return false;

	return true;
}

void CResourceManager::Update()
{
	m_SoundManager->Update();
}

CMesh* CResourceManager::FindMesh(const std::string& name)
{
	return m_MeshManager->FindMesh(name);
}

bool CResourceManager::CreateMesh(Mesh_Type type, const std::string& name, void* vtxData, int size, int count, D3D11_USAGE usage, D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData, int idxSize, int idxCount, D3D11_USAGE idxUsage, DXGI_FORMAT fmt, CScene* scene)
{
	return m_MeshManager->CreateMesh(type, name, vtxData, size, count, usage, primitive, idxData, idxSize, idxCount, idxUsage,
		fmt, scene);
}

bool CResourceManager::LoadMesh(Mesh_Type type, const std::string& name, const TCHAR* fileName, const std::string& pathName,
	class CScene* scene)
{
	return m_MeshManager->LoadMesh(type, name, fileName, pathName, scene);
}

bool CResourceManager::LoadMeshFullPath(Mesh_Type type, const std::string& name, const TCHAR* fullPath,
	class CScene* scene)
{
	return m_MeshManager->LoadMeshFullPath(type, name, fullPath, scene);
}

bool CResourceManager::LoadMeshMultibyte(Mesh_Type type, const std::string& name, const char* fileName, const std::string& pathName,
	class CScene* scene)
{
	return m_MeshManager->LoadMeshMultibyte(type, name, fileName, pathName, scene);
}

bool CResourceManager::LoadMeshFullPathMultibyte(Mesh_Type type, const std::string& name, const char* fullPath,
	class CScene* scene )
{
	return m_MeshManager->LoadMeshFullPathMultibyte(type, name, fullPath, scene);
}

bool CResourceManager::SetMeshSkeleton(const std::string& name, const std::string& skeletonName)
{
	CSkeleton* skeleton = FindSkeleton(skeletonName);

	if (!skeleton)
		return false;

	return SetMeshSkeleton(name, skeleton);
}

bool CResourceManager::SetMeshSkeleton(const std::string& name, CSkeleton* skeleton)
{
	return m_MeshManager->SetMeshSkeleton(name, skeleton);
}

void CResourceManager::ReleaseMesh(const std::string& name)
{
	m_MeshManager->ReleaseMesh(name);
}

CShader* CResourceManager::FindShader(const std::string& name)
{
	return m_ShaderManager->FindShader(name);
}

void CResourceManager::ReleaseShader(const std::string& name)
{
	m_ShaderManager->ReleaseShader(name);
}

bool CResourceManager::CreateConstantBuffer(const std::string& name, int size, int registerNum, int constantBufferShaderType)
{
	return m_ShaderManager->CreateConstantBuffer(name, size, registerNum, constantBufferShaderType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& name)
{
	return m_ShaderManager->FindConstantBuffer(name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& name)
{
	return m_MaterialManager->FindMaterial(name);
}

void CResourceManager::ReleaseMaterial(const std::string& name)
{
	m_MaterialManager->ReleaseMaterial(name);
}

bool CResourceManager::LoadTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return m_TextureManger->LoadTexture(name, fileName, pathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const TCHAR* fullPath)
{
	return m_TextureManger->LoadTextureFullPath(name, fullPath);
}

bool CResourceManager::LoadTexture(const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	return m_TextureManger->LoadTexture(name, vecFileName, pathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& name, const std::vector<TCHAR*>& vecFullPath)
{
	return m_TextureManger->LoadTextureFullPath(name, vecFullPath);
}

CTexture* CResourceManager::FindTexture(const std::string& name)
{
	return m_TextureManger->FindTexture(name);
}

void CResourceManager::ReleaseTexture(const std::string& name)
{
	m_TextureManger->ReleaseTexture(name);
}

bool CResourceManager::CreateTarget(const std::string& name, unsigned int width, unsigned int height, DXGI_FORMAT pixelFormat)
{
	return m_TextureManger->CreateTarget(name, width, height, pixelFormat);
}

void CResourceManager::RenderTarget()
{
	CMesh* mesh = m_MeshManager->FindMesh("WidgetMesh");
	CShader* shader = m_ShaderManager->FindShader("RenderTargetShader");

	m_TextureManger->RenderTarget(mesh, shader);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName, const std::string& pathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, textureName, fileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string name, CTexture* texture)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, texture);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFileName, const std::string& pathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(name, textureName, vecFileName, pathName);
}

bool CResourceManager::CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFullPath)
{
	return m_AnimationManager->CreateAnimationSequence2DFullPath(name, textureName, vecFullPath);
}

void CResourceManager::AddAnimationSequence2DFrame(const std::string& name, const Vector2& start, const Vector2& size)
{
	m_AnimationManager->AddFrame(name, start, size);
}

void CResourceManager::AddAnimationSequence2DFrame(const std::string& name, float startX, float startY, float width, float height)
{
	m_AnimationManager->AddFrame(name, startX, startY, width, height);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& name)
{
	return m_AnimationManager->FindSequence(name);
}

void CResourceManager::ReleaseAnimationSequnce2D(const std::string& name)
{
	m_AnimationManager->ReleaseSequence(name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnimation2DCBuffer() const
{
	return m_AnimationManager->GetAnimation2DCBuffer();
}

bool CResourceManager::SaveSequence2DFullPath(const std::string& name, const char* fullPath)
{
	return m_AnimationManager->SaveSequenceFullPath(name, fullPath);
}

bool CResourceManager::LoadSequence2DFullPath(std::string& resultName, const char* fullPath, CScene* scene)
{
	return m_AnimationManager->LoadSequenceFullPath(resultName, fullPath, scene);
}

bool CResourceManager::LoadSequence2DFullPath(const char* fullPath, CScene* scene)
{
	std::string name;
	return m_AnimationManager->LoadSequenceFullPath(name, fullPath, scene);
}

bool CResourceManager::SaveSequence2D(const std::string& name, const char* fileName, const std::string& pathName)
{
	return m_AnimationManager->SaveSequence(name, fileName, pathName);
}

bool CResourceManager::LoadSequence2D(const char* fileName, const std::string& pathName, CScene* scene)
{
	std::string name;
	return m_AnimationManager->LoadSequence(name, fileName, pathName, scene);
}

bool CResourceManager::LoadSequence2D(std::string& resultName, const char* fileName, const std::string& pathName, CScene* scene)
{
	return m_AnimationManager->LoadSequence(resultName, fileName, pathName, scene);
}

bool CResourceManager::LoadSound(const std::string& channelGroupName, bool loop, const std::string& name, const char* fileName, const std::string& pathName)
{
	return m_SoundManager->LoadSound(channelGroupName, loop, name, fileName, pathName);
}

bool CResourceManager::CreateSoundChannelGroup(const std::string& name)
{
	return m_SoundManager->CreateSoundChannelGroup(name);
}

bool CResourceManager::SetVolume(int volume)
{
	return m_SoundManager->SetVolume(volume);
}

bool CResourceManager::SetVolume(const std::string& channelGroupName, int volume)
{
	return m_SoundManager->SetVolume(channelGroupName, volume);
}

bool CResourceManager::SoundPlay(const std::string& name)
{
	return m_SoundManager->SoundPlay(name);
}

bool CResourceManager::SoundStop(const std::string& name)
{
	return m_SoundManager->SoundStop(name);
}

bool CResourceManager::SoundPause(const std::string& name)
{
	return m_SoundManager->SoundPause(name);
}

bool CResourceManager::SoundResume(const std::string& name)
{
	return m_SoundManager->SoundResume(name);
}

CSound* CResourceManager::FindSound(const std::string& name)
{
	return m_SoundManager->FindSound(name);
}

FMOD::ChannelGroup* CResourceManager::FindChannelGroup(const std::string& name)
{
	return m_SoundManager->FindChannelGroup(name);
}

void CResourceManager::ReleaseSound(const std::string& name)
{
	m_SoundManager->ReleaseSound(name);
}

bool CResourceManager::CreateFontFile(const std::string& name, const TCHAR* fileName, const std::string& pathName)
{
	return m_FontManager->CreateFontFile(name, fileName, pathName);
}

bool CResourceManager::LoadFont(const std::string& name, const TCHAR* fontName, int weight, float fontSize, const TCHAR* localName, int stretch)
{
	return m_FontManager->LoadFont(name, fontName, weight, fontSize, localName, stretch);
}

const TCHAR* CResourceManager::GetFontFaceName(const std::string& name)
{
	return m_FontManager->GetFontFaceName(name);
}

const char* CResourceManager::GetFontFaceNameMultibyte(const std::string& name)
{
	return m_FontManager->GetFontFaceNameMultibyte(name);
}

bool CResourceManager::CreateFontColor(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColor(r, g, b, a);
}

bool CResourceManager::CreateFontColor(const Vector4& color)
{
	return m_FontManager->CreateFontColor(color);
}

bool CResourceManager::CreateFontColor(unsigned int color)
{
	return m_FontManager->CreateFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(float r, float g, float b, float a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->FindFontColor(r, g, b, a);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(const Vector4& color)
{
	return m_FontManager->FindFontColor(color);
}

ID2D1SolidColorBrush* CResourceManager::FindFontColor(unsigned int color)
{
	return m_FontManager->FindFontColor(color);
}

unsigned int CResourceManager::CreateFontColorKey(float r, float g, float b, float a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return m_FontManager->CreateFontColorKey(r, g, b, a);
}

unsigned int CResourceManager::CreateFontColorKey(const Vector4& color)
{
	return m_FontManager->CreateFontColorKey(color);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* text, IDWriteTextFormat* font, float width, float height)
{
	return m_FontManager->CreateTextLayout(text, font, width, height);
}

IDWriteTextLayout* CResourceManager::CreateTextLayout(const TCHAR* text, const std::string& fontName, float width, float height)
{
	return m_FontManager->CreateTextLayout(text, fontName, width, height);
}

IDWriteFontCollection1* CResourceManager::FindFontFile(const std::string& name)
{
	return m_FontManager->FindFontFile(name);
}

IDWriteTextFormat* CResourceManager::FindFont(const std::string& name)
{
	return m_FontManager->FindFont(name);
}

bool CResourceManager::CreateParticle(const std::string& name)
{
	return m_ParticleManager->CreateParticle(name);;
}

CParticle* CResourceManager::FindParticle(const std::string& name)
{
	return m_ParticleManager->FindParticle(name);
}

void CResourceManager::ReleaseParticle(const std::string& name)
{
	m_ParticleManager->ReleaseParticle(name);
}

bool CResourceManager::LoadAnimationSequence(const std::string& name, bool loop, _tagFbxAnimationClip* clip, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequence(name, loop, clip, scene);
}

bool CResourceManager::LoadAnimationSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime, const std::vector<BoneKeyFrame*>& vecFrame, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequence(name, loop, startFrame, endFrame, playTime, vecFrame, scene);
}

bool CResourceManager::LoadAnimationSequence(bool loop, const std::string& name, const TCHAR* fileName, const std::string& pathName, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequence(loop, name, fileName, pathName, scene);
}

bool CResourceManager::LoadAnimationSequenceFullPath(bool loop, const std::string& name, const TCHAR* fullPath, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequenceFullPath(loop, name, fullPath, scene);
}

bool CResourceManager::LoadAnimationSequenceMultibyte(bool loop, const std::string& name, const char* fileName, const std::string& pathName, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequenceMultibyte(loop, name, fileName, pathName, scene);
}

bool CResourceManager::LoadAnimationSequenceFullPathMultibyte(bool loop, const std::string& name, const char* fullPath, CScene* scene)
{
	return m_AnimationManager3D->LoadAnimationSequenceFullPathMultibyte(loop, name, fullPath, scene);
}

CAnimationSequence* CResourceManager::FindAnimationSequence(const std::string& name)
{
	return m_AnimationManager3D->FindAnimationSequence(name);
}

void CResourceManager::ReleaseAnimationSequence3D(const std::string& name)
{
	m_AnimationManager3D->ReleaseSequence(name);
}

bool CResourceManager::LoadSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName, CScene* scene)
{
	return m_AnimationManager3D->LoadSkeleton(name, fileName, pathName, scene);
}

bool CResourceManager::LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath, CScene* scene)
{
	return m_AnimationManager3D->LoadSkeletonFullPath(name, fullPath, scene);
}

bool CResourceManager::LoadSkeletonMulitybyte(const std::string& name, const char* fileName, const std::string& pathName, CScene* scene)
{
	return m_AnimationManager3D->LoadSkeletonMulitybyte(name, fileName, pathName, scene);
}

bool CResourceManager::LoadSkeletonFullPathMultibyte(const std::string& name, const char* fullPath, CScene* scene)
{
	return m_AnimationManager3D->LoadSkeletonFullPathMultibyte(name, fullPath, scene);
}

CSkeleton* CResourceManager::FindSkeleton(const std::string& name)
{
	return m_AnimationManager3D->FindSkeleton(name);
}

void CResourceManager::ReleaseSkeleton(const std::string& name)
{
	m_AnimationManager3D->ReleaseSkeleton(name);
}
