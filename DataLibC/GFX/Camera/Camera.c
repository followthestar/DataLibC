//
// Created by 18030 on 2025/10/11.
//

#include "Camera.h"

#include <math.h>

#include "DataLibC/DS/Allocation.h"
#include "DataLibC/LA/Math/MathTool.h"
#include "DataLibC/LA/Vector/Vector3.h"

typedef struct Camera
{
    /// Location of the camera
    Vector3 position;

    /// Euler angle of the orientation (In degrees)
    Vector3 rotation;

    /// Ratio of Width / Height
    float aspect;

    /// Always store it as VERTICAL fov (In degrees)
    float fov;

    /// Used to rebuild cache
    bool isDirty;
} Camera;

Camera* Camera_Create()
{
    Camera* camera = BASE_NEW(Camera);
    Vector3_Set(&camera->position, VEC3_ZERO);
    Vector3_Set(&camera->rotation, VEC3_ZERO);
    camera->aspect = 1.0f;
    camera->fov = 45.0f;
    return camera;
}

void Camera_Destroy(Camera* camera)
{
    BASE_DELETE(Camera, camera);
}

void Camera_SetPosition(Camera* camera, const Vector3* position)
{
    Vector3_Set(&camera->position, *position);
    camera->isDirty = true;
}

Vector3 Camera_GetPosition(const Camera* camera)
{
    return camera->position;
}

void Camera_SetRotation(Camera* camera, Vector3* rotation)
{
    Vector3_Set(&camera->rotation, *rotation);
    camera->isDirty = true;
}

Vector3 Camera_GetRotation(const Camera* camera)
{
    return camera->rotation;
}

void Camera_SetAspect(Camera* camera, const float aspect)
{
    camera->aspect = aspect;
    camera->isDirty = true;
}

void Camera_SetHorizontalFieldOfView(Camera* camera, float fov)
{
    fov = Clamp(fov, 1, 90);
    const float hFovRad = DegreeToRadian(fov);
    const float vFovRad = 2 * atanf(tanf(hFovRad * 0.5f) / camera->aspect);
    const float vFov = RadianToDegree(vFovRad);
    camera->fov = Clamp(vFov, 1, 90);
}

void Camera_SetVerticalFieldOfView(Camera* camera, float fov)
{
    fov = Clamp(fov, 1, 90);
    camera->fov = fov;
}
