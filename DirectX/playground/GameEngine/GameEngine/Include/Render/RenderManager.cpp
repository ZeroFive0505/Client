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

	// �ش� �̸��� ���̾ �߰����� ���ߴٸ� �ٷ� ���ư���.
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

	// �⺻ ���̾� ����
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

	// ��ũ�� ���� ���̾� ����
	layer = new sRenderLayer;
	layer->name = "ScreenWidgetComponent";
	layer->layerPriority = 3;

	m_RenderLayerList.push_back(layer);

	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");
	m_LightAccBlend = m_RenderStateManager->FindRenderState("LightAcc");

	// ���۵� �������� Ÿ�� ����
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

	// ���� ó���� ����
	
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
			// ���⼭ ������Ʈ���� PrevRender�Լ��� ȣ���ϸ鼭 ��� ������Ʈ�� PrevRender�� ȣ���ϰ� �ȴ�.
			(*iter)->PrevRender();
		}
	}

	// GBuffer�� ������.
	RenderGBuffer();
	
	// ���� ���� ���۸� ������.
	RenderLightAcc();

	// ���� �������ۿ� GBuffer�� �̿��Ͽ� ����ȭ���� ������.
	RenderLightBlend();

	// ������� ���� ȭ���� ����ۿ� �׷�����.
	RenderFinalScreen();

	// ���� ������ Shader�� �Ѱ��ش�.
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

	// UI, Widget ���
	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// ������ ����Ÿ���� ����Ѵ�.
	CResourceManager::GetInst()->RenderTarget();


	// ���콺 ���
	CWidgetWindow* mouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (mouseWidget)
		mouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::RenderGBuffer()
{
	// ���� ����۷� ����Ÿ���� �����Ǿ� �ִ�.
	// �̸� GBuffer ����Ÿ������ ��ü�ؾ� �Ѵ�.
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

	// ���� �����Ǿ� �ִ� ����Ÿ�ٰ� ���� Ÿ���� �����´�.
	CDevice::GetInst()->GetContext()->OMGetRenderTargets((unsigned int)GBufferSize, &vecPrevTarget[0],
		&prevDepthTarget);
	CDevice::GetInst()->GetContext()->OMSetRenderTargets((unsigned int)GBufferSize, &vecTarget[0],
		prevDepthTarget);

	// Back, Default ���̾ ����ִ� ������ ��ҵ鸸 ����Ÿ�ٿ� ����
	for (size_t i = 0; i <= 1; i++)
	{
		for (int j = 0; j < m_RenderLayerList[i]->renderCount; j++)
		{
			m_RenderLayerList[i]->renderList[j]->Render();
		}
	}

	// �ٽ� ���� ���� Ÿ������ ���ư���.
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

	// ���� �׷������� ���� ������Ʈ�� �����Ѵ�.
	// ���� ������ ���ؼ� ������ ������Ʈ�� �����ϰ�
	m_LightAccBlend->SetState();

	// ���� ���ۿ� ���⸦ �����Ͽ� �ٸ� ��ü�� ���� ���� �����Ѵ�.
	m_DepthDisable->SetState();

	// �̸� ���Ǿ��� G������ ���� �����Ѵ�.
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

	// ���� ���۴� ���� ���� �״�� �̿�
	finalScreenTarget->SetTarget(nullptr);

	// G���� 0������ ��ǻ�� �ؽ����� ������ ����Ǿ� ������ �̰� 14�� �������� ���۷� ������.
	m_vecGBuffer[0]->SetTargetShader(14);
	// ������ ������ ����� 18(Diffuse), 19(Specular), 20(Emissive)�� ����
	m_vecLightBuffer[0]->SetTargetShader(18);
	m_vecLightBuffer[1]->SetTargetShader(19);
	m_vecLightBuffer[2]->SetTargetShader(20);

	// ���������� ���� ȥ���ϱ� ���ؼ� ���� ���̴��� �̿��Ѵ�.
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

	// ���������� ������ Ÿ���� ������ 21�� �������Ϳ� ����
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
