//
// Created by 18030 on 2024/12/10.
//

#include "Matrix3x3.h"
#include "DataLibC/LA/Vector/Vector2.h"
#include "DataLibC/Tool/Utility.h"
#include "DataLibC/LA/Math/MathTool.h"
#include <assert.h>

#ifdef __SSE2__
#include <emmintrin.h>
#include <math.h>
#include <string.h>

#define USE_SSE2
#endif

#define Get(r, c) GetM(m, r, c)
#define GetM(m, r, c) *(&((m)->matrix[c].x) + r)

#define DEG2RAD (float)(M_PI / 180.0f)

float Matrix3x3_Index(const Matrix3x3* matrix, size_t row, size_t col)
{
    assert(row < 3 && col < 3);
    return *(&matrix->matrix[col].x + row);
}

Matrix3x3 Matrix3x3_Plus(const Matrix3x3* m1, const Matrix3x3* m2)
{
    Matrix3x3 result;

#ifdef USE_SSE2
    __m128 v1;
    __m128 v2;
    __m128 sum;
    alignas(16) float tmp[4];
#endif

    for (int i = 0; i < 3; ++i)
    {
#ifdef USE_SSE2
        v1 = _mm_loadu_ps(&m1->matrix[i].x);
        v2 = _mm_loadu_ps(&m2->matrix[i].x);

        sum = _mm_add_ps(v1, v2);
        _mm_store_ps(tmp, sum);
        result.matrix[i] = VEC3(tmp[0], tmp[1], tmp[2]);
#else
    result.matrix[i] = VEC3(m1->matrix[i].x + m2->matrix[i].x,
                            m1->matrix[i].y + m2->matrix[i].y,
                            m1->matrix[i].z + m2->matrix[i].z);
#endif
    }
    return result;
}

Matrix3x3 Matrix3x3_Multiply(const Matrix3x3* lhs, const Matrix3x3* rhs)
{
    Matrix3x3 result = Matrix3x3_Zero;
    __m128 v1;
    __m128 v2;
    __m128 sum;

    alignas(16) float tmp[4];
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
#ifdef USE_SSE2
            alignas(16) float row[4] = {*(&lhs->matrix[0].x + i),
                                        *(&lhs->matrix[1].x + i),
                                        *(&lhs->matrix[2].x + i), 0};
            v1 = _mm_load_ps(row);
            v2 = _mm_loadu_ps(&rhs->matrix[j].x);

            sum = _mm_mul_ps(v1, v2);
            _mm_store_ps(tmp, sum);
            *(&result.matrix[j].x + i) = tmp[0] + tmp[1] + tmp[2];
#else
            for(int k = 0; k < 3; ++k)
            {
                *(&result.matrix[j] + i) += *(&lhs->matrix[k].x + i) * *(&rhs->matrix[j].x + k);
            }
#endif
        }
    }
    return result;
}

Matrix3x3 Matrix3x3_MakeTranslation(const Vector2* position)
{
    return MAT3x3(VEC3_RIGHT, VEC3_UP, VEC3(position->x, position->y, 1));
}

Matrix3x3 Matrix3x3_MakeRotation(float angle)
{
    float radius = angle / 57.29578f;
    float cosTheta = cosf(radius);
    float sinTheta = sinf(radius);
    return MAT3x3(VEC3(cosTheta, sinTheta, 0), VEC3(-sinTheta, cosTheta, 0), VEC3(0, 0, 1));
}

Matrix3x3 Matrix3x3_MakeScaling(const Vector2* scale)
{
    return MAT3x3(VEC3(scale->x, 0, 0), VEC3(0, scale->y, 0), VEC3(0, 0, 1));
}

Matrix3x3* Matrix3x3_Transpose(Matrix3x3* m)
{
    swap(m->matrix[0].y, m->matrix[1].x);
    swap(m->matrix[0].z, m->matrix[2].x);
    swap(m->matrix[1].z, m->matrix[2].y);
    return m;
}

Matrix3x3* Matrix3x3_Translate(Matrix3x3* m, const Vector2* position)
{
    m->matrix[2].x += m->matrix[0].x * position->x + m->matrix[1].x * position->y;
    m->matrix[2].y += m->matrix[0].y * position->x + m->matrix[1].y * position->y;
    m->matrix[2].z += m->matrix[0].z * position->x + m->matrix[1].z * position->y;
    return m;
}

Matrix3x3* Matrix3x3_Rotate(Matrix3x3* m, float angle)
{
    auto rotation = Matrix3x3_MakeRotation(angle);
    *m = Matrix3x3_Multiply(m, &rotation);
    return m;
}

Matrix3x3* Matrix3x3_Scale(Matrix3x3* m, const Vector2* scale)
{
    m->matrix[0].x *= scale->x;
    m->matrix[0].y *= scale->x;
    m->matrix[0].z *= scale->x;

    m->matrix[1].x *= scale->y;
    m->matrix[1].y *= scale->y;
    m->matrix[1].z *= scale->y;

    return m;
}

float Matrix3x3_GetDeterminant(const Matrix3x3* m)
{
    float f1 = Get(0, 0) * Get(1, 1) * Get(2, 2);
    float f2 = Get(0, 1) * Get(1, 2) * Get(2, 0);
    float f3 = Get(0, 2) * Get(1, 0) * Get(2, 1);

    float f4 = Get(0, 2) * Get(1, 1) * Get(2, 0);
    float f5 = Get(0, 1) * Get(1, 0) * Get(2, 2);
    float f6 = Get(0, 0) * Get(1, 2) * Get(2, 1);

    return f1 + f2 + f3 - f4 - f5 - f6;
}

Matrix3x3 Matrix3x3_Adjoint(const Matrix3x3* m)
{
    Matrix3x3 inverse = {};
    GetM(&inverse,0, 0) = Get (1, 1) * Get (2, 2) - Get (1, 2) * Get (2, 1);
    GetM(&inverse,0, 1) = Get (0, 2) * Get (2, 1) - Get (0, 1) * Get (2, 2);
    GetM(&inverse,0, 2) = Get (0, 1) * Get (1, 2) - Get (0, 2) * Get (1, 1);
    GetM(&inverse,1, 0) = Get (1, 2) * Get (2, 0) - Get (1, 0) * Get (2, 2);
    GetM(&inverse,1, 1) = Get (0, 0) * Get (2, 2) - Get (0, 2) * Get (2, 0);
    GetM(&inverse,1, 2) = Get (0, 2) * Get (1, 0) - Get (0, 0) * Get (1, 2);
    GetM(&inverse,2, 0) = Get (1, 0) * Get (2, 1) - Get (1, 1) * Get (2, 0);
    GetM(&inverse,2, 1) = Get (0, 1) * Get (2, 0) - Get (0, 0) * Get (2, 1);
    GetM(&inverse,2, 2) = Get (0, 0) * Get (1, 1) - Get (0, 1) * Get (1, 0);
    return inverse;
}

bool Matrix3x3_Invert(Matrix3x3* m)
{
    Matrix3x3 inverse = Matrix3x3_Adjoint(m);

    float det = Get(0, 0) * GetM(&inverse, 0, 0) + Get(0, 1) * GetM(&inverse, 1, 0) + Get(0, 2) * GetM(&inverse, 2, 0);
    if (fabsf(det) > epsilon)
    {
        *m = Matrix3x3_Divide(&inverse,det);
        return true;
    }
    else
    {
        return false;
    }
}

Matrix3x3 Matrix3x3_Divide(const Matrix3x3* m, float f)
{
    return Matrix3x3_MultyplyF(m, 1.0f / f);
}

Matrix3x3 Matrix3x3_MultyplyF(const Matrix3x3* m, float f)
{
    Matrix3x3 result = *m;
    float* data = MAT3x3_AS_ARRAY(&result);
    for (int i = 0; i < 9; ++i)
        data[i] *= f;
    return result;
}

Matrix3x3 Matrix3x3_Ortho(float width, float height)
{
    return MAT3x3(VEC3(2 / width, 0, 0), VEC3(0, 2 / height, 0), VEC3(0, 0, 1));
}

Matrix3x3 Matrix3x3_View(const Vector2* position)
{
    return Matrix3x3_MakeTranslation(&VEC2(-position->x, -position->y));
}

void* Matrix3x3_Ctor(const void* data)
{
    Matrix3x3* m = malloc(sizeof(Matrix3x3));
    memcpy(m, data, sizeof(Matrix3x3));
    return m;
}

Vector2 Matrix3x3_Apply(const Matrix3x3* m, const Vector2* v)
{
    Vector2 result;
    result.x = Get(0, 0) * v->x + Get(0, 1) * v->y + Get(0, 2);
    result.y = Get(1, 0) * v->x + Get(1, 1) * v->y + Get(1, 2);
    return result;
}

Matrix3x3 Matrix3x3_MakeSkew(const Vector2* skew)
{
    Matrix3x3 result = Matrix3x3_Identity;
    result.matrix[1].x = tanf(skew->x * DEG2RAD);
    result.matrix[0].y = tanf(skew->y * DEG2RAD);
    return result;
}

