#include "ColliderBox2D.h"
#include "../Collision/Collision.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"
#include "ColliderComponent.h"
#include "CameraComponent.h"
#include "ColliderCircle.h"
#include "../Scene/CameraManager.h"
#include "ColliderPixel.h"

CColliderBox2D::CColliderBox2D()
{
	SetTypeID<CColliderBox2D>();
	m_ComponentType = Component_Type::SceneComponent;
	m_Render = true;

	m_ColliderType = Collider_Type::Box2D;
}

CColliderBox2D::CColliderBox2D(const CColliderBox2D& com) :
	CColliderComponent(com)
{
	m_Info = com.m_Info;
}

CColliderBox2D::~CColliderBox2D()
{
}

void CColliderBox2D::Start()
{
	CColliderComponent::Start();
}

bool CColliderBox2D::Init()
{
	if (!CColliderComponent::Init())
		return false;

	// 초기에는 x축과 y축이 회전하지 않았다고 가정
	m_Info.axis[0] = Vector2(1.0f, 0.0f);
	m_Info.axis[1] = Vector2(0.0f, 1.0f);

	m_Info.length = Vector2(50.0f, 50.0f);


	// z축 회전을 상속받는다.
	SetInheritRotZ(true);

	// 최종적으로 100, 100 크기의 사각형
	SetWorldScale(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, 1.0f);


	if (m_Scene)
		m_Mesh = m_Scene->GetSceneResource()->FindMesh("Box2D");
	else
		m_Mesh = CResourceManager::GetInst()->FindMesh("Box2D");

	return true;
}

void CColliderBox2D::Update(float deltaTime)
{
	CColliderComponent::Update(deltaTime);
}

void CColliderBox2D::PostUpdate(float deltaTime)
{
	CColliderComponent::PostUpdate(deltaTime);

	// 모든 업데이트가 끝난후 충돌체의 중심점 축이 얼마나 회전했는지 계산한다.

	// 충돌체의 중심점
	m_Info.center.x = GetWorldPos().x + m_Offset.x;
	m_Info.center.y = GetWorldPos().y + m_Offset.y;

	// 충돌체의 회전 이후 x축
	m_Info.axis[0].x = GetWorldAxis(AXIS_X).x;
	m_Info.axis[0].y = GetWorldAxis(AXIS_X).y;

	// 충돌체의 회전 이후 y축
	m_Info.axis[1].x = GetWorldAxis(AXIS_Y).x;
	m_Info.axis[1].y = GetWorldAxis(AXIS_Y).y;

	Vector2 pos[4] = {};

	// 사격형의 4개의 꼭지점의 좌표를 구한다.
	pos[0] = m_Info.center - m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y;
	pos[1] = m_Info.center + m_Info.axis[0] * m_Info.length.x + m_Info.axis[1] * m_Info.length.y;
	pos[2] = m_Info.center - m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y;
	pos[3] = m_Info.center + m_Info.axis[0] * m_Info.length.x - m_Info.axis[1] * m_Info.length.y;

	// 첫번째 꼭지점을 초기값으로 잡는다.
	m_Min.x = pos[0].x;
	m_Min.y = pos[0].y;

	m_Max.x = pos[0].x;
	m_Max.y = pos[0].y;

	for (int i = 1; i < 4; i++)
	{
		if (m_Min.x > pos[i].x)
			m_Min.x = pos[i].x;

		if (m_Min.y > pos[i].y)
			m_Min.y = pos[i].y;

		if (m_Max.x < pos[i].x)
			m_Max.x = pos[i].x;

		if (m_Max.y < pos[i].y)
			m_Max.y = pos[i].y;
	}

	m_Info.Min.x = m_Min.x;
	m_Info.Min.y = m_Min.y;
	
	m_Info.Max.x = m_Max.x;
	m_Info.Max.y = m_Max.y;
}

void CColliderBox2D::PrevRender()
{
	CColliderComponent::PrevRender();
}

void CColliderBox2D::Render()
{
	CColliderComponent::Render();

	CCameraComponent* camera = m_Scene->GetCameraManager()->GetCurrentCamera();

	Matrix matWorld, matView, matProj, matWVP;
	
	matView = camera->GetViewMatrix();
	matProj = camera->GetProjMatrix();

	Matrix matScale, matRot, matTrans;

	matScale.Scaling(m_Info.length.x * 2.0f, m_Info.length.y * 2.0f, 1.0f);
	matRot.Rotation(GetWorldRot());
	matTrans.Translation(m_Info.center);

	matWorld = matScale * matRot * matTrans;

	matWVP = matWorld * matView * matProj;

	// 쉐이더는 전치된 행렬을 받아서 처리한다.
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

void CColliderBox2D::PostRender()
{
	CColliderComponent::PostRender();
}

CColliderBox2D* CColliderBox2D::Clone()
{
	return new CColliderBox2D(*this);
}

void CColliderBox2D::Save(FILE* pFile)
{
	CColliderComponent::Save(pFile);

	fwrite(&m_Info, sizeof(sBox2DInfo), 1, pFile);
}

void CColliderBox2D::Load(FILE* pFile)
{
	CColliderComponent::Load(pFile);

	fread(&m_Info, sizeof(sBox2DInfo), 1, pFile);
}

bool CColliderBox2D::Collision(CColliderComponent* dest)
{
	switch (dest->GetColliderType())
	{
	case Collider_Type::Box2D:
		return CCollision::CollisionBox2DToBox2D(this, (CColliderBox2D*)dest);
	case Collider_Type::Circle:
		return CCollision::CollisionBox2DToCircle(this, (CColliderCircle*)dest);
	case Collider_Type::Pixel:
		return CCollision::CollisionBox2DToPixel(this, (CColliderPixel*)dest);
	}

	return false;
}

bool CColliderBox2D::CollisionMouse(const Vector2& mousePos)
{
	sCollisionResult result;

	m_MouseCollision = CCollision::CollisionBox2DToPoint(m_MouseResult, result, m_Info, mousePos);

	return m_MouseCollision;
}
