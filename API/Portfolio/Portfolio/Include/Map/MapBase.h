#pragma once

#include "../GameInfo.h"

class CMapBase
{
	friend class CScene;

protected:
	CMapBase();
	virtual ~CMapBase();

protected:
	class CScene* m_Scene;

	std::string m_Name;

	Vector2 m_Pos;

	Vector2 m_Size;

	int m_ZOrder;
	int m_Enable;
	int m_Active;

public:
	bool IsActive() const
	{
		return m_Active;
	}

	bool IsEnable() const
	{
		return m_Enable;
	}

	const std::string& GetName() const
	{
		return m_Name;
	}

	const Vector2& GetPos() const
	{
		return m_Pos;
	}

	const Vector2& GetSize() const
	{
		return m_Size;
	}

	int GetZOrder() const
	{
		return m_ZOrder;
	}

public:
	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	void SetScene(class CScene* scene)
	{
		m_Scene = scene;
	}

	void SetName(const std::string& name)
	{
		m_Name = name;
	}

	void SetPos(const Vector2& pos)
	{
		m_Pos = pos;
	}

	void SetSize(const Vector2& size)
	{
		m_Size = size;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float detlaTime);
	virtual void Render(HDC hDC);
	virtual void SaveFile(const char* fileName, const std::string& pathName = MAP_PATH);
	virtual void SaveFullPath(const char* fullPath);
	virtual void Save(FILE* pFile);
	virtual void LoadFile(const char* fileName, const std::string& pathName = MAP_PATH);
	virtual void LoadFullPath(const char* fullPath);
	virtual void Load(FILE* pFile);
};

