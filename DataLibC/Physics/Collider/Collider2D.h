//
// Created by 18030 on 2025/3/22.
//

#ifndef DATALIBC_COLLIDER2D_H
#define DATALIBC_COLLIDER2D_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/LA/Vector/Vector2.h"

/// Base class for all colliders in 2D space.
typedef struct Collider2D Collider2D;


typedef enum Collider2DType
{
    kCollider2D_Box,
    kCollider2D_Circle,
    kCollider2D_Polygon,
    kCollider2D_Sprite
} Collider2DType;


typedef struct Collider2D_VTable { } Collider2D_VTable;


struct Collider2D
{
    const Collider2D_VTable* vtable;
};

/// Create a new Collider2D object from factory method.
/// \param type The type of the collider.
/// \param positionHandle A Vector2 handle representing the position of the collider. This should be a persistent pointer.
/// \param rotationHandle A float handle representing the rotation of the collider. This should be a persistent pointer.
/// \param isTrigger Should it be treated as a trigger? A trigger means that it will not have an impact in physics field
/// when colliding with other colliders. But it will still send collision messages.
/// \return A pointer to the new Collider2D object. It is polymorphic, so you can cast it to the specific type you just created.
/// \warning All handles in the object should have the same or longer lifetime as the Collider2D object.
API_MODULE Collider2D*
Collider2D_Create(Collider2DType type, Vector2* positionHandle, float* rotationHandle, bool isTrigger);

/// Set the trigger state of the collider.
/// \param collider2D The collider to set.
/// \param isTrigger
API_MODULE void Collider2D_SetTrigger(Collider2D* collider2D, bool isTrigger);

/// Is the collider a trigger?
/// \param collider2D The collider to check.
/// \return True if it is a trigger, false otherwise.
API_MODULE bool Collider2D_IsTrigger(Collider2D* collider2D);

#endif //DATALIBC_COLLIDER2D_H
