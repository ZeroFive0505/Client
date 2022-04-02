#include "WidgetWindow.h"
#include "../Component/WidgetComponent.h"
#include "../Scene/Scene.h"

CWidgetWindow::CWidgetWindow() :
    m_Viewport(nullptr),
    m_OwnerComponent(nullptr),
    m_ZOrder(0),
    m_Start(false),
    m_Size(100.0f, 100.0f)
{
}

CWidgetWindow::CWidgetWindow(const CWidgetWindow& window)
{
    *this = window;
    m_OwnerComponent = nullptr;

    // 복사 대상이 가지고 있는 모든 위젯을 복사해온다.
    auto iter = window.m_WidgetList.begin();
    auto iterEnd = window.m_WidgetList.end();

    m_WidgetList.clear();

    for (; iter != iterEnd; iter++)
    {
        CWidget* widget = (*iter)->Clone();

        widget->m_Owner = this;

        m_WidgetList.push_back(widget);
    }
}

CWidgetWindow::~CWidgetWindow()
{
}

void CWidgetWindow::Start()
{
    m_Start = true;

    auto iter = m_WidgetList.begin();
    auto iterEnd = m_WidgetList.end();

    for (; iter != iterEnd; iter++)
    {
        (*iter)->Start();
    }

    m_WidgetList.sort(CWidgetWindow::SortWidget);

    // 만약 이 윈도우를 소유하고있는 컴포넌트가 있다면
    if (m_OwnerComponent)
        m_Viewport = m_OwnerComponent->GetScene()->GetViewport();
}

bool CWidgetWindow::Init()
{
    return true;
}

void CWidgetWindow::Update(float deltaTime)
{
    // 중간에 새로 추가된 윈도우의 경우
    if (!m_Start)
        Start();

    auto iter = m_WidgetList.begin();
    auto iterEnd = m_WidgetList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_WidgetList.erase(iter);
            iterEnd = m_WidgetList.end();
            continue;
        }
        else if (!(*iter)->IsEnable())
        {
            iter++;
            continue;
        }

        (*iter)->Update(deltaTime);
        iter++;
    }
}

void CWidgetWindow::PostUpdate(float deltaTime)
{
    if (!m_Start)
        Start();

    auto iter = m_WidgetList.begin();
    auto iterEnd = m_WidgetList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_WidgetList.erase(iter);
            iterEnd = m_WidgetList.end();
            continue;
        }
        else if (!(*iter)->IsEnable())
        {
            iter++;
            continue;
        }

        (*iter)->PostUpdate(deltaTime);
        iter++;
    }
}

void CWidgetWindow::Render()
{
    if (!m_Start)
        Start();

    m_WidgetList.sort(CWidgetWindow::SortWidget);

    auto iter = m_WidgetList.begin();
    auto iterEnd = m_WidgetList.end();

    for (; iter != iterEnd;)
    {
        if (!(*iter)->IsActive())
        {
            iter = m_WidgetList.erase(iter);
            iterEnd = m_WidgetList.end();
            continue;
        }
        else if (!(*iter)->IsEnable())
        {
            iter++;
            continue;
        }

        (*iter)->Render();
        iter++;
    }
}

bool CWidgetWindow::CollisionMouse(const Vector2& mousePos)
{
    // 위젯과 충돌전에 윈도우와 충돌을 하는지 확인한다.

    if (m_Pos.x > mousePos.x)
        return false;
    else if (m_Pos.x + m_Size.x < mousePos.x)
        return false;
    else if (m_Pos.y > mousePos.y)
        return false;
    else if (m_Pos.y + m_Size.y < mousePos.y)
        return false;

    // Z오더값을 기준으로 정렬해준다. 오름차순
    m_WidgetList.sort(CWidgetWindow::SortWidget);

    auto iter = m_WidgetList.rbegin();
    auto iterEnd = m_WidgetList.rend();

    for (; iter != iterEnd; iter++)
    {
        if (!(*iter)->IsEnable())
            continue;

        if ((*iter)->CollisionMouse(mousePos))
        {
            (*iter)->m_MouseHovered = true;
            return true;
        }
        else
            (*iter)->m_MouseHovered = false;
    }

    return false;
}

CWidgetWindow* CWidgetWindow::Clone()
{
    return new CWidgetWindow(*this);
}

bool CWidgetWindow::SortWidget(const CSharedPtr<CWidget> src, const CSharedPtr<CWidget> dest)
{
    return src->GetZOrder() < dest->GetZOrder();
}
