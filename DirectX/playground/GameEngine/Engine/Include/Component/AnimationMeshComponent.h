#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/AnimationMesh.h"
#include "../Resource/Material/Material.h"
#include "../Resource/Animation/Skeleton.h"
#include "../Animation/AnimationSequenceInstance.h"


class CAnimationMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CAnimationMeshComponent();
	CAnimationMeshComponent(const CAnimationMeshComponent& com);
	virtual ~CAnimationMeshComponent();

protected:
	CSharedPtr<CAnimationMesh> m_Mesh;
	// 3D메쉬는 여러 마테리얼을 가질 수 있다.
	std::vector<CSharedPtr<CMaterial>> m_vecMaterialSlot;
	CSharedPtr<CSkeleton> m_Skeleton;
	class CAnimationSequenceInstance* m_Animation;

public:
	inline CMaterial* GetMaterial(int index = 0) const
	{
		return m_vecMaterialSlot[index];
	}

	inline class CAnimationSequenceInstance* GetAnimationInstance() const
	{
		return m_Animation;
	}

public:
	// 메쉬 매니저에 이름을 전달해서 메쉬를 결정하는 함수
	void SetMesh(const std::string& name);
	// 메쉬 자체를 전달해서 메쉬를 결정하는 함수
	void SetMesh(CAnimationMesh* mesh);
	// 마테리얼을 바로 넘겨서 적용하는 함수
	void SetMaterial(CMaterial* material, int index = 0);

	void AddMaterial(CMaterial* material);

	inline void SetPickable(bool val)
	{
		m_Pickable = val;
	}

public:
	void SetBaseColor(const Vector4& color, int index = 0);
	void SetBaseColor(float r, float g, float b, float a, int index = 0);
	void SetAmbientColor(const Vector4& color, int index = 0);
	void SetAmbientColor(float r, float g, float b, float a, int index = 0);
	void SetSpecularColor(const Vector4& color, int index = 0);
	void SetSpecularColor(float r, float g, float b, float a, int index = 0);
	void SetSpecularPower(float power, int index = 0);
	void SetEmissiveColor(const Vector4& color, int index = 0);
	void SetEmissiveColor(float r, float g, float b, float a, int index = 0);
	void SetRenderState(class CRenderState* state, int index = 0);
	void SetRenderState(const std::string& name, int index = 0);
	void SetTransparency(bool enable, int index = 0);
	void SetOpacity(float opacity, int index = 0);
	void AddOpacity(float opacity, int index = 0);

public:
	void AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTextureFullPath(int materialIndex, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void AddTexture(int materialIndex, int registerNum, int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

	void SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(int materialIndex, int index, int registerNum,
		int shaderType, const std::string& name, const TCHAR* fullPath);
	void SetTexture(int materialIndex, int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH);

public:
	virtual void AddChild(CSceneComponent* child, const std::string& socketName = "");
	virtual void AddChild(class CGameObject* child, const std::string& socketName = "");
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CAnimationMeshComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	template <typename T>
	void CreateAnimationInstance()
	{
		T* anim = new T;

		anim->SetScene(m_Scene);
		anim->SetOwner(this);

		if (!anim->Init())
		{
			SAFE_DELETE(anim);
			return;
		}

		SAFE_DELETE(m_Animation);

		m_Animation = anim;

		if (m_Skeleton)
			m_Animation->SetSkeleton(m_Skeleton);
	}

	template <typename T>
	void LoadAnimationInstance()
	{
		T* anim = new T;

		anim->SetScene(m_Scene);
		anim->SetOwner(this);

		SAFE_DELETE(m_Animation);

		m_Animation = anim;

		if (m_Skeleton)
			m_Animation->SetSkeleton(m_Skeleton);
	}
};

