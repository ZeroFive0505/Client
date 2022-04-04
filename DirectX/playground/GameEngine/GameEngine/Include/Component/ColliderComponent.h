#pragma once

#include "SceneComponent.h"

/// <summary>
/// ���� ��ġ�Ǵ� �ݶ��̴� ������Ʈ
/// </summary>
class CColliderComponent :
	public CSceneComponent
{
	friend class CGameObject;
	friend class CCollision;

protected:
	CColliderComponent();
	CColliderComponent(const CColliderComponent& com);
	virtual ~CColliderComponent();

protected:
	// �浹ü Ÿ��
	Collider_Type m_ColliderType;
	// �浹ü ������
	Vector3 m_Offset;
	// �浹ü �ּ� ����
	Vector3 m_Min;
	// �浹ü �ִ� ����
	Vector3 m_Max;
	// �浹ü�� ���� ������ �������� ��� ���� �ε����� ���� ���ڰ� ���� �� �ִ�.
	std::vector<int> m_vecSectionIndex;
	// ������ �浹�� �ߴ� �浹ü���� ����Ʈ
	std::list<CColliderComponent*> m_PrevCollisionList;
	// ���� �浹ü ����Ʈ
	std::list<CColliderComponent*> m_CurrentCollisionList;
	// �� �浹ü�� ��������
	sCollisionProfile* m_Profile;
	// ���� �������� �浹üũ�� �ߴ���
	bool m_CurrentSectionCheck;
	// �浹 ���
	sCollisionResult m_Result;
	// ���콺 �浹 ���
	sCollisionResult m_MouseResult;
	// �Ϲ� �浹�� �ݹ� �Լ�
	std::list<std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>> m_CollisionCallback[(int)Collision_State::Max];
	// ���콺 �浹�� �ݹ� �Լ�
	std::list<std::function<void(const sCollisionResult&)>> m_CollisionMouseCallback[(int)Collision_State::Max];
	// �� ������Ʈ�� ���콺�� �浹�ߴ���
	bool m_MouseCollision;

	// ������ ��� �޽�
	CSharedPtr<class CMesh> m_Mesh;
	CSharedPtr<class CShader> m_Shader;
	// ������ ��� �޽��� ������ �����ֱ����� ��� ����
	class CColliderConstantBuffer* m_CBuffer;

public:
	inline Collider_Type GetColliderType() const
	{
		return m_ColliderType;
	}

	inline void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	inline Vector3 GetOffset() const
	{
		return m_Offset;
	}

	inline Vector3 GetMin() const
	{
		return m_Min;
	}

	inline Vector3 GetMax() const
	{
		return m_Max;
	}

	inline sCollisionResult GetCollisionResult() const
	{
		return m_Result;
	}

	inline sCollisionProfile* GetCollisionProfile() const
	{
		return m_Profile;
	}

	inline void AddSectionIndex(int index)
	{
		m_vecSectionIndex.push_back(index);
	}

	inline void CurrentSectionCheck()
	{
		m_CurrentSectionCheck = true;
	}

	inline bool GetCurrentSectionCheck() const
	{
		return m_CurrentSectionCheck;
	}

	inline const std::list<CColliderComponent*>& GetPrevColliderList() const
	{
		return m_PrevCollisionList;
	}

	inline const std::list<CColliderComponent*>& GetCurrentColliderList() const
	{
		return m_CurrentCollisionList;
	}

	inline void RenderToggle()
	{
		m_Render = !m_Render;
	}

public:
	// �� �浹ü ������Ʈ�� �浹 ���������� ����
	void SetCollisionProfile(const std::string& name);
	// �� �����ӿ� �浹��� ������ üũ�Ѵ�.
	void CheckPrevColliderSection();
	// �� ������ �浹ü ��Ͽ� �߰�
	void AddPrevCollision(CColliderComponent* collider);
	// �� ������ �浹ü ��Ͽ��� ����
	void DeletePrevCollision(CColliderComponent* collider);
	// �� ������ �浹ü Ŭ����
	bool EmptyPrevCollision();
	// �� ������ �浹ü�� ���� �̹����� ���� �浹ü ������ ��ġ���� �˻��Ѵ�.
	bool CheckPrevCollision(CColliderComponent* collider);
	// ���� ������ �浹ü ����Ʈ�� �ش� �浹ü�� �ִ��� �˻��Ѵ�.
	bool CheckCurrentFrameCollision(CColliderComponent* collider);
	// ���� ������ �浹ü ����Ʈ�� �߰��Ѵ�.
	void AddCurrentFrameCollision(CColliderComponent* collider);
	// �浹�� ���� �ݹ� �Լ��� ȣ��
	void CallCollisionCallback(Collision_State state);
	void CallCollisionMouseCallback(Collision_State state);
	// ��� �浹ó���� ������ �ʱ�ȭ
	void ClearFrame();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void CheckCollision();
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CColliderComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual bool Collision(CColliderComponent* dest) = 0;
	virtual bool CollisionMouse(const Vector2& mousePos) = 0;


	inline void DeleteCollisionCallback(CGameObject* obj)
	{
		m_CollisionCallback[(int)Collision_State::Begin].erase(std::remove_if(m_CollisionCallback[(int)Collision_State::Begin].begin(), m_CollisionCallback[(int)Collision_State::Begin].end(),
			[&](std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>& item) { return item.second == obj; }));

		m_CollisionCallback[(int)Collision_State::End].erase(std::remove_if(m_CollisionCallback[(int)Collision_State::End].begin(), m_CollisionCallback[(int)Collision_State::End].end(),
			[&](std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>& item) { return item.second == obj; }));
	}

	inline void ClearCallback()
	{
		m_CollisionCallback[(int)Collision_State::Begin].clear();
		m_CollisionCallback[(int)Collision_State::End].clear();
		m_CollisionMouseCallback[(int)Collision_State::Begin].clear();
		m_CollisionMouseCallback[(int)Collision_State::End].clear();
	}
	
public:
	template <typename T>
	void AddCollisionCallback(Collision_State state, T* obj, void(T::* func)(const sCollisionResult&))
	{
		m_CollisionCallback[(int)state].push_back(std::make_pair(std::bind(func, obj, std::placeholders::_1), (CGameObject*)obj));
	}

	template <typename T>
	void AddCollisionMouseCallback(Collision_State state, T* obj, void(T::* func)(const sCollisionResult&))
	{
		m_CollisionMouseCallback[(int)state].push_back(std::bind(func, obj, std::placeholders::_1));
	}
};

