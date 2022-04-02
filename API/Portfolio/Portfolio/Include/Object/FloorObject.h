#pragma once

#include "../GameInfo.h"
#include "../Ref.h"
#include "../Map/MapInfo.h"

#include "Driver.h"

// �ٴ� ������Ʈ�� ��� �簢�� vs �� �浹�� �浹�� üũ�Ѵ�.
struct sHitBox
{
	Vector2 leftTop;
	Vector2 rightBottom;
};

/// <summary>
/// ������ �ڽ�, ����, �⸧ �� �ٴ� ������Ʈ�� ����̵Ǵ� Ŭ����
/// </summary>
class CFloorObject :
	public CRef
{
protected:
	// ���� ������
	Vector2 m_TopLeftPixel;

	// ũ��
	Vector2 m_Size;

	// �浹ü�� ũ��
	sHitBox m_Hitbox;

	// ���� �ٴ� ������Ʈ�� ����
	EFloorObjectOrientation m_Orientation;

	// ����
	EFloorObjectState m_State;

	// Ÿ��
	EFloorObjectType m_Type;

	// ���� ��ȭ ť�� �ٴڿ�����Ʈ�� ��ȣ�ۿ��ϴ� ����
	// �÷��̾�Ӹ� �ƴ϶� cpu�� �����ϴ�.
	// ���� �ż��� ��ȭ�� ���涧���� �� ť�� ����ְ�
	// ���̽� ���߿� ť�� ������� �ʴٸ� ��ü�� ��ȸ�ؼ� ��ȭ�� �����Ѵ�.
	static std::vector<CSharedPtr<CFloorObject>> vecChangesQueue;

	// �ʱ� ����
	virtual EFloorObjectState GetInitState() const = 0;

	// vecChangesQueue�� �̿��ϱ� ���ؼ� ���� ���� �� ����
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

