//
// Created by 18030 on 2024/12/9.
//

#include "MathTool.h"

float Clamp(float t, float a, float b)
{
    if (t < a)
        return a;
    if (t > b)
        return b;
    return t;
}

float RadianToDegree(const float radian)
{
    return (float)(radian * 180.0f / pi);
}

float DegreeToRadian(const float degree)
{
    return (float)(degree * pi / 180.0f);
}
