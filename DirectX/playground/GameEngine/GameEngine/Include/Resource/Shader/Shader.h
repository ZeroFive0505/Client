#pragma once

#include "../../Ref.h"

/// <summary>
/// ���̴��� �⺻�� �Ǵ� Ŭ����
/// </summary>
class CShader
	: public CRef
{
	friend class CShaderManager;

protected:
	CShader();
	CShader(const CShader& shader);
	virtual ~CShader() = 0;

protected:
	// ���̴��� Ÿ��
	Shader_Type m_Type;

public:
	Shader_Type GetShaderType() const
	{
		return m_Type;
	}

public:
	virtual bool Init() = 0;
	virtual void SetShader() = 0;
	virtual CShader* Clone() = 0;
};

