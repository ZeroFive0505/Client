#pragma once

#include "UIWindow.h"

class CCourseWindow :
	public CUIWindow
{
	friend class CScene;
private:
	CCourseWindow();
	virtual ~CCourseWindow();
private:
	std::vector<std::pair<class CUISpriteText*, std::string>> m_vecCourses;

public:
	virtual bool Init() override;
	virtual void Render(HDC hDC) override;
	virtual void Update(float deltaTime) override;

public:
	void AddCircuit(const std::string& name, const std::string& path, const Vector2& pos);
	std::string GetCurrentCourse(int index = 0);
	Vector2 GetCursorPos(int index = 0);

	inline size_t GetCourseSize() const
	{
		return m_vecCourses.size();
	}
};

