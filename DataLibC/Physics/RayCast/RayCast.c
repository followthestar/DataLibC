//
// Created by 18030 on 2025/10/12.
//

#include "RayCast.h"

#include <math.h>
#include <float.h>

bool RayCast2D_DDA(
    const Ray2D* ray, const int maxX, const int maxY, const MapDetectCallbackOfDDA callback, RayCastHit2D* out)
{
    if (ray == nullptr || callback == nullptr || out == nullptr)
        return false;

    int currX = (int)floorf(ray->start.x);
    int currY = (int)floorf(ray->start.y);

    if (currX < 0 || currX >= maxX || currY < 0 || currY >= maxY)
        return false;

    const float dirX = ray->direction.x;
    const float dirY = ray->direction.y;
    const int stepX = dirX > 0 ? 1 : -1;
    const int stepY = dirY > 0 ? 1 : -1;

    const float xDelta = dirX == 0 ? FLT_MAX : fabsf(1.0f / dirX);
    const float yDelta = dirY == 0 ? FLT_MAX : fabsf(1.0f / dirY);

    float xDist, yDist;
    if (dirX < 0)
        xDist = (ray->start.x - (float)currX) * xDelta;
    else
        xDist = ((float)currX + 1 - ray->start.x) * xDelta;

    if (dirY < 0)
        yDist = (ray->start.y - (float)currY) * yDelta;
    else
        yDist = ((float)currY + 1 - ray->start.y) * yDelta;

    typedef enum Side
    {
        None,
        X, Y
    } Side;

    Side side = None;
    bool hit = false;

    while (currX >= 0 && currX < maxX && currY >= 0 && currY < maxY)
    {
        if (callback(currX, currY))
        {
            hit = true;
            break;
        }

        if (xDist < yDist)
        {
            xDist += xDelta;
            currX += stepX;
            side = X;
        }
        else
        {
            yDist += yDelta;
            currY += stepY;
            side = Y;
        }
    }

    if (!hit)
        return false;

    float wallDist = 0;
    if (side == X)
        wallDist = ((float)currX - ray->start.x + (1.0f - (float)stepX) * 0.5f) / dirX;
    else if (side == Y)
        wallDist = ((float)currY - ray->start.y + (1.0f - (float)stepY) * 0.5f) / dirY;

    out->distance = wallDist;
    out->hitPoint = VEC2(ray->start.x + wallDist * dirX, ray->start.y + wallDist * dirY);

    if (side == X)
    {
        out->hitNormal.y = 0;
        out->hitNormal.x = (float)-stepX;
    }
    else if (side == Y)
    {
        out->hitNormal.x = 0;
        out->hitNormal.y = (float)-stepY;
    }

    // In digital differential analyzer algorithm, we don't care about collider, just set it to nullptr :)
    out->collider = nullptr;
    return true;
}
