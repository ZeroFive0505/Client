#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"

struct TileFrameData
{
	Vector2 Start;
	Vector2 End;
};

struct TileTextureFrameData
{
	std::vector<TileFrameData> vecData;
};


/// <summary>
/// 에디터 씬에서 활용되는 에디터 다이얼로그
/// </summary>
class CEditorDlg
{
public:
	CEditorDlg();
	~CEditorDlg();

private:
	// 다이얼로그 핸들
	HWND m_hDlg;
	// 현재 다이얼로그의 아이디
	int m_ID;
	// 열려있는지
	bool m_Open;
	// 에디터 씬
	class CEditorScene* m_Scene;
	// 텍스쳐 리스트박스 핸들
	HWND m_TextureListBox;
	// 텍스쳐 리스트에서 선택된 아이템의 인덱스
	int m_SelectTextureListIndex;
	// 선택된 아이템의 이름
	TCHAR m_SelectTextureListText[256];
	// 선택된 텍스쳐
	CSharedPtr<CTexture> m_SelectTileTexture;
	// 각 타일마다의 텍스쳐 프레임 데이타
	std::vector<TileTextureFrameData> m_vecTextureFrameData;

	HWND m_EditModeCombo;
	ETileEditMode m_TileEditMode;

	HWND m_TilePropertyCombo;
	ETileProperty m_TileProperty;

	HWND m_FrameListBox;
	int m_SelectFrameIndex;

	HWND m_SideCollisionCheckHandle;
	bool m_SideCollision;

public:
	bool GetSideCollision() const
	{
		return m_SideCollision;
	}

	ETileProperty GetTileProperty() const
	{
		return (ETileProperty)SendMessage(m_TilePropertyCombo, CB_GETCURSEL, 0, 0);
	}

	TileFrameData GetTileFrameData() const
	{
		if (m_SelectTextureListIndex == -1 || m_SelectFrameIndex == -1)
			return TileFrameData();

		return m_vecTextureFrameData[m_SelectTextureListIndex].vecData[m_SelectFrameIndex];
	}

	ETileEditMode GetTileEditMode() const
	{
		return (ETileEditMode)SendMessage(m_EditModeCombo, CB_GETCURSEL, 0, 0);
	}


	bool IsOpen() const
	{
		return m_Open;
	}

	// 현재 스크린좌표에서 윈도우의 위치를 구해서 반환한다.
	RECT GetRect() const
	{
		RECT rc;
		GetWindowRect(m_hDlg, &rc);
		return rc;
	}

	void SetScene(class CEditorScene* Scene)
	{
		m_Scene = Scene;
	}

public:
	bool Init(int ID);
	void Show();
	void CreateMap();
	void LoadTileTexture();
	void SelectTexture();
	void SelectList();
	void AddFrame();
	void DeleteFrame();
	void EditFrame();
	void ChangeFrame();
	void Save();
	void Load();
	void TileImageClear();
	void SideCollisionCheck();

private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

