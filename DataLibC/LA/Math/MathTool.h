//
// Created by 18030 on 2024/12/9.
//

#ifndef TEST_MATHTOOL_H
#define TEST_MATHTOOL_H

#include "DataLibC/Tool/ExportModuleMacro.h"

#define Clamp01(t) Clamp(t, 0, 1)

constexpr float epsilon = 0.00001f;

constexpr double pi = 3.14159265358979323846;

API_MODULE float Clamp(float t, float a, float b);

/**
 * @brief Convert radian to degree
 * @param radian The radian
 * @return The degree
 */
API_MODULE float RadianToDegree(float radian);

/**
 * @brief Convert degree to radian
 * @param degree The degree
 * @return The radian
 */
API_MODULE float DegreeToRadian(float degree);

#endif //TEST_MATHTOOL_H
