#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"

/// <summary>
/// ��������Ʈ ������Ʈ�� �⺻������ ����ϱ� ���� �޽��� ������ ������
/// �� ������Ʈ�̴�.
/// </summary>
class CSpriteComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CSpriteComponent();
	CSpriteComponent(const CSpriteComponent& com);
	virtual ~CSpriteComponent();

protected:
	// �޽�
	CSharedPtr<CSpriteMesh> m_Mesh;
	// �޽��� ����
	CSharedPtr<CMaterial> m_Material;
	// �ִϸ��̼�
	class CAnimationSequence2DInstance* m_Animation;

public:
	inline CMaterial* GetMaterial() const
	{
		return m_Material;
	}

	inline class CAnimationSequence2DInstance* GetAnimationInstance() const
	{
		return m_Animation;
	}

	inline Vector4 GetBaseColor() const
	{
		return m_Material->GetBaseColor();
	}

public:
	inline void SetMaterial(CMaterial* material)
	{
		m_Material = material->Clone();

		m_Material->SetScene(m_Scene);
	}

public: // ��������Ʈ ������Ʈ���� ���׸��� �����ϱ����� ��ƿ��Ƽ �Լ���
	inline void SetBaseColor(const Vector4& color)
	{
		m_Material->SetBaseColor(color);
	}

	inline void SetBaseColor(float r, float g, float b, float a)
	{
		m_Material->SetBaseColor(r, g, b, a);
	}

	inline void SetRenderState(CRenderState* state)
	{
		m_Material->SetRenderState(state);
	}

	inline void SetRenderState(const std::string& name)
	{
		m_Material->SetRenderState(name);
	}

	inline void SetTransparency(bool enable)
	{
		m_Material->SetTransparency(enable);
	}

	inline void SetOpacity(float opacity)
	{
		m_Material->SetOpacity(opacity);
	}

	inline void AddOpacity(float opacity)
	{
		m_Material->AddOpacity(opacity);
	}

	inline void AddTexture(int registerNum, int shaderType, const std::string& name, CTexture* texture)
	{
		m_Material->AddTexture(registerNum, shaderType, name, texture);
	}

	inline void AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH)
	{
		m_Material->AddTexture(registerNum, shaderType, name, fileName, pathName);
	}

	inline void AddTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
	{
		m_Material->AddTextureFullPath(registerNum, shaderType, name, fullPath);
	}

	inline void AddTexture(int registerNum, int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileNmae, const std::string& pathName = TEXTURE_PATH)
	{
		m_Material->AddTexture(registerNum, shaderType, name, vecFileNmae, pathName);
	}

	inline void SetTexture(int index, int registerNum, int shaderType, const std::string& name, CTexture* texture)
	{
		m_Material->SetTexture(index, registerNum, shaderType, name, texture);
	}

	inline void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH)
	{
		m_Material->SetTexture(index, registerNum, shaderType, name, fileName, pathName);
	}

	inline void SetTextureFullPath(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath)
	{
		m_Material->SetTextureFullPath(index, registerNum, shaderType, name, fullPath);
	}

	inline void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName,
		const std::string& pathName = TEXTURE_PATH)
	{
		m_Material->SetTexture(index, registerNum, shaderType, name, vecFileName, pathName);
	}

	
public:
	void ChangeAnimation(const std::string& name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSpriteComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	// �ִϸ��̼��� �����ϴ� �Լ�
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
	}

	template <typename T>
	void LoadAnimationInstance()
	{
		T* anim = new T;

		anim->SetScene(m_Scene);
		anim->SetOwner(this);

		SAFE_DELETE(m_Animation);

		m_Animation = anim;
	}
};

