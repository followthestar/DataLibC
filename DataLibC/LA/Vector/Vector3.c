//
// Created by 18030 on 2024/12/8.
//

#include "Vector3.h"

#include <math.h>

Vector3 Vector3_Plus(const Vector3* v1, const Vector3* v2)
{
    return VEC3(v1->x + v2->x, v1->y + v2->y, v1->z + v2->z);
}

float Vector3_Dot(const Vector3* v1, const Vector3* v2)
{
    return v1->x * v2->x + v1->y * v2->y + v1->z * v2->z;
}

Vector3* Vector3_Minus(Vector3* v)
{
    v->x = -v->x;
    v->y = -v->y;
    v->z = -v->z;
    return v;
}

void Vector3_Set(Vector3* v, const Vector3 value)
{
    v->x = value.x;
    v->y = value.y;
    v->z = value.z;
}

void Vector3_Set3f(Vector3* v, const float x, const float y, const float z)
{
    v->x = x;
    v->y = y;
    v->z = z;
}

Vector3 Vector3_Subtract(const Vector3* v1, const Vector3* v2)
{
    return VEC3(v1->x - v2->x, v1->y - v2->y, v1->z - v2->z);
}

void Vector3_Normalize(Vector3* v)
{
    const float length = Vector3_Magnitude(v);
    if (length >= 1e-5f)
    {
        const float invLength = 1.0f / length;
        v->x *= invLength;
        v->y *= invLength;
        v->z *= invLength;
    }
    else
    {
        *v = VEC3_ZERO;
    }
}

float Vector3_Magnitude(const Vector3* v)
{
    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

Vector3 Vector3_Multiply(const Vector3* v, float factor)
{
    return VEC3(v->x * factor, v->y * factor, v->z * factor);
}
