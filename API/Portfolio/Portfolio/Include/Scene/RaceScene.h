#pragma once

#include "Scene.h"
#include "../GameInfo.h"
#include "../Map/MapInfo.h"
#include "../Object/Driver.h"
#include "../Object/WallObject.h"
#include "../Object/FloorObject.h"

// CPU가 아이템을 사용할지 말지 결정하는 시간의 텀
const float TIME_BETWEEN_ITEM_CHECKS = 1.0f / 15.0f;

// 처음 인트로시에 카메라의 회전 속도
const float ANIMATION_TURN_TIME = 3.5f;

// 레이스가 끝나고 난 뒤의 줌 아웃 속도
const float ANIMATION_ZOOM_OUT_TIME = 6.0f;

// 총 애니메이션 시간
const float ANIMATION_TOTAL_TIME = 10.0f;

// 인트로시에 얼마나 멀리 떨어져 있을지 정한다.
const float ANIMATION_FORWARD_DIST = 0.5f;

/// <summary>
/// 레이스 씬은 전체적인 레이스를 관리한다.
/// </summary>
class CRaceScene :
	public CScene
{
	friend class CSceneManager;
private:
	CRaceScene();
	virtual ~CRaceScene();

private:
	// 서킷 관련 텍스쳐들
	CSharedPtr<CTexture> m_CircuitTexture;
	CSharedPtr<CTexture> m_OriginalCircuit;
	CSharedPtr<CTexture> m_EdgeTexture;
	CSharedPtr<CTexture> m_SkybackTexture;
	CSharedPtr<CTexture> m_SkyFrontTexture;

private:
	// 아이템 관련 텍스쳐들
	CSharedPtr<CTexture> m_GreenShell;
	CSharedPtr<CTexture> m_RedShell;
	CSharedPtr<CTexture> m_Banana;
	CSharedPtr<CTexture> m_Coins[3];
	CSharedPtr<CTexture> m_CollisionParticles[4];

private:
	// 플레이어가 선택한 캐릭터
	EMenuPlayer m_PlayerSelect;

	// 순위
	std::array<CDriver*, (int)EMenuPlayer::END> m_Positions;

	// 드라이버 업데이트를 위한 배열
	CSharedPtr<CDriver> m_DriverArray[(int)EMenuPlayer::END];

	// 플레이어
	CSharedPtr<CDriver> m_Player;

	// 보이지 않는 캐릭터
	CSharedPtr<CDriver> m_InvisiblePlayer;

	// 벽 오브젝트들
	std::list<CSharedPtr<CWallObject>> m_WallObjList;

	// 아이템 리스트
	std::list<CSharedPtr<CItem>> m_ItemList;

	// 바닥 오브젝트(상호작용후에 다시 그릴때 사용)
	std::vector<CSharedPtr<CFloorObject>> m_vecFloor;

	// 드라이버와 충돌시에 특별한 기능을 하는 바닥
	std::vector<CSharedPtr<CFloorObject>> m_SpecialFloor;

protected:
	// 레이스 화면의 모드7 값
	float MODE7_FOV_HALF = 0.5f;
	float MODE7_CLIP_NEAR = 0.0005f;
	float MODE7_CLIP_FAR = 0.045f;

	// 미니맵 카메라의 위치
	float MINIMAP_POS_DISTANCE = 6.5f;

	// 미니맵 카메라의 FOV
	float MINIMAP_FOV_HALF = (float)M_PI / 32.0f;

	// 미니맵의 좌표
	float MINIMAP_BOTTOM_X = 20.0f / 512.0f;
	float MINIMAP_BOTTOM_Y = 438.0f / 448.0f;
	float MINIMAP_TOP_X = 57.0f / 512.0f;
	float MINIMAP_TOP_Y = 252.0f / 448.0f;


	// 하늘의 스케일
	float SKY_SCALE = 2.0f;

	// 하늘이 잘리는 비율
	float SKY_CUT_PCT = 20.0f / 32.0f;

	// 윗 상단화면에서 하늘이 차지하는 비율
	float SKY_HEIGHT_PCT = 2.0f / 22.4f;

	// 서킷이 차지하는 비율
	float CIRCUIT_HEIGHT_PCT = 9.2f / 22.4f;

	// 미내맵이 차지하는 비율
	float MINIMAP_HEIGHT_PCT = 11.2f / 22.4f;

	// 플레이어와 카메라의 거리
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
	// 김수한무
	class CLakitu* m_Lakitu;

private:
	// 현재 인트로 중인지.
	bool m_Intro;

	// 레이스가 끝났는지
	bool m_RaceFinished;

	// 서킷 메인테마가 시작됬는지
	bool m_MainBgmStart;

	// 레이스가 끝나고 난 뒤에 시간(애니메이션을 위해서 사용된다.)
	float m_TimeAfterRaceFinish;

	// 레이스 인트로 누적 시간
	float m_IntroTime;

	// 아이템 사용 시간 체크
	float m_ItemUseAt;

	// 만약 플레이어가 꼴지일시에 일정시간 기다릴 시간
	float m_WaitTime;

	// 백미러
	bool m_LookBack;

	// 현재까지 누적 레이스 시간
	float m_CurrentRaceTime;

	// 바닥 오브젝트를 새로 갱신해야하는지
	bool m_FloorObjectUpdated;

public:
	// 서킷을 불러오면서 초기화
	virtual bool Init() override;

	// 그려내기
	virtual bool Render(HDC hDC) override;

	// 갱신
	virtual bool Update(float deltaTime) override;

	// 현재 레이스씬에서 동전 효과 추가
	virtual void AddCoinEffect(const CDriver* driver, int amount, bool positive);

	// 현재 레이스씬에서 충돌 효과 추가
	virtual void AddCollisionEffect(const Vector2& pos);

	// 현재 서킷에 아이템 추가
	inline void AddItem(const CSharedPtr<CItem>& item)
	{
		m_ItemList.push_back(item);
	}

	bool CircuitInit(ECCOption cc, ECupSelect cup, const std::string& course, EMenuPlayer player);

public:
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
	void SetCircuitTexture(CSharedPtr<CDriver> player, CTexture& circuitTexture);

	// player를 기준으로 하늘과 뒷배경의 텍스쳐를 그려낸다.
	void SetSkyTexture(CSharedPtr<CDriver> player, CTexture& back, CTexture& front);

	// 하단의 미니맵을 설정한다.
	void SetMiniMapTexture(CTexture& miniMap);

	
public:
	// 0 ~ 1사이의 위치를 미니맵의 좌표(512 x 224)로 옮긴다.
	Vector2 MinimapToScreen(const Vector2& position);

	// 현재 서킷의 범위(0 ~ 1)에서 계산을 거쳐 상단(512 x 224)의 화면에 어디에 그릴지를 정한다.
	// target의 시야각에 들어오지 않는다면 거짓을 반환한다.
	bool MapToScreen(CSharedPtr<CDriver> target, const Vector2& mapCoords, Vector2& screenCoords, float& z);

	// target을 기준으로 시야각에 들어오는 객체를 vecRenderable에 넣는다.
	void RenderableDrivers(const float screenScale, CSharedPtr<CDriver> target,
		std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

	// target을 기준으로 시야각에 들어오는 벽 오브젝트들을 담는다.
	void RenderableWallObjects(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

	// target을 기준으로 시야각에 들어오는 아이템을 담는다.
	void RenderableItems(const float screenScale, CSharedPtr<CDriver> target, std::vector<std::pair<std::pair<float, CSharedPtr<CTexture>>, CSharedPtr<CWallObject>>>& vecRenderable);

public:
	// 바닥 오브젝트를 갱신시킨다.
	virtual void SetFloorObjectUpdated(bool val);

	// 코인이나 아이템박스와의 충돌
	virtual void CollideWithFloor(const CSharedPtr<CDriver>& driver);

public:
	// 현재 씬에 소속된 김수한무 반환
	virtual class CLakitu* GetLakitu() const
	{
		return m_Lakitu;
	}

	// 현재 씬의 플레이어 반환
	virtual CDriver* GetPlayer() const
	{
		return m_Player;
	}

	inline float GetCurrentRaceTime() const
	{
		return m_CurrentRaceTime;
	}

private:
	// 소리 및 효과음 모두 불러오기
	void LoadSound();

	// 아이템 모두 불러오기
	void LoadItem();

	
private:
	// 콜백 함수와 개인 기타 개인 
	
	// 초기화는 한번만 하기 위해서 start변수를 이용
	bool m_Start = false;

	// 현재 레이스가 시작됬는지
	bool m_RaceStart = false;

	// 플레이어가 악셀을 얼마나 밟았는지
	float m_PlayerAccelTime;

	
private:
	// 드리프트 시작
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

