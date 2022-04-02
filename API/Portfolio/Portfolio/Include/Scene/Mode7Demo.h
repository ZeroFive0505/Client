#pragma once

#include "Scene.h"
#include "../Resource/Texture.h"
#include "../Object/Player.h"

/// <summary>
/// 모드7 효과의 데모를 위한 씬
/// </summary>
class CMode7Demo :
	public CScene
{
	friend class CSceneManager;

private:
	CMode7Demo();
	virtual ~CMode7Demo();

protected:
	float MODE7_FOV_HALF = 0.5f;
	float MODE7_CLIP_NEAR = 0.0005f;
	float MODE7_CLIP_FAR = 0.045f;

	float CAM_TO_PLAYER_DIST = 46.0f;

private:
	CSharedPtr<CTexture> m_CircuitTexture;
	CSharedPtr<CTexture> m_EdgeTexture;

	class CMode7UI* m_UI;

private:
	CSharedPtr<CPlayer> m_Observer;

	bool m_Perspective;
	bool m_Start;

public:
	inline float GetFOV() const
	{
		return MODE7_FOV_HALF;
	}

	inline float GetClipFar() const
	{
		return MODE7_CLIP_FAR;
	}

	inline float GetClipNear() const
	{
		return MODE7_CLIP_NEAR;
	}

	inline bool GetPerspective() const
	{
		return m_Perspective;
	}

public:
	virtual bool Init() override;

	virtual bool Render(HDC hDC) override;

	virtual bool Update(float deltaTime) override;

	// 모드 7
	void Mode7(const Vector2& cameraPos,
		const float angle, const float fovHalf, const float clipNear, const float clipFar,
		const Vector2& circuitSize, const bool perspective, CTexture& sampleMap);

	// 해당좌표에서 색깔 가져오기
	COLORREF SampleMap(const Vector2& pos);
	COLORREF SampleMap(unsigned int x, unsigned int y);

	// 맵을 빠져나간 부분은 edge 텍스쳐에서 색깔을 가져온다.
	COLORREF SampleEdge(const Vector2& pos);
	COLORREF SampleEdge(unsigned int x, unsigned int y);

	// player를 기준으로 프러스텀을 생성하여 서킷을 그려낸다.
	void SetCircuitTexture(CSharedPtr<CPlayer> player, CTexture& circuitTexture);


public:
	void IncreaseFar(float deltaTime);
	void DecreaseFar(float deltaTime);

	void IncreaseNear(float deltaTime);
	void DecreaseNear(float dletaTime);

	void IncreaseFOV(float deltaTime);
	void DecreaseFOV(float deltaTime);

	void BackToTitle(float deltaTime);

	void TogglePerspective(float deltaTime);
};

