#include "Collision.h"
#include "ColliderBox.h"
#include "ColliderSphere.h"

bool CCollision::CollisionBoxToBox(CColliderBox* Src, CColliderBox* Dest)
{
    if (CollisionBoxToBox(Src->GetInfo(), Dest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionBoxToBox(const RectInfo& Src, const RectInfo& Dest)
{
    if (Src.Left > Dest.Right)
        return false;
    else if (Dest.Left > Src.Right)
        return false;
    else if (Src.Top > Dest.Bottom)
        return false;
    else if (Dest.Top > Src.Bottom)
        return false;

    return true;
}

bool CCollision::CollisionSphereToSphere(CColliderSphere* Src, CColliderSphere* Dest)
{
    if (CollisionSphereToSphere(Src->GetInfo(), Dest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionSphereToSphere(const SphereInfo& Src, const SphereInfo& Dest)
{
    float Dist = Distance(Src.Center, Dest.Center);

    return Dist <= Src.Radius + Dest.Radius;
}

bool CCollision::CollisionBoxToSphere(CColliderBox* Src, CColliderSphere* Dest)
{

    if (CollisionBoxToSphere(Src->GetInfo(), Dest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionBoxToSphere(const RectInfo& Src, const SphereInfo& Dest)
{
    float nx = max(Src.Left, min(Src.Right, Dest.Center.x));
    float ny = max(Src.Top, min(Src.Bottom, Dest.Center.y));

    Vector2 vNearestPoint(nx, ny);
    
    float dist = Distance(vNearestPoint, Dest.Center);

    if (dist <= Dest.Radius)
        return true;
    else
        return false;
}

bool CCollision::CollisionBoxToSphere2(CColliderBox* Src, CColliderSphere* Dest)
{
    if (CollisionBoxToSphere2(Src->GetInfo(), Dest->GetInfo()))
    {
        return true;
    }

    return false;
}

bool CCollision::CollisionBoxToSphere2(const RectInfo& Src, const SphereInfo& Dest)
{

    if ((Dest.Center.x >= Src.Left && Dest.Center.x <= Src.Right) ||
        (Dest.Center.y >= Src.Top && Dest.Center.y <= Src.Bottom))
    {
        RectInfo Padding = Src;
        
        Padding.Left -= Dest.Radius;
        Padding.Right += Dest.Radius;
        Padding.Top -= Dest.Radius;
        Padding.Bottom += Dest.Radius;

        if (Dest.Center.x < Padding.Left)
            return false;
        else if (Dest.Center.x > Padding.Right)
            return false;
        else if (Dest.Center.y < Padding.Top)
            return false;
        else if (Dest.Center.y > Padding.Bottom)
            return false;

        return true;
    }

    Vector2 points[4];

    points[0] = Vector2(Src.Left, Src.Top);
    points[1] = Vector2(Src.Right, Src.Top);
    points[2] = Vector2(Src.Left, Src.Bottom);
    points[3] = Vector2(Src.Right, Src.Bottom);


    for (int i = 0; i < 4; i++)
    {
        if (Distance(Dest.Center, points[i]) <= Dest.Radius)
            return true;
    }


    return false;
}
