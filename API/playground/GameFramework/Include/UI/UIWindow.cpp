#include "UIWindow.h"

CUIWindow::CUIWindow() :
    m_Visibility(true),
    m_WidgetArray(nullptr),
    m_Scene(nullptr),
    m_ZOrder(0),
    m_WidgetCount(0),
    m_WidgetCapacity(4)
{
    m_WidgetArray = new CUIWidget * [m_WidgetCapacity];
}

CUIWindow::CUIWindow(const CUIWindow& widget)
{
}

CUIWindow::~CUIWindow()
{
    // 윈도우 소멸시에는 안에 들어있는 위젯들을 전부 삭제
    for (int i = 0; i < m_WidgetCount; i++)
    {
        SAFE_RELEASE(m_WidgetArray[i]);
    }

    SAFE_DELETE_ARRAY(m_WidgetArray);
}

bool CUIWindow::Init()
{
    return true;
}

void CUIWindow::Update(float DeltaTime)
{
    // 위젯의 활성화 여부로 위젯을 삭제한다.
    for (int i = 0; i < m_WidgetCount;)
    {
        if (!m_WidgetArray[i]->IsActive())
        {
            SAFE_RELEASE(m_WidgetArray[i]);

            for (int j = 1; j < m_WidgetCount - 1; j++)
            {
                m_WidgetArray[j] = m_WidgetArray[j + 1];
            }

            m_WidgetCount--;

            continue;
        }
        // 만약 보이지 않는 위젯이라면 업데이트는 필요없다.
        else if (!m_WidgetArray[i]->GetVisibility())
        {
            i++;
            continue;
        }

        // 위젯을 업데이트 한다.
        m_WidgetArray[i]->Update(DeltaTime);
        i++;
    }
}

void CUIWindow::PostUpdate(float DeltaTime)
{   
    // Update와 마찬가지다.
    for (int i = 0; i < m_WidgetCount;)
    {
        if (!m_WidgetArray[i]->IsActive())
        {
            SAFE_RELEASE(m_WidgetArray[i]);

            for (int j = 1; j < m_WidgetCount - 1; j++)
            {
                m_WidgetArray[j] = m_WidgetArray[j + 1];
            }

            m_WidgetCount--;

            continue;
        }
        else if (!m_WidgetArray[i]->GetVisibility())
        {
            i++;
            continue;
        }

        m_WidgetArray[i]->PostUpdate(DeltaTime);
        i++;
    }
}

void CUIWindow::Collision(float DeltaTime)
{
    // ZOrder에따라 위젯들을 정렬한다.
    if (m_WidgetCount >= 2)
    {
        qsort(m_WidgetArray, (size_t)m_WidgetCount, sizeof(CUIWidget*), CUIWindow::SortZOrder);
    }
}

void CUIWindow::Render(HDC hDC)
{
    for (int i = 0; i < m_WidgetCount;)
    {
        if (!m_WidgetArray[i]->IsActive())
        {
            SAFE_RELEASE(m_WidgetArray[i]);

            for (int j = 1; j < m_WidgetCount - 1; j++)
            {
                m_WidgetArray[j] = m_WidgetArray[j + 1];
            }

            m_WidgetCount--;

            continue;
        }

        i++;
    }

 
    // 윈도우에서 렌더할때도 마찬가지로 위젯을 거꾸로 출력해준다.
    for (int i = m_WidgetCount - 1; i >= 0; i--)
    {
        if (!m_WidgetArray[i]->GetVisibility())
            continue;

        m_WidgetArray[i]->Render(hDC);
    }
}

int CUIWindow::SortZOrder(const void* Src, const void* Dest)
{
    CUIWidget* SrcObj = *(CUIWidget**)Src;
    CUIWidget* DestObj = *(CUIWidget**)Dest;

    int SrcZ = SrcObj->GetZOrder();
    int DestZ = DestObj->GetZOrder();

    if (SrcZ > DestZ)
        return -1;

    else if (SrcZ < DestZ)
        return 1;

    return 0;
}
