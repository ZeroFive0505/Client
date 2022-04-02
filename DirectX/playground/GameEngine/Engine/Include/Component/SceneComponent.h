#pragma once

#include "Component.h"
#include "Transform.h"

/// <summary>
/// ���� �������� ������Ʈ���̴�.
/// �޽� ������Ʈ, ��������Ʈ ������Ʈ�� �̿� ���Ѵ�.
/// </summary>
class CSceneComponent :
	public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();

protected:
	// �� ������Ʈ�� �⺻������ Ʈ������ ������Ʈ�� ������ �ִ´�.
	CTransform* m_Transform;
	// �θ� ������Ʈ
	CSceneComponent* m_Parent;
	// ���� ������Ʈ�� ������ �ִ� ��� �ڽ� ������Ʈ
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
	// ������ ����
	bool m_Render;
	// � ������ ���̾��� �����ִ���
	std::string m_LayerName;

public:
	inline std::string GetLayerName() const
	{
		return m_LayerName;
	}

	inline bool InRender() const
	{
		return m_Render;
	}

public:
	inline void SetLayerName(const std::string& name)
	{
		m_LayerName = name;
	}

public:
	void SetSceneComponent(class CGameObject* obj);
	void GetAllSceneComponentsName(std::vector<sFindComponentName>& vecNames);
	virtual void Destroy();

public:
	virtual void SetScene(class CScene* scene);
	virtual void SetGameObject(class CGameObject* obj);

public:
	void AddChild(CSceneComponent* child);
	bool DeleteChild(CSceneComponent* child);
	bool DeleteChild(const std::string& name);
	CSceneComponent* FindComponent(const std::string& name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float deltaTime);
	virtual void PostUpdate(float deltaTime);
	virtual void CheckCollision();
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CSceneComponent* Clone();
	virtual void Save(FILE* pFile);
	virtual void Load(FILE* pFile);

	
public: // �� ������Ʈ�� Ʈ�������� �����ؼ� �����ϴ� �Լ���
	inline void SetInheritScale(bool inherit)
	{
		m_Transform->SetInheritScale(inherit);
	}

	inline void SetInheritRotX(bool inherit)
	{
		m_Transform->SetInheritRotX(inherit);
	}

	inline void SetInheritRotY(bool inherit)
	{
		m_Transform->SetInheritRotY(inherit);
	}

	inline void SetInheritRotZ(bool inherit)
	{
		m_Transform->SetInheritRotZ(inherit);
	}

	inline void SetInheritParentRotationPosX(bool inherit)
	{
		m_Transform->SetInheritParentRotationPosX(inherit);
	}

	inline void SetInheritParentRotationPosY(bool inherit)
	{
		m_Transform->SetInheritParentRotationPosY(inherit);
	}

	inline void SetInheritParentRotationPosZ(bool inherit)
	{
		m_Transform->SetInheritParentRotationPosZ(inherit);
	}

	inline void InheritScale(bool current)
	{
		m_Transform->InheritScale(current);
	}

	inline void InheritRotation(bool current)
	{
		m_Transform->InheritRotation(current);
	}

	inline void InheritParentRotationPos(bool current)
	{
		m_Transform->InheritParentRotationPos(current);
	}

	inline void InheritWorldScale(bool current)
	{
		m_Transform->InheritWorldScale(current);
	}

	inline void InheritWorldRotation(bool current)
	{
		m_Transform->InheritWorldRotation(current);
	}

	
public:
	inline Vector3 GetRelativeScale() const
	{
		return m_Transform->GetRelativeScale();
	}

	inline Vector3 GetRelativeRot() const
	{
		return m_Transform->GetRelativeRot();
	}

	inline Vector3 GetRelativePos() const
	{
		return m_Transform->GetRelativePos();
	}

	inline Vector3 GetRelativeAxis(AXIS axis) const
	{
		return m_Transform->GetRelativeAxis(axis);
	}
	
public:
	inline void SetRelativeScale(const Vector3& scale)
	{
		m_Transform->SetRelativeScale(scale);
	}

	inline void SetRelativeScale(float x, float y, float z)
	{
		m_Transform->SetRelativeScale(x, y, z);
	}

	inline void MulRelativeScale(float x, float y, float z)
	{
		m_Transform->MulRelativeScale(x, y, z);
	}

	inline void SetRelativeRotation(const Vector3& rot)
	{
		m_Transform->SetRelativeRotation(rot);
	}

	inline void SetRelativeRotation(float x, float y, float z)
	{
		m_Transform->SetRelativeRotation(x, y, z);
	}

	inline void SetRelativeRotationX(float x)
	{
		m_Transform->SetRelativeRotationX(x);
	}

	inline void SetRelativeRotationY(float y)
	{
		m_Transform->SetRelativeRotationY(y);
	}

	inline void SetRelativeRotationZ(float z)
	{
		m_Transform->SetRelativeRotationZ(z);
	}

	inline void SetRelativePos(const Vector3& pos)
	{
		m_Transform->SetRelativePos(pos);
	}

	inline void SetRelativePos(float x, float y, float z)
	{
		m_Transform->SetRelativePos(x, y, z);
	}

	inline void AddRelativeScale(const Vector3& scale)
	{
		m_Transform->AddRelativeScale(scale);
	}

	inline void AddRelativeScale(float x, float y, float z)
	{
		m_Transform->AddRelativeScale(x, y, z);
	}

	inline void AddRelativeRotation(const Vector3& rot)
	{
		m_Transform->AddRelativeRotation(rot);
	}

	inline void AddRelativeRotation(float x, float y, float z)
	{
		m_Transform->AddRelativeRotation(x, y, z);
	}

	inline void AddRelativeRotationX(float x)
	{
		m_Transform->AddRelativeRotationX(x);
	}

	inline void AddRelativeRotationY(float y)
	{
		m_Transform->AddRelativeRotationY(y);
	}

	inline void AddRelativeRotationZ(float z)
	{
		m_Transform->AddRelativeRotationZ(z);
	}

	inline void AddRelativePos(const Vector3& pos)
	{
		m_Transform->AddRelativePos(pos);
	}

	inline void AddRelativePos(float x, float y, float z)
	{
		m_Transform->AddRelativePos(x, y, z);
	}
	
public:
	inline Vector3 GetWorldScale() const
	{
		return m_Transform->GetWorldScale();
	}

	inline Vector3 GetWorldRot() const
	{
		return m_Transform->GetWorldRot();
	}

	inline Vector3 GetWorldPos() const
	{
		return m_Transform->GetWorldPos();
	}

	inline Vector3 GetPivot() const
	{
		return m_Transform->GetPivot();
	}

	inline Vector3 GetMeshSize() const
	{
		return m_Transform->GetMeshSize();
	}

	inline const Matrix& GetWorldMatrix() const
	{
		return m_Transform->GetWorldMatrix();
	}

	inline Vector3 GetWorldAxis(AXIS axis)
	{
		return m_Transform->GetWorldAxis(axis);
	}

public:
	inline void SetPivot(const Vector3& pivot)
	{
		m_Transform->SetPivot(pivot);
	}

	inline void SetPivot(float x, float y, float z)
	{
		m_Transform->SetPivot(x, y, z);
	}

	inline void SetMeshSize(const Vector3& size)
	{
		m_Transform->SetMeshSize(size);
	}

	inline void SetMeshSize(float x, float y, float z)
	{
		m_Transform->SetMeshSize(x, y, z);
	}

	
public:
	inline void SetWorldScale(const Vector3& scale)
	{
		m_Transform->SetWorldScale(scale);
	}

	inline void SetWorldScale(float x, float y, float z)
	{
		m_Transform->SetWorldScale(x, y, z);
	}

	inline void MulworldScale(float x, float y, float z)
	{
		m_Transform->MulWorldScale(x, y, z);
	}

	inline void SetWorldRotation(const Vector3& rot)
	{
		m_Transform->SetWorldRotation(rot);
	}

	inline void SetWorldRotation(float x, float y, float z)
	{
		m_Transform->SetWorldRotation(x, y, z);
	}

	inline void SetWorldRotationX(float x)
	{
		m_Transform->SetWorldRotationX(x);
	}

	inline void SetWorldRotationY(float y)
	{
		m_Transform->SetWorldRotationY(y);
	}

	inline void SetWorldRotationZ(float z)
	{
		m_Transform->SetWorldRotationZ(z);
	}

	inline void SetWorldPos(const Vector3& pos)
	{
		m_Transform->SetWorldPos(pos);
	}

	inline void SetWorldPos(float x, float y, float z)
	{
		m_Transform->SetWorldPos(x, y, z);
	}

	inline void AddWorldScale(const Vector3& scale)
	{
		m_Transform->AddWorldScale(scale);
	}

	inline void AddWorldScale(float x, float y, float z)
	{
		m_Transform->AddWorldScale(x, y, z);
	}

	inline void AddWorldRotation(const Vector3& rot)
	{
		m_Transform->AddWorldRotation(rot);
	}

	inline void AddWorldRotation(float x, float y, float z)
	{
		m_Transform->AddWorldRotation(x, y, z);
	}

	inline void AddWorldRotationX(float x)
	{
		m_Transform->AddWorldRotationX(x);
	}

	inline void AddWorldRotationY(float y)
	{
		m_Transform->AddWorldRotationY(y);
	}

	inline void AddWorldRotationZ(float z)
	{
		m_Transform->AddWorldRotationZ(z);
	}

	inline void AddWorldPos(const Vector3& pos)
	{
		m_Transform->AddWorldPos(pos);
	}

	inline void AddWorldPos(float x, float y, float z)
	{
		m_Transform->AddWorldPos(x, y, z);
	}
};

