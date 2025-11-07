//
// Created by 18030 on 2024/12/8.
//

#ifndef TEST_VECTOR2_H
#define TEST_VECTOR2_H

#include "DataLibC/Tool/ExportModuleMacro.h"

struct LIB_STRUCT Vector2
{
    float x;
    float y;
};

typedef struct Vector2 Vector2;

#define VEC2(x, y) (Vector2){x, y}

#define VEC2_AS_ARRAY(v) &(v.x)

#define VEC2_UP VEC2(0, 1)

#define VEC2_RIGHT VEC2(1, 0)

#define VEC2_DOWN VEC2(0, -1)

#define VEC2_LEFT VEC2(-1, 0)

#define VEC2_ZERO VEC2(0, 0)

#define VEC2_ONE VEC2(1, 1)

#define Vector2_Plus(v1, v2) _Vector2_Plus(&(v1), &(v2))

#define Vector2_Multiply(v, factor) _Vector2_Multiply(&(v), factor)

#define Vector2_Dot(v1, v2) _Vector2_Dot(&(v1), &(v2))

#define Vector2_Magnitude(v) _Vector2_Magnitude(&(v))

#define Vector2_SqrMagitude(v) _Vector2_SqrMagnitude(&(v))

#define Vector2_Normalized(v) _Vector2_Normalized(&(v))

#define Vector2_Scale(v, scale) _Vector2_Scale(&(v), &(scale))

#define Vector2_Equals(lhs, rhs) _Vector2_Equals(&(lhs), &(rhs))

#define Vector2_Lerp(a, b, t) _Vector2_Lerp(&(a), &(b), t)

#define Vector2_Angle(from, to) _Vector2_Angle(&(from), &(to))

#define Vector2_SignedAngle(from, to) _Vector2_SignedAngle(&(from), &(to))

#define Vector2_Reflect(inDirection, inNormal) _Vector2_Reflect(&(inDirection), &(inNormal))

#define Vector2_MoveTowards(current, target, distanceDelta) _Vector2_MoveTowards(&(current), &(target), distanceDelta)

API_MODULE Vector2 _Vector2_Plus(const Vector2* lhs, const Vector2* rhs);

API_MODULE Vector2 _Vector2_Multiply(const Vector2* lhs, float a);

API_MODULE float _Vector2_Dot(const Vector2* lhs, const Vector2* rhs);

API_MODULE float _Vector2_Magnitude(const Vector2* v);

API_MODULE float _Vector2_SqrMagnitude(const Vector2* v);

API_MODULE Vector2 _Vector2_Normalized(const Vector2* v);

API_MODULE void Vector2_Normalize(Vector2* v);

API_MODULE void _Vector2_Scale(Vector2* v, const Vector2* scale);

API_MODULE float _Vector2_Distance(const Vector2* a, const Vector2* b);

API_MODULE bool _Vector2_Equals(const Vector2* lhs, const Vector2* rhs);

API_MODULE Vector2 _Vector2_Lerp(const Vector2* a, const Vector2* b, float t);

API_MODULE float _Vector2_Angle(const Vector2* from, const Vector2* to);

API_MODULE float _Vector2_SignedAngle(const Vector2* from, const Vector2* to);

API_MODULE Vector2 _Vector2_Reflect(const Vector2* inDirection, const Vector2* inNormal);

API_MODULE Vector2 _Vector2_MoveTowards(const Vector2* current, const Vector2* target, float distanceDelta);

API_MODULE Vector2 Vector2_Subtract(const Vector2* lhs, const Vector2* rhs);

#endif //TEST_VECTOR2_H
