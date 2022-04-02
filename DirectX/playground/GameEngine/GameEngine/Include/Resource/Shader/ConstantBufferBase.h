#pragma once

#include "../../GameInfo.h"

/// <summary>
/// ��������� �⺻ Ŭ���� ����
/// </summary>
class CConstantBufferBase
{
public:
	CConstantBufferBase();
	CConstantBufferBase(const CConstantBufferBase& buffer);
	virtual ~CConstantBufferBase();

protected:
	// ��������� ������۸� ������ �ִ�.
	CSharedPtr<class CConstantBuffer> m_Buffer;

protected:
	void SetConstantBuffer(const std::string& name);

public:
	virtual bool Init() = 0;
	virtual void UpdateCBuffer() = 0;
	virtual CConstantBufferBase* Clone() = 0;
};

