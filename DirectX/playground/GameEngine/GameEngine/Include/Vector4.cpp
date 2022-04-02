#include "Vector4.h"

Vector4 Vector4::Black(0.0f, 0.0f, 0.0f, 1.0f);
Vector4 Vector4::White(1.0f, 1.0f, 1.0f, 1.0f);
Vector4 Vector4::Red(1.0f, 0.0f, 0.0f, 1.0f);
Vector4 Vector4::Green(0.0f, 1.0f, 0.0f, 1.0f);
Vector4 Vector4::Blue(0.0f, 0.0f, 1.0f, 1.0f);

Vector4::Vector4() :
    x(0.0f),
    y(0.0f),
    z(0.0f),
    w(0.0f)
{
}

Vector4::Vector4(float _x, float _y, float _z, float _w) :
    x(_x),
    y(_y),
    z(_z),
    w(_w)
{
}

Vector4::Vector4(const Vector4& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w)
{
}

Vector4::Vector4(const XMVECTOR& v)
{
    XMStoreFloat4((XMFLOAT4*)this, v);
}

Vector4& Vector4::operator=(const Vector4& v)
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

Vector4& Vector4::operator=(const XMVECTOR& v)
{
    XMStoreFloat4((XMFLOAT4*)this, v);

    return *this;
}

Vector4& Vector4::operator=(float f)
{
    x = f;
    y = f;
    z = f;
    w = f;

    return *this;
}

bool Vector4::operator==(const Vector4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}

bool Vector4::operator==(const XMVECTOR& v) const
{
    Vector4	v1;
    XMStoreFloat4((XMFLOAT4*)&v1, v);

    return x == v1.x && y == v1.y && z == v1.z && w == v1.w;
}

bool Vector4::operator!=(const Vector4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}

bool Vector4::operator!=(const XMVECTOR& v) const
{
    Vector4	v1;
    XMStoreFloat4((XMFLOAT4*)&v1, v);

    return x != v1.x || y != v1.y || z != v1.z || w != v1.w;
}

float& Vector4::operator[](int index)
{
    if (index < 0 || index > 3)
    {
        assert(false);
    }

    if (index == 0)
        return x;

    else if (index == 1)
        return y;

    // 상수를 왼쪽에 배치해서
    // 실수를 막는다.
    // 상수의 경우 수정가능한 lvalue가 아님
    else if (2 == index)
        return z;

    return w;
}

Vector4 Vector4::operator+(const Vector4& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector4(v1 + v2);
}

Vector4 Vector4::operator+(const XMVECTOR& v) const
{
    return Vector4(Convert() + v);
}

Vector4 Vector4::operator+(const float f) const
{
    return Vector4(x + f, y + f, z + f, w + f);
}

void Vector4::operator+=(const Vector4& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 + v2);
}

void Vector4::operator+=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 + v);
}

void Vector4::operator+=(const float f)
{
    x += f;
    y += f;
    z += f;
    w += f;
}

Vector4 Vector4::operator-(const Vector4& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector4(v1 - v2);
}

Vector4 Vector4::operator-(const XMVECTOR& v) const
{
    return Vector4(Convert() - v);
}

Vector4 Vector4::operator-(const float f) const
{
    return Vector4(x - f, y - f, z - f, w - f);
}

void Vector4::operator-=(const Vector4& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 - v2);
}

void Vector4::operator-=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 - v);
}

void Vector4::operator-=(const float f)
{
    x -= f;
    y -= f;
    z -= f;
    w -= f;
}

Vector4 Vector4::operator*(const Vector4& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector4(v1 * v2);
}

Vector4 Vector4::operator*(const XMVECTOR& v) const
{
    return Vector4(Convert() * v);
}

Vector4 Vector4::operator*(const float f) const
{
    return Vector4(x * f, y * f, z * f, w * f);
}

void Vector4::operator*=(const Vector4& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 * v2);
}

void Vector4::operator*=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 * v);
}

void Vector4::operator*=(const float f)
{
    x *= f;
    y *= f;
    z *= f;
    w *= f;
}

Vector4 Vector4::operator/(const Vector4& v) const
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    return Vector4(v1 / v2);
}

Vector4 Vector4::operator/(const XMVECTOR& v) const
{
    return Vector4(Convert() / v);
}

Vector4 Vector4::operator/(const float f) const
{
    return Vector4(x / f, y / f, z / f, w / f);
}

void Vector4::operator/=(const Vector4& v)
{
    XMVECTOR	v1 = Convert();
    XMVECTOR	v2 = v.Convert();

    Convert(v1 / v2);
}

void Vector4::operator/=(const XMVECTOR& v)
{
    XMVECTOR	v1 = Convert();

    Convert(v1 / v);
}

void Vector4::operator/=(const float f)
{
    x /= f;
    y /= f;
    z /= f;
    w /= f;
}

const Vector4& Vector4::operator++()
{
    x += 1.0f;
    y += 1.0f;
    z += 1.0f;
    w += 1.0f;

    return *this;
}

const Vector4& Vector4::operator++(int)
{
    x += 1.0f;
    y += 1.0f;
    z += 1.0f;
    w += 1.0f;

    return *this;
}

const Vector4& Vector4::operator--()
{
    x -= 1.0f;
    y -= 1.0f;
    z -= 1.0f;
    w -= 1.0f;

    return *this;
}

const Vector4& Vector4::operator--(int)
{
    x -= 1.0f;
    y -= 1.0f;
    z -= 1.0f;
    w -= 1.0f;

    return *this;
}

XMVECTOR Vector4::Convert() const
{
    return XMLoadFloat4((XMFLOAT4*)this);
}

void Vector4::Convert(const XMVECTOR& v)
{
    XMStoreFloat4((XMFLOAT4*)this, v);
}
