#include "CameraManager.h"

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
