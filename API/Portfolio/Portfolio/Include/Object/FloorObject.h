#pragma once

#include "../GameInfo.h"
#include "../Ref.h"
#include "../Map/MapInfo.h"

#include "Driver.h"

// 바닥 오브젝트의 경우 사각형 vs 점 충돌로 충돌을 체크한다.
struct sHitBox
{
	Vector2 leftTop;
	Vector2 rightBottom;
};

/// <summary>
/// 아이템 박스, 코인, 기름 등 바닥 오브젝트의 기반이되는 클래스
/// </summary>
class CFloorObject :
	public CRef
{
protected:
	// 왼쪽 꼭지점
	Vector2 m_TopLeftPixel;

	// 크기
	Vector2 m_Size;

	// 충돌체의 크기
	sHitBox m_Hitbox;

	// 현재 바닥 오브젝트의 방향
	EFloorObjectOrientation m_Orientation;

	// 상태
	EFloorObjectState m_State;

	// 타입
	EFloorObjectType m_Type;

	// 정적 변화 큐로 바닥오브젝트와 상호작용하는 것은
	// 플레이어뿐만 아니라 cpu도 가능하다.
	// 따라서 매순간 변화가 생길때마다 이 큐에 집어넣고
	// 레이스 도중에 큐가 비어있지 않다면 전체를 순회해서 변화를 적용한다.
	static std::vector<CSharedPtr<CFloorObject>> vecChangesQueue;

	// 초기 상태
	virtual EFloorObjectState GetInitState() const = 0;

	// vecChangesQueue를 이용하기 위해서 쓰는 정적 씬 변수
	static class CRaceScene* m_StaticScene;

public:
	inline Vector2 GetPos() const
	{
		return m_TopLeftPixel;
	}

	inline Vector2 GetSize() const
	{
		return m_Size;
	}

	inline EFloorObjectType GetType() const
	{
		return m_Type;
	}

	static void SetScene(class CRaceScene* scene)
	{
		m_StaticScene = scene;
	}

public:
	CFloorObject(const Vector2& pos, const Vector2& size, EFloorObjectType type, const int mapWidth, const int mapHeight, const EFloorObjectOrientation orientation, class CRaceScene* scene);

	virtual ~CFloorObject();

	EFloorObjectState GetState() const;

	void SetState(EFloorObjectState state);

	virtual void ApplyChanges() const = 0;

	virtual void Update(float deltaTime);

	static void DefaultApplyChanges(const CFloorObject* that);

	bool CollidesWith(const CSharedPtr<CDriver>& driver) const;
	virtual void InteractWith(const CSharedPtr<CDriver>& driver, float currentTime) = 0;

	virtual CTexture* GetCurrentTexture() const = 0;

	virtual ETileProperty GetCurrentTile() const = 0;

	static bool ApplyAllChanges();

	static void ResetChanges();
};

