#pragma once

// ���� ���
const float FRICTION_LINEAR_ACELERATION = -0.04f;
// �극��ũ�� ���Ӽӵ�
const float BREAK_ACELERATION = -0.2f;
// ����Ÿ�Ͽ��� ���� �ӵ�
const float SLOW_LAND_LINEAR_ACELERATION = -0.2f;
// ��ŷ������ ���ʽ� �ӵ�
const float POSITION_ACCELERATION_BONUS_PCT = 0.0085f;

/// <summary>
/// īƮ(����̹�)�� Ÿ��
/// </summary>
class CKartProperties
{
public:
	// ����Ÿ��
	static CKartProperties BALACNED;

	// ����Ÿ��
	static CKartProperties ACCELERATION;

	// ����Ÿ��
	static CKartProperties HEAVY;

	// �ڵ鸵Ÿ��
	static CKartProperties HANDLING;

	// �������ʴ� ����̹�(��Ʈ�ο� ���̽��� �������� �ʿ���)
	static CKartProperties INVISIBLE;

	// �÷��̾� �ӵ� ���
	static float PLAYER_CHARACTER_MULTIPLIER;
	
	// ���� ���ӵ�
	float m_MotorAcceleration;

	// �ִ� �����ӵ�
	float m_MaxNormalLinearSpeed;
	// �ִ� ���� ���ӵ�
	float m_MaxSpeedUpLinearSpeed;
	// �ִ� ���ӵ�
	float m_MaxSpeedDownLinearSpeed;

	// ȸ�� ���ӵ�
	float m_TurningAcceleration;
	// �ִ� ȸ�� ���ӵ�
	float m_MaxTurningAngularSpeed;

	// ����Ÿ�Ͽ����� �ִ�ӵ�
	float m_SlowTileMaxLinearSpeed;

	// ����
	float m_Weight;

	// ��������
	bool m_Convex;

	// ��ġ ����
	static void SetScaleFactor(const float scaleFactor, const float playerCharacterMultiplier);

	// �÷��̾� ��� ����
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

