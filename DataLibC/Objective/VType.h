//
// Created by 18030 on 2025/1/2.
//

#ifndef DATALIBC_VTYPE_H
#define DATALIBC_VTYPE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <stddef.h>
#include <stdint.h>
#include <pthread.h>
#include "DataLibC/Tool/TypeMacro.h"
#include "DataLibC/Tool/Magic.h"

#pragma region Type Declare
typedef UInt32 VType;
typedef char vchar;
typedef short vshort;
typedef int vint;
typedef long long vllong;
typedef unsigned char vuchar;
typedef unsigned short vushort;
typedef unsigned int vuint;
typedef unsigned long long vullong;
typedef float vfloat;
typedef double vdouble;
typedef long double vldouble;
typedef bool vbool;
typedef void* vpointer;
typedef void vnull;
//typedef atomic_flag vatomicflag;
//typedef pthread_once_t vonce_flag;

/* typedef structures */
typedef struct _VValue VValue;
typedef struct _VTypeInfo VTypeInfo;
typedef struct _VTypeInstance VTypeInstance;


typedef struct _VObject VObject;
typedef struct _VClass VClass;

typedef void (* VClassInitFunc)();

typedef void (* VClassFinalizeFunc)(vpointer klass);

typedef void (* VInstanceInitFunc)(vpointer instance);

typedef void (* VInstanceFinalizeFunc)(vpointer instance);

typedef enum _VTypeFlag
{
    VTYPE_FLAG_NONE = 0,
    VTYPE_FLAG_IS_SEALED = 1 << 0,
    VTYPE_FLAG_IS_ABSTRACT = 1 << 1,
    VTYPE_FLAG_IS_INTERFACE = 1 << 2,
    VTYPE_FLAG_IS_BASE = 1 << 3,
    VTYPE_FLAG_PUBLIC_ONLY = 1 << 4
} VTypeFlag;

#pragma endregion

#define __V_MAKE_BUILTIN_TYPE(seq) (seq)

#pragma region Builtin VType
#define V_TYPE_INVALID                  __V_MAKE_BUILTIN_TYPE(0)
#define V_TYPE_NULL                     __V_MAKE_BUILTIN_TYPE(1)
#define V_TYPE_CHAR                     __V_MAKE_BUILTIN_TYPE(2)
#define V_TYPE_UCHAR                    __V_MAKE_BUILTIN_TYPE(3)
#define V_TYPE_SHORT                    __V_MAKE_BUILTIN_TYPE(4)
#define V_TYPE_USHORT                   __V_MAKE_BUILTIN_TYPE(5)
#define V_TYPE_INT                      __V_MAKE_BUILTIN_TYPE(6)
#define V_TYPE_UINT                     __V_MAKE_BUILTIN_TYPE(7)
#define V_TYPE_LLONG                    __V_MAKE_BUILTIN_TYPE(8)
#define V_TYPE_ULLONG                   __V_MAKE_BUILTIN_TYPE(9)
#define V_TYPE_FLOAT                    __V_MAKE_BUILTIN_TYPE(10)
#define V_TYPE_DOUBLE                   __V_MAKE_BUILTIN_TYPE(11)
#define V_TYPE_LDOUBLE                  __V_MAKE_BUILTIN_TYPE(12)
#define V_TYPE_BOOL                     __V_MAKE_BUILTIN_TYPE(13)
#define V_TYPE_POINTER                  __V_MAKE_BUILTIN_TYPE(14)
#define V_TYPE_STRING                   __V_MAKE_BUILTIN_TYPE(15)
#define V_TYPE_OBJECT                   __V_MAKE_BUILTIN_TYPE(16)
#pragma endregion

#define V_GET_TYPE(type)    type##_get_type()
#define __V_GET_TYPE_FUNC(type) type##_get_type
#define __DECLARE_VTYPE_INIT_FUNC(newType) void newType##_init(newType* self)
#define __DECLARE_VTYPE_CLASS_INIT_FUNC(newType) void newType##_class_init()

#define V_TYPE_IS_DERIVED_FROM(derived, base) VIsDerivedFrom(V_GET_TYPE(derived), V_GET_TYPE(base))


#define __VTYPE_INIT_FUNC(newType) newType##_init
#define __VTYPE_CLASS_INIT_FUNC(newType) newType##_class_init

#define DEFINE_VTYPE_INIT(newType) void newType##_init(newType* self)
#define DEFINE_VTYPE_CLASS_INIT(newType) void newType##_class_init()

#define __DECLARE_V_GET_TYPE(newType) VType newType##_get_type();
#define __DEFINE_V_GET_TYPE(newType)  VType newType##_get_type()

/**
 * @brief Declare a new type with a method to get its type and generate a type class
 * @param newType A new type name
 * @attention This macro should be used in .h file
 */
#define DECLARE_VTYPE(newType)                              \
ENSURE_ARG_EXIST(newType);                                  \
__DECLARE_V_GET_TYPE(newType)                               \
typedef struct _##newType newType;                          \
__DECLARE_VTYPE_INIT_FUNC(newType);                         \
__DECLARE_VTYPE_CLASS_INIT_FUNC(newType);

#pragma region Getter/Setter/Property

#define DECLARE_VTYPE_PROPERTY(vType, propertyType, fieldName)      \
DECLARE_VTYPE_GETTER(vType, propertyType, fieldName);               \
DECLARE_VTYPE_SETTER(vType, propertyType, fieldName);

#define DECLARE_VTYPE_GETTER(vType, propertyType, fieldName) \
ENSURE_ARG_EXIST(fieldName);                                 \
propertyType vType##_get_##fieldName(vType* instance)

#define DECLARE_VTYPE_SETTER(vType, propertyType, fieldName) \
ENSURE_ARG_EXIST(fieldName);                                 \
void vType##_set_##fieldName(vType* instance, propertyType value)

#define V_GET_PROPERTY(vType, fieldName, instance) vType##_get_##fieldName(instance)
#define V_SET_PROPERTY(vType, fieldName, instance, value) vType##_set_##fieldName(instance, value)

#pragma endregion

#pragma region Ctor/Dtor
#define DEFINE_CONSTRUCTOR(...) __VA_ARGS__
#define DEFAULT_CONSTRUCTOR DEFINE_CONSTRUCTOR()
#define DEFINE_DESTRUCTOR(type) void type##_destructor(type* self)
#define V_DESTRUCTOR(type) type##_destructor
#define DEFAULT_DESTRUCTOR nullptr
#pragma endregion

#define __VTYPE_CLASS_INSTANCE(type) type##_class_instance

#define __REGISTER_VTYPE_META(newType, constructor)             \
static vint newType##_private_offset = 0;                       \
static VClass* newType##_class_instance = nullptr;              \
__GEN_CONSTRUCTOR_INFO(newType, constructor)


#define __DEFINE_VTYPE_REGISTER(newType, parentType, destructor)            \
static VType newType##_get_type_internal()                                  \
{                                                                           \
    static VTypeInfo define_type_info = {                                   \
         .typeSize = sizeof(newType),                                       \
         .klass = &__VTYPE_CLASS_INSTANCE(newType),                         \
         .ctorInfo = &__GET_TYPE_CONSTRUCTOR_INFO(newType),                 \
         .dtor = (VInstanceFinalizeFunc)destructor,                         \
         .memberInfos = &newType##_fields,                                  \
         .classInit = __VTYPE_CLASS_INIT_FUNC(newType),                     \
         .classFinalize = nullptr,                                          \
         .instanceInit = (VInstanceInitFunc)__VTYPE_INIT_FUNC(newType)      \
    };                                                                      \
    return RegisterNewVType(__V_GET_TYPE_FUNC(parentType), #newType, &define_type_info, VTYPE_FLAG_NONE);\
}                                                       \
VType newType##_get_type()                              \
{                                                       \
    static VType static_type_id = 0;                    \
    if (static_type_id == 0)                            \
        static_type_id =  newType##_get_type_internal();\
    return static_type_id;                              \
}

#define DEFINE_VTYPE_EMPTY_INIT(newType)    \
DEFINE_VTYPE_INIT(newType){}\
DEFINE_VTYPE_CLASS_INIT(newType){}

#define DEFINE_VTYPE_SIMPLE(newType, fields, methods)    \
    DEFINE_VTYPE(newType, VObject, DECLARE_FIELDS(fields), DECLARE_METHODS(methods))

#define DEFINE_FINAL_VTYPE_SIMPLE(newType) DEFINE_FINAL_VTYPE_SIMPLE(newType, VObject)

#define DEFINE_FINAL_VTYPE(newType, parentType)

#define DECLARE_MEMBER(type, name) type, name

#define __DEFINE_MEMBER(type, name) type name;

#define __GEN_VTYPE_PROPERTY(newType, ...) \
    EXPAND_ARGS2_FIXED(DECLARE_VTYPE_PROPERTY, newType, __VA_ARGS__)

#define __EXPAND_GETTER_SETTER_DECL(newType, ...) \
__GEN_VTYPE_PROPERTY(newType, __VA_ARGS__)

#define __GEN_FIELD_INFO(type, ...)                          \
    EXPAND_ARGS2_FIXED(__FIELD_INFO_WITH_TYPE_PROVIDE, type, __VA_ARGS__)

#define DECLARE_FIELDS(...) __VA_ARGS__
#define DECLARE_METHODS(...) __VA_ARGS__
/**
 * @brief Define a new type with super type and members
 * @param newType A new type name
 * @param parentType The parent type of the new type
 * @param ... The members of the new type
 * @attention This macro should be used in .h file
 */
#define DEFINE_VTYPE(newType, parentType, fields, methods)          \
DECLARE_VTYPE(newType)                                              \
typedef struct _##newType{                                          \
    const parentType super;                                         \
    EXPAND_ARGS2(__DEFINE_MEMBER, fields)                           \
} newType;                                                          \
constexpr _VTypeMemberFieldInfo newType##_fields = {                \
    .paramCount = COUNT_ARG2(fields),                               \
    .paramInfos = {                                                 \
        __GEN_FIELD_INFO(newType, fields)                           \
    }                                                               \
};

#pragma region Type Generic
#define __V_GET_TYPE_FROM_ARG(arg) _Generic((arg),          \
    int: V_TYPE_INT,                                        \
    unsigned int: V_TYPE_UINT,                              \
    float: V_TYPE_FLOAT,                                    \
    double: V_TYPE_DOUBLE,                                  \
    char: V_TYPE_CHAR,                                      \
    unsigned char: V_TYPE_UCHAR,                            \
    short: V_TYPE_SHORT,                                    \
    unsigned short: V_TYPE_USHORT,                          \
    long long: V_TYPE_LLONG,                                \
    unsigned long long: V_TYPE_ULLONG,                      \
    bool: V_TYPE_BOOL,                                      \
    char*: V_TYPE_STRING,                                   \
    const char*: V_TYPE_STRING,                             \
    void*: V_TYPE_POINTER,                                  \
    float*: V_TYPE_POINTER,                                 \
    int*: V_TYPE_POINTER,                                   \
    double*: V_TYPE_POINTER,                                \
    bool*: V_TYPE_POINTER,                                  \
    const void*: V_TYPE_POINTER,                            \
    default: V_TYPE_OBJECT                                  \
)
#pragma endregion

#define __GET_MEMBER_OFFSET(type, member) offsetof(type, member)
#define __GET_MEMBER_NAME(member) #member
#define __GET_MEMBER_TYPE(type, member) __V_GET_TYPE_FROM_ARG(((type*)nullptr)->member)

#define __FIELD_INFO_NO_TYPE_PROVIDE(vtype, member)         \
{                                                           \
.isStatic = false,                                          \
.fieldName = __GET_MEMBER_NAME(member),                     \
.size = sizeof(((vtype*)nullptr)->member),                  \
.offset = __GET_MEMBER_OFFSET(vtype, member),               \
.type = __GET_MEMBER_TYPE(vtype, member)                    \
}

#define __FIELD_INFO_WITH_TYPE_PROVIDE(vtype, memberType, member)   \
{                                                                   \
    .isStatic = false,                                              \
    .fieldName = __GET_MEMBER_NAME(member),                         \
    .size = sizeof(((vtype*)nullptr)->member),                      \
    .offset = __GET_MEMBER_OFFSET(vtype, member),                   \
    .type = __GET_MEMBER_TYPE(vtype, member)                        \
}

#define __GEN_CTOR_FIELD_INFO(type, ...)                          \
    EXPAND_ARGS_FIXED(__FIELD_INFO_NO_TYPE_PROVIDE, type, __VA_ARGS__)


#define __GEN_CONSTRUCTOR_INFO(type, ...)                       \
static const struct _VTypeConstructorInfo type##_ctor_info = {  \
           .paramCount = COUNT_ARG(__VA_ARGS__),                \
           .paramInfos = {                                      \
               __GEN_CTOR_FIELD_INFO(type, ##__VA_ARGS__)            \
           }                                                    \
};

#define __GET_TYPE_CONSTRUCTOR_INFO(type) type##_ctor_info

/**
 * @brief Register a new type into the type system
 * @param newType A new type name
 * @attention This macro should be used in .c file
 */
#define REGISTER_VTYPE_SIMPLE(newType, constructor, destructor)\
    REGISTER_VTYPE(newType, VObject, DEFINE_CONSTRUCTOR(constructor), destructor)

/**
 * @brief Register a new type with given super type into the type system
 * @param newType A new type name
 * @param parentType The parent type of the new type
 * @attention This macro should be used in .c file
 */
#define REGISTER_VTYPE(newType, parentType, constructor, destructor)        \
__REGISTER_VTYPE_META(newType, DEFINE_CONSTRUCTOR(constructor))             \
__DEFINE_VTYPE_REGISTER(newType, parentType, destructor)


API_MODULE VType
RegisterNewVType(
    VType (* parentTypeGetType)(), const char* typeName, const VTypeInfo* typeInfo, VTypeFlag typeFlag);

API_MODULE vpointer GetParentClass(const VClass* klass);


typedef struct _VFieldInfo
{
    bool isStatic;
    const char fieldName[64];
    int size;
    int offset;
    VType type;
} VFieldInfo;

struct _VTypeConstructorInfo
{
    int paramCount;
    struct _VFieldInfo paramInfos[];
};

typedef struct _VTypeConstructorInfo _VTypeMemberFieldInfo;

typedef enum VAccessFlag
{
    Default = 0,
    ENUM_FLAG(Public, 1),
    ENUM_FLAG(NonPublic, 2),
    ENUM_FLAG(Static, 3),
    ENUM_FLAG(Instance, 4)
} VAccessFlag;

typedef struct _VMethodInfo
{
    const char methodName[64];
    VAccessFlag accessFlag;
    void* method;
    VType returnType;
    VType paramTypes[];
} VMethodInfo;

typedef struct _VTypeMethodInfo
{
    int methodCount;
    int publicMethodCount;
    int privateMethodCount;
    struct _VMethodInfo methodInfos[];
} _VTypeMemberMethodInfo;

struct _VTypeInfo
{
    vuint typeSize;
    VClass** klass;
    const struct _VTypeConstructorInfo* ctorInfo;
    VInstanceFinalizeFunc dtor;
    const _VTypeMemberFieldInfo* memberInfos;
    VClassInitFunc classInit;
    VClassFinalizeFunc classFinalize;
    VInstanceInitFunc instanceInit;
};

#define __EXTRACT_FIELD_INFO(...)

#define __EXTRACT_ARGS_TYPE(arg) __V_GET_TYPE_FROM_ARG(arg), arg

#define V_OBJECT_NEW(type, ...) \
(type*)VNewObject(V_GET_TYPE(type), COUNT_ARG(__VA_ARGS__) __VA_OPT__(,) EXPAND_ARGS(__EXTRACT_ARGS_TYPE, __VA_ARGS__))

#define V_DYNAMIC_CAST(instance, type) (type*)VObjectCastTo(instance, V_GET_TYPE(type))

#define V_GET_CLASS(type) VGetClassFromType(V_GET_TYPE(type))

/*------------------------------------------------------------------------------------------*/


/**
 * @brief Enable GC to allow the type system to manage the lifetime of all objects constructed on the heap through
 * functions or macros such as \code V_OBJECT_NEW\endcode ,\code VNewObjec\endcode , ect
 * @warning If this function is called, you cannot disable it in the same process!
 * @attention To ensure proper functionality, please check that the GC system has been initialized before calling
 * this function. (Use \code GC_AUTO_INIT\endcode macro)
 */
API_MODULE bool VEnableGarbageCollection();

/**
 * @brief Get a constant string of the type name
 * @param typeid The type id in the type system. (Use \code V_GET_TYPE(type)\endcode to get the type id)
 * @return A constant string
 */
API_MODULE const char* VGetTypeName(VType typeid);

/**
 * @brief Get the class of the given type
 * @param typeid The type id in the type system. (Use \code V_GET_TYPE(type)\endcode to get the type id)
 * @return The base class of the given type. You can cast it to child class manually but this is not recommended!
 */
API_MODULE VClass* VGetClassFromType(VType typeid);

/**
 * @brief Cast a VObject object to another type
 * @param object The object to be cast
 * @param typeid The type id in the type system. (Use \code V_GET_TYPE(type)\endcode to get the type id)
 * @return The target object of given type. If the cast failed, return nullptr
 */
API_MODULE vpointer VObjectCastTo(VObject* object, VType typeid);

/**
 * @brief Check if the child type is derived from the parent type
 * @param child The child type
 * @param parent The parent type
 * @return True if the child type is derived from the parent type
 * @attention True is always returned when the child type is equal to the parent type
 */
API_MODULE bool VIsDerivedFrom(VType child, VType parent);

/**
 * @brief Create a new object of the given type
 * @param typeid The type id in the type system. (Use \code V_GET_TYPE(type)\endcode to get the type id)
 * @param paramCount The number of parameters
 * @param ... The parameters
 * @return The new object
 * @attention This function will call the constructor of the given type
 */
API_MODULE vpointer VNewObject(VType typeid, int paramCount, ...);

/**
 * @brief Display the constructor signature of the given type
 * @param typeid The type id in the type system. (Use \code V_GET_TYPE(type)\endcode to get the type id)
 * @note This method is only used to test
 */
API_MODULE void VDisplayConstructorSignature(VType typeid);

/**
 * @brief Get the class of the given type name
 * @param name The type name
 * @return The class object represents the given type
 * @attention Due to the limitation of the C language, this function can only return the class of type which has been
 * registered to the type system directly or indirectly.
 * For example:
 * \code
 * struct Derived extends struct Base. And you called any methods or macros in type system to get the type of the struct,
 * or used any methods about type, such as V_NEW_OBJECT, V_GET_TYPE, etc.
 * // .....
 * auto type = V_GET_TYPE(Derived);
 * // .....
 * \endcode
 * Here you can get the class of Derived and Base through this method because the type system registers the types from
 * derived to base. (Derived -> Base)
 * So if you use this method to get a type that you haven't used, nullptr will be returned.
 */
API_MODULE VClass* VGetClassFromName(const char* name);

/**
 * @brief Dump the object to the console
 * @param object The object that can be converted to VObject
 */
API_MODULE void VDump(const void* object);

/**
 * @brief Get the field info of the given class
 * @param klass The class
 * @param fieldName The field name
 * @return The field info of the given class
 */
API_MODULE const VFieldInfo* VGetFieldInfo(const VClass* klass, const char* fieldName);

/**
 * @brief Get the method info of the given class
 * @param klass The class
 * @param methodName The method name
 * @param accessFlag Configure the searching range (Public, NonPublic, Static, Instance, etc.)
 * @return The method info of the given class
 */
API_MODULE const VMethodInfo* VGetMethodInfo(const VClass* klass, const char* methodName, VAccessFlag accessFlag);

#endif //DATALIBC_VTYPE_H
