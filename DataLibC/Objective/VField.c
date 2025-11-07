#include "VField.h"

#include <string.h>

#include "DataLibC/Exception/Exception.h"


void VFieldSetValue(const VFieldInfo* fieldInfo, void* object, const void* value)
{
    if (!fieldInfo)
        throw(NullPointerException, "Param fieldInfo cannot be nullptr");

    assert(fieldInfo);
    if (!object && !fieldInfo->isStatic)
        throw(ArgumentException, "Set value to a instance field must provide an object!");

    assert(object);
    if (fieldInfo->isStatic)
    {
        //TODO
        return;
    }

    char* ptr = (char*)object + fieldInfo->offset;
    memcpy_s(ptr, fieldInfo->size, value, fieldInfo->size);
}

void VFieldGetValue(const VFieldInfo* fieldInfo, void* object, void* out)
{
    if (!fieldInfo)
        throw(NullPointerException, "Param fieldInfo cannot be nullptr");

    assert(fieldInfo);
    if (!object && !fieldInfo->isStatic)
        throw(ArgumentException, "Set value to a instance field must provide an object!");

    assert(object);
    if (fieldInfo->isStatic)
    {
        //TODO
        return;
    }
    const char* ptr = (char*)object + fieldInfo->offset;
    memcpy_s(out, fieldInfo->size, ptr, fieldInfo->size);
}
