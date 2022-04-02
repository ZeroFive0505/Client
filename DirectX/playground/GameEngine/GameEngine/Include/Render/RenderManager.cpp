#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "../Component/SceneComponent.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Engine.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr),
	m_DepthDisable(nullptr),
	m_AlphaBlend(nullptr)
{

}

CRenderManager::~CRenderManager()
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_RenderStateManager);
	SAFE_DELETE(m_Standard2DCBuffer);
}

void CRenderManager::AddRenderList(CSceneComponent* componenet)
{
	sRenderLayer* layer = nullptr;

	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->name == componenet->GetLayerName())
		{
			layer = *iter;
			break;
		}
	}

	// 해당 이름의 레이어를 발견하지 못했다면 바로 돌아간다.
	if (!layer)
		return;

	if (layer->renderCount == (int)layer->renderList.size())
		layer->renderList.resize(layer->renderCount * 2);

	layer->renderList[layer->renderCount++] = componenet;
}

void CRenderManager::CreateLayer(const std::string& name, int priority)
{
	sRenderLayer* layer = new sRenderLayer;
	layer->name = name;
	layer->layerPriority = priority;

	m_RenderLayerList.push_back(layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

void CRenderManager::SetLayerPriority(const std::string& name, int priority)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->name == name)
		{
			(*iter)->layerPriority = priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::SortLayer);
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	if (!m_RenderStateManager->Init())
		return false;

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();

	// 기본 레이어 생성
	sRenderLayer* layer = new sRenderLayer;
	layer->name = "Back";
	layer->layerPriority = 0;

	m_RenderLayerList.push_back(layer);

	layer = new sRenderLayer;
	layer->name = "Shadow";
	layer->layerPriority = 1;

	m_RenderLayerList.push_back(layer);

	layer = new sRenderLayer;
	layer->name = "Default";
	layer->layerPriority = 2;

	m_RenderLayerList.push_back(layer);

	layer = new sRenderLayer;
	layer->name = "Particle";
	layer->layerPriority = 3;

	m_RenderLayerList.push_back(layer);

	// 스크린 위젯 레이어 생성
	layer = new sRenderLayer;
	layer->name = "ScreenWidgetComponent";
	layer->layerPriority = 4;

	m_RenderLayerList.push_back(layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");

	return true;
}

void CRenderManager::Render()
{
	m_DepthDisable->SetState();

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->renderCount = 0;
		}
	}

	{
		auto iter = m_ObjectList->begin();
		auto iterEnd = m_ObjectList->end();

		for (; iter != iterEnd; iter++)
		{
			// 여기서 오브젝트들의 PrevRender함수를 호출하면서 모든 컴포넌트의 PrevRender를 호출하게 된다.
			(*iter)->PrevRender();
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; iter++)
		{
			for (int i = 0; i < (*iter)->renderCount; i++)
			{
				(*iter)->renderList[i]->Render();
			}
		}
	}

	{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; iter++)
		{
			for (int i = 0; i < (*iter)->renderCount; i++)
			{
				(*iter)->renderList[i]->PostRender();
			}
		}
	}

	m_DepthDisable->ResetState();

	m_DepthDisable->SetState();

	// UI, Widget 출력
	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// 마우스 출력
	CWidgetWindow* mouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (mouseWidget)
		mouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::SetBlendFactor(const std::string& name, float r, float g, float b, float a)
{
	m_RenderStateManager->SetBlendFactor(name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& name, bool BlendEnable, D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp, D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha, D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& name, bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& name)
{
	return m_RenderStateManager->FindRenderState(name);
}

bool CRenderManager::SortLayer(sRenderLayer* src, sRenderLayer* dest)
{
	return src->layerPriority < dest->layerPriority;
}
