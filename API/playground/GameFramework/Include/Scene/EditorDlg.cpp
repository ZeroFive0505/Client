#include "EditorDlg.h"
#include "../GameManager.h"
#include "../PathManager.h"
#include "Scene.h"
#include "SceneResource.h"
#include "EditorScene.h"

// 윈도우 프로시져의 메세지를 처리하기위해 전역변수로 만든다.
CEditorDlg* g_Dlg;

CEditorDlg::CEditorDlg() :
	m_ID(0),
	m_hDlg(0),
	m_Open(true),
	m_Scene(nullptr),
	m_SelectTextureListText{},
	m_SelectFrameIndex(-1)
{
	// 생성시 바로 초기화.
	g_Dlg = this;
}

CEditorDlg::~CEditorDlg()
{
	if (m_hDlg)
		DestroyWindow(m_hDlg);
}

bool CEditorDlg::Init(int ID)
{
	// Modal : 부모 윈도우는 멈추고 열리는 다이얼로그만 동작되는 방식
	// Modeless : 부모 윈두오와 다이얼로그 모두 동작되는 방식

	m_hDlg = CreateDialog(CGameManager::GetInst()->GetWindowInstance(),
		MAKEINTRESOURCE(ID), CGameManager::GetInst()->GetWindowHandle(),
		CEditorDlg::WndProc);

	// 다이얼로그를 보인다.
	ShowWindow(m_hDlg, SW_SHOW);

	// 다이얼로그의 해당 아이디의 정수 값을 지정한다.
	SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, 40, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, 40, TRUE);

	SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, 40, TRUE);
	SetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, 53, TRUE);

	// 리스트박스의 핸들을 받아온다.
	m_TextureListBox = GetDlgItem(m_hDlg, IDC_LIST_TILETEXTURE);

	m_FrameListBox = GetDlgItem(m_hDlg, IDC_LIST_TEXTUREFRAME);

	m_SelectTextureListIndex = -1;

	m_EditModeCombo = GetDlgItem(m_hDlg, IDC_COMBO_EDITMODE);

	TCHAR TileEditMode[(int)ETileEditMode::End][30] =
	{
		TEXT("Tile property"),
		TEXT("Tile image")
	};

	for (int i = 0; i < (int)ETileEditMode::End; i++)
	{
		SendMessage(m_EditModeCombo, CB_ADDSTRING, 0, (LPARAM)TileEditMode[i]);
	}

	SendMessage(m_EditModeCombo, CB_SETCURSEL, 0, 0);

	m_TileEditMode = ETileEditMode::Property;

	m_TilePropertyCombo = GetDlgItem(m_hDlg, IDC_COMBO_TILEPROPERTY);

	TCHAR TileProperty[(int)ETileProperty::End][30] =
	{
		TEXT("Normal"),
		TEXT("Wall"),
		TEXT("Slow")
	};

	for (int i = 0; i < (int)ETileProperty::End; i++)
	{
		SendMessage(m_TilePropertyCombo, CB_ADDSTRING, 0, (LPARAM)TileProperty[i]);
	}

	SendMessage(m_TilePropertyCombo, CB_SETCURSEL, 0, 0);

	m_TileProperty = ETileProperty::Normal;

	m_SideCollisionCheckHandle = GetDlgItem(m_hDlg, IDC_CHECK_SIDECOLLISION);
	m_SideCollision = false;

	return false;
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
	BOOL Transfer = FALSE;

	// 타일 정보들을 가져온다.
	int TileCountX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTX, &Transfer, TRUE);
	int TileCountY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILECOUNTY, &Transfer, TRUE);
	int TileSizeX = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEX, &Transfer, TRUE);
	int TileSizeY = GetDlgItemInt(m_hDlg, IDC_EDIT_TILESIZEY, &Transfer, TRUE);

	// 현재 씬에서 타일맵을 생성한다.
	m_Scene->CreateTileMap();

	// 타일들의 속성을 설정한다.
	m_Scene->SetTileInfo(TileCountX, TileCountY, TileSizeX, TileSizeY);
}

void CEditorDlg::LoadTileTexture()
{
	// 타일을 불러온다.
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};
	
	OpenFile.lStructSize = sizeof(OPENFILENAME);
	// 현재 핸들의 오너
	OpenFile.hwndOwner = m_hDlg;
	// 파일 필터 값
	OpenFile.lpstrFilter = TEXT("All file\0*.*\0Bmp\0*.bmp");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	// 초기위치는 텍스쳐가 배치된 곳의 위치
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		// 전체 경로에서 파일 이름만 얻어온다.
		TCHAR FileName[128] = {};

		_wsplitpath_s(FilePath, nullptr, 0, nullptr, 0, FileName, 128, nullptr, 0);

		char TextureName[256] = {};

#ifdef UNICODE
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FileName, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, FileName, -1, TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// 현재 씬 리소스에 텍스쳐를 추가한다.
		if (!m_Scene->GetSceneResource()->LoadTextureFullPath(TextureName, FilePath))
			return;

		CTexture* Texture = m_Scene->GetSceneResource()->FindTexture(TextureName);

		Texture->SetColorKey(255, 0, 255);

		// 텍스쳐 리스트박스에 텍스쳐 파일의 이름을 문자열로 추가한다.
		SendMessage(m_TextureListBox, LB_ADDSTRING, 0, (LPARAM)FileName);

		TileTextureFrameData data;
		m_vecTextureFrameData.push_back(data);

	}
}

void CEditorDlg::SelectTexture()
{
	// 만약 텍스쳐가 선택이 되었다면
	if (m_SelectTextureListIndex != -1)
	{
		char TextureName[256] = {};

#ifdef UNICODE
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, m_SelectTextureListText, -1, TextureName, ConvertLength, 0, 0);
#else
		strcpy_s(TextureName, FileName);

#endif // UNICODE

		// 현재 선택된 텍스쳐는 씬 리소스에 파일이름 그대로 저장되어있다. 따라서 그대로 가져온다.
		m_SelectTileTexture = m_Scene->GetSceneResource()->FindTexture(TextureName);

		// 타일맵을 만든다.
		m_Scene->CreateTileMap();

		// 타일 설정
		m_Scene->SetTileTexture(m_SelectTileTexture);

	}
}

void CEditorDlg::SelectList()
{
	// 인덱스의 값을 리스트박스에서 가져온다.
	m_SelectTextureListIndex = (int)SendMessage(m_TextureListBox, LB_GETCURSEL, 0, 0);
	
	// 만약 무언가 선택이 되었다면
	if (m_SelectTextureListIndex != -1)
	{
		memset(m_SelectTextureListText, 0, sizeof(TCHAR) * 256);
		// 리스트박스에서 인덱스에 배치된 텍스트 값을 m_SelectTextureListText여기에 저장한다. 
		SendMessage(m_TextureListBox, LB_GETTEXT, m_SelectTextureListIndex, (LPARAM)m_SelectTextureListText);

		int Count = (int)SendMessage(m_FrameListBox, LB_GETCOUNT, 0, 0);

		for (int i = 0; i < Count; i++)
		{
			SendMessage(m_FrameListBox, LB_DELETESTRING, 0, 0);
		}

		for (size_t i = 0; i < m_vecTextureFrameData[m_SelectTextureListIndex].vecData.size(); i++)
		{
			TCHAR Text[32] = {};

			wsprintf(Text, TEXT("%d"), (int)i);

			SendMessage(m_FrameListBox, LB_ADDSTRING, 0, (LPARAM)Text);
		}

		m_SelectFrameIndex = -1;
	}
}

void CEditorDlg::AddFrame()
{
	if (m_SelectTextureListIndex == -1)
		return;

	BOOL Transfer = FALSE;

	int StartFrameX = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTX, &Transfer, TRUE);
	int StartFrameY = GetDlgItemInt(m_hDlg, IDC_EDIT_STARTY, &Transfer, TRUE);
	int EndFrameX = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDX, &Transfer, TRUE);
	int EndFrameY = GetDlgItemInt(m_hDlg, IDC_EDIT_ENDY, &Transfer, TRUE);

	TileFrameData data;

	data.Start.x = (float)StartFrameX;
	data.Start.y = (float)StartFrameY;
	data.End.x = (float)EndFrameX;
	data.End.y = (float)EndFrameY;

	TCHAR Text[32] = {};

	wsprintf(Text, TEXT("%d"), (int)m_vecTextureFrameData[m_SelectTextureListIndex].vecData.size());

	SendMessage(m_FrameListBox, LB_ADDSTRING, 0, (LPARAM)Text);

	m_vecTextureFrameData[m_SelectTextureListIndex].vecData.push_back(data);
}

void CEditorDlg::DeleteFrame()
{
}

void CEditorDlg::EditFrame()
{
}

void CEditorDlg::ChangeFrame()
{
	m_SelectFrameIndex = (int)SendMessage(m_FrameListBox, LB_GETCURSEL, 0, 0);

	if (m_SelectFrameIndex != -1)
	{
		TileFrameData data = m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];

		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTX, (int)data.Start.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_STARTY, (int)data.Start.y, TRUE);

		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDX, (int)data.End.x, TRUE);
		SetDlgItemInt(m_hDlg, IDC_EDIT_ENDY, (int)data.End.y, TRUE);
	}
}

void CEditorDlg::Save()
{
	// 세이브 버튼을 눌렀을시 파일을 저장한다.
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	OpenFile.lpstrFilter = TEXT("All files\0*.*\0MapFile\0*.map");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	// 파일이 저장되었다면 
	if (GetSaveFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};

		// 멀티바이트 문자열로 바꾼다.
#ifdef UNICODE
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, ConvertLength, 0, 0);
#else
		strcpy_s(FullPath, FilePath);
#endif // UNICODE


		// 에디터 씬의 저장함수를 호출한다.
		m_Scene->Save(FullPath);
	}
}

void CEditorDlg::Load()
{
	TCHAR FilePath[MAX_PATH] = {};

	OPENFILENAME OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = m_hDlg;
	OpenFile.lpstrFilter = TEXT("All files\0*.*\0MapFile\0*.map");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(MAP_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char FullPath[MAX_PATH] = {};
#ifdef UNICODE
		int ConvertLength = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, nullptr, 0, 0, 0);

		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, ConvertLength, 0, 0);

#else
		strcpy_s(FullPath, FilePath);
#endif // UNICODE

		// 로드시에는 에디터씬의 로드함수를 호출한다.
		m_Scene->Load(FullPath);
	}
}

void CEditorDlg::TileImageClear()
{
	m_Scene->TileImageClear();
}

void CEditorDlg::SideCollisionCheck()
{
	if (SendMessage(m_SideCollisionCheckHandle, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
	{
		m_SideCollision = false;
	}
	else
	{
		m_SideCollision = true;
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
			// 리스트박스에 변화가 생겼을시
		case IDC_LIST_TILETEXTURE:
			switch (HIWORD(wParam))
			{
				// 선택된 아이템이 바뀌었을시
			case LBN_SELCHANGE:
			{
				g_Dlg->SelectList();
			}
				break;
			}
			break;
		case IDC_LIST_TEXTUREFRAME:
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				g_Dlg->ChangeFrame();
			}
			break;
			}
			break;
		case IDOK:
			break;
		case IDCANCEL:
			break;
		case IDC_BUTTON_CREATE_MAP:
			g_Dlg->CreateMap();
			break;
		case IDC_BUTTON_TILELOAD:
			g_Dlg->LoadTileTexture();
			break;
		case IDC_BUTTON_TILESELECT:
			g_Dlg->SelectTexture();
			break;
		case IDC_BUTTON_FRAMEADD:
			g_Dlg->AddFrame();
			break;
		case IDC_BUTTON_FRAMEDELETE:
			g_Dlg->DeleteFrame();
			break;
		case IDC_BUTTON_FRAMEEDIT:
			g_Dlg->EditFrame();	
			break;
		case IDC_BUTTON_SAVE:
			g_Dlg->Save();
			break;
		case IDC_BUTTON_LOAD:
			g_Dlg->Load();
			break;
		case IDC_BUTTON_CLEARTILES:
			g_Dlg->TileImageClear();
			break;
		case IDC_CHECK_SIDECOLLISION:
			g_Dlg->SideCollisionCheck();
			break;
		}
	default:
		break;
	}


	return 0;
}
