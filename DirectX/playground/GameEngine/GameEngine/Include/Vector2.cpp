#include "Vector2.h"

Vector2::Vector2() :
    x(0.0f),
    y(0.0f)
{
}

Vector2::Vector2(float _x, float _y) :
    x(_x),
    y(_y)
{
}

Vector2::Vector2(const Vector2& v) :
    x(v.x),
    y(v.y)
{
}

Vector2::Vector2(const XMVECTOR& v)
{
    // XMFLOAT2는 2개의 float변수를 가지고 있다.
    // 따라서 간단하게 변환이 가능하다.
    XMStoreFloat2((XMFLOAT2*)this, v);
}

Vector2& Vector2::operator=(const Vector2& v)
{
    x = v.x;
    y = v.y;

    return *this;
}

Vector2& Vector2::operator=(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);

    return *this;
}

Vector2& Vector2::operator=(float f)
{
    x = f;
    y = f;

    return *this;
}

bool Vector2::operator==(const Vector2& v) const
{
    return x == v.x && y == v.y;
}

bool Vector2::operator==(const XMVECTOR& v) const
{
    Vector2	v1;
    XMStoreFloat2((XMFLOAT2*)&v1, v);

    return x == v1.x && y == v1.y;
}

bool Vector2::operator!=(const Vector2& v) const
{
    return x != v.x || y != v.y;
}

bool Vector2::operator!=(const XMVECTOR& v) const
{
    Vector2	v1;
    XMStoreFloat2((XMFLOAT2*)&v1, v);

    return x != v1.x || y != v1.y;
}

float& Vector2::operator[](int index)
{
    if (index < 0 || index > 1)
    {
        assert(false);
    }

    if (index == 0)
        return x;

    return y;
}

Vector2 Vector2::operator+(const Vector2& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector2(v1 + v2);
}

Vector2 Vector2::operator+(const XMVECTOR& v) const
{
    return Vector2(Convert() + v);
}

Vector2 Vector2::operator+(const float f) const
{
    return Vector2(x + f, y + f);
}

void Vector2::operator+=(const Vector2& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 + v2);
}

void Vector2::operator+=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 + v);
}

void Vector2::operator+=(const float f)
{
    x += f;
    y += f;
}

Vector2 Vector2::operator-(const Vector2& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector2(v1 - v2);
}

Vector2 Vector2::operator-(const XMVECTOR& v) const
{
    return Vector2(Convert() - v);
}

Vector2 Vector2::operator-(const float f) const
{
    return Vector2(x - f, y - f);
}

void Vector2::operator-=(const Vector2& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 - v2);
}

void Vector2::operator-=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 - v);
}

void Vector2::operator-=(const float f)
{
    x -= f;
    y -= f;
}

Vector2 Vector2::operator*(const Vector2& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector2(v1 * v2);
}

Vector2 Vector2::operator*(const XMVECTOR& v) const
{
    return Vector2(Convert() * v);
}

Vector2 Vector2::operator*(const float f) const
{
    return Vector2(x * f, y * f);
}

void Vector2::operator*=(const Vector2& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 * v2);
}

void Vector2::operator*=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 * v);
}

void Vector2::operator*=(const float f)
{
    x *= f;
    y *= f;
}

Vector2 Vector2::operator/(const Vector2& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector2(v1 / v2);
}

Vector2 Vector2::operator/(const XMVECTOR& v) const
{
    return Vector2(Convert() / v);
}

Vector2 Vector2::operator/(const float f) const
{
    return Vector2(x / f, y / f);
}

void Vector2::operator/=(const Vector2& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 / v2);
}

void Vector2::operator/=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 / v);
}

void Vector2::operator/=(const float f)
{
    x /= f;
    y /= f;
}

const Vector2& Vector2::operator++()
{
    x += 1.0f;
    y += 1.0f;

    return *this;
}

const Vector2& Vector2::operator++(int)
{
    x += 1.0f;
    y += 1.0f;

    return *this;
}

Vector2 Vector2::operator--()
{
    x -= 1.0f;
    y -= 1.0f;

    return *this;
}

Vector2 Vector2::operator--(int)
{
    x -= 1.0f;
    y -= 1.0f;

    return *this;
}

float Vector2::Length() const
{
    // X에 길이 값을 넣어서 반환한다. 따라서 X값을 가져온다.
    return XMVectorGetX(XMVector2Length(Convert()));
}

float Vector2::Distance(const Vector2& v) const
{
    Vector2	Dist = v - *this;

    return Dist.Length();
}

void Vector2::Normalize()
{
    *this = XMVector2Normalize(Convert());
}

float Vector2::Dot(const Vector2& v) const
{
    // 마찬가지로 계산결과가 X에 들어온다.
    return XMVectorGetX(XMVector2Dot(Convert(), v.Convert()));
}

XMVECTOR Vector2::Convert() const
{
    return XMLoadFloat2((XMFLOAT2*)this);
}

void Vector2::Convert(const XMVECTOR& v)
{
    XMStoreFloat2((XMFLOAT2*)this, v);
}
