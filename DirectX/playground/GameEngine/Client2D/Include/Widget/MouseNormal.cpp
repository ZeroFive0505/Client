#include "MouseNormal.h"

CMouseNormal::CMouseNormal()
{
}

CMouseNormal::CMouseNormal(const CMouseNormal& window) :
	CWidgetWindow(window)
{
	m_Image = FindWidget<CImage>("Image");
}

CMouseNormal::~CMouseNormal()
{
}

void CMouseNormal::Start()
{
}

bool CMouseNormal::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(32.0f, 31.0f);

	m_Image = CreateWidget<CImage>("Image");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 12; i++)
	{
		TCHAR* fileName = new TCHAR[MAX_PATH];
		memset(fileName, 0, sizeof(TCHAR) * MAX_PATH);

		wsprintf(fileName, TEXT("Mouse/Default/%d.png"), i);

		vecFileName.push_back(fileName);
	}

	m_Image->SetTexture("MouseNormal", vecFileName);
	m_Image->SetSize(32.0f, 31.0f);

	m_Image->AddFrameData(13);

	for (int i = 0; i <= 12; i++)
	{
		SAFE_DELETE(vecFileName[i]);
	}

	return true;
}

void CMouseNormal::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CMouseNormal::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMouseNormal::Render()
{
	CWidgetWindow::Render();
}

CMouseNormal* CMouseNormal::Clone()
{
	return new CMouseNormal(*this);
}
