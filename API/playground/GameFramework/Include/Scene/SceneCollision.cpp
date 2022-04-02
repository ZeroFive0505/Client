#include "SceneCollision.h"
#include "../Collision//Collider.h"
#include "../UI/UIWindow.h"
#include "../UI/UIWidget.h"
#include "../Input.h"
#include "Camera.h"
#include "Scene.h"

CSceneCollision::CSceneCollision()
{
	m_vecCollider.reserve(500);
	m_vecUIWindow.reserve(10);
	m_SelectWidget = nullptr;
	m_MouseHoverdWidget = nullptr;
	m_MouseCollision = nullptr;
}

CSceneCollision::~CSceneCollision()
{
}

// 씬 안에서 충돌검사를 하기위해서 충돌체를 추가한다.
void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);
}

// 씬 안에서 UI 충돌을 감지하기위해 추가
void CSceneCollision::AddUIWindow(CUIWindow* Window)
{
	m_vecUIWindow.push_back(Window);
}

// 추가된 충돌체를 모두 순회하면서 충돌을 검사한다.
void CSceneCollision::Collision(float DeltaTime)
{
	size_t Size = m_vecCollider.size();

	if (Size <= 1)
	{
		m_vecCollider.clear();
		return;
	}

	for (size_t i = 0; i < Size - 1; i++)
	{
		CCollider* Src = m_vecCollider[i];

		for (size_t j = i + 1; j < Size; j++)
		{
			CCollider* Dest = m_vecCollider[j];

			if (Src->GetOwner() == Dest->GetOwner())
				continue;

			CollisionProfile* SrcProfile = Src->GetProfile();
			CollisionProfile* DestProfile = Dest->GetProfile();

			// 서로 상대방에 대한 State를 얻어온다.
			ECollision_State SrcState = SrcProfile->vecState[DestProfile->Channel];
			ECollision_State DestState = DestProfile->vecState[SrcProfile->Channel];

			if (SrcState == ECollision_State::Ignore || DestState == ECollision_State::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// 이전에 충돌되고 있었는지를 판단한다.
				if (!Src->CheckCollisionList(Dest))
				{
					// 서로 상대방을 충돌목록에 지정한다.
					Src->AddCollisionList(Dest);
					Dest->AddCollisionList(Src);

					Src->CallCollisionBegin(Dest, DeltaTime);
					Dest->CallCollisionBegin(Src, DeltaTime);
				}
			}
			else if(Src->CheckCollisionList(Dest))
			{
				Src->DeleteCollisionList(Dest);
				Dest->DeleteCollisionList(Src);

				Src->CallCollisionEnd(Dest, DeltaTime);
				Dest->CallCollisionEnd(Src, DeltaTime);
			}
		}
	}

	m_vecCollider.clear();
}

void CSceneCollision::CollisionMouse(float DeltaTime)
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();
	// 이제 카메라가 있고 전체 월드의 크기가 현재 보이는 공간보다 커졌다.
	// 따라서 현재 마우스 위치에서 카메라의 위치를 오프셋으로 더해준다.
	Vector2 MouseWorldPos = MousePos + m_Scene->GetCamera()->GetPos();

	// 먼저 마우스와 UI를 충돌처리한다.
	int WidgetCount = 0;

	// 먼저 모든 윈도우의 수를 센다.
	size_t WindowCount = m_vecUIWindow.size();

	// 모든 위젯수를 센다.
	for (size_t i = 0; i < WindowCount; i++)
	{
		WidgetCount += m_vecUIWindow[i]->GetWidgetCount();
	}

	std::vector<CUIWidget*> vecWidget;
	vecWidget.resize(WidgetCount);

	int WidgetOffset = 0;

	// 만약 현재 선택된 위젯이 있다면.
	if (m_SelectWidget)
		vecWidget[WidgetOffset++] = m_SelectWidget;
	
	// 정렬된 윈도우에서 정렬된 위젯을 꺼내온다. 따라서 위젯도 정렬된 순서로 들어온다.
	// 가장 위에있는 윈도우에서 정렬된 위젯을 꺼내온다.
	for (size_t i = 0; i < WindowCount; i++)
	{
		// 현재 윈도우에 포함된 위젯들의 수
		int Count = m_vecUIWindow[i]->GetWidgetCount();

		for (int j = 0; j < Count; j++)
		{
			CUIWidget* Widget = m_vecUIWindow[i]->GetWidget(j);

			// 만약 현재 위젯이 지금 선택된 위젯이 다르면 선택.
			if (Widget != m_SelectWidget)
				vecWidget[WidgetOffset++] = Widget;
			
		}
	}

	// 마우스와 UI(위젯)의 충돌을 시작한다.
	// 만약 이때 하나라도 충돌이된다면 충돌을 바로 멈추고 오브젝트 충돌도 하지 않는다.
	bool EnableCollision = false;

	for (int i = 0; i < WidgetCount; i++)
	{
		if (vecWidget[i]->CollisionMouse(MousePos, DeltaTime))
		{
			// 만약 지금 마우스가 올라가 있는 위젯이 존재하고 바뀐다면 기존의 위젯은 릴리즈 함수를 호출한다.
			if (m_MouseHoverdWidget && m_MouseHoverdWidget != vecWidget[i])
				m_MouseHoverdWidget->CollisionMouseReleaseCallback(DeltaTime);

			// 만약 UI와 충돌했을때 오브젝트와 충돌체가 있다면 없애준다.
			if (m_MouseCollision)
			{
				m_MouseCollision->SetMouseCollision(false);
				m_MouseCollision = nullptr;
			}

			// 그게 아니라면 호버링 오브젝트를 바꾸고 충돌을 나간다.

			m_MouseHoverdWidget = vecWidget[i];

			EnableCollision = true;

			break;
		}
	}

	// 마우스와 월드에 배치된 오브젝트와 충돌을 진행한다.
	// 이때 UI와 충돌이 되어있다면 할 필요는 없다.
	if (!EnableCollision)
	{
		// 만약 전에 호버링 위젯이 있었다면 풀어준다.
		if (m_MouseHoverdWidget)
		{
			m_MouseHoverdWidget->CollisionMouseReleaseCallback(DeltaTime);
			m_MouseHoverdWidget = nullptr;
		}

		// 또한 선택된 오브젝트도 풀어준다
		if (m_SelectWidget)
			m_SelectWidget = nullptr;

		// 월드상의 충돌체와 마우스와의 충돌처리를 한다.
		// 충돌체를 화면에 나오는 충돌체들만 걸러내고 충돌체의 바닥 기준으로
		// 정렬을 시켜준다.

		size_t Size = m_vecCollider.size();

		if (Size > 1)
		{
			qsort(&m_vecCollider[0], (size_t)Size, sizeof(CCollider*), CSceneCollision::SortY);


			bool MouseCollision = false;

			
			for (size_t i = 0; i < Size; i++)
			{	
				// 만약 충돌했다면. (마우스 월드 기준으로)
				if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				{
					// 만약 전에 충돌한 것이있고 현재 충돌체랑 다르다면 충돌을 거짓으로 만들어준다.
					if (m_MouseCollision && m_MouseCollision != m_vecCollider[i])
						m_MouseCollision->SetMouseCollision(false);
					
					// 새롭게 충돌체를 저장한다.
					m_MouseCollision = m_vecCollider[i];
					m_MouseCollision->SetMouseCollision(true);

					MouseCollision = true;
				}
			}

			// 만약 마우스 오브젝트 충돌도 일어나지 않았다면
			if (!MouseCollision)
			{
				if (m_MouseCollision)
				{
					m_MouseCollision->SetMouseCollision(false);
					m_MouseCollision = nullptr;
				}
			}

		}
	}
}

int CSceneCollision::SortY(const void* Src, const void* Dest)
{
	CCollider* SrcObj = *(CCollider**)Src;
	CCollider* DestObj = *(CCollider**)Dest;

	float SrcY = SrcObj->GetBottom();
	float DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;

	else if (SrcY > DestY)
		return 1;

	return 0;
}
