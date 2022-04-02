#pragma once

#include "Scene.h"
#include "../GameInfo.h"
#include "../Resource/Texture.h"
#include "../Object/ObjectInfo.h"

const float UI_FADEIN_TIME = 1.5f;
const float UI_FADEOUT_TIME = 1.5f;

/// <summary>
/// �Ű� �ڽ��� ���� ��
/// </summary>
class CSelectScene :
	public CScene
{
	friend class CSceneManager;

private:
	CSelectScene();
	virtual ~CSelectScene();

	enum class EUIState
	{
		DRIVER,
		COURSE
	};

private:
	// ����̹� ���� ������ â
	class CDriverSelectUI* m_DriverSelectUI;
	// �� ���� ������ â
	class CCourseSelectUI* m_CourseSelectUI;

	// ���� ���� ��� ������ ������
	class CCourseWindow* m_MushroomWindow;

	// �ö�� ���� ��� ������ ������
	class CCourseWindow* m_FlowerWindow;

	// ��Ÿ ���� ���
	class CCourseWindow* m_StarWindow;

	// ����� ���� ���
	class CCourseWindow* m_SpecialWindow;

	// �÷��̾ ����̹� �����ҽÿ� ������ Ŀ���� ��ġ��
	Vector2 m_PlayerOnePos[(int)EMenuPlayer::END] =
	{
		Vector2(98.0f, 128.0f),
		Vector2(195.0f, 128.0f),
		Vector2(290.0f, 128.0f),
		Vector2(386.0f, 128.0f),
		Vector2(98.0f, 255.0f),
		Vector2(194.0f, 255.0f),
		Vector2(289.0f, 255.0f),
		Vector2(387.0f, 255.0f)
	};

	// Ȯ��
	CSharedPtr<CTexture> m_Confirm;

	// 1p Ŀ��
	CSharedPtr<CTexture> m_1P;

	// ���� ����̹�
	EMenuPlayer m_CurrentDriver;

	// ���� UI�� ���� (����̹� �������� �� ��������)
	EUIState m_UIState;

	// ���� ���õ� ��
	ECupSelect m_CurrentCup;

	// ���� CC 
	ECCOption m_CurrentCCOption;

	// ���̵� ��, �ƿ��� ���� ��ǥ
	Vector2 m_FadeLT;
	Vector2 m_FadeRB;

	// ���� Ŀ���� ��ǥ
	Vector2 m_CursorPos;

	float m_fElapsedTime;

	int m_CourseIndex;

	bool m_DriverSelected;
	bool m_CupSelected;
	bool m_FadeOut;
	bool m_FadeIn;

public:
	virtual bool Init() override;
	virtual bool Update(float deltaTime) override;
	virtual bool Render(HDC hDC) override;

public:
	inline void SetCurrentCC(ECCOption option)
	{
		m_CurrentCCOption = option;
	}

	void SetCCText(ECCOption option);

private:
	bool m_Start;
	void LoadSound();

	void Up(float deltaTime);
	void Down(float deltaTime);
	void Left(float deltaTime);
	void Right(float deltaTime);
	void Next(float deltaTime);
	void Back(float deltaTime);
};

