#pragma once

#include "../GameInfo.h"
#include "Scene.h"
#include "../Resource/Texture.h"

/// <summary>
/// Ÿ��Ʋ ȭ�� ��
/// </summary>
class CTitleScene :
	public CScene
{
	friend class CSceneManager;

private:
	CTitleScene();
	virtual ~CTitleScene();

private:
	enum class EMenuState
	{
		RACE,
		TOOL,
		MODE7,
		EXIT
	};

	// Ÿ��Ʋ UI ������
	class CTitleUI* m_TitleUI;

	// ���̽� �޴� ������
	class CRaceMenuUI* m_RaceMenuUI;

	// Ÿ��Ʋ������ ���� ��ũ�� ���
	class CScrollMap* m_ScrollMap;

	// ���� �޴� ����
	EMenuState m_CurrentMenu;
	
	// �� cc�� �����ߴ���
	ECCOption m_CCSelect;

	// Ŀ���� ��ġ
	Vector2 m_CursorPos;

	bool m_Start;
	
	// �޴� �˾�
	bool m_ShowMenu;

	// ���̽� �޴� �˾�
	bool m_ShowRaceMenu;

	// �޴����� ����̹��� �ִϸ��̼��� ���ؼ� �̿�
	float m_DriverFrequency[(int)EMenuPlayer::END];
	float m_CurrentTime;

public:
	virtual bool Init() override;
	virtual bool Update(float deltaTime) override;
	virtual bool Render(HDC hDC) override;

private:
	void LoadSound();

private:
	void MoveCursorUp(float deltaTime);
	void MoveCursorDown(float deltaTime);
	void Next(float deltaTime);
	void Back(float deltaTime);
};

