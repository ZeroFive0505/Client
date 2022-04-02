#include "Lakitu.h"
#include "../Scene/RaceScene.h"
#include "../Scene/SceneResource.h"
#include "../GameManager.h"
#include "Driver.h"


CLakitu::CLakitu() :
	m_Catch(nullptr),
	m_Shadow(nullptr),
	m_LakituLab(nullptr),
	m_CurrentLakitu(nullptr),
	m_CurrentLight(nullptr),
	m_CurrentLap(nullptr),
	m_Scene(nullptr)
{

	for (int i = 0; i < 3; i++)
	{
		m_Finish[i] = nullptr;
	}

	for (int i = 0; i < 2; i++)
	{
		m_WrongDir[i] = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		m_LapTexture[i] = nullptr;
	}

	for (int i = 0; i < 2; i++)
	{
		m_Start[i] = nullptr;
	}

	for (int i = 0; i < 4; i++)
	{
		m_Lights[i] = nullptr;
	}
}

CLakitu::~CLakitu()
{

}

// ����ѹ��� �ʱ�ȭ�Ѵ�.
bool CLakitu::Init(CRaceScene* scene)
{
	m_Scene = scene;

	m_Scale = 2.0f;

	Resolution rs = CGameManager::GetInst()->GetResolution();

	for (int i = 0; i < 3; i++)
	{
		std::string name = "Finish_" + std::to_string(i + 1);
		TCHAR pathName[MAX_PATH] = {};
		wsprintf(pathName, TEXT("Character/Lakitu/Finish_%d.bmp"), i + 1);
		m_Scene->GetSceneResoruce()->LoadTexture(name, pathName);

		m_Finish[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

		m_Finish[i]->SetColorKey(255, 0, 255);
		m_Finish[i]->SetScale(m_Scale, m_Scale);
	}

	for (int i = 0; i < 2; i++)
	{
		std::string name = "WrongDir_" + std::to_string(i + 1);
		TCHAR pathName[MAX_PATH] = {};
		wsprintf(pathName, TEXT("Character/Lakitu/WrongDir_%d.bmp"), i + 1);
		m_Scene->GetSceneResoruce()->LoadTexture(name, pathName);

		m_WrongDir[i] = m_Scene->GetSceneResoruce()->FindTexture(name);
		
		m_WrongDir[i]->SetColorKey(255, 0, 255);
		m_WrongDir[i]->SetScale(m_Scale, m_Scale);
	}

	for (int i = 0; i < 4; i++)
	{
		std::string name = "Lap_" + std::to_string(i + 1);
		TCHAR pathName[MAX_PATH] = {};
		wsprintf(pathName, TEXT("Character/Lakitu/Lap_%d.bmp"), i + 1);
		m_Scene->GetSceneResoruce()->LoadTexture(name, pathName);
		
		m_LapTexture[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

		m_LapTexture[i]->SetColorKey(255, 0, 255);
		m_LapTexture[i]->SetScale(m_Scale, m_Scale);
	}

	for (int i = 0; i < 2; i++)
	{
		std::string name = "Start_" + std::to_string(i + 1);
		TCHAR pathName[MAX_PATH] = {};
		wsprintf(pathName, TEXT("Character/Lakitu/Start_%d.bmp"), i + 1);
		m_Scene->GetSceneResoruce()->LoadTexture(name, pathName);

		m_Start[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

		m_Start[i]->SetColorKey(255, 0, 255);
		m_Start[i]->SetScale(m_Scale, m_Scale);
	}

	for (int i = 0; i < 4; i++)
	{
		std::string name = "Lights_" + std::to_string(i + 1);
		TCHAR pathName[MAX_PATH] = {};
		wsprintf(pathName, TEXT("Character/Lakitu/Lights_%d.bmp"), i + 1);

		m_Scene->GetSceneResoruce()->LoadTexture(name, pathName);

		m_Lights[i] = m_Scene->GetSceneResoruce()->FindTexture(name);

		m_Lights[i]->SetColorKey(255, 0, 255);
		m_Lights[i]->SetScale(m_Scale, m_Scale);
	}

	m_Scene->GetSceneResoruce()->LoadTexture("Shadow", TEXT("ETC/Shadow.bmp"));

	m_Shadow = m_Scene->GetSceneResoruce()->FindTexture("Shadow");
	m_Shadow->SetPivot((float)m_Shadow->GetWidth() / 2.0f, (float)m_Shadow->GetHeight());

	m_Scene->GetSceneResoruce()->LoadTexture("Catch", TEXT("Character/Lakitu/Catch.bmp"));

	m_Catch = m_Scene->GetSceneResoruce()->FindTexture("Catch");

	m_Catch->SetColorKey(255, 0, 255);

	m_Scene->GetSceneResoruce()->LoadTexture("Lakitu Lab", TEXT("Character/Lakitu/LakituLap.bmp"));

	m_LakituLab = m_Scene->GetSceneResoruce()->FindTexture("Lakitu Lab");

	m_LakituLab->SetScale(m_Scale, m_Scale);

	m_LakituLab->SetColorKey(255, 0, 255);

	m_CurrentLakitu = m_Start[0];

	m_CurrentLakitu->SetPos(rs.width / 4.0f, -20.0f);
	
	m_State = ELakituState::SLEEP;

	m_ScreenTime = 0.0f;
	m_NextFrameTime = 0.5f;
	m_FrameTime = 0.0f;

	m_Laps = 2;
	m_Light = 0;
	m_Started = false;

	m_DrawShadow = true;
	m_SoundStarted = false;
	m_OnPosition = false;

	m_CurrentAnimationPriority = 0;

	m_AnimationPriorities[(int)ELakituState::START] = 8;
	m_AnimationPriorities[(int)ELakituState::WRONG_DIR] = 0;
	m_AnimationPriorities[(int)ELakituState::PICKUP] = 9;
	m_AnimationPriorities[(int)ELakituState::LAP] = 5;
	m_AnimationPriorities[(int)ELakituState::FINISH] = 10;
	m_AnimationPriorities[(int)ELakituState::SLEEP] = 0;

	m_TextIdx = 0;

	return true;
}

// ���̽� ���� �ִϸ��̼��� �����Ѵ�.
void CLakitu::ShowRaceStart()
{
	m_FrameTime = 0.0f;
	m_ScreenTime = 0.0f;
	m_TextIdx = 0;

	m_Started = false;

	m_State = ELakituState::START;

	m_CurrentLakitu = m_Start[0];
	m_Start[1]->SetPivot(m_Start[1]->GetWidth() / 2.0f, m_Start[1]->GetHeight() / 2.0f);
	m_CurrentLakitu->SetPivot(m_CurrentLakitu->GetWidth() / 2.0f, m_CurrentLakitu->GetHeight() / 2.0f);

	Resolution rs = CGameManager::GetInst()->GetInst()->GetResolution();

	m_CurrentLakitu->SetPos(rs.width / 4.0f, -20.0f);

	m_Light = 0;

	for (int i = 0; i < 4; i++)
	{
		m_Lights[i]->SetPivot(m_Lights[i]->GetWidth() / 2.0f, 0.0f);
		m_Lights[i]->SetScale(m_Scale, m_Scale);
	}

	m_CurrentLight = m_Lights[m_Light];
	m_CurrentLight->SetPivot(m_CurrentLight->GetWidth() / 2.0f, 0.0f);
	m_CurrentLight->SetScale(m_Scale, m_Scale);

	// 1�ʰ������� ���� �ִϸ��̼� ���
	m_NextFrameTime = 1.0f;
	m_CurrentAnimationPriority = m_AnimationPriorities[(int)ELakituState::START];
}

// ���� ������ �����ش�.
void CLakitu::ShowCurrentLap(int numLap)
{
	if (numLap <= 5 && numLap >= 2 &&
		m_AnimationPriorities[(int)ELakituState::LAP])
	{
		m_LakituLab->SetPos(0.0f, 0.0f);

		m_FrameTime = 0.0f;
		m_ScreenTime = 0;
		m_TextIdx = 0;

		m_CurrentLakitu = m_LakituLab;

		m_State = ELakituState::LAP;

		m_CurrentLakitu->SetPivot(m_CurrentLakitu->GetWidth() / 2.0f, m_CurrentLakitu->GetHeight() / 2.0f);

		// 1���� �����ϰ� 2������ �����ϴ� -2
		m_CurrentLap = m_LapTexture[numLap - 2];

		m_CurrentLap->SetPivot(m_CurrentLap->GetWidth() / 2.0f, m_CurrentLap->GetHeight() / 2.0f);

		m_CurrentLight->SetScale(m_Scale, m_Scale);

		m_Laps = numLap;

		m_NextFrameTime = 0.5f;
		m_CurrentAnimationPriority = m_AnimationPriorities[(int)ELakituState::LAP];
	}
}

// ���̽��� ������ ���
void CLakitu::ShowRaceFinish()
{
	m_FrameTime = 0.0f;
	m_ScreenTime = 0.0f;
	m_TextIdx = 0;
	m_Started = false;
	m_State = ELakituState::FINISH;

	m_CurrentLakitu->SetPivot(m_Finish[0]->GetWidth() / 1.5f, m_Finish[0]->GetHeight() / 2.f);
	m_CurrentLakitu->SetPos(0.0f, 0.0f);
	m_NextFrameTime = 0.5f;
	m_CurrentAnimationPriority = m_AnimationPriorities[(int)ELakituState::FINISH];
}

// �÷��̾ �Ųٷ� �������� ���
void CLakitu::WarningWrongDir(bool wrongDir)
{
	// �ִϸ��̼��� �켱������ ���Ѵ�.
	if (wrongDir && m_CurrentAnimationPriority <= m_AnimationPriorities[(int)ELakituState::WRONG_DIR])
	{
		// ���� ���� �����°� �ƴ϶��
		if (m_State != ELakituState::WRONG_DIR)
		{
			// �����·� �ٲٰ�
			m_CurrentAnimationPriority = m_AnimationPriorities[(int)ELakituState::WRONG_DIR];

			// �Ҹ� ���
			m_Scene->GetSceneResoruce()->SoundPlay("WrongDir");
			m_CurrentLakitu = m_WrongDir[0];
			m_State = ELakituState::WRONG_DIR;
			m_CurrentLakitu->SetPivot(m_WrongDir[0]->GetWidth() / 2.0f, m_WrongDir[0]->GetHeight() / 2.0f);
			m_CurrentLakitu->SetPos(0.0f, 0.0f);
			m_Right = true;
			m_NextFrameTime = 0.5f;
		}
	}
	else
	{
		// ��� �����ϰ� �ö󰣴�.
		if (m_State == ELakituState::WRONG_DIR)
		{
			m_Scene->GetSceneResoruce()->SoundStop("WrongDir");
			m_State = ELakituState::GOING_UP;
			m_ScreenTime = 0.0f;
		}
	}
}

// ����̹��� ��Ŷ�� �ٱ����� ���������� ��
void CLakitu::PickUpDriver(CDriver* driver)
{
	if (m_CurrentAnimationPriority <= m_AnimationPriorities[(int)ELakituState::PICKUP])
	{
		Sleep();
		m_FrameTime = 0.0f;
		m_ScreenTime = 0.0f;
		m_TextIdx = 0;
		m_State = ELakituState::PICKUP;

		m_CurrentLakitu = m_Catch;
		m_CurrentLakitu->SetScale(m_Scale, m_Scale);

		Resolution rs = CGameManager::GetInst()->GetResolution();

		m_CurrentLakitu->SetPos(rs.width / 2.0f + 8.0f, -40);

		m_Driver = driver;

		m_Driver->m_OnLakitu = true;

		m_NextFrameTime = 0.5f;
		m_CurrentAnimationPriority = m_AnimationPriorities[(int)ELakituState::PICKUP];
	}
}

// seconds�� ��ŭ ����ѹ��� �����ش�.
void CLakitu::ShowUntil(float seconds, float deltaTime)
{
	// ���� �����ñ� �̻��̶�� ���¸� �������� �ٲٰ� �ʱ�ȭ
	if (m_ScreenTime > seconds)
	{
		m_State = ELakituState::SLEEP;
		m_ScreenTime = 0;
		m_WrongTime = 0.0f;
		m_OnPosition = false;
		m_TextIdx = 0;
		m_CurrentAnimationPriority = 0;
		m_Started = false;
	}
}

// ����ѹ��� ������ �������·θ����.
void CLakitu::Sleep()
{
	// ���� ������� ����ȴٸ� ����
	m_Scene->GetSceneResoruce()->SoundStop("WrongDir");
	m_CurrentLakitu->SetPivot(m_Start[0]->GetWidth() / 2.0f, m_Start[0]->GetHeight() / 2.0f);

	m_State = ELakituState::SLEEP;
	m_CurrentLakitu->SetPos(-20, -20);
	m_ScreenTime = 0.0f;
	m_TextIdx = 0;
	m_Started = false;
}

// ���̽��� ���ۉ����
bool CLakitu::HasRaceStarted()
{
	return m_Started;
}

// ����ѹ��� �ڰ��ִ���
bool CLakitu::IsSleeping()
{
	return m_State == ELakituState::SLEEP;
}

void CLakitu::Update(float deltaTime)
{
	// ���� ��Ȳ�� �´� ������Ʈ�� �����Ѵ�.
	switch (m_State)
	{
	// ���� ī��Ʈ �����Ͻÿ���
	case ELakituState::START:
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();

		// ������ ��ġ���� õõ�� �����´�.
		if (m_CurrentLakitu->GetPos().y < rs.height / 2 / 4 && !m_Started)
		{
			m_TextIdx = 0;
			m_CurrentLakitu->Move(0, 2);
		}
		// ���� ������ ��ġ�� ����������
		else
		{
			if (!m_SoundStarted)
			{
				// ����ѹ� ���̽� ī��Ʈ�ٿ� �Ҹ� ���
				m_SoundStarted = true;
				m_Scene->GetSceneResoruce()->SoundPlay("RaceStart");
			}

			// 1�ʾ� ī��Ʈ�� ����.
			m_FrameTime += deltaTime;

			// m_NextFrameTime�� 1���̸� 1�ʰ� ���������� ���� �ؽ��ķ� ��ȯ�ȴ�.
			if (m_FrameTime >= m_NextFrameTime)
			{
				m_TextIdx++;
				m_FrameTime = 0.0f;
			}

			// ��ȣ �ؽ�ó�� ��ü�Ѵ�. ���� �ؽ��Ŀ� �˸°�
			if (m_TextIdx > 1 && m_TextIdx < 5)
			{
				m_CurrentLight = m_Lights[m_TextIdx - 1];
			}

			// ���� �ؽ��ĸ� �ٲٰ�
			// ���� �÷��׸� �����.
			if (m_TextIdx >= 4)
			{
				m_Start[1]->SetPos(m_CurrentLakitu->GetPos());
				m_CurrentLakitu = m_Start[1];
				m_Started = true;
			}

			m_ScreenTime += deltaTime;
			ShowUntil(10.0f, deltaTime);
		}

		// �ٽ� ���� �ö󰣴�.
		if (m_Started && m_TextIdx >= 5)
		{
			m_CurrentLakitu->Move(0, -2);
		}

		Vector2 pos = m_CurrentLakitu->GetPos();

		if (m_TextIdx >= 4)
			m_CurrentLight->SetPos(pos.x - m_CurrentLakitu->GetWidth() / 2 * 1.1f, pos.y);
		else
			m_CurrentLight->SetPos(pos.x - m_CurrentLakitu->GetWidth() / 2 * 1, pos.y);
	}
		break;
	// �Ųٷ� �������� ���
	case ELakituState::WRONG_DIR:
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();
		m_CurrentLakitu = m_WrongDir[m_TextIdx % 2];
		m_CurrentLakitu->SetPivot(m_CurrentLakitu->GetWidth() / 2.0f, m_CurrentLakitu->GetHeight() / 2.0f);
		m_WrongDir[0]->SetPos(m_CurrentLakitu->GetPos());
		m_WrongDir[1]->SetPos(m_CurrentLakitu->GetPos());
		m_FrameTime += deltaTime;

		if (m_FrameTime >= m_NextFrameTime)
		{
			m_TextIdx++;
			m_FrameTime = 0.0f;
		}


		if (m_WrongTime < 1.5f && !m_OnPosition)
		{
			m_CurrentLakitu->Move(3.0f, 1.2f);
		}
		else if(!m_OnPosition)
		{
			m_OnPosition = true;
			m_WrongTime = 0.0f;
			m_fElapsedTime = 0.0f;
		}

		// ������ ��ġ�� ����������� �¿�� �����δ�.
		if (m_OnPosition)
		{
			if (m_Right)
			{
				// �ﰢ�Լ��� �̿��Ͽ� ���Ʒ��� �ణ�� �����̰�
				m_CurrentLakitu->Move(2.0f, sinf(m_fElapsedTime * 2.0f));

				if (m_WrongTime >= 3.5f)
				{
					m_Right = false;
					m_WrongTime = 0.0f;
				}
			}
			else
			{
				m_CurrentLakitu->Move(-2.0f, sinf(m_fElapsedTime * 2.0f));

				if (m_WrongTime >= 3.5f)
				{
					m_Right = true;
					m_WrongTime = 0.0f;
				}
			}

			m_fElapsedTime += deltaTime;
		}

		m_WrongTime += deltaTime;

	}
		break;
	// ������� ��
	case ELakituState::GOING_UP:
	{
		m_CurrentLakitu->Move(0, -3.5f);
		m_ScreenTime += deltaTime;
		m_Scene->GetSceneResoruce()->SoundStop("WrongDir");
		ShowUntil(5.0f, deltaTime);
	}
		break;
	case ELakituState::LAP:
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();

		float x = 0.0f;
		float y = 0.0f;

		if (m_ScreenTime < 1.5f)
		{
			x = 2.5f;
			y = 1.0f;
		}
		else
		{
			x -= 0.5f;
			y -= 3.0f;
		}

		m_CurrentLakitu->Move(x, y);

		Vector2 pos = m_CurrentLakitu->GetPos();

		m_CurrentLap->SetPos(pos.x - 5, pos.y - 10);

		m_ScreenTime += deltaTime;
		ShowUntil(5.0f, deltaTime);
	}
		break;
	// ���̽� ���� ȣ
	case ELakituState::FINISH:
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();
		m_CurrentLakitu = m_Finish[m_TextIdx % 3];
		m_FrameTime += deltaTime;

		if (m_FrameTime >= m_NextFrameTime)
		{
			m_TextIdx++;
			m_FrameTime = 0.0f;
		}

		// �����Լ��� �̿��Ͽ� ���������� �����̰��Ѵ�.
		// ���� ����ؼ� �պ��� �ؾ��ϹǷ� x�� ���� �ﰢ�Լ��� ��°����� ����
		float x = -3.0f * cosf(m_ScreenTime);
		// ��ũ���� y���� �ݴ밡 �Ǿ������Ƿ� �Ʒ��� ������ �������� -�� �ٿ����Ѵ�.
		float y = (-(x * x) / 40.0f);

		// x, y ���� �˸°� ��ġ ����
		m_CurrentLakitu->SetPos(rs.width / 6.0f + 30.0f * x , y * rs.height + rs.height / 15.0f);

		m_ScreenTime += deltaTime;
		ShowUntil(12.0f, deltaTime);
	}
		break;
	case ELakituState::SLEEP:
		break;
	// ����̹� �Ⱦ� ����
	case ELakituState::PICKUP:
	{
		Resolution rs = CGameManager::GetInst()->GetResolution();

		// Ư�� ��ġ���� �����´�.
		if (m_CurrentLakitu->GetPos().y < m_CurrentLakitu->GetHeight() / 2 / 5 && m_Driver->m_OnLakitu)
		{
			m_TextIdx = 0;
			m_CurrentLakitu->Move(0, 1.5f);
		}
		else
		{
			m_FrameTime += deltaTime;
			// �ð��� �����ϸ鼭 �ؽ��ĸ� �ٲ۴�.
			if (m_FrameTime >= m_NextFrameTime)
			{
				m_TextIdx++;
				m_FrameTime = 0.0f;
			}

			// ���� ���� �ִϸ��̼��Ͻÿ���
			if (m_TextIdx == 1)
			{
				m_TextIdx++;
				// ����̹��� �����ش�.
				m_Driver->m_OnLakitu = false;
				m_Driver->AddCoin(-2);
				// �ٽ� ���������� ���·θ�����ش�.
				m_Driver->PopStateEnd(EDriverState::STOPPED);
			}

			m_ScreenTime += deltaTime;
			ShowUntil(8.0f, deltaTime);
		}

		// ����ѹ��� ���� �ö󰡸鼭 �������.
		if (!m_Driver->m_OnLakitu)
		{
			m_CurrentLakitu->Move(0, -1);
		}

		Vector2 pos = m_CurrentLakitu->GetPos();

		if (m_Driver->m_OnLakitu)
		{
			float scaleFactor = ((float)rs.width / BASIC_WIDTH);
			// ����ѹ����� �����������Ƿ� ��ġ�� y������ �� �ö� ����
			float driverY = m_Driver->m_Animator.GetTexture()->GetHeight() * 1.6 * scaleFactor;

			m_Driver->SetHeight(((rs.height / 2 - pos.y - m_CurrentLakitu->GetHeight() - driverY) / 10.0f) / scaleFactor);
		}
	}
		break;
	}
}

void CLakitu::Render(HDC hDC)
{
	// �������°� �ƴ϶�� �׷�����.
	if (m_State != ELakituState::SLEEP)
	{
		m_CurrentLakitu->Render(hDC, m_CurrentLakitu->GetPos(), Vector2(0.0f, 0.0f), m_CurrentLakitu->GetSize());

		if (m_State == ELakituState::START)
			m_CurrentLight->Render(hDC, m_CurrentLight->GetPos(), Vector2(0.0f, 0.0f), m_CurrentLight->GetSize());
		else if (m_State == ELakituState::LAP)
			m_CurrentLap->Render(hDC, m_CurrentLap->GetPos(), Vector2(0.0f, 0.0f), m_CurrentLap->GetSize());

		Vector2 pos = m_CurrentLakitu->GetPos();
		Resolution rs = CGameManager::GetInst()->GetResolution();

		// ���� ���̰� ���� ���� �̻��̶�� �׸��ڸ� �׸���.
		if (m_Shadow && pos.y > 0)
		{
			CTexture* cloneShadow = m_Shadow->Clone();
			pos = m_CurrentLakitu->GetPos();

			float y = rs.height * 0.45f;

			cloneShadow->SetPivot(cloneShadow->GetWidth() / 2.0f, cloneShadow->GetHeight() / 2.0f);
			if (m_State == ELakituState::FINISH)
				cloneShadow->SetPos(pos.x + 16.0f, y);
			else
				cloneShadow->SetPos(pos.x, y);
			cloneShadow->SetColorKey(255, 0, 255);
			cloneShadow->SetScale(m_CurrentLakitu->GetScale() / 1.5f);
			cloneShadow->Render(hDC, cloneShadow->GetPos(), Vector2(0.0f, 0.0f), Vector2(cloneShadow->GetWidth(), cloneShadow->GetHeight()));
			SAFE_DELETE(cloneShadow);
		}
	}
}

void CLakitu::Reset()
{
	WarningWrongDir(false);
	Sleep();
	m_FrameTime = 0.0f;
	m_Laps = 2;
	m_Light = 0;
	m_Started = false;
	m_SoundStarted = false;
	m_CurrentAnimationPriority = 0;
	m_OnPosition = false;
	m_Right = true;
}
