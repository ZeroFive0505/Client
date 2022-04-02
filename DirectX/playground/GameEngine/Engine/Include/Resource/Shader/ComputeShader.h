#pragma once

#include "Shader.h"

/// <summary>
/// ��ǻƮ ���̴��� �Ϲ����� �������� �������� �ʴ´�.
/// GPU�� ��Ƽ�����带 �̿��Ͽ� ������ ����� �ϱ� ���� ��������
/// ȭ�鿡 ������ �غ� ���� �������� �̿��̵ȴ�. (��ü�� �̵� ���..)
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

