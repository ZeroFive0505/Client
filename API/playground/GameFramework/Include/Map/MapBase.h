#pragma once

#include "../GameInfo.h"

/// <summary>
/// 타일맵의 기반이 되는 클래스이다.
/// </summary>
class CMapBase
{
	// 씬은 맵을 마음대로 생성이 가능.
	friend class CScene;

protected:
	CMapBase();
	virtual ~CMapBase();

protected:
	// 어느 씬에 속해있는지
	class CScene* m_Scene;
	// 이름은 무엇인지 (MapBase클래스의 경우 Ref클래스를 상속받지 않아 이름을 따로 정의한다.)
	std::string m_Name;
	// 위치
	Vector2 m_Pos;
	// 크기
	Vector2 m_Size;
	// Z
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

	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos.x = x;
		m_Pos.y = y;
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size.x = x;
		m_Size.y = y;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render(HDC hDC);
	virtual void SaveFile(const char* FileName, const std::string& PathName = MAP_PATH);
	virtual void SaveFullPath(const char* FullPath);
	virtual void Save(FILE* pFile);
	virtual void LoadFile(const char* FileName, const std::string& PathName = MAP_PATH);
	virtual void LoadFullPath(const char* FullPath);
	virtual void Load(FILE* pFile);
};

