
#include "GameObject.h"
#include "../GameManager.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene//SceneCollision.h"
#include "../Resource/AnimationSequence.h"
#include "../Scene/Camera.h"
#include "../Map/TileMap.h"

CGameObject::CGameObject() :
	m_Scene(nullptr),
	m_MoveSpeed(200.f),
	m_TimeScale(1.f),
	m_Animation(nullptr),
	m_CameraCull(false),
	m_Start(false),
	m_DamageEnable(true),
	m_ObjType(EObject_Type::GameObject),
	m_PhysicsSimulate(false),
	m_IsGround(true),
	m_FallTime(0.0f),
	m_FallStartY(0.0f),
	m_Jump(false),
	m_JumpVelocity(0.0f),
	m_GravityAccel(10.0f),
	m_LifeTime(0.0f),
	m_FloorCheck(false),
	m_SideWallCheck(false)
{
	m_PrevPos.y = -1.0f;
}

// 복사 생성자 호출시 애니메이션은 새롭게 복사해서 넣는다.
CGameObject::CGameObject(const CGameObject& obj) :
	CRef(obj)
{
	m_GravityAccel = obj.m_GravityAccel;
	m_PhysicsSimulate = obj.m_PhysicsSimulate;
	m_IsGround = obj.m_IsGround;
	m_FallTime = obj.m_FallTime;
	m_FallStartY = obj.m_FallStartY;
	m_Jump = obj.m_Jump;
	m_JumpVelocity = obj.m_JumpVelocity;
	m_DamageEnable = obj.m_DamageEnable;
	m_ObjType = obj.m_ObjType;
	m_FloorCheck = obj.m_FloorCheck;
	m_SideWallCheck = obj.m_SideWallCheck;
	m_PrevPos.y = -1.0f;


	m_Start = false;

	m_Scene = obj.m_Scene;

	if (obj.m_Animation)
		m_Animation = obj.m_Animation->Clone();

	m_Animation->m_Owner = this;

	m_ColliderList.clear();

	auto iter = obj.m_ColliderList.begin();
	auto iterEnd = obj.m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		CCollider* Collider = (*iter)->Clone();

		Collider->SetOwner(this);
		Collider->SetScene(m_Scene);

		m_ColliderList.push_back(Collider);
	}

	m_WidgetComponentList.clear();

	{
		auto iter = obj.m_WidgetComponentList.begin();
		auto iterEnd = obj.m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			CWidgetComponent* Widget = (*iter)->Clone();
			Widget->SetOwner(this);
			Widget->SetScene(m_Scene);

			m_WidgetComponentList.push_back(Widget);
		}
	}


	m_PrevPos = Vector2(obj.m_PrevPos.x, -1.0f);
	m_Pos = obj.m_Pos;
	m_Size = obj.m_Size;
	m_Pivot = obj.m_Pivot;
	m_Velocity = obj.m_Velocity;
	m_Offset = obj.m_Offset;
	m_MoveSpeed = obj.m_MoveSpeed;
	m_TimeScale = obj.m_TimeScale;
	m_Texture = obj.m_Texture;
	m_ImageStart = obj.m_ImageStart;
}

// 소멸시 애니메이션 오브젝트도 소멸한다.
CGameObject::~CGameObject()
{
	SAFE_DELETE(m_Animation);

	// 현재 오브젝트가 소멸시에 이 오브젝트와 충돌했던 오브젝트를 순회하면서 삭제 요청을 한다.
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		(*iter)->ClearCollisionList();
	}
}

// 방향만 인자로 기본 속도를 곱해서 새로운 위치를 선정한다.
void CGameObject::Move(const Vector2& Dir)
{
	Vector2	CurrentMove = Dir * m_MoveSpeed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

// 방향과 속도를 동시에 전달받아 새로운 위치를 선정한다.
void CGameObject::Move(const Vector2& Dir, float Speed)
{
	Vector2	CurrentMove = Dir * Speed * CGameManager::GetInst()->GetDeltaTime() * m_TimeScale;
	m_Velocity += CurrentMove;
	m_Pos += CurrentMove;
}

// 현재 오브젝트의 텍스쳐를 설정한다.
void CGameObject::SetTexture(const std::string& Name)
{
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

// 텍스쳐를 설정한다.
void CGameObject::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	// 현재 씬의 씬 리소스를 받아와서 텍스쳐를 불러온다.
	m_Scene->GetSceneResource()->LoadTexture(Name, FileName, PathName);

	// 현재 텍스쳐에 지정한다.
	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

// 텍스쳐를 전체 경로를 통해서 설정한다.
void CGameObject::SetTextrueFullPath(const std::string& Name, const TCHAR* FullPath)
{
	m_Scene->GetSceneResource()->LoadTextureFullPath(Name, FullPath);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}

}

// 낱장단위로 있는 텍스쳐를 전부 다 불러와서 설정한다.
void CGameObject::SetTexture(const std::string& Name,
	const std::vector<std::wstring>& vecFileName,
	const std::string& PathName)
{
	m_Scene->GetSceneResource()->LoadTexture(Name, vecFileName, PathName);

	m_Texture = m_Scene->GetSceneResource()->FindTexture(Name);

	if (m_Texture)
	{
		m_Size.x = (float)m_Texture->GetWidth();
		m_Size.y = (float)m_Texture->GetHeight();
	}
}

// 칼라 키를 설정한다.
void CGameObject::SetTextureColorKey(unsigned char r, unsigned char g, unsigned char b, int Index)
{
	if (m_Texture)
		m_Texture->SetColorKey(r, g, b, Index);
}

void CGameObject::Start()
{
	m_Start = true;

	m_FallStartY = m_Pos.y;
}

bool CGameObject::Init()
{
	return true;
}

void CGameObject::Update(float DeltaTime)
{
	if (!m_Start)
	{
		Start();
	}

	if (m_LifeTime > 0.0f)
	{
		m_LifeTime -= DeltaTime;

		if (m_LifeTime <= 0.0f)
		{
			Destroy();
			return;
		}
	}

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	// 중력을 적용한다.
	if (!m_IsGround && m_PhysicsSimulate)
	{
		// 떨어지는 시간을 누젹시켜준다.
		m_FallTime += DeltaTime * m_GravityAccel;

		// 9.8 m/s^2
		// t초 후 y의 값
		// V : 속도, A : 가속도, G : 중력
		// y = V * A - 0.5f * G * t * t (자유 낙하)
		// 0 = -0.5 * G * T^2 * V * A - y
		// (-b +- sqrt(b^2 - 4ac) / 2a
		float Velocity = 0.0f;

		if (m_Jump)
			Velocity = m_JumpVelocity * m_FallTime;

		float SaveY = m_Pos.y;

		m_Pos.y = m_FallStartY - (Velocity - 0.5f * GRAVITY * m_FallTime * m_FallTime);
	}

	{
		auto iter = m_ColliderList.begin();
		auto iterEnd = m_ColliderList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ColliderList.erase(iter);
				iterEnd = m_ColliderList.end();
				continue;
			}
			else if ((*iter)->GetEnable())
			{
				(*iter)->Update(DeltaTime);
			}

			iter++;
		}
	}

	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->Update(DeltaTime);
		}
	}
}

void CGameObject::PostUpdate(float DeltaTime)
{
	// 애니메이션이 있을 경우라면 애니메이션의 사이즈에 맞게 새롭게 사이즈를 갱신한다.
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		m_Size = FrameData.Size;
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		else if ((*iter)->GetEnable())
		{
			(*iter)->PostUpdate(DeltaTime);
		}

		iter++;
	}

	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->PostUpdate(DeltaTime);
		}
	}

	float DirX = m_Pos.x - m_PrevPos.x;

	if (DirX != 0.0f && m_SideWallCheck)
	{
		Vector2 LT = m_Pos - m_Pivot * m_Size;
		Vector2 RB = LT + m_Size;

		Vector2 PrevLT = m_PrevPos - m_Pivot * m_Size;
		Vector2 PrevRB = PrevLT + m_Size;


		CTileMap* TileMap = m_Scene->GetTileMap();

		int LTIndexX, LTIndexY, RBIndexX, RBIndexY;

		bool SideCollision = false;

		// 오른쪽으로 이동할때
		if (DirX > 0.0f)
		{
			Vector2 resultLT, resultRB;

			resultLT = LT;
			resultRB = RB;
			// 타일맵과 충돌했을시 영역의 범위를 구한다.
			// resultLT의 경우 타일맵과 겹치는 부분중에
			// PrevRB.x의 좌표를 이용해서 구한다.
			// resultLT.y의 경우 더 작은 PrevLT.y, LT.y 중 더 작은 값을 취한다.
			resultLT.x = PrevRB.x;
			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;

			// ResultRB의 경우 x의 값은 그대로 RB.x의 값과 같고
			// y의 값은 더 큰 값을 가져간다.
			resultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.y);

			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

			for (int i = LTIndexY; i <= RBIndexY; i++)
			{
				for (int j = LTIndexX; j <= RBIndexX; j++)
				{
					int Index = i * TileMap->GetTileCountX() + j;

					if (TileMap->GetTile(Index)->GetTileProperty() != ETileProperty::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					Vector2 TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2 TileSize = TileMap->GetTile(Index)->GetSize();

					// 땅위를 미세하게 밟고있으면 건너뛴다.
					if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
						continue;

					// 프레임 드랍으로 플레이어가 타일맵에 깊숙하게 들어갈 경우
					// 이 경우는 충돌을 무시한다.
					if (RB.x - TilePos.x > 8.0f)
						continue;


					// 렉트 충돌
					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && RB.y >= TilePos.y)
					{
						SideCollision = true;

						// 현재 TilePos.x와 RB.x를 빼면 음수가 나온다. 여기서 약간 더 왼쪽으로 민다.
						float MoveX = TilePos.x - RB.x - 0.001f;

						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}

				if (SideCollision)
					break;
			}
		}

		// 왼쪽으로 이동할때
		else
		{
			Vector2 resultLT, resultRB;

			resultLT = LT;
			resultRB = RB;

			// 이번엔 왼쪽으로 이동했을때 겹치는 범위를 구한다.
			// resultLT.y의 값의 경우 LT, PrevLT 둘중에 더 작은 y의 값을 가져오며
			// resultRB.y의 값의 경우 RB, PrevRV 둘중에 더 큰 y의 값을 가져온다.
			resultLT.y = LT.y < PrevLT.y ? LT.y : PrevLT.y;
			resultRB.y = RB.y > PrevRB.y ? RB.y : PrevRB.y;

			// resultRB.x의 값은 PrevLT.x의 값으로 설정한다.
			resultRB.x = PrevLT.x;

			LTIndexX = TileMap->GetOriginTileIndexX(resultLT.x);
			LTIndexY = TileMap->GetOriginTileIndexY(resultLT.y);
			RBIndexX = TileMap->GetOriginTileIndexX(resultRB.x);
			RBIndexY = TileMap->GetOriginTileIndexY(resultRB.x);

			LTIndexX = LTIndexX < 0 ? 0 : LTIndexX;
			LTIndexY = LTIndexY < 0 ? 0 : LTIndexY;

			RBIndexX = RBIndexX > TileMap->GetTileCountX() - 1 ? TileMap->GetTileCountX() - 1 : RBIndexX;
			RBIndexY = RBIndexY > TileMap->GetTileCountY() - 1 ? TileMap->GetTileCountY() - 1 : RBIndexY;

			for (int i = LTIndexY; i <= RBIndexY; i++)
			{
				for (int j = RBIndexX; j >= LTIndexX; j--)
				{
					int Index = i * TileMap->GetTileCountX() + j;

					if (TileMap->GetTile(Index)->GetTileProperty() != ETileProperty::Wall ||
						!TileMap->GetTile(Index)->GetSideCollision())
						continue;

					Vector2 TilePos = TileMap->GetTile(Index)->GetPos();
					Vector2 TileSize = TileMap->GetTile(Index)->GetSize();

					if (TilePos.y - 0.001f <= RB.y && RB.y <= TilePos.y + 0.001f)
						continue;

					if (TilePos.x + TileSize.x - LT.x > 8.0f)
						continue;

					if (LT.x <= TilePos.x + TileSize.x &&
						LT.y <= TilePos.y + TileSize.y &&
						RB.x >= TilePos.x && RB.y >= TilePos.y)
					{
						SideCollision = true;

						float MoveX = TilePos.x + TileSize.x - LT.x + 0.001f;

						m_Pos.x += MoveX;
						m_Velocity.x += MoveX;
					}
				}

				if (SideCollision)
					break;
			}
		}
	}

	if (m_PhysicsSimulate && m_Pos.y - m_PrevPos.y >= 0.0f)
	{
		CTileMap* TileMap = m_Scene->GetTileMap();

		float PrevBottom = m_PrevPos.y + (1.0f - m_Pivot.y) * m_Size.y;
		float CurBottom = m_Pos.y + (1.0f - m_Pivot.y) * m_Size.y;
		float PrevLeft = m_PrevPos.x - m_Pivot.x * m_Size.x;
		float CurLeft = m_Pos.x - m_Pivot.x * m_Size.x;
		float PrevRight = PrevLeft + m_Size.x;
		float CurRight = CurLeft + m_Size.x;

		float resultLeft = PrevLeft < CurLeft ? PrevLeft : CurLeft;
		float resultRight = PrevRight > CurRight ? PrevRight : CurRight;
		float resultTop = PrevBottom < CurBottom ? PrevBottom : CurBottom;
		float resultBottom = PrevBottom > CurBottom ? PrevBottom : CurBottom;

		// 이전 위치와 현재 위치의 타일 인덱스를 구해온다.
		int LeftIndex, TopIndex, RightIndex, BottomIndex;

		LeftIndex = TileMap->GetOriginTileIndexX(resultLeft);
		TopIndex = TileMap->GetOriginTileIndexY(resultTop);
		RightIndex = TileMap->GetOriginTileIndexX(resultRight);
		BottomIndex = TileMap->GetOriginTileIndexY(resultBottom);

		if (LeftIndex < 0)
			LeftIndex = 0;

		if (TopIndex < 0)
			TopIndex = 0;

		if (RightIndex >= TileMap->GetTileCountX())
			RightIndex = TileMap->GetTileCountX() - 1;

		if (BottomIndex >= TileMap->GetTileCountY())
			BottomIndex = TileMap->GetTileCountY() - 1;

		bool Check = false;

		for (int i = TopIndex; i <= BottomIndex; i++)
		{
			for (int j = LeftIndex; j <= RightIndex; j++)
			{
				// 이전위치이 Bottom이 타일의 Top보다 클 경우는 무시한다.
				if (TileMap->GetTile(j, i)->GetPos().y < PrevBottom)
					continue;

				if (TileMap->GetTile(j, i)->GetTileProperty() == ETileProperty::Wall)
				{
					Check = true;
					m_FallTime = 0.0f;
					m_Pos.y = TileMap->GetTile(j, i)->GetPos().y - (1.0f - m_Pivot.y) * m_Size.y;
					m_IsGround = true;
					m_Jump = false;
					break;
				}
			}

			if (Check)
				break;
		}

		if (!Check)
		{
			if (m_IsGround)
			{
				m_FallTime = 0.0f;
				m_FallStartY = m_Pos.y;
			}

			m_IsGround = false;
		}
	}
}

void CGameObject::Collision(float DeltaTime)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		m_Scene->GetSceneCollision()->AddCollider(*iter);
	}


	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->Collision(DeltaTime);
		}
	}
}

void CGameObject::PrevRender()
{
	CCamera* Camera = m_Scene->GetCamera();

	m_RenderPos = m_Pos - Camera->GetPos();

	Vector2 Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	Vector2 LT = m_RenderPos - m_Pivot * Size + m_Offset;

	m_CameraCull = false;

	if (LT.x + Size.x <= 0.0f)
		m_CameraCull = true;
	else if (LT.x >= Camera->GetResolution().x)
		m_CameraCull = true;
	else if (LT.y + Size.y <= 0.0f)
		m_CameraCull = true;
	else if (LT.y >= Camera->GetResolution().y)
		m_CameraCull = true;
}

// 게임 오브젝트를 렌더링 시에 애니메이션이 있는지, 또는 텍스쳐가 있는지 여부에 따라 다른 렌더링을 한다.
void CGameObject::Render(HDC hDC)
{
	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		// 현재 애니메이션의 프레임으로 프레임의 데이터를 가져온다.
		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		// 오프셋 처리를 해준다.
		Vector2 LT = m_RenderPos - m_Pivot * FrameData.Size + m_Offset;

		// 애니메이션이 한장으로 이루어졌을 시
		if (AnimInfo->Sequence->GetTextureType() == ETexture_Type::Atlas)
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT, FrameData.StartPos, FrameData.Size);
		}
		// 애니메이션이 여러장으로 만들어졌을 시
		else
		{
			AnimInfo->Sequence->GetTexture()->Render(hDC, LT, FrameData.StartPos, FrameData.Size, AnimInfo->Frame);
		}
	}
	else
	{
		Vector2	LT = m_RenderPos - m_Pivot * m_Size + m_Offset;
		if (m_Texture)
		{
			// 이미지를 이용해서 출력한다.
			m_Texture->Render(hDC, LT, m_ImageStart, m_Size);
		}

		else
		{
			Rectangle(hDC, (int)LT.x, (int)LT.y,
				(int)(LT.x + m_Size.x), (int)(LT.y + m_Size.y));
		}
	}

	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_ColliderList.erase(iter);
			iterEnd = m_ColliderList.end();
			continue;
		}
		else if ((*iter)->GetEnable())
		{
			(*iter)->Render(hDC);
		}

		iter++;
	}

	{
		auto iter = m_WidgetComponentList.begin();
		auto iterEnd = m_WidgetComponentList.end();

		for (; iter != iterEnd; iter++)
		{
			(*iter)->Render(hDC);
		}
	}

	m_PrevPos = m_Pos;
	m_Velocity = Vector2(0.f, 0.f);
}

CGameObject* CGameObject::Clone()
{
	return nullptr;
}

float CGameObject::SetDamage(float Damage)
{
	if (!m_DamageEnable)
		return 0.0f;

	return Damage;
}

float CGameObject::SetMP(float MP)
{
	return MP;
}

CCollider* CGameObject::FindCollider(const std::string& Name)
{
	auto iter = m_ColliderList.begin();
	auto iterEnd = m_ColliderList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

// 애니메이션을 생성한다.
void CGameObject::CreateAnimation()
{
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		// 이 애니메이션의 소속된 오브젝트는 this
		m_Animation->m_Owner = this;
		// 이 애니메이션이 소속된 씬은 현재 게임 오브젝트가 가지고있는 씬
		m_Animation->m_Scene = m_Scene;
	}
}

// 애니메이션을 추가한다.
void CGameObject::AddAnimation(const std::string& SequenceName, bool Loop, float PlayTime, float PlayScale, bool Reverse)
{
	// 만약 애니메이션이 없을시에는
	if (!m_Animation)
	{
		m_Animation = new CAnimation;

		m_Animation->m_Owner = this;
		m_Animation->m_Scene = m_Scene;
	}

	// 애니메이션을 추가한다.
	m_Animation->AddAnimation(SequenceName, Loop, PlayTime, PlayScale, Reverse);
}

void CGameObject::SetAniamtionPlayTime(const std::string& Name, float PlayTime)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayTime(Name, PlayTime);
}

void CGameObject::SetAnimationPlayScale(const std::string& Name, float PlayScale)
{
	if (!m_Animation)
		return;

	m_Animation->SetPlayScale(Name, PlayScale);
}

void CGameObject::SetCurrentAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->SetCurrentAnimation(Name);
}

void CGameObject::ChangeAnimation(const std::string& Name)
{
	if (!m_Animation)
		return;

	m_Animation->ChangeAnimation(Name);
}

bool CGameObject::CheckCurrentAnimation(const std::string& Name)
{
	return m_Animation->CheckCurrentAnimation(Name);
}

void CGameObject::SetAnimationReverse(const std::string& Name, bool Reverse)
{
	if (!m_Animation)
		return;

	m_Animation->SetReverse(Name, Reverse);
}

void CGameObject::SetAnimationLoop(const std::string& Name, bool Loop)
{
	if (!m_Animation)
		return;

	m_Animation->SetLoop(Name, Loop);
}

float CGameObject::GetBottom() const
{
	Vector2 Size = m_Size;

	if (m_Animation)
	{
		AnimationInfo* AnimInfo = m_Animation->m_CurrentAnimation;

		const AnimationFrameData& FrameData = AnimInfo->Sequence->GetFrameData(AnimInfo->Frame);

		Size = FrameData.Size;
	}

	return m_Pos.y + (1.0f - m_Pivot.y) * Size.y + m_Offset.y;
}
