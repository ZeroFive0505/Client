#include "ColliderCircle.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"
#include "ColliderBox2D.h"
#include "ColliderPixel.h"

CColliderCircle::CColliderCircle()
{
	SetTypeID<CColliderCircle>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Circle;
}

CColliderCircle::CColliderCircle(const CColliderCircle& com) :
	CColliderComponent(com)
{
	m_Info = com.m_Info;
}

CColliderCircle::~CColliderCircle()
{
}

void CColliderCircle::Start()
{
	CColliderComponent::Start();
}

bool CColliderCircle::Init()
{
	if (!CColliderComponent::Init())
		return false;

	m_Info.radius = 50.0f;

	SetInheritRotZ(true);

	SetWorldScale(m_Info.radius * 2.0f, m_Info.radius * 2.0f, 1.0f);
	
	if (m_Scene)
		m_Mesh = m_Scene->GetSceneResource()->FindMesh("Circle");
	else
		m_Mesh = CResourceManager::GetInst()->FindMesh("Circle");

	return true;
}

void CColliderCircle::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderCircle::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	m_Info.center.x = GetWorldPos().x + m_Offset.x;
	m_Info.center.y = GetWorldPos().y + m_Offset.y;

	m_Min.x = m_Info.center.x - m_Info.radius;
	m_Min.y = m_Info.center.y - m_Info.radius;

	m_Max.x = m_Info.center.x + m_Info.radius;
	m_Max.y = m_Info.center.y + m_Info.radius;

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;

	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;
}

void CColliderCircle::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderCircle::Render()
{
	CColliderComponent::Render();

	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix matWorld, matView, matProj, matWVP;

	matView = camera->GetViewMatrix();
	matProj = camera->GetProjMatrix();

	Matrix matScale, matTrans;

	// 반지름 0.5를 2배해서 지름 1의 원을 생성
	matScale.Scaling(m_Info.radius * 2.0f, m_Info.radius * 2.0f, 1.0f);
	// matRot.Rotation(GetWorldRot());
	matTrans.Translation(m_Info.center);

	matWorld = matScale * matTrans;

	matWVP = matWorld * matView * matProj;

	matWVP.Transpose();

	m_CBuffer->SetWVP(matWVP);

	if (m_PrevCollisionList.empty())
		m_CBuffer->SetColliderColor(Vector4(0.0f, 1.0f, 0.0f, 1.0f));
	else
		m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	if (m_MouseCollision)
		m_CBuffer->SetColliderColor(Vector4(1.0f, 0.0f, 0.0f, 1.0f));

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

void CColliderCircle::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderCircle* CColliderCircle::Clone()
{
	return new CColliderCircle(*this);
}

void CColliderCircle::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);

	fwrite(&m_Info, sizeof(sCircleInfo), 1, pFile);
}

void CColliderCircle::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);

	fread(&m_Info, sizeof(sCircleInfo), 1, pFile);
}

bool CColliderCircle::Collision(CColliderComponent* dest)
{
	switch (dest->GetColliderType())
	{
	case Collider_Type::Box2D:
		return CCollision::CollisionBox2DToCircle((CColliderBox2D*)dest, this);
	case Collider_Type::Circle:
		return CCollision::CollisionCircleToCircle(this, (CColliderCircle*)dest);
	case Collider_Type::Pixel:
		return CCollision::CollisionCircleToPixel(this, (CColliderPixel*)dest);
	}

	return false;
}

bool CColliderCircle::CollisionMouse(const Vector2& mousePos)
{
	sCollisionResult result;

	m_MouseCollision = CCollision::CollisionCircleToPoint(m_MouseResult, result, m_Info, mousePos);

	return m_MouseCollision;
}
