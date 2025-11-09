#include "VMethod.h"

#include "DataLibC/DS/String/String.h"
#include "DataLibC/IO/Print.h"

void VDisplayMethodSignature(const VMethodInfo* methodInfo)
{
    String signature = { };
    String_Init(&signature, 0, "(Method): ");
    String_Append(&signature, VGetTypeName(methodInfo->returnType));
    String_Append(&signature, " ");
    String_Append(&signature, methodInfo->methodName);
    String_AppendChar(&signature, '(');
    for (int i = 0; i < methodInfo->paramCount; i++)
    {
        String_Append(&signature, VGetTypeName(methodInfo->paramTypes[i]));
        if (i < methodInfo->paramCount - 1)
            String_Append(&signature, ", ");
    }
    String_AppendChar(&signature, ')');
    print_any(String_ToString(&signature));
    String_Release(&signature);
}

typedef void (* InvokePtr)(void* object, void** args, void* out);

void VMethodInvoke(const VMethodInfo* methodInfo, void* object, void** params, void* out)
{
    const auto methodPtr = methodInfo->method;
    const auto invokePtr = (InvokePtr)methodPtr;
    invokePtr(object, params, out);
}
