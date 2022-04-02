#include "Mover.h"
#include "Engine.h"
#include "Liquid.h"

CMover::CMover()
{
    SetTypeID<CMover>();
    m_OnMouse = false;
}

CMover::CMover(const CMover& obj) :
    CGameObject(obj)
{
    m_Sprite = (CSpriteComponent*)FindComponent("MoverSprite");
    m_Collider = (CColliderCircle*)FindComponent("Collider");
}

CMover::~CMover()
{
}

bool CMover::Init()
{
    m_Sprite = CreateComponent<CSpriteComponent>("MoverSprite");

    m_Collider = CreateComponent<CColliderCircle>("Collider");

    SetRootComponent(m_Sprite);

    m_Sprite->AddChild(m_Collider);

    CInput::GetInst()->CreateKey("MoverLeft", VK_LEFT);
    CInput::GetInst()->CreateKey("MoverRight", VK_RIGHT);
    CInput::GetInst()->CreateKey("MoverUp", VK_UP);
    CInput::GetInst()->CreateKey("MoverDown", VK_DOWN);

    sResolution rs = CEngine::GetInst()->GetResolution();

    m_ScreenSize.x = (float)rs.width;
    m_ScreenSize.y = (float)rs.height;

    m_Collider->SetInfo(Vector2(0.0f, 0.0f), 25.0f);

    m_Collider->AddCollisionMouseCallback<CMover>(Collision_State::Begin, this, &CMover::OnMouseCollisionEnter);
    m_Collider->AddCollisionMouseCallback<CMover>(Collision_State::End, this, &CMover::OnMouseCollisionExit);

    m_Sprite->SetPivot(0.5f, 0.5f, 0.0f);


    m_Sprite->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "Mover", TEXT("awesomeface.png"));

    m_Sprite->SetWorldScale(50.0f, 50.0f, 1.0f);
    

    SetWorldPos(m_ScreenSize.x / 2.0f, m_ScreenSize.y / 2.0f, 0.0f);

    m_Angle = 0.0f;
    m_AngularAcceleration = 0.0f;
    m_AngularVelocity = 0.0f;

    m_Mass = 2.0f;


    return true;
}

void CMover::Start()
{
    CGameObject::Start();

    CInput::GetInst()->SetCallback<CMover>("MoverLeft", KeyState_Push, this, &CMover::MoveLeft);
    CInput::GetInst()->SetCallback<CMover>("MoverRight", KeyState_Push, this, &CMover::MoveRight);
    CInput::GetInst()->SetCallback<CMover>("MoverUp", KeyState_Push, this, &CMover::MoveUp);
    CInput::GetInst()->SetCallback<CMover>("MoverDown", KeyState_Push, this, &CMover::MoveDown);
}

void CMover::Update(float deltaTime)
{
    CGameObject::Update(deltaTime);

    //EdgeCheck();

    if (m_OnMouse && CInput::GetInst()->GetMouseLButtonClick())
    {
        Vector2 mousePos = CInput::GetInst()->GetMouseWorld2DPos();

        SetWorldPos(mousePos.x, mousePos.y, 0.0f);
    }
}

void CMover::PostUpdate(float deltaTime)
{
    CGameObject::PostUpdate(deltaTime);
}

CMover* CMover::Clone()
{
    return new CMover(*this);
}

void CMover::ApplyForce(const Vector2& force)
{
    Vector2 accel = force / m_Mass;
    m_Acceleration = accel;
    m_Velocity += m_Acceleration;

    AddWorldPos(Vector3(0.0f, m_Velocity.y, 0.0f));
    AddWorldPos(Vector3(m_Velocity.x, 0.0f, 0.0f));

    m_Acceleration = Vector2(0.0f, 0.0f);
}

void CMover::ApplyForce(float x, float y)
{
    Vector2 accel = Vector2(x, y) / m_Mass;
    m_Acceleration = accel;
    m_Velocity += m_Acceleration;

    AddWorldPos(Vector3(0.0f, m_Velocity.y, 0.0f));
    AddWorldPos(Vector3(m_Velocity.x, 0.0f, 0.0f));

    m_Acceleration = Vector2(0.0f, 0.0f);
}

void CMover::AddAngularForceX(float force)
{
    m_AngularAcceleration += force;
    m_AngularVelocity.x += m_AngularAcceleration;

    m_AngularVelocity.x = std::fmaxf(-0.001f, std::fminf(0.001f, m_AngularVelocity.x));

    m_Angle.x += m_AngularVelocity.x;

    // AddRelativeRotationZ(m_Angle.x);
    m_AngularAcceleration = 0.0f;
}

void CMover::AddAngularForceY(float force)
{
    m_AngularAcceleration += force;
    m_AngularVelocity.y += m_AngularAcceleration;

    m_AngularVelocity.y = std::fmaxf(-0.001f, std::fminf(0.001f, m_AngularVelocity.y));

    m_Angle.y += m_AngularVelocity.y;

    // AddRelativeRotationZ(m_Angle.x);
    m_AngularAcceleration = 0.0f;
}


void CMover::EdgeCheck()
{
    if (m_Sprite->GetWorldPos().y - m_Sprite->GetWorldScale().y / 2.0f < 0.0f)
    {
        m_Velocity.y *= -0.9f;
        Vector3 currentPos = m_Sprite->GetWorldPos();
        currentPos.y = m_Sprite->GetWorldScale().y / 2.0f;

        m_Sprite->SetWorldPos(currentPos);
    }
    else if (m_Sprite->GetWorldPos().y + m_Sprite->GetWorldScale().y / 2.0f > m_ScreenSize.y)
    {
        m_Velocity.y *= -0.9f;
        Vector3 currentPos = m_Sprite->GetWorldPos();
        currentPos.y = m_ScreenSize.y - m_Sprite->GetWorldScale().y / 2.0f;

        m_Sprite->SetWorldPos(currentPos);
    }

    if (m_Sprite->GetWorldPos().x - m_Sprite->GetWorldScale().x / 2.0f < 0.0f)
    {
        m_Velocity.x *= -0.9f;

        Vector3 currentPos = m_Sprite->GetWorldPos();
        currentPos.x = m_Sprite->GetWorldScale().x / 2.0f;

        m_Sprite->SetWorldPos(currentPos);
    }
    else if (m_Sprite->GetWorldPos().x + m_Sprite->GetWorldScale().x / 2.0f > m_ScreenSize.x)
    {
        m_Velocity.x *= -0.9f;

        Vector3 currentPos = m_Sprite->GetWorldPos();
        currentPos.x = m_ScreenSize.x - m_Sprite->GetWorldScale().x / 2.0f;

        m_Sprite->SetWorldPos(currentPos);
    }
}

void CMover::MoveUp(float deltaTime)
{
    Vector3 axis = GetRelativeAxis(AXIS_Y);
    Vector2 force = Vector2(axis.x, axis.y) * 10.0f * deltaTime;
    ApplyForce(force);
}

void CMover::MoveDown(float deltaTime)
{
    Vector3 axis = GetRelativeAxis(AXIS_Y);
    Vector2 force = Vector2(axis.x, axis.y) * -10.0f * deltaTime;
    ApplyForce(force);
}

void CMover::MoveLeft(float deltaTime)
{
    AddRelativeRotationZ(0.1f);
}

void CMover::MoveRight(float deltaTime)
{
    AddRelativeRotationZ(-0.1f);
}

void CMover::OnMouseCollisionEnter(const sCollisionResult& result)
{
    m_OnMouse = true;
}

void CMover::OnMouseCollisionExit(const sCollisionResult& result)
{
    m_OnMouse = false;
}
