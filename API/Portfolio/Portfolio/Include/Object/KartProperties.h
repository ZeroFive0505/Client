#pragma once

// 마찰 계수
const float FRICTION_LINEAR_ACELERATION = -0.04f;
// 브레이크시 감속속도
const float BREAK_ACELERATION = -0.2f;
// 감속타일에서 감속 속도
const float SLOW_LAND_LINEAR_ACELERATION = -0.2f;
// 랭킹에따른 보너스 속도
const float POSITION_ACCELERATION_BONUS_PCT = 0.0085f;

/// <summary>
/// 카트(드라이버)의 타입
/// </summary>
class CKartProperties
{
public:
	// 균형타입
	static CKartProperties BALACNED;

	// 가속타입
	static CKartProperties ACCELERATION;

	// 무게타입
	static CKartProperties HEAVY;

	// 핸들링타입
	static CKartProperties HANDLING;

	// 보이지않는 드라이버(인트로와 레이스가 끝났을시 필요함)
	static CKartProperties INVISIBLE;

	// 플레이어 속도 배수
	static float PLAYER_CHARACTER_MULTIPLIER;
	
	// 모터 가속도
	float m_MotorAcceleration;

	// 최대 선형속도
	float m_MaxNormalLinearSpeed;
	// 최대 선형 가속도
	float m_MaxSpeedUpLinearSpeed;
	// 최대 감속도
	float m_MaxSpeedDownLinearSpeed;

	// 회전 가속도
	float m_TurningAcceleration;
	// 최대 회전 가속도
	float m_MaxTurningAngularSpeed;

	// 감속타일에서의 최대속도
	float m_SlowTileMaxLinearSpeed;

	// 무게
	float m_Weight;

	// 공기저항
	bool m_Convex;

	// 수치 조정
	static void SetScaleFactor(const float scaleFactor, const float playerCharacterMultiplier);

	// 플레이어 배수 설정
	const CKartProperties& MakePlayer() const;

private:
	static CKartProperties PLAYER;

	CKartProperties(const float motorAcceleration,
		const float turningAcceleration,
		const float maxNormalLinearSpeed,
		const float weight,
		const bool convex)
		: m_MotorAcceleration(motorAcceleration),
		m_MaxNormalLinearSpeed(maxNormalLinearSpeed),
		m_MaxSpeedUpLinearSpeed(maxNormalLinearSpeed * 2.0f),
		m_MaxSpeedDownLinearSpeed(maxNormalLinearSpeed * 0.85f),
		m_TurningAcceleration(turningAcceleration),
		m_MaxTurningAngularSpeed(turningAcceleration * 10.0f),
		m_SlowTileMaxLinearSpeed(maxNormalLinearSpeed * 0.45f),
		m_Weight(weight),
		m_Convex(convex)
	{}
};

