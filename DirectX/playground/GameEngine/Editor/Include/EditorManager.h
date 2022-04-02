#pragma once

#include "Gameinfo.h"

enum class EditMode
{
	Scene,
	Sprite,
	TileMap
};

class CEditorManager
{
private:
	EditMode m_EditMode;
	class CDragObject* m_DragObj;
	class CSpriteWindow* m_SpriteWindow;
	class CDetailWindow* m_DetailWindow;
	class CEditorMenu* m_EditorMenu;
	class CTileMapWindow* m_TileMapWindow;
	class CObjectHierarchy* m_ObjectHierarchy;
	Vector2 m_StartPos;
	Vector2 m_EndPos;
	bool m_DragStart;
	bool m_DragEnd;

	bool m_MouseLButtonPush;
	float m_CameraMoveSpeed;

public:
	inline bool GetMouseLButtonPush() const
	{
		return m_MouseLButtonPush;
	}

	inline EditMode GetEditMode() const
	{
		return m_EditMode;
	}

	void SetEditMode(EditMode mode);

	inline class CDragObject* GetDragObject() const
	{
		return m_DragObj;
	}

public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void MouseLButtonDown(float deltaTime);
	void MouseLButtonPush(float deltaTime);
	void MouseLButtonUp(float deltaTime);

	void MouseRButtonPush(float deltaTime);

	void KeyboardUp(float deltaTime);
	void KeyboardDown(float deltaTime);
	void KeyboardLeft(float deltaTime);
	void KeyboardRight(float deltaTime);

	void CameraMoveUp(float deltaTime);
	void CameraMoveDown(float deltaTime);
	void CameraMoveLeft(float deltaTime);
	void CameraMoveRight(float deltaTime);

	void MoveFrameUp(float deltaTime);
	void MoveFrameDown(float deltaTime);
	void MoveFrameLeft(float detlaTime);
	void MoveFrameRight(float deltaTime);

	void MoveFrameStartXRight(float deltaTime);
	void MoveFrameStartXLeft(float deltaTime);
	void MoveFrameStartYUp(float deltaTime);
	void MoveFrameStartYDown(float deltaTime);
	void MoveFrameEndXRight(float deltaTime);
	void MoveFrameEndXLeft(float deltaTime);
	void MoveFrameEndYUp(float deltaTime);
	void MoveFrameEndYDown(float deltaTime);

public:
	void CreateSceneMode(class CScene* scene, size_t type);
	class CGameObject* CreateObject(class CScene* scene, size_t type);
	class CComponent* CreateComponet(class CGameObject* obj, size_t type);
	void CreateAnimInstance(class CSpriteComponent* sprite, size_t type);

	DECLARE_SINGLE(CEditorManager)
};

