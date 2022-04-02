#include "EditorScene.h"
#include "SceneResource.h"
#include "EditorDlg.h"
#include "../Input.h"
#include "../Map/TileMap.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Scene/SceneManager.h"
#include "../Scene/TitleScene.h"

CEditorScene::CEditorScene() :
	m_Dlg(nullptr),
	m_Start(false),
	m_TileMap(nullptr),
	m_ScrollSpeed(200.0f),
	m_RefImage(nullptr),
	m_FixX(nullptr),
	m_FixY(nullptr)
{
	CGameManager::GetInst()->SetEditorMode(true);
	CGameManager::GetInst()->SetShowGrid(true);
}

CEditorScene::~CEditorScene()
{
	CGameManager::GetInst()->SetEditorMode(false);
	CGameManager::GetInst()->SetShowGrid(false);
	SAFE_DELETE(m_Dlg);
}

bool CEditorScene::Init()
{
	m_Dlg = new CEditorDlg;

	m_Dlg->SetScene(this);

	m_Dlg->Init(IDD_DIALOG_EDITOR);

	GetSceneResoruce()->LoadTexture("GreenPipe", TEXT("WallObjects/GreenPipe.bmp"));

	m_GreenPipe = GetSceneResoruce()->FindTexture("GreenPipe");

	m_GreenPipe->SetColorKey(255, 0, 255);

	m_GreenPipe->SetPivot((float)m_GreenPipe->GetWidth() / 2.0f, (float)m_GreenPipe->GetHeight());

	GetSceneResoruce()->LoadTexture("OrangePipe", TEXT("WallObjects/OrangePipe.bmp"));

	m_OrangePipe = GetSceneResoruce()->FindTexture("OrangePipe");

	m_OrangePipe->SetColorKey(255, 0, 255);

	m_OrangePipe->SetPivot((float)m_OrangePipe->GetWidth() / 2.0f, (float)m_OrangePipe->GetHeight());

	GetSceneResoruce()->LoadTexture("Super Thwomp", TEXT("WallObjects/SuperThwomp.bmp"));

	m_SuperThwomp = GetSceneResoruce()->FindTexture("Super Thwomp");

	m_SuperThwomp->SetColorKey(255, 0, 255);

	m_SuperThwomp->SetPivot((float)m_SuperThwomp->GetWidth() / 2.0f, (float)m_SuperThwomp->GetHeight());

	GetSceneResoruce()->LoadTexture("Thwomp", TEXT("WallObjects/Thwomp.bmp"));

	m_Thwomp = GetSceneResoruce()->FindTexture("Thwomp");

	m_Thwomp->SetColorKey(255, 0, 255);

	m_Thwomp->SetPivot((float)m_Thwomp->GetWidth() / 2.0f, (float)m_Thwomp->GetHeight());

	GetSceneResoruce()->LoadTexture("Flower", TEXT("WallObjects/Flower_1.bmp"));

	m_Flower = GetSceneResoruce()->FindTexture("Flower");

	m_Flower->SetColorKey(255, 0, 255);

	m_Flower->SetPivot((float)m_Flower->GetWidth() / 2.0f, (float)m_Flower->GetHeight());

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			wsprintf(filePath, TEXT("FloorObjects/ItemBoxUp.bmp"));
			name = "ItemBoxUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			wsprintf(filePath, TEXT("FloorObjects/ItemBoxRight.bmp"));
			name = "ItemBoxRight";
			break;
		case EFloorObjectOrientation::DOWN:
			wsprintf(filePath, TEXT("FloorObjects/ItemBoxDown.bmp"));
			name = "ItemBoxDown";
			break;
		case EFloorObjectOrientation::LEFT:
			wsprintf(filePath, TEXT("FloorObjects/ItemBoxLeft.bmp"));
			name = "ItemBoxLeft";
			break;
		}

		GetSceneResoruce()->LoadTexture(name, filePath, TEXTURE_PATH);

		m_ItemBoxes[i] = GetSceneResoruce()->FindTexture(name);

		m_ItemBoxes[i]->SetColorKey(255, 0, 255);
	}

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			wsprintf(filePath, TEXT("FloorObjects/CoinUp.bmp"));
			name = "CoinUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			wsprintf(filePath, TEXT("FloorObjects/CoinRight.bmp"));
			name = "CoinRight";
			break;
		case EFloorObjectOrientation::DOWN:
			wsprintf(filePath, TEXT("FloorObjects/CoinDown.bmp"));
			name = "CoinDown";
			break;
		case EFloorObjectOrientation::LEFT:
			wsprintf(filePath, TEXT("FloorObjects/CoinLeft.bmp"));
			name = "CoinLeft";
			break;
		}

		GetSceneResoruce()->LoadTexture(name, filePath, TEXTURE_PATH);

		m_Coins[i] = GetSceneResoruce()->FindTexture(name);

		m_Coins[i]->SetColorKey(255, 0, 255);
	}

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			wsprintf(filePath, TEXT("FloorObjects/OilUp.bmp"));
			name = "OilUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			wsprintf(filePath, TEXT("FloorObjects/OilRight.bmp"));
			name = "OilRight";
			break;
		case EFloorObjectOrientation::DOWN:
			wsprintf(filePath, TEXT("FloorObjects/OilDown.bmp"));
			name = "OilDown";
			break;
		case EFloorObjectOrientation::LEFT:
			wsprintf(filePath, TEXT("FloorObjects/OilLeft.bmp"));
			name = "OilLeft";
			break;
		}

		GetSceneResoruce()->LoadTexture(name, filePath, TEXTURE_PATH);

		m_Oils[i] = GetSceneResoruce()->FindTexture(name);

		m_Oils[i]->SetColorKey(255, 0, 255);
	}

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		TCHAR filePath[MAX_PATH] = {};
		std::string name;
		switch ((EFloorObjectOrientation)i)
		{
		case EFloorObjectOrientation::UP:
			wsprintf(filePath, TEXT("FloorObjects/AccelUp.bmp"));
			name = "AccelUp";
			break;
		case EFloorObjectOrientation::RIGHT:
			wsprintf(filePath, TEXT("FloorObjects/AccelRight.bmp"));
			name = "AccelRight";
			break;
		case EFloorObjectOrientation::DOWN:
			wsprintf(filePath, TEXT("FloorObjects/AccelDown.bmp"));
			name = "AccelDown";
			break;
		case EFloorObjectOrientation::LEFT:
			wsprintf(filePath, TEXT("FloorObjects/AccelLeft.bmp"));
			name = "AccelLeft";
			break;
		}

		GetSceneResoruce()->LoadTexture(name, filePath, TEXTURE_PATH);

		m_AccelPanels[i] = GetSceneResoruce()->FindTexture(name);

		m_AccelPanels[i]->SetColorKey(255, 0, 255);
	}


	return true;
}

bool CEditorScene::Update(float deltaTime)
{
	if (!m_Start)
	{
		m_Start = true;
		CInput::GetInst()->SetEditorKeySet();

		CInput::GetInst()->SetCallback<CEditorScene>("Editor", KeyState_Down, this, &CEditorScene::OnEditor);

		CInput::GetInst()->SetCallback<CEditorScene>("MoveUp", KeyState_Hold, this, &CEditorScene::CameraMoveUp);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveDown", KeyState_Hold, this, &CEditorScene::CameraMoveDown);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveLeft", KeyState_Hold, this, &CEditorScene::CameraMoveLeft);
		CInput::GetInst()->SetCallback<CEditorScene>("MoveRight", KeyState_Hold, this, &CEditorScene::CameraMoveRight);

		CInput::GetInst()->SetCallback<CEditorScene>("MouseLButton", KeyState_Hold, this, &CEditorScene::MouseLButton);
		CInput::GetInst()->SetCallback<CEditorScene>("MouseRButton", KeyState_Hold, this, &CEditorScene::MouseRButton);

		CInput::GetInst()->SetCallback<CEditorScene>("Instantiate", KeyState_Down, this, &CEditorScene::InstantiateObject);
		CInput::GetInst()->SetCallback<CEditorScene>("DeleteObject", KeyState_Down, this, &CEditorScene::DeleteObject);

		CInput::GetInst()->SetCallback<CEditorScene>("FixX", KeyState_Hold, this, &CEditorScene::SetFixX);
		CInput::GetInst()->SetCallback<CEditorScene>("FixX", KeyState_Up, this, &CEditorScene::ResetFixX);
		CInput::GetInst()->SetCallback<CEditorScene>("FixY", KeyState_Hold, this, &CEditorScene::SetFixY);
		CInput::GetInst()->SetCallback<CEditorScene>("FixY", KeyState_Up, this, &CEditorScene::ResetFixY);
		CInput::GetInst()->SetCallback<CEditorScene>("BackToTitle", KeyState_Down, this, &CEditorScene::BackToTitle);

		CInput::GetInst()->SetCallback<CEditorScene>("Hide", KeyState_Down, this, &CEditorScene::HideGrid);
	}


	return false;
}

bool CEditorScene::Render(HDC hDC)
{

	CCamera* camera = GetCamera();
	Vector2 camPos = camera->GetPos();

	if (m_RefImage)
		m_RefImage->Render(hDC, Vector2(0.0f, 0.0f), GetCamera()->GetPos(), 
			Vector2(m_RefImage->GetWidth() - camPos.x, m_RefImage->GetHeight() - camPos.y));

	if (m_TileMap)
	{
		{
			std::list<std::pair<EWallObjectType, Vector2>> wallObjects = m_TileMap->GetWallObjects();
			auto iter = wallObjects.begin();
			auto iterEnd = wallObjects.end();


			for (; iter != iterEnd; iter++)
			{
				switch (iter->first)
				{
					case EWallObjectType::GREEN_PIPE:
						m_GreenPipe->Render(hDC, iter->second - camPos, Vector2(0.0f, 0.0f), Vector2((float)m_GreenPipe->GetWidth(), (float)m_GreenPipe->GetHeight()));
						break;
					case EWallObjectType::ORNAGE_PIPE:
						m_OrangePipe->Render(hDC, iter->second - camPos, Vector2(0.0f, 0.0f), Vector2((float)m_OrangePipe->GetWidth(), (float)m_OrangePipe->GetHeight()));
						break;
					case EWallObjectType::SUPER_THWOMP:
						m_SuperThwomp->Render(hDC, iter->second - camPos, Vector2(0.0f, 0.0f), Vector2((float)m_SuperThwomp->GetWidth(), (float)m_SuperThwomp->GetHeight()));
						break;
					case EWallObjectType::THWOMP:
						m_Thwomp->Render(hDC, iter->second - camPos, Vector2(0.0f, 0.0f), Vector2((float)m_SuperThwomp->GetWidth(), (float)m_SuperThwomp->GetHeight()));
						break;
					case EWallObjectType::FLOWER:
						m_Flower->Render(hDC, iter->second - camPos, Vector2(0.0f, 0.0f), Vector2((float)m_Flower->GetWidth(), (float)m_Flower->GetHeight()));
						break;
				}
			}
		}


		{
			std::list<std::pair<std::pair<EFloorObjectType, Vector2>, EFloorObjectOrientation>> floorObjects = m_TileMap->GetFloorObjects();

			auto iter = floorObjects.begin();
			auto iterEnd = floorObjects.end();

			for (; iter != iterEnd; iter++)
			{
				switch (iter->first.first)
				{
				case EFloorObjectType::ACCEL_PANEL:
				{
					Vector2 size = Vector2((float)m_AccelPanels[(int)iter->second]->GetWidth(), (float)m_AccelPanels[(int)iter->second]->GetHeight());
					m_AccelPanels[(int)iter->second]->Render(hDC, iter->first.second - camPos, Vector2(0.0f, 0.0f), size);
				}
					break;
				case EFloorObjectType::ITEM_BOX:
				{
					Vector2 size = Vector2((float)m_ItemBoxes[(int)iter->second]->GetWidth(), (float)m_ItemBoxes[(int)iter->second]->GetHeight());
					m_ItemBoxes[(int)iter->second]->Render(hDC, iter->first.second - camPos, Vector2(0.0f, 0.0f), size);
				}
					break;
				case EFloorObjectType::OIL_SLICK:
				{
					Vector2 size = Vector2((float)m_Oils[(int)iter->second]->GetWidth(), (float)m_Oils[(int)iter->second]->GetHeight());
					m_Oils[(int)iter->second]->Render(hDC, iter->first.second - camPos, Vector2(0.0f, 0.0f), size);
				}
					break;
				case EFloorObjectType::COIN:
				{
					Vector2 size = Vector2((float)m_Coins[(int)iter->second]->GetWidth(), (float)m_Coins[(int)iter->second]->GetHeight());
					m_Coins[(int)iter->second]->Render(hDC, iter->first.second - camPos, Vector2(0.0f, 0.0f), size);
				}
					break;
				case EFloorObjectType::RAMP_HORIZONTAL:
					break;
				case EFloorObjectType::RAMP_VERTICAL:
					break;
				}
			}
		}
	}

	CScene::Render(hDC);

	return false;
}

bool CEditorScene::PostUpdate(float deltaTime)
{
	CScene::PostUpdate(deltaTime);


	return false;
}

void CEditorScene::OnEditor(float deltaTime)
{
	m_Dlg->Show();
}

void CEditorScene::CreateTileMap()
{
	if (m_TileMap)
		return;

	m_TileMap = CreateMap<CTileMap>("TileMap");
}

void CEditorScene::SetTileInfo(int countX, int countY, int sizeX, int sizeY)
{
	m_TileMap->CreateTile(countX, countY, Vector2((float)sizeX, (float)sizeY));
}

void CEditorScene::LoadRefImage(const std::string& name)
{
	m_RefImage = GetSceneResoruce()->FindTexture(name);
}

void CEditorScene::ClearRefImage()
{
	if (m_RefImage)
	{
		std::string name = m_RefImage->GetName();
		m_RefImage = nullptr;
		GetSceneResoruce()->ReleaseTexture(name);
	}
}

void CEditorScene::CameraMoveUp(float deltaTime)
{
	CCamera* camera = GetCamera();

	camera->Move(Vector2(0.0f, -1.0f) * m_ScrollSpeed * deltaTime);
}

void CEditorScene::CameraMoveDown(float deltaTime)
{
	CCamera* camera = GetCamera();

	camera->Move(Vector2(0.0f, 1.0f) * m_ScrollSpeed * deltaTime);
}

void CEditorScene::CameraMoveLeft(float deltaTime)
{
	CCamera* camera = GetCamera();

	camera->Move(Vector2(-1.0f, 0.0f) * m_ScrollSpeed * deltaTime);
}

void CEditorScene::CameraMoveRight(float deltaTime)
{
	CCamera* camera = GetCamera();

	camera->Move(Vector2(1.0f, 0.0f) * m_ScrollSpeed * deltaTime);
}

void CEditorScene::MouseLButton(float deltaTime)
{
	if (!m_TileMap)
		return;


	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	CCamera* camera = GetCamera();

	if (mousePos.x < 0.0f || mousePos.x > camera->GetResolution().x ||
		mousePos.y < 0.0f || mousePos.y > camera->GetResolution().y)
		return;

	ETileEditMode editMode = m_Dlg->GetTileEditMode();

	if (m_FixX)
		mousePos.x = m_FixXPos;

	if (m_FixY)
		mousePos.y = m_FixYPos;

	mousePos += camera->GetPos();

	switch (editMode)
	{
	case ETileEditMode::PROPERTY:
	{

		ETileProperty tileProperty = m_Dlg->GetTileProperty();

		m_TileMap->ChangeTileProperty(mousePos, tileProperty);

	}
	break;
	}
}

void CEditorScene::MouseRButton(float deltaTime)
{
	if (!m_TileMap)
		return;

	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	CCamera* camera = GetCamera();

	if (mousePos.x < 0.0f || mousePos.x > camera->GetResolution().x ||
		mousePos.y < 0.0f || mousePos.y > camera->GetResolution().y)
		return;

	ETileEditMode editMode = m_Dlg->GetTileEditMode();

	if (m_FixX)
		mousePos.x = m_FixXPos;

	if (m_FixY)
		mousePos.y = m_FixYPos;

	mousePos += camera->GetPos();

	switch (editMode)
	{
	case ETileEditMode::PROPERTY:
		m_TileMap->ChangeTileProperty(mousePos, ETileProperty::TRACK);
		break;
	}
}

void CEditorScene::InstantiateObject(float deltaTime)
{
	if (!m_TileMap)
		return;

	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	CCamera* camera = GetCamera();

	if (mousePos.x < 0.0f || mousePos.x > camera->GetResolution().x ||
		mousePos.y < 0.0f || mousePos.y > camera->GetResolution().y)
		return;

	ETileEditMode editMode = m_Dlg->GetTileEditMode();

	mousePos += camera->GetPos();

	switch (editMode)
	{
	case ETileEditMode::WALL:
	{
		EWallObjectType wallType = m_Dlg->GetWallType();

		m_TileMap->CreateWallObject(mousePos, wallType);
	}
		break;
	case ETileEditMode::FLOOR:
	{
		EFloorObjectType floorType = m_Dlg->GetFloorType();
		EFloorObjectOrientation orientation = m_Dlg->GetOrientation();

		m_TileMap->CreateFloorObject(mousePos, floorType, orientation);
	}
		break;
	}
}

void CEditorScene::DeleteObject(float deltaTime)
{
	if (!m_TileMap)
		return;

	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	CCamera* camera = GetCamera();

	if (mousePos.x < 0.0f || mousePos.x > camera->GetResolution().x ||
		mousePos.y < 0.0f || mousePos.y > camera->GetResolution().y)
		return;

	ETileEditMode editMode = m_Dlg->GetTileEditMode();

	mousePos += camera->GetPos();

	switch (editMode)
	{
	case ETileEditMode::WALL:
	{
		EWallObjectType wallType = m_Dlg->GetWallType();

		Vector2 size;
		Vector2 pivot;

		switch (wallType)
		{
		case EWallObjectType::GREEN_PIPE:
			size = Vector2((float)m_GreenPipe->GetWidth(), (float)m_GreenPipe->GetHeight());
			pivot = Vector2(m_GreenPipe->GetPivot().x, m_GreenPipe->GetPivot().y);
			break;
		case EWallObjectType::ORNAGE_PIPE:
			size = Vector2((float)m_OrangePipe->GetWidth(), (float)m_OrangePipe->GetHeight());
			pivot = Vector2(m_OrangePipe->GetPivot().x, m_OrangePipe->GetPivot().y);
			break;
		case EWallObjectType::SUPER_THWOMP:
			size = Vector2((float)m_SuperThwomp->GetWidth(), (float)m_SuperThwomp->GetHeight());
			pivot = Vector2(m_SuperThwomp->GetPivot().x, m_SuperThwomp->GetPivot().y);
			break;
		case EWallObjectType::THWOMP:
			size = Vector2((float)m_Thwomp->GetWidth(), (float)m_Thwomp->GetHeight());
			pivot = Vector2(m_Thwomp->GetPivot().x, m_Thwomp->GetPivot().y);
			break;
		}


		m_TileMap->DeleteWallObject(mousePos, size, pivot);
	}
	break;
	case ETileEditMode::FLOOR:
	{
		EFloorObjectType floorType = m_Dlg->GetFloorType();
		EFloorObjectOrientation orientation = m_Dlg->GetOrientation();

		Vector2 size;
		Vector2 pivot = Vector2(0.0f, 0.0f);

		switch (floorType)
		{
		case EFloorObjectType::ACCEL_PANEL:
			size = Vector2((float)m_AccelPanels[0]->GetWidth(), (float)m_AccelPanels[0]->GetHeight());
			break;
		case EFloorObjectType::ITEM_BOX:
			size = Vector2((float)m_ItemBoxes[0]->GetWidth(), (float)m_ItemBoxes[0]->GetHeight());
			break;
		case EFloorObjectType::OIL_SLICK:
			size = Vector2((float)m_Oils[0]->GetWidth(), (float)m_Oils[0]->GetHeight());
			break;
		case EFloorObjectType::COIN:
			size = Vector2((float)m_Coins[0]->GetWidth(), (float)m_Coins[0]->GetHeight());
			break;
		case EFloorObjectType::RAMP_HORIZONTAL:
			size = Vector2(8.0f, 8.0f);
			break;
		case EFloorObjectType::RAMP_VERTICAL:
			size = Vector2(8.0f, 8.0f);
			break;
		}

		m_TileMap->DeleteFloorObject(mousePos, size, pivot, floorType);
	}
	break;
	}
}

void CEditorScene::HideGrid(float deltaTime)
{
	CGameManager::GetInst()->SetShowGrid(!CGameManager::GetInst()->GetShowGrid());
}

void CEditorScene::SetFixX(float deltaTime)
{
	if (m_FixX)
		return;

	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	m_FixXPos = mousePos.x;

	m_FixX = true;
}

void CEditorScene::ResetFixX(float deltaTime)
{
	m_FixX = false;
}

void CEditorScene::SetFixY(float deltaTime)
{
	if (m_FixY)
		return;


	Vector2 mousePos = CInput::GetInst()->GetMousePos();

	m_FixYPos = mousePos.y;

	m_FixY = true;
}

void CEditorScene::ResetFixY(float deltaTime)
{
	m_FixY = false;
}

void CEditorScene::BackToTitle(float deltaTime)
{
	CSceneManager::GetInst()->CreateScene<CTitleScene>();
}

void CEditorScene::Save(const char* fullPath)
{
	if (!m_TileMap)
	{
		MessageBox(0, TEXT("Please create map first!"), TEXT("Error"), MB_OK);
		return;
	}

	m_TileMap->SetAIVision(m_Dlg->GetAIVision());

	m_TileMap->SetTrackMat(m_Dlg->GetTrackMaterial());
	m_TileMap->SetSlowMat(m_Dlg->GetSlowTrackMaterial1());
	m_TileMap->SetSlowMat2(m_Dlg->GetSlowTrackMaterial2());
	m_TileMap->SetHazardMat(m_Dlg->GetHazardTrackMaterial());

	m_TileMap->SetWideGoalStartX(m_Dlg->GetWideGoalStartX());
	m_TileMap->SetWideGoalStartY(m_Dlg->GetWideGoalStartY());
	m_TileMap->SetWideGoalWidth(m_Dlg->GetWideGoalWidth());
	m_TileMap->SetWideGoalHeight(m_Dlg->GetWideGoalHeight());

	m_TileMap->SetGoalStartX(m_Dlg->GetGoalStartX());
	m_TileMap->SetGoalStartY(m_Dlg->GetGoalStartY());
	m_TileMap->SetGoalWidth(m_Dlg->GetGoalWidth());
	m_TileMap->SetGoalHeight(m_Dlg->GetGoalHeight());

	m_TileMap->SaveFullPath(fullPath);
}

void CEditorScene::Load(const char* fullPath)
{
	if (!m_TileMap)
		m_TileMap = CreateMap<CTileMap>("TileMap");

	m_TileMap->LoadFullPath(fullPath);

	m_Dlg->SetAIVision(m_TileMap->GetAIFarVision());

	m_Dlg->SetTrackMaterial(m_TileMap->GetTrackMat());
	m_Dlg->SetSlowMaterial(m_TileMap->GetSlowMat());
	m_Dlg->SetSlowMaterial2(m_TileMap->GetSlowMat2());
	m_Dlg->SetOuterMaterial(m_TileMap->GetHazardMat());

	m_Dlg->SetWideGoalStartX(m_TileMap->GetWideGoalStartX());
	m_Dlg->SetWideGoalStartY(m_TileMap->GetWideGoalStartY());
	m_Dlg->SetWideGoalWidth(m_TileMap->GetWideGoalWidth());
	m_Dlg->SetWideGoalHeight(m_TileMap->GetWideGoalHeight());

	m_Dlg->SetGoalStartX(m_TileMap->GetGoalStartX());
	m_Dlg->SetGoalStartY(m_TileMap->GetGoalStartY());
	m_Dlg->SetGoalWidth(m_TileMap->GetGoalWidth());
	m_Dlg->SetGoalHeight(m_TileMap->GetGoalHeight());
}

void CEditorScene::SaveWallObjects(const char* fullPath)
{
	if (!m_TileMap)
	{
		MessageBox(0, TEXT("Please create map first!"), TEXT("Error"), MB_OK);
		return;
	}

	m_TileMap->SaveWallObjects(fullPath);
}

void CEditorScene::LoadWallObjects(const char* fullPath)
{
	m_TileMap->LoadWallObjects(fullPath);
}

void CEditorScene::SaveFloorObjects(const char* fullPath)
{
	if (!m_TileMap)
	{
		MessageBox(0, TEXT("Please create map first!"), TEXT("Error"), MB_OK);
		return;
	}

	m_TileMap->SaveFloorObjects(fullPath);
}

void CEditorScene::LoadFloorObjects(const char* fullPath)
{ 
	m_TileMap->LoadFloorObjects(fullPath);
}
