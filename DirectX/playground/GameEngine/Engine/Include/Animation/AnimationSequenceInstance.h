#pragma once

#include "AnimationSequenceData.h"

class CAnimationSequenceInstance
{
public:
	CAnimationSequenceInstance();
	CAnimationSequenceInstance(const CAnimationSequenceInstance& anim);
	virtual ~CAnimationSequenceInstance();

protected:
	size_t m_TypeID;
	class CAnimationMeshComponent* m_Owner;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationSequenceData*> m_mapAnimation;
	CAnimationSequenceData* m_CurrentAnimation;
	CAnimationSequenceData* m_ChangeAnimation;
	bool m_PlayAnimation;

	CSharedPtr<class CComputeShader> m_AnimationUpdateShader;
	class CAnimationUpdateConstantBuffer* m_AnimationUpdateCBuffer;
	class CStructuredBuffer* m_OutputBuffer; // 애니메이션 결과 저장용 버퍼
	class CStructuredBuffer* m_BoneBuffer;
	ID3D11Buffer* m_BoneDataBuffer;
	std::vector<Matrix> m_vecBoneMatrix;
	CSharedPtr<class CSkeleton> m_Skeleton;

	Vector3 m_BlendScale;
	Vector3 m_BlendPos;
	Vector4 m_BlendRot;
	
	float m_GlobalTime;
	float m_SequenceProgress;
	float m_ChangeTimeAcc;
	float m_ChangeTime;

public:
	inline size_t GetTypeID() const
	{
		return m_TypeID;
	}

	inline int GetAnimationCount() const
	{
		return (int)m_mapAnimation.size();
	}

public:
	template <typename T>
	void SetTypeID()
	{
		m_TypeID = typeid(T).hash_code();
	}

	void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

	void SetOwner(class CAnimationMeshComponent* owner)
	{
		m_Owner = owner;
	}

	void SetSkeleton(class CSkeleton* skeleton);

public:
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

	inline CAnimationSequenceData* GetCurrentAnimation() const
	{
		return m_CurrentAnimation;
	}

public:
	void AddAnimation(const std::string& sequenceName, const std::string& name, bool loop = true, float playTime = 1.0f,
		float playScale = 1.0f);
	void AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool loop = true, float playTime = 1.0f,
		float playScale = 1.0f);
	void SetPlayTime(const std::string& name, float playTime);
	void SetPlayScale(const std::string& name, float playScale);
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
	virtual CAnimationSequenceInstance* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	CAnimationSequenceData* FindAnimation(const std::string& name);

public:
	template <typename T>
	void SetEndFunction(const std::string& name, T* obj, void(T::* func)())
	{
		CAnimationSequenceData* data = FindAnimation(name);

		if (!data)
			return;

		data->SetEndFunction<T>(obj, func);
	}

	template <typename T>
	void AddNotify(const std::string& name, const std::string& notifyName, int frame, T* obj, void(T::* func)())
	{
		CAnimationSequenceData* data = FindAnimation(name);

		if (!data)
			return;

		data->AddNotify<T>(notifyName, frame, obj, func);
	}
};

