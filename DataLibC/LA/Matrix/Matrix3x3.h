//
// Created by 18030 on 2024/12/10.
//

#ifndef TEST_MATRIX3X3_H
#define TEST_MATRIX3X3_H

#include "DataLibC/LA/Vector/Vector3.h"
#include "DataLibC/LA/Vector/Vector2.h"
#include <stddef.h>

typedef struct Vector2 Vector2;

struct LIB_STRUCT Matrix3x3
{
    Vector3 matrix[3];
};

typedef struct Matrix3x3 Matrix3x3;

#define MAT3x3(v1, v2, v3) (Matrix3x3){v1, v2, v3}

#define Matrix3x3_Identity MAT3x3(VEC3_RIGHT, VEC3_UP, VEC3_FRONT)

#define Matrix3x3_Zero MAT3x3(VEC3_ZERO, VEC3_ZERO, VEC3_ZERO)

#define MAT3x3_AS_ARRAY(m) &((m)->matrix[0].x)

API_MODULE float Matrix3x3_Index(const Matrix3x3* matrix, size_t row, size_t col);

API_MODULE Matrix3x3 Matrix3x3_Plus(const Matrix3x3* m1, const Matrix3x3* m2);

API_MODULE Matrix3x3 Matrix3x3_Multiply(const Matrix3x3* lhs, const Matrix3x3* rhs);

API_MODULE Matrix3x3 Matrix3x3_MultyplyF(const Matrix3x3* m, float f);

API_MODULE Matrix3x3 Matrix3x3_Divide(const Matrix3x3* m, float f);

API_MODULE Matrix3x3* Matrix3x3_Translate(Matrix3x3* m, const Vector2* position);

API_MODULE Matrix3x3* Matrix3x3_Rotate(Matrix3x3* m, float angle);

API_MODULE Matrix3x3* Matrix3x3_Scale(Matrix3x3* m, const Vector2* scale);

API_MODULE Matrix3x3 Matrix3x3_MakeTranslation(const Vector2* position);

API_MODULE Matrix3x3 Matrix3x3_MakeRotation(float angle);

API_MODULE Matrix3x3 Matrix3x3_MakeScaling(const Vector2* scale);

API_MODULE Matrix3x3 Matrix3x3_MakeSkew(const Vector2* skew);

API_MODULE Matrix3x3* Matrix3x3_Transpose(Matrix3x3* m);

API_MODULE float Matrix3x3_GetDeterminant(const Matrix3x3* m);

API_MODULE Matrix3x3 Matrix3x3_Adjoint(const Matrix3x3* m);

API_MODULE bool Matrix3x3_Invert(Matrix3x3* m);

API_MODULE Matrix3x3 Matrix3x3_Ortho(float width, float height);

API_MODULE Matrix3x3 Matrix3x3_View(const Vector2* position);

API_MODULE void* Matrix3x3_Ctor(const void* data);

API_MODULE Vector2 Matrix3x3_Apply(const Matrix3x3* m, const Vector2* v);

#endif //TEST_MATRIX3X3_H
