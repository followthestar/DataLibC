//
// Created by 18030 on 2025/3/22.
//

#ifndef DATALIBC_OBJECTPOOL_H
#define DATALIBC_OBJECTPOOL_H

#include "DataLibC/Tool/ExportModuleMacro.h"

typedef void* (* CopyConstructor)(void* origin);

typedef void* (* DefaultConstructor)();

typedef void (* ResetObject)(void* object);

typedef void (* ObjectDestructor)(void* object);

/// ObjectPool provides a method to acquire and release objects without frequently creating and destroying them.
typedef struct ObjectPool ObjectPool;

/// Create a new thread-safe ObjectPool.
/// \param origin The prototype object.
/// \param cctor The copy constructor used to copy the prototype object.
/// \param ctor The default constructor used to create a new object.
/// \param reset The reset method used to reset the object after releasing one object.
/// \param dtor The destructor used to destruct the object.
/// \return A new ObjectPool.
/// \attention This kind of ObjectPool allows you to access object through two ways. One is copying from a prototype object and
/// another is creating a new object by default constructor. These two ways are not compatible and Prototype has higher priority.
/// So please create pool by setting corresponding arguments to NULL according to the way that you want. Nontheless, all NULL is not allowed.
API_MODULE ObjectPool*
CreateObjectPool(
        void* origin, CopyConstructor cctor, DefaultConstructor ctor, ResetObject reset, ObjectDestructor dtor);

/// Enable the threshold mode of the pool.
/// \param pool The ObjectPool object.
/// \param threshold When the left object in this pool is less than the threshold, the pool will start a new thread to
/// allocate the number of objects specified by allocateAmount. Set this to 0 to disable the threshold mode.
/// \param allocateAmount The number of objects to be allocated when triggering the threshold. It will be ignored if it is less than 1.
API_MODULE void EnableObjectPoolThreshold(ObjectPool* pool, size_t threshold, size_t allocateAmount);

/// Acquire an object from the pool.
/// \param pool The ObjectPool object.
/// \return The new object.
API_MODULE void* AcquireObject(ObjectPool* pool);

/// Release an object to the pool.
/// \param pool The ObjectPool object.
/// \param object The object to be released. And it will be reset by the reset method.
API_MODULE void ReleaseObject(ObjectPool* pool, void* object);

/// Destroy the pool. And destructor will be called for each left object.
/// \param pool The ObjectPool object.
/// \attention You must make sure there is no thread still trying to acquire or release objects.
API_MODULE void DestroyObjectPool(ObjectPool* pool);

#endif //DATALIBC_OBJECTPOOL_H
