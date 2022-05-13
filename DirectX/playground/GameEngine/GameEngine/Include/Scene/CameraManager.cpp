#include "CameraManager.h"
#include "../Input.h"
#include "../Engine.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::Start()
{
	if (m_CurrentCamera->GetName() == "MainCamera")
		m_CurrentCamera->Start();

	m_UICamera->Start();
}

void CCameraManager::Init()
{
	m_CurrentCamera = new CCameraComponent;

	m_CurrentCamera->Init();

	m_CurrentCamera->SetName("MainCamera");
	m_CurrentCamera->m_Scene = m_Scene;
	m_CurrentCamera->SetCameraType(Camera_Type::Camera3D);

	m_UICamera = new CCameraComponent;

	m_UICamera->Init();
	
	m_UICamera->SetName("UICamera");
	m_UICamera->m_Scene = m_Scene;
	m_UICamera->SetCameraType(Camera_Type::CameraUI);
}

void CCameraManager::Update(float deltaTime)
{
	// 현재 메인 카메라만 업데이트를 갱신한다.
	if (m_CurrentCamera->GetName() == "MainCamera")
		m_CurrentCamera->Update(deltaTime);
}

void CCameraManager::PostUpdate(float deltaTime)
{
	if (m_CurrentCamera->GetName() == "MainCamera")
		m_CurrentCamera->PostUpdate(deltaTime);
}

void CCameraManager::RayCast()
{
	float pointX, pointY;
	Vector2 mousePos = CInput::GetInst()->GetMousePos();
	sResolution rs = CEngine::GetInst()->GetResolution();
	Matrix projMat, viewMat, invViewMat, worldMat, translateMat, invWorldMat;
	Vector3 direction, origin, rayOrigin, rayDirection;

	// Move the mouse cursor coordinates into the -1 to +1 range.
	pointX = ((2.0f * (float)mousePos.x) / (float)rs.width) - 1.0f;
	pointY = (((2.0f * (float)mousePos.y) / (float)rs.height) - 1.0f);

	projMat = m_CurrentCamera->GetProjMatrix();
	pointX = pointX / projMat._11;
	pointY = pointY / projMat._22;

	// Get the inverse of the view matrix.
	viewMat = m_CurrentCamera->GetViewMatrix();
	invViewMat = Matrix::StaticInverse(viewMat);

	// Calculate the direction of the picking ray in view space.
	direction.x = (pointX * invViewMat._11) + (pointY * invViewMat._21) + invViewMat._31;
	direction.y = (pointX * invViewMat._12) + (pointY * invViewMat._22) + invViewMat._32;
	direction.z = (pointX * invViewMat._13) + (pointY * invViewMat._23) + invViewMat._33;

	// Get the origin of the picking ray which is the position of the camera.
	origin = m_CurrentCamera->GetWorldPos();

	worldMat = m_CurrentCamera->GetWorldMatrix();

	// Now get the inverse of the translated world matrix.
	invWorldMat = Matrix::StaticInverse(worldMat);

	// Now transform the ray origin and the ray direction from view space to world space.
	rayOrigin = origin;
	rayOrigin.TransformCoord(invWorldMat);
	rayDirection = direction;
	rayDirection.TransformNormal(invWorldMat);

	rayDirection.Normalize();

	m_CurrentCamera->SetRay(rayDirection);
}