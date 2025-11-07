//
// Created by 18030 on 2025/10/12.
//

#include "Ray.h"

Ray2D Ray2D_FromPoints(const Vector2 start, const Vector2 point)
{
    Ray2D ray = { .start = point };
    ray.direction = Vector2_Subtract(&point, &start);
    Vector2_Normalize(&ray.direction);
    return ray;
}

Ray Ray_FromPoints(const Vector3 start, const Vector3 point)
{
    Ray ray = { .start = start };
    ray.direction = Vector3_Subtract(&point, &start);
    Vector3_Normalize(&ray.direction);
    return ray;
}
