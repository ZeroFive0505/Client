#include "CourseWindow.h"
#include "UISpriteText.h"
#include "../GameManager.h"

CCourseWindow::CCourseWindow()
{
}

CCourseWindow::~CCourseWindow()
{
}

bool CCourseWindow::Init()
{
	m_Visibility = false;

	return true;
}

void CCourseWindow::Render(HDC hDC)
{
	if (m_Visibility)
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();
		RECT rc;

		rc.left = rs.width * 30.0f / 100.0f;
		rc.right = rs.width * 80.0f / 100.0f;
		rc.top = rs.height * 40.0f / 100.0f;
		rc.bottom = rs.height * 70.0f / 100.0f;

		FillRect(hDC, &rc, CGameManager::GetInst()->GetBlackBrush());
	}

	CUIWindow::Render(hDC);
}

void CCourseWindow::Update(float deltaTime)
{
	CUIWindow::Update(deltaTime);
}

void CCourseWindow::AddCircuit(const std::string& name, const std::string& path, const Vector2& pos)
{
	CUISpriteText* text = CreateWidget<CUISpriteText>(name);
	text->SetText(name);
	text->SetPos(pos);

	m_vecCourses.push_back(std::make_pair(text, path));
}

std::string CCourseWindow::GetCurrentCourse(int index)
{
	return m_vecCourses[index].second;
}

Vector2 CCourseWindow::GetCursorPos(int index)
{
	return m_vecCourses[index].first->GetPos();
}
