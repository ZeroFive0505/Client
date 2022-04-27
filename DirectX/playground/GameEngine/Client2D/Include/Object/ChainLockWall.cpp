#include "ChainLockWall.h"
#include "RCGPlayer.h"
#include "../ClientManager.h"
#include "Component/ColliderComponent.h"

CChainLockWall::CChainLockWall() :
	m_WallSprite(nullptr),
	m_LockSprite(nullptr),
	m_Collider(nullptr)
{
}

CChainLockWall::CChainLockWall(const CChainLockWall& obj) :
	CGameObject(obj)
{
	m_WallSprite = (CSpriteComponent*)FindComponent("ChainWallSprite");
	m_LockSprite = (CSpriteComponent*)FindComponent("LockSprite");
	m_Collider = (CColliderBox2D*)FindComponent("ChainWallCollider");
}

CChainLockWall::~CChainLockWall()
{
	if (m_Collider)
	{
		m_Collider->ClearCallback();
	}
}

bool CChainLockWall::Init()
{
	m_FadeInStart = false;
	m_FadeOutStart = false;
	m_FadeOutComplete = false;

	return true;
}

void CChainLockWall::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	if (m_FadeInStart)
	{
		m_LerpTime += deltaTime / 1.5f;

		if (m_LerpTime > 1.0f)
		{
			m_LerpTime = 1.0f;
			m_FadeInStart = false;
		}

		if (m_WallSprite)
			m_WallSprite->SetOpacity(m_LerpTime);

		if (m_LockSprite)
			m_LockSprite->SetOpacity(m_LerpTime);
	}

	if (m_FadeOutStart)
	{
		m_LerpTime -= deltaTime;

		if (m_LerpTime < 0.0f)
		{
			m_LerpTime = 0.0f;
			m_FadeOutStart = false;
			m_FadeOutComplete = true;
		}

		if (m_WallSprite)
			m_WallSprite->SetOpacity(m_LerpTime);

		if (m_LockSprite)
			m_LockSprite->SetOpacity(m_LerpTime);
	}
}

void CChainLockWall::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CChainLockWall* CChainLockWall::Clone()
{
	return new CChainLockWall(*this);
}

void CChainLockWall::SetAsTop()
{
	m_WallSprite = CreateComponent<CSpriteComponent>("ChainWallSprite");

	m_WallSprite->SetTransparency(true);

	m_WallSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_WallSprite->SetOpacity(0.0f);

	m_LockSprite = CreateComponent<CSpriteComponent>("LockSprite");

	m_WallSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TopWall", TEXT("UI_screenlock_chain_top_13-(STATIC).png"));

	m_WallSprite->AddChild(m_LockSprite);


	m_LockSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Lock", TEXT("UI_screenlock_lock_appear_NEUTRAL.png"));

	m_LockSprite->SetTransparency(true);

	m_LockSprite->SetOpacity(0.0f);

	Vector2 size = Vector2((float)m_LockSprite->GetMaterial()->GetTextureWidth(), (float)m_LockSprite->GetMaterial()->GetTextureHeight());

	m_LockSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_LockSprite->SetRelativeScale(size.x, size.y, 1.0f);

	size = Vector2((float)m_WallSprite->GetMaterial()->GetTextureWidth(), (float)m_WallSprite->GetMaterial()->GetTextureHeight());

	m_WallSprite->SetWorldScale(size.x, size.y, 1.0f);

	m_LockSprite->SetRelativePos(0.0f, -size.y * 0.5f, 0.0f);
}

void CChainLockWall::SetAsBottom()
{
	m_WallSprite = CreateComponent<CSpriteComponent>("ChainWallSprite");

	m_WallSprite->SetTransparency(true);

	m_WallSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_WallSprite->SetOpacity(0.0f);

	m_WallSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "BottomWall", TEXT("UI_screenlock_chain_bottom_13-(STATIC).png"));

	Vector2 size = Vector2((float)m_WallSprite->GetMaterial()->GetTextureWidth(), (float)m_WallSprite->GetMaterial()->GetTextureHeight());

	m_WallSprite->SetWorldScale(size.x, size.y, 1.0f);
}

void CChainLockWall::SetAsLeft()
{
	m_Collider = CreateComponent<CColliderBox2D>("ChainWallCollider");
	m_Collider->SetPivot(0.5f, 0.5f, 0.0f);
	m_Collider->SetCollisionProfile("SideWall");

	m_WallSprite = CreateComponent<CSpriteComponent>("ChainWallSprite");

	m_WallSprite->SetTransparency(true);

	m_WallSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_WallSprite->SetOpacity(0.0f);

	m_WallSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "LeftWall", TEXT("UI_screenlock_chain_left_13-(STATIC).png"));

	Vector2 size = Vector2((float)m_WallSprite->GetMaterial()->GetTextureWidth(), (float)m_WallSprite->GetMaterial()->GetTextureHeight());

	m_WallSprite->SetWorldScale(size.x, size.y, 1.0f);

	m_Collider->SetExtent(size.x, size.y);

	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	player->AddWallCollisionCallback(m_Collider);

	m_Collider->Start();

	m_Collider->AddChild(m_WallSprite);
}

void CChainLockWall::SetAsRight()
{
	m_Collider = CreateComponent<CColliderBox2D>("ChainWallCollider");
	m_Collider->SetPivot(0.5f, 0.5f, 0.0f);
	m_Collider->SetCollisionProfile("SideWall");

	m_WallSprite = CreateComponent<CSpriteComponent>("ChainWallSprite");

	m_WallSprite->SetTransparency(true);

	m_WallSprite->SetPivot(0.5f, 0.5f, 0.0f);

	m_WallSprite->SetOpacity(0.0f);

	m_WallSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "RightWall", TEXT("UI_screenlock_chain_right_13-(STATIC).png"));

	Vector2 size = Vector2((float)m_WallSprite->GetMaterial()->GetTextureWidth(), (float)m_WallSprite->GetMaterial()->GetTextureHeight());

	m_WallSprite->SetWorldScale(size.x, size.y, 1.0f);

	m_Collider->SetExtent(size.x, size.y);

	CRCGPlayer* player = (CRCGPlayer*)m_Scene->GetPlayerObject();

	player->AddWallCollisionCallback(m_Collider);

	m_Collider->Start();

	m_Collider->AddChild(m_WallSprite);
}
