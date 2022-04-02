#include "MouseAttack.h"

CMouseAttack::CMouseAttack()
{
}

CMouseAttack::CMouseAttack(const CMouseAttack& window) :
	CWidgetWindow(window)
{
	m_Image = FindWidget <CImage>("Image");
}

CMouseAttack::~CMouseAttack()
{
}

void CMouseAttack::Start()
{
}

bool CMouseAttack::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(32.0f, 31.0f);

	m_Image = CreateWidget<CImage>("Image");

	std::vector<TCHAR*> vecFileName;

	for (int i = 0; i <= 6; i++)
	{
		TCHAR fileName[MAX_PATH] = {};
		
		wsprintf(fileName, TEXT("Mouse/Temp/Attack/000%d.png"), i);

		vecFileName.push_back(fileName);
	}

	m_Image->SetTexture("MouseAttack", vecFileName);
	m_Image->SetSize(30.0f, 30.0f);

	m_Image->AddFrameData(7);

	return true;
}

void CMouseAttack::Update(float deltaTime)
{
	CWidgetWindow::Update(deltaTime);
}

void CMouseAttack::PostUpdate(float deltaTime)
{
	CWidgetWindow::PostUpdate(deltaTime);
}

void CMouseAttack::Render()
{
	CWidgetWindow::Render();
}

CMouseAttack* CMouseAttack::Clone()
{
	return new CMouseAttack(*this);
}
