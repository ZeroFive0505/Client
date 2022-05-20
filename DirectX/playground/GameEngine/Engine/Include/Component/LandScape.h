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
	class CLandScapeConstantBuffer* m_CBuffer;
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
	void CreateLandScape(const std::string& name, int countX = 0, int countZ = 0, const TCHAR* heightMap = nullptr, const std::string& pathName = TEXTURE_PATH);
	void SetMaterial(class CMaterial* material, int index = 0);
	void SetMaterial(const std::string& name, int index = 0);
	void AddMaterial(class CMaterial* material);
	void AddMaterial(const std::string& name);
	void SetUVScale(float x, float y);
	void SetUVScale(const Vector2& scale);
	void SetSplatCount(int count);

public:
	float GetHeight(const Vector3& pos);
	Vector3 Bresenham(int x2, int z2);

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

