
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Resource/ResourceManager.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Engine.h"
#include "../Device.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager()	:
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = *iter;
			break;
		}
	}

	if (!Layer)
		return;

	if (Layer->RenderCount == (int)Layer->RenderList.size())
		Layer->RenderList.resize(Layer->RenderCount * 2);

	Layer->RenderList[Layer->RenderCount] = Component;
	++Layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();

	// 기본 레이어 생성
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Back";
	Layer->LayerPriority = 0;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Default";
	Layer->LayerPriority = 1;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 2;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 3;

	m_RenderLayerList.push_back(Layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");

	// 디퍼드 렌더링용 타겟 생성
	Resolution RS = CDevice::GetInst()->GetResolution();

	if (!CResourceManager::GetInst()->CreateTarget("Diffuse",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("Normal",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("Depth",
		RS.Width, RS.Height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;





	return true;
}

void CRenderManager::Render()
{
	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	{
		auto	iter = m_ObjectList->begin();
		auto	iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}	
	}

	// 조명 정보를 Shader로 넘겨준다.
	CSceneManager::GetInst()->GetScene()->GetLightManager()->SetShader();

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->Render();
			}
		}
	}

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				(*iter)->RenderList[i]->PostRender();
			}
		}
	}


	// Widget 출력
	m_DepthDisable->SetState();

	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// 마우스 출력
	CWidgetWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (MouseWidget)
		MouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, 
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, 
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name, 
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

bool CRenderManager::Sortlayer(RenderLayer* Src, RenderLayer* Dest)
{
	return Src->LayerPriority < Dest->LayerPriority;
}
