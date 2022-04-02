#pragma once

#include "../Ref.h"

/// <summary>
/// Collider Ŭ������ �������� Ŭ�����̴�. ���� �ν��Ͻ�ȭ�� �Ұ����ϸ� ���� ����� �ްų� �ؾ��Ѵ�.
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
	// Collider�� �����ִ� ���� ������
	class CScene* m_Scene;
	class CGameObject* m_Owner;
	// �浹ü Ÿ��
	ECollider_Type m_Type;
	Vector2 m_Offset;
	bool m_Enable;
	CollisionProfile* m_Profile;
	// �浹�� ����
	std::function<void(CCollider*, CCollider*, float)> m_BeginFunction;
	// �浹�� ��������
	std::function<void(CCollider*, CCollider*, float)> m_EndFunction;

	// ���콺�� �浹�� �˻�
	std::function<void(CCollider*, const Vector2&, float)> m_MouseBeginFunction;
	std::function<void(CCollider*, const Vector2&, float)> m_MouseEndFunction;

	std::list<CSharedPtr<CCollider>> m_CollisionList;

	bool m_MouseCollision;

public:
	void SetMouseCollision(bool MouseCollision)
	{
		m_MouseCollision = MouseCollision;
	}


	// �ش� �浹ü�� �浹���θ� ����
	void SetEnable(bool Enable)
	{
		m_Enable = Enable;
	}

	// �ش� �浹ü�� �������� ����
	void SetOffset(const Vector2& Offset)
	{
		m_Offset = Offset;
	}

	void SetOffset(float x, float y)
	{
		m_Offset = Vector2(x, y);
	}

	// �浹ü�� �����ɽ� � ���� �ҼӵǾ��ִ���
	void SetScene(class CScene* Scene)
	{
		m_Scene = Scene;
	}

	// �浹ü ������ �����ڴ� ��������.
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
	// �ش� �浹ü�� �浹 �������� ����
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
	// ��� Ÿ�԰��� �浹�� �׽�Ʈ�� �� �ְ� ���� �⺻���� ColliderŸ���� ���ڷ� �޴´�.
	// ���� ���� �Լ��̱⿡ ������ ������ ������Ѵ�.
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

