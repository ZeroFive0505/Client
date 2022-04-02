#pragma once

#include "AnimationSequence2D.h"

// 애니메이션 매니저는 시퀀스의 이름으로 다양한 애니메이션을 가지고 있는 클래스
class CAnimationManager
{
	friend class CResourceManager;

private:
	CAnimationManager();
	~CAnimationManager();

private:
	std::unordered_map<std::string, CSharedPtr<CAnimationSequence2D>> m_mapSequence2D;
	class CAnimation2DConstantBuffer* m_Animation2DCBuffer;


public:
	class CAnimation2DConstantBuffer* GetAnimation2DCBuffer() const
	{
		return m_Animation2DCBuffer;
	}

public:
	bool Init();
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2D(const std::string& name, class CTexture* texture);
	bool CreateAnimationSequence2D(const std::string& name, const std::string& textureName, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);
	bool CreateAnimationSequence2DFullPath(const std::string& name, const std::string& textureName, 
		const std::vector<TCHAR*>& vecFullPath);
	void AddFrame(const std::string& name, const Vector2& start, const Vector2& size);
	void AddFrame(const std::string& name, float startX, float startY, float width, float height);
	bool SaveSequenceFullPath(const std::string& name, const char* fullPath);
	bool LoadSequenceFullPath(std::string& resultName, const char* fullPath, class CScene* scene = nullptr);

	bool SaveSequence(const std::string& name, const char* fileName, const std::string& pathName);
	bool LoadSequence(std::string& resultName, const char* fileName, const std::string& pathName, class CScene* scene = nullptr);

	CAnimationSequence2D* FindSequence(const std::string& name);
	void ReleaseSequence(const std::string& name);
};

