#pragma once

#include "IMGUIWindow.h"
#include "../EditorInfo.h"
#include "Component/TileMapComponent.h"

class CTileMapWindow :
	public CIMGUIWindow
{
public:
	CTileMapWindow();
	virtual ~CTileMapWindow();

private:
	class CIMGUITextInput* m_CountX;
	class CIMGUITextInput* m_CountY;
	class CIMGUITextInput* m_SizeX;
	class CIMGUITextInput* m_SizeY;
	class CIMGUIComboBox* m_ShapeCombo;
	class CIMGUIButton* m_TileMapCreateButton;
	CSharedPtr<CTileMapComponent> m_TileMap;

private:
	class CIMGUIComboBox* m_TypeCombo;
	class CIMGUIComboBox* m_TileEditCombo;
	class CIMGUITextInput* m_FrameStartX;
	class CIMGUITextInput* m_FrameStartY;
	class CIMGUITextInput* m_FrameEndX;
	class CIMGUITextInput* m_FrameEndY;
	class CIMGUIButton* m_DefaultFrameButton;

private:
	class CIMGUIButton* m_BackTextureSetButton;
	class CIMGUITextInput* m_BackScaleX;
	class CIMGUITextInput* m_BackScaleY;
	class CIMGUITextInput* m_OffsetX;
	class CIMGUITextInput* m_OffsetY;

private:
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_Frame;
	class CIMGUITextInput* m_CutFrameStartX;
	class CIMGUITextInput* m_CutFrameStartY;
	class CIMGUITextInput* m_CutFrameEndX;
	class CIMGUITextInput* m_CutFrameEndY;

private:
	class CIMGUIButton* m_TileMapSaveButton;
	class CIMGUIButton* m_TileMapLoadButton;

public:
	inline void SetTileMap(CTileMapComponent* tileMap)
	{
		m_TileMap = tileMap;
	}

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

private:
	void CountXCallback();
	void CountYCallback();
	void SizeXCallback();
	void SizeYCallback();
	void SetOffsetXCallback();
	void SetOffsetYCallback();
	void TileMapCreateButton();
	void TileMapSaveButton();
	void TileMapLoadButton();

private:
	void TileFrameStartXCallback();
	void TileFrameStartYCallback();
	void TileFrameEndXCallback();
	void TileFrameEndYCallback();

	void LoadTexture();

	void CutTile(const Vector2& start, const Vector2& end);

private:
	void DefaultFrameButton();

private:
	void CreateTileEditControl();
	void CreateFrameControl();
};

