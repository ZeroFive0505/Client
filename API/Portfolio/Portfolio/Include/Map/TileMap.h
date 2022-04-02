#pragma once

#include "MapBase.h"
#include "../Resource/Texture.h"
#include "Tile.h"

class CTileMap :
	public CMapBase
{
	friend class CScene;

protected:
	CTileMap();
	virtual ~CTileMap();

protected:
	std::vector<CTile*> m_vecTile;

	std::list<std::pair<EWallObjectType, Vector2>> m_WallObjects;
	std::list<std::pair<std::pair<EFloorObjectType, Vector2>, EFloorObjectOrientation>> m_FloorObjects;

	ETileMaterial m_TrackMat;
	ETileMaterial m_SlowMat1;
	ETileMaterial m_SlowMat2;
	ETileMaterial m_HazardMat;


	int m_WideGoalStartX;
	int m_WideGoalStartY;

	int m_WideGoalWidth;
	int m_WideGoalHeight;

	int m_GoalStartX;
	int m_GoalStartY;

	int m_GoalWidth;
	int m_GoalHeight;

	int m_TileCountX;
	int m_TileCountY;

	Vector2 m_TileSize;

	int m_StartX;
	int m_StartY;
	int m_EndX;
	int m_EndY;

	int m_AIFarVision;

public:
	inline int GetAIFarVision() const
	{
		return m_AIFarVision;
	}

	inline int GetTileCountX() const
	{
		return m_TileCountX;
	}

	inline int GetTileCountY() const
	{
		return m_TileCountY;
	}

	inline Vector2 GetTileSize() const
	{
		return m_TileSize;
	}

	inline ETileMaterial GetTrackMat() const
	{
		return m_TrackMat;
	}

	inline ETileMaterial GetSlowMat() const
	{
		return m_SlowMat1;
	}

	inline ETileMaterial GetSlowMat2() const
	{
		return m_SlowMat2;
	}

	inline ETileMaterial GetHazardMat() const
	{
		return m_HazardMat;
	}

	inline int GetWideGoalStartX() const
	{
		return m_WideGoalStartX;
	}

	inline int GetWideGoalStartY() const
	{
		return m_WideGoalStartY;
	}

	inline int GetWideGoalWidth() const
	{
		return m_WideGoalWidth;
	}

	inline int GetWideGoalHeight() const
	{
		return m_WideGoalHeight;
	}

	inline int GetGoalStartX() const
	{
		return m_GoalStartX;
	}

	inline int GetGoalStartY() const
	{
		return m_GoalStartY;
	}

	inline int GetGoalWidth() const
	{
		return m_GoalWidth;
	}

	inline int GetGoalHeight() const
	{
		return m_GoalHeight;
	}

	inline std::list<std::pair<EWallObjectType, Vector2>> GetWallObjects() const
	{
		return m_WallObjects;
	}

	inline std::list<std::pair<std::pair<EFloorObjectType, Vector2>, EFloorObjectOrientation>> GetFloorObjects() const
	{
		return m_FloorObjects;
	}


public:

	void inline SetTrackMat(ETileMaterial mat)
	{
		m_TrackMat = mat;
	}

	void inline SetSlowMat(ETileMaterial mat)
	{
		m_SlowMat1 = mat;
	}

	void inline SetSlowMat2(ETileMaterial mat)
	{
		m_SlowMat2 = mat;
	}

	void inline SetHazardMat(ETileMaterial mat)
	{
		m_HazardMat = mat;
	}

	void inline SetWideGoalStartX(int x)
	{
		m_WideGoalStartX = x;
	}

	void inline SetWideGoalStartY(int y)
	{
		m_WideGoalStartY = y;
	}

	void inline SetWideGoalWidth(int width)
	{
		m_WideGoalWidth = width;
	}

	void inline SetWideGoalHeight(int height)
	{
		m_WideGoalHeight = height;
	}

	void inline SetGoalStartX(int x)
	{
		m_GoalStartX = x;
	}

	void inline SetGoalStartY(int y)
	{
		m_GoalStartY = y;
	}

	void inline SetGoalWidth(int width)
	{
		m_GoalWidth = width;
	}

	void inline SetGoalHeight(int height)
	{
		m_GoalHeight = height;
	}

	void inline SetAIVision(int vision)
	{
		m_AIFarVision = vision;
	}

	
	bool CreateTile(int countX, int countY, const Vector2& tileSize);

	void ChangeTileProperty(const Vector2& pos, ETileProperty tileProperty);

	void CreateWallObject(const Vector2& pos, const EWallObjectType type);

	void DeleteWallObject(const Vector2& pos, const Vector2& size, const Vector2& pivot);

	void CreateFloorObject(const Vector2& pos, const EFloorObjectType type, const EFloorObjectOrientation orientation);

	void DeleteFloorObject(const Vector2& pos, const Vector2& size, const Vector2& pivot, EFloorObjectType type);

	CTile* GetTile(const Vector2& pos);
	CTile* GetTile(int index);
	CTile* GetTile(int indexX, int indexY);

public:
	Vector2 GetPlayerInitPos(int index);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void Render(HDC hDC);
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

public:
	void LoadWallFile(const char* fileName, const std::string& pathName = MAP_PATH);
	void SaveWallObjects(const char* fullPath);
	void LoadWallObjects(const char* fullPath);

	
public:
	void LoadFloorfile(const char* fileName, const std::string& pathName = MAP_PATH);
	void SaveFloorObjects(const char* fullPath);
	void LoadFloorObjects(const char* fullPath);
	
public:
	void LoadAI(const std::string& currentPath, const std::string& pathName = MAP_PATH);
};

