#include "AnimationManager3D.h"
#include "../../PathManager.h"

CAnimationManager3D::CAnimationManager3D()
{
}

CAnimationManager3D::~CAnimationManager3D()
{
}

bool CAnimationManager3D::Init()
{
	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(const std::string& name, bool loop, _tagFbxAnimationClip* clip, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	if (!sequence->CreateSequence(loop, clip))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));
	
	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(const std::string& name, bool loop, int startFrame, int endFrame, float playTime, const std::vector<BoneKeyFrame*>& vecFrame, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	if (!sequence->CreateSequence(name, loop, startFrame, endFrame, playTime, vecFrame))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequence(bool loop, const std::string& name, const TCHAR* fileName, const std::string& pathName, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	if (!sequence->CreateSequence(loop, name, fileName, pathName))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceFullPath(bool loop, const std::string& name, const TCHAR* fullPath, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	if (!sequence->CreateSequence(fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->m_Loop = loop;
	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceMultibyte(bool loop, const std::string& name, const char* fileName, const std::string& pathName, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	char fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		strcpy_s(fullPath, info->pathMultiByte);

	strcat_s(fullPath, fileName);

	if (!sequence->CreateSequenceMultibyte(fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->m_Loop = loop;
	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));

	return true;
}

bool CAnimationManager3D::LoadAnimationSequenceFullPathMultibyte(bool loop, const std::string& name, const char* fullPath, CScene* scene)
{
	if (FindAnimationSequence(name))
		return true;

	CAnimationSequence* sequence = new CAnimationSequence;

	sequence->m_Scene = scene;

	if (!sequence->CreateSequenceMultibyte(fullPath))
	{
		SAFE_DELETE(sequence);
		return false;
	}

	sequence->m_Loop = loop;
	sequence->SetName(name);

	m_mapSequence.insert(std::make_pair(name, sequence));

	return true;
}

CAnimationSequence* CAnimationManager3D::FindAnimationSequence(const std::string& name)
{
	auto iter = m_mapSequence.find(name);

	if (iter == m_mapSequence.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager3D::ReleaseSequence(const std::string& name)
{
	auto iter = m_mapSequence.find(name);

	if (iter != m_mapSequence.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSequence.erase(iter);
	}
}

bool CAnimationManager3D::LoadSkeleton(const std::string& name, const TCHAR* fileName, const std::string& pathName, CScene* scene)
{
	TCHAR fullPath[MAX_PATH] = {};

	const sPathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		lstrcpy(fullPath, info->path);

	lstrcat(fullPath, fileName);

	return LoadSkeletonFullPath(name, fullPath, scene);
}

bool CAnimationManager3D::LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath, CScene* scene)
{
	char fullPathMultibyte[MAX_PATH] = {};

#if UNICODE

	int convertLength = WideCharToMultiByte(CP_ACP, 0, fullPath, -1, nullptr, 0, nullptr, nullptr);
	WideCharToMultiByte(CP_ACP, 0, fullPath, -1, fullPathMultibyte, convertLength, nullptr, nullptr);

#else

	strcpy_s(fullPathMultibyte, fullPath);

#endif // UNICODE

	return LoadSkeletonFullPathMultibyte(name, fullPathMultibyte, scene);
}

bool CAnimationManager3D::LoadSkeletonMulitybyte(const std::string& name, const char* fileName, const std::string& pathName, CScene* scene)
{
	if (FindSkeleton(name))
		return true;

	CSkeleton* skeleton = new CSkeleton;

	skeleton->m_Scene = scene;

	if (!skeleton->LoadSkeleton(scene, name, fileName, pathName))
	{
		SAFE_DELETE(skeleton);
		return false;
	}

	skeleton->SetName(name);

	m_mapSkeleton.insert(std::make_pair(name, skeleton));

	return true;
}

bool CAnimationManager3D::LoadSkeletonFullPathMultibyte(const std::string& name, const char* fullPath, CScene* scene)
{
	if (FindSkeleton(name))
		return true;

	CSkeleton* skeleton = new CSkeleton;

	skeleton->m_Scene = scene;

	if (!skeleton->LoadSkeletonFullPath(scene, name, fullPath))
	{
		SAFE_DELETE(skeleton);
		return false;
	}

	skeleton->SetName(name);

	m_mapSkeleton.insert(std::make_pair(name, skeleton));

	return true;
}

bool CAnimationManager3D::AddSocket(const std::string& skeletonName, const std::string& boneName, const std::string& socketName, const Vector3& offset, const Vector3& offsetRot)
{
	CSkeleton* skeleton = FindSkeleton(skeletonName);

	if (!skeleton)
		return false;

	skeleton->AddSocket(boneName, socketName, offset, offsetRot);

	return true;
}

CSkeleton* CAnimationManager3D::FindSkeleton(const std::string& name)
{
	auto iter = m_mapSkeleton.find(name);

	if (iter == m_mapSkeleton.end())
		return nullptr;

	return iter->second;
}

void CAnimationManager3D::ReleaseSkeleton(const std::string& name)
{
	auto iter = m_mapSkeleton.find(name);

	if (iter != m_mapSkeleton.end())
	{
		if (iter->second->GetRefCount() == 1)
			m_mapSkeleton.erase(iter);
	}
}
