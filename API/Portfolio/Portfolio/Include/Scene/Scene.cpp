#include "Scene.h"
#include "SceneResource.h"
#include "Camera.h"
#include "../Input.h"
#include "../Map/MapBase.h"

CScene::CScene()
{
	m_Resouece = new CSceneResource;
	
	m_UICount = 0;
	m_UICapacity = 10;
	m_UIArray = new CUIWindow * [m_UICapacity];

	m_Camera = new CCamera;

	m_Camera->Init();


	m_TileMap = nullptr;
}

CScene::~CScene()
{
	SAFE_DELETE(m_Camera);

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
	}

	m_ObjList.clear();

	SAFE_DELETE(m_Resouece);

}


bool CScene::Init()
{
	return true;
}

bool CScene::Update(float deltaTime)
{
	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Update(deltaTime);
			iter++;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j];
				}

				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			m_UIArray[i]->Update(deltaTime);
			i++;
		}
	}

	return false;
}

bool CScene::PostUpdate(float deltaTime)
{
	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->PostUpdate(deltaTime);
			iter++;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j];
				}

				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			m_UIArray[i]->PostUpdate(deltaTime);
			i++;
		}
	}

	m_Camera->Update(deltaTime);

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

			(*iter)->Update(deltaTime);
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

			(*iter)->PostUpdate(deltaTime);
			iter++;
		}
	}

	return false;
}

bool CScene::Collision(float deltaTime)
{

	{
		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Collision(deltaTime);
			iter++;
		}
	}

	return false;
}

bool CScene::Render(HDC hDC)
{
	{
		if (m_MapList.size() >= 2)
		{
			qsort(&m_MapList[0], (size_t)m_MapList.size(), sizeof(CMapBase*), CScene::SortZ);
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


	{

		if (m_ObjList.size() >= 2)
		{
			m_ObjList.sort([](const CSharedPtr<CGameObject> lhs, const CSharedPtr<CGameObject> rhs)
			{
				return lhs->GetBottom() < rhs->GetBottom();
			});
		}

		auto iter = m_ObjList.begin();
		auto iterEnd = m_ObjList.end();

		for (; iter != iterEnd;)
		{
			if (!(*iter)->IsActive())
			{
				iter = m_ObjList.erase(iter);
				iterEnd = m_ObjList.end();
				continue;
			}

			(*iter)->Render(hDC);
			iter++;
		}
	}

	{
		for (int i = 0; i < m_UICount;)
		{
			if (!m_UIArray[i]->IsActive())
			{
				m_UICount--;

				for (int j = i; j < m_UICount; j++)
				{
					m_UIArray[j] = m_UIArray[j];
				}

				continue;
			}
			else if (!m_UIArray[i]->GetVisibility())
			{
				i++;
				continue;
			}

			m_UIArray[i]->Render(hDC);
			i++;
		}
	}

	return false;
}

int CScene::SortY(const void* src, const void* dest)
{
	CGameObject* srcObj = *(CGameObject**)src;
	CGameObject* destObj = *(CGameObject**)dest;

	float srcY = srcObj->GetBottom();
	float destY = destObj->GetBottom();

	if (srcY < destY)
		return -1;
	else if (srcY > destY)
		return 1;

	return 0;
}

int CScene::SortZ(const void* src, const void* dest)
{
	CMapBase* srcObj = *(CMapBase**)src;
	CMapBase* destObj = *(CMapBase**)dest;

	int srcZ = srcObj->GetZOrder();
	int destZ = destObj->GetZOrder();

	if (srcZ > destZ)
		return 1;
	else if (srcZ < destZ)
		return -1;

	return 0;
}
