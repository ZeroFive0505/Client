#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Map/MapInfo.h"

class CEditorDlg
{
public:
	CEditorDlg();
	~CEditorDlg();

private:
	HWND m_hDlg;

	int m_ID;

	bool m_Open;

	class CEditorScene* m_Scene;

	HWND m_TileListBox;

	HWND m_EditModeCombo;
	HWND m_TilePropertyCombo;

	HWND m_WallTypeCombo;
	HWND m_FloorTypeCombo;

	HWND m_FloorOrientationCombo;

	HWND m_TrackMatCombo;
	HWND m_SlowTrackMatCombo1;
	HWND m_SlowTrackMatCombo2;
	HWND m_HazardTrackMatCombo;

public:

	inline ETileProperty GetTileProperty() const
	{
		return (ETileProperty)SendMessage(m_TilePropertyCombo, CB_GETCURSEL, 0, 0);
	}

	inline ETileEditMode GetTileEditMode() const
	{
		return (ETileEditMode)SendMessage(m_EditModeCombo, CB_GETCURSEL, 0, 0);
	}

	inline ETileMaterial GetTrackMaterial() const
	{
		return (ETileMaterial)SendMessage(m_TrackMatCombo, CB_GETCURSEL, 0, 0);
	}

	inline ETileMaterial GetSlowTrackMaterial1() const
	{
		return (ETileMaterial)SendMessage(m_SlowTrackMatCombo1, CB_GETCURSEL, 0, 0);
	}

	inline ETileMaterial GetSlowTrackMaterial2() const
	{
		return (ETileMaterial)SendMessage(m_SlowTrackMatCombo2, CB_GETCURSEL, 0, 0);
	}

	inline ETileMaterial GetHazardTrackMaterial() const
	{
		return (ETileMaterial)SendMessage(m_HazardTrackMatCombo, CB_GETCURSEL, 0, 0);
	}

	inline EWallObjectType GetWallType() const
	{
		return (EWallObjectType)SendMessage(m_WallTypeCombo, CB_GETCURSEL, 0, 0);
	}

	inline EFloorObjectType GetFloorType() const
	{
		return (EFloorObjectType)SendMessage(m_FloorTypeCombo, CB_GETCURSEL, 0, 0);
	}

	inline EFloorObjectOrientation GetOrientation() const
	{
		return (EFloorObjectOrientation)SendMessage(m_FloorOrientationCombo, CB_GETCURSEL, 0, 0);
	}

	inline int GetAIVision() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_AIVISION, &transfer, TRUE);
	}

	inline int GetWideGoalStartX() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_STARTX, &transfer, TRUE);
	}

	inline int GetWideGoalStartY() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_STARTY, &transfer, TRUE);
	}

	inline int GetWideGoalWidth() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_WIDTH, &transfer, TRUE);
	}

	inline int GetWideGoalHeight() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_HEIGHT, &transfer, TRUE);
	}

	inline int GetGoalStartX() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_STARTX, &transfer, TRUE);
	}

	inline int GetGoalStartY() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_STARTY, &transfer, TRUE);
	}

	inline int GetGoalWidth() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_WIDTH, &transfer, TRUE);
	}

	inline int GetGoalHeight() const
	{
		BOOL transfer;

		return GetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_HEIGHT, &transfer, TRUE);
	}

	inline void SetTrackMaterial(ETileMaterial mat) const
	{
		SendMessage(m_TrackMatCombo, CB_SETCURSEL, (int)mat, 0);
	}

	inline void SetSlowMaterial(ETileMaterial mat) const
	{
		SendMessage(m_SlowTrackMatCombo1, CB_SETCURSEL, (int)mat, 0);
	}

	inline void SetSlowMaterial2(ETileMaterial mat) const
	{
		SendMessage(m_SlowTrackMatCombo2, CB_SETCURSEL, (int)mat, 0);
	}

	inline void SetOuterMaterial(ETileMaterial mat) const
	{
		SendMessage(m_HazardTrackMatCombo, CB_SETCURSEL, (int)mat, 0);
	}

	inline void SetAIVision(int vision)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_AIVISION, vision, TRUE);
	}

	inline void SetWideGoalStartX(int x)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_STARTX, x, TRUE);
	}

	inline void SetWideGoalStartY(int y)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_STARTY, y, TRUE);
	}

	inline void SetWideGoalWidth(int width)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_WIDTH, width, TRUE);
	}

	inline void SetWideGoalHeight(int height)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_WIDEGOAL_HEIGHT, height, TRUE);
	}

	inline void SetGoalStartX(int x)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_STARTX, x, TRUE);
	}

	inline void SetGoalStartY(int y)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_STARTY, y, TRUE);
	}

	inline void SetGoalWidth(int width)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_WIDTH, width, TRUE);
	}

	inline void SetGoalHeight(int height)
	{
		SetDlgItemInt(m_hDlg, IDC_EDIT_GOAL_HEIGHT, height, TRUE);
	}

	inline bool IsOpen() const
	{
		return m_Open;
	}

	inline void SetScene(class CEditorScene* scene)
	{
		m_Scene = scene;
	}

public:
	bool Init(int ID);
	void Show();
	void CreateMap();
	void LoadRefImage();
	void ClearRefImage();
	void Save();
	void Load();
	
public:
	void WallObjectSave();
	void WallObjectLoad();
	
public:
	void FloorObjectSave();
	void FloorObjectLoad();


private:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
};

