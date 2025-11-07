//
// Created by 18030 on 2024/12/8.
//

#ifndef TEST_VECTOR3_H
#define TEST_VECTOR3_H

#include "DataLibC/Tool/ExportModuleMacro.h"

struct LIB_STRUCT Vector3
{
    float x;
    float y;
    float z;
};

typedef struct Vector3 Vector3;

#define VEC3(x, y, z) (Vector3){x, y, z}

#define VEC3_AS_ARRAY(v) &(v.x)

#define VEC3_UP VEC3(0, 1, 0)

#define VEC3_RIGHT VEC3(1, 0, 0)

#define VEC3_DOWN VEC3(0, -1, 0)

#define VEC3_LEFT VEC3(-1, 0, 0)

#define VEC3_FRONT VEC3(0, 0, 1)

#define VEC3_BACK VEC3(0, 0, -1)

#define VEC3_ZERO VEC3(0, 0, 0)

#define VEC3_ONE VEC3(1, 1, 1)

API_MODULE Vector3 Vector3_Plus(const Vector3* v1, const Vector3* v2);

API_MODULE Vector3 Vector3_Multiply(const Vector3* v, float factor);

API_MODULE float Vector3_Dot(const Vector3* v1, const Vector3* v2);

API_MODULE Vector3* Vector3_Minus(Vector3* v);

API_MODULE void Vector3_Set(Vector3* v, Vector3 value);

API_MODULE void Vector3_Set3f(Vector3* v, float x, float y, float z);

API_MODULE Vector3 Vector3_Subtract(const Vector3* v1, const Vector3* v2);

API_MODULE void Vector3_Normalize(Vector3* v);

API_MODULE float Vector3_Magnitude(const Vector3* v);

#endif //TEST_VECTOR3_H
