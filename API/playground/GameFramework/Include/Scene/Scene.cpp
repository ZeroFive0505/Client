
#include "Scene.h"
#include "SceneResource.h"
#include "SceneCollision.h"
#include "Camera.h"
#include "../Map/MapBase.h"

// 씬이 생성될때는 씬 리소스도 같이 생성이된다.
CScene::CScene()
{
	// 텍스쳐와 같은 리소스
	m_Resource = new CSceneResource;

	// 충돌체 리소스
	m_Collision = new CSceneCollision;
	m_Collision->m_Scene = this;

	// 화면에 렌더될 오브젝트 배열
	m_RenderCount = 0;
	m_RenderCapacity = 100;
	m_RenderArray = new CGameObject * [m_RenderCapacity];

	// 화면에 표시될 UI 
	m_UICount = 0;
	m_UICapacity = 10;
	m_UIArray = new CUIWindow * [m_UICapacity];
	
	m_Camera = new CCamera;

	m_Camera->Init();
}

// 소멸시에는 씬 리소스도 같이 소멸이된다.
CScene::~CScene()
{
	SAFE_DELETE(m_Camera);
	SAFE_DELETE_ARRAY(m_RenderArray);

	for (int i = 0; i < m_UICount; i++)
	{
		SAFE_RELEASE(m_UIArray[i]);
	}

	SAFE_DELETE_ARRAY(m_UIArray);

	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd; iter++)
		{
			SAFE_DELETE((*iter));
		}

		m_MapList.clear();
	}


	// 씬 리소스를 없애기전에 씬에 속해있는 오브젝트들을 없앤다.
	m_ObjList.clear();

	m_mapPrototype.clear();

	m_Player = nullptr;

	SAFE_DELETE(m_Resource);
	SAFE_DELETE(m_Collision);
}

// 해당 씬의 리소스들을 참조하기 위해서 이용한다.
CSceneResource* CScene::GetSceneResource() const
{
	return m_Resource;
}

CSceneCollision* CScene::GetSceneCollision() const
{
	return m_Collision;
}

CCamera* CScene::GetCamera() const
{
	return m_Camera;
}

CGameObject* CScene::FindObject(const std::string& Name)
{
	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == Name)
			return *iter;
	}

	return nullptr;
}

void CScene::SetPlayer(const std::string& Name)
{
	CGameObject* Player = FindObject(Name);

	SetPlayer(Player);
}

void CScene::SetPlayer(CGameObject* Player)
{
	m_Player = Player;

	auto iter = m_ObjList.begin();
	auto iterEnd = m_ObjList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == Player)
		{
			m_ObjList.erase(iter);
			break;
		}
	}
}

bool CScene::Init()
{
	return true;
}

// 모든 오브젝트들을 순회하면서 업데이트.
bool CScene::Update(float DeltaTime)
{
	if(m_Player)
		m_Player->Update(DeltaTime * m_Player->m_TimeScale);

	{
		auto	iter = m_ObjList.begin();
		auto	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			// 만약 레퍼런스 카운트를 해서 아무도 참조하고있지 않다면 없앤다.
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Update(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			// 만약 활성화된 윈도우가 아니라면 지운다.
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}
			// 만약 보이지 않는 윈도우라면 건너뛴다.
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			if(m_Player)
				m_UIArray[i]->Update(DeltaTime * m_Player->m_TimeScale);
			else
				m_UIArray[i]->Update(DeltaTime);
			i++;
		}
	}

	return false;
}

// 모든 오브젝트를 순회하면서 PostUpdate
bool CScene::PostUpdate(float DeltaTime)
{
	if(m_Player)
		m_Player->PostUpdate(DeltaTime * m_Player->m_TimeScale);

	{
		std::list<CSharedPtr<CGameObject>>::iterator	iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PostUpdate(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}
	}


	{
		for (int i = 0; i < m_UICount;)
		{
			// 만약 활성화된 윈도우가 아니라면 지운다.
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}
			// 만약 보이지 않는 윈도우라면 건너뛴다.
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			if (m_Player)
				m_UIArray[i]->PostUpdate(DeltaTime * m_Player->m_TimeScale);
			else
				m_UIArray[i]->PostUpdate(DeltaTime);
			i++;
		}
	}

	if (m_Player)
		m_Camera->Update(DeltaTime * m_Player->m_TimeScale);
	else
		m_Camera->Update(DeltaTime);

	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}
			else if (!(*iter)->IsEnable())
			{
				iter++;
				continue;
			}

			(*iter)->Update(DeltaTime);
			iter++;
		}
	}


	{
		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}
			else if (!(*iter)->IsEnable())
			{
				iter++;
				continue;
			}

			(*iter)->PostUpdate(DeltaTime);
			iter++;
		}
	}

	return false;
}

bool CScene::Collision(float DeltaTime)
{
	if(m_Player)
		m_Player->Collision(DeltaTime * m_Player->m_TimeScale);

	{
		std::list<CSharedPtr<CGameObject>>::iterator	iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Collision(DeltaTime * (*iter)->m_TimeScale);
			++iter;
		}

	}

	// UI 충돌을 한다.
	{
		// 윈도우수가 2개 이상이라면 Z값에 따라 정렬한다.
		// 이떄 Z값에 따라 내림차순으로 정렬한다.
		if (m_UICount >= 2)
		{
			qsort(m_UIArray, (size_t)m_UICount, sizeof(CUIWindow*), CScene::SortZOrder);
		}

		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			// 현재 윈도우에 속해있는 위젯을 Z값 순서대로 정렬한다.
			m_UIArray[i]->Collision(DeltaTime);

			// 윈도우 정렬 그리고 위젯 정렬을 끝낸후에 충돌체 리스트에 추가한다.
			m_Collision->AddUIWindow(m_UIArray[i]);

			i++;
		}
	}

	m_Collision->CollisionMouse(DeltaTime);

	m_Collision->Collision(DeltaTime);

	return false;
}

bool CScene::Render(HDC hDC)
{
	{

		// ZOrder의 값에따라 정렬
		if (m_MapList.size() >= 2)
		{
			qsort(&m_MapList[0], (size_t)m_MapList.size(), sizeof(CMapBase*), CScene::SortZOrderMap);
		}

		auto iter = m_MapList.begin();
		auto iterEnd = m_MapList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				SAFE_DELETE((*iter));
				iter = m_MapList.erase(iter);
				iterEnd = m_MapList.end();
				continue;
			}
			else if (!(*iter)->IsEnable())
			{
				iter++;
				continue;
			}

			(*iter)->Render(hDC);
			iter++;
		}
	}



	if(m_Player)
		m_Player->PrevRender();

	{
		std::list<CSharedPtr<CGameObject>>::iterator	iter = m_ObjList.begin();
		std::list<CSharedPtr<CGameObject>>::iterator	iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PrevRender();


			if (!(*iter)->IsCull())
			{
				if (m_RenderCount == m_RenderCapacity)
				{
					m_RenderCapacity *= 2;

					CGameObject** Array = new CGameObject * [m_RenderCapacity];

					memcpy(Array, m_RenderArray, sizeof(CGameObject*) * m_RenderCount);

					delete[] m_RenderArray;

					m_RenderArray = Array;
				}

				m_RenderArray[m_RenderCount++] = *iter;
			}

			++iter;
		}
	}

	if (m_RenderCount == m_RenderCapacity)
	{
		m_RenderCapacity *= 2;

		CGameObject** Array = new CGameObject * [m_RenderCapacity];

		memcpy(Array, m_RenderArray, sizeof(CGameObject*) * m_RenderCount);

		delete[] m_RenderArray;

		m_RenderArray = Array;
	}

	
	if (m_Player && !m_Player->IsCull())
	{
		m_RenderArray[m_RenderCount++] = m_Player;
	}

	// Y높낮이로 정렬한다.
	qsort(m_RenderArray, (size_t)m_RenderCount, sizeof(CGameObject*), CScene::SortY);

	for (int i = 0; i < m_RenderCount; i++)
	{
		m_RenderArray[i]->Render(hDC);
	}

	m_RenderCount = 0;
	
	

	// UI 렌더.
	{
		// 먼저 활성화된 UI만 출력하기위해 걸러낸다.
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j + 1];
				}

				continue;
			}

			i++;
		}

		// Z값에 따라 내림차순으로 정렬했다. 따라서 출력할때는
		// 반대로 출력하여 가장 Z값이 큰 UI가 나머지를 덮게 만든다.
		for (int i = m_UICount - 1; i >= 0;)
		{
			if (!m_UIArray[i]->GetVisibility())
			{
				i--;
				continue;
			}

			m_UIArray[i]->Render(hDC);
			i--;
		}
	}


	return false;
}

int CScene::SortY(const void* Src, const void* Dest)
{
	CGameObject* SrcObj = *(CGameObject**)Src;
	CGameObject* DestObj = *(CGameObject**)Dest;

	// Bottom 값을 구한다.
	float SrcY = SrcObj->GetBottom();
	float DestY = DestObj->GetBottom();

	if (SrcY < DestY)
		return -1;
	else if (SrcY > DestY)
		return 1;

	return 0;
}

int CScene::SortZOrder(const void* Src, const void* Dest)
{
	CUIWindow* SrcObj = *(CUIWindow**)Src;
	CUIWindow* DestObj = *(CUIWindow**)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return -1;

	else if (SrcZ < DestZ)
		return 1;

	return 0;
}

int CScene::SortZOrderMap(const void* Src, const void* Dest)
{
	CMapBase* SrcObj = *(CMapBase**)Src;
	CMapBase* DestObj = *(CMapBase**)Dest;

	int SrcZ = SrcObj->GetZOrder();
	int DestZ = DestObj->GetZOrder();

	if (SrcZ > DestZ)
		return 1;

	else if (SrcZ < DestZ)
		return -1;

	return 0;
}

CGameObject* CScene::FindPrototype(const std::string& Name)
{
	auto iter = m_mapPrototype.find(Name);

	if (iter == m_mapPrototype.end())
		return nullptr;

	return iter->second;
}
