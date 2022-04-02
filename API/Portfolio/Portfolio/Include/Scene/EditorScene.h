#pragma once

#include "Scene.h"

class CEditorScene :
	public CScene
{
	friend class CSceneManager;

private:
	CEditorScene();
	virtual ~CEditorScene();

private:
	bool m_Start;
	class CEditorDlg* m_Dlg;
	class CTileMap* m_TileMap;
	float m_ScrollSpeed;
	CSharedPtr<CTexture> m_RefImage;
	CSharedPtr<CTexture> m_GreenPipe;
	CSharedPtr<CTexture> m_OrangePipe;
	CSharedPtr<CTexture> m_SuperThwomp;
	CSharedPtr<CTexture> m_Thwomp;
	CSharedPtr<CTexture> m_Flower;

	CSharedPtr<CTexture> m_ItemBoxes[(int)EFloorObjectOrientation::END];
	CSharedPtr<CTexture> m_Coins[(int)EFloorObjectOrientation::END];
	CSharedPtr<CTexture> m_Oils[(int)EFloorObjectOrientation::END];
	CSharedPtr<CTexture> m_AccelPanels[(int)EFloorObjectOrientation::END];

	bool m_FixX;
	bool m_FixY;

	float m_FixXPos;
	float m_FixYPos;

public:
	virtual bool Init() override;
	virtual bool Update(float deltaTime) override;
	virtual bool Render(HDC hDC) override;
	virtual bool PostUpdate(float deltaTime) override;

public:
	void OnEditor(float deltaTime);
	void CreateTileMap();
	void SetTileInfo(int countX, int countY, int sizeX, int sizeY);
	void LoadRefImage(const std::string& name);
	void ClearRefImage();

public:
	void CameraMoveUp(float deltaTime);
	void CameraMoveDown(float deltaTime);
	void CameraMoveLeft(float deltaTime);
	void CameraMoveRight(float deltaTime);

	void MouseLButton(float deltaTime);
	void MouseRButton(float deltaTime);

	void InstantiateObject(float deltaTime);
	void DeleteObject(float deltaTime);
	void HideGrid(float deltaTime);

	void SetFixX(float deltaTime);

	void ResetFixX(float deltaTime);

	void SetFixY(float deltaTime);

	void ResetFixY(float deltaTime);

	void BackToTitle(float deltaTime);

public:
	void Save(const char* fullPath);
	void Load(const char* fullPath);

public:
	void SaveWallObjects(const char* fullPath);
	void LoadWallObjects(const char* fullPath);

public:
	void SaveFloorObjects(const char* fullPath);
	void LoadFloorObjects(const char* fullPath);
};

