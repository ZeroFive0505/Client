#include "EditorDlg.h"
#include "../GameManager.h"
#include "../PathManager.h"
#include "Scene.h"
#include "SceneResource.h"
#include "EditorScene.h"
#include "../Scene/Camera.h"

CEditorDlg* g_Dlg;

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0),
	m_Open(true),
	m_Scene(nullptr)
{
	g_Dlg = this;
}

CEditorDlg::~CEditorDlg()
{
	if (m_hDlg)
		DestroyWindow(m_hDlg);
}

bool CEditorDlg::Init(int ID)
{
	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(),
		MAKEINTRESOURCE(ID), CGameManager::GetInst()->GetWindowHandle(),
		CEditorDlg::WndProc);

	ShowWindow(m_hDlg, SW_SHOW);

	SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, 128, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, 128, TRUE);

	SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, 8, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, 8, TRUE);

	m_TileListBox = GetDlgItem(m_hDlg, IDC_LIST_TILETEXTURES);

	m_EditModeCombo = GetDlgItem(m_hDlg, IDC_COMBO_EDITMODE);

	TCHAR tileEditModes[(int)ETileEditMode::END][30] = 
	{
		TEXT("Tile property"),
		TEXT("Wall"),
		TEXT("Floor")
	};

	for (int i = 0; i < (int)ETileEditMode::END; i++)
	{
		SendMessage(m_EditModeCombo, CB_ADDSTRING, 0, (LPARAM)tileEditModes[i]);
	}

	SendMessage(m_EditModeCombo, CB_SETCURSEL, 0, 0);

	m_TilePropertyCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILEPROPERTY);

	TCHAR tileProperties[(int)ETileProperty::END][30] =
	{
		TEXT("Track"),
		TEXT("Block"),
		TEXT("Slow_1"),
		TEXT("Slow_2"),
		TEXT("13H"),
		TEXT("Hazard")
	};

	for (int i = 0; i < 6; i++)
	{
		SendMessage(m_TilePropertyCombo, CB_ADDSTRING, 0, (LPARAM)tileProperties[i]);
	}

	SendMessage(m_TilePropertyCombo, CB_SETCURSEL, 0, 0);

	m_WallTypeCombo = GetDlgItem(m_hDlg, IDC_COMBO_WALLTYPE);

	TCHAR wallTypes[(int)EWallObjectType::END][30] =
	{
		TEXT("Green Pipe"),
		TEXT("Orange Pipe"),
		TEXT("Super thwomp"),
		TEXT("Thwomp"),
		TEXT("Flower")
	};

	for (int i = 0; i < (int)EWallObjectType::END; i++)
	{
		SendMessage(m_WallTypeCombo, CB_ADDSTRING, 0, (LPARAM)wallTypes[i]);
	}

	SendMessage(m_WallTypeCombo, CB_SETCURSEL, 0, 0);

	m_FloorTypeCombo = GetDlgItem(m_hDlg, IDC_COMBO_FLOORTYPE);

	TCHAR floorTypes[(int)EFloorObjectType::END][30] =
	{
		TEXT("Accel panel"),
		TEXT("Item box"),
		TEXT("Oil"),
		TEXT("Coin"),
		TEXT("Ramp horizontal"),
		TEXT("Ramp vertical")
	};

	for (int i = 0; i < (int)EFloorObjectType::END; i++)
	{
		SendMessage(m_FloorTypeCombo, CB_ADDSTRING, 0, (LPARAM)floorTypes[i]);
	}

	SendMessage(m_FloorTypeCombo, CB_SETCURSEL, 0, 0);

	m_FloorOrientationCombo = GetDlgItem(m_hDlg, IDC_COMBO_FLOOR_ORIENTAION);

	TCHAR floorOrientations[(int)EFloorObjectOrientation::END][30] =
	{
		TEXT("UP"),
		TEXT("RIGHT"),
		TEXT("DOWN"),
		TEXT("LEFT")
	};

	for (int i = 0; i < (int)EFloorObjectOrientation::END; i++)
	{
		SendMessage(m_FloorOrientationCombo, CB_ADDSTRING, 0, (LPARAM)floorOrientations[i]);
	}

	SendMessage(m_FloorOrientationCombo, CB_SETCURSEL, 0, 0);


	TCHAR trackMaterials[(int)ETileMaterial::END][30] =
	{
		TEXT("DIRT"),
		TEXT("GRASS"),
		TEXT("SPOOKY WOOD"),
		TEXT("STONE"),
		TEXT("WATER"),
		TEXT("LAVA"),
		TEXT("EMPTY"),
		TEXT("RAINBOW"),
		TEXT("SWAMP")
	};


	m_TrackMatCombo = GetDlgItem(m_hDlg, IDC_COMBO_TRACKMAT);

	for (int i = 0; i < (int)ETileMaterial::END; i++)
	{
		SendMessage(m_TrackMatCombo, CB_ADDSTRING, 0, (LPARAM)trackMaterials[i]);
	}

	SendMessage(m_TrackMatCombo, CB_SETCURSEL, 0, 0);

	m_SlowTrackMatCombo1 = GetDlgItem(m_hDlg, IDC_COMBO_SLOWTRACKMAT);

	for (int i = 0; i < (int)ETileMaterial::END; i++)
	{
		SendMessage(m_SlowTrackMatCombo1, CB_ADDSTRING, 0, (LPARAM)trackMaterials[i]);
	}

	SendMessage(m_SlowTrackMatCombo1, CB_SETCURSEL, 0, 0);

	m_SlowTrackMatCombo2 = GetDlgItem(m_hDlg, IDC_COMBO_SLOWTRACKMAT2);

	for (int i = 0; i < (int)ETileMaterial::END; i++)
	{
		SendMessage(m_SlowTrackMatCombo2, CB_ADDSTRING, 0, (LPARAM)trackMaterials[i]);
	}

	SendMessage(m_SlowTrackMatCombo2, CB_SETCURSEL, 0, 0);

	m_HazardTrackMatCombo = GetDlgItem(m_hDlg, IDC_COMBO_OUTERTRACKMAT);

	for (int i = 0; i < (int)ETileMaterial::END; i++)
	{
		SendMessage(m_HazardTrackMatCombo, CB_ADDSTRING, 0, (LPARAM)trackMaterials[i]);
	}

	SendMessage(m_HazardTrackMatCombo, CB_SETCURSEL, 0, 0);

	return true;
}

void CEditorDlg::Show()
{
	if (m_Open)
	{
		m_Open = false;
		ShowWindow(m_hDlg, SW_HIDE);
	}
	else
	{
		m_Open = true;
		ShowWindow(m_hDlg, SW_SHOW);
	}
}

void CEditorDlg::CreateMap()
{
	BOOL transfer = FALSE;

	int tileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &transfer, TRUE);
	int tileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &transfer, TRUE);
	int tileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &transfer, TRUE);
	int tileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &transfer, TRUE);

	m_Scene->CreateTileMap();

	m_Scene->SetTileInfo(tileCountX, tileCountY, tileSizeX, tileSizeY);
}


void CEditorDlg::LoadRefImage()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All file\0*.*\0Bmp\0*.bmp");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->unicodePath;

	if (GetOpenFileName(&openFile) != 0)
	{
		TCHAR fileName[128] = {};

		_wsplitpath_s(filePath, nullptr, 0, nullptr, 0, fileName, 128, nullptr, 0);

		char textureName[256] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, fileName, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, fileName, -1, textureName, convertLength, 0, 0);
#else
		strcpy_s(textureName, fileName);

#endif // UNICODE

		if (!m_Scene->GetSceneResoruce()->LoadTextureFullPath(textureName, filePath))
			return;

		CTexture* texture = m_Scene->GetSceneResoruce()->FindTexture(textureName);

		texture->SetColorKey(255, 0, 255);

		m_Scene->LoadRefImage(textureName);

	}
}

void CEditorDlg::ClearRefImage()
{
	m_Scene->ClearRefImage();
}



void CEditorDlg::Save()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0Map file\0*.map");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetSaveFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);

#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE

		m_Scene->Save(fullPath);
	}
}

void CEditorDlg::Load()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0Map file\0*.map");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetOpenFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);
#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE


		m_Scene->Load(fullPath);
	}
}


void CEditorDlg::WallObjectSave()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0txt file\0*.txt");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetSaveFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);

#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE

		m_Scene->SaveWallObjects(fullPath);
	}
}

void CEditorDlg::WallObjectLoad()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0txt file\0*.txt");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetOpenFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);
#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE


		m_Scene->LoadWallObjects(fullPath);
	}
}



void CEditorDlg::FloorObjectSave()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0txt file\0*.txt");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetSaveFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);

#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE

		m_Scene->SaveFloorObjects(fullPath);
	}
}

void CEditorDlg::FloorObjectLoad()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = m_hDlg;
	openFile.lpstrFilter = TEXT("All files\0*.*\0txt file\0*.txt");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->unicodePath;

	if (GetOpenFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

#ifdef UNICODE
		int convertLength = WideCharToMultiByte(CP_ACP, 0, filePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, convertLength, 0, 0);
#else
		strcpy_s(fullPath, filePath);

#endif // UNICODE


		m_Scene->LoadFloorObjects(fullPath);
	}
}


LRESULT CEditorDlg::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
			break;
		case IDCANCEL:
			break;
		case IDC_BUTTON_CREATEMAP:
			g_Dlg->CreateMap();
			break;
		case IDC_BUTTON_SAVE:
			g_Dlg->Save();
			break;
		case IDC_BUTTON_LOAD:
			g_Dlg->Load();
			break;
		case IDC_BUTTON_LOADREFIMAGE:
			g_Dlg->LoadRefImage();
			break;
		case IDC_BUTTON_CLEARREFIMAGE:
			g_Dlg->ClearRefImage();
			break;
		case IDC_BUTTON_WALLOBJECT_LOCATION_SAVE:
			g_Dlg->WallObjectSave();
			break;
		case IDC_BUTTON_WALLOBJECT_LOCATION_LOAD:
			g_Dlg->WallObjectLoad();
			break;
		case IDC_BUTTON_FLOOROBJECT_LOCATION_SAVE:
			g_Dlg->FloorObjectSave();
			break;
		case IDC_BUTTON_FLOOROBJECT_LOCATION_LOAD:
			g_Dlg->FloorObjectLoad();
		}
	}

	return 0;
}
