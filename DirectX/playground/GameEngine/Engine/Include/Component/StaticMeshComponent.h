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
	CSharedPtr<CMaterial> m_Material;

public:
	inline CMaterial* GetMaterial() const
	{
		return m_Material;
	}

public:
	// �޽� �Ŵ����� �̸��� �����ؼ� �޽��� �����ϴ� �Լ�
	void SetMesh(const std::string& name);
	// �޽� ��ü�� �����ؼ� �޽��� �����ϴ� �Լ�
	void SetMesh(CStaticMesh* mesh);
	// ���׸����� �ٷ� �Ѱܼ� �����ϴ� �Լ�
	void SetMaterial(CMaterial* material);

public:
	void SetBaseColor(const Vector4& color);
	void SetBaseColor(float r, float g, float b, float a);
	void SetRenderState(class CRenderState* state);
	void SetRenderState(const std::string& name);
	void SetTransparency(bool enable);
	void SetOpacity(float opacity);
	void AddOpacity(float opacity);

public:
	void AddTexture(int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void AddTexture(int registerNum, int shaderType, const std::string& name,
		const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName,
		const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(int index, int registerNum, 
		int shaderType, const std::string& name, const TCHAR* fullPath);
	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName,
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

