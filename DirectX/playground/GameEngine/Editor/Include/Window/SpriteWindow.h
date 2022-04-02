#pragma once

#include "IMGUIWindow.h"

class CSpriteWindow :
	public CIMGUIWindow
{
public:
	CSpriteWindow();
	virtual ~CSpriteWindow();

private:
	class CIMGUIImage* m_Sprite;
	class CIMGUIImage* m_SpriteFrame;
	class CIMGUIListBox* m_AnimationList;
	class CIMGUIListBox* m_AnimationFrameList;
	class CIMGUICheckBox* m_LoopBox;
	class CIMGUICheckBox* m_ReverseBox;
	class CIMGUITextInput* m_AnimName;
	class CIMGUITextInput* m_EditX;
	class CIMGUITextInput* m_EditY;
	class CIMGUITextInput* m_EditSizeX;
	class CIMGUITextInput* m_EditSizeY;
	class CIMGUITextInput* m_StartX;
	class CIMGUITextInput* m_StartY;
	class CIMGUITextInput* m_SizeX;
	class CIMGUITextInput* m_SizeY;
	class CIMGUITextInput* m_PlayTimeInput;
	class CIMGUITextInput* m_PlayScaleInput;

	CSharedPtr<class CSpriteEditObject> m_SpriteObject;
	class CAnimationSequence2DInstance* m_AnimInstance;

	class CIMGUIText* m_MousePos;

	class CIMGUIText* m_DragObjStartPos;
	class CIMGUIText* m_DragObjEndPos;

	class CIMGUIText* m_AtlasStartText;
	class CIMGUIText* m_AtlasEndText;

	class CIMGUIText* m_AtlasSizeText;

	bool m_Loop;
	bool m_Reverse;

	Vector2 m_AtlasStartPos;
	Vector2 m_AtlasEndPos;
	Vector2 m_AtlasSize;

	Vector2 m_RectStartPos;
	Vector2 m_RectEndPos;

public:
	virtual bool Init();
	virtual void Update(float deltaTime);

public:
	void LoadTextureButton();
	void SpriteEditButton();
	void AddAnimationButton();
	void AddAnimationFrameButton();
	void DeleteFrameButton();
	void CreateNewAnimInst();
	void PlayAnimation();
	void StopAnimation();
	void EditFrameButton();

	void EditStartX();
	void EditStartY();
	void EditSizeX();
	void EditSizeY();

	void SetStartX();
	void SetStartY();
	void SetSizeX();
	void SetSizeY();

public:
	void AddFrameShortCut(float deltaTime);
	void DeleteFrameShortCut(float deltaTime);

public:
	void SaveSequence();
	void LoadSequence();
	void LoadAllSequence();
	void SaveAnimationInst();
	void LoadAnimationInst();

public:
	void SetLoop();
	void SetReverse();

	void ChangePlayTime();
	void ChangePlayScale();

public:
	void ChangeAnimation(int index, const char* item);
	void SetFrame(int index, const char* item);

public:
	void CutSprite();
	void CutSpriteByValue();
};

