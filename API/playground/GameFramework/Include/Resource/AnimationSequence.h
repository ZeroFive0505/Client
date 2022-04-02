#pragma once

#include "Texture.h"

/// <summary>
/// 애니메이션 시퀀스 관리 클래스
/// </summary>
class CAnimationSequence :
	public CRef
{
	// 애니메이션 시퀀스또한 리소스 매니저에 의해서 생성 및 관리가 된다.
	friend class CResourceManager;

private:
	CAnimationSequence();
	~CAnimationSequence();

private:
	// 해당 애니메이션의 이름
	std::string		m_Name;
	// 텍스쳐의 타입
	ETexture_Type	m_TextureType;
	// 텍스쳐
	CSharedPtr<CTexture>	m_Texture;
	// 각각 프레임에 대한 애니메이션 정보
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

	// 인덱스를 전달받아 현재 프레임의 시작위치와 크기등을 반환한다.
	const AnimationFrameData& GetFrameData(int Index) const
	{
		return m_vecFrameData[Index];
	}

	// 전제 프레임수를 반환한다.
	int GetFrameCount() const
	{
		return (int)m_vecFrameData.size();
	}

public:
	bool Init(const std::string& Name, CTexture* Texture);
	void AddFrameData(const Vector2& StartPos, const Vector2& Size);
	void AddFrameData(float PosX, float PosY, float SizeX, float SizeY);
};

