#pragma once

#include "ObjectComponent.h"
#include "../Resource/Shader/PaperBurnConstantBuffer.h"
#include "../Resource/Texture/Texture.h"

class CPaperBurnComponent :
	public CObjectComponent
{
	friend class CGameObject;

protected:
	CPaperBurnComponent();
	CPaperBurnComponent(const CPaperBurnComponent& com);
	virtual ~CPaperBurnComponent();

private:
	CPaperBurnConstantBuffer* m_CBuffer;
	std::function<void()> m_FinishCallback;
	float m_FinishTime;
	CSharedPtr<class CMaterial> m_Material;
	CSharedPtr<CTexture> m_BurnTexture;
	bool m_StartPaperBurn;
	float m_Filter;
	
public:
	void StartPaperBurn();
	void SetMaterial(class CMaterial* material);
	void SetInverse(bool enable);

	inline void SetFinishTime(float time)
	{
		m_FinishTime = time;
	}

	void SetBurnTexture(const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);

	void SetOutLineColor(const Vector4& color);
	void SetOutLineColor(float r, float g, float b, float a);

	void SetInLineColor(const Vector4& color);
	void SetInLineColor(float r, float g, float b, float a);

	void SetCenterLineColor(const Vector4& color);
	void SetCenterLineColor(float r, float g, float b, float a);

	void SetInFilter(float filter);
	void SetOutFilter(float filter);

	void SetCenterFilter(float filter);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CPaperBurnComponent* Clone();

public:
	void SetShader();

public:
	template <typename T>
	void SetFinishCallback(T* obj, void(T::* func)())
	{
		m_FinishCallback = std::bind(func, obj);
	}
};

