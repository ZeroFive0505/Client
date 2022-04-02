#include "Tile.h"
#include "TileMapComponent.h"
#include "../Resource/Animation/AnimationSequence2D.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"

CTile::CTile() :
	m_Shape(Tile_Shape::Rect),
	m_TileType(Tile_Type::Normal),
	m_AnimInstance(nullptr),
	m_Owner(nullptr),
	m_IndexX(-1),
	m_IndexY(-1),
	m_Index(-1),
	m_Opacity(1.0f)
{
}

CTile::CTile(const CTile& tile)
{
	*this = tile;

	m_AnimInstance = tile.m_AnimInstance->Clone();
}

CTile::~CTile()
{
	SAFE_DELETE(m_AnimInstance);
}

void CTile::AddAnimation(const std::string& sequenceName, const std::string& name, bool loop, float playTime, float playScale, bool reverse)
{
	if (!m_AnimInstance)
		m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->AddAnimation(sequenceName, name, loop, playTime, playScale, reverse);
}

void CTile::AddAnimation(const TCHAR* fileName, const std::string& pathName, const std::string& name, bool loop, float playTime, float playScale, bool reverse)
{
	if (!m_AnimInstance)
		m_AnimInstance = new CAnimationSequence2DInstance;

	m_AnimInstance->AddAnimation(fileName, pathName, name, loop, playTime, playScale, reverse);
}

void CTile::SetPlayTime(const std::string& name, float playTime)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetPlayTime(name, playTime);
}

void CTile::SetPlayScale(const std::string& name, float playScale)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetPlayScale(name, playScale);
}

void CTile::SetReverse(const std::string& name, bool reverse)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetReverse(name, reverse);
}

void CTile::SetLoop(const std::string& name, bool loop)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetLoop(name, loop);
}

void CTile::SetCurrentAnimation(const std::string& name)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->SetCurrentAnimation(name);
}

void CTile::ChangeAnimation(const std::string& name)
{
	if (!m_AnimInstance)
		return;

	m_AnimInstance->ChangeAnimation(name);
}

bool CTile::CheckCurrentAnimation(const std::string& name)
{
	if (!m_AnimInstance)
		return false;

	return m_AnimInstance->CheckCurrentAnimation(name);
}

void CTile::Start()
{
	m_Center = (m_Pos + m_Offset) + m_Size / 2.0f;
}

void CTile::Update(float deltaTime, bool sortDisable)
{
	if (m_AnimInstance)
	{
		m_AnimInstance->Update(deltaTime);

		int currentFrame = m_AnimInstance->GetCurrentAnimation()->GetCurrentFrame();

		m_FrameStart = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).start;
		m_FrameEnd = m_FrameStart + m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(currentFrame).size;
	}

	Vector3 ownerPos = m_Owner->GetWorldPos();

	Vector3 pos = ownerPos + m_Pos + m_Offset;;

	if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
	{
		if (sortDisable)
			pos.z = 999.0f;
		else
			pos.z = pos.y / 30000.0f * 1000.0f;

		pos.z = std::fmaxf(0.001f, pos.z);
	}

	// 행렬을 만들어낸다.
	Matrix matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.0f);
	matTranslate.Translation(pos);

	m_matWorld = matScale * matTranslate;

}


void CTile::Save(FILE* pFile)
{
	fwrite(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fwrite(&m_TileType, sizeof(Tile_Type), 1, pFile);

	fwrite(&m_Pos, sizeof(Vector3), 1, pFile);
	fwrite(&m_Size, sizeof(Vector3), 1, pFile);
	fwrite(&m_Center, sizeof(Vector3), 1, pFile);

	fwrite(&m_IndexX, sizeof(int), 1, pFile);
	fwrite(&m_IndexY, sizeof(int), 1, pFile);
	fwrite(&m_Index, sizeof(int), 1, pFile);

	fwrite(&m_FrameStart, sizeof(Vector2), 1, pFile);
	fwrite(&m_FrameEnd, sizeof(Vector2), 1, pFile);

	fwrite(&m_Opacity, sizeof(float), 1, pFile);
}

void CTile::Load(FILE* pFile)
{
	fread(&m_Shape, sizeof(Tile_Shape), 1, pFile);
	fread(&m_TileType, sizeof(Tile_Type), 1, pFile);

	fread(&m_Pos, sizeof(Vector3), 1, pFile);
	fread(&m_Size, sizeof(Vector3), 1, pFile);
	fread(&m_Center, sizeof(Vector3), 1, pFile);

	fread(&m_IndexX, sizeof(int), 1, pFile);
	fread(&m_IndexY, sizeof(int), 1, pFile);
	fread(&m_Index, sizeof(int), 1, pFile);

	fread(&m_FrameStart, sizeof(Vector2), 1, pFile);
	fread(&m_FrameEnd, sizeof(Vector2), 1, pFile);

	fread(&m_Opacity, sizeof(float), 1, pFile);
}
