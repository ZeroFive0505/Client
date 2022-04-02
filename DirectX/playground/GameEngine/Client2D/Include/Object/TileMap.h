#pragma once

#include "GameObject/GameObject.h"
#include "Component/TileMapComponent.h"

class CTileMap :
	public CGameObject
{
	friend class CScene;

protected:
	CTileMap();
	CTileMap(const CTileMap& obj);
	virtual ~CTileMap();

private:
	CSharedPtr<CTileMapComponent> m_TileMap;

public:
	inline CTileMapComponent* GetTileMap() const
	{
		return m_TileMap;
	}

	inline void SetTileMap()
	{
		m_TileMap = (CTileMapComponent*)m_SceneComponentList.front();
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual CTileMap* Clone();
};

