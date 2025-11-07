//
// Created by 18030 on 2025/6/20.
//

#include "Pointer_Internal.h"
#include <stdatomic.h>
#include <stdlib.h>

void _ReleaseRefControl(RefControl* refControl)
{
    if (atomic_load(&refControl->observeCount) == 0)
        free(refControl);
}

void _ReleaseRefObj(const RefControl* refControl, void* obj)
{
    if (!obj)
        return;
    refControl->dtor(obj);
}
