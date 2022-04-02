#pragma once

#include "Texture.h"

class CAnimationSequence :
	public CRef
{
	friend class CResourceManager;

private:
	CAnimationSequence();
	~CAnimationSequence();

private:
	ETexture_Type m_TextureType;
	CSharedPtr<CTexture> m_Texture;
	std::vector<AnimationFrameData> m_vecFrameData;

public:
	inline CTexture* GetTexture() const
	{
		return m_Texture;
	}

	inline ETexture_Type GetTextureType() const
	{
		return m_TextureType;
	}

	inline const AnimationFrameData& GetFrameData(int index) const
	{
		return m_vecFrameData[index];
	}

	inline int GetFrameCount() const
	{
		return (int)m_vecFrameData.size();
	}

public:
	bool Init(const std::string& name, CTexture* texture);
	void AddFrameData(const Vector2& startPos, const Vector2& size);
	void AddFrameData(float posX, float posY, float sizeX, float sizeY);
};

