#include "LightManager.h"
#include "Scene.h"
#include "../GameObject/GameObject.h"

CLightManager::CLightManager()
{
}

CLightManager::~CLightManager()
{
}

void CLightManager::DeleteLight(CLightComponent* light)
{
	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	for (; iter != iterEnd; iter++)
	{
		if (*iter == light)
		{
			m_LightList.erase(iter);
			return;
		}
	}
}

void CLightManager::DeleteLight(const std::string& name)
{
	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	for (; iter != iterEnd; iter++)
	{
		if ((*iter)->GetName() == name)
		{
			m_LightList.erase(iter);
			return;
		}
	}
}

void CLightManager::Start()
{
}

void CLightManager::Init()
{
	m_GlobalLight = m_Scene->CreateGameObject<CGameObject>("GlobalLight");

	m_GlobalLightComponent = m_GlobalLight->CreateComponent<CLightComponent>("Light");

	//m_GlobalLightComponent->SetRelativeRotation(45.0f, 90.0f, 0.0f);

	//m_GlobalLightComponent->SetLightType(Light_Type::Directional);

	m_GlobalLightComponent->SetRelativePos(0.0f, 10.0f, 0.f);

	m_GlobalLightComponent->SetWorldRotationX(90.0f);

	m_GlobalLightComponent->SetLightType(Light_Type::Spot);

	m_GlobalLightComponent->SetDistance(20.0f);
	m_GlobalLightComponent->SetAttConst3(0.1f);
}

void CLightManager::Update(float deltaTime)
{
	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	m_GlobalLightComponent->SetWorldPos(Vector3(cosf(CEngine::GetInst()->GetCurrentPlayTime()) * 5.0f, 10.0f, 0.0f));

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_LightList.erase(iter);
			iterEnd = m_LightList.end();

			continue;
		}

		iter++;
	}
}

void CLightManager::PostUpdate(float deltaTime)
{
}

void CLightManager::SetShader()
{
	auto iter = m_LightList.begin();
	auto iterEnd = m_LightList.end();

	for (; iter != iterEnd; iter++)
	{
		if (!(*iter)->IsEnable())
			continue;

		(*iter)->SetShader();
	}
}

void CLightManager::Destroy()
{
	m_LightList.clear();
	m_GlobalLightComponent = nullptr;
	m_GlobalLight = nullptr;
}
