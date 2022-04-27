#pragma once

#include "../Shader/GraphicShader.h"
#include "../Texture/Texture.h"
#include "../Shader/MaterialConstantBuffer.h"

// 마테리얼이 가지고 있는 텍스처의 정보
struct sMaterialTextureInfo
{
	std::string name;
	Sampler_Type samplerType;
	CSharedPtr<CTexture> texture;
	int registerNum;
	int shaderType;

	sMaterialTextureInfo() :
		samplerType(Sampler_Type::Linear),
		registerNum(0),
		shaderType((int)Buffer_Shader_Type::Pixel)
	{

	}
};

// Dissolve 효과등을 이용할때 
// 세이더 설정을 위한 콜백 함수 구조체
struct sRenderCallback
{
	std::function<void()> func;
	void* obj;
};

/// <summary>
/// 쉐이더를 가지고있는 재질 클래스
/// </summary>
class CMaterial :
	public CRef
{
	friend class CMaterialManager;
	friend class CMesh;
	friend class CStaticMeshComponent;
	friend class CAnimationMeshComponent;

protected:
	CMaterial();
	CMaterial(const CMaterial& material);
	virtual ~CMaterial();

private:
	class CScene* m_Scene;

public:
	inline void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

protected:
	CSharedPtr<CGraphicShader> m_Shader;
	std::vector<sMaterialTextureInfo> m_vecTextureInfo;
	Vector4 m_BaseColor;
	Vector4 m_AmbientColor;
	Vector4 m_SpecularColor;
	Vector4 m_EmissiveColor;
	float m_Opacity;
	bool m_Animation3D;
	bool m_SpecularTex;
	bool m_EmissiveTex;
	bool m_Bump;
	class CMaterialConstantBuffer* m_CBuffer;
	CSharedPtr<class CRenderState> m_RenderStateArray[(int)RenderState_Type::MAX];
	std::list<sRenderCallback*> m_RenderCallback;

private:
	inline void SetConstantBuffer(CMaterialConstantBuffer* buffer)
	{
		// 마테리얼마다 Dissolve여부를 정해야하기에 복사.
		m_CBuffer = buffer->Clone();
	}

	void CreateConstantBuffer();

public:
	inline CTexture* GetTexture(int index = 0) const
	{
		return m_vecTextureInfo[index].texture;
	}

	inline Vector4 GetBaseColor() const
	{
		return m_BaseColor;
	}

	inline unsigned int GetTextureWidth(int index = 0) const
	{
		return m_vecTextureInfo[index].texture->GetWidth();
	}

	inline unsigned int GetTextureHeight(int index = 0) const
	{
		return m_vecTextureInfo[index].texture->GetHeight();
	}

	inline bool HasTexture() const
	{
		return !m_vecTextureInfo.empty();
	}

	void EnableBump();
	void EnableAnimation3D();
	void EnableSpecularTex();
	void EnableEmissiveTex();

public:
	void SetRenderState(class CRenderState* state);
	void SetRenderState(const std::string& name);
	void SetTransparency(bool enable);
	void SetOpacity(float opacity);
	void AddOpacity(float opacity);

public:
	void SetBaseColor(const Vector4& color);
	void SetBaseColor(float r, float g, float b, float a);
	void SetAmbientColor(const Vector4& color);
	void SetAmbientColor(float r, float g, float b, float a);
	void SetSpecularColor(const Vector4& color);
	void SetSpecularColor(float r, float g, float b, float a);
	void SetEmissiveColor(const Vector4& color);
	void SetEmissiveColor(float r, float g, float b, float a);
	void SetSpecularPower(float power);

public:
	void AddTexture(int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void AddTexture(int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void AddTextureFullPath(int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void AddTexture(int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, class CTexture* texture);
	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fileName, const std::string& pathName = TEXTURE_PATH);
	void SetTextureFullPath(int index, int registerNum, int shaderType, const std::string& name, const TCHAR* fullPath);
	void SetTexture(int index, int registerNum, int shaderType, const std::string& name, const std::vector<TCHAR*>& vecFileName, const std::string& pathName = TEXTURE_PATH);

public:
	void SetPaperBurn(bool enable);

public:
	void SetShader(const std::string& name);
	void Render();
	void Reset();
	CMaterial* Clone();
	void Save(FILE* pFile);
	void Load(FILE* pFile);

public:
	template <typename T>
	void AddRenderCallback(T* obj, void(T::* func)())
	{
		sRenderCallback* callBack = new sRenderCallback;
		callBack->obj = obj;
		callBack->func = std::bind(func, obj);
		m_RenderCallback.push_back(callBack);
	}

	void DeleteRenderCallback(void* obj)
	{
		auto iter = m_RenderCallback.begin();
		auto iterEnd = m_RenderCallback.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->obj == obj)
			{
				SAFE_DELETE(*iter);
				m_RenderCallback.erase(iter);
				break;
			}
		}
	}
};

