#include "Widget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "../Scene/SceneManager.h"

CWidget::CWidget() :
	m_Owner(nullptr),
	m_ZOrder(0),
	m_Start(false),
	m_Angle(0.0f),
	m_CBuffer(nullptr),
	m_Size(50.0f, 50.0f),
	m_MouseHovered(false),
	m_MouseCollisionEnable(true),
	m_Opacity(1.0f)
{
}

CWidget::CWidget(const CWidget& widget)
{
	*this = widget;

	// 막 새로 복사되서 생성된 위젯이므로 참조수는 0
	m_RefCount = 0;

	m_Owner = nullptr;
	m_MouseHovered = false;

	m_CBuffer = new CWidgetConstantBuffer;

	m_CBuffer->Init();
}

CWidget::~CWidget()
{
	SAFE_DELETE(m_CBuffer);
}

void CWidget::SetShader(const std::string& name)
{
	m_Shader = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindShader(name);
}

void CWidget::SetUseTexture(bool use)
{
	m_CBuffer->SetUseTexture(use);
}

void CWidget::Start()
{
	m_Start = true;
}

bool CWidget::Init()
{
	if (m_Owner->GetViewport())
	{
		m_Shader = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindShader("WidgetShader");
		m_Mesh = m_Owner->GetViewport()->GetScene()->GetSceneResource()->FindMesh("WidgetMesh");
	}
	else
	{
		m_Shader = CResourceManager::GetInst()->FindShader("WidgetShader");
		m_Mesh = CResourceManager::GetInst()->FindMesh("WidgetMesh");
	}

	m_CBuffer = new CWidgetConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CWidget::Update(float deltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::PostUpdate(float deltaTime)
{
	if (!m_Start)
		Start();

	m_RenderPos = m_Pos;

	if (m_Owner)
		m_RenderPos += m_Owner->GetWindowPos();
}

void CWidget::Render()
{
	if (!m_Start)
		Start();

	Matrix matScale, matRot, matTrans;

	matScale.Scaling(m_Size.x, m_Size.y, 1.0f);
	matRot.Rotation(0.0f, 0.0f, m_Angle);
	matTrans.Translation(m_RenderPos.x, m_RenderPos.y, 0.0f);

	CCameraComponent* UICamera = nullptr;

	if (m_Owner->GetViewport())
		UICamera = m_Owner->GetViewport()->GetScene()->GetCameraManager()->GetUICamera();
	else
		UICamera = CSceneManager::GetInst()->GetScene()->GetCameraManager()->GetUICamera();

	// 여기서 뷰 행렬은 고려하지 않는다. 따라서 UI는 화면 기준에 고정된 위치에 출력된다.
	Matrix matWP = matScale * matRot * matTrans * UICamera->GetProjMatrix();

	matWP.Transpose();

	m_CBuffer->SetWP(matWP);
	m_CBuffer->SetTint(m_Tint);
	m_CBuffer->SetOpacity(m_Opacity);

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

bool CWidget::CollisionMouse(const Vector2& mousePos)
{
	if (!m_MouseCollisionEnable)
		return false;

	if (m_RenderPos.x > mousePos.x)
		return false;
	else if (m_RenderPos.x + m_Size.x < mousePos.x)
		return false;
	else if (m_RenderPos.y > mousePos.y)
		return false;
	else if (m_RenderPos.y + m_Size.y < mousePos.y)
		return false;

	return true;
}

CWidget* CWidget::Clone()
{
	return new CWidget(*this);
}
