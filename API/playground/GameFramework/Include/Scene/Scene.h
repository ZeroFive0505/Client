#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../UI/UIWindow.h"
#include "../Map/TileMap.h"

/// <summary>
/// 가장 기본이 되는 씬의 베이스 클래스이다.
/// 씬은 해당 씬에 속해있는 리소드들을 가지고 있다.
/// </summary>
class CScene
{
	// 메인씬과 마찬가지로 씬 관리자는 이러한 씬들을 생성 관리하기에 프렌드로 선언한다.
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	// 해당 씬에 속해있는 리소스들은 씬 리소스가 가지고 있는다.
	class CSceneResource* m_Resource;
	// 해당 씬에 속해있는 충돌체들을 가지고 있는다.
	class CSceneCollision* m_Collision;
	// 씬이 가지고 있는 카메라
	class CCamera* m_Camera;

public:
	class CSceneResource* GetSceneResource()	const;
	class CSceneCollision* GetSceneCollision() const;
	class CCamera* GetCamera() const;

private:
	std::vector<class CMapBase*> m_MapList;
	CSharedPtr<CGameObject> m_Player;

	// 게임 오브젝트를 기반으로한 리스트이다. 각 게임 오브젝트를 순회하면서
	// Update를 호출하면 해당 오브젝트에 맞는 Update 함수가 호출될 것이다. (다형성)
	std::list<CSharedPtr<CGameObject>>	m_ObjList;

	CGameObject** m_RenderArray;
	int m_RenderCount;
	int m_RenderCapacity;

	// 프로토 타입
	std::unordered_map<std::string, CSharedPtr<CGameObject>> m_mapPrototype;


	// std::list<CSharedPtr<CUIWindow>> m_UIList;

	CUIWindow** m_UIArray;
	int m_UICount;
	int m_UICapacity;

	CTileMap* m_TileMap;

public:
	CGameObject* FindObject(const std::string& Name);
	void SetPlayer(const std::string& Name);
	void SetPlayer(CGameObject* Player);
	CGameObject* GetPlayer() const
	{
		return m_Player;
	}

	CTileMap* GetTileMap() const
	{
		return m_TileMap;
	}

public:
	virtual bool Init();
	virtual bool Update(float DeltaTime);
	virtual bool PostUpdate(float DeltaTime);
	virtual bool Collision(float DeltaTime);
	virtual bool Render(HDC hDC);

public:
	static int SortY(const void* Src, const void* Dest);
	static int SortZOrder(const void* Src, const void* Dest);
	static int SortZOrderMap(const void* Src, const void* Dest);

private:
	// 씬에 저장되어 있는 프로토타입을 조회하고 반환한다.
	CGameObject* FindPrototype(const std::string& Name);

public:
	/// <summary>
	/// 씬에 T 타입의 오브젝트를 만든다.
	/// </summary>
	/// <typeparam name="T">오브젝트 타입</typeparam>
	/// <param name="Name">이름</param>
	/// <param name="Pos">위치</param>
	/// <param name="Size">크기</param>
	/// <returns>해당 오브젝트 반환</returns>
	template <typename T>
	T* CreateObject(const std::string& Name,
		const Vector2& Pos = Vector2(0.f, 0.f),
		const Vector2& Size = Vector2(100.f, 100.f))
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_ObjList.push_back(Obj);

		return Obj;
	}


	/// <summary>
	/// 프로토타입을 만든다.
	/// 생성 후 unordered_map에 기록된다.
	/// </summary>
	/// <typeparam name="T">생성하고자 하는 타입</typeparam>
	/// <param name="Name">프로토타입 이름</param>
	/// <returns>생성된 프로토 타입 반환</returns>
	template <typename T>
	T* CreatePrototype(const std::string& Name)
	{
		T* Obj = new T;

		Obj->SetScene(this);
		Obj->SetName(Name);

		if (!Obj->Init())
		{
			SAFE_DELETE(Obj);
			return nullptr;
		}

		m_mapPrototype.insert(std::make_pair(Name, Obj));

		return Obj;
	}

	/// <summary>
	/// 씬에 저장되어있는 프로로타입을 이름으로 조회해서 생성한다.
	/// 이 경우 Init을 하지 않고 Start를 호출하면서 값을 초기화한다.
	/// </summary>
	/// <typeparam name="T">생성하고자 하는 타입</typeparam>
	/// <param name="Name">생성 후 이름</param>
	/// <param name="PrototypeName">프로토 타입 이름(꼭 매치되는 이름이어야함)</param>
	/// <param name="Pos">위치</param>
	/// <param name="Size">크기</param>
	/// <returns>복사된 프로토타입 반환</returns>
	template <typename T>
	T* CreateObject(const std::string& Name, const std::string& PrototypeName, const Vector2& Pos = Vector2(0.0f, 0.0f),
		const Vector2& Size = Vector2(100.0f, 100.0f))
	{
		CGameObject* Prototype = FindPrototype(PrototypeName);

		if (!Prototype)
			return nullptr;

		T* Obj = (T*)Prototype->Clone();

		Obj->SetScene(this);
		Obj->SetPos(Pos);
		Obj->SetSize(Size);
		Obj->SetName(Name);

		m_ObjList.push_back(Obj);

		return Obj;
	}

	/// <summary>
	/// 새로운 윈도우를 씬에 생성한다.
	/// </summary>
	/// <typeparam name="T">생성하고자 하는 윈도우 타입</typeparam>
	/// <param name="Name">윈도우의 이름</param>
	/// <returns>생성된 윈도우</returns>
	template <typename T>
	T* CreateUIWindow(const std::string& Name)
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->SetScene(this);

		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}


		// 꽉차면 늘려준다.
		if (m_UICount == m_UICapacity)
		{
			m_UICapacity *= 2;

			CUIWindow** Array = new CUIWindow * [m_UICapacity];

			memcpy(Array, m_UIArray, sizeof(CUIWindow*) * m_UICount);

			SAFE_DELETE_ARRAY(m_UIArray);

			m_UIArray = Array;
		}

		m_UIArray[m_UICount++] = Window;

		return Window;
	}


	template <typename T>
	T* FindUIWindow(const std::string& Name)
	{
		for (int i = 0; i < m_UICount; i++)
		{
			if (m_UIArray[i]->GetName() == Name)
				return (T*)m_UIArray[i];
		}
		

		return nullptr;
	}

	template <typename T>
	T* CreateMap(const std::string& Name, const Vector2& Pos = Vector2(0.0f, 0.0f),
		const Vector2& Size = Vector2(1000.0f, 1000.f))
	{
		T* Map = new T;

		Map->SetScene(this);
		Map->SetPos(Pos);
		Map->SetSize(Size);
		Map->SetName(Name);

		if (!Map->Init())
		{
			SAFE_DELETE(Map);
			return nullptr;
		}

		if (typeid(T).hash_code() == typeid(CTileMap).hash_code())
			m_TileMap = (CTileMap*)Map;

		m_MapList.push_back(Map);

		return Map;
	}
};

