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

// �� �ȿ��� �浹�˻縦 �ϱ����ؼ� �浹ü�� �߰��Ѵ�.
void CSceneCollision::AddCollider(CCollider* Collider)
{
	m_vecCollider.push_back(Collider);
}

// �� �ȿ��� UI �浹�� �����ϱ����� �߰�
void CSceneCollision::AddUIWindow(CUIWindow* Window)
{
	m_vecUIWindow.push_back(Window);
}

// �߰��� �浹ü�� ��� ��ȸ�ϸ鼭 �浹�� �˻��Ѵ�.
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

			// ���� ���濡 ���� State�� ���´�.
			ECollision_State SrcState = SrcProfile->vecState[DestProfile->Channel];
			ECollision_State DestState = DestProfile->vecState[SrcProfile->Channel];

			if (SrcState == ECollision_State::Ignore || DestState == ECollision_State::Ignore)
				continue;

			if (Src->Collision(Dest))
			{
				// ������ �浹�ǰ� �־������� �Ǵ��Ѵ�.
				if (!Src->CheckCollisionList(Dest))
				{
					// ���� ������ �浹��Ͽ� �����Ѵ�.
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
	// ���� ī�޶� �ְ� ��ü ������ ũ�Ⱑ ���� ���̴� �������� Ŀ����.
	// ���� ���� ���콺 ��ġ���� ī�޶��� ��ġ�� ���������� �����ش�.
	Vector2 MouseWorldPos = MousePos + m_Scene->GetCamera()->GetPos();

	// ���� ���콺�� UI�� �浹ó���Ѵ�.
	int WidgetCount = 0;

	// ���� ��� �������� ���� ����.
	size_t WindowCount = m_vecUIWindow.size();

	// ��� �������� ����.
	for (size_t i = 0; i < WindowCount; i++)
	{
		WidgetCount += m_vecUIWindow[i]->GetWidgetCount();
	}

	std::vector<CUIWidget*> vecWidget;
	vecWidget.resize(WidgetCount);

	int WidgetOffset = 0;

	// ���� ���� ���õ� ������ �ִٸ�.
	if (m_SelectWidget)
		vecWidget[WidgetOffset++] = m_SelectWidget;
	
	// ���ĵ� �����쿡�� ���ĵ� ������ �����´�. ���� ������ ���ĵ� ������ ���´�.
	// ���� �����ִ� �����쿡�� ���ĵ� ������ �����´�.
	for (size_t i = 0; i < WindowCount; i++)
	{
		// ���� �����쿡 ���Ե� �������� ��
		int Count = m_vecUIWindow[i]->GetWidgetCount();

		for (int j = 0; j < Count; j++)
		{
			CUIWidget* Widget = m_vecUIWindow[i]->GetWidget(j);

			// ���� ���� ������ ���� ���õ� ������ �ٸ��� ����.
			if (Widget != m_SelectWidget)
				vecWidget[WidgetOffset++] = Widget;
			
		}
	}

	// ���콺�� UI(����)�� �浹�� �����Ѵ�.
	// ���� �̶� �ϳ��� �浹�̵ȴٸ� �浹�� �ٷ� ���߰� ������Ʈ �浹�� ���� �ʴ´�.
	bool EnableCollision = false;

	for (int i = 0; i < WidgetCount; i++)
	{
		if (vecWidget[i]->CollisionMouse(MousePos, DeltaTime))
		{
			// ���� ���� ���콺�� �ö� �ִ� ������ �����ϰ� �ٲ�ٸ� ������ ������ ������ �Լ��� ȣ���Ѵ�.
			if (m_MouseHoverdWidget && m_MouseHoverdWidget != vecWidget[i])
				m_MouseHoverdWidget->CollisionMouseReleaseCallback(DeltaTime);

			// ���� UI�� �浹������ ������Ʈ�� �浹ü�� �ִٸ� �����ش�.
			if (m_MouseCollision)
			{
				m_MouseCollision->SetMouseCollision(false);
				m_MouseCollision = nullptr;
			}

			// �װ� �ƴ϶�� ȣ���� ������Ʈ�� �ٲٰ� �浹�� ������.

			m_MouseHoverdWidget = vecWidget[i];

			EnableCollision = true;

			break;
		}
	}

	// ���콺�� ���忡 ��ġ�� ������Ʈ�� �浹�� �����Ѵ�.
	// �̶� UI�� �浹�� �Ǿ��ִٸ� �� �ʿ�� ����.
	if (!EnableCollision)
	{
		// ���� ���� ȣ���� ������ �־��ٸ� Ǯ���ش�.
		if (m_MouseHoverdWidget)
		{
			m_MouseHoverdWidget->CollisionMouseReleaseCallback(DeltaTime);
			m_MouseHoverdWidget = nullptr;
		}

		// ���� ���õ� ������Ʈ�� Ǯ���ش�
		if (m_SelectWidget)
			m_SelectWidget = nullptr;

		// ������� �浹ü�� ���콺���� �浹ó���� �Ѵ�.
		// �浹ü�� ȭ�鿡 ������ �浹ü�鸸 �ɷ����� �浹ü�� �ٴ� ��������
		// ������ �����ش�.

		size_t Size = m_vecCollider.size();

		if (Size > 1)
		{
			qsort(&m_vecCollider[0], (size_t)Size, sizeof(CCollider*), CSceneCollision::SortY);


			bool MouseCollision = false;

			
			for (size_t i = 0; i < Size; i++)
			{	
				// ���� �浹�ߴٸ�. (���콺 ���� ��������)
				if (m_vecCollider[i]->CollisionMouse(MouseWorldPos))
				{
					// ���� ���� �浹�� �����ְ� ���� �浹ü�� �ٸ��ٸ� �浹�� �������� ������ش�.
					if (m_MouseCollision && m_MouseCollision != m_vecCollider[i])
						m_MouseCollision->SetMouseCollision(false);
					
					// ���Ӱ� �浹ü�� �����Ѵ�.
					m_MouseCollision = m_vecCollider[i];
					m_MouseCollision->SetMouseCollision(true);

					MouseCollision = true;
				}
			}

			// ���� ���콺 ������Ʈ �浹�� �Ͼ�� �ʾҴٸ�
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
