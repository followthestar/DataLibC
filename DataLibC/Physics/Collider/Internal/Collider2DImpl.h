//
// Created by 18030 on 2025/3/22.
//

#ifndef DATALIBC_COLLIDER2DIMPL_H
#define DATALIBC_COLLIDER2DIMPL_H

#include "DataLibC/Physics/Collider/Collider2D.h"
#include "DataLibC/LA/Vector/Vector2.h"

typedef struct Collider2DImpl
{
    const struct Collider2D_VTable* vtable;

    Vector2 m_LocalPosition;
    float m_LocalRotation;

    bool isTrigger;
} Collider2DImpl;

#endif //DATALIBC_COLLIDER2DIMPL_H
