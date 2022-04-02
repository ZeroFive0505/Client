#include "AnimationSequence.h"

CAnimationSequence::CAnimationSequence()
{
}

CAnimationSequence::~CAnimationSequence()
{
}

bool CAnimationSequence::Init(const std::string& name, CTexture* texture)
{
	m_Name = name;
	m_Texture = texture;
	m_TextureType = m_Texture->GetTextureType();

	return true;
}

void CAnimationSequence::AddFrameData(const Vector2& startPos, const Vector2& size)
{
	AnimationFrameData data;
	data.startPos = startPos;
	data.size = size;

	m_vecFrameData.push_back(data);
}

void CAnimationSequence::AddFrameData(float posX, float posY, float sizeX, float sizeY)
{
	AnimationFrameData data;
	data.startPos = Vector2(posX, posY);
	data.size = Vector2(sizeX, sizeY);

	m_vecFrameData.push_back(data);
}
