//
// Created by 18030 on 2025/10/12.

#ifndef RAYCAST_H
#define RAYCAST_H

#include "Ray.h"
#include "DataLibC/Tool/ExportModuleMacro.h"


/**
 * @brief A callback function used to check whether the wall exists at position [\p x, \p y]
 */
typedef bool (* MapDetectCallbackOfDDA)(int x, int y);

API_MODULE bool RayCast(Ray* ray, float maxDistance, RayCastHit* out);

API_MODULE bool RayCast2D(Ray2D* ray, float maxDistance, RayCastHit2D* out);

/**
 * @brief Cast a ray to test the wall in the map
 * @param ray The ray to cast
 * @param maxX The maximum grid in a row of the map
 * @param maxY The maximum grid in a column of the map
 * @param callback A callback function used to check whether wall exists at position [\p x, \p y]
 * @param out The hit result
 * @return If the ray hit a wall, return true. Otherwise, return false.
 */
API_MODULE bool RayCast2D_DDA(const Ray2D* ray, int maxX, int maxY, MapDetectCallbackOfDDA callback, RayCastHit2D* out);

#endif //RAYCAST_H
