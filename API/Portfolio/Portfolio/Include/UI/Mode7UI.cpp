#include "Mode7UI.h"
#include "../GameManager.h"
#include "UIText.h"
#include "../Scene/Mode7Demo.h"

CMode7UI::CMode7UI() :
    m_Far(nullptr),
    m_Near(nullptr),
    m_FOV(nullptr)
{
}

CMode7UI::~CMode7UI()
{
}

bool CMode7UI::Init()
{
    Resolution rs = CGameManager::GetInst()->GetResolution();

    m_Far = CreateWidget<CUIText>("Far plane");
    m_Far->SetPos(rs.width * 5 / 100, rs.height * 10 / 100);
    m_Far->SetTextColor(255, 255, 255);

    m_Near = CreateWidget<CUIText>("Near plane");
    m_Near->SetPos(rs.width * 5 / 100, rs.height * 15 / 100);
    m_Near->SetTextColor(255, 255, 255);

    m_FOV = CreateWidget<CUIText>("FOV");
    m_FOV->SetPos(rs.width * 5 / 100, rs.height * 20 / 100);
    m_FOV->SetTextColor(255, 255, 255);

    m_Perspective = CreateWidget<CUIText>("Perspective");
    m_Perspective->SetPos(rs.width * 5 / 100, rs.height * 25 / 100);
    m_Perspective->SetTextColor(255, 255, 255);

    return true;
}

void CMode7UI::Update(float deltaTime)
{
    CMode7Demo* scene = (CMode7Demo*)m_Scene;
    std::wstring farStr = L"Far : ";

    farStr += std::to_wstring(scene->GetClipFar());

    m_Far->SetText(farStr.c_str());

    std::wstring nearStr = L"Near : ";

    nearStr += std::to_wstring(scene->GetClipNear());

    m_Near->SetText(nearStr.c_str());

    std::wstring fovStr = L"FOV : ";

    fovStr += std::to_wstring(scene->GetFOV());

    m_FOV->SetText(fovStr.c_str());

    std::wstring perspectiveStr = L"Perspective : ";

    perspectiveStr += scene->GetPerspective() ? L"ON" : L"OFF";

    m_Perspective->SetText(perspectiveStr.c_str());
}
