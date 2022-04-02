#include "Pipe.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"

CPipe::CPipe(const Vector2& pos, const EWallObjectType type, CRaceScene* scene, const std::string& name) :
	CWallObject(pos, 2.5f, 2.0f, 0.0f, MAP_ASSETS_WIDTH, MAP_ASSETS_HEIGHT, scene, name)
{
	m_Type = type;

	switch (m_Type)
	{
	case EWallObjectType::GREEN_PIPE:
		scene->GetSceneResoruce()->LoadTexture("GreenPipe", TEXT("WallObjects/GreenPipe.bmp"), TEXTURE_PATH);
		m_Texture = scene->GetSceneResoruce()->FindTexture("GreenPipe");
		m_Texture->SetColorKey(255, 0, 255);
		m_Texture->SetPivot((float)m_Texture->GetWidth() / 2.0f, (float)m_Texture->GetHeight());
		break;
	case EWallObjectType::ORNAGE_PIPE:
		scene->GetSceneResoruce()->LoadTexture("OrangePipe", TEXT("WallObjects/OrangePipe.bmp"), TEXTURE_PATH);
		m_Texture = scene->GetSceneResoruce()->FindTexture("OrangePipe");
		m_Texture->SetColorKey(255, 0, 255);
		m_Texture->SetPivot((float)m_Texture->GetWidth() / 2.0f, (float)m_Texture->GetHeight());
		break;
	}
}

CTexture* CPipe::GetTexture()
{
	CTexture* cloneTexture = m_Texture->Clone();
	cloneTexture->SetColorKey(255, 0, 255);
	cloneTexture->SetPivot((float)cloneTexture->GetWidth() / 2.0f, (float)cloneTexture->GetHeight());

	return cloneTexture;
}
