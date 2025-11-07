//
// Created by 18030 on 2025/6/20.
//

#ifndef POINTER_INTERNAL_H
#define POINTER_INTERNAL_H

#include <stdatomic.h>

#include "DataLibC/DS/Allocation.h"

typedef struct _RefControl RefControl;

struct _RefControl
{
     atomic_int refCount;
     atomic_int observeCount;
     Destructor* dtor;
};

void _ReleaseRefControl(RefControl* refControl);

void _ReleaseRefObj(const RefControl* refControl, void* obj);

#endif //POINTER_INTERNAL_H
