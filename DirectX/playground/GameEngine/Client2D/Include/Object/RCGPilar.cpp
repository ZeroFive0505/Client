#include "RCGPilar.h"
#include "RCGPlayer.h"
#include "RCGEnemy.h"
#include "Component/TileMapComponent.h"
#include "Component/Tile.h"
#include "BossEventTrigger.h"

CRCGPilar::CRCGPilar() :
	m_Sprite(nullptr),
	m_Bottom(nullptr),
	m_Pilar(nullptr)
{
	SetTypeID<CRCGPilar>();
}

CRCGPilar::CRCGPilar(const CRCGPilar& obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("Pilar Sprite");
	m_Bottom = (CColliderBox2D*)FindComponent("Pilar Bottom");
	m_Pilar = (CColliderBox2D*)FindComponent("Pilar Collider");
}

CRCGPilar::~CRCGPilar()
{
}

bool CRCGPilar::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("Pilar Sprite");

	m_Sprite->SetPivot(0.5f, 0.0f, 0.0f);

	m_Sprite->SetTransparency(true);

	m_Bottom = CreateComponent<CColliderBox2D>("Pilar Bottom");

	m_Bottom->SetCollisionProfile("Wall");

	m_Bottom->SetPivot(0.5f, 0.0f, 0.0f);

	m_Pilar = CreateComponent<CColliderBox2D>("Pilar Collider");

	m_Pilar->SetCollisionProfile("SideWall");

	m_Pilar->Enable(false);

	m_Pilar->SetPivot(0.5f, 0.5f, 0.0f);

	m_Sprite->AddChild(m_Bottom);

	m_Sprite->AddChild(m_Pilar);

	m_Bottom->AddCollisionCallback(Collision_State::Begin, this, &CRCGPilar::OnPlayerContact);
	m_Bottom->AddCollisionCallback(Collision_State::End, this, &CRCGPilar::OnPlayerExit);

	return true;
}

void CRCGPilar::Update(float deltaTime)
{
	SetCustomY(m_Bottom->GetWorldPos().y);

	CGameObject::Update(deltaTime);

	CGameObject* player = m_Scene->GetPlayerObject();

	if (player->GetCustomY() > m_YPos)
	{
		Vector3 size = m_Sprite->GetWorldScale();

		sBox2DInfo info = m_Bottom->GetInfo();

		if (info.center.x - size.x < (player->GetWorldPos().x - 10.0f) &&
			(player->GetWorldPos().x + 10.0f) < info.center.x + size.x)
		{
			m_Sprite->SetOpacity(0.5f);
		}
		else
			m_Sprite->SetOpacity(1.0f);
	}
	else
		m_Sprite->SetOpacity(1.0f);
}

void CRCGPilar::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CRCGPilar* CRCGPilar::Clone()
{
	return new CRCGPilar(*this);
}

void CRCGPilar::SetPilarTexture(const char* name, const TCHAR* fileName)
{
	m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, name, fileName);

	m_Sprite->SetWorldScale((float)m_Sprite->GetMaterial()->GetTextureWidth() * 2.0f, (float)m_Sprite->GetMaterial()->GetTextureHeight() * 3.0f, 1.0f);

	m_Bottom->SetExtent((float)m_Sprite->GetMaterial()->GetTextureWidth() - 20.0f, 50.0f);

	m_Bottom->SetOffset(0.0f, 50.0f, 0.0f);

	m_Pilar->SetExtent((float)m_Sprite->GetMaterial()->GetTextureWidth(), (float)m_Sprite->GetMaterial()->GetTextureHeight());

	m_Pilar->SetOffset(0.0f, (float)m_Sprite->GetMaterial()->GetTextureHeight() + 100.0f, 0.0f);


	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	player->AddWallCollisionCallback(m_Bottom);
	player->AddWallCollisionCallback(m_Pilar);
}

void CRCGPilar::ResetTile()
{
	CTileMapComponent* tileMap = m_Scene->GetNavigationManager()->GetNavData();
	CTile* tile = tileMap->GetTile(m_TileIndex);

	tile->SetTileType(Tile_Type::Normal);

	m_OwnerTrigger->DeletePilar(this);

	CResourceManager::GetInst()->SoundPlay("Misuzu_tackle_pilar");
}

void CRCGPilar::OnPlayerContact(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGEnemy>())
		return;
	if (result.src->GetGameObject()->TypeCheck<CRCGEnemy>())
		return;

	sBox2DInfo pilarBottomBox = ((CColliderBox2D*)result.src)->GetInfo();
	sBox2DInfo playerBottomBox = ((CColliderBox2D*)result.dest)->GetInfo();

	Vector2 pilarBottomCenter = pilarBottomBox.center;
	Vector2 playerBottomCenter = playerBottomBox.center;

	if ((pilarBottomCenter.y - pilarBottomBox.length.y) < playerBottomCenter.y && 
		playerBottomCenter.y < (pilarBottomCenter.y + pilarBottomBox.length.y))
		m_Pilar->Enable(true);
}

void CRCGPilar::OnPlayerExit(const sCollisionResult& result)
{
	m_Pilar->Enable(false);
}
