#pragma once

#include "SceneComponent.h"

class CLandScape :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CLandScape();
	CLandScape(const CLandScape& com);
	virtual ~CLandScape();

protected:
	CSharedPtr<class CStaticMesh> m_Mesh;
	int m_CountX;
	int m_CountZ;

	std::vector<sVertex3D> m_vecVtx;
	std::vector<Vector3> m_vecPos;
	std::vector<unsigned int> m_vecIndex;
	std::vector<Vector3> m_vecFaceNormal;
	std::vector<CSharedPtr<class CMaterial>> m_vecMaterialSlot;

public:
	CMaterial* GetMaterial(int index = 0) const
	{
		return m_vecMaterialSlot[index];
	}

public:
	void CreateLandScape(const std::string& name, int countX = 0, int countZ = 0, const TCHAR* heightMap = nullptr, const std::string& pathName = TEXTURE_PATH);
	void SetMaterial(class CMaterial* material, int index = 0);
	void SetMaterial(const std::string& name, int index = 0);
	void AddMaterial(class CMaterial* material);
	void AddMaterial(const std::string& name);
public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CLandScape* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

private:
	void ComputeNormal();
	void ComputeTangent();
};
