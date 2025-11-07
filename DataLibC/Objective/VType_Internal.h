/**
  ******************************************************************************
  * @file           : VType_Internal.h
  * @author         : 18030
  * @brief          : None
  * @attention      : None
  * @date           : 2025/11/6
  ******************************************************************************
  */


#ifndef DATALIBC_VTYPE_INTERNAL_H
#define DATALIBC_VTYPE_INTERNAL_H

#include "VType.h"

struct _VClass
{
    VClass* super;
    VType typeId;
    const char* className;
    vuint typeSize;
    const struct _VTypeConstructorInfo* ctorInfo;
    VClassInitFunc classInit;
    VClassFinalizeFunc classFinalize;
    VInstanceInitFunc instanceInit;
    VInstanceFinalizeFunc instanceFinalize;

    // refl
    const _VTypeMemberFieldInfo* fieldInfo;
    const _VTypeMemberMethodInfo* methodInfo;
};

#endif //DATALIBC_VTYPE_INTERNAL_H
