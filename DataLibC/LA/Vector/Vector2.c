//
// Created by 18030 on 2024/12/8.
//

#include <math.h>
#include <stddef.h>
#include "Vector2.h"
#include "DataLibC/LA/Math/MathTool.h"

Vector2 _Vector2_Plus(const Vector2* lhs, const Vector2* rhs)
{
    return VEC2(lhs->x + rhs->x, lhs->y + rhs->y);
}

float _Vector2_Dot(const Vector2* lhs, const Vector2* rhs)
{
    return lhs->x * rhs->x + lhs->y * rhs->y;
}

float _Vector2_Magnitude(const Vector2* v)
{
    return sqrtf(v->x * v->x + v->y * v->y);
}

float _Vector2_SqrMagnitude(const Vector2* v)
{
    return v->x * v->x + v->y * v->y;
}

Vector2 _Vector2_Normalized(const Vector2* v)
{
    Vector2* result = &VEC2(v->x, v->y);
    Vector2_Normalize(result);
    return *result;
}

void Vector2_Normalize(Vector2* v)
{
    float length = _Vector2_Magnitude(v);
    if (length >= 1e-5f)
    {
        v->x /= length;
        v->y /= length;
    }
    else
    {
        *v = VEC2(0, 0);
    }
}

void _Vector2_Scale(Vector2* v, const Vector2* scale)
{
    v->x *= scale->x;
    v->y *= scale->y;
}

float _Vector2_Distance(const Vector2* a, const Vector2* b)
{
    float num = a->x - b->x;
    float num2 = a->y - b->y;
    return sqrtf(num * num + num2 * num2);
}

bool _Vector2_Equals(const Vector2* lhs, const Vector2* rhs)
{
    if (lhs == rhs)
        return true;
    if (lhs == NULL || rhs == NULL)
        return false;
    return lhs->x == rhs->x && lhs->y == rhs->y;
}

Vector2 _Vector2_Lerp(const Vector2* a, const Vector2* b, float t)
{
    t = Clamp01(t);
    return VEC2(a->x + (b->x - a->x) * t, a->y + (b->y - a->y) * t);
}

float _Vector2_Angle(const Vector2* from, const Vector2* to)
{
    float num = sqrtf(_Vector2_SqrMagnitude(from) * _Vector2_SqrMagnitude(to));
    if (num < 1e-15)
        return 0;
    float num2 = Clamp(_Vector2_Dot(from, to) / num, -1, 1);
    return acosf(num2) * 57.29578f;
}

float _Vector2_SignedAngle(const Vector2* from, const Vector2* to)
{
    float num = _Vector2_Angle(from, to);
    return copysignf(num, from->x * to->y - from->y * to->x);
}

Vector2 _Vector2_Reflect(const Vector2* inDirection, const Vector2* inNormal)
{
    float num = -2.0f * _Vector2_Dot(inDirection, inNormal);
    return VEC2(inDirection->x + inNormal->x * num, inDirection->y + inNormal->y * num);
}

Vector2 _Vector2_MoveTowards(const Vector2* current, const Vector2* target, float distanceDelta)
{
    float num = target->x - current->x;
    float num2 = target->y - current->y;
    float num3 = num * num + num2 * num2;
    if (num3 == 0.0f || (distanceDelta >= 0.0f && num3 <= distanceDelta * distanceDelta))
        return *target;

    float num4 = sqrtf(num3);
    return VEC2(current->x + num / num4 * distanceDelta, current->y + num2 / num4 * distanceDelta);
}

Vector2 _Vector2_Multiply(const Vector2* v, float a)
{
    return VEC2(v->x * a, v->y * a);
}

Vector2 Vector2_Subtract(const Vector2* lhs, const Vector2* rhs)
{
    return VEC2(lhs->x - rhs->x, lhs->y - rhs->y);
}
