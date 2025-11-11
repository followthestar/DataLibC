#include "PhysicsCore.h"

#include <stdlib.h>
#include <string.h>

#include "DataLibC/Tool/LogMacro.h"
#define MAX_BODIES 1024
#define MAX_SHAPES 2048

typedef struct PhysicsManager
{
    RigidbodyData2D bodies[MAX_BODIES];
    ShapeData2D shapes[MAX_SHAPES];
    BodyID bodyCount;
    ShapeID shapeCount;
} PhysicsManager;

static PhysicsManager* gPhysicsManager = nullptr;

bool Physics_InitSystem()
{
    if (gPhysicsManager == nullptr)
    {
        gPhysicsManager = malloc(sizeof(PhysicsManager));
        if (gPhysicsManager == nullptr)
        {
            RuntimeErrorLog(OutOfMemory, "Failed to initialize the physics system!");
            return false;
        }
        memset(gPhysicsManager, 0, sizeof(PhysicsManager));
    }
    return true;
}

ShapeID Physics_CreateShape2D(const PhysicsShape2D shapeType)
{
    if (gPhysicsManager->shapeCount >= MAX_SHAPES)
        return INVALID_SHAPE_ID;
    const ShapeID id = gPhysicsManager->shapeCount++;
    gPhysicsManager->shapes[id].type = shapeType;
    return id;
}
