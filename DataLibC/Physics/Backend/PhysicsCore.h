/**
  ******************************************************************************
  * @file           : PhysicsCore.h
  * @author         : followthestar
  * @brief          : None
  * @attention      : None
  * @date           : 2025/11/11
  ******************************************************************************
  */


#ifndef DATALIBC_PHYSICSCORE_H
#define DATALIBC_PHYSICSCORE_H

#include "DataLibC/LA/Vector/Vector2.h"
#include "DataLibC/Physics/Rigidbody/RigidbodyType.h"
#include "DataLibC/Tool/TypeMacro.h"

typedef enum _PhysicsShape2D
{
    Shape2D_Box,
    Shape2D_Circle
} PhysicsShape2D;

typedef struct _ShapeData2D
{
    PhysicsShape2D type;

    union
    {
        struct
        {
            float radius;
        } circle;

        struct
        {
            Vector2 halfExtents;
        } box;
    };
} ShapeData2D;

typedef struct _RigidbodyData2D
{
    Vector2 position;
    Vector2 velocity;
    RigidbodyType shape;

    bool isSleep;
} RigidbodyData2D;

typedef UInt32 BodyID;
typedef UInt32 ShapeID;

constexpr BodyID INVALID_BODY_ID = (BodyID)-1;
constexpr ShapeID INVALID_SHAPE_ID = (ShapeID)-1;


ShapeID Physics_CreateShape2D(PhysicsShape2D shapeType);

#endif //DATALIBC_PHYSICSCORE_H
