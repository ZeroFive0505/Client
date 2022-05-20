#pragma once

#include "SceneComponent.h"

class CWaterPlane :
	public CSceneComponent
{
	friend class CGameObject;

protected:
	CWaterPlane();
	CWaterPlane(const CWaterPlane& com);
	virtual ~CWaterPlane();

protected:
	CSharedPtr<class CStaticMesh> m_Mesh;
	int m_CountX;
	int m_CountZ;

	std::vector<sVertex3D> m_vecVtx;
	std::vector<Vector3> m_vecPos;
	std::vector<unsigned int> m_vecIndex;
	std::vector<Vector3> m_vecFaceNormal;
	std::vector<CSharedPtr<class CMaterial>> m_vecMaterialSlot;
	class CWaterConstantBuffer* m_CBuffer;
	Vector3 m_Min;
	Vector3 m_Max;

public:
	void SetPickable(bool val);

	inline const Vector3& GetMin() const
	{
		return m_Min;
	}

	inline const Vector3& GetMax() const
	{
		return m_Max;
	}

	CMaterial* GetMaterial(int index = 0) const
	{
		return m_vecMaterialSlot[index];
	}

public:
	void CreateWaterPlane(const std::string& name, int countX = 0, int countZ = 0, const std::string& pathName = TEXTURE_PATH);
	void SetMaterial(class CMaterial* material, int index = 0);
	void SetMaterial(const std::string& name, int index = 0);
	void AddMaterial(class CMaterial* mateirla);
	void AddMaterial(const std::string& name);
	void SetUVScale(float x, float y);
	void SetUVScale(const Vector2& scale);
	void SetScrollSpeed(float x, float y);
	void SetScrollSpeed(const Vector2& speed);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CWaterPlane* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
};

