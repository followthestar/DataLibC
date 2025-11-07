//
// Created by 18030 on 2025/6/20.
//

#ifndef POINTER_H
#define POINTER_H

#include "DataLibC/DS/Allocation.h"
#include "DataLibC/Tool/ExportModuleMacro.h"

/// @brief Pointer is a proxy type used like smart pointer which manages the
/// objects' lifetime by reference counting.
/// @details You can pass a pointer to another pointer through @code CopyPointer @endcode or just use '='
/// but call @code Retain @endcode manually.
typedef struct Pointer Pointer;

/// @brief ObservePointer is a used to observe a Pointer but not increase the reference count.
/// You can use @code GetPointer @endcode to convert an ObservePointer to a Pointer if the raw
/// object is valid.
typedef struct ObservePointer ObservePointer;

//***************************** Pointer *************************************//

/**
 * @brief Make a new Pointer object with given object and destructor.
 * @param object The object to be managed. If NULL, the pointer refers to nothing.
 * @param dtor The destructor of the object. NULL means using default destructor.
 * @return A new Pointer object refers to the object.
 * @attention Don't pass address of a local variable! That will be a fatal error!
 */
API_MODULE Pointer* MakePointer(void* object, Destructor* dtor);

API_MODULE Pointer* CopyPointer(const Pointer* pointer);

API_MODULE void AssignPointer(Pointer* dst, const Pointer* src);

API_MODULE void Retain(const Pointer* pointer);

API_MODULE void Release(Pointer* pointer);

API_MODULE bool IsNull(const Pointer* pointer);

API_MODULE void* GetRawPointer(const Pointer* pointer);

API_MODULE int UseCount(const Pointer* pointer);

API_MODULE int ObserveCount(const Pointer* pointer);

API_MODULE bool DestroyPointer(Pointer* pointer);

#define DeRef(TYPE, PTR) ((TYPE*)GetRawPointer(PTR))


//***************************** ObservePointer *************************************//


/**
 * @brief Observe a Pointer with weak reference.
 * @param pointer The Pointer to be observed.
 * @return An ObservePointer object refers to the Pointer.
 */
API_MODULE ObservePointer* ObserveOne(const Pointer* pointer);

API_MODULE Pointer* ObtainFromObserve(const ObservePointer* observe);

API_MODULE bool IsValid(const ObservePointer* pointer);

#endif //POINTER_H
