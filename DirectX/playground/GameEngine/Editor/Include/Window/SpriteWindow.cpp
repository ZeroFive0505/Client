#include "SpriteWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIImage.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/SpriteComponent.h"
#include "Input.h"
#include "IMGUITreeNode.h"
#include "../Object/DragObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "Resource/Animation/AnimationSequence2D.h"

static int CALLBACK BrowserCallbackProc(HWND hWnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
	switch (uMsg)
	{
	case BFFM_INITIALIZED:
	{
		SendMessage(hWnd, BFFM_SETSELECTION, TRUE, (LPARAM)lpData);
	}
	break;
	}


	return 0;
}

CSpriteWindow::CSpriteWindow() :
	m_Sprite(nullptr),
	m_SpriteObject(nullptr),
	m_AnimationFrameList(nullptr),
	m_AnimationList(nullptr),
	m_MousePos(nullptr),
	m_SpriteFrame(nullptr),
	m_DragObjStartPos(nullptr),
	m_DragObjEndPos(nullptr),
	m_AtlasStartText(nullptr),
	m_AtlasEndText(nullptr),
	m_AtlasSizeText(nullptr),
	m_AnimName(nullptr),
	m_AnimInstance(nullptr),
	m_EditX(nullptr),
	m_EditY(nullptr),
	m_EditSizeX(nullptr),
	m_EditSizeY(nullptr),
	m_PlayTimeInput(nullptr),
	m_PlayScaleInput(nullptr),
	m_LoopBox(nullptr),
	m_ReverseBox(nullptr),
	m_Loop(true),
	m_Reverse(false),
	m_StartX(nullptr),
	m_StartY(nullptr),
	m_SizeX(nullptr),
	m_SizeY(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
	SAFE_DELETE(m_AnimInstance);
}

bool CSpriteWindow::Init()
{
	CIMGUIWindow::Init();

	CInput::GetInst()->SetCallback<CSpriteWindow>("AddFrameShortCut", KeyState_Down, this, &CSpriteWindow::AddFrameShortCut);
	CInput::GetInst()->SetCallback<CSpriteWindow>("DeleteLastFrameShortCut", KeyState_Down, this, &CSpriteWindow::DeleteFrameShortCut);

	CIMGUIButton* button = AddWidget<CIMGUIButton>("LoadTexture", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadTextureButton);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("SpriteEdit", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SpriteEditButton);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("SaveSequence", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SaveSequence);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("LoadSequence", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadSequence);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("Load All Seq", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadAllSequence);

	line = AddWidget<CIMGUISameLine>("Line");
	
	button = AddWidget<CIMGUIButton>("SaveAnimInst", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SaveAnimationInst);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("LoadAnimInst", 100.0f, 50.0f);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadAnimationInst);

	line = AddWidget<CIMGUISameLine>("Line");

	button = AddWidget<CIMGUIButton>("CreateNewAnimInst", 100.0f, 50.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::CreateNewAnimInst);

	m_MousePos = AddWidget<CIMGUIText>("Mouse Position", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	button = button = AddWidget<CIMGUIButton>("EditFrame", 70.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::EditFrameButton);

	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

	m_DragObjStartPos = AddWidget<CIMGUIText>("Mouse Start", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_EditX = AddWidget<CIMGUITextInput>("EditX", 50.0f, 30.0f);

	m_EditX->SetTextType(ImGuiText_Type::Float);
	m_EditX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::EditStartX);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(400.0f);

	m_EditY = AddWidget<CIMGUITextInput>("EditY", 50.0f, 30.0f);
	m_EditY->SetTextType(ImGuiText_Type::Float);
	m_EditY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::EditStartY);



	m_DragObjStartPos->SetText("Drag Start X, Y : ");

	m_DragObjEndPos = AddWidget<CIMGUIText>("Mouse End", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_EditSizeX = AddWidget<CIMGUITextInput>("EditSizeX", 50.0f, 30.0f);
	m_EditSizeX->SetTextType(ImGuiText_Type::Float);
	m_EditSizeX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::EditSizeX);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(400.0f);

	m_EditSizeY = AddWidget<CIMGUITextInput>("EditSizeY", 50.0f, 30.0f);
	m_EditSizeY->SetTextType(ImGuiText_Type::Float);
	m_EditSizeY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::EditSizeY);

	m_DragObjEndPos->SetText("Drag End X, Y : ");

	m_AtlasStartText = AddWidget<CIMGUIText>("Atlas Start : ", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);
	button = AddWidget<CIMGUIButton>("Cut Frame", 70.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

	m_AtlasEndText = AddWidget<CIMGUIText>("Atlas End : ", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_StartX = AddWidget<CIMGUITextInput>("Start X", 50.0f, 30.0f);
	m_StartX->SetTextType(ImGuiText_Type::Float);
	m_StartX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::SetStartX);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(400.0f);

	m_StartY = AddWidget<CIMGUITextInput>("Start Y", 50.0f, 30.0f);
	m_StartY->SetTextType(ImGuiText_Type::Float);
	m_StartY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::SetStartY);

	m_AtlasSizeText = AddWidget<CIMGUIText>("Atlas Size : ", 100.0f, 100.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_SizeX = AddWidget<CIMGUITextInput>("Size X", 50.0f, 30.0f);
	m_SizeX->SetTextType(ImGuiText_Type::Float);
	m_SizeX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::SetSizeX);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(400.0f);

	m_SizeY = AddWidget<CIMGUITextInput>("Size Y", 50.0f, 30.0f);
	m_SizeY->SetTextType(ImGuiText_Type::Float);
	m_SizeY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::SetSizeY);

	m_Sprite = AddWidget<CIMGUIImage>("SpriteOrigin", 200.0f, 200.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	m_SpriteFrame = AddWidget<CIMGUIImage>("Cutted Sprite", 200.0f, 200.0f);

	CIMGUILabel* label = AddWidget<CIMGUILabel>("Animation", 200.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(300.0f);

	label = AddWidget<CIMGUILabel>("Current Frame", 200.0f, 30.0f);

	label->SetColor(0, 0, 255);
	label->SetAlign(0.5f, 0.0f);

	CIMGUIText* text = AddWidget<CIMGUIText>("AnimNameLabel");
	text->SetText("Animation name");

	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(120.0f);

	m_AnimName = AddWidget<CIMGUITextInput>("AnimationName");
	m_AnimName->SetHideName(true);

	button = AddWidget<CIMGUIButton>("AddAnimation", 100.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationButton);


	m_LoopBox = AddWidget<CIMGUICheckBox>("Loop On/Off");
	m_LoopBox->SetSameLine(true);
	m_LoopBox->AddItem("Loop");
	m_LoopBox->SetVal(0, true);
	m_LoopBox->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SetLoop, 0);

	m_ReverseBox = AddWidget<CIMGUICheckBox>("Reverse On/Off");
	m_ReverseBox->SetSameLine(true);
	m_ReverseBox->AddItem("Reverse");
	m_ReverseBox->SetVal(0, false);
	m_ReverseBox->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SetReverse, 0);

	m_PlayTimeInput = AddWidget<CIMGUITextInput>("PlayTime");
	m_PlayTimeInput->SetTextType(ImGuiText_Type::Float);
	m_PlayTimeInput->SetCallback<CSpriteWindow>(this, &CSpriteWindow::ChangePlayTime);
	m_PlayTimeInput->SetFloat(1.0f);

	m_PlayScaleInput = AddWidget<CIMGUITextInput>("PlayScale");
	m_PlayScaleInput->SetTextType(ImGuiText_Type::Float);
	m_PlayScaleInput->SetCallback<CSpriteWindow>(this, &CSpriteWindow::ChangePlayScale);
	m_PlayScaleInput->SetFloat(1.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	button = AddWidget<CIMGUIButton>("AddAnimtionFrame", 150.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

	button = AddWidget<CIMGUIButton>("Play", 50.0, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::PlayAnimation);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(100.0f);

	button = AddWidget<CIMGUIButton>("Stop", 50.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::StopAnimation);

	line = AddWidget<CIMGUISameLine>("Line");

	line->SetOffsetX(300.0f);

	button = AddWidget<CIMGUIButton>("DeleteAnimFrame", 150.0f, 30.0f);
	button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteFrameButton);

	m_AnimationList = AddWidget<CIMGUIListBox>("AnimationList", 200.0f, 300.0f);
	m_AnimationList->SetHideName(true);
	m_AnimationList->SetPageItemCount(6);

	m_AnimationList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::ChangeAnimation);

	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(300.0f);

	line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(300.0f);


	m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 200.0f, 300.0f);
	m_AnimationFrameList->SetHideName(true);
	m_AnimationFrameList->SetPageItemCount(6);

	m_AnimationFrameList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SetFrame);

	m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->Init();

	m_AnimInstance->Start();

	m_AnimInstance->Stop();

	return true;
}

void CSpriteWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);

	Vector2 pos = CInput::GetInst()->GetMouseWorld2DPos();

	std::string currnetPos = "X : " + std::to_string(pos.x) + ", " + "Y : " + std::to_string(pos.y);

	m_MousePos->SetText(currnetPos.c_str());
	
	m_AnimInstance->Update(deltaTime);

	if (m_AnimInstance->IsPlay())
	{
		CAnimationSequence2DData* animData = m_AnimInstance->GetCurrentAnimation();

		if (animData->GetAnimationSequence()->GetFrameCount() > 0)
		{
			int frame = animData->GetCurrentFrame();

			sAnimationFrameData frameData = animData->GetAnimationSequence()->GetFrameData(frame);

			m_Sprite->SetImageStart(frameData.start.x, frameData.start.y);
			m_Sprite->SetImageEnd(frameData.start.x + frameData.size.x, frameData.start.y + frameData.size.y);

		}
	}
}

void CSpriteWindow::LoadTextureButton()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Sprite)
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

		m_Sprite->SetTextureFullPath(convertFileName, filePath);
		m_SpriteFrame->SetTextureFullPath(convertFileName, filePath);

		m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel,
			convertFileName, filePath);

		m_SpriteObject->GetSpriteComponent()->SetWorldScale((float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.0f);

		if (m_AnimInstance->GetCurrentAnimation())
		{
			m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->SetTexture(m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture());
		}

		sResolution rs = CEngine::GetInst()->GetResolution();

		m_SpriteObject->SetPivot(0.0f, 1.0f, 0.0f);

		m_SpriteObject->SetWorldPos(0.0f, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 0.0f);

		m_DragObjStartPos->SetText("Drag Object Start (X, Y) : ");
		m_DragObjEndPos->SetText("Drag Object End (X, Y) : ");

		std::string temp;

		m_AtlasStartPos = Vector2(m_SpriteObject->GetRelativePos().x, m_SpriteObject->GetRelativePos().y);

		temp = "Atlas texture start (X, Y) : " + std::to_string(m_AtlasStartPos.x) + ", " + std::to_string(m_AtlasStartPos.y);

		m_AtlasStartText->SetText(temp.c_str());
		m_SpriteObject->GetRelativeScale();
		m_AtlasEndPos = m_AtlasStartPos + Vector2(m_SpriteObject->GetWorldScale().x, m_SpriteObject->GetWorldScale().y);

		temp = "Atlas texture end (X, Y) : " + std::to_string(m_AtlasEndPos.x) + ", " + std::to_string(m_AtlasEndPos.y);

		m_AtlasEndText->SetText(temp.c_str());

		m_AtlasSize = m_AtlasEndPos - m_AtlasStartPos;

		temp = "Atlas texture size (X, Y) : " + std::to_string(m_AtlasSize.x) + ", " + std::to_string(m_AtlasSize.y);

		m_AtlasSizeText->SetText(temp.c_str());
	}
}

void CSpriteWindow::ChangeAnimation(int index, const char* item)
{
	m_AnimInstance->SetCurrentAnimation(item);

	m_LoopBox->SetVal(0, m_AnimInstance->GetCurrentAnimation()->IsLoop());
	m_ReverseBox->SetVal(0, m_AnimInstance->GetCurrentAnimation()->IsReverse());
	m_PlayScaleInput->SetFloat(m_AnimInstance->GetCurrentAnimation()->GetPlayScale());
	m_PlayTimeInput->SetFloat(m_AnimInstance->GetCurrentAnimation()->GetPlayTime());

	m_SpriteFrame->SetTexture(m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());
	m_Sprite->SetTexture(m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());
	m_SpriteObject->GetSpriteComponent()->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel,
		m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetName(), m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());

	m_SpriteObject->GetSpriteComponent()->SetWorldScale((float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
		(float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.0f);

	sResolution rs = CEngine::GetInst()->GetResolution();

	m_SpriteObject->SetPivot(0.0f, 1.0f, 0.0f);

	m_SpriteObject->SetRelativePos(0.0f, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 0.0f);

	std::string temp;

	m_AtlasStartPos = Vector2(m_SpriteObject->GetRelativePos().x, m_SpriteObject->GetRelativePos().y);

	temp = "Atlas texture start (X, Y) : " + std::to_string(m_AtlasStartPos.x) + ", " + std::to_string(m_AtlasStartPos.y);

	m_AtlasStartText->SetText(temp.c_str());

	m_AtlasEndPos = m_AtlasStartPos + Vector2(m_SpriteObject->GetRelativeScale().x, m_SpriteObject->GetRelativeScale().y);

	temp = "Atlas texture end (X, Y) : " + std::to_string(m_AtlasEndPos.x) + ", " + std::to_string(m_AtlasEndPos.y);

	m_AtlasEndText->SetText(temp.c_str());

	m_AtlasSize = m_AtlasEndPos - m_AtlasStartPos;

	temp = "Atlas texture size (X, Y) : " + std::to_string(m_AtlasSize.x) + ", " + std::to_string(m_AtlasSize.y);

	m_AtlasSizeText->SetText(temp.c_str());

	m_AnimationFrameList->Clear();

	int frameCount = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount();

	for (int i = 0; i < frameCount; i++)
	{
		m_AnimationFrameList->AddItem(std::to_string(i));
	}

	if (frameCount > 0)
	{
		sAnimationFrameData data = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(0);

		m_SpriteFrame->SetImageStart(data.start.x, data.start.y);
		m_SpriteFrame->SetImageEnd(data.start.x + data.size.x, data.start.y + data.size.y);
	}
}

void CSpriteWindow::SpriteEditButton()
{
	CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

	if (!m_SpriteObject)
		m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");

	m_DragObjStartPos->SetText("Drag Object Start (X, Y) : ");
	m_DragObjEndPos->SetText("Drag Object End (X, Y) : ");
}

void CSpriteWindow::AddAnimationButton()
{
	if (m_AnimName->Empty())
		return;

	const char* text = m_AnimName->GetTextMultibyte();

	if (m_AnimationList->CheckItem(text))
		return;

	CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

	if (!sceneResource->CreateAnimationSequence2D(text, m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
		return;

	m_AnimationList->AddItem(text);

	m_AnimInstance->AddAnimation(text, text, true, 1.0f, 
		1.0f, false);
}

void CSpriteWindow::AddAnimationFrameButton()
{
	int selectIdx = m_AnimationList->GetSelectIndex();

	if (selectIdx == -1)
		return;

	CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

	CAnimationSequence2D* anim = sceneResource->FindAnimationSequence2D(m_AnimationList->GetItem(selectIdx));

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	Vector2 size = dragObj->GetEndPos() - dragObj->GetStartPos();

	size.x = abs(size.x);
	size.y = abs(size.y);

	if (size.x < 0.0f)
		size.x = 0.0f;

	if (size.x > m_AtlasSize.x)
		size.x = m_AtlasSize.x;

	if (size.y < 0.0f)
		size.y = 0.0f;

	if (size.y > m_AtlasSize.y)
		size.y = m_AtlasSize.y;

	anim->AddFrame(m_RectStartPos, m_RectEndPos - m_RectStartPos);

	int frameCount = anim->GetFrameCount() - 1;

	char frameName[32] = {};

	sprintf_s(frameName, "%d", frameCount);

	m_AnimationFrameList->AddItem(frameName);
}

void CSpriteWindow::DeleteFrameButton()
{
	int frameIdx = m_AnimationFrameList->GetSelectIndex();

	if (frameIdx == -1)
		return;

	int animIdx = m_AnimationList->GetSelectIndex();

	if (animIdx == -1)
		return;

	CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

	CAnimationSequence2D* anim = sceneResource->FindAnimationSequence2D(m_AnimationList->GetItem(animIdx));

	anim->DeleteFrame(frameIdx);

	m_AnimationFrameList->DeleteItem(frameIdx);

	m_AnimationFrameList->Clear();

	int frameCount = anim->GetFrameCount();

	for (int i = 0; i < frameCount; i++)
	{
		m_AnimationFrameList->AddItem(std::to_string(i));
	}
}

void CSpriteWindow::CreateNewAnimInst()
{
	if (m_AnimInstance->GetAnimationCount() > 1)
	{
		SaveAnimationInst();
	}

	SAFE_DELETE(m_AnimInstance);

	m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->Init();

	m_AnimInstance->Start();

	m_AnimInstance->Stop();

	m_AnimationList->Clear();
	m_AnimationFrameList->Clear();
}

void CSpriteWindow::PlayAnimation()
{
	if (!m_SpriteObject)
		return;

	m_AnimInstance->Play();
}

void CSpriteWindow::StopAnimation()
{
	if (!m_SpriteObject)
		return;

	m_AnimInstance->Stop();
}

void CSpriteWindow::EditFrameButton()
{
	int animIdx = m_AnimationList->GetSelectIndex();

	if (animIdx == -1)
		return;

	int frameIdx = m_AnimationFrameList->GetSelectIndex();

	if (frameIdx == -1)
		return;

	CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

	CAnimationSequence2D* anim = sceneResource->FindAnimationSequence2D(m_AnimationList->GetItem(animIdx));

	sAnimationFrameData data = anim->GetFrameData(frameIdx);

	data.start.x = m_RectStartPos.x;
	data.start.y = m_RectStartPos.y;
	data.size.x = m_RectEndPos.x - m_RectStartPos.x;
	data.size.y = m_RectEndPos.y - m_RectStartPos.y;

	anim->SetFrameData(frameIdx, data);
}

void CSpriteWindow::EditSizeX()
{
	float x = m_EditSizeX->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	
	if (dragObj)
	{
		Vector2 startPos = Vector2(m_RectStartPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - m_RectStartPos.y);
		dragObj->SetStartPos(startPos);
		Vector2 endPos = Vector2(x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - m_RectEndPos.y);
		dragObj->SetEndPos(endPos);
		CutSprite();
	}

}

void CSpriteWindow::EditStartX()
{
	float x = m_EditX->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();
	if (dragObj)
	{
		Vector2 startPos = Vector2(x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - m_RectStartPos.y);
		dragObj->SetStartPos(startPos);
		Vector2 endPos = Vector2(m_RectEndPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - m_RectEndPos.y);
		dragObj->SetEndPos(endPos);
		CutSprite();
	}
}

void CSpriteWindow::EditStartY()
{
	float y = m_EditY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(m_RectStartPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - y);
		dragObj->SetStartPos(startPos);
		Vector2 endPos = Vector2(m_RectEndPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - m_RectEndPos.y);
		dragObj->SetEndPos(endPos);
		CutSprite();
	}
}


void CSpriteWindow::EditSizeY()
{

	float y = m_EditSizeY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(m_RectStartPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - y);
		dragObj->SetStartPos(startPos);
		Vector2 endPos = Vector2(m_RectEndPos.x, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - y);
		dragObj->SetEndPos(endPos);
		CutSprite();
	}
}

void CSpriteWindow::SetStartX()
{
	float startX = m_StartX->GetValueFloat();
	float startY = m_StartY->GetValueFloat();
	float sizeX = m_SizeX->GetValueFloat();
	float sizeY = m_SizeY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(startX, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - startY);
		Vector2 size = Vector2(startX + sizeX, startPos.y - sizeY);

		dragObj->SetStartPos(startPos);
		dragObj->SetEndPos(size);

		CutSpriteByValue();
	}
}

void CSpriteWindow::SetStartY()
{
	float startX = m_StartX->GetValueFloat();
	float startY = m_StartY->GetValueFloat();
	float sizeX = m_SizeX->GetValueFloat();
	float sizeY = m_SizeY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(startX, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - startY);
		Vector2 size = Vector2(startX + sizeX, startPos.y - sizeY);

		dragObj->SetStartPos(startPos);
		dragObj->SetEndPos(size);

		CutSpriteByValue();
	}
}

void CSpriteWindow::SetSizeX()
{
	float startX = m_StartX->GetValueFloat();
	float startY = m_StartY->GetValueFloat();
	float sizeX = m_SizeX->GetValueFloat();
	float sizeY = m_SizeY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(startX, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - startY);
		Vector2 size = Vector2(startX + sizeX, startPos.y - sizeY);

		dragObj->SetStartPos(startPos);
		dragObj->SetEndPos(size);

		CutSpriteByValue();
	}
}

void CSpriteWindow::SetSizeY()
{
	float startX = m_StartX->GetValueFloat();
	float startY = m_StartY->GetValueFloat();
	float sizeX = m_SizeX->GetValueFloat();
	float sizeY = m_SizeY->GetValueFloat();

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{
		Vector2 startPos = Vector2(startX, (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - startY);
		Vector2 size = Vector2(startX + sizeX, startPos.y - sizeY);

		dragObj->SetStartPos(startPos);
		dragObj->SetEndPos(size);

		CutSpriteByValue();
	}
}

void CSpriteWindow::AddFrameShortCut(float deltaTime)
{
	AddAnimationFrameButton();
}

void CSpriteWindow::DeleteFrameShortCut(float deltaTime)
{
	if (m_AnimationFrameList->GetItemCount() > 0)
	{
		int animIdx = m_AnimationList->GetSelectIndex();

		if (animIdx == -1)
			return;

		int frameIdx = m_AnimationFrameList->GetItemCount() - 1;

		m_AnimationFrameList->DeleteItem(m_AnimationFrameList->GetItemCount() - 1);

		CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

		CAnimationSequence2D* anim = sceneResource->FindAnimationSequence2D(m_AnimationList->GetItem(animIdx));

		anim->DeleteFrame(frameIdx);
	}
}


void CSpriteWindow::SetLoop()
{
	m_Loop = !m_Loop;

	if (m_AnimInstance->GetCurrentAnimation())
	{
		m_AnimInstance->SetLoop(m_AnimInstance->GetCurrentAnimation()->GetName(), m_Loop);
	}

}

void CSpriteWindow::SetReverse()
{
	m_Reverse = !m_Reverse;

	if (m_AnimInstance->GetCurrentAnimation())
	{
		m_AnimInstance->SetReverse(m_AnimInstance->GetCurrentAnimation()->GetName(), m_Reverse);
	}
}

void CSpriteWindow::ChangePlayTime()
{
	if (m_AnimInstance->GetCurrentAnimation())
	{
		m_AnimInstance->SetPlayTime(m_AnimInstance->GetCurrentAnimation()->GetName(), m_PlayTimeInput->GetValueFloat());
	}
}

void CSpriteWindow::ChangePlayScale()
{
	if (m_AnimInstance->GetCurrentAnimation())
	{
		m_AnimInstance->SetPlayScale(m_AnimInstance->GetCurrentAnimation()->GetName(), m_PlayScaleInput->GetValueFloat());
	}
}

void CSpriteWindow::SaveSequence()
{
	int animIdx = m_AnimationList->GetSelectIndex();

	if (animIdx == -1)
		return;

	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("Sequence File\0*.sqc\0All Files\0*.*");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;

	if (GetSaveFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

		// 전체 경로를 멀티바이트로 바꾼다.
		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);

		// 전체경로 기준으로 현재 애니메이션의 시퀀스를 저장한다.
		m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(fullPath);
	}
}

void CSpriteWindow::LoadSequence()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("Sequence File\0*.sqc\0All Files\0*.*");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;

	if (GetOpenFileName(&openFile) != 0)
	{
		char fullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);

		CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

		std::string sequenceName;

		// 전체 경로 기준으로 시퀀스를 불러온다.
		sceneResource->LoadSequence2DFullPath(sequenceName, fullPath);
	}
}

void CSpriteWindow::LoadAllSequence()
{
	TCHAR path[MAX_PATH] = {};

	LPITEMIDLIST pDirList;

	BROWSEINFO browseInfo = {};

	const TCHAR* initDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;

	browseInfo.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	browseInfo.pidlRoot = nullptr;
	browseInfo.lpszTitle = TEXT("Browse sequence folder");
	browseInfo.pszDisplayName = path;
	browseInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	browseInfo.lpfn = BrowserCallbackProc;
	browseInfo.lParam = (LPARAM)initDir;

	pDirList = SHBrowseForFolder(&browseInfo);

	if (pDirList == nullptr)
		return;

	SHGetPathFromIDList(pDirList, path);

	WIN32_FIND_DATAA data;

	char multibytePath[MAX_PATH] = {};

	int length = WideCharToMultiByte(CP_ACP, 0, path, -1, 0, 0, 0, 0);
	WideCharToMultiByte(CP_ACP, 0, path, -1, multibytePath, length, 0, 0);
	
	std::string targetName = multibytePath;
	targetName += "\\*.sqc";

	HANDLE hFind = FindFirstFileA(targetName.c_str(), &data);

	if (hFind == INVALID_HANDLE_VALUE)
		return;

	do
	{
		CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

		std::string sequenceName;

		std::string fullPath = multibytePath;

		fullPath += '\\';
		fullPath += data.cFileName;

		// 전체 경로 기준으로 시퀀스를 불러온다.
		sceneResource->LoadSequence2DFullPath(sequenceName, fullPath.c_str());

	} while (FindNextFileA(hFind, &data));

	FindClose(hFind);
}

void CSpriteWindow::SaveAnimationInst()
{
	if (m_AnimInstance->GetAnimationCount() >= 1)
	{
		TCHAR filePath[MAX_PATH] = {};

		OPENFILENAME openFile = {};

		openFile.lStructSize = sizeof(OPENFILENAME);
		openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
		openFile.lpstrFilter = TEXT("Anim Inst File\0*.anim\0All Files\0*.*");
		openFile.lpstrFile = filePath;
		openFile.nMaxFile = MAX_PATH;
		openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;

		if (GetSaveFileName(&openFile) != 0)
		{
			FILE* pFile = nullptr;

			char fullPath[MAX_PATH] = {};

			// 전체 경로를 멀티바이트로 바꾼다.
			int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);

			// 전체경로 기준으로  저장한다.
			m_AnimInstance->Save(fullPath);
		}
	}
}

void CSpriteWindow::LoadAnimationInst()
{
	TCHAR filePath[MAX_PATH] = {};

	OPENFILENAME openFile = {};

	openFile.lStructSize = sizeof(OPENFILENAME);
	openFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	openFile.lpstrFilter = TEXT("Anim Inst File\0*.anim\0All Files\0*.*");
	openFile.lpstrFile = filePath;
	openFile.nMaxFile = MAX_PATH;
	openFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->path;

	if (GetOpenFileName(&openFile) != 0)
	{
		FILE* pFile = nullptr;

		char fullPath[MAX_PATH] = {};

		int length = WideCharToMultiByte(CP_ACP, 0, filePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, filePath, -1, fullPath, length, 0, 0);

		SAFE_DELETE(m_AnimInstance);

		m_AnimInstance = new CAnimationSequence2DInstance;

		m_AnimInstance->Init();

		m_AnimInstance->Start();

		m_AnimInstance->Stop();
		m_AnimInstance->Load(fullPath);

		m_AnimationList->Clear();
		m_AnimationFrameList->Clear();

		auto iter = m_AnimInstance->GetMapAnim().begin();
		auto iterEnd = m_AnimInstance->GetMapAnim().end();

		CSceneResource* resource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

		for (; iter != iterEnd; iter++)
		{
			m_AnimationList->AddItem(iter->first);
		}

		TCHAR fileName[MAX_PATH] = {};

		_wsplitpath_s(filePath, 0, 0, 0, 0, fileName, MAX_PATH, 0, 0);

		char convertFileName[MAX_PATH] = {};

		length = WideCharToMultiByte(CP_ACP, 0, fileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, fileName, -1, convertFileName, length, 0, 0);

		m_SpriteObject->GetSpriteComponent()->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetName(),
			m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());

		m_SpriteObject->GetSpriteComponent()->SetWorldScale((float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
			(float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.0f);

		m_Sprite->SetTexture(m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());
		m_SpriteFrame->SetTexture(m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());

		m_AnimationList->ResetSelectedIndex();
	}
}

void CSpriteWindow::SetFrame(int index, const char* item)
{
	int animIndex = m_AnimationList->GetSelectIndex();

	if (animIndex == -1)
		return;

	CSceneResource* sceneResource = CSceneManager::GetInst()->GetScene()->GetSceneResource();

	CAnimationSequence2D* anim = sceneResource->FindAnimationSequence2D(m_AnimationList->GetItem(animIndex));
	sAnimationFrameData data = anim->GetFrameData(index);

	m_RectStartPos = data.start;
	m_RectEndPos = data.start + data.size;

	std::string currnetPos;

	currnetPos = "Drag Object Start (X, Y) : " + std::to_string(m_RectStartPos.x) + ", " + std::to_string(m_RectStartPos.y);

	m_DragObjStartPos->SetText(currnetPos.c_str());

	currnetPos = "Drag Object End (X, Y) : " + std::to_string(m_RectEndPos.x) + ", " + std::to_string(m_RectEndPos.y);

	m_DragObjEndPos->SetText(currnetPos.c_str());

	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	if (dragObj)
	{

		Vector2 startPos, endPos;

		startPos.x = data.start.x + m_SpriteObject->GetWorldPos().x;
		startPos.y = (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight() - data.start.y;

		endPos.x = startPos.x + data.size.x;
		endPos.y = startPos.y - data.size.y;

		dragObj->SetStartPos(startPos);
		dragObj->SetEndPos(endPos);
	}

	m_SpriteFrame->SetImageStart(m_RectStartPos.x, m_RectStartPos.y);
	m_SpriteFrame->SetImageEnd(m_RectEndPos.x, m_RectEndPos.y);

	m_EditX->SetFloat(m_RectStartPos.x);

	m_EditY->SetFloat(m_RectStartPos.y);

	m_EditSizeX->SetFloat(m_RectEndPos.x);

	m_EditSizeY->SetFloat(m_RectEndPos.y);
}

void CSpriteWindow::CutSprite()
{
	if (CEditorManager::GetInst()->GetEditMode() != EditMode::Sprite)
		return;

	Vector2 startPos = Vector2(CEditorManager::GetInst()->GetDragObject()->GetWorldPos().x,
		CEditorManager::GetInst()->GetDragObject()->GetWorldPos().y);
	Vector2 endPos = startPos + Vector2(CEditorManager::GetInst()->GetDragObject()->GetWorldScale().x,
		CEditorManager::GetInst()->GetDragObject()->GetWorldScale().y);

	// 시작점과 끝점이 바뀌었을 경우가 있으니 교체한다.
	m_RectStartPos.x = startPos.x < endPos.x ? startPos.x : endPos.x;
	m_RectStartPos.y = startPos.y > endPos.y ? startPos.y : endPos.y;

	m_RectEndPos.x = startPos.x < endPos.x ? endPos.x : startPos.x;
	m_RectEndPos.y = startPos.y > endPos.y ? endPos.y : startPos.y;

	bool sizeOver = false;

	std::string currentPos;

	currentPos = "Drag Object Start (X, Y) : " + std::to_string(m_RectStartPos.x) + ", " + std::to_string(m_RectStartPos.y);

	m_DragObjStartPos->SetText(currentPos.c_str());

	currentPos = "Drag Object End (X, Y) : " + std::to_string(m_RectEndPos.x) + ", " + std::to_string(m_RectEndPos.y);

	m_DragObjEndPos->SetText(currentPos.c_str());

	// 일단 한번 Clamp를 연산을 한다.
	// 더 큰 쪽을 기준으로 Clamp를 한다. 만약 텍스처의 크기가 화면의 해상도보다 클 경우는 텍스쳐
	// 아닐 경우는 해상도
	sResolution rs = CEngine::GetInst()->GetResolution();

	if ((float)rs.width > m_AtlasSize.x)
	{
		m_RectStartPos.x = std::fmax(0.0f, std::fmin(m_RectStartPos.x, 
			(float)rs.width));
	}
	else if ((float)rs.width < m_AtlasSize.x)
	{
		m_RectStartPos.x = std::fmax(0.0f, std::fmin(m_RectStartPos.x,
			m_AtlasSize.x));
	}

	
	if ((float)rs.height > m_AtlasSize.y)
	{
		m_RectStartPos.y = std::fmax(0.0f, std::fmin(m_RectStartPos.y, 
			(float)rs.height));
	}
	else if ((float)rs.height < m_AtlasSize.y)
	{
		m_RectStartPos.y = std::fmax(0.0f, std::fmin(m_RectStartPos.y,
			m_AtlasSize.y));
	}

	if ((float)rs.width > m_AtlasSize.x)
	{
		m_RectEndPos.x = std::fmax(0.0f, std::fmin(m_RectEndPos.x, 
			(float)rs.width));
	}
	else if ((float)rs.width < m_AtlasSize.x)
	{
		m_RectEndPos.x = std::fmax(0.0f, std::fmin(m_RectEndPos.x,
			m_AtlasSize.x));
	}

	if ((float)rs.height > m_AtlasSize.y)
	{
		m_RectEndPos.y = std::fmax(0.0f, std::fmin(m_RectEndPos.y, 
			(float)rs.height));
	}
	else if ((float)rs.height < m_AtlasSize.y)
	{
		m_RectEndPos.y = std::fmax(0.0f, std::fmin(m_RectEndPos.y,
			m_AtlasSize.y));
	}

	Vector2 textureSize = Vector2(m_RectEndPos.x - m_RectStartPos.x, m_RectStartPos.y - m_RectEndPos.y);

	// 스프라이트의 위치를 이용해서 최종적으로 이미지 공간에서의 위치로 변환한다.
	startPos = Vector2(m_RectStartPos.x + m_SpriteObject->GetSpriteComponent()->GetWorldPos().x, 
		m_SpriteObject->GetSpriteComponent()->GetWorldPos().y - m_RectStartPos.y);
	endPos = startPos + textureSize;

	// 이 경우에는 텍스쳐의 크기로 Clamp를 한다.
	startPos.x = std::fmax(0.0f, std::fmin(startPos.x, m_AtlasSize.x));
	startPos.y = std::fmax(0.0f, std::fmin(startPos.y, m_AtlasSize.y));

	endPos.x = std::fmax(0.0f, std::fmin(endPos.x, m_AtlasSize.x));
	endPos.y = std::fmax(0.0f, std::fmin(endPos.y, m_AtlasSize.y));

	// 최종적으로 정해진 좌표를 이용해서 이미지를 출력하고 설정한다.
	m_SpriteFrame->SetImageStart(startPos.x, startPos.y);
	m_SpriteFrame->SetImageEnd(endPos.x, endPos.y);
	m_RectStartPos = startPos;
	m_RectEndPos = endPos;

	m_EditX->SetFloat(m_RectStartPos.x);

	m_EditY->SetFloat(m_RectStartPos.y);

	m_EditSizeX->SetFloat(m_RectEndPos.x);

	m_EditSizeY->SetFloat(m_RectEndPos.y);
}

void CSpriteWindow::CutSpriteByValue()
{
	CDragObject* dragObj = CEditorManager::GetInst()->GetDragObject();

	Vector2 startPos = Vector2(dragObj->GetWorldPos().x,
		CEditorManager::GetInst()->GetDragObject()->GetWorldPos().y);
	Vector2 endPos = startPos + Vector2(dragObj->GetWorldScale().x,
		dragObj->GetWorldScale().y);

	m_RectStartPos.x = startPos.x < endPos.x ? startPos.x : endPos.x;
	m_RectStartPos.y = startPos.y > endPos.y ? startPos.y : endPos.y;

	m_RectEndPos.x = startPos.x < endPos.x ? endPos.x : startPos.x;
	m_RectEndPos.y = startPos.y > endPos.y ? endPos.y : startPos.y;

	std::string currnetPos;

	// 스프라이트의 위치를 이용해서 최종적으로 이미지 공간에서의 위치로 변환한다.
	startPos = Vector2(m_RectStartPos.x + m_SpriteObject->GetSpriteComponent()->GetWorldPos().x,
		m_SpriteObject->GetSpriteComponent()->GetWorldPos().y - m_RectStartPos.y);

	Vector2 textureSize = Vector2(m_RectEndPos.x - m_RectStartPos.x, m_RectStartPos.y - m_RectEndPos.y);

	endPos = startPos + textureSize;

	currnetPos = "Drag Object Start (X, Y) : " + std::to_string(dragObj->GetStartPos().x) + ", " + std::to_string(dragObj->GetStartPos().y);

	m_DragObjStartPos->SetText(currnetPos.c_str());

	currnetPos = "Drag Object End (X, Y) : " + std::to_string(dragObj->GetEndPos().x) + ", " + std::to_string(dragObj->GetEndPos().y);

	m_DragObjEndPos->SetText(currnetPos.c_str());

	m_SpriteFrame->SetImageStart(startPos.x, startPos.y);
	m_SpriteFrame->SetImageEnd(endPos.x, endPos.y);
	m_RectStartPos = startPos;
	m_RectEndPos = endPos;
}
