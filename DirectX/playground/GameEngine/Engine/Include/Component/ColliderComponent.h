#pragma once

#include "SceneComponent.h"

/// <summary>
/// 씬에 배치되는 콜라이더 컴포넌트
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
	// 충돌체 타입
	Collider_Type m_ColliderType;
	// 충돌체 오프셋
	Vector3 m_Offset;
	// 충돌체 최소 지점
	Vector3 m_Min;
	// 충돌체 최대 지점
	Vector3 m_Max;
	// 충돌체가 여러 영역에 걸쳐있을 경우 영역 인덱스는 여러 숫자가 들어올 수 있다.
	std::vector<int> m_vecSectionIndex;
	// 이전에 충돌을 했던 충돌체들의 리스트
	std::list<CColliderComponent*> m_PrevCollisionList;
	// 현재 충돌체 리스트
	std::list<CColliderComponent*> m_CurrentCollisionList;
	// 이 충돌체의 프로파일
	sCollisionProfile* m_Profile;
	// 현재 영역에서 충돌체크를 했는지
	bool m_CurrentSectionCheck;
	// 충돌 결과
	sCollisionResult m_Result;
	// 마우스 충돌 결과
	sCollisionResult m_MouseResult;
	// 일반 충돌시 콜백 함수
	std::list<std::pair<std::function<void(const sCollisionResult&)>, CGameObject*>> m_CollisionCallback[(int)Collision_State::Max];
	// 마우스 충돌시 콜백 함수
	std::list<std::function<void(const sCollisionResult&)>> m_CollisionMouseCallback[(int)Collision_State::Max];
	// 이 컴포넌트가 마우스와 충돌했는지
	bool m_MouseCollision;

	// 디버깅용 출력 메쉬
	CSharedPtr<class CMesh> m_Mesh;
	CSharedPtr<class CShader> m_Shader;
	// 디버깅용 출력 메쉬의 색깔을 정해주기위한 상수 버퍼
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
	// 이 충돌체 컴포넌트의 충돌 프로파일을 설정
	void SetCollisionProfile(const std::string& name);
	// 전 프레임에 충돌됬던 영역을 체크한다.
	void CheckPrevColliderSection();
	// 전 프레임 충돌체 목록에 추가
	void AddPrevCollision(CColliderComponent* collider);
	// 전 프레임 충돌체 목록에서 제거
	void DeletePrevCollision(CColliderComponent* collider);
	// 전 프레임 충돌체 클리어
	bool EmptyPrevCollision();
	// 전 프레임 충돌체와 같은 이번에도 같은 충돌체 영역이 겹치는지 검사한다.
	bool CheckPrevCollision(CColliderComponent* collider);
	// 현재 프레임 충돌체 리스트에 해당 충돌체가 있는지 검사한다.
	bool CheckCurrentFrameCollision(CColliderComponent* collider);
	// 현재 프레임 충돌체 리스트에 추가한다.
	void AddCurrentFrameCollision(CColliderComponent* collider);
	// 충돌시 묶인 콜백 함수를 호출
	void CallCollisionCallback(Collision_State state);
	void CallCollisionMouseCallback(Collision_State state);
	// 모든 충돌처리가 끝난후 초기화
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

