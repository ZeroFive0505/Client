#pragma once

#include "../GameInfo.h"
#include "../Object/GameObject.h"
#include "../UI/UIWindow.h"
#include "../Map/TileMap.h"

/// <summary>
/// ���� �⺻�� �Ǵ� ���� ���̽� Ŭ�����̴�.
/// ���� �ش� ���� �����ִ� ���ҵ���� ������ �ִ�.
/// </summary>
class CScene
{
	// ���ξ��� ���������� �� �����ڴ� �̷��� ������ ���� �����ϱ⿡ ������� �����Ѵ�.
	friend class CSceneManager;

protected:
	CScene();
	virtual ~CScene();

private:
	// �ش� ���� �����ִ� ���ҽ����� �� ���ҽ��� ������ �ִ´�.
	class CSceneResource* m_Resource;
	// �ش� ���� �����ִ� �浹ü���� ������ �ִ´�.
	class CSceneCollision* m_Collision;
	// ���� ������ �ִ� ī�޶�
	class CCamera* m_Camera;

public:
	class CSceneResource* GetSceneResource()	const;
	class CSceneCollision* GetSceneCollision() const;
	class CCamera* GetCamera() const;

private:
	std::vector<class CMapBase*> m_MapList;
	CSharedPtr<CGameObject> m_Player;

	// ���� ������Ʈ�� ��������� ����Ʈ�̴�. �� ���� ������Ʈ�� ��ȸ�ϸ鼭
	// Update�� ȣ���ϸ� �ش� ������Ʈ�� �´� Update �Լ��� ȣ��� ���̴�. (������)
	std::list<CSharedPtr<CGameObject>>	m_ObjList;

	CGameObject** m_RenderArray;
	int m_RenderCount;
	int m_RenderCapacity;

	// ������ Ÿ��
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
	// ���� ����Ǿ� �ִ� ������Ÿ���� ��ȸ�ϰ� ��ȯ�Ѵ�.
	CGameObject* FindPrototype(const std::string& Name);

public:
	/// <summary>
	/// ���� T Ÿ���� ������Ʈ�� �����.
	/// </summary>
	/// <typeparam name="T">������Ʈ Ÿ��</typeparam>
	/// <param name="Name">�̸�</param>
	/// <param name="Pos">��ġ</param>
	/// <param name="Size">ũ��</param>
	/// <returns>�ش� ������Ʈ ��ȯ</returns>
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
	/// ������Ÿ���� �����.
	/// ���� �� unordered_map�� ��ϵȴ�.
	/// </summary>
	/// <typeparam name="T">�����ϰ��� �ϴ� Ÿ��</typeparam>
	/// <param name="Name">������Ÿ�� �̸�</param>
	/// <returns>������ ������ Ÿ�� ��ȯ</returns>
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
	/// ���� ����Ǿ��ִ� ���η�Ÿ���� �̸����� ��ȸ�ؼ� �����Ѵ�.
	/// �� ��� Init�� ���� �ʰ� Start�� ȣ���ϸ鼭 ���� �ʱ�ȭ�Ѵ�.
	/// </summary>
	/// <typeparam name="T">�����ϰ��� �ϴ� Ÿ��</typeparam>
	/// <param name="Name">���� �� �̸�</param>
	/// <param name="PrototypeName">������ Ÿ�� �̸�(�� ��ġ�Ǵ� �̸��̾����)</param>
	/// <param name="Pos">��ġ</param>
	/// <param name="Size">ũ��</param>
	/// <returns>����� ������Ÿ�� ��ȯ</returns>
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
	/// ���ο� �����츦 ���� �����Ѵ�.
	/// </summary>
	/// <typeparam name="T">�����ϰ��� �ϴ� ������ Ÿ��</typeparam>
	/// <param name="Name">�������� �̸�</param>
	/// <returns>������ ������</returns>
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


		// ������ �÷��ش�.
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

