#pragma once

#include "Shader.h"

/// <summary>
/// 컴퓨트 쉐이더는 일반적인 렌더링에 사용되지는 않는다.
/// GPU의 멀티스레드를 이용하여 빠르게 계산을 하기 위한 목적으로
/// 화면에 렌더링 준비를 위한 목적으로 이용이된다. (물체의 이동 등등..)
/// </summary>
class CComputeShader :
	public CShader
{
	friend class CShaderManager;
protected:
	CComputeShader();
	CComputeShader(const CComputeShader& shader);
	virtual ~CComputeShader() = 0;

private:
	ID3D11ComputeShader* m_CS;
	ID3DBlob* m_CSBlob;
	char m_EntryName[64];
	TCHAR m_FileName[MAX_PATH];
	std::string m_PathName;

public:
	virtual bool Init() = 0;
	virtual void SetShader();
	virtual CComputeShader* Clone();

public:
	bool LoadComputeShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	void Execute(unsigned int x, unsigned int y, unsigned int z);
};

