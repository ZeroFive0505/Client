#pragma once

#include "AnimationSequence2DData.h"

/// <summary>
/// 실제 애니메이션을 관리하는 클래스
/// </summary>
class CAnimationSequence2DInstance 
{
public:
	CAnimationSequence2DInstance();
	CAnimationSequence2DInstance(const CAnimationSequence2DInstance& anim);
	virtual ~CAnimationSequence2DInstance();

protected:
	size_t m_TypeID;
	class CSpriteComponent* m_Owner;
	class CScene* m_Scene;
	// 문자열을 키로 애니메이션 데이터를 가지고 있는다.
	std::unordered_map<std::string, CAnimationSequence2DData*> m_mapAnimation;
	// 현재 애니메이션
	CAnimationSequence2DData* m_CurrentAnimation;
	// 애니메이션 재생여부를 상수버퍼에 담아서 쉐이더에 전달한다.
	class CAnimation2DConstantBuffer* m_CBuffer;

	bool m_PlayAnimation;

	bool m_HorizontalFlip;

public:
	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	inline size_t GetTypeID() const
	{
		return m_TypeID;
	}

	inline void SetHorizontalFlip(bool flip)
	{
		m_HorizontalFlip = flip;
	}

	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

	inline void SetOwner(class CSpriteComponent* owner)
	{
		m_Owner = owner;
	}

	inline int GetAnimationCount() const
	{
		return (int)m_mapAnimation.size();
	}

	inline void Play()
	{
		m_PlayAnimation = true;
	}

	inline void Stop()
	{
		m_PlayAnimation = false;
	}

	inline bool IsPlay() const
	{
		return m_PlayAnimation;
	}

	CAnimationSequence2DData* GetCurrentAnimation() const
	{
		return m_CurrentAnimation;
	}

	inline const std::unordered_map<std::string, CAnimationSequence2DData*>& GetMapAnim() const
	{
		return m_mapAnimation;
	}

public:
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool loop = true, float playTime = 1.0f,
		float playScale = 1.0f, bool reverse = false, bool isMenual = false);
	// 미리 저장되어있는 애니메이션의 파일이름으로 초기화한다.
	void AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool loop = true, float playTime = 1.0f, 
		float playScale = 1.0f, bool reverse = false, bool isMenual = false);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
	void SetReverse(const std::string& name, bool reverse);
	void SetLoop(const std::string& name, bool loop);
	void SetCurrentAnimation(const std::string& name);
	void ChangeAnimation(const std::string& name);
	bool CheckCurrentAnimation(const std::string& name);


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	void SetShader();
	void ResetShader();
	CAnimationSequence2DInstance* Clone();

public:
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void Save(const char* fullPath);
	virtual void Load(const char* fullPath);

private:
	CAnimationSequence2DData* FindAnimation(const std::string& name);

public:
	template <typename T>
	void SetEndFunction(const std::string& name, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = FindAnimation(name);

		if (!data)
			return;

		data->SetEndFunction<T>(obj, func);
	}

	template <typename T>
	void SetStartFunction(const std::string& name, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = FindAnimation(name);

		if (!data)
			return;

		data->SetStartFucntion<T>(obj, func);
	}

	template <typename T>
	void AddNotify(const std::string& name, const std::string& notifyName, int frame, T* obj, void(T::* func)())
	{
		CAnimationSequence2DData* data = FindAnimation(name);

		if (!data)
			return;

		data->AddNotify<T>(notifyName, frame, obj, func);
	}
};

