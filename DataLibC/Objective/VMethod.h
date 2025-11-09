/**
  ******************************************************************************
  * @file           : VMethod.h
  * @author         : 18030
  * @brief          : None
  * @attention      : None
  * @date           : 2025/11/6
  ******************************************************************************
  */


#ifndef DATALIBC_VMETHOD_H
#define DATALIBC_VMETHOD_H

#include "VType.h"


/**
 * @brief Display the signature of a method
 * @param methodInfo The method in a type
 */
API_MODULE void VDisplayMethodSignature(const VMethodInfo* methodInfo);

/**
 * @brief Invoke a method through reflection
 * @param methodInfo The method to be invoked
 * @param object The instance object of the type which this method belongs to. If the method is static, pass nullptr.
 * @param params The parameters to be passed to the method
 * @param out Used to receive the return value
 * @attention Due to the limitation of the C language, we only support reflection calls to functions registered through
 * macro @code MemberMethod@endcode or \code StaticMethod\endcode, and the signature of such functions will be uniformly
 * fixed as void*(*)(void* object, void** args)
 * @warning This method is not safe, please use @code VMethodInvokeSafe@endcode instead.
 */
API_MODULE void VMethodInvoke(const VMethodInfo* methodInfo, void* object, void** params, void* out);

/**
 * @brief Invoke a method through reflection
 * @param methodInfo The method to be invoked
 * @param object The instance object of the type which this method belongs to. If the method is static, pass nullptr.
 * @param params The parameters to be passed to the method
 * @param paramCount The number of parameters
 * @param out Used to receive the return value
 * @attention Due to the limitation of the C language, we only support reflection calls to functions registered through
 * macro @code MemberMethod@endcode or \code StaticMethod\endcode, and the signature of such functions will be uniformly
 * fixed as void*(*)(void* object, void** args)
 */
API_MODULE void VMethodInvokeSafe(VMethodInfo* methodInfo, void* object, void** params, int paramCount, void* out);

#endif //DATALIBC_VMETHOD_H
