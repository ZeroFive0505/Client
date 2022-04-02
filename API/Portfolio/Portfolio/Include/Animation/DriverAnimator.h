#pragma once

#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Object/ObjectInfo.h"

// 360���� 32���� �ؽ��Ŀ� ��Ī
const int NUM_360_TEXTURES = 32;

// �帮��Ʈ�� ��鸮�� ����
const float MOVEMENT_DRIFT_AMPLITUDE = 2.5f;

// ������ ��鸮�� ����
const float MOVEMENT_SPEED_AMPLITUDE = 0.4f;

// �帮��Ʈ �ﰢ�Լ� �ֱ�
const float MOVEMENT_DRIFT_PERIOD = 20.0f;

// ������ �ﰢ�Լ� �ֱ�
const float MOVEMENT_SPEED_PERIOD = 0.015f;

class CDriverAnimator
{
private:
	enum class EPlayerState
	{
		GO_RIGHT,
		GO_LEFT,
		GO_FORWARD,
		GO_BACK,
		FALLING,
		HIT,
		JUMP
	};

	// ���� ����̹��� ��Ʈ�� Ÿ��
	EDriverControlType m_ControlType;

	// �Ҽӵ� ��
	class CRaceScene* m_Scene;

	// ���� ����̹� �ִϸ����� ����
	EPlayerState m_State;
	
	// ��Ÿ ������ �ð�
	float m_StarTime;

	float m_Scale;
	float m_FallScale = 1.0f;

	// ���� �ؽ����� �ε���
	int m_TexIdx = 0;

	// 360�� �ѹ��� �ؽ��� �ε���
	int m_360TexIdx[NUM_360_TEXTURES] = {
		  0,  1,  2,  3, 4, 5, 6, 7, 7, 8, 8, 9, 9, 10, 10, 11,
		  11, 10, 10, 9, 9, 8, 8, 7, 7, 6, 5, 4, 3, 2,  1,  0
	};


	int m_DriftIndex = 0;
	int m_GrassIndex = 0;


	float m_SpriteMovementDriftTime = 0.0f;
	float m_SpriteMovementSpeedTime = 0.0f;

private:
	CSharedPtr<CTexture> m_RDriving[12];
	CSharedPtr<CTexture> m_LDriving[12];
	CSharedPtr<CTexture> m_LDriftTxtParticles[3], m_RDriftTxtParticles[3], m_GrassTxtParticles[4];
	CSharedPtr<CTexture> m_SwampTxtParticles[2];
	CSharedPtr<CTexture> m_CurrentTexture, m_DriftParticles[5], m_GrassParticle;

	float m_SmashTime;
	float m_SmallTime;

	bool m_Drifting = false;

	bool m_OnSwamp = false;

	float m_SpriteMovementDrift = 0.0f;
	float m_SpriteMovementSpeed = 0.0f;

public:
	inline float GetSpriteDriftMovement() const
	{
		return m_SpriteMovementDrift;
	}

	inline float GetSpriteMovementSpeed() const
	{
		return m_SpriteMovementSpeed;
	}

	inline float GetSmallTime() const
	{
		return m_SmallTime;
	}

	inline float GetSmashTime() const
	{
		return m_SmashTime;
	}

	inline void SetControlType(EDriverControlType type)
	{
		m_ControlType = type;
	}

public:
	CDriverAnimator(const std::string& textureName, const EDriverControlType& control, class CRaceScene* scene, bool IsInvisible = false);
	CDriverAnimator(const std::string& textureName, class CRaceScene* scene, bool IsInvisible = false)
		: CDriverAnimator(textureName, EDriverControlType::DISABLED, scene) {};
	CDriverAnimator() {};
	virtual ~CDriverAnimator();

	void Init(const std::string& name, class CRaceScene* scene);

	void GoForward();
	void GoRight(bool drift = false);
	void GoLeft(bool drift = false);
	void Fall();
	void Feather();
	void Hit();

	void Small(float duration);
	void Smash(float duration);
	void Star(float duration);

	void DrawParticles(HDC hDC, CTexture* driverTexture, bool IsSmall, Vector2 position);

	void Update(const float speedForward, const float speedTurn, const float height, const float deltaTime, const Vector2& pos);

	bool CanDrive() const;

	void SetViewTexture(float angle);

	CTexture GetMinimapTexture(float angle, const float screenScale) const;

	void Reset();

	inline CTexture* GetTexture() const
	{
		return m_CurrentTexture;
	}

	inline CTexture* GetFrontFaceTexture()
	{
		return m_RDriving[11];
	}

	inline bool IsDrifting() const
	{
		return m_Drifting;
	}
};

