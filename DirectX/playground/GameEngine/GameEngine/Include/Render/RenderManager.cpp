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
#include "../Device.h"
#include "../Resource/Shader/Shader.h"

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

	//layer = new sRenderLayer;
	//layer->name = "Shadow";
	//layer->layerPriority = 1;

	//m_RenderLayerList.push_back(layer);

	layer = new sRenderLayer;
	layer->name = "Default";
	layer->layerPriority = 1;

	m_RenderLayerList.push_back(layer);

	layer = new sRenderLayer;
	layer->name = "Particle";
	layer->layerPriority = 2;

	m_RenderLayerList.push_back(layer);

	// 스크린 위젯 레이어 생성
	layer = new sRenderLayer;
	layer->name = "ScreenWidgetComponent";
	layer->layerPriority = 3;

	m_RenderLayerList.push_back(layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");
	m_LightAccBlend = m_RenderStateManager->FindRenderState("LightAcc");

	// 디퍼드 렌더링용 타겟 설정
	sResolution rs = CDevice::GetInst()->GetResolution();

	if (!CResourceManager::GetInst()->CreateTarget("Diffuse",
		rs.width, rs.height, DXGI_FORMAT_R8G8B8A8_UNORM))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer1",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer2",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("GBuffer3",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("Diffuse");
	GBufferTarget->SetPos(Vector3(0.0f, 0.0f, 0.0f));
	GBufferTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer1");
	GBufferTarget->SetPos(Vector3(0.0f, 150.0f, 0.0f));
	GBufferTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer2");
	GBufferTarget->SetPos(Vector3(0.0f, 300.0f, 0.0f));
	GBufferTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	GBufferTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("GBuffer3");
	GBufferTarget->SetPos(Vector3(0.0f, 450.0f, 0.0f));
	GBufferTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	GBufferTarget->SetDebugRender(true);
	m_vecGBuffer.push_back(GBufferTarget);

	// 광원 처리용 버퍼
	
	if (!CResourceManager::GetInst()->CreateTarget("LightDif",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightSpc",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	if (!CResourceManager::GetInst()->CreateTarget("LightEmv",
		rs.width, rs.height, DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightDif");
	lightTarget->SetPos(Vector3(150.0f, 0.0f, 0.0f));
	lightTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightSpc");
	lightTarget->SetPos(Vector3(150.0f, 150.0f, 0.0f));
	lightTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	lightTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("LightEmv");
	lightTarget->SetPos(Vector3(150.0f, 300.0f, 0.0f));
	lightTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	lightTarget->SetDebugRender(true);
	m_vecLightBuffer.push_back(lightTarget);

	if (!CResourceManager::GetInst()->CreateTarget("FinalScreen", rs.width, rs.height, 
		DXGI_FORMAT_R32G32B32A32_FLOAT))
		return false;

	CRenderTarget* finalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");
	finalScreenTarget->SetPos(Vector3(300.0f, 0.0f, 0.0f));
	finalScreenTarget->SetScale(Vector3(150.0f, 150.0f, 1.0f));
	finalScreenTarget->SetDebugRender(true);

	m_LightBlendShader = CResourceManager::GetInst()->FindShader("LightBlendShader");
	m_LightBlendRenderShader = CResourceManager::GetInst()->FindShader("LightBlendRenderShader");

	return true;
}

void CRenderManager::Render()
{
	// m_DepthDisable->SetState();

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

	// GBuffer를 만들어낸다.
	RenderGBuffer();
	
	// 조명 누적 버퍼를 만들어낸다.
	RenderLightAcc();

	// 조명 누적버퍼와 GBuffer를 이용하여 최종화면을 만들어낸다.
	RenderLightBlend();

	// 만들어진 최종 화면을 백버퍼에 그려낸다.
	RenderFinalScreen();

	// 조명 정보를 Shader로 넘겨준다.
	// CSceneManager::GetInst()->GetScene()->GetLightManager()->SetShader();

	/*{
		auto iter = m_RenderLayerList.begin();
		auto iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; iter++)
		{
			for (int i = 0; i < (*iter)->renderCount; i++)
			{
				(*iter)->renderList[i]->Render();
			}
		}
	}*/

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

	// m_DepthDisable->ResetState();

	m_DepthDisable->SetState();

	// UI, Widget 출력
	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// 디버깅용 렌더타겟을 출력한다.
	CResourceManager::GetInst()->RenderTarget();


	// 마우스 출력
	CWidgetWindow* mouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (mouseWidget)
		mouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::RenderGBuffer()
{
	// 현재 백버퍼로 렌더타겟이 지정되어 있다.
	// 이를 GBuffer 렌더타겟으로 교체해야 한다.
	std::vector<ID3D11RenderTargetView*> vecTarget;
	std::vector<ID3D11RenderTargetView*> vecPrevTarget;
	ID3D11DepthStencilView* prevDepthTarget = nullptr;

	size_t GBufferSize = m_vecGBuffer.size();

	vecPrevTarget.resize(GBufferSize);

	for (size_t i = 0; i < GBufferSize; i++)
	{
		m_vecGBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecGBuffer[i]->GetTargetView());
	}

	// 현재 지정되어 있는 렌더타겟과 깊이 타겟을 가져온다.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize, &vecPrevTarget[0],
		&prevDepthTarget);
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize, &vecTarget[0],
		prevDepthTarget);

	// Back, Default 레이어에 들어있는 불투명 요소들만 렌더타겟에 렌더
	for (size_t i = 0; i <= 1; i++)
	{
		for (int j = 0; j < m_RenderLayerList[i]->renderCount; j++)
		{
			m_RenderLayerList[i]->renderList[j]->Render();
		}
	}

	// 다시 원래 렌더 타겟으로 돌아간다.
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize, &vecPrevTarget[0],
		prevDepthTarget);

	SAFE_RELEASE(prevDepthTarget);

	for (size_t i = 0; i < GBufferSize; i++)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderLightAcc()
{
	std::vector<ID3D11RenderTargetView*> vecTarget;
	std::vector<ID3D11RenderTargetView*> vecPrevTarget;
	ID3D11DepthStencilView* prevDepthTarget = nullptr;

	size_t lightBufferSize = m_vecLightBuffer.size();

	vecPrevTarget.resize(lightBufferSize);

	for (size_t i = 0; i < lightBufferSize; i++)
	{
		m_vecLightBuffer[i]->ClearTarget();
		vecTarget.push_back(m_vecLightBuffer[i]->GetTargetView());
	}

	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)lightBufferSize, 
		&vecPrevTarget[0], &prevDepthTarget);

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)lightBufferSize, 
		&vecTarget[0], prevDepthTarget);

	// 먼저 그려내기전 블렌드 스테이트를 설정한다.
	// 광원 누적을 위해서 만들어는 스테이트를 설정하고
	m_LightAccBlend->SetState();

	// 깊이 버퍼에 쓰기를 금지하여 다른 물체를 덮는 것을 방지한다.
	m_DepthDisable->SetState();

	// 미리 계산되어진 G버퍼의 값을 전달한다.
	m_vecGBuffer[0]->SetTargetShader(14);
	m_vecGBuffer[1]->SetTargetShader(15);
	m_vecGBuffer[2]->SetTargetShader(16);
	m_vecGBuffer[3]->SetTargetShader(17);

	CSceneManager::GetInst()->GetScene()->GetLightManager()->Render();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecGBuffer[1]->ResetTargetShader(15);
	m_vecGBuffer[2]->ResetTargetShader(16);
	m_vecGBuffer[3]->ResetTargetShader(17);

	m_DepthDisable->ResetState();

	m_LightAccBlend->ResetState();

	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)lightBufferSize, 
		&vecPrevTarget[0], prevDepthTarget);

	SAFE_RELEASE(prevDepthTarget);

	for (size_t i = 0; i < lightBufferSize; i++)
	{
		SAFE_RELEASE(vecPrevTarget[i]);
	}
}

void CRenderManager::RenderLightBlend()
{
	CRenderTarget* finalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	finalScreenTarget->ClearTarget();

	// 깊이 버퍼는 이전 버퍼 그대로 이용
	finalScreenTarget->SetTarget(nullptr);

	// G버퍼 0번에는 디퓨즈 텍스쳐의 정보가 저장되어 있으며 이걸 14번 레지스터 버퍼로 보낸다.
	m_vecGBuffer[0]->SetTargetShader(14);
	// 누적된 조명의 결과를 18(Diffuse), 19(Specular), 20(Emissive)로 전달
	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);

	// 최종적으로 색을 혼합하기 위해서 블렌드 쉐이더를 이용한다.
	m_LightBlendShader->SetShader();

	m_DepthDisable->SetState();

	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_DepthDisable->ResetState();

	m_vecGBuffer[0]->ResetTargetShader(14);
	m_vecLightBuffer[0]->ResetTargetShader(18);
	m_vecLightBuffer[1]->ResetTargetShader(19);
	m_vecLightBuffer[2]->ResetTargetShader(20);

	finalScreenTarget->ResetTarget();
}

void CRenderManager::RenderFinalScreen()
{
	CRenderTarget* finalScreenTarget = (CRenderTarget*)CResourceManager::GetInst()->FindTexture("FinalScreen");

	// 최종적으로 합쳐진 타겟의 정보를 21번 레지스터에 연결
	finalScreenTarget->SetTargetShader(21);

	m_LightBlendRenderShader->SetShader();

	m_DepthDisable->SetState();

	UINT Offset = 0;
	CDevice::GetInst()->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	CDevice::GetInst()->GetContext()->IASetVertexBuffers(0, 0, nullptr, nullptr, &Offset);
	CDevice::GetInst()->GetContext()->IASetIndexBuffer(nullptr, DXGI_FORMAT_UNKNOWN, 0);

	CDevice::GetInst()->GetContext()->Draw(4, 0);

	m_DepthDisable->ResetState();

	finalScreenTarget->ResetTargetShader(21);
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
