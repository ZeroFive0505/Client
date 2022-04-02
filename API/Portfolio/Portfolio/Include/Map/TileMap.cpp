#include "TileMap.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../PathManager.h"
#include "../AI/AIGradientDescent.h"

CTileMap::CTileMap() :
	m_TileCountX(0),
	m_TileCountY(0),
	m_StartX(-1),
	m_StartY(-1),
	m_EndX(-1),
	m_EndY(-1),
	m_WideGoalHeight(0),
	m_WideGoalWidth(0),
	m_WideGoalStartX(0),
	m_WideGoalStartY(0),
	m_GoalStartX(0),
	m_GoalStartY(0),
	m_GoalWidth(0),
	m_GoalHeight(0)
{
	m_TrackMat = ETileMaterial::STONE;
	m_SlowMat1 = ETileMaterial::DIRT;
	m_SlowMat2 = ETileMaterial::DIRT;
	m_HazardMat = ETileMaterial::WATER;
}

CTileMap::~CTileMap()
{
	size_t size = m_vecTile.size();

	for (size_t i = 0; i < size; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();
}

bool CTileMap::CreateTile(int countX, int countY, const Vector2& tileSize)
{
	m_TileCountX = countX;
	m_TileCountY = countY;
	m_TileSize = tileSize;

	{
		auto iter = m_vecTile.begin();
		auto iterEnd = m_vecTile.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE((*iter));
		}

		m_vecTile.clear();
	}

	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	CCamera* camera = m_Scene->GetCamera();

	camera->SetWorldResolution(m_Size);

	for (int i = 0; i < m_TileCountY; i++)
	{
		for (int j = 0; j < m_TileCountX; j++)
		{
			CTile* tile = new CTile;

			tile->Init();

			Vector2 pos = Vector2((float)j, (float)i) * m_TileSize;

			tile->SetTileInfo(pos, m_TileSize, j, i, i * m_TileCountX + j);

			m_vecTile.push_back(tile);
		}
	}

	return true;
}


void CTileMap::ChangeTileProperty(const Vector2& pos, ETileProperty tileProperty)
{
	CTile* tile = GetTile(pos);

	if (!tile)
		return;

	tile->SetTileProperty(tileProperty);
}

void CTileMap::CreateWallObject(const Vector2& pos, const EWallObjectType type)
{
	Vector2 pixelPos = Vector2((int)pos.x, (int)pos.y);
	m_WallObjects.push_back(std::make_pair(type, pixelPos));
}

void CTileMap::DeleteWallObject(const Vector2& pos, const Vector2& size, const Vector2& pivot)
{
	auto iter = m_WallObjects.begin();
	auto iterEnd = m_WallObjects.end();

	for (; iter != iterEnd; iter++)
	{
		Vector2 LT = iter->second - pivot * size;
		Vector2 RB = LT + size;

		if (LT.x <= pos.x && pos.x <= RB.x &&
			LT.y <= pos.y && pos.y <= RB.y)
		{
			m_WallObjects.erase(iter);
			break;
		}
	}
}


void CTileMap::CreateFloorObject(const Vector2& pos, const EFloorObjectType type, const EFloorObjectOrientation orientation)
{
	int size = 0;

	CTile* tile = GetTile(pos);

	if (tile)
	{
		Vector2 pos = tile->GetPos();

		if (tile->GetTileProperty() != ETileProperty::TRACK)
			return;

		if (type == EFloorObjectType::ITEM_BOX || type == EFloorObjectType::OIL_SLICK
			|| type == EFloorObjectType::ACCEL_PANEL)
		{
			int indexX = tile->GetIndexX();
			int indexY = tile->GetIndexY();

			if (indexX > m_TileCountX - 2 || indexY > m_TileCountY - 2)
				return;

			CTile* tile2 = GetTile(indexX + 1, indexY);

			if (tile2->GetTileProperty() != ETileProperty::TRACK)
				return;

			CTile* tile3 = GetTile(indexX, indexY + 1);

			if (tile3->GetTileProperty() != ETileProperty::TRACK)
				return;

			CTile* tile4 = GetTile(indexX + 1, indexY + 1);

			if (tile4->GetTileProperty() != ETileProperty::TRACK)
				return;


			switch (type)
			{
			case EFloorObjectType::ACCEL_PANEL:
				tile->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile2->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile3->SetTileProperty(ETileProperty::ACCEL_PANEL);
				tile4->SetTileProperty(ETileProperty::ACCEL_PANEL);
				break;
			case EFloorObjectType::ITEM_BOX:
				tile->SetTileProperty(ETileProperty::OTHER);
				tile2->SetTileProperty(ETileProperty::OTHER);
				tile3->SetTileProperty(ETileProperty::OTHER);
				tile4->SetTileProperty(ETileProperty::OTHER);
				break;
			case EFloorObjectType::OIL_SLICK:
				tile->SetTileProperty(ETileProperty::OIL_SLICK);
				tile2->SetTileProperty(ETileProperty::OIL_SLICK);
				tile3->SetTileProperty(ETileProperty::OIL_SLICK);
				tile4->SetTileProperty(ETileProperty::OIL_SLICK);
				break;
			}

		}
		else if (type == EFloorObjectType::COIN)
		{
			if (tile->GetTileProperty() != ETileProperty::TRACK)
				return;

			tile->SetTileProperty(ETileProperty::OTHER);
		}
		else if (type == EFloorObjectType::RAMP_HORIZONTAL)
		{
			if (tile->GetTileProperty() != ETileProperty::TRACK)
				return;

			tile->SetTileProperty(ETileProperty::RAMP_HORIZONTAL);
		}
		else if (type == EFloorObjectType::RAMP_VERTICAL)
		{
			if (tile->GetTileProperty() != ETileProperty::TRACK)
				return;

			tile->SetTileProperty(ETileProperty::RAMP_VERTICAL);
		}



		m_FloorObjects.push_back(std::make_pair(std::make_pair(type, pos), orientation));
	}
}

void CTileMap::DeleteFloorObject(const Vector2& pos, const Vector2& size, const Vector2& pivot, EFloorObjectType type)
{
	auto iter = m_FloorObjects.begin();
	auto iterEnd = m_FloorObjects.end();

	for (; iter != iterEnd; iter++)
	{
		Vector2 LT = iter->first.second - pivot * size;
		Vector2 RB = LT + size;

		if (LT.x <= pos.x && pos.x <= RB.x &&
			LT.y <= pos.y && pos.y <= RB.y)
		{
			switch (type)
			{
			case EFloorObjectType::ACCEL_PANEL:
			case EFloorObjectType::ITEM_BOX:
			case EFloorObjectType::OIL_SLICK:
			{
				CTile* tile1 = GetTile(LT);
				int indexX = tile1->GetIndexX();
				int indexY = tile1->GetIndexY();

				CTile* tile2 = GetTile(indexX + 1, indexY);
				CTile* tile3 = GetTile(indexX, indexY + 1);
				CTile* tile4 = GetTile(indexX + 1, indexY + 1);

				tile1->SetTileProperty(ETileProperty::TRACK);
				tile2->SetTileProperty(ETileProperty::TRACK);
				tile3->SetTileProperty(ETileProperty::TRACK);
				tile4->SetTileProperty(ETileProperty::TRACK);
				m_FloorObjects.erase(iter);
			}
			return;
			default:
			{
				CTile* tile = GetTile(LT);
				tile->SetTileProperty(ETileProperty::TRACK);
				m_FloorObjects.erase(iter);
			}
			return;
			}
		}
	}
}


CTile* CTileMap::GetTile(const Vector2& pos)
{
	int indexX = (int)(pos.x / m_TileSize.x);
	int indexY = (int)(pos.y / m_TileSize.y);

	if (indexX < 0 || indexX >= m_TileCountX)
		return nullptr;

	if (indexY < 0 || indexY >= m_TileCountY)
		return nullptr;

	return m_vecTile[indexY * m_TileCountX + indexX];
}

CTile* CTileMap::GetTile(int index)
{
	if (index >= MAP_TILES_HEIGHT * MAP_TILES_WIDTH)
		return nullptr;
	return m_vecTile[index];
}

CTile* CTileMap::GetTile(int indexX, int indexY)
{
	indexX = indexX < 0 ? 0 : indexX;
	indexY = indexY < 0 ? 0 : indexY;

	indexX = indexX >= m_TileCountX ? m_TileCountX - 1 : indexX;
	indexY = indexY >= m_TileCountY ? m_TileCountY - 1 : indexY;

	return m_vecTile[indexY * m_TileCountX + indexX];
}

Vector2 CTileMap::GetPlayerInitPos(int index)
{
	Vector2 goalPos = Vector2((m_GoalStartX + m_GoalWidth / 2.0f ), 
		(m_GoalStartY + m_GoalHeight));

	float deltaX = goalPos.x < MAP_ASSETS_WIDTH / 2.0f ? 16.0f * (2.0f * (index % 2) - 1.0f) :
		16.0f * (1.0f - 2.0f * (index % 2));
	float deltaY = 28.0f + 24.0f * (index - 1);

	Vector2 playerPos = goalPos + Vector2(deltaX, deltaY);

	playerPos.x = floor(playerPos.x / 8.0f) * 8.0f;

	return playerPos;
}

void CTileMap::Start()
{
	CMapBase::Start();
}

bool CTileMap::Init()
{
	CMapBase::Init();

	return true;
}

void CTileMap::Update(float deltaTime)
{
	CMapBase::Update(deltaTime);

	if (!m_vecTile.empty())
	{
		CCamera* camera = m_Scene->GetCamera();

		Vector2 cameraPos = camera->GetPos();
		Vector2 resolution = camera->GetResolution();

		m_StartX = (int)(cameraPos.x / m_TileSize.x);
		m_StartY = (int)(cameraPos.y / m_TileSize.y);
		m_EndX = (int)((cameraPos.x + resolution.x) / m_TileSize.x);
		m_EndY = (int)((cameraPos.y + resolution.y) / m_TileSize.y);

		m_StartX = m_StartX < 0 ? 0 : m_StartX;
		m_StartY = m_StartY < 0 ? 0 : m_StartY;
		m_EndX = m_EndX >= m_TileCountX ? m_TileCountX - 1 : m_EndX;
		m_EndY = m_EndY >= m_TileCountY ? m_TileCountY - 1 : m_EndY;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->Update(deltaTime);
			}
		}
	}
}

void CTileMap::PostUpdate(float deltaTime)
{
	CMapBase::PostUpdate(deltaTime);

	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->PostUpdate(deltaTime);
			}
		}
	}
}

void CTileMap::Render(HDC hDC)
{
	CMapBase::Render(hDC);

	if (!m_vecTile.empty())
	{
		if (m_StartY == -1 || m_StartX == -1 || m_EndX == -1 || m_EndY == -1)
			return;

		for (int i = m_StartY; i <= m_EndY; i++)
		{
			for (int j = m_StartX; j <= m_EndX; j++)
			{
				m_vecTile[i * m_TileCountX + j]->Render(hDC);
			}
		}
	}
}

void CTileMap::Save(FILE* pFile)
{
	CMapBase::Save(pFile);
	
	fwrite(&m_TrackMat, sizeof(ETileMaterial), 1, pFile);
	fwrite(&m_SlowMat1, sizeof(ETileMaterial), 1, pFile);
	fwrite(&m_SlowMat2, sizeof(ETileMaterial), 1, pFile);
	fwrite(&m_HazardMat, sizeof(ETileMaterial), 1, pFile);


	fwrite(&m_AIFarVision, sizeof(int), 1, pFile);

	fwrite(&m_WideGoalStartX, sizeof(int), 1, pFile);
	fwrite(&m_WideGoalStartY, sizeof(int), 1, pFile);
	fwrite(&m_WideGoalWidth, sizeof(int), 1, pFile);
	fwrite(&m_WideGoalHeight, sizeof(int), 1, pFile);

	fwrite(&m_GoalStartX, sizeof(int), 1, pFile);
	fwrite(&m_GoalStartY, sizeof(int), 1, pFile);
	fwrite(&m_GoalWidth, sizeof(int), 1, pFile);
	fwrite(&m_GoalHeight, sizeof(int), 1, pFile);

	fwrite(&m_TileCountX, sizeof(int), 1, pFile);
	fwrite(&m_TileCountY, sizeof(int), 1, pFile);
	fwrite(&m_TileSize, sizeof(Vector2), 1, pFile);
	fwrite(&m_StartX, sizeof(int), 1, pFile);
	fwrite(&m_StartY, sizeof(int), 1, pFile);
	fwrite(&m_EndX, sizeof(int), 1, pFile);
	fwrite(&m_EndY, sizeof(int), 1, pFile);

	int tileCount = (int)m_vecTile.size();
	fwrite(&tileCount, sizeof(int), 1, pFile);

	for (int i = 0; i < tileCount; i++)
	{
		m_vecTile[i]->Save(pFile);
	}
}

void CTileMap::Load(FILE* pFile)
{
	CMapBase::Load(pFile);

	fread(&m_TrackMat, sizeof(ETileMaterial), 1, pFile);
	fread(&m_SlowMat1, sizeof(ETileMaterial), 1, pFile);
	fread(&m_SlowMat2, sizeof(ETileMaterial), 1, pFile);
	fread(&m_HazardMat, sizeof(ETileMaterial), 1, pFile);

	fread(&m_AIFarVision, sizeof(int), 1, pFile);

	fread(&m_WideGoalStartX, sizeof(int), 1, pFile);
	fread(&m_WideGoalStartY, sizeof(int), 1, pFile);
	fread(&m_WideGoalWidth, sizeof(int), 1, pFile);
	fread(&m_WideGoalHeight, sizeof(int), 1, pFile);

	fread(&m_GoalStartX, sizeof(int), 1, pFile);
	fread(&m_GoalStartY, sizeof(int), 1, pFile);
	fread(&m_GoalWidth, sizeof(int), 1, pFile);
	fread(&m_GoalHeight, sizeof(int), 1, pFile);

	fread(&m_TileCountX, sizeof(int), 1, pFile);
	fread(&m_TileCountY, sizeof(int), 1, pFile);
	fread(&m_TileSize, sizeof(Vector2), 1, pFile);
	fread(&m_StartX, sizeof(int), 1, pFile);
	fread(&m_StartY, sizeof(int), 1, pFile);
	fread(&m_EndX, sizeof(int), 1, pFile);
	fread(&m_EndY, sizeof(int), 1, pFile);

	int tileCount = 0;
	fread(&tileCount, sizeof(int), 1, pFile);

	size_t tileSize = m_vecTile.size();

	for (size_t i = 0; i < tileSize; i++)
	{
		SAFE_DELETE(m_vecTile[i]);
	}

	m_vecTile.clear();

	for (int i = 0; i < tileCount; i++)
	{
		CTile* tile = new CTile;
		tile->m_Scene = m_Scene;

		m_vecTile.push_back(tile);

		m_vecTile[i]->Load(pFile);
	}

	m_Size = m_TileSize * Vector2((float)m_TileCountX, (float)m_TileCountY);

	CCamera* camera = m_Scene->GetCamera();
	camera->SetWorldResolution(m_Size);
}

void CTileMap::LoadWallFile(const char* fileName, const std::string& pathName)
{
	char fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		strcpy_s(fullPath, info->multiBytePath);

	strcat_s(fullPath, fileName);

	LoadWallObjects(fullPath);
}

void CTileMap::SaveWallObjects(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "w");

	if (!pFile)
		return;

	auto iter = m_WallObjects.begin();
	auto iterEnd = m_WallObjects.end();

	fprintf_s(pFile, "%d\n", (int)m_WallObjects.size());

	for (; iter != iterEnd; iter++)
	{
		fprintf_s(pFile, "%d %d %d\n", (int)iter->first, (int)iter->second.x, (int)iter->second.y);
	}

	fclose(pFile);
}

void CTileMap::LoadWallObjects(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "r");

	if (!pFile)
		return;

	m_WallObjects.clear();

	int num;

	fscanf_s(pFile, "%d\n", &num);


	for (int i = 0; i < num; i++)
	{
		int typeID;
		int x;
		int y;

		fscanf_s(pFile, "%d %d %d\n", &typeID, &x, &y);

		Vector2 temp = Vector2((float)x, (float)y);

		m_WallObjects.push_back(std::make_pair((EWallObjectType)typeID, temp));
	}

	fclose(pFile);
}

void CTileMap::LoadFloorfile(const char* fileName, const std::string& pathName)
{
	char fullPath[MAX_PATH] = {};

	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
		strcpy_s(fullPath, info->multiBytePath);

	strcat_s(fullPath, fileName);

	LoadFloorObjects(fullPath);
}

void CTileMap::SaveFloorObjects(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "w");

	if (!pFile)
		return;

	auto iter = m_FloorObjects.begin();
	auto iterEnd = m_FloorObjects.end();

	fprintf_s(pFile, "%d\n", (int)m_FloorObjects.size());

	for (; iter != iterEnd; iter++)
	{
		fprintf_s(pFile, "%d %d %d %d \n", (int)iter->first.first, (int)iter->first.second.x, (int)iter->first.second.y, 
			(int)iter->second);
	}

	fclose(pFile);
}

void CTileMap::LoadFloorObjects(const char* fullPath)
{
	FILE* pFile = nullptr;

	fopen_s(&pFile, fullPath, "r");

	if (!pFile)
		return;

	int size = 0;

	fscanf_s(pFile, "%d\n", &size);

	for (int i = 0; i < size; i++)
	{
		int type;
		int x;
		int y;
		int orientation;

		fscanf_s(pFile, "%d %d %d %d\n", &type, &x, &y, &orientation);

		Vector2 pos = Vector2((float)x, (float)y);

		m_FloorObjects.push_back(std::make_pair(std::make_pair((EFloorObjectType)type, pos), (EFloorObjectOrientation)orientation));
	}

	for (const auto& obj : m_FloorObjects)
	{
		CTile* tile = GetTile(obj.first.second);

		if (tile)
		{
			if (obj.first.first == EFloorObjectType::ITEM_BOX || obj.first.first == EFloorObjectType::OIL_SLICK
				|| obj.first.first == EFloorObjectType::ACCEL_PANEL)
			{
				int indexX = tile->GetIndexX();
				int indexY = tile->GetIndexY();

				if (indexX > m_TileCountX - 2 || indexY > m_TileCountY - 2)
					return;

				CTile* tile2 = GetTile(indexX + 1, indexY);
				CTile* tile3 = GetTile(indexX, indexY + 1);
				CTile* tile4 = GetTile(indexX + 1, indexY + 1);

				tile->SetTileProperty(ETileProperty::OTHER);
				tile2->SetTileProperty(ETileProperty::OTHER);
				tile3->SetTileProperty(ETileProperty::OTHER);
				tile4->SetTileProperty(ETileProperty::OTHER);
			}
			else if (obj.first.first == EFloorObjectType::COIN)
				tile->SetTileProperty(ETileProperty::OTHER);
			else if (obj.first.first == EFloorObjectType::RAMP_HORIZONTAL)
				tile->SetTileProperty(ETileProperty::RAMP_HORIZONTAL);
			else if (obj.first.first == EFloorObjectType::RAMP_VERTICAL)
				tile->SetTileProperty(ETileProperty::RAMP_VERTICAL);
		}
	}

	fclose(pFile);
}

void CTileMap::LoadAI(const std::string& currentPath, const std::string& pathName)
{
	char positionsPath[MAX_PATH] = {};
	char gradientPath[MAX_PATH] = {};


	const PathInfo* info = CPathManager::GetInst()->FindPath(pathName);

	if (info)
	{
		strcpy_s(positionsPath, info->multiBytePath);
		strcpy_s(gradientPath, info->multiBytePath);
	}

	strcat_s(positionsPath, currentPath.c_str());
	strcat_s(gradientPath, currentPath.c_str());

	strcat_s(positionsPath, "positions.txt");
	strcat_s(gradientPath, "gradient.txt");

	CAIGradientDescent::SetGradient(m_vecTile, Vector2(m_WideGoalStartX, m_WideGoalStartY), Vector2(m_WideGoalWidth, m_WideGoalHeight), positionsPath,
		gradientPath);
}
