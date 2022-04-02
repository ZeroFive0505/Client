#pragma once

#include "../../GameInfo.h"

/// <summary>
/// ���ҽ��Ŵ����� ���̴� �Ŵ����� �����ؼ� ���̴��� ����� �����Ѵ�.
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
	// ��� Ÿ���� ���̴��� ���� �� �ֵ��� ���ø��� �̿��Ѵ�.
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

