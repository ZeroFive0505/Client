#pragma once

#include "../GameInfo.h"
#include "../Map/MapInfo.h"
#include "ObjectInfo.h"
#include "KartProperties.h"
#include "WallObject.h"
#include "../Animation/DriverAnimator.h"
#include "../Input.h"


/// <summary>
/// 현재 드라이버의 상태를 표시하는 열거체
/// 이진수기반으로 왼쪽부터 1비트씩 증가할 경우를 그대로
/// 표현하고있다. 64의 경우 2^7 = 64 따라서 MAX의 경우 8이다.
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

// 드라이버의 충돌 범위
const float DRIVER_HITBOX_RADIUS = 1.5f;


// 드라이버는 일종의 벽이다. (움직이는)
class CDriver : 
	public CWallObject
{
private:
	// 가속 시간
	const float SPEED_UP_DURATION = 1.5f;

	// 가속 + 가속 시간
	const float MORE_SPEED_UP_DURATION = 0.75f;

	// 감속 시간(아이템이나 상태로 인한)
	const float SPEED_DOWN_DURATION = 10.0f;

	// 스타 지속 시간
	const float STAR_DURATION = 10.0f;

	// 아이템 사용을 위한 대기 시간
	const float ITEM_USE_WAIT = 2.0f;

	// 통제 불능 시간
	const float UNCONTROLLED_DURATION = 1.0f;
	
	// 최대 회피 각도
	const float MAX_EVADE_ANGLE = (float)M_PI / 3.0f;

	// 최대 회피 거리
	const float MAX_EVADE_DISTANCE = 1.5f / MAP_TILES_WIDTH;

	// 코인의 갯수에따른 속도 변화
	const float COIN_SPEED = 0.002f;
	
	// 아이템 사용 대기 시간
	const float ITME_READY = 2.0f;

	// 지나온 경로 업데이트 간격
	const float FOLLWED_PATH_UPDATE_INTERVAL = 0.25f;

	// 위치 재설정시
	const int STEPS_BACK_FOR_RELOCATION = 4;
	const int STPES_STILL_FOR_RELOCATION = 12;

	// 현재 플레이어
	EMenuPlayer m_CurrentPlayer;

	// 현재 랩
	int m_Laps = 0;
	int m_MaxLapSoFar = 0;


	// 마지막 변화량
	int m_LastGradient;
	int m_ConsecutiveGradientIncrement;

	float m_SpinOver;

	// 현재 코인의 수
	int m_Coins;

	
	EItem m_CurrentItem = EItem::NONE;

	float m_CanUseAt = 0;


	const float SPINOVERTIME = 1.0f;

	// 지금까지 지나온 경로
	std::vector<Vector2> m_FollwedPath;
	std::vector<int> m_PrevLab;
	std::vector<int> m_IndexOfLap;
	float m_PathLastUpdatedAt;

	int m_CurrentState = (int)EDriverState::NORMAL;

	// 배열을 이용해서 모든 상태와 상태의 시간을 기록한다.
	float m_StateEnd[(int)EDriverState::MAX] = { 0.0f };

	ECCOption m_CCoption;

public:
	virtual void Start() override;
	virtual void Init() override;
	virtual void Update(float deltaTime) override;

public:
	// 상태를 추가한다.
	void PushStateEnd(EDriverState state, const float endTime);

	// 매 업데이트 루프시마다 호출하여 끝난 상태는 제거한다.
	void PopStateEnd(EDriverState state);

	inline void SetCCOption(ECCOption option)
	{
		m_CCoption = option;
	}

private:

	// 끝난 상태를 제거와 동시 기록하여 반환한다.
	int PopStateEnd(const float currentTime);

	// 유저 컨트롤
	void PlayerControl(float& accelerationLinear);

	// AI 컨트롤
	void UseGradientControl(float& accelerationLinear);

	// 변화량을 기록한다.
	void UpdateGradientPosition();

public:
	// 드라이버의 애니메이터
	CDriverAnimator m_Animator;

	// 현재 각도
	float m_Angle;
	
	// 공중에서의 각도
	float m_FlightAngle;

	// 전진속도와, 회전 속도
	float m_SpeedForward, m_SpeedTurn;

	// 상승 속도
	float m_SpeedUpwards;

	// 김수한무의 픽업을 받고 있는지
	bool m_OnLakitu = false;

	// 현재 떨어지고 있는 상태인지
	bool m_Falling = false;

	// 드리프트 상태인지
	bool m_DriftPressed = false;

	// 현재 언덕으로 인한 점프 중인지
	bool m_JumpByRamp = false;

	// 깃털로 인한 점프
	bool m_JumpByFeather;

	// 현재 트랙에 따라 잘 주행하고 있는지
	bool m_GoingForward = true;

	// 충돌시에 사용되는 속도와 벡터
	Vector2 m_VectorialSpeed;
	Vector2 m_CollisionMomentum;


	EDriverControlType m_ControlType;
	const CKartProperties* m_Kart;
	const std::array<CDriver*, (int)EMenuPlayer::END>& m_Positions;

	bool m_IsRealPlayer;

	int m_Rank;

	// AI 설정 요소 값들
	int m_FarVisionModifier;
	float m_ItemProbModifier = 1;
	unsigned int m_ImpedimentModifier = 0;

	// 드라이버 소멸시 랭킹 배열에 있는 값도 소멸시켜야함
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
	// 버섯 사용
	void ApplyMushroom();

	// 스타 사용
	void ApplyStar();

	// 번개 사용
	void ApplyThunder(float duration);

	// 깃털 사용
	void ApplyFeather();
	
public:
	// 점프
	void ShortJump();

	// 언덕길로 인한 점프
	void JumpByRamp(const ETileProperty& land);

	// 피격
	void ApplyHit();

	// 도로가 아닌 곳에서 과하게 드리프트시에 스핀오버
	void ApplySpinOver();

	// 스매쉬 (쿵쿵이)
	void ApplySmash();

	// 코인 추가
	void AddCoin(int amount = 1);


	// 현재 코인 반환
	inline int GetCoins() const
	{
		return m_Coins;
	}

	// 현재 랩 반환
	inline int GetLaps() const
	{
		return m_Laps;
	}

	// 마지막 변화량 반환
	inline int GetLastGradient() const
	{
		return m_LastGradient;
	}

	// 현재 플레이어 반환
	inline EMenuPlayer GetPlayer() const
	{
		return m_CurrentPlayer;
	}


	// 지금 운전이 가능한 상태인지
	bool CanDrive() const;

	// 지금 무적인지 (스타)
	bool IsImmune() const;

	// 지금 보이는 상태인지(낙하시에 안 보임)
	bool IsVisible() const;

	// 아이템 사용이 가능한지
	bool CanUseItem() const;

	// 아이템 픽업
	void PickUpItem(EItem item);

	// 현재 아이템
	inline EItem GetItem() const
	{
		return m_CurrentItem;
	}

	// 현재 순위
	inline int GetRank() const
	{
		return m_Rank;
	}

	// 현재 각도
	inline float GetAngle() const
	{
		return m_Angle;
	}

	// 각도 설정
	inline void SetAngle(float angle)
	{
		m_Angle = angle;
	}

	// 컨트롤 타입 설정
	inline void SetControlType(EDriverControlType type)
	{
		m_ControlType = type;
		m_Animator.SetControlType(type);
	}

public:
	// 리셋
	void Reset();
	void EndRaceAndReset();
	void SetPositionAndReset(const Vector2& position, const float angle = M_PI_2 * -1.0f);

	// 현재 텍스쳐 반환
	virtual CTexture* GetTexture() override;

	// 플레이어를 그려낼때 호출 (플레이어만 AI제외)
	void Render(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables, const bool withShadow = true);

	// 백미러 작동시 플레이러의 앞모습을 그려낸다.
	void RenderFront(const float scale, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& drawables);

	// 충돌 처리
	bool SolveCollision(CollisionData& collisionData, const Vector2& otherSpeed, const Vector2& otherPos, const float otherWeight,
		const float otherHeight, const bool otherIsImmune, const float distance) override;

	// 위치 재조정
	void RelocateToNearestGoodPosition();
};

