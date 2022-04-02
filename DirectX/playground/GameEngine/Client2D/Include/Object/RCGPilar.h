#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CRCGPilar :
	public CGameObject
{
	friend class CScene;

protected:
	CRCGPilar();
	CRCGPilar(const CRCGPilar& obj);
	virtual ~CRCGPilar();

protected:
	CSharedPtr<CSpriteComponent> m_Sprite;
	CSharedPtr<CColliderBox2D> m_Bottom;
	CSharedPtr<CColliderBox2D> m_Pilar;
	int m_TileIndex;
	class CBossEventTrigger* m_OwnerTrigger;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CRCGPilar* Clone();

public:
	void SetPilarTexture(const char* name, const TCHAR* fileName);

	inline void SetPilarSize(const Vector2& size)
	{
		m_Sprite->SetWorldScale(size.x, size.y, 1.0f);
	}

	inline void SetPilarSize(float x, float y)
	{
		m_Sprite->SetWorldScale(x, y, 1.0f);
	}

	inline void SetBottomColliderExtent(const Vector2& size)
	{
		m_Bottom->SetExtent(size.x, size.y);
	}

	inline void SetBottomColliderExtent(float x, float y)
	{
		m_Bottom->SetExtent(x, y);
	}

	inline void SetPilarColliderExtent(const Vector2& size)
	{
		m_Pilar->SetExtent(size.x, size.y);
	}

	inline void SetPilarColliderExtent(float x, float y)
	{
		m_Pilar->SetExtent(x, y);
	}

	inline void SetTileIndex(int index)
	{
		m_TileIndex = index;
	}

public:
	inline CColliderBox2D* GetPilarBottomCollider() const
	{
		return m_Bottom;
	}

	inline void SetOwnerTrigger(class CBossEventTrigger* trigger)
	{
		m_OwnerTrigger = trigger;
	}

	void ResetTile();

private:
	void OnPlayerContact(const sCollisionResult& result);
	void OnPlayerExit(const sCollisionResult& result);
};

