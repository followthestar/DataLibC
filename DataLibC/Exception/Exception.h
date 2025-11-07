/**
  ******************************************************************************
  * @file           : Exception.h
  * @author         : 18030
  * @brief          : None
  * @attention      : None
  * @date           : 2025/11/1
  ******************************************************************************
  */


#ifndef DATALIBC_EXCEPTION_H
#define DATALIBC_EXCEPTION_H

#ifndef __cplusplus

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <setjmp.h>
typedef int ExceptionCode;

typedef typeof(jmp_buf) ExceptionHandle;

typedef struct _ExceptionData
{
    int code;
    const char* message;
} ExceptionData;

#define try                                                 \
ExceptionData e = {};                                       \
int exCode = setjmp(*GetExceptionHandle());                 \
if (exCode == 0)

#define catch(exception)  else if(JudgeExceptionInternal(&e, exception, exCode))

#define finally

#define throw(exception, message) BuildExceptionAndThrow(__BuildException(exception, message))

#define __BuildException(code, message) (ExceptionData){code, message}

API_MODULE const int __BuiltinException_BaseException;
API_MODULE const int __BuiltinException_RuntimeException;
API_MODULE const int __BuiltinException_IOException;
API_MODULE const int __BuiltinException_InvalidOperationException;
API_MODULE const int __BuiltinException_IndexOutOfRangeException;
API_MODULE const int __BuiltinException_NullPointerException;
API_MODULE const int __BuiltinException_TypeCastException;
API_MODULE const int __BuiltinException_ArithmeticException;
API_MODULE const int __BuiltinException_ArgumentException;

#define Exception __BuiltinException_BaseException

#define RuntimeException __BuiltinException_RuntimeException

#define IOException __BuiltinException_IOException

#define InvalidOperationException __BuiltinException_InvalidOperationException

#define IndexOutOfRangeException __BuiltinException_IndexOutOfRangeException

#define NullPointerException __BuiltinException_NullPointerException

#define TypeCastException __BuiltinException_TypeCastException

#define ArithmeticException __BuiltinException_ArithmeticException

#define ArgumentException __BuiltinException_ArgumentException

API_MODULE void BuildExceptionAndThrow(ExceptionData exception);

API_MODULE ExceptionHandle* GetExceptionHandle();

API_MODULE bool JudgeExceptionInternal(ExceptionData* outException, int exception, int code);

#endif

#endif //DATALIBC_EXCEPTION_H
