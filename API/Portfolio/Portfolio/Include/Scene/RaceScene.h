#pragma once

#include "Scene.h"
#include "../GameInfo.h"
#include "../Map/MapInfo.h"
#include "../Object/Driver.h"
#include "../Object/WallObject.h"
#include "../Object/FloorObject.h"

// CPU�� �������� ������� ���� �����ϴ� �ð��� ��
const float TIME_BETWEEN_ITEM_CHECKS = 1.0f / 15.0f;

// ó�� ��Ʈ�νÿ� ī�޶��� ȸ�� �ӵ�
const float ANIMATION_TURN_TIME = 3.5f;

// ���̽��� ������ �� ���� �� �ƿ� �ӵ�
const float ANIMATION_ZOOM_OUT_TIME = 6.0f;

// �� �ִϸ��̼� �ð�
const float ANIMATION_TOTAL_TIME = 10.0f;

// ��Ʈ�νÿ� �󸶳� �ָ� ������ ������ ���Ѵ�.
const float ANIMATION_FORWARD_DIST = 0.5f;

/// <summary>
/// ���̽� ���� ��ü���� ���̽��� �����Ѵ�.
/// </summary>
class CRaceScene :
	public CScene
{
	friend class CSceneManager;
private:
	CRaceScene();
	virtual ~CRaceScene();

private:
	// ��Ŷ ���� �ؽ��ĵ�
	CSharedPtr<CTexture> m_CircuitTexture;
	CSharedPtr<CTexture> m_OriginalCircuit;
	CSharedPtr<CTexture> m_EdgeTexture;
	CSharedPtr<CTexture> m_SkybackTexture;
	CSharedPtr<CTexture> m_SkyFrontTexture;

private:
	// ������ ���� �ؽ��ĵ�
	CSharedPtr<CTexture> m_GreenShell;
	CSharedPtr<CTexture> m_RedShell;
	CSharedPtr<CTexture> m_Banana;
	CSharedPtr<CTexture> m_Coins[3];
	CSharedPtr<CTexture> m_CollisionParticles[4];

private:
	// �÷��̾ ������ ĳ����
	EMenuPlayer m_PlayerSelect;

	// ����
	std::array<CDriver*, (int)EMenuPlayer::END> m_Positions;

	// ����̹� ������Ʈ�� ���� �迭
	CSharedPtr<CDriver> m_DriverArray[(int)EMenuPlayer::END];

	// �÷��̾�
	CSharedPtr<CDriver> m_Player;

	// ������ �ʴ� ĳ����
	CSharedPtr<CDriver> m_InvisiblePlayer;

	// �� ������Ʈ��
	std::list<CSharedPtr<CWallObject>> m_WallObjList;

	// ������ ����Ʈ
	std::list<CSharedPtr<CItem>> m_ItemList;

	// �ٴ� ������Ʈ(��ȣ�ۿ��Ŀ� �ٽ� �׸��� ���)
	std::vector<CSharedPtr<CFloorObject>> m_vecFloor;

	// ����̹��� �浹�ÿ� Ư���� ����� �ϴ� �ٴ�
	std::vector<CSharedPtr<CFloorObject>> m_SpecialFloor;

protected:
	// ���̽� ȭ���� ���7 ��
	float MODE7_FOV_HALF = 0.5f;
	float MODE7_CLIP_NEAR = 0.0005f;
	float MODE7_CLIP_FAR = 0.045f;

	// �̴ϸ� ī�޶��� ��ġ
	float MINIMAP_POS_DISTANCE = 6.5f;

	// �̴ϸ� ī�޶��� FOV
	float MINIMAP_FOV_HALF = (float)M_PI / 32.0f;

	// �̴ϸ��� ��ǥ
	float MINIMAP_BOTTOM_X = 20.0f / 512.0f;
	float MINIMAP_BOTTOM_Y = 438.0f / 448.0f;
	float MINIMAP_TOP_X = 57.0f / 512.0f;
	float MINIMAP_TOP_Y = 252.0f / 448.0f;


	// �ϴ��� ������
	float SKY_SCALE = 2.0f;

	// �ϴ��� �߸��� ����
	float SKY_CUT_PCT = 20.0f / 32.0f;

	// �� ���ȭ�鿡�� �ϴ��� �����ϴ� ����
	float SKY_HEIGHT_PCT = 2.0f / 22.4f;

	// ��Ŷ�� �����ϴ� ����
	float CIRCUIT_HEIGHT_PCT = 9.2f / 22.4f;

	// �̳����� �����ϴ� ����
	float MINIMAP_HEIGHT_PCT = 11.2f / 22.4f;

	// �÷��̾�� ī�޶��� �Ÿ�
	float CAM_TO_PLAYER_DIST = 46.0f;
	
public:
	inline float GetCircuitHeightPCT() const
	{
		return CIRCUIT_HEIGHT_PCT;
	}

	inline float GetCamDist() const
	{
		return CAM_TO_PLAYER_DIST;
	}

private:
	// ����ѹ�
	class CLakitu* m_Lakitu;

private:
	// ���� ��Ʈ�� ������.
	bool m_Intro;

	// ���̽��� ��������
	bool m_RaceFinished;

	// ��Ŷ �����׸��� ���ۉ����
	bool m_MainBgmStart;

	// ���̽��� ������ �� �ڿ� �ð�(�ִϸ��̼��� ���ؼ� ���ȴ�.)
	float m_TimeAfterRaceFinish;

	// ���̽� ��Ʈ�� ���� �ð�
	float m_IntroTime;

	// ������ ��� �ð� üũ
	float m_ItemUseAt;

	// ���� �÷��̾ �����Ͻÿ� �����ð� ��ٸ� �ð�
	float m_WaitTime;

	// ��̷�
	bool m_LookBack;

	// ������� ���� ���̽� �ð�
	float m_CurrentRaceTime;

	// �ٴ� ������Ʈ�� ���� �����ؾ��ϴ���
	bool m_FloorObjectUpdated;

public:
	// ��Ŷ�� �ҷ����鼭 �ʱ�ȭ
	virtual bool Init() override;

	// �׷�����
	virtual bool Render(HDC hDC) override;

	// ����
	virtual bool Update(float deltaTime) override;

	// ���� ���̽������� ���� ȿ�� �߰�
	virtual void AddCoinEffect(const CDriver* driver, int amount, bool positive);

	// ���� ���̽������� �浹 ȿ�� �߰�
	virtual void AddCollisionEffect(const Vector2& pos);

	// ���� ��Ŷ�� ������ �߰�
	inline void AddItem(const CSharedPtr<CItem>& item)
	{
		m_ItemList.push_back(item);
	}

	bool CircuitInit(ECCOption cc, ECupSelect cup, const std::string& course, EMenuPlayer player);

public:
	// ��� 7
	void Mode7(const Vector2& cameraPos,
		const float angle, const float fovHalf, const float clipNear, const float clipFar,
		const Vector2& circuitSize, const bool perspective, CTexture& sampleMap);

	// �ش���ǥ���� ���� ��������
	COLORREF SampleMap(const Vector2& pos);
	COLORREF SampleMap(unsigned int x, unsigned int y);

	// ���� �������� �κ��� edge �ؽ��Ŀ��� ������ �����´�.
	COLORREF SampleEdge(const Vector2& pos);
	COLORREF SampleEdge(unsigned int x, unsigned int y);
	
	// player�� �������� ���������� �����Ͽ� ��Ŷ�� �׷�����.
	void SetCircuitTexture(CSharedPtr<CDriver> player, CTexture& circuitTexture);

	// player�� �������� �ϴð� �޹���� �ؽ��ĸ� �׷�����.
	void SetSkyTexture(CSharedPtr<CDriver> player, CTexture& back, CTexture& front);

	// �ϴ��� �̴ϸ��� �����Ѵ�.
	void SetMiniMapTexture(CTexture& miniMap);

	
public:
	// 0 ~ 1������ ��ġ�� �̴ϸ��� ��ǥ(512 x 224)�� �ű��.
	Vector2 MinimapToScreen(const Vector2& position);

	// ���� ��Ŷ�� ����(0 ~ 1)���� ����� ���� ���(512 x 224)�� ȭ�鿡 ��� �׸����� ���Ѵ�.
	// target�� �þ߰��� ������ �ʴ´ٸ� ������ ��ȯ�Ѵ�.
	bool MapToScreen(CSharedPtr<CDriver> target, const Vector2& mapCoords, Vector2& screenCoords, float& z);

	// target�� �������� �þ߰��� ������ ��ü�� vecRenderable�� �ִ´�.
	void RenderableDrivers(const float screenScale, CSharedPtr<CDriver> target,
		std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

	// target�� �������� �þ߰��� ������ �� ������Ʈ���� ��´�.
	void RenderableWallObjects(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

	// target�� �������� �þ߰��� ������ �������� ��´�.
	void RenderableItems(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

public:
	// �ٴ� ������Ʈ�� ���Ž�Ų��.
	virtual void SetFloorObjectUpdated(bool val);

	// �����̳� �����۹ڽ����� �浹
	virtual void CollideWithFloor(const CSharedPtr<CDriver>& driver);

public:
	// ���� ���� �Ҽӵ� ����ѹ� ��ȯ
	virtual class CLakitu* GetLakitu() const
	{
		return m_Lakitu;
	}

	// ���� ���� �÷��̾� ��ȯ
	virtual CDriver* GetPlayer() const
	{
		return m_Player;
	}

	inline float GetCurrentRaceTime() const
	{
		return m_CurrentRaceTime;
	}

private:
	// �Ҹ� �� ȿ���� ��� �ҷ�����
	void LoadSound();

	// ������ ��� �ҷ�����
	void LoadItem();

	
private:
	// �ݹ� �Լ��� ���� ��Ÿ ���� 
	
	// �ʱ�ȭ�� �ѹ��� �ϱ� ���ؼ� start������ �̿�
	bool m_Start = false;

	// ���� ���̽��� ���ۉ����
	bool m_RaceStart = false;

	// �÷��̾ �Ǽ��� �󸶳� ��Ҵ���
	float m_PlayerAccelTime;

	
private:
	// �帮��Ʈ ����
	void DriftStart(float deltaTime)
	{
		m_Player->ShortJump();
	}

	void ToggleAutoDriver(float deltaTime);

	void GetFeather(float deltaTime);

	void UseItemForward(float deltaTime);

	void UseItemBackWard(float deltaTime);

	void LookBack(float deltaTime);

	void LookForward(float deltaTime);

	void BackToTitle(float deltaTime);
};

