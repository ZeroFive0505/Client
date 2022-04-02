#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Object/GameObject.h"
#include "../Map/TileMap.h"
#include "../Object/WallObject.h"
#include "../Object/FloorObject.h"
#include "../UI/UIWindow.h"
#include "../Object/Item.h"
#include "../Object/Driver.h"

class CScene
{
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	class CSceneResource* m_Resouece;
	class CCamera* m_Camera;

protected:
	std::vector<class CMapBase*> m_MapList;

	std::list<CSharedPtr<CGameObject>> m_ObjList;


protected:
	CTileMap* m_TileMap;

	CUIWindow** m_UIArray;
	int m_UICount;
	int m_UICapacity;

public:
	inline class CSceneResource* GetSceneResoruce() const
	{
		return m_Resouece;
	}

	inline class CCamera* GetCamera() const
	{
		return m_Camera;
	}

	inline CTileMap* GetTileMap() const
	{
		if (!m_TileMap)
			return nullptr;

		return m_TileMap;
	}

public:
	virtual bool Init();
	virtual bool Update(float deltaTime);
	virtual bool PostUpdate(float deltaTime);
	virtual bool Collision(float deltaTime);
	virtual bool Render(HDC hDC);


public:
	static int SortY(const void* src, const void* dest);
	static int SortZ(const void* src, const void* dest);

public:
	template <typename T>
	T* CreateObject(const std::string& name, const Vector2& pos = Vector2(0.0f, 0.0f),
		const Vector2& size = Vector2(100.0f, 100.0f))
	{
		T* obj = new T;

		obj->SetScene(this);
		obj->SetPos(pos);
		obj->SetSize(size);
		obj->SetName(name);

		if (!obj->Init())
		{
			SAFE_DELETE(obj);
			return nullptr;
		}

		m_ObjList.push_back(obj);

		return obj;
	}



	template <typename T>
	T* CreateMap(const std::string& name, const Vector2& pos = Vector2(0.0f, 0.0f),
		const Vector2& size = Vector2(1024.0f, 1024.0f))
	{
		T* map = new T;

		map->SetScene(this);
		map->SetPos(pos);
		map->SetSize(size);
		map->SetName(name);

		if (!map->Init())
		{
			SAFE_DELETE(map);

			return nullptr;
		}

		if (typeid(T).hash_code() == typeid(CTileMap).hash_code())
			m_TileMap = (CTileMap*)map;

		m_MapList.push_back(map);

		return map;
	}

	template <typename T>
	T* CreateUIWindow(const std::string& name)
	{
		T* window = new T;

		window->SetName(name);
		window->SetScene(this);

		if (!window->Init())
		{
			SAFE_DELETE(window);
			return nullptr;
		}

		if (m_UICount == m_UICapacity)
		{
			m_UICapacity *= 2;

			CUIWindow** arr = new CUIWindow * [m_UICapacity];

			memcpy(arr, m_UIArray, sizeof(CUIWindow*) * m_UICount);

			SAFE_DELETE_ARRAY(m_UIArray);

			m_UIArray = arr;
		}

		m_UIArray[m_UICount++] = window;

		return window;
	}

	template <typename T>
	T* FindUIWindow(const std::string& name)
	{
		for (int i = 0; i < m_UICount; i++)
		{
			if (m_UIArray[i]->GetName() == name)
				return (T*)m_UIArray[i];
		}

		return nullptr;
	}
};

