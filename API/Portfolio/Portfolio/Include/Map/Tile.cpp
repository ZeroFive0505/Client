#include "Tile.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../GameManager.h"

CTile::CTile() :
	m_Index(0),
	m_IndexX(0),
	m_IndexY(0),
	m_Property(ETileProperty::TRACK)
{
}

CTile::~CTile()
{
}

bool CTile::Init()
{
	return true;
}

void CTile::Update(float deltaTime)
{
}

void CTile::PostUpdate(float deltaTime)
{
}

void CTile::Render(HDC hDC)
{

	if (CGameManager::GetInst()->IsEditorMode() && CGameManager::GetInst()->GetShowGrid())
	{
		HBRUSH brush = 0;

		switch (m_Property)
		{
		case ETileProperty::TRACK:
			brush = CGameManager::GetInst()->GetGreenBrush();
			break;
		case ETileProperty::BLOCK:
			brush = CGameManager::GetInst()->GetRedBrush();
			break;
		case ETileProperty::SLOW:
			brush = CGameManager::GetInst()->GetYellowBrush();
			break;
		case ETileProperty::SLOW_2:
			brush = CGameManager::GetInst()->GetDarkerYellowBrush();
			break;
		case ETileProperty::SPECIAL_13H:
			brush = CGameManager::GetInst()->GetOrangeBrush();
			break;
		case ETileProperty::HAZARD:
			brush = CGameManager::GetInst()->GetBlackBrush();
			break;
		default:
			// 만약 바닥의 특별한 오브젝트가 깔려있다면 타일의 속성도 같이 바꿔줘야한다.
			// brush = CGameManager::GetInst()->GetOrangeBrush();
			break;
		}

		CCamera* camera = CSceneManager::GetInst()->GetScene()->GetCamera();

		Vector2 cameraPos = camera->GetPos();

		RECT rc;

		rc.left = (LONG)m_Pos.x - (LONG)cameraPos.x;
		rc.top = (LONG)m_Pos.y - (LONG)cameraPos.y;
		rc.right = (LONG)m_Pos.x + (LONG)m_Size.x - (LONG)cameraPos.x;
		rc.bottom = (LONG)m_Pos.y + (LONG)m_Size.y - (LONG)cameraPos.y;

		FrameRect(hDC, &rc, brush);
	}
}

void CTile::Save(FILE* pFile)
{
	fwrite(&m_Pos, sizeof(Vector2), 1, pFile);
	fwrite(&m_Size, sizeof(Vector2), 1, pFile);

	fwrite(&m_IndexX, sizeof(int), 1, pFile);
	fwrite(&m_IndexY, sizeof(int), 1, pFile);
	fwrite(&m_IndexX, sizeof(int), 1, pFile);

	fwrite(&m_Property, sizeof(ETileProperty), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	fread(&m_Pos, sizeof(Vector2), 1, pFile);
	fread(&m_Size, sizeof(Vector2), 1, pFile);

	fread(&m_IndexX, sizeof(int), 1, pFile);
	fread(&m_IndexY, sizeof(int), 1, pFile);
	fread(&m_Index, sizeof(int), 1, pFile);

	fread(&m_Property, sizeof(ETileProperty), 1, pFile);
}
