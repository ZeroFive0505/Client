#pragma once

#include "../Component/SceneComponent.h"
#include "../Component/ObjectComponent.h"

class CGameObject :
	public CRef
{
	friend class CScene;

protected:
	CGameObject();
	CGameObject(const CGameObject& obj);
	virtual ~CGameObject();

protected:
	class CScene* m_Scene;

public:
	class CScene* GetScene() const
	{
		return m_Scene;
	}

public:
	void SetScene(class CScene* scene);

protected:
	// 씬 컴포넌트의 경우 상하계층 구조로 이루어져 있음
	CSharedPtr<CSceneComponent> m_RootComponent;
	std::list<CSceneComponent*> m_SceneComponentList;
	std::vector<CSharedPtr<CObjectComponent>> m_vecObjectComponent;

	CGameObject* m_Parent;
	std::vector<CSharedPtr<CGameObject>> m_vecChildObject;
	float m_LifeSpan;
	float m_YPos;

public:
	inline void SetRootComponent(CSceneComponent* component)
	{
		m_RootComponent = component;
	}

	inline CSceneComponent* GetRootComponent() const
	{
		return m_RootComponent;
	}

	inline void AddSceneComponent(CSceneComponent* component)
	{
		m_SceneComponentList.push_back(component);
	}

	inline void SetLifeSpan(float lifeSpan)
	{
		m_LifeSpan = lifeSpan;
	}

	inline void SetCustomY(float y)
	{
		m_YPos = y;
	}

	inline float GetCustomY() const
	{
		return m_YPos;
	}

	// 게임오브젝트안에서 컴포넌트를 찾아온다.
	class CComponent* FindComponent(const std::string name);
	void GetAllSceneComponentsName(std::vector<sFindComponentName>& vecNames);


	void AddChildGameObject(CGameObject* obj);

public:
	virtual void Destroy();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	void AddCollision();
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CGameObject* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);
	virtual void Save(const char* fullPath);
	virtual void Load(const char* fullPath);
	virtual void Save(const char* fileName, const std::string& pathName);
	virtual void Load(const char* fileName, const std::string& pathName);

public:
	// NavAgent
	void Move(const Vector3& pos);

public:
	template <typename T>
	T* FindComponentByType()
	{
		auto iter = m_SceneComponentList.begin();
		auto iterEnd = m_SceneComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			if ((*iter)->TypeCheck<T>())
				return (T*)*iter;
		}

		return nullptr;
	}

public:
	template <typename T>
	T* CreateComponent(const std::string& name)
	{
		T* component = new T;

		component->SetName(name);
		component->SetScene(m_Scene);
		component->SetGameObject(this);

		if (!component->Init())
		{
			SAFE_RELEASE(component);
			return nullptr;
		}

		if (component->GetComponentType() == Component_Type::ObjectComponent)
			m_vecObjectComponent.push_back((CObjectComponent*)component);
		else
		{
			m_SceneComponentList.push_back((CSceneComponent*)component);

			if (!m_RootComponent)
				m_RootComponent = (CSceneComponent*)component;
		}

		return component;
	}

	template <typename T>
	T* LoadComponent()
	{
		T* component = new T;

		component->SetScene(m_Scene);
		component->SetGameObject(this);

		if (component->GetComponentType() == Component_Type::ObjectComponent)
			m_vecObjectComponent.push_back((CObjectComponent*)component);
		else
		{
			m_SceneComponentList.push_back((CSceneComponent*)component);

			if (!m_RootComponent)
				m_RootComponent = component;
		}

		return component;
	}

public: // 게임오브젝트에 루트 컴포넌트에 접근하는 함수
	inline void SetInheritScale(bool inherit)
	{
		m_RootComponent->SetInheritScale(inherit);
	}

	inline void SetInheritRotx(bool inherit)
	{
		m_RootComponent->SetInheritRotX(inherit);
	}

	inline void SetInheritRotY(bool inherit)
	{
		m_RootComponent->SetInheritRotY(inherit);
	}

	inline void SetInheritRotZ(bool inherit)
	{
		m_RootComponent->SetInheritRotZ(inherit);
	}

	inline void SetInheritParentRotationPosX(bool inherit)
	{
		m_RootComponent->SetInheritParentRotationPosX(inherit);
	}

	inline void SetInheritParentRotationPosY(bool inherit)
	{
		m_RootComponent->SetInheritParentRotationPosY(inherit);
	}

	inline void SetInheritParentRotationPosZ(bool inherit)
	{
		m_RootComponent->SetInheritParentRotationPosZ(inherit);
	}

	inline void InheritScale(bool current)
	{
		m_RootComponent->InheritScale(current);
	}

	inline void InheritRotation(bool current)
	{
		m_RootComponent->InheritRotation(current);
	}

	inline void InheritParentRotationPos(bool current)
	{
		m_RootComponent->InheritParentRotationPos(current);
	}

	inline void InheritWorldScale(bool current)
	{
		m_RootComponent->InheritWorldScale(current);
	}

	inline void InheritWorldRotation(bool current)
	{
		m_RootComponent->InheritWorldRotation(current);
	}


public:
	inline Vector3 GetRelativeScale() const
	{
		return m_RootComponent->GetRelativeScale();
	}

	inline Vector3 GetRelativeRot() const
	{
		return m_RootComponent->GetRelativeRot();
	}

	inline Vector3 GetRelativePos() const
	{
		return m_RootComponent->GetRelativePos();
	}

	inline Vector3 GetRelativeAxis(AXIS axis) const
	{
		return m_RootComponent->GetRelativeAxis(axis);
	}

public:
	inline void SetRelativeScale(const Vector3& scale)
	{
		m_RootComponent->SetRelativeScale(scale);
	}

	inline void SetRelativeScale(float x, float y, float z)
	{
		m_RootComponent->SetRelativeScale(x, y, z);
	}

	inline void SetRelativeRotation(const Vector3& rot)
	{
		m_RootComponent->SetRelativeRotation(rot);
	}

	inline void SetRelativeRotation(float x, float y, float z)
	{
		m_RootComponent->SetRelativeRotation(x, y, z);
	}

	inline void SetRelativeRotationX(float x)
	{
		m_RootComponent->SetRelativeRotationX(x);
	}

	inline void SetRelativeRotationY(float y)
	{
		m_RootComponent->SetRelativeRotationY(y);
	}

	inline void SetRelativeRotationZ(float z)
	{
		m_RootComponent->SetRelativeRotationZ(z);
	}

	inline void SetRelativePos(const Vector3& pos)
	{
		m_RootComponent->SetRelativePos(pos);
	}

	inline void SetRelativePos(float x, float y, float z)
	{
		m_RootComponent->SetRelativePos(x, y, z);
	}

	inline void AddRelativeScale(const Vector3& scale)
	{
		m_RootComponent->AddRelativeScale(scale);
	}

	inline void AddRelativeScale(float x, float y, float z)
	{
		m_RootComponent->AddRelativeScale(x, y, z);
	}

	inline void AddRelativeRotation(const Vector3& rot)
	{
		m_RootComponent->AddRelativeRotation(rot);
	}

	inline void AddRelativeRotation(float x, float y, float z)
	{
		m_RootComponent->AddRelativeRotation(x, y, z);
	}

	inline void AddRelativeRotationX(float x)
	{
		m_RootComponent->AddRelativeRotationX(x);
	}

	inline void AddRelativeRotationY(float y)
	{
		m_RootComponent->AddRelativeRotationY(y);
	}

	inline void AddRelativeRotationZ(float z)
	{
		m_RootComponent->AddRelativeRotationZ(z);
	}

	inline void AddRelativePos(const Vector3& pos)
	{
		m_RootComponent->AddRelativePos(pos);
	}

	inline void AddRelativePos(float x, float y, float z)
	{
		m_RootComponent->AddRelativePos(x, y, z);
	}

public:
	inline Vector3 GetWorldScale() const
	{
		return m_RootComponent->GetWorldScale();
	}

	inline Vector3 GetWorldRot() const
	{
		return m_RootComponent->GetWorldRot();
	}

	inline Vector3 GetWorldPos() const
	{
		return m_RootComponent->GetWorldPos();
	}

	inline Vector3 GetPivot() const
	{
		return m_RootComponent->GetPivot();
	}

	inline Vector3 GetMeshSize() const
	{
		return m_RootComponent->GetMeshSize();
	}

	inline const Matrix& GetWorldMatrix() const
	{
		return m_RootComponent->GetWorldMatrix();
	}

	inline Vector3 GetWorldAxis(AXIS axis)
	{
		return m_RootComponent->GetWorldAxis(axis);
	}

public:
	inline void SetPivot(const Vector3& pivot)
	{
		m_RootComponent->SetPivot(pivot);
	}

	inline void SetPivot(float x, float y, float z)
	{
		m_RootComponent->SetPivot(x, y, z);
	}

	inline void SetMeshSize(const Vector3& size)
	{
		m_RootComponent->SetMeshSize(size);
	}

	inline void SetMeshSize(float x, float y, float z)
	{
		m_RootComponent->SetMeshSize(x, y, z);
	}


public:
	inline void SetWorldScale(const Vector3& scale)
	{
		m_RootComponent->SetWorldScale(scale);
	}

	inline void SetWorldScale(float x, float y, float z)
	{
		m_RootComponent->SetWorldScale(x, y, z);
	}

	inline void SetWorldRotation(const Vector3& rot)
	{
		m_RootComponent->SetWorldRotation(rot);
	}

	inline void SetWorldRotation(float x, float y, float z)
	{
		m_RootComponent->SetWorldRotation(x, y, z);
	}

	inline void SetWorldRotationX(float x)
	{
		m_RootComponent->SetWorldRotationX(x);
	}

	inline void SetWorldRotationY(float y)
	{
		m_RootComponent->SetWorldRotationY(y);
	}

	inline void SetWorldRotationZ(float z)
	{
		m_RootComponent->SetWorldRotationZ(z);
	}

	inline void SetWorldPos(const Vector3& pos)
	{
		m_RootComponent->SetWorldPos(pos);
	}

	inline void SetWorldPos(float x, float y, float z)
	{
		m_RootComponent->SetWorldPos(x, y, z);
	}

	inline void AddWorldScale(const Vector3& scale)
	{
		m_RootComponent->AddWorldScale(scale);
	}

	inline void AddWorldScale(float x, float y, float z)
	{
		m_RootComponent->AddWorldScale(x, y, z);
	}

	inline void AddWorldRotation(const Vector3& rot)
	{
		m_RootComponent->AddWorldRotation(rot);
	}

	inline void AddWorldRotation(float x, float y, float z)
	{
		m_RootComponent->AddWorldRotation(x, y, z);
	}

	inline void AddWorldRotationX(float x)
	{
		m_RootComponent->AddWorldRotationX(x);
	}

	inline void AddWorldRotationY(float y)
	{
		m_RootComponent->AddWorldRotationY(y);
	}

	inline void AddWorldRotationZ(float z)
	{
		m_RootComponent->AddWorldRotationZ(z);
	}

	inline void AddWorldPos(const Vector3& pos)
	{
		m_RootComponent->AddWorldPos(pos);
	}

	inline void AddWorldPos(float x, float y, float z)
	{
		m_RootComponent->AddWorldPos(x, y, z);
	}
};

