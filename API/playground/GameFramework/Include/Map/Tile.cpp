#include "Tile.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../GameManager.h"

CTile::CTile() :
    m_IndexX(0),
    m_IndexY(0),
    m_Index(0),
    m_Property(ETileProperty::Wall),
    m_SideCollision(false)
{
}

CTile::~CTile()
{
}

bool CTile::Init()
{
    return true;
}

void CTile::Update(float DeltaTime)
{
}

void CTile::PostUpdate(float DeltaTime)
{
}

void CTile::PrevRender()
{
}

void CTile::Render(HDC hDC)
{
    // ���� �ؽ��İ� �����Ǿ����� ��� �׸���.
    if (m_Texture)
    {
        CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCamera();

        Vector2 CameraPos = Camera->GetPos();

        // ī�޶��� ������� ��ġ�� �׸���.
        m_Texture->Render(hDC, m_Pos - CameraPos, m_StartFrame, m_Size);
    }

    if (CGameManager::GetInst()->IsEditorMode())
    {
        HBRUSH Brush = 0;

        switch (m_Property)
        {
        case ETileProperty::Normal:
            Brush = CGameManager::GetInst()->GetGreenBrush();
            break;
        case ETileProperty::Wall:
            if (m_SideCollision)
                Brush = CGameManager::GetInst()->GetYellowBrush();
            else
                Brush = CGameManager::GetInst()->GetRedBrush();
            break;
        case ETileProperty::Slow:
            Brush = CGameManager::GetInst()->GetYellowBrush();
            break;
        }

        CCamera* Camera = CSceneManager::GetInst()->GetScene()->GetCamera();

        Vector2 CameraPos = Camera->GetPos();

        RECT rc;

        rc.left = (LONG)m_Pos.x - (LONG)CameraPos.x;
        rc.top = (LONG)m_Pos.y - (LONG)CameraPos.y;
        rc.right = (LONG)m_Pos.x + (LONG)m_Size.x - (LONG)CameraPos.x;
        rc.bottom = (LONG)m_Pos.y + (LONG)m_Size.y - (LONG)CameraPos.y;

        FrameRect(hDC, &rc, Brush);
    }
}

void CTile::Save(FILE* pFile)
{
    // Ÿ���� �������� �����Ѵ�.
    fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
    fwrite(&m_Size, sizeof(Vector2), 1, pFile);

    fwrite(&m_IndexX, sizeof(int), 1, pFile);
    fwrite(&m_IndexY, sizeof(int), 1, pFile);
    fwrite(&m_Index, sizeof(int), 1, pFile);

    fwrite(&m_StartFrame, sizeof(Vector2), 1, pFile);
    fwrite(&m_EndFrame, sizeof(Vector2), 1, pFile);
    fwrite(&m_Property, sizeof(ETileProperty), 1, pFile);
    fwrite(&m_SideCollision, sizeof(bool), 1, pFile);

    // Ÿ�϶��� �ؽ��� ������ �����Ѵ�.
    if (m_Texture)
    {
        bool Tex = true;
        fwrite(&Tex, sizeof(bool), 1, pFile);

        m_Texture->Save(pFile);
    }
    else
    {
        bool Tex = false;
        fwrite(&Tex, sizeof(bool), 1, pFile);
    }
}

void CTile::Load(FILE* pFile)
{
    fread(&m_Pos, sizeof(Vector2), 1, pFile);
    fread(&m_Size, sizeof(Vector2), 1, pFile);

    fread(&m_IndexX, sizeof(int), 1, pFile);
    fread(&m_IndexY, sizeof(int), 1, pFile);
    fread(&m_Index, sizeof(int), 1, pFile);

    fread(&m_StartFrame, sizeof(Vector2), 1, pFile);
    fread(&m_EndFrame, sizeof(Vector2), 1, pFile);
    fread(&m_Property, sizeof(ETileProperty), 1, pFile);
    fread(&m_SideCollision, sizeof(bool), 1, pFile);

    bool Tex = true;
    fread(&Tex, sizeof(bool), 1, pFile);

    // �ؽ��ĸ� �ҷ��� �����Ѵ�.
    if (Tex)
        m_Texture = CTexture::LoadStatic(pFile, m_Scene);
}
