//
// Created by 18030 on 2025/6/20.
//

#include "Pointer.h"

#include <stdlib.h>

#include "Pointer_Internal.h"

struct Pointer
{
    void* object;
    RefControl* refControl;
};

struct ObservePointer
{
    void* ptr;
    RefControl* refControl;
};

static Pointer* _DefaultPointerCtor()
{
    Pointer* pointer = malloc(sizeof(Pointer));
    pointer->object = NULL;
    pointer->refControl = NULL;
    return pointer;
}

Pointer* MakePointer(void* object, Destructor* dtor)
{
    Pointer* pointer = _DefaultPointerCtor();
    if (!object)
        return pointer;

    pointer->refControl = malloc(sizeof(RefControl));
    atomic_store(&pointer->refControl->observeCount, 0);
    atomic_store(&pointer->refControl->refCount, 1);

    if (dtor)
        pointer->refControl->dtor = dtor;
    else
        pointer->refControl->dtor = GetBuiltinDtor(kCUSTOM);

    return pointer;
}

Pointer* CopyPointer(const Pointer* pointer)
{
    Pointer* newPointer = malloc(sizeof(Pointer));
    newPointer->object = pointer->object;
    newPointer->refControl = pointer->refControl;
    Retain(pointer);
    return newPointer;
}

void AssignPointer(Pointer* dst, const Pointer* src)
{
    if (dst->object != src->object)
        Release(dst);

    dst->object = src->object;
    dst->refControl = src->refControl;
    Retain(src);
}

void Retain(const Pointer* pointer)
{
    if (pointer->refControl)
    {
        atomic_fetch_add(&pointer->refControl->refCount, 1);
    }
}

void Release(Pointer* pointer)
{
    if (pointer->refControl)
    {
        atomic_fetch_sub(&pointer->refControl->refCount, 1);
        if (atomic_load(&pointer->refControl->refCount) == 0)
        {
            _ReleaseRefObj(pointer->refControl, pointer->object);
            _ReleaseRefControl(pointer->refControl);
            pointer->object = NULL;
            pointer->refControl = NULL;
        }
    }
}

bool IsNull(const Pointer* pointer)
{
    return pointer->object == NULL;
}

void* GetPointer(const Pointer* pointer)
{
    return pointer->object;
}

int UseCount(const Pointer* pointer)
{
    if (pointer->refControl)
        return atomic_load(&pointer->refControl->refCount);
    return 0;
}

int ObserveCount(const Pointer* pointer)
{
    if (pointer->refControl)
        return atomic_load(&pointer->refControl->observeCount);
    return 0;
}

bool DestroyPointer(Pointer* pointer)
{
    if (!pointer->refControl)
    {
        free(pointer);
        return false;
    }
    Release(pointer);
    free(pointer);
    return true;
}

ObservePointer* ObserveOne(const Pointer* pointer)
{
    ObservePointer* observePointer = malloc(sizeof(ObservePointer));
    observePointer->ptr = pointer->object;
    observePointer->refControl = pointer->refControl;
    if (observePointer->refControl)
    {
        atomic_fetch_add(&observePointer->refControl->observeCount, 1);
    }
    return observePointer;
}

bool IsValid(const ObservePointer* pointer)
{
    if (!pointer->refControl)
        return false;
    if (atomic_load(&pointer->refControl->refCount) == 0)
        return false;
    return true;
}

Pointer* ObtainFromObserve(const ObservePointer* observe)
{
    Pointer* pointer = _DefaultPointerCtor();
    if (!IsValid(observe))
        return pointer;
    pointer->refControl = observe->refControl;
    pointer->object = observe->ptr;
    Retain(pointer);
    return pointer;
}
