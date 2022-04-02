#pragma once

#include "../../Ref.h"

/// <summary>
/// 쉐이더의 기본이 되는 클래스
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
	// 쉐이더의 타입
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

