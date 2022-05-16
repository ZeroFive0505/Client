#pragma once

#include "../GameInfo.h"


/// <summary>
/// 트랜스폼 컴포넌트는 씬 컴포넌트의
/// 위치 정보를 표현하는 컴포넌트이다.
/// 씬 컴포넌트는 기본적으로 트랜스폼 컴포넌트를 내장하고있다.
/// </summary>
class CTransform
{
	friend class CSceneComponent;

private:
	CTransform();
	CTransform(const CTransform& transform);
	~CTransform();

private:
	// 어떤 씬에 소속되어있는지
	class CScene* m_Scene;
	// 어떤 오브젝트에 소속되어있는지
	class CGameObject* m_Object;
	// 어떤 씬 컴포넌트에 소속되어있는지
	class CSceneComponent* m_Owner;

	Transform_State m_State;
	class CSkeletonSocket* m_Socket;

private:
	// 트랜스폼은 계층 구조로 이뤄져있다.
	// 따라서 부모 노드가 존재
	CTransform* m_Parent;
	std::vector<CTransform*> m_vecChild;

	// 트랜스폼은 월드 행렬 연산을 위해서 상수버퍼를 가지고있다.
	class CTransformConstantBuffer* m_CBuffer;


private:
	bool m_InheritScale;
	bool m_InheritRotX;
	bool m_InheritRotY;
	bool m_InheritRotZ;
	bool m_InheritParentRotationPosX;
	bool m_InheritParentRotationPosY;
	bool m_InheritParentRotationPosZ;
	bool m_UpdateScale;
	bool m_UpdateRot;
	bool m_UpdatePos;

public:
	inline void SetSocket(class CSkeletonSocket* socket)
	{
		m_Socket = socket;
	}

	inline void SetInheritScale(bool inherit)
	{
		m_InheritScale = inherit;
	}

	inline void SetInheritRotX(bool inherit)
	{
		m_InheritRotX = inherit;
	}

	inline void SetInheritRotY(bool inherit)
	{
		m_InheritRotY = inherit;
	}

	inline void SetInheritRotZ(bool inherit)
	{
		m_InheritRotZ = inherit;
	}

	inline void SetInheritParentRotationPosX(bool inherit)
	{
		m_InheritParentRotationPosX = inherit;
	}

	inline void SetInheritParentRotationPosY(bool inherit)
	{
		m_InheritParentRotationPosY = inherit;
	}

	inline void SetInheritParentRotationPosZ(bool inherit)
	{
		m_InheritParentRotationPosZ = inherit;
	}

	inline void SetTransformState(Transform_State state)
	{
		m_State = state;
	}

	void InheritScale(bool currnet);
	void InheritRotation(bool current);
	void InheritParentRotationPos(bool current);

	void InheritWorldScale(bool currnet);
	void InheritWorldRotation(bool current);
	void InheritParentRotationWolrdPos(bool currnet);

private: // Relative
	Vector3 m_RelativeScale;
	Vector3 m_RelativeRot;
	Vector3 m_RelativePos;
	Vector3 m_RelativeAxis[AXIS_MAX];

public:
	inline Vector3 GetRelativeScale() const
	{
		return m_RelativeScale;
	}

	inline Vector3 GetRelativeRot() const
	{
		return m_RelativeRot;
	}

	inline Vector3 GetRelativePos() const
	{
		return m_RelativePos;
	}

	inline Vector3 GetRelativeAxis(AXIS axis) const
	{
		return m_RelativeAxis[axis];
	}

public:
	void SetRelativeScale(const Vector3& scale);
	void SetRelativeScale(float x, float y, float z);
	void MulRelativeScale(float x, float y, float z);

	void SetRelativeRotation(const Vector3& rot);
	void SetRelativeRotation(float x, float y, float z);
	void SetRelativeRotationX(float x);
	void SetRelativeRotationY(float y);
	void SetRelativeRotationZ(float z);

	void SetRelativePos(const Vector3& pos);
	void SetRelativePos(float x, float y, float z);

	void AddRelativeScale(const Vector3& scale);
	void AddRelativeScale(float x, float y, float z);

	void AddRelativeRotation(const Vector3& rot);
	void AddRelativeRotation(float x, float y, float z);
	void AddRelativeRotationX(float x);
	void AddRelativeRotationY(float y);
	void AddRelativeRotationZ(float z);

	void AddRelativePos(const Vector3& pos);
	void AddRelativePos(float x, float y, float z);
	
private: // World
	Vector3 m_WorldScale;
	Vector3 m_WorldRot;
	Vector3 m_WorldPos;
	Vector3 m_WorldAxis[AXIS_MAX];

	Vector3 m_Pivot;
	Vector3 m_MeshSize;

	Matrix m_matScale;
	Matrix m_matRot;
	Matrix m_matPos;
	Matrix m_matWorld;
	
public:
	inline Vector3 GetWorldScale() const
	{
		return m_WorldScale;
	}

	inline Vector3 GetWorldRot() const
	{
		return m_WorldRot;
	}

	inline Vector3 GetWorldPos() const
	{
		return m_WorldPos;
	}

	inline Vector3 GetPivot() const
	{
		return m_Pivot;
	}

	inline Vector3 GetMeshSize() const
	{
		return m_MeshSize;
	}

	inline Vector3 GetWorldAxis(AXIS axis) const
	{
		return m_WorldAxis[axis];
	}

	inline const Matrix& GetWorldMatrix() const
	{
		return m_matWorld;
	}
	
public:
	inline void SetPivot(const Vector3& pivot)
	{
		m_Pivot = pivot;
	}

	inline void SetPivot(float x, float y, float z)
	{
		m_Pivot = Vector3(x, y, z);
	}

	inline void SetMeshSize(const Vector3& size)
	{
		m_MeshSize = size;
	}

	inline void SetMeshSize(float x, float y, float z)
	{
		m_MeshSize = Vector3(x, y, z);
	}
	
public:
	void SetWorldScale(const Vector3& scale);
	void SetWorldScale(float x, float y, float z);
	void MulWorldScale(float x, float y, float z);

	void SetWorldRotation(const Vector3& rot);
	void SetWorldRotation(float x, float y, float z);
	void SetWorldRotationX(float x);
	void SetWorldRotationY(float y);
	void SetWorldRotationZ(float z);

	void SetWorldPos(const Vector3& pos);
	void SetWorldPos(float x, float y, float z);

	void AddWorldScale(const Vector3& scale);
	void AddWorldScale(float x, float y, float z);

	void AddWorldRotation(const Vector3& rot);
	void AddWorldRotation(float x, float y, float z);
	void AddWorldRotationX(float x);
	void AddWorldRotationY(float y);
	void AddWorldRotationZ(float z);

	void AddWorldPos(const Vector3& pos);
	void AddWorldPos(float x, float y, float z);

public:
	void Start();
	void Init();
	void Update(float deltaTime);
	void PostUpdate(float deltaTime);
	void SetTransform();
	void ComputeWorld();
	CTransform* Clone();
	void Save(FILE* pFile);
	void Load(FILE* pFile);
};

