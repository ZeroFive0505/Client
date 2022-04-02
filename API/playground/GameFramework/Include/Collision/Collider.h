#pragma once

#include "../Ref.h"

/// <summary>
/// Collider 클래스는 순수가상 클래스이다. 따라서 인스턴스화는 불가능하며 따로 상속을 받거나 해야한다.
/// </summary>
class CCollider :
	public CRef
{
	friend class CGameObject;
	
protected:
	CCollider();
	CCollider(const CCollider& collider);
	virtual ~CCollider() = 0;

protected:
	// Collider가 속해있는 씬과 소유자
	class CScene* m_Scene;
	class CGameObject* m_Owner;
	// 충돌체 타입
	ECollider_Type m_Type;
	Vector2 m_Offset;
	bool m_Enable;
	CollisionProfile* m_Profile;
	// 충돌을 시작
	std::function<void(CCollider*, CCollider*, float)> m_BeginFunction;
	// 충돌이 끝났을시
	std::function<void(CCollider*, CCollider*, float)> m_EndFunction;

	// 마우스와 충돌을 검사
	std::function<void(CCollider*, const Vector2&, float)> m_MouseBeginFunction;
	std::function<void(CCollider*, const Vector2&, float)> m_MouseEndFunction;

	std::list<CSharedPtr<CCollider>> m_CollisionList;

	bool m_MouseCollision;

public:
	void SetMouseCollision(bool MouseCollision)
	{
		m_MouseCollision = MouseCollision;
	}


	// 해당 충돌체의 충돌여부를 설정
	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}

	// 해당 충돌체의 오프셋을 설정
	void SetOffset(const Vector2& Offset)
	{
		m_Offset = Offset;
	}

	void SetOffset(float x, float y)
	{
		m_Offset = Vector2(x, y);
	}

	// 충돌체가 생설될시 어떤 씬에 소속되어있는지
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	// 충돌체 생성시 소유자는 누구인지.
	void SetOwner(class CGameObject* Owner)
	{
		m_Owner = Owner;
	}

public:
	virtual float GetBottom() const
	{
		return 0.0f;
	}

	bool GetEnable() const
	{
		return m_Enable;
	}

	class CGameObject* GetOwner() const
	{
		return m_Owner;
	}

	class CScene* GetScene() const
	{
		return m_Scene;
	}

	ECollider_Type GetColliderType() const
	{
		return m_Type;
	}

	CollisionProfile* GetProfile() const
	{
		return m_Profile;
	}

public:
	// 해당 충돌체의 충돌 프로파일 설정
	void SetCollisionProfile(const std::string& Name);
	void AddCollisionList(CCollider* Collider);
	bool CheckCollisionList(CCollider* Collider);
	void DeleteCollisionList(CCollider* Collider);
	void ClearCollisionList();
	void CallCollisionBegin(CCollider* Dest, float DeltaTime);
	void CallCollisionEnd(CCollider* Dest, float DeltaTime);
	void CallMouseCollsionBegin(const Vector2& MousePos, float DeltaTime);
	void CallMouseCollsionEnd(const Vector2& MousePos, float DeltaTime);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render(HDC hDC);
	virtual CCollider* Clone();
	// 어떠한 타입과도 충돌을 테스트할 수 있게 가장 기본원형 Collider타입을 인자로 받는다.
	// 순수 가상 함수이기에 무조건 재정의 해줘야한다.
	virtual bool Collision(CCollider* Dest) = 0;
	virtual bool CollisionMouse(const Vector2& MousePos) = 0;

public:
	template <typename T>
	void SetCollisionBeginFunction(T* Obj, void(T::* Func)(CCollider*, CCollider*, float))
	{
		m_BeginFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	template <typename T>
	void SetCollisionEndFunction(T* Obj, void(T::* Func)(CCollider*, CCollider*, float))
	{
		m_EndFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	template <typename T>
	void SetMouseCollisionBeginFunction(T* Obj, void(T::* Func)(CCollider*, const Vector2&, float))
	{
		m_MouseBeginFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}

	template <typename T>
	void SetMouseCollisionEndFunction(T* Obj, void(T::* Func)(CCollider*, const Vector2&, float))
	{
		m_MouseEndFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	}
};

