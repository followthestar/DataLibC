//
// Created by 18030 on 2025/10/12.
//

#ifndef RAY_H
#define RAY_H

#include "DataLibC/LA/Vector/Vector2.h"
#include "DataLibC/LA/Vector/Vector3.h"
#include "DataLibC/Physics/Collider/Collider.h"
#include "DataLibC/Physics/Collider/Collider2D.h"
#include "DataLibC/Tool/ExportModuleMacro.h"


typedef struct Ray
{
    /// The start point of the ray
    Vector3 start;

    /// The direction of the ray (Normalized best)
    Vector3 direction;
} Ray;

typedef struct Ray2D
{
    /// The start point of the ray
    Vector2 start;

    /// The direction of the ray (Normalized best)
    Vector2 direction;
} Ray2D;

typedef struct RayCastHit
{
    /// The distance from the start point to the hit point
    float distance;

    /// The hit point
    Vector3 hitPoint;

    /// The normal of the hit surface
    Vector3 hitNormal;

    /// The collider hit by the ray
    Collider* collider;
} RayCastHit;

typedef struct RayCastHit2D
{
    /// The distance from the start point to the hit point
    float distance;

    /// The hit point
    Vector2 hitPoint;

    /// The normal of the hit surface
    Vector2 hitNormal;

    /// The collider hit by the ray
    Collider2D* collider;
} RayCastHit2D;


/**
 * @brief Compute a ray starts from \p start to \p point
 * @param start The start point
 * @param point Another point on the ray
 * @return The Ray2D object
 */
API_MODULE Ray2D Ray2D_FromPoints(Vector2 start, Vector2 point);

/**
 * @brief Compute a ray starts from \p start to \p point
 * @param start The start point
 * @param point Another point on the ray
 * @return The Ray object
 */
API_MODULE Ray Ray_FromPoints(Vector3 start, Vector3 point);

#endif //RAY_H
