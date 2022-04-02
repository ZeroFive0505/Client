#pragma once

#include "ColliderComponent.h"

class CColliderPixel :
	public CColliderComponent
{
	friend class CGameObject;

protected:
	CColliderPixel();
	CColliderPixel(const CColliderPixel& com);
	virtual ~CColliderPixel();

protected:
	sPixelInfo m_Info;
	CSharedPtr<class CMesh> m_PixelMesh;
	CSharedPtr<class CShader> m_PixelShader;

public:
	inline sPixelInfo GetInfo() const
	{
		return m_Info;
	}

	void SetInfo(const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetInfoFullPath(const TCHAR* fullPath);
	void SetInfoMultibyte(const char* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetInfoMultibyteFullPath(const char* fullPath);
	void SetPixelCollisionType(PixelCollision_Type type);
	void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);
	void SetPixelAlpha(unsigned char a);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderPixel* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual bool Collision(CColliderComponent* dest);
	virtual bool CollisionMouse(const Vector2& mousePos);
};

