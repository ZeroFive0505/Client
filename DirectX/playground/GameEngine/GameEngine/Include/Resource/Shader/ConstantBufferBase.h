#pragma once

#include "../../GameInfo.h"

/// <summary>
/// 상수버퍼의 기본 클래스 형태
/// </summary>
class CConstantBufferBase
{
public:
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& buffer);
	virtual ~CConstantBufferBase();

protected:
	// 멤버변수로 상수버퍼를 가지고 있다.
	CSharedPtr<class CConstantBuffer> m_Buffer;

protected:
	void SetConstantBuffer(const std::string& name);

public:
	virtual bool Init() = 0;
	virtual void UpdateCBuffer() = 0;
	virtual CConstantBufferBase* Clone() = 0;
};

