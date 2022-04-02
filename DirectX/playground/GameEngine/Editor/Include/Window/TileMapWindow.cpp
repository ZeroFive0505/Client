#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture/Texture.h"
#include "../EditorManager.h"
#include "Input.h"
#include "Component/Tile.h"
#include "PathManager.h"

CTileMapWindow::CTileMapWindow() :
	m_CountX(nullptr),
	m_CountY(nullptr),
	m_SizeX(nullptr),
	m_SizeY(nullptr),
	m_ShapeCombo(nullptr),
	m_TileMapCreateButton(nullptr),
	m_TypeCombo(nullptr),
	m_TileEditCombo(nullptr),
	m_FrameStartX(nullptr),
	m_FrameStartY(nullptr),
	m_FrameEndX(nullptr),
	m_FrameEndY(nullptr),
	m_DefaultFrameButton(nullptr),
	m_Sprite(nullptr),
	m_Frame(nullptr),
	m_CutFrameStartX(nullptr),
	m_CutFrameStartY(nullptr),
	m_CutFrameEndX(nullptr),
	m_CutFrameEndY(nullptr),
	m_BackTextureSetButton(nullptr),
	m_BackScaleX(nullptr),
	m_BackScaleY(nullptr),
	m_OffsetX(nullptr),
	m_OffsetY(nullptr)
{
}

CTileMapWindow::~CTileMapWindow()
{
}

bool CTileMapWindow::Init()
{
	if (!CIMGUIWindow::Init())
		return false;

	CIMGUILabel* label = AddWidget<CIMGUILabel>("TileMapInfo", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	label = AddWidget<CIMGUILabel>("TileShape", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	m_ShapeCombo = AddWidget<CIMGUIComboBox>("TileShapeCombo", 130.0f, 30.0f);

	m_ShapeCombo->SetHideName(true);
	m_ShapeCombo->AddItem("Rect");
	m_ShapeCombo->AddItem("Rhombus");
	m_ShapeCombo->AddItem("Rect Wire");

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("CountX", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_CountX = AddWidget<CIMGUITextInput>("CountX", 100.0f, 30.0f);
	m_CountX->SetHideName(true);
	m_CountX->SetTextType(ImGuiText_Type::Int);
	m_CountX->SetCallback(this, &CTileMapWindow::CountXCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("CountY", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_CountY = AddWidget<CIMGUITextInput>("CountY", 100.0f, 30.0f);
	m_CountY->SetHideName(true);
	m_CountY->SetTextType(ImGuiText_Type::Int);
	m_CountY->SetCallback(this, &CTileMapWindow::CountYCallback);

	label = AddWidget<CIMGUILabel>("SizeX", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_SizeX = AddWidget<CIMGUITextInput>("SizeX", 100.0f, 30.0f);
	m_SizeX->SetHideName(true);
	m_SizeX->SetTextType(ImGuiText_Type::Float);
	m_SizeX->SetCallback(this, &CTileMapWindow::SizeXCallback);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("SizeY", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_SizeY = AddWidget<CIMGUITextInput>("SizeY", 100.0f, 30.0f);
	m_SizeY->SetHideName(true);
	m_SizeY->SetTextType(ImGuiText_Type::Float);
	m_SizeY->SetCallback(this, &CTileMapWindow::SizeYCallback);

	m_TileMapCreateButton = AddWidget<CIMGUIButton>("TileMapCreateButton", 150.0f, 30.0f);

	m_TileMapCreateButton->SetClickCallback(this, &CTileMapWindow::TileMapCreateButton);

	CreateTileEditControl();

	CreateFrameControl();

	m_TileMapSaveButton = AddWidget<CIMGUIButton>("TileMapSaveButton", 150.0f, 30.0f);

	m_TileMapSaveButton->SetClickCallback(this, &CTileMapWindow::TileMapSaveButton);

	m_TileMapLoadButton = AddWidget<CIMGUIButton>("TileMapLoadButton", 150.0f, 30.0f);

	m_TileMapLoadButton->SetClickCallback(this, &CTileMapWindow::TileMapLoadButton);

	m_CountX->SetInt(50);
	m_CountY->SetInt(50);
	m_SizeX->SetFloat(160.f);
	m_SizeY->SetFloat(80.f);

	m_FrameStartX->SetFloat(160.f);
	m_FrameStartY->SetFloat(80.f);
	m_FrameEndX->SetFloat(320.f);
	m_FrameEndY->SetFloat(160.f);

	return true;
}

void CTileMapWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);

	// 마우스 드래그 처리
	if (CEditorManager::GetInst()->GetEditMode() == EditMode::TileMap && m_TileMap)
	{
		m_TileMap->EnableEditMode(true);

		if (CEditorManager::GetInst()->GetMouseLButtonPush())
		{
			// 마우스의 위치를 얻어와서 어떤 타일인지 구한다.
			Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

			mousePos -= Vector2(m_OffsetX->GetValueFloat(), m_OffsetY->GetValueFloat());

			CTile* tile = m_TileMap->GetTile(Vector3(mousePos.x, mousePos.y, 0.0f));

			if (tile)
			{
				int tileEditMode = m_TileEditCombo->GetSelectIndex();

				if (tileEditMode == -1)
					return;

				switch ((TileEdit_Mode)tileEditMode)
				{
				case TileEdit_Mode::Type:
				{
					int tileType = m_TypeCombo->GetSelectIndex();

					if (tileType == -1)
						return;

					Tile_Type type = (Tile_Type)tileType;

					tile->SetTileType(type);
				}
				break;
				case TileEdit_Mode::Frame:
				{
					float startX, startY, endX, endY;

					startX = m_CutFrameStartX->GetValueFloat();
					startY = m_CutFrameStartY->GetValueFloat();
					endX = m_CutFrameEndX->GetValueFloat();
					endY = m_CutFrameEndY->GetValueFloat();

					tile->SetFrameStart(startX, startY);
					tile->SetFrameEnd(endX, endY);
				}
				break;
				}
			}
		}
	}
	else
	{
		if(m_TileMap)
			m_TileMap->EnableEditMode(false);
	}
}

void CTileMapWindow::CountXCallback()
{
}

void CTileMapWindow::CountYCallback()
{
}

void CTileMapWindow::SizeXCallback()
{
	if (!m_TileMap)
		return;


	m_TileMap->SetScaleX(m_BackScaleX->GetValueFloat());
}

void CTileMapWindow::SizeYCallback()
{
	if (!m_TileMap)
		return;


	m_TileMap->SetScaleY(m_BackScaleY->GetValueFloat());
}

void CTileMapWindow::SetOffsetXCallback()
{
	if (!m_TileMap)
		return;


	m_TileMap->SetOffsetX(m_OffsetX->GetValueFloat());
}

void CTileMapWindow::SetOffsetYCallback()
{
	if (!m_TileMap)
		return;


	m_TileMap->SetOffsetY(m_OffsetY->GetValueFloat());
}

void CTileMapWindow::TileMapCreateButton()
{
	if (!m_TileMap)
		return;

	int shapeIndex = m_ShapeCombo->GetSelectIndex();

	if (shapeIndex >= (int)Tile_Shape::End || shapeIndex < 0)
		return;

	int countX, countY;

	countX = m_CountX->GetValueInt();
	countY = m_CountY->GetValueInt();

	Vector3 size;
	size.x = m_SizeX->GetValueFloat();
	size.y = m_SizeY->GetValueFloat();

	m_TileMap->CreateTile((Tile_Shape)shapeIndex, countX, countY, size);

	CTexture* texture = nullptr;

	if ((Tile_Shape)shapeIndex == Tile_Shape::Rect)
	{
		CSceneManager::GetInst()->GetScene()->GetSceneResource()->LoadTexture("DefaultRectTile", TEXT("Floors.png"));

		texture = CSceneManager::GetInst()->GetScene()->GetSceneResource()->FindTexture("DefaultRectTile");

		m_Sprite->SetTexture(texture);

		m_Frame->SetTexture(texture);
	}
	else if((Tile_Shape)shapeIndex == Tile_Shape::Rhombus)
	{
		CSceneManager::GetInst()->GetScene()->GetSceneResource()->LoadTexture("DefaultRhombusTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

		texture = CSceneManager::GetInst()->GetScene()->GetSceneResource()->FindTexture("DefaultRhombusTile");

		m_Sprite->SetTexture(texture);

		m_Frame->SetTexture(texture);
	}


	if ((Tile_Shape)shapeIndex != Tile_Shape::RectWire)
	{
		CMaterial* material = m_TileMap->GetTileMaterial();

		if (!material->HasTexture())
			material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "TileTexture", texture);
		else
			material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TileTexture", texture);
	}

}

void CTileMapWindow::TileMapSaveButton()
{
	if (!m_TileMap)
		return;

	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("All files\0*.*\0GameObject File\0*.gobj");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->path;

	if (GetSaveFileName(&openFile) != 0)
	{
		char convertFullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		CGameObject* tileMapObj = m_TileMap->GetGameObject();

		tileMapObj->Save(convertFullPath);
	}

}

void CTileMapWindow::TileMapLoadButton()
{
	if (!m_TileMap)
		return;

	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("All files\0*.*\0GameObject File\0*.gobj");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->path;

	if (GetOpenFileName(&openFile) != 0)
	{
		char convertFullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, convertFullPath, length, 0, 0);

		CGameObject* tileMapObj = m_TileMap->GetGameObject();

		tileMapObj->Load(convertFullPath);
	}
}

void CTileMapWindow::TileFrameStartXCallback()
{
	float startX, startY, endX, endY;

	startX = m_CutFrameStartX->GetValueFloat();
	startY = m_CutFrameStartY->GetValueFloat();
	endX = m_CutFrameEndX->GetValueFloat();
	endY = m_CutFrameEndY->GetValueFloat();

	Vector2 start = Vector2(startX, startY);
	Vector2 end = Vector2(endX, endY);

	CutTile(start, end);
}

void CTileMapWindow::TileFrameStartYCallback()
{
	float startX, startY, endX, endY;

	startX = m_CutFrameStartX->GetValueFloat();
	startY = m_CutFrameStartY->GetValueFloat();
	endX = m_CutFrameEndX->GetValueFloat();
	endY = m_CutFrameEndY->GetValueFloat();

	Vector2 start = Vector2(startX, startY);
	Vector2 end = Vector2(endX, endY);

	CutTile(start, end);
}

void CTileMapWindow::TileFrameEndXCallback()
{
	float startX, startY, endX, endY;

	startX = m_CutFrameStartX->GetValueFloat();
	startY = m_CutFrameStartY->GetValueFloat();
	endX = m_CutFrameEndX->GetValueFloat();
	endY = m_CutFrameEndY->GetValueFloat();

	Vector2 start = Vector2(startX, startY);
	Vector2 end = Vector2(endX, endY);

	CutTile(start, end);
}

void CTileMapWindow::TileFrameEndYCallback()
{
	float startX, startY, endX, endY;

	startX = m_CutFrameStartX->GetValueFloat();
	startY = m_CutFrameStartY->GetValueFloat();
	endX = m_CutFrameEndX->GetValueFloat();
	endY = m_CutFrameEndY->GetValueFloat();

	Vector2 start = Vector2(startX, startY);
	Vector2 end = Vector2(endX, endY);

	CutTile(start, end);
}

void CTileMapWindow::LoadTexture()
{
	if (!m_TileMap)
		return;

	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter =
		TEXT("All files\0*.*\0DDS File\0*.dds\0TGA file\0*.tga\0PNG file\0*.png\0JPG file\0*.jpg\0JPEG file\0*.jpeg\0BMP file\0*.bmp");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->path;

	if (GetOpenFileName(&openFile) != 0)
	{
		TCHAR fileName[MAX_PATH] = {};

		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);

		char convertFileName[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		m_TileMap->SetBackMaterial(CSceneManager::GetInst()->GetScene()->GetSceneResource()->FindMaterial("BaseTexture"));

		m_TileMap->SetBackTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, convertFileName, filePath);
	}
}

void CTileMapWindow::CutTile(const Vector2& start, const Vector2& end)
{
	m_Frame->SetImageStart(start.x, start.y);
	m_Frame->SetImageEnd(end.x, end.y);
}

void CTileMapWindow::DefaultFrameButton()
{
	if (!m_TileMap)
		return;


	float startX, startY, endX, endY;

	startX = m_FrameStartX->GetValueFloat();
	startY = m_FrameStartY->GetValueFloat();
	endX = m_FrameEndX->GetValueFloat();
	endY = m_FrameEndY->GetValueFloat();

	m_TileMap->SetTileDefaultFrame(startX, startY, endX, endY);
}

void CTileMapWindow::CreateTileEditControl()
{
	CIMGUILabel* label = AddWidget<CIMGUILabel>("TileEditInfo", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5, 0.0f);

	label = AddWidget<CIMGUILabel>("TileType", 100.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	m_TypeCombo = AddWidget<CIMGUIComboBox>("TileTypeCombo", 130.0f, 30.0f);

	m_TypeCombo->SetHideName(true);
	m_TypeCombo->AddItem("Normal");
	m_TypeCombo->AddItem("Wall");
	m_TypeCombo->AddItem("SideWall");
	m_TypeCombo->AddItem("Pilar");

	label = AddWidget<CIMGUILabel>("TileEditType", 100.0f, 30.0f);
	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);
	
	line = AddWidget<CIMGUISameLine>("Line");

	m_TileEditCombo = AddWidget<CIMGUIComboBox>("TileEditCombo", 130.0f, 30.0f);

	m_TileEditCombo->SetHideName(true);
	m_TileEditCombo->AddItem("Type");
	m_TileEditCombo->AddItem("Frame");

	label = AddWidget<CIMGUILabel>("FrameStartX", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_FrameStartX = AddWidget<CIMGUITextInput>("FrameStartX", 100.0f, 30.0f);
	m_FrameStartX->SetHideName(true);
	m_FrameStartX->SetTextType(ImGuiText_Type::Float);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("FrameStartY", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_FrameStartY = AddWidget<CIMGUITextInput>("FrameStartY", 100.0f, 30.0f);
	m_FrameStartY->SetHideName(true);
	m_FrameStartY->SetTextType(ImGuiText_Type::Float);

	label = AddWidget<CIMGUILabel>("FrameEndX", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_FrameEndX = AddWidget<CIMGUITextInput>("FrameEndX", 100.0f, 30.0f);
	m_FrameEndX->SetHideName(true);
	m_FrameEndX->SetTextType(ImGuiText_Type::Float);

	line = AddWidget<CIMGUISameLine>("Line");

	label = AddWidget<CIMGUILabel>("FrameEndY", 130.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	m_FrameEndY = AddWidget<CIMGUITextInput>("FrameEndY", 100.0f, 30.0f);
	m_FrameEndY->SetHideName(true);
	m_FrameEndY->SetTextType(ImGuiText_Type::Float);

	m_DefaultFrameButton = AddWidget<CIMGUIButton>("DefaultFrameButton", 150.0f, 30.0f);

	m_DefaultFrameButton->SetClickCallback(this, &CTileMapWindow::DefaultFrameButton);

	m_BackTextureSetButton = AddWidget<CIMGUIButton>("SetBackGround", 150.0f, 30.0f);

	m_BackTextureSetButton->SetClickCallback(this, &CTileMapWindow::LoadTexture);

	m_BackScaleX = AddWidget<CIMGUITextInput>("BackGroundScaleX", 100.0f, 30.0f);

	m_BackScaleX->SetTextType(ImGuiText_Type::Float);

	m_BackScaleX->SetFloat(1.0f);

	m_BackScaleX->SetCallback(this, &CTileMapWindow::SizeXCallback);

	m_BackScaleY = AddWidget<CIMGUITextInput>("BackGroundScaleY", 100.0f, 30.0f);

	m_BackScaleY->SetTextType(ImGuiText_Type::Float);

	m_BackScaleY->SetFloat(1.0f);

	m_BackScaleY->SetCallback(this, &CTileMapWindow::SizeYCallback);

	m_OffsetX = AddWidget<CIMGUITextInput>("BackGroundOffsetX", 100.0f, 30.0f);

	m_OffsetX->SetTextType(ImGuiText_Type::Float);

	m_OffsetX->SetFloat(0.0f);

	m_OffsetX->SetCallback(this, &CTileMapWindow::SetOffsetXCallback);

	m_OffsetY = AddWidget<CIMGUITextInput>("BackGroundOffsetY", 100.0f, 30.0f);

	m_OffsetY->SetTextType(ImGuiText_Type::Float);

	m_OffsetY->SetFloat(0.0f);

	m_OffsetY->SetCallback(this, &CTileMapWindow::SetOffsetYCallback);
}

void CTileMapWindow::CreateFrameControl()
{
	CIMGUILabel* label = AddWidget<CIMGUILabel>("Spirte", 100.0f, 30.0f);

	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	label = AddWidget<CIMGUILabel>("Frame", 100.0f, 30.0f);

	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	m_Sprite = AddWidget<CIMGUIImage>("Tile Sprite", 200.0f, 200.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_Frame = AddWidget<CIMGUIImage>("Tile Frame", 200.0f, 200.0f);

	m_CutFrameStartX = AddWidget<CIMGUITextInput>("TileFrameStartX", 150.0f, 100.0f);
	m_CutFrameStartX->SetTextType(ImGuiText_Type::Float);
	m_CutFrameStartX->SetCallback<CTileMapWindow>(this, &CTileMapWindow::TileFrameStartXCallback);

	m_CutFrameStartY = AddWidget<CIMGUITextInput>("TileFrameStartY", 150.0f, 100.0f);
	m_CutFrameStartY->SetTextType(ImGuiText_Type::Float);
	m_CutFrameStartY->SetCallback<CTileMapWindow>(this, &CTileMapWindow::TileFrameStartYCallback);

	m_CutFrameEndX = AddWidget<CIMGUITextInput>("TileFrameEndX", 150.0f, 100.0f);
	m_CutFrameEndX->SetTextType(ImGuiText_Type::Float);
	m_CutFrameEndX->SetCallback<CTileMapWindow>(this, &CTileMapWindow::TileFrameEndXCallback);

	m_CutFrameEndY = AddWidget<CIMGUITextInput>("TileFrameEndY", 150.0f, 100.0f);
	m_CutFrameEndY->SetTextType(ImGuiText_Type::Float);
	m_CutFrameEndY->SetCallback<CTileMapWindow>(this, &CTileMapWindow::TileFrameEndYCallback);
}
