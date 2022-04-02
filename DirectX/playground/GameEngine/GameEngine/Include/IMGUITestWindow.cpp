#include "IMGUITestWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUIRadioButton.h"
#include "IMGUICheckBox.h"

CIMGUITestWindow::CIMGUITestWindow()
{
}

CIMGUITestWindow::~CIMGUITestWindow()
{
}

bool CIMGUITestWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUIButton* button = AddWidget<CIMGUIButton>("Button1");

	button->SetClickCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton1);

	CIMGUISameLine* line = AddWidget<CIMGUISameLine>("Line");
	line->SetOffsetX(200.0f);
	line->SetSpacing(100.0f);

	button = AddWidget<CIMGUIButton>("Button2", 100.0f, 30.0f);

	button->SetClickCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton2);

	CIMGUILabel* label = AddWidget<CIMGUILabel>("Test", 100.0f, 30.0f);

	label->SetColor(0, 255, 0);
	label->SetAlign(0.5f, 0.0f);

	line = AddWidget<CIMGUISameLine>("Line");

	CIMGUIText* text = AddWidget<CIMGUIText>("Text", 100.0f, 30.0f);
	text->SetText("한글 테스트");
	text->SetColor(255, 255, 0);

	CIMGUITextInput* textInput = AddWidget<CIMGUITextInput>("TextInput", 200.0f, 30.0f);
	textInput->SetHideName(true);

	CIMGUIListBox* listBox = AddWidget<CIMGUIListBox>("ListBox", 200.0f, 30.0f);
	listBox->SetHideName(true);

	listBox->AddItem("한글");
	listBox->AddItem("영어");
	listBox->AddItem("리스트 박스 테스트");

	CIMGUIComboBox* comboBox = AddWidget<CIMGUIComboBox>("ComboBox", 200.0f, 30.0f);
	comboBox->SetHideName(true);

	comboBox->AddItem("한글");
	comboBox->AddItem("영어");
	comboBox->AddItem("콤보 박스 테스트");

	CIMGUIImage* image = AddWidget<CIMGUIImage>("Image", 200.0f, 200.0f);

	image->SetTexture("TeemoTest", TEXT("Teemo.jpg"));
	image->SetImageStart(0.0f, 0.0f);
	image->SetImageEnd(1215.0f, 717.0f);
	image->SetTint(0, 0, 0);

	CIMGUIRadioButton* radioButton = AddWidget<CIMGUIRadioButton>("RadioButton");
	radioButton->SetSameLine(true);
	radioButton->AddItem("한글");
	radioButton->AddItem("영어");
	radioButton->AddItem("라디오 버튼 테스트");

	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToKorean, 0);
	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToEnglish, 1);

	CIMGUICheckBox* checkBox = AddWidget<CIMGUICheckBox>("CheckBox");
	checkBox->SetSameLine(true);
	checkBox->AddItem("빨강");
	checkBox->AddItem("초록");
	checkBox->AddItem("파랑");

	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetRed, 0);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetGreen, 1);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetBlue, 2);

	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetRed, 0);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetGreen, 1);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetBlue, 2);

	return true;
}

void CIMGUITestWindow::Update(float deltaTime)
{
	CIMGUIWindow::Update(deltaTime);
}

void CIMGUITestWindow::ClickButton1()
{
	MessageBox(0, TEXT("11"), TEXT("11"), MB_OK);
}

void CIMGUITestWindow::ClickButton2()
{
	MessageBox(0, TEXT("22"), TEXT("22"), MB_OK);
}

void CIMGUITestWindow::ChangeToKorean()
{
	CIMGUIText* text = (CIMGUIText*)FindWidget("Text");
	text->SetText("한글 테스트");
	text->SetColor(255, 255, 0);

	CIMGUIListBox* listBox = (CIMGUIListBox*)FindWidget("ListBox");
	listBox->Clear();
	listBox->AddItem("한글");
	listBox->AddItem("영어");
	listBox->AddItem("리스트 박스 테스트");

	CIMGUIComboBox* comboBox = (CIMGUIComboBox*)FindWidget("ComboBox");
	comboBox->Clear();
	comboBox->AddItem("한글");
	comboBox->AddItem("영어");
	comboBox->AddItem("콤보 박스 테스트");

	CIMGUIRadioButton* radioButton = (CIMGUIRadioButton*)FindWidget("RadioButton");
	radioButton->Clear();
	radioButton->AddItem("한글");
	radioButton->AddItem("영어");
	radioButton->AddItem("라디오 버튼 테스트");

	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToKorean, 0);
	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToEnglish, 1);

	CIMGUICheckBox* checkBox = (CIMGUICheckBox*)FindWidget("CheckBox");
	checkBox->Clear();
	checkBox->AddItem("빨강");
	checkBox->AddItem("초록");
	checkBox->AddItem("파랑");


	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetRed, 0);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetGreen, 1);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetBlue, 2);

	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetRed, 0);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetGreen, 1);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetBlue, 2);
}

void CIMGUITestWindow::ChangeToEnglish()
{
	CIMGUIText* text = (CIMGUIText*)FindWidget("Text");
	text->SetText("English Test");
	text->SetColor(255, 255, 0);

	CIMGUIListBox* listBox = (CIMGUIListBox*)FindWidget("ListBox");
	listBox->Clear();
	listBox->AddItem("Korean");
	listBox->AddItem("English");
	listBox->AddItem("ListBox Test");


	CIMGUIComboBox* comboBox = (CIMGUIComboBox*)FindWidget("ComboBox");
	comboBox->Clear();
	comboBox->AddItem("Korean");
	comboBox->AddItem("English");
	comboBox->AddItem("ComboBox Test");

	CIMGUIRadioButton* radioButton = (CIMGUIRadioButton*)FindWidget("RadioButton");
	radioButton->Clear();
	radioButton->AddItem("Korean");
	radioButton->AddItem("English");
	radioButton->AddItem("RadioButton Test");

	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToKorean, 0);
	radioButton->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ChangeToEnglish, 1);

	CIMGUICheckBox* checkBox = (CIMGUICheckBox*)FindWidget("CheckBox");
	checkBox->Clear();
	checkBox->AddItem("Red");
	checkBox->AddItem("Green");
	checkBox->AddItem("Blue");


	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetRed, 0);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetGreen, 1);
	checkBox->SetSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::SetBlue, 2);

	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetRed, 0);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetGreen, 1);
	checkBox->SetDeSelectCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ResetBlue, 2);
}

void CIMGUITestWindow::SetRed()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetRedTint(255);
}

void CIMGUITestWindow::SetGreen()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetGreenTint(255);
}

void CIMGUITestWindow::SetBlue()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetBlueTint(255);
}

void CIMGUITestWindow::ResetRed()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetRedTint(0);
}

void CIMGUITestWindow::ResetGreen()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetGreenTint(0);
}

void CIMGUITestWindow::ResetBlue()
{
	CIMGUIImage* image = (CIMGUIImage*)FindWidget("Image");

	image->SetBlueTint(0);
}
