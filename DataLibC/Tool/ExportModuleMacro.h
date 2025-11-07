//
// Created by 18030 on 2024/11/13.
//

#ifndef TEST_EXPORTMODULEMACRO_H
#define TEST_EXPORTMODULEMACRO_H

#ifdef __cplusplus
#define EXTERN_C extern "C"
#else
#define EXTERN_C_
#endif


#ifdef EXPORT_MODULE
#define LIB_C_API EXTERN_C_ __declspec(dllexport)
#else
#define LIB_C_API EXTERN_C_ __declspec(dllimport)
#endif


#define LIB_STRUCT
#define LIB_INTERFACE struct
#define LIB_VTABLE struct
#define LIB_CLASS struct
#define ABSTRACT
#define DETAIL
#define INTERNAL

#define API_MODULE LIB_C_API


#define PRIVATE
#define READ_ONLY

#define EXTEND(x)

#define INHER

#if __STDC_VERSION__ > 201710L
#define ARG_CONVERT(arg) &(typeof(arg)){arg}
#endif

#define INIT_ASSIGN(obj, member, value) *(typeof_unqual(obj->member)*)((void*)obj + offsetof(typeof(*obj), member)) = value
#define PLUS_ASSIGN(obj, member, value) *(typeof_unqual(obj->member)*)((void*)obj + offsetof(typeof(*obj), member)) = obj->member + value


#if  WIN32 || WIN64
#define LIB_WIN 1
#elif  __linux__
#define LIB_LINUX 1
#endif


#endif //TEST_EXPORTMODULEMACRO_H
