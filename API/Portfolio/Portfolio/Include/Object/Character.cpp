#include "Character.h"

CCharacter::CCharacter()
{
}

CCharacter::CCharacter(const CCharacter& obj) :
    CGameObject(obj)
{
}

CCharacter::~CCharacter()
{
}

void CCharacter::Start()
{
    CGameObject::Start();
}

bool CCharacter::Init()
{
    if (!CGameObject::Init())
        return false;

    return true;
}

void CCharacter::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);
}

void CCharacter::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

void CCharacter::Collision(float deltaTime)
{
    CGameObject::Collision(deltaTime);
}

void CCharacter::Render(HDC hDC)
{
    CGameObject::Render(hDC);
}

CCharacter* CCharacter::Clone()
{
    return nullptr;
}
