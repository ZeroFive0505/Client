#pragma once

#include "../../GameInfo.h"

class CSkeletonSocket
{
	friend class CSkeleton;
	friend class CAnimationSequenceInstance;

private:
	CSkeletonSocket();
	CSkeletonSocket(const CSkeletonSocket& socket);
	~CSkeletonSocket();

private:
	std::string m_Name;
	std::string m_BoneName;
	int m_BoneIndex;
	Vector3 m_Offset;
	Vector3 m_OffsetRot;
	Matrix m_matSocket;

public:
	inline const std::string& GetName() const
	{
		return m_Name;
	}

	inline const Matrix& GetSocketMatrix() const
	{
		return m_matSocket;
	}

public:
	inline void SetOffset(float x, float y, float z)
	{
		m_Offset = Vector3(x, y, z);
	}

	inline void SetOffset(const Vector3& offset)
	{
		m_Offset = offset;
	}

	inline void SetOffsetRotation(float x, float y, float z)
	{
		m_OffsetRot = Vector3(x, y, z);
	}

	inline void SetOffsetRotation(const Vector3& offset)
	{
		m_OffsetRot = offset;
	}

public:
	void Update(const Matrix& matBone);
	CSkeletonSocket* Clone();
};

