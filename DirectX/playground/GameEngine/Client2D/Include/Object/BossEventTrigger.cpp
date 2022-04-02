#include "BossEventTrigger.h"
#include "Scene/Scene.h"
#include "Scene/SceneManager.h"
#include "../ClientManager.h"
#include "../RCGGameManager.h"
#include "Component/TileMapComponent.h"
#include "RCGPlayer.h"
#include "RCGPilar.h"


CBossEventTrigger::CBossEventTrigger() :
	m_Trigger(nullptr)
{
	SetTypeID<CBossEventTrigger>();
}

CBossEventTrigger::CBossEventTrigger(const CBossEventTrigger& obj) :
	CGameObject(obj)
{
	m_Trigger = (CColliderBox2D*)FindComponent("BossTrigger");
}

CBossEventTrigger::~CBossEventTrigger()
{
}

bool CBossEventTrigger::Init()
{
	m_Trigger = CreateComponent<CColliderBox2D>("BossTrigger");

	m_Trigger->SetCollisionProfile("SideWall");

	m_Trigger->SetPivot(0.5f, 0.5f, 0.0f);

	m_Trigger->AddCollisionCallback(Collision_State::Begin, this, &CBossEventTrigger::OnPlayerTrigger);

	return true;
}

void CBossEventTrigger::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);
}

void CBossEventTrigger::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CBossEventTrigger* CBossEventTrigger::Clone()
{
	return new CBossEventTrigger(*this);
}

void CBossEventTrigger::DeletePilar(CRCGPilar* pilar)
{
	auto iter = std::find(m_vecPilar.begin(), m_vecPilar.end(), pilar);

	if (iter != m_vecPilar.end())
		m_vecPilar.erase(iter);
}

void CBossEventTrigger::OnPlayerTrigger(const sCollisionResult& result)
{
	if (result.dest->GetGameObject()->TypeCheck<CRCGPlayer>())
	{
		CResourceManager::GetInst()->SoundStop("Detention");
		CResourceManager::GetInst()->SoundPlay("Boss_Intro");
		CResourceManager::GetInst()->SoundPlay("Boss");

		m_RCGBossWidget = m_Scene->GetViewport()->CreateWidgetWindow<CRCGBossWidget>("Boss Widget");

		m_RCGBossWidget->WidgetFadeInStart();

		m_Boss = m_Scene->CreateGameObject<CMisuzu>("Misuzu");

		CTileMapComponent* tileMap = m_Scene->GetNavigationManager()->GetNavData();

		m_Boss->SetWorldPos(1900.0f, tileMap->GetBackMaterial()->GetTextureHeight() * 2.5f - 640.0f, 0.0f);

		m_Boss->PursueStart();

		m_Boss->SetTrigger(this);

		size_t size = m_vecPilar.size();

		for (size_t i = 0; i < size; i++)
		{
			m_Boss->AddTackleCollision(m_vecPilar[i]->GetPilarBottomCollider());
		}

		m_RCGBossWidget->SetBossHP(m_Boss->GetCurrentHP());

		m_Trigger->Enable(false);
	}
}
