#include "TileMap.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "Engine.h"
#include "Component/ColliderBox2D.h"

CTileMap::CTileMap()
{
	SetTypeID<CTileMap>();
}

CTileMap::CTileMap(const CTileMap& obj) :
	CGameObject(obj)
{
	m_TileMap = (CTileMapComponent*)FindComponent("TileMap");
}

CTileMap::~CTileMap()
{
}

void CTileMap::Start()
{
	CGameObject::Start();
}

bool CTileMap::Init()
{
	m_TileMap = CreateComponent<CTileMapComponent>("TileMap");

	SetRootComponent(m_TileMap);

	m_TileMap->CreateTile(Tile_Shape::Rhombus, 100, 100, Vector3(160.0f, 80.0f, 0.0f));

	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("DiabloTileMap");

	m_TileMap->SetTileMaterial(material);

	m_TileMap->SetTileDefaultFrame(160.0f, 80.0f, 320.0f, 160.0f);

	return true;
}

void CTileMap::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CTileMap::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CTileMap* CTileMap::Clone()
{
	return new CTileMap(*this);
}
