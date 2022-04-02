#pragma once

#include "../../GameInfo.h"

/// <summary>
/// 리소스매니저로 쉐이더 매니저에 접근해서 쉐이더를 만들고 관리한다.
/// </summary>
class CShaderManager
{
	friend class CResourceManager;

private:
	CShaderManager();
	~CShaderManager();

private:
	std::unordered_map<std::string, CSharedPtr<class CShader>> m_mapShader;
	std::unordered_map<std::string, CSharedPtr<class CConstantBuffer>> m_mapConstantBuffer;

public:
	bool Init();
	class CShader* FindShader(const std::string& name);
	void ReleaseShader(const std::string& name);

	bool CreateConstantBuffer(const std::string& name, int size, int registerNum,
		int constantBufferShaderType = (int)Buffer_Shader_Type::All);
	class CConstantBuffer* FindConstantBuffer(const std::string& name);

public:
	// 어떠한 타입의 쉐이더도 만들 수 있도록 템플릿을 이용한다.
	template <typename T>
	bool CreateShader(const std::string& name)
	{
		T* shader = (T*)FindShader(name);

		if (shader)
			return false;

		shader = new T;

		shader->SetName(name);

		if (!shader->Init())
		{
			SAFE_RELEASE(shader);
			return false;
		}

		m_mapShader.insert(std::make_pair(name, shader));

		return true;
	}
};

