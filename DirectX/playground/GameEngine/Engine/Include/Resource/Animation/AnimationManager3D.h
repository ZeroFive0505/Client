#pragma once

#include "AnimationSequence.h"
#include "Skeleton.h"

class CAnimationManager3D
{
	friend class CResourceManager;

private:
	CAnimationManager3D();
	~CAnimationManager3D();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence>> m_mapSequence;
	std::unordered_map<std::string, CSharedPtr<CSkeleton>> m_mapSkeleton;

public:
	bool Init();
	bool LoadAnimationSequence(const std::string& name, bool loop,
		struct _tagFbxAnimationClip* clip, class CScene* scene = nullptr);
	bool LoadAnimationSequence(const std::string& name, bool loop,
		int startFrame, int endFrame, float playTime, const std::vector<BoneKeyFrame*>& vecFrame, class CScene* scene = nullptr);
	bool LoadAnimationSequence(bool loop, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadAnimationSequenceFullPath(bool loop, const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadAnimationSequenceMultibyte(bool loop, const std::string& name, const char* fileName, const std::string& pathName = ANIMATION_PATH,
		class CScene* scene = nullptr);
	bool LoadAnimationSequenceFullPathMultibyte(bool loop, const std::string& name, const char* fullPath, class CScene* scene = nullptr);
	CAnimationSequence* FindAnimationSequence(const std::string& name);
	void ReleaseSequence(const std::string& name);

	bool LoadSkeleton(const std::string& name, const TCHAR* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSkeletonFullPath(const std::string& name, const TCHAR* fullPath, class CScene* scene = nullptr);
	bool LoadSkeletonMulitybyte(const std::string& name, const char* fileName,
		const std::string& pathName = ANIMATION_PATH, class CScene* scene = nullptr);
	bool LoadSkeletonFullPathMultibyte(const std::string& name, const char* fullPath,
		class CScene* scene = nullptr);

	CSkeleton* FindSkeleton(const std::string& name);
	void ReleaseSkeleton(const std::string& name);
};

