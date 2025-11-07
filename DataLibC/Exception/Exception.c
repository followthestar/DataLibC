#include "Exception.h"


const int __BuiltinException_BaseException = 1;
const int __BuiltinException_RuntimeException = 2;
const int __BuiltinException_IOException = 3;
const int __BuiltinException_InvalidOperationException = 4;
const int __BuiltinException_IndexOutOfRangeException = 5;
const int __BuiltinException_NullPointerException = 6;
const int __BuiltinException_TypeCastException = 7;
const int __BuiltinException_ArithmeticException = 8;
const int __BuiltinException_ArgumentException = 8;


typedef struct ExceptionContext
{
    ExceptionHandle handle;
    const char* lastMessage;
} ExceptionContext;

static ExceptionContext gExceptionContext = { };

void BuildExceptionAndThrow(const ExceptionData exception)
{
    gExceptionContext.lastMessage = exception.message;
    longjmp(gExceptionContext.handle, exception.code);
}

ExceptionHandle* GetExceptionHandle()
{
    return &gExceptionContext.handle;
}

bool JudgeExceptionInternal(ExceptionData* outException, const int exception, const int code)
{
    if (exception == code)
    {
        outException->message = gExceptionContext.lastMessage;
        return true;
    }
    return false;
}
