#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Material/Material.h"

// ������ ���� ���� �޽��� ������ ȭ�鿡 ��µ� �� ������Ʈ
class CStaticMeshComponent :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CStaticMeshComponent();
	CStaticMeshComponent(const CStaticMeshComponent& com);
	virtual ~CStaticMeshComponent();

protected:
	CSharedPtr<CStaticMesh> m_Mesh;
	// 3D�޽��� ���� ���׸����� ���� �� �ִ�.
	std::vector<CSharedPtr<CMaterial>> m_vecMaterialSlot;

public:
	inline CMaterial* GetMaterial(int index = 0) const
	{
		return m_vecMaterialSlot[index];
	}

public:
	// �޽� �Ŵ����� �̸��� �����ؼ� �޽��� �����ϴ� �Լ�
	void SetMesh(const std::string& name);
	// �޽� ��ü�� �����ؼ� �޽��� �����ϴ� �Լ�
	void SetMesh(CStaticMesh* mesh);
	// ���׸����� �ٷ� �Ѱܼ� �����ϴ� �Լ�
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
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CStaticMeshComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

