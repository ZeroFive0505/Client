#include "Player2D.h"
#include "Scene/Scene.h"
#include "Bullet.h"
#include "BulletTornado.h"
#include "Input.h"
#include "Resource/Material/Material.h"
#include "PlayerAnimation2D.h"
#include "BulletCamera.h"
#include "../Widget/SimpleHUD.h"

CPlayer2D::CPlayer2D()
{
	SetTypeID<CPlayer2D>();
	m_Opacity = 1.0f;
}

CPlayer2D::CPlayer2D(const CPlayer2D& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("PlayerSprite");
	m_LeftChildSprite = (CSpriteComponent*)FindComponent("PlayerLeftChildSprite");	
	m_RightChildSprite = (CSpriteComponent*)FindComponent("PlayerRightChildSprite");
	m_LeftChildMuzzle = (CSceneComponent*)FindComponent("LeftMuzzle");
	m_RightChildMuzzle = (CSceneComponent*)FindComponent("RightMuzzle");
	m_RootChild = (CSceneComponent*)FindComponent("PlayerRootChild");
	m_Muzzle = (CSceneComponent*)FindComponent("Muzzle");

	m_OrbitChild1 = (CSpriteComponent*)FindComponent("PlayerOrbitChild1");
	m_OrbitChild2 = (CSpriteComponent*)FindComponent("PlayerOrbitChild2");
	m_OrbitChild3 = (CSpriteComponent*)FindComponent("PlayerOrbitChild3");
	m_OrbitChild4 = (CSpriteComponent*)FindComponent("PlayerOrbitChild4");
	m_Body = (CColliderBox2D*)FindComponent("Body");
	m_Camera = (CCameraComponent*)FindComponent("Camera");
	m_SimpleHUDWidget = (CWidgetComponent*)FindComponent("SimpleHUD");

	m_Opacity = obj.m_Opacity;
}

CPlayer2D::~CPlayer2D()
{
}

bool CPlayer2D::Init()
{
	// 스프라이트 컴포넌트 생성시 
	// 왼쪽 최하단을 0, 0으로 하는 메쉬가 생성이된다.
	// 크기는 1
	m_Sprite = CreateComponent<CSpriteComponent>("PlayerSprite");

	m_Body = CreateComponent<CColliderBox2D>("Body");

	m_Body->SetCollisionProfile("Player");

	// m_Sprite 컴포넌트가 이 게임 오브젝트의 루트 컴포넌트이다.
	SetRootComponent(m_Sprite);

	// m_Camera = CreateComponent<CCameraComponent>("Camera");

	// m_Camera->OnViewPortCenter();

	m_SimpleHUDWidget = CreateComponent<CWidgetComponent>("SimpleHUD");

	m_SimpleHUDWidget->CreateWidgetWindow<CSimpleHUD>("SimpleHUDWidget");

	
	// 알파 온
	m_Sprite->SetTransparency(true);

	// 플레이어 애니메이션을 생성하면서 스프라이트를 자동으로 교체한다.
	m_Sprite->CreateAnimationInstance<CPlayerAnimation2D>();


	m_LeftChildSprite = CreateComponent<CSpriteComponent>("PlayerLeftChildSprite");
	m_RightChildSprite = CreateComponent<CSpriteComponent>("PlayerRightChildSprite");


	m_LeftChildSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Teemo", TEXT("Teemo.jpg"));
	m_RightChildSprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Teemo", TEXT("Teemo.jpg"));

	m_LeftChildSprite->SetBaseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_RightChildSprite->SetBaseColor(1.0f, 0.0f, 0.0f, 1.0f);

	// 일반 씬컴포넌트들은 메쉬나 스프라이트들을 가지고 있지는 않아 화면에 렌더링 되지는 않지만
	// 트랜스폼 컴포넌트를 소유하고 있기에 그걸 이용해서 활용할 수 있다.
	m_RootChild = CreateComponent<CSceneComponent>("PlayerRootChild");
	m_Muzzle = CreateComponent<CSceneComponent>("Muzzle");
	m_LeftChildMuzzle = CreateComponent<CSceneComponent>("LeftMuzzle");
	m_RightChildMuzzle = CreateComponent<CSceneComponent>("RightMuzzle");

	m_OrbitChild1 = CreateComponent<CSpriteComponent>("PlayerOrbitChild1");
	m_OrbitChild2 = CreateComponent<CSpriteComponent>("PlayerOrbitChild2");
	m_OrbitChild3 = CreateComponent<CSpriteComponent>("PlayerOrbitChild3");
	m_OrbitChild4 = CreateComponent<CSpriteComponent>("PlayerOrbitChild4");

	// 순서 중요
	// m_Sprite를 먼저 설정하고 자식들을 넣을게 아니라 자식들도 전부 다 같이 설정하고
	// 마지막에 m_Sprite를 설정한다.

	// 현재 루트 컴포넌트인 m_Sprite에 자식 오브젝트를 추가한다.
	m_Sprite->AddChild(m_Body);
	// m_Sprite->AddChild(m_Camera);
	m_Sprite->AddChild(m_LeftChildSprite);
	m_Sprite->AddChild(m_RightChildSprite);
	m_Sprite->AddChild(m_Muzzle);
	m_Sprite->AddChild(m_RootChild);
	m_Sprite->AddChild(m_SimpleHUDWidget);

	m_SimpleHUDWidget->SetRelativePos(-50.0f, 50.0f, 0.0f);

	// 왼쪽 자식에 씬 컴포넌트를 추가한다.
	m_LeftChildSprite->AddChild(m_LeftChildMuzzle);
	// 오른쪽 자식에 씬 컴포넌트를 추가한다.
	m_RightChildSprite->AddChild(m_RightChildMuzzle);

	// 비어있는 씬 컴포넌트에 주위를 돌 Orbit 자식을 넣는다.
	m_RootChild->AddChild(m_OrbitChild1);
	m_RootChild->AddChild(m_OrbitChild2);
	m_RootChild->AddChild(m_OrbitChild3);
	m_RootChild->AddChild(m_OrbitChild4);

	// 머즐은 플레이어의 위치에서 상대적으로 y축 위에 위치한다.
	m_Muzzle->SetRelativePos(0.0f, 100.0f, 0.0f);
	m_Muzzle->SetInheritRotZ(true);

	// 왼쪽 자식의 머즐은 상대적으로 왼쪽 자식의 y축 위에 위치한다.
	m_LeftChildMuzzle->SetRelativePos(0.0f, 100.0f, 0.0f);
	m_LeftChildMuzzle->SetInheritRotZ(true);

	// 위와 마찬가지
	m_RightChildMuzzle->SetRelativePos(0.0f, 100.0f, 0.0f);
	m_RightChildMuzzle->SetInheritRotZ(true);

	// 여기서 스케일일 재조정해서 전체 화면 해상도에서 가로 100 세로 100
	m_Sprite->SetRelativeScale(100.0f, 100.0f, 1.0f);

	// 가로 100 픽셀 세로 50픽셀 위로 올린다.
	m_Sprite->SetRelativePos(100.0f, 50.0f, 0.0f);

	// 여기서 메쉬의 가운데를 피벗으로 정한다.
	m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);


	m_RightChildSprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
	m_RightChildSprite->SetInheritScale(false);
	m_RightChildSprite->SetRelativePos(100.0f, 0.0f, 0.0f);
	m_RightChildSprite->SetPivot(0.5f, 0.5f, 0.0f);
	m_RightChildSprite->SetInheritRotZ(true);

	m_LeftChildSprite->SetRelativeScale(50.0f, 50.0f, 1.0f);
	m_LeftChildSprite->SetInheritScale(false);
	m_LeftChildSprite->SetRelativePos(-100.0f, 0.0f, 0.0f);
	m_LeftChildSprite->SetPivot(0.5f, 0.5f, 0.0f);
	m_LeftChildSprite->SetInheritRotZ(true);

	m_OrbitChild1->SetRelativeScale(25.0f, 25.0f, 1.0f);
	m_OrbitChild1->SetInheritScale(false);
	m_OrbitChild1->SetRelativePos(200.0f, 0.0f, 0.0f);
	m_OrbitChild1->SetPivot(0.5f, 0.5f, 0.0f);
	m_OrbitChild1->SetInheritRotZ(true);

	m_OrbitChild2->SetRelativeScale(25.0f, 25.0f, 1.0f);
	m_OrbitChild2->SetInheritScale(false);
	m_OrbitChild2->SetRelativePos(-200.0f, 0.0f, 0.0f);
	m_OrbitChild2->SetPivot(0.5f, 0.5f, 0.0f);
	m_OrbitChild2->SetInheritRotZ(true);

	m_OrbitChild3->SetRelativeScale(25.0f, 25.0f, 1.0f);
	m_OrbitChild3->SetInheritScale(false);
	m_OrbitChild3->SetRelativePos(0.0f, 200.0f, 0.0f);
	m_OrbitChild3->SetPivot(0.5f, 0.5f, 0.0f);
	m_OrbitChild3->SetInheritRotZ(true);

	m_OrbitChild4->SetRelativeScale(25.0f, 25.0f, 1.0f);
	m_OrbitChild4->SetInheritScale(false);
	m_OrbitChild4->SetRelativePos(0.0f, -200.0f, 0.0f);
	m_OrbitChild4->SetPivot(0.5f, 0.5f, 0.0f);
	m_OrbitChild4->SetInheritRotZ(true);

	CInput::GetInst()->SetCallback<CPlayer2D>("MoveUp", KeyState_Push, this, &CPlayer2D::MoveUp);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveDown", KeyState_Push, this, &CPlayer2D::MoveDown);
	//CInput::GetInst()->SetCallback<CPlayer2D>("RotationZInv", KeyState_Push, this, &CPlayer2D::RotationZInv);
	//CInput::GetInst()->SetCallback<CPlayer2D>("RotationZ", KeyState_Push, this, &CPlayer2D::RotationZ);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveLeft", KeyState_Push, this, &CPlayer2D::MoveLeft);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveRight", KeyState_Push, this, &CPlayer2D::MoveRight);
	//CInput::GetInst()->SetCallback<CPlayer2D>("Attack", KeyState_Down, this, &CPlayer2D::Attack);
	//CInput::GetInst()->SetCallback<CPlayer2D>("Attack1", KeyState_Push, this, &CPlayer2D::Attack1);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveUp", KeyState_Up, this, &CPlayer2D::BackToIdle);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveDown", KeyState_Up, this, &CPlayer2D::BackToIdle);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveLeft", KeyState_Up, this, &CPlayer2D::BackToIdle);
	CInput::GetInst()->SetCallback<CPlayer2D>("MoveRight", KeyState_Up, this, &CPlayer2D::BackToIdle);
	// CInput::GetInst()->SetCallback<CPlayer2D>("Skill", KeyState_Down, this, &CPlayer2D::Skill);

	return true;
}

void CPlayer2D::Update(float deltaTime)
{
	CGameObject::Update(deltaTime);

	static bool fire = false;

	static bool hide = false;

	if (GetAsyncKeyState('2') & 0x8000)
	{
		fire = true;
	}
	else if (fire)
	{
		fire = false;

		CBulletTornado* bullet = m_Scene->CreateGameObject<CBulletTornado>("Bullet");

		bullet->SetWorldPos(m_Muzzle->GetWorldPos());
		bullet->SetWorldRotation(m_Muzzle->GetWorldRot());

		hide = true;
	}

	m_RootChild->AddRelativeRotation(0.0f, 0.0f, -180.0f * deltaTime);

	if (hide)
	{
		m_Opacity -= deltaTime / 5.0f;

		if (m_Opacity < 0.0f)
			m_Opacity = 0.0f;

		m_Sprite->SetOpacity(m_Opacity);
	}
}

void CPlayer2D::PostUpdate(float deltaTime)
{
	CGameObject::PostUpdate(deltaTime);
}

CPlayer2D* CPlayer2D::Clone()
{
	return new CPlayer2D(*this);
}

void CPlayer2D::MoveUp(float deltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * 300.0f * deltaTime);

	m_Sprite->ChangeAnimation("Walk");
}

void CPlayer2D::MoveDown(float deltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_Y) * -300.0f * deltaTime);

	m_Sprite->ChangeAnimation("Walk");
}

void CPlayer2D::MoveLeft(float deltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * -300.0f * deltaTime);

	m_Sprite->ChangeAnimation("Walk");
}

void CPlayer2D::MoveRight(float deltaTime)
{
	m_Sprite->AddRelativePos(m_Sprite->GetWorldAxis(AXIS_X) * 300.0f * deltaTime);

	m_Sprite->ChangeAnimation("Walk");
}

void CPlayer2D::RotationZInv(float deltaTime)
{
	// 반시계방향 따라서 +
	m_Sprite->AddRelativeRotationZ(180.0f * deltaTime);

}

void CPlayer2D::RotationZ(float deltaTime)
{
	// 시계방향으로 회전한다. 따라서 - 회전하면서 Y축은 바뀌게된다.		
	m_Sprite->AddRelativeRotationZ(-180.0f * deltaTime);

}

void CPlayer2D::Attack(float deltaTime)
{
	// 총알을 현재 씬에 생성한다.
	CBullet* bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	// 총알의 위치는 현재 플레이어의 월드 위치에서 Y축으로 75픽셀만큼 오프셋을 위치로한다.
	// bullet->SetWorldPos(GetWorldPos() + GetWorldAxis(AXIS_Y) * 75.0f);
	bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	bullet->SetWorldRotation(m_Muzzle->GetWorldRot());
	// 총알의 회전수치는 현재 플레이어의 회전 수치와 동일하게 한다.
	// 따라서 총알은 현재 플레이어가 바라보고있는 곳으로 날라가게된다.
	bullet->SetWorldRotation(GetWorldRot());
}

void CPlayer2D::Attack1(float deltaTime)
{
	CBullet* bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	bullet->SetWorldRotation(m_Muzzle->GetWorldRot());

	bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	bullet->SetWorldRotation(m_Muzzle->GetWorldRot() + Vector3(0.0f, 0.0f, 45.0f));

	bullet = m_Scene->CreateGameObject<CBullet>("Bullet");

	bullet->SetWorldPos(m_Muzzle->GetWorldPos());
	bullet->SetWorldRotation(m_Muzzle->GetWorldRot() + Vector3(0.0f, 0.0f, -45.0f));
}

void CPlayer2D::BackToIdle(float deltaTime)
{
	m_Sprite->ChangeAnimation("Idle");
}

void CPlayer2D::Skill(float deitlaTime)
{
	CBulletCamera* bullet = m_Scene->CreateGameObject<CBulletCamera>("Bullet");

	bullet->SetWorldPos(m_Sprite->GetWorldPos());
	bullet->SetWorldRotation(m_Sprite->GetWorldRot());
}
