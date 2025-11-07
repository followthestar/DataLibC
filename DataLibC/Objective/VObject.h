//
// Created by 18030 on 2025/1/2.
//

#ifndef DATALIBC_VOBJECT_H
#define DATALIBC_VOBJECT_H


#include "DataLibC/Tool/ExportModuleMacro.h"
#include "VType.h"


/* typedef structures */
typedef struct _VObject VObject;
typedef struct _VClass VClass;


#define V_TYPE_IS_OBJECT(type)      (V_TYPE_IS_DERIVED_FROM(type, V_TYPE_OBJECT))

#define VOBJECT_NEW(klass)

struct _VObject
{
    const VClass* vptr;
};


API_MODULE __DECLARE_V_GET_TYPE(VObject)

#endif //DATALIBC_VOBJECT_H
