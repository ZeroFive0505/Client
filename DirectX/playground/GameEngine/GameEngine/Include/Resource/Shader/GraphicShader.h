#pragma once

#include "Shader.h"

/// <summary>
/// ȭ�鿡 ������ �ɶ� �̿�Ǵ� ���̴��̴�.
/// </summary>
class CGraphicShader :
	public CShader
{
	friend class CShaderManager;

protected:
	CGraphicShader();
	CGraphicShader(const CGraphicShader& shader);
	virtual ~CGraphicShader() = 0;

private:
	// ���̴� �Է½� ���̾ƿ��� ����
	ID3D11InputLayout* m_InputLayout;
	// ���ؽ����� �پ��� �Է� ���̾ƿ��� ���� �� �ִ�.
	std::vector<D3D11_INPUT_ELEMENT_DESC> m_vecInputDesc;
	unsigned int m_InputSize;

private:
	// ���ؽ� ���̴�
	ID3D11VertexShader* m_VS;
	// ���ؽ� ���̴� ����Ʈ �ڵ�
	ID3DBlob* m_VSBlob;

	// �ȼ� ���̴�
	ID3D11PixelShader* m_PS;
	// �ȼ� ���̴� ����Ʈ �ڵ�
	ID3DBlob* m_PSBlob;

	// �� ���̴�
	ID3D11HullShader* m_HS;
	// �� ���̴� ����Ʈ �ڵ�
	ID3DBlob* m_HSBlob;

	// ������ ���̴�
	ID3D11DomainShader* m_DS;
	// ������ ���̴� ����Ʈ �ڵ�
	ID3DBlob* m_DSBlob;

	// ������Ʈ�� ���̴�
	ID3D11GeometryShader* m_GS;
	// ������Ʈ�� ���̴� ����Ʈ �ڵ�
	ID3DBlob* m_GSBlob;

public:
	void AddInputDesc(const char* name, unsigned int index, DXGI_FORMAT fmt, unsigned int slot, unsigned int size,
		D3D11_INPUT_CLASSIFICATION slotClass, unsigned int instanceDataStep);

	bool CreateInputLayout();

public:
	virtual bool Init() = 0;
	virtual void SetShader();
	virtual CGraphicShader* Clone();

public:
	bool LoadVertexShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadPixelShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadHullShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadDomainShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
	bool LoadGeometryShader(const char* entryName, const TCHAR* fileName, const std::string& pathName);
};

