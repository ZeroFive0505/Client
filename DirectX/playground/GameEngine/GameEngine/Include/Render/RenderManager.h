#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture/RenderTarget.h"

struct sRenderLayer
{
	std::string name;
	int layerPriority;
	std::vector<class CSceneComponent*> renderList;
	int renderCount;

	sRenderLayer()
	{
		layerPriority = 0;
		renderCount = 0;
		renderList.resize(500);
	}
};

class CRenderManager
{
private:
	class CRenderStateManager* m_RenderStateManager;

private:
	const std::list<CSharedPtr<class CGameObject>>* m_ObjectList;
	std::vector<sRenderLayer*> m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaBlend;
	class CRenderState* m_LightAccBlend;
	CSharedPtr<class CShader> m_LightBlendShader;
	CSharedPtr<class CShader> m_LightBlendRenderShader;

	std::vector<CSharedPtr<CRenderTarget>> m_vecGBuffer;
	std::vector<CSharedPtr<CRenderTarget>> m_vecLightBuffer;

public:
	inline class CStandard2DConstantBuffer* GetStandard2DCBuffer() const
	{
		return m_Standard2DCBuffer;
	}

public:
	inline void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* objList)
	{
		m_ObjectList = objList;
	}

	void AddRenderList(class CSceneComponent* component);
	void CreateLayer(const std::string& name, int priority);
	void SetLayerPriority(const std::string& name, int priority);

public:
	bool Init();
	void Render();

private:
	void RenderGBuffer();
	void RenderLightAcc();
	void RenderLightBlend();
	void RenderFinalScreen();

public:
	void SetBlendFactor(const std::string& name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
	class CRenderState* FindRenderState(const std::string& name);

public:
	static bool SortLayer(sRenderLayer* src, sRenderLayer* dest);

	DECLARE_SINGLE(CRenderManager)
};

