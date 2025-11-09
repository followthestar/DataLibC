//
// Created by 18030 on 2025/1/2.
//

#include "VType.h"

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#include "VObject.h"
#include "DataLibC/DS/BitSet/BitSet.h"
#include "DataLibC/DS/Map/SortedMap.h"
#include "DataLibC/DS/Pair/Pair.h"
#include "DataLibC/DS/String/String.h"
#include "DataLibC/GC/GC.h"
#include "DataLibC/IO/Print.h"
#include "DataLibC/Tool/LogMacro.h"
#include "VType_Internal.h"

typedef struct TypeSystem
{
    /// Is the type system initialized. (True means the builtin VTypes have been registered)
    bool isInit;

    /// The number of builtin types
    int builtinTypeCount;

    /// The number of all registered types
    int typeCount;

    /// This is a bitmap to record the inheritance relationship between types.
    BitSet* inheritMap;

    /// This is a map to record all registered types
    SortedMap typeMap;

    /// Enable garbage collection
    bool enableGC;
} TypeSystem;


static void DefaultInstanceFinalizer(vpointer instance)
{
    const VObject* obj = instance;
    const VClass* klass = obj->vptr;
    const VClass* super = klass;

    while (super)
    {
        if (super->instanceFinalize != nullptr)
            super->instanceFinalize(instance);
        super = super->super;
    }
}

static TypeSystem s_TypeSystem = {
    .isInit = false,
    .builtinTypeCount = 0,
    .typeCount = 0,
    .inheritMap = nullptr,
    .typeMap = { }
};

static constexpr int TYPEMAP_INIT_CAPACITY = 128;

static void SetInheritance(const VType child, const VType parent)
{
    const UInt32 inheritIndex = (child - 1) * TYPEMAP_INIT_CAPACITY + (parent - 1);
    BitSet_Set(s_TypeSystem.inheritMap, (int)inheritIndex, true);
}

extern VClass VOBJECT_CLASS;

void RegisterBuiltinTypeInternal(VClass* klass)
{
    if (klass != &VOBJECT_CLASS)
        klass->super = &VOBJECT_CLASS;
    if (klass->instanceFinalize == nullptr)
        klass->instanceFinalize = nullptr;
    SortedMap_Insert(&s_TypeSystem.typeMap, &(UInt32){ klass->typeId }, &klass);
    SetInheritance(klass->typeId, V_TYPE_OBJECT);

    s_TypeSystem.builtinTypeCount++;
    s_TypeSystem.typeCount++;
}


static void RegisterBuiltinVTypes()
{
    RegisterBuiltinTypeInternal(&VOBJECT_CLASS);

    s_TypeSystem.builtinTypeCount = V_TYPE_OBJECT;
    s_TypeSystem.typeCount = s_TypeSystem.builtinTypeCount;
}


static void CheckTypeSystem()
{
    if (!s_TypeSystem.isInit)
    {
        s_TypeSystem.inheritMap = BitSet_Create(TYPEMAP_INIT_CAPACITY * TYPEMAP_INIT_CAPACITY, false);
        SortedMap_Init(&s_TypeSystem.typeMap, kUINT, kPOINTER, nullptr, nullptr, nullptr);
        RegisterBuiltinVTypes();
        s_TypeSystem.isInit = true;
    }
}


VType RegisterNewVType(
    VType (* parentTypeGetType)(), const char* typeName, const VTypeInfo* typeInfo, VTypeFlag typeFlag)
{
    CheckTypeSystem();
    const VType super = parentTypeGetType();
    if (super == V_TYPE_INVALID)
        return V_TYPE_INVALID;

    VClass** klassPtr = typeInfo->klass;
    *klassPtr = malloc(sizeof(VClass));
    VClass* klass = *klassPtr;
    klass->super = VGetClassFromType(super);
    klass->typeId = ++s_TypeSystem.typeCount;

    if (VIsDerivedFrom(klass->super->typeId, klass->typeId))
        // Error! Recursive inheritance. Although it is impossible to reach here, just check it.
        return V_TYPE_INVALID;


    klass->className = typeName;
    klass->typeSize = typeInfo->typeSize;
    klass->ctorInfo = typeInfo->ctorInfo;
    klass->instanceInit = typeInfo->instanceInit;
    klass->instanceFinalize = typeInfo->dtor;
    klass->fieldInfo = typeInfo->memberInfos;
    klass->methodInfo = typeInfo->methodInfos;
    klass->classInit = typeInfo->classInit;
    klass->classFinalize = typeInfo->classFinalize;

    klass->classInit();

    SetInheritance(klass->typeId, klass->typeId);
    for (int i = 1; i <= klass->super->typeId; i++)
    {
        if (VIsDerivedFrom(klass->super->typeId, i))
            SetInheritance(klass->typeId, i);
    }
    SortedMap_Insert(&s_TypeSystem.typeMap, &(UInt32){ klass->typeId }, &klass);
    return klass->typeId;
}

static const char* GetBuiltinTypeName(const VType typeid)
{
    switch (typeid)
    {
        case V_TYPE_VOID:
            return "void";
        case V_TYPE_CHAR:
            return "char";
        case V_TYPE_UCHAR:
            return "uchar";
        case V_TYPE_SHORT:
            return "short";
        case V_TYPE_USHORT:
            return "ushort";
        case V_TYPE_INT:
            return "int";
        case V_TYPE_UINT:
            return "uint";
        case V_TYPE_LLONG:
            return "llong";
        case V_TYPE_ULLONG:
            return "ullong";
        case V_TYPE_FLOAT:
            return "float";
        case V_TYPE_DOUBLE:
            return "double";
        case V_TYPE_LDOUBLE:
            return "ldouble";
        case V_TYPE_BOOL:
            return "bool";
        case V_TYPE_POINTER:
            return "vpointer";
        case V_TYPE_STRING:
            return "string";
        case V_TYPE_OBJECT:
            return "VObject";
        default:
            return "?";
    }
}

extern bool EnsureGCSystemInitInternal();

bool VEnableGarbageCollection()
{
    CheckTypeSystem();
    if (s_TypeSystem.enableGC)
        return false;
    if (!EnsureGCSystemInitInternal())
        return false;
    s_TypeSystem.enableGC = true;
    return true;
}

const char* VGetTypeName(const VType typeid)
{
    if (typeid == V_TYPE_INVALID)
        return "Invalid type";
    if (typeid < V_TYPE_OBJECT)
    {
        // builtin type
        return GetBuiltinTypeName(typeid);
    }

    const VClass* klass = VGetClassFromType(typeid);
    if (klass == nullptr)
        return "Unknown type";
    return klass->className;
}

VClass* VGetClassFromType(VType typeid)
{
    CheckTypeSystem();
    return *(VClass**)SortedMap_KeyAt(&s_TypeSystem.typeMap, &typeid);
}

vpointer VObjectCastTo(VObject* object, const VType typeid)
{
    if (typeid == V_TYPE_INVALID)
        return nullptr;

    if (VIsDerivedFrom(object->vptr->typeId, typeid))
        return object;
    return nullptr;
}

bool VIsDerivedFrom(const VType child, const VType parent)
{
    CheckTypeSystem();
    const UInt32 inheritIndex = (child - 1) * TYPEMAP_INIT_CAPACITY + (parent - 1);
    return BitSet_Get(s_TypeSystem.inheritMap, (int)inheritIndex);
}

static void ConsumeArgs(char* fieldPtr, va_list* list, const VType fieldType, const bool assign)
{
    switch (fieldType)
    {
        case V_TYPE_CHAR:
        case V_TYPE_UCHAR:
        case V_TYPE_SHORT:
        case V_TYPE_USHORT:
        case V_TYPE_INT:
            {
                const int v = va_arg(*list, int);
                if (assign)
                    *(int*)fieldPtr = v;
                break;
            }
        case V_TYPE_UINT:
            {
                const unsigned int v = va_arg(*list, unsigned int);
                if (assign)
                    *(unsigned int*)fieldPtr = v;
                break;
            }
        case V_TYPE_LLONG:
            {
                const long long v = va_arg(*list, long long);
                if (assign)
                    *(long long*)fieldPtr = v;
                break;
            }
        case V_TYPE_ULLONG:
            {
                const unsigned long long v = va_arg(*list, unsigned long long);
                if (assign)
                    *(unsigned long long*)fieldPtr = v;
                break;
            }
        case V_TYPE_FLOAT:
        case V_TYPE_DOUBLE:
            {
                const double v = va_arg(*list, double);
                if (assign)
                    *(double*)fieldPtr = v;
                break;
            }
        case V_TYPE_BOOL:
            {
                const bool v = va_arg(*list, int);
                if (assign)
                    *(bool*)fieldPtr = v;
                break;
            }
        case V_TYPE_POINTER:
            {
                const vpointer v = va_arg(*list, vpointer);
                if (assign)
                    *(vpointer*)fieldPtr = v;
                break;
            }
        case V_TYPE_STRING:
            {
                const char* v = va_arg(*list, const char*);
                if (assign)
                    *(const char**)fieldPtr = v;
                break;
            }
        case V_TYPE_OBJECT:
            //TODO
            break;
        default:
            __builtin_unreachable();
    }
}

vpointer VNewObject(const VType typeid, const int paramCount, ...)
{
    if (typeid == V_TYPE_INVALID)
    {
        RuntimeErrorLog(InvalidArgument, "You cannot constructor an object of invalid type!");
        return nullptr;
    }

    const VClass* klass = VGetClassFromType(typeid);
    if (klass == nullptr)
    {
        RuntimeErrorLog(InvalidArgument, "You cannot constructor an object of unknown type!");
        return nullptr;
    }

    if (paramCount != klass->ctorInfo->paramCount)
    {
        RuntimeErrorLog(InvalidArgument, "Wrong parameters! Construct an object of type [%s] expects "
            "%d params but %d is provided.", klass->className, klass->ctorInfo->paramCount, paramCount);
        return nullptr;
    }
    va_list args, copyArgs;
    va_start(args, paramCount);
    va_copy(copyArgs, args);
    for (int i = 0; i < paramCount; i++)
    {
        const auto paramInfo = &klass->ctorInfo->paramInfos[i];
        const VType nextParamType = va_arg(args, VType);
        if (paramInfo->type != nextParamType)
        {
            RuntimeErrorLog(InvalidParameterType, "Wrong parameters! Construct an object of type [%s] expects "
                "param %d - %s to be of type [%s] but %s is provided.", klass->className, i, paramInfo->fieldName,
                VGetTypeName(paramInfo->type), VGetTypeName(nextParamType));
            return nullptr;
        }
        ConsumeArgs(nullptr, &args, nextParamType, false);
    }

    VObject* object;
    if (s_TypeSystem.enableGC)
    {
        object = GC_Malloc_Ext(klass->typeSize, DefaultInstanceFinalizer);
    }
    else
    {
        object = malloc(klass->typeSize);
    }
    memset(object, 0, klass->typeSize);
    object->vptr = klass;
    for (int i = 0; i < paramCount; i++)
    {
        [[maybe_unused]] VType _ = va_arg(copyArgs, VType);
        const auto paramInfo = &klass->ctorInfo->paramInfos[i];
        char* fieldPtr = (char*)object + paramInfo->offset;
        ConsumeArgs(fieldPtr, &copyArgs, paramInfo->type, true);
    }

    va_end(args);
    va_end(copyArgs);
    return object;
}

void VDisplayConstructorSignature(const VType typeid)
{
    if (typeid == V_TYPE_INVALID)
        return;
    const VClass* klass = VGetClassFromType(typeid);
    const auto info = klass->ctorInfo;
    if (info->paramCount == 0)
        print_fmt("Type: {}, Constructor: (void)", klass->className);
    else
    {
        String signature = { };
        String_Init(&signature, 0, "Type: ");
        String_Append(&signature, klass->className);
        String_Append(&signature, ", Constructor: (");
        for (int i = 0; i < info->paramCount; i++)
        {
            const auto param = &info->paramInfos[i];
            String_Append(&signature, VGetTypeName(param->type));
            String_AppendChar(&signature, ' ');
            String_Append(&signature, param->fieldName);
            if (i != info->paramCount - 1)
                String_Append(&signature, ", ");
        }
        String_AppendChar(&signature, ')');
        print_any(String_ToString(&signature));
        String_Release(&signature);
    }
}

VClass* VGetClassFromName(const char* name)
{
    CheckTypeSystem();
    auto iter = GetIterator(&s_TypeSystem.typeMap);
    while (HasNext(iter))
    {
        const Pair* p = Next(iter);
        const auto klass = *(VClass**)Second(*p);
        if (strcmp(klass->className, name) == 0)
            return klass;
    }
    return nullptr;
}

static void CastFieldToString(const struct _VFieldInfo* fieldInfo, void* object, char* buf, size_t bufSize)
{
    char* fieldPtr = (char*)object + fieldInfo->offset;
    switch (fieldInfo->type)
    {
        case V_TYPE_CHAR:
        case V_TYPE_UCHAR:
        case V_TYPE_SHORT:
        case V_TYPE_USHORT:
        case V_TYPE_INT:
            {
                sprintf_s(buf, bufSize, "%d", *(int*)fieldPtr);
                break;
            }
        case V_TYPE_UINT:
            {
                sprintf_s(buf, bufSize, "%u", *(unsigned int*)fieldPtr);
                break;
            }
        case V_TYPE_LLONG:
            {
                sprintf_s(buf, bufSize, "%lld", *(long long*)fieldPtr);
                break;
            }
        case V_TYPE_ULLONG:
            {
                sprintf_s(buf, bufSize, "%llu", *(unsigned long long*)fieldPtr);
                break;
            }
        case V_TYPE_FLOAT:
            {
                sprintf_s(buf, bufSize, "%g", *(float*)fieldPtr);
                break;
            }
        case V_TYPE_DOUBLE:
            {
                sprintf_s(buf, bufSize, "%g", *(double*)fieldPtr);
                break;
            }
        case V_TYPE_BOOL:
            {
                sprintf_s(buf, bufSize, "%s", *(bool*)fieldPtr ? "true" : "false");
                break;
            }
        case V_TYPE_POINTER:
            {
                sprintf_s(buf, bufSize, "%p", *(void**)fieldPtr);
                break;
            }
        case V_TYPE_STRING:
            {
                sprintf_s(buf, bufSize, "%s", *(const char**)fieldPtr);
                break;
            }
        case V_TYPE_OBJECT:
            //TODO
            break;
        default:
            __builtin_unreachable();
    }
}

void VDump(const void* object)
{
    const auto obj = (VObject*)object;
    const VClass* klass = obj->vptr;
    const auto fields = klass->fieldInfo;
    String dumpStr = { };
    String_Init(&dumpStr, 0, "(object) ");
    String_Append(&dumpStr, klass->className);
    String_Append(&dumpStr, ": {\n");
    char buf[512] = { };
    for (int i = 0; i < fields->paramCount; i++)
    {
        const auto field = &fields->paramInfos[i];
        CastFieldToString(field, obj, buf, 512);
        String_AppendChar(&dumpStr, '\t');
        String_Append(&dumpStr, field->fieldName);
        String_Append(&dumpStr, ": ");
        String_Append(&dumpStr, buf);
        String_AppendChar(&dumpStr, '\n');
    }
    String_AppendChar(&dumpStr, '}');
    print_any(String_ToString(&dumpStr));
    String_Release(&dumpStr);
}

const VFieldInfo* VGetFieldInfo(const VClass* klass, const char* fieldName)
{
    const auto fieldInfos = klass->fieldInfo;
    for (int i = 0; i < fieldInfos->paramCount; i++)
    {
        const auto fieldInfo = &fieldInfos->paramInfos[i];
        if (strcmp(fieldInfo->fieldName, fieldName) == 0)
            return fieldInfo;
    }
    return nullptr;
}

const VMethodInfo* VGetMethodInfo(const VClass* klass, const char* methodName, const VAccessFlag accessFlag)
{
    const auto methodInfos = klass->methodInfo;
    for (int i = 0; i < methodInfos->methodCount; i++)
    {
        const auto methodInfo = &methodInfos->methodInfos[i];
        if ((methodInfo->accessFlag & accessFlag) != 0 && strcmp(methodInfo->methodName, methodName) == 0)
            return methodInfo;
    }
    return nullptr;
}

