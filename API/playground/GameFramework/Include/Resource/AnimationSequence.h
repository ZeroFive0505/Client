#pragma once

#include "Texture.h"

/// <summary>
/// �ִϸ��̼� ������ ���� Ŭ����
/// </summary>
class CAnimationSequence :
	public CRef
{
	// �ִϸ��̼� ���������� ���ҽ� �Ŵ����� ���ؼ� ���� �� ������ �ȴ�.
	friend class CResourceManager;

private:
	CAnimationSequence();
	~CAnimationSequence();

private:
	// �ش� �ִϸ��̼��� �̸�
	std::string		m_Name;
	// �ؽ����� Ÿ��
	ETexture_Type	m_TextureType;
	// �ؽ���
	CSharedPtr<CTexture>	m_Texture;
	// ���� �����ӿ� ���� �ִϸ��̼� ����
	std::vector<AnimationFrameData>	m_vecFrameData;

public:
	const std::string& GetName() const
	{
		return m_Name;
	}

	CTexture* GetTexture()
	{
		return m_Texture;
	}

	ETexture_Type GetTextureType() const
	{
		return m_TextureType;
	}

	// �ε����� ���޹޾� ���� �������� ������ġ�� ũ����� ��ȯ�Ѵ�.
	const AnimationFrameData& GetFrameData(int Index) const
	{
		return m_vecFrameData[Index];
	}

	// ���� �����Ӽ��� ��ȯ�Ѵ�.
	int GetFrameCount() const
	{
		return (int)m_vecFrameData.size();
	}

public:
	bool Init(const std::string& Name, CTexture* Texture);
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
	void AddFrameData(float PosX, float PosY, float SizeX, float SizeY);
};

