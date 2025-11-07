/**
  ******************************************************************************
  * @file           : VField.h
  * @author         : followthestar
  * @brief          : None
  * @attention      : None
  * @date           : 2025/11/7
  ******************************************************************************
  */


#ifndef DATALIBC_VFIELD_H
#define DATALIBC_VFIELD_H

#include "VType.h"


/**
 * @brief Set field value through reflection
 * @param fieldInfo The info represents the field in type
 * @param object The instance object. If the field is static, pass nullptr
 * @param value The pointer to the value to be set
 */
API_MODULE void VFieldSetValue(const VFieldInfo* fieldInfo, void* object, const void* value);

/**
 * @brief Get field value through reflection
 * @param fieldInfo The info represents the field in type
 * @param object The instance object. If the field is static, pass nullptr
 * @param out The buffer to store the value
 * @warning You need to ensure the buffer is large enough to hold the value
 */
API_MODULE void VFieldGetValue(const VFieldInfo* fieldInfo, void* object, void* out);


#endif //DATALIBC_VFIELD_H
