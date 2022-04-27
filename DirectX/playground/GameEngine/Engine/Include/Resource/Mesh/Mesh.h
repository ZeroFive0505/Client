#pragma once

#include "../../Ref.h"
#include "../Material/Material.h"

/// <summary>
/// �޽� �⺻ Ŭ����
/// </summary>
class CMesh :
	public CRef
{
	friend class CMeshManager;

protected:
	CMesh();
	virtual ~CMesh() = 0;

protected:
	std::vector<sMeshContainer*> m_vecContainer;
	std::vector<sMeshSlot*> m_vecMeshSlot;
	std::vector<CSharedPtr<CMaterial>> m_vecMaterialSlot;
	class CScene* m_Scene;

	// ���� �޽��� x, y, z �ּ����� ����
	Vector3 m_Min;
	// ���� �޽��� x, y, z �ִ����� ����
	Vector3 m_Max;

	Mesh_Type m_MeshType;

public:
	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

public:
	inline void AddMaterialSlot(CMaterial* material)
	{
		m_vecMaterialSlot.push_back(material);
	}

	void AddMaterialSlot(const std::string& name);

public:
	inline const std::vector<CSharedPtr<CMaterial>>* GetMaterialSlots() const
	{
		return &m_vecMaterialSlot;
	}

public:
	// ���ؽ��� �ε��� ���۸� �����ؼ� �޽��� ������ �Լ�
	virtual bool CreateMesh(void* vtxData, int size, int count, D3D11_USAGE usage,
		D3D11_PRIMITIVE_TOPOLOGY primitive, void* idxData = nullptr, int idxSize = 0, int idxCount = 0,
		D3D11_USAGE idxUsage = D3D11_USAGE_DEFAULT, DXGI_FORMAT fmt = DXGI_FORMAT_UNKNOWN);
	bool LoadMesh(const TCHAR* fileName, const std::string& pathName = MESH_PATH);
	bool LoadMeshFullPath(const TCHAR* fullPath);
	bool LoadMeshMultibyte(const char* fileName, const std::string& pathName = MESH_PATH);
	virtual bool LoadMeshFullPathMultibyte(const char* fullPath);
	virtual bool Init();
	virtual void Render();
	virtual void Render(int slotNumber);

	// �ν��Ͻ��� �̿��� ������
	virtual void RenderInstancing(int count);
	virtual void RenderInstancing(ID3D11Buffer* instancingBuffer, unsigned int instanceSize, int count);

protected:
	// ���۸� �����Ѵ�.
	// �����ʹ� void* �� �޾Ƽ� ��� �����͵� ���� �� �ְ� �Ѵ�.
	bool CreateBuffer(Buffer_Type type, void* data, int size, int count, D3D11_USAGE usage, ID3D11Buffer** buffer);


public:
	virtual bool SaveMeshFile(const char* fullPath);
	virtual bool LoadMeshFile(const char* fullPath);
	// FBX������ �о�鿩 ��ü �������� �����Ѵ�.
	virtual bool ConvertFBX(class CFBXLoader* loader, const char* fullPath);
	virtual bool SaveMesh(FILE* pFile);
	virtual bool LoadMesh(FILE* pFile);
};

