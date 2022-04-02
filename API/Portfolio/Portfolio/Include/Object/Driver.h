#pragma once

#include "../GameInfo.h"
#include "../Map/MapInfo.h"
#include "ObjectInfo.h"
#include "KartProperties.h"
#include "WallObject.h"
#include "../Animation/DriverAnimator.h"
#include "../Input.h"


/// <summary>
/// ���� ����̹��� ���¸� ǥ���ϴ� ����ü
/// ������������� ���ʺ��� 1��Ʈ�� ������ ��츦 �״��
/// ǥ���ϰ��ִ�. 64�� ��� 2^7 = 64 ���� MAX�� ��� 8�̴�.
/// </summary>
enum class EDriverState : int
{
	NORMAL = 0,
	SPEED_UP = 1,
	MORE_SPEED_UP = 2,
	SPEED_DOWN = 4,
	STAR = 8,
	UNCONTROLLED = 16,
	STOPPED = 32,
	INVISIBLE = 64,
	MAX = 8
};

// ����̹��� �浹 ����
const float DRIVER_HITBOX_RADIUS = 1.5f;


// ����̹��� ������ ���̴�. (�����̴�)
class CDriver : 
	public CWallObject
{
private:
	// ���� �ð�
	const float SPEED_UP_DURATION = 1.5f;

	// ���� + ���� �ð�
	const float MORE_SPEED_UP_DURATION = 0.75f;

	// ���� �ð�(�������̳� ���·� ����)
	const float SPEED_DOWN_DURATION = 10.0f;

	// ��Ÿ ���� �ð�
	const float STAR_DURATION = 10.0f;

	// ������ ����� ���� ��� �ð�
	const float ITEM_USE_WAIT = 2.0f;

	// ���� �Ҵ� �ð�
	const float UNCONTROLLED_DURATION = 1.0f;
	
	// �ִ� ȸ�� ����
	const float MAX_EVADE_ANGLE = (float)M_PI / 3.0f;

	// �ִ� ȸ�� �Ÿ�
	const float MAX_EVADE_DISTANCE = 1.5f / MAP_TILES_WIDTH;

	// ������ ���������� �ӵ� ��ȭ
	const float COIN_SPEED = 0.002f;
	
	// ������ ��� ��� �ð�
	const float ITME_READY = 2.0f;

	// ������ ��� ������Ʈ ����
	const float FOLLWED_PATH_UPDATE_INTERVAL = 0.25f;

	// ��ġ �缳����
	const int STEPS_BACK_FOR_RELOCATION = 4;
	const int STPES_STILL_FOR_RELOCATION = 12;

	// ���� �÷��̾�
	EMenuPlayer m_CurrentPlayer;

	// ���� ��
	int m_Laps = 0;
	int m_MaxLapSoFar = 0;


	// ������ ��ȭ��
	int m_LastGradient;
	int m_ConsecutiveGradientIncrement;

	float m_SpinOver;

	// ���� ������ ��
	int m_Coins;

	
	EItem m_CurrentItem = EItem::NONE;

	float m_CanUseAt = 0;


	const float SPINOVERTIME = 1.0f;

	// ���ݱ��� ������ ���
	std::vector<Vector2> m_FollwedPath;
	std::vector<int> m_PrevLab;
	std::vector<int> m_IndexOfLap;
	float m_PathLastUpdatedAt;

	int m_CurrentState = (int)EDriverState::NORMAL;

	// �迭�� �̿��ؼ� ��� ���¿� ������ �ð��� ����Ѵ�.
	float m_StateEnd[(int)EDriverState::MAX] = { 0.0f };

	ECCOption m_CCoption;

public:
	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float deltaTime) override;

public:
	// ���¸� �߰��Ѵ�.
	void PushStateEnd(EDriverState state, const float endTime);

	// �� ������Ʈ �����ø��� ȣ���Ͽ� ���� ���´� �����Ѵ�.
	void PopStateEnd(EDriverState state);

	inline void SetCCOption(ECCOption option)
	{
		m_CCoption = option;
	}

private:

	// ���� ���¸� ���ſ� ���� ����Ͽ� ��ȯ�Ѵ�.
	int PopStateEnd(const float currentTime);

	// ���� ��Ʈ��
	void PlayerControl(float& accelerationLinear);

	// AI ��Ʈ��
	void UseGradientControl(float& accelerationLinear);

	// ��ȭ���� ����Ѵ�.
	void UpdateGradientPosition();

public:
	// ����̹��� �ִϸ�����
	CDriverAnimator m_Animator;

	// ���� ����
	float m_Angle;
	
	// ���߿����� ����
	float m_FlightAngle;

	// �����ӵ���, ȸ�� �ӵ�
	float m_SpeedForward, m_SpeedTurn;

	// ��� �ӵ�
	float m_SpeedUpwards;

	// ����ѹ��� �Ⱦ��� �ް� �ִ���
	bool m_OnLakitu = false;

	// ���� �������� �ִ� ��������
	bool m_Falling = false;

	// �帮��Ʈ ��������
	bool m_DriftPressed = false;

	// ���� ������� ���� ���� ������
	bool m_JumpByRamp = false;

	// ���з� ���� ����
	bool m_JumpByFeather;

	// ���� Ʈ���� ���� �� �����ϰ� �ִ���
	bool m_GoingForward = true;

	// �浹�ÿ� ���Ǵ� �ӵ��� ����
	Vector2 m_VectorialSpeed;
	Vector2 m_CollisionMomentum;


	EDriverControlType m_ControlType;
	const CKartProperties* m_Kart;
	const std::array<CDriver*, (int)EMenuPlayer::END>& m_Positions;

	bool m_IsRealPlayer;

	int m_Rank;

	// AI ���� ��� ����
	int m_FarVisionModifier;
	float m_ItemProbModifier = 1;
	unsigned int m_ImpedimentModifier = 0;

	// ����̹� �Ҹ�� ��ŷ �迭�� �ִ� ���� �Ҹ���Ѿ���
	bool m_DeletePosition;

	CDriver(const std::string& textureName, class CRaceScene* scene, const Vector2& position,
		const float angle, const int mapWidth, const int mapHeight,
		const EDriverControlType controlType,
		const CKartProperties& kart, const EMenuPlayer player,
		const std::array<CDriver*, (int)EMenuPlayer::END>& positions, bool IsRealPlayer = false,
		int farVisionMod = 0, float itemProbMod = 1,
		unsigned int impedimentMod = 8, bool IsInvisible = false);

	CDriver(const std::string& textureName, class CRaceScene* scene, const Vector2& position, const float angle,
		const int mapWidth, const int mapHeight, const EDriverControlType controlType, const CKartProperties& kart,
		const EMenuPlayer player, bool IsInvisible = false);

	virtual ~CDriver();

public:
	// ���� ���
	void ApplyMushroom();

	// ��Ÿ ���
	void ApplyStar();

	// ���� ���
	void ApplyThunder(float duration);

	// ���� ���
	void ApplyFeather();
	
public:
	// ����
	void ShortJump();

	// ������ ���� ����
	void JumpByRamp(const ETileProperty& land);

	// �ǰ�
	void ApplyHit();

	// ���ΰ� �ƴ� ������ ���ϰ� �帮��Ʈ�ÿ� ���ɿ���
	void ApplySpinOver();

	// ���Ž� (������)
	void ApplySmash();

	// ���� �߰�
	void AddCoin(int amount = 1);


	// ���� ���� ��ȯ
	inline int GetCoins() const
	{
		return m_Coins;
	}

	// ���� �� ��ȯ
	inline int GetLaps() const
	{
		return m_Laps;
	}

	// ������ ��ȭ�� ��ȯ
	inline int GetLastGradient() const
	{
		return m_LastGradient;
	}

	// ���� �÷��̾� ��ȯ
	inline EMenuPlayer GetPlayer() const
	{
		return m_CurrentPlayer;
	}


	// ���� ������ ������ ��������
	bool CanDrive() const;

	// ���� �������� (��Ÿ)
	bool IsImmune() const;

	// ���� ���̴� ��������(���Ͻÿ� �� ����)
	bool IsVisible() const;

	// ������ ����� ��������
	bool CanUseItem() const;

	// ������ �Ⱦ�
	void PickUpItem(EItem item);

	// ���� ������
	inline EItem GetItem() const
	{
		return m_CurrentItem;
	}

	// ���� ����
	inline int GetRank() const
	{
		return m_Rank;
	}

	// ���� ����
	inline float GetAngle() const
	{
		return m_Angle;
	}

	// ���� ����
	inline void SetAngle(float angle)
	{
		m_Angle = angle;
	}

	// ��Ʈ�� Ÿ�� ����
	inline void SetControlType(EDriverControlType type)
	{
		m_ControlType = type;
		m_Animator.SetControlType(type);
	}

public:
	// ����
	void Reset();
	void EndRaceAndReset();
	void SetPositionAndReset(const Vector2& position, const float angle = M_PI_2 * -1.0f);

	// ���� �ؽ��� ��ȯ
	virtual CTexture* GetTexture() override;

	// �÷��̾ �׷����� ȣ�� (�÷��̾ AI����)
	void Render(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables, const bool withShadow = true);

	// ��̷� �۵��� �÷��̷��� �ո���� �׷�����.
	void RenderFront(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables);

	// �浹 ó��
	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float otherWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

	// ��ġ ������
	void RelocateToNearestGoodPosition();
};

