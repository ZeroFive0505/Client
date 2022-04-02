#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Object/EditorObserver.h"

CDefaultScene::CDefaultScene()
{
	SetTypeID<CDefaultScene>();
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
	if (!LoadAnimationSequence2D())
		return false;

	CreateMaterial();

	// CEditorObserver* observer = m_Scene->CreateGameObject<CEditorObserver>("Observer");

	// SetPlayerObject(observer);

	return true;
}

bool CDefaultScene::LoadAnimationSequence2D()
{
	// ���� ������ ���� �������� �����̸����� �ҷ��´�.

	/*if (!m_Scene->GetSceneResource()->LoadSequence2D("Idle.sqc"))
		return false;

	if (!m_Scene->GetSceneResource()->LoadSequence2D("Walk.sqc"))
		return false;*/

	return true;
}

bool CDefaultScene::CreateMaterial()
{
	m_Scene->GetSceneResource()->CreateMaterial<CMaterial>("TileMap");
	CMaterial* material = m_Scene->GetSceneResource()->FindMaterial("TileMap");

	material->SetShader("TileMapShader");
	material->SetRenderState("AlphaBlend");

	return true;
}
