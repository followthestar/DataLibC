//
// Created by 18030 on 2025/10/11.
//

#ifndef CAMERA_H
#define CAMERA_H

#include "DataLibC/LA/Vector/Vector3.h"
#include "DataLibC/Tool/ExportModuleMacro.h"

/// Camera is used to render the graphics to the screen
typedef struct Camera Camera;

/**
 * @brief Create a new camera
 * @return The new camera object
 */
API_MODULE Camera* Camera_Create();

/**
 * @brief Destroy the camera
 * @param camera The camera object
 */
API_MODULE void Camera_Destroy(Camera* camera);

/**
 * @brief Set the position of the camera
 * @param camera The camera object
 * @param position The position of the camera
 */
API_MODULE void Camera_SetPosition(Camera* camera, const Vector3* position);

/**
 * @brief Get the position of the camera
 * @param camera The camera object
 * @return The position of the camera
 */
API_MODULE Vector3 Camera_GetPosition(const Camera* camera);

/**
 * @brief Set the rotation of the camera
 * @param camera The camera object
 * @param rotation The rotation of the camera
 */
API_MODULE void Camera_SetRotation(Camera* camera, Vector3* rotation);

/**
 * @brief Get the rotation of the camera
 * @param camera The camera object
 * @return The rotation of the camera
 */
API_MODULE Vector3 Camera_GetRotation(const Camera* camera);

/**
 * @brief Set the aspect ratio of the camera
 * @param camera The camera object
 * @param aspect The aspect ratio (Width / Height)
 */
API_MODULE void Camera_SetAspect(Camera* camera, float aspect);

/**
 * @brief Set the horizontal field of view of the camera
 * @param camera The camera object
 * @param fov The field of view (Range[0, 90] in degrees)
 */
API_MODULE void Camera_SetHorizontalFieldOfView(Camera* camera, float fov);

/**
 * @brief Set the vertical field of view of the camera
 * @param camera The camera object
 * @param fov The field of view (Range[0, 90] in degrees)
 */
API_MODULE void Camera_SetVerticalFieldOfView(Camera* camera, float fov);

#endif //CAMERA_H
