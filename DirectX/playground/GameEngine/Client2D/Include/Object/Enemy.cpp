#include "Enemy.h"
#include "Scene/Scene.h"
#include "Resource/Material/Material.h"
#include "EnemyAnimation2D.h"
#include "../Widget/SimpleHUD.h"
#include "Engine.h"
#include "Input.h"

CEnemy::CEnemy() :
    m_SimpleHUD(nullptr)
{
    SetTypeID<CEnemy>();
    m_HitPoint = 3;
    m_MaxHitPoint = m_HitPoint;
}

CEnemy::CEnemy(const CEnemy& obj) :
    CGameObject(obj)
{
    m_Sprite = (CSpriteComponent*)FindComponent("EnemySprite");
    m_Body = (CColliderCircle*)FindComponent("Body");
    m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");
    m_PaperBurn = (CPaperBurnComponent*)FindComponent("PaperBurn");
}

CEnemy::~CEnemy()
{
}

void CEnemy::DealDamage(int damage)
{
    m_HitPoint -= damage;

    m_Sprite->SetOpacity(m_HitPoint / (float)m_MaxHitPoint);

    if (m_HitPoint <= 0)
        Destroy();
}

void CEnemy::Start()
{
    CGameObject::Start();

    m_PaperBurn->SetFinishCallback<CEnemy>(this, &CEnemy::PaperBurnEnd);
}

bool CEnemy::Init()
{
    m_Sprite = CreateComponent<CSpriteComponent>("EnemySprite");
    m_Body = CreateComponent<CColliderCircle>("Body");
    m_Body->SetCollisionProfile("Enemy");
    m_PaperBurn = CreateComponent<CPaperBurnComponent>("PaperBurn");

    m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

    m_SimpleHUD = m_SimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

    SetRootComponent(m_Sprite);

    m_Sprite->AddChild(m_Body);
    m_Sprite->AddChild(m_SimpleHUDWidget);

    m_SimpleHUDWidget->SetRelativePos(-50.0f, 50.0f, 0.0f);

    m_SimpleHUD->SetText(TEXT("Enemy"));
    m_SimpleHUD->SetHPPercent(0.5f);
    m_SimpleHUD->SetHPDirection(ProgressBar_Direction::RightToLeft);

    m_Sprite->SetTransparency(true);

    m_Sprite->CreateAnimationInstance<CEnemyAnimation2D>();

    m_Sprite->SetRelativeScale(100.0f, 100.0f, 1.0f);
    m_Sprite->SetRelativeRotationZ(45.0f);
    m_Sprite->SetRelativePos(500.0f, 300.0f, 0.0f);
    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);

    m_PaperBurn->SetMaterial(m_Sprite->GetMaterial());

    m_Body->AddCollisionMouseCallback(Collision_State::Begin, this, &CEnemy::OnMouseEnter);
    m_Body->AddCollisionMouseCallback(Collision_State::End, this, &CEnemy::OnMouseExit);

    return true;
}

void CEnemy::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CEnemy::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CEnemy* CEnemy::Clone()
{
    return new CEnemy(*this);
}

void CEnemy::OnMouseEnter(const sCollisionResult& result)
{
    CEngine::GetInst()->SetMouseState(Mouse_State::State1);

 
	m_HitPoint--;

	if (m_HitPoint <= 0)
	{
		m_PaperBurn->StartPaperBurn();
		m_Body->Enable(false);
	}
   
}

void CEnemy::OnMouseExit(const sCollisionResult& result)
{
    CEngine::GetInst()->SetMouseState(Mouse_State::Normal);
}

void CEnemy::PaperBurnEnd()
{
    Destroy();
}
