//
// Created by 18030 on 2025/1/2.
//

#include "VObject.h"
#include "VType.h"
#include "VType_Internal.h"

__DEFINE_V_GET_TYPE(VObject)
{
    return V_TYPE_OBJECT;
}

__DECLARE_VTYPE_INIT_FUNC(VObject);


VClass VOBJECT_CLASS = {
    .super = nullptr,
    .typeId = V_TYPE_OBJECT,
    .className = "VObject",
    .classInit = nullptr,
    .classFinalize = nullptr,
    .instanceInit = (VInstanceInitFunc)__VTYPE_INIT_FUNC(VObject),
    .instanceFinalize = nullptr
};

DEFINE_VTYPE_INIT(VObject)
{
    self->vptr = &VOBJECT_CLASS;
}


vpointer GetParentClass(const VClass* klass)
{
    return klass->super;
}




