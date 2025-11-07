//
// Created by 18030 on 2025/8/27.

#ifndef MAGIC_H
#define MAGIC_H

#include "DataLibC/DS/Iterator.h"
#include <stdio.h>
#include <assert.h>
#define in ,

#define __foreach_impl(expr,  collection)\
for (auto collection##iter = GetIterator(&collection); HasNext(collection##iter) && (expr = Next(collection##iter));)

#define foreach(...) __foreach_impl(__VA_ARGS__)

#ifndef println
#define println(fmt, ...) printf(fmt "\n" __VA_OPT__(,__VA_ARGS__))
#endif

#define __DOUBLE(x) x, x

#define __SELECT(_1,_2,_3,_4,_5,_6,_7, _8, _9, _10, _11, _12, _13, _14, _15, _16, T, ...) T

#define __SELECT2(_1, _1_1, _2, _2_2, _3, _3_3, _4, _4_4, _5, _5_5, _6, _6_6, _7, _7_7, _8, _8_8, _9, _9_9, _10, _1010,\
    _11, _1111, _12, _1212, _13, _1313, _14, _1414, _15, _1515, _16, _1616, T, TT, ...) TT

/**
 * @brief Used to get the number of arguments passed to a macro.
 */
#define COUNT_ARG(...) __SELECT(__VA_ARGS__ __VA_OPT__(,) 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define COUNT_ARG2(...) __SELECT2(__VA_ARGS__ __VA_OPT__(,) 16, 16, 15, 15, 14, 14, 13, 13, 12, 12, 11, 11, 10, 10, 9, 9,\
8, 8, 7, 7, 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 1, 1, 0, 0)

#define EXPAND_0(...)
#define EXPAND_1(func, var) func(var)
#define EXPAND_2(func, var, ...) func(var), EXPAND_1(func, __VA_ARGS__)
#define EXPAND_3(func, var, ...) func(var), EXPAND_2(func, __VA_ARGS__)
#define EXPAND_4(func, var, ...) func(var), EXPAND_3(func, __VA_ARGS__)
#define EXPAND_5(func, var, ...) func(var), EXPAND_4(func, __VA_ARGS__)
#define EXPAND_6(func, var, ...) func(var), EXPAND_5(func, __VA_ARGS__)
#define EXPAND_7(func, var, ...) func(var), EXPAND_6(func, __VA_ARGS__)
#define EXPAND_8(func, var, ...) func(var), EXPAND_7(func, __VA_ARGS__)
#define EXPAND_9(func, var, ...) func(var), EXPAND_8(func, __VA_ARGS__)
#define EXPAND_10(func, var, ...) func(var), EXPAND_9(func, __VA_ARGS__)
#define EXPAND_11(func, var, ...) func(var), EXPAND_10(func, __VA_ARGS__)
#define EXPAND_12(func, var, ...) func(var), EXPAND_11(func, __VA_ARGS__)
#define EXPAND_13(func, var, ...) func(var), EXPAND_12(func, __VA_ARGS__)
#define EXPAND_14(func, var, ...) func(var), EXPAND_13(func, __VA_ARGS__)
#define EXPAND_15(func, var, ...) func(var), EXPAND_14(func, __VA_ARGS__)
#define EXPAND_16(func, var, ...) func(var), EXPAND_15(func, __VA_ARGS__)

#define EXPAND_FIXED0(...)
#define EXPAND_FIXED1(func, fixed, var) func(fixed, var)
#define EXPAND_FIXED2(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED1(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED3(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED2(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED4(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED3(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED5(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED4(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED6(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED5(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED7(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED6(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED8(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED7(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED9(func, fixed, var, ...) func(fixed, var), EXPAND_FIXED8(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED10(func, fixed, var, ...) func(var), EXPAND_FIXED9(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED11(func, fixed, var, ...) func(var), EXPAND_FIXED10(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED12(func, fixed, var, ...) func(var), EXPAND_FIXED11(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED13(func, fixed, var, ...) func(var), EXPAND_FIXED12(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED14(func, fixed, var, ...) func(var), EXPAND_FIXED13(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED15(func, fixed, var, ...) func(var), EXPAND_FIXED14(func, fixed, __VA_ARGS__)
#define EXPAND_FIXED16(func, fixed, var, ...) func(var), EXPAND_FIXED15(func, fixed, __VA_ARGS__)

#define EXPAND2_0(...)
#define EXPAND2_1(func, var1, var2) func(var1, var2)
#define EXPAND2_2(func, var1, var2, ...) func(var1, var2) EXPAND2_1(func, __VA_ARGS__)
#define EXPAND2_3(func, var1, var2, ...) func(var1, var2) EXPAND2_2(func, __VA_ARGS__)
#define EXPAND2_4(func, var1, var2, ...) func(var1, var2) EXPAND2_3(func, __VA_ARGS__)
#define EXPAND2_5(func, var1, var2, ...) func(var1, var2) EXPAND2_4(func, __VA_ARGS__)
#define EXPAND2_6(func, var1, var2, ...) func(var1, var2) EXPAND2_5(func, __VA_ARGS__)
#define EXPAND2_7(func, var1, var2, ...) func(var1, var2) EXPAND2_6(func, __VA_ARGS__)
#define EXPAND2_8(func, var1, var2, ...) func(var1, var2) EXPAND2_7(func, __VA_ARGS__)
#define EXPAND2_9(func, var1, var2, ...) func(var1, var2) EXPAND2_8(func, __VA_ARGS__)
#define EXPAND2_10(func, var1, var2, ...) func(var1, var2) EXPAND2_9(func, __VA_ARGS__)
#define EXPAND2_11(func, var1, var2, ...) func(var1, var2) EXPAND2_10(func, __VA_ARGS__)
#define EXPAND2_12(func, var1, var2, ...) func(var1, var2) EXPAND2_11(func, __VA_ARGS__)
#define EXPAND2_13(func, var1, var2, ...) func(var1, var2) EXPAND2_12(func, __VA_ARGS__)
#define EXPAND2_14(func, var1, var2, ...) func(var1, var2) EXPAND2_13(func, __VA_ARGS__)
#define EXPAND2_15(func, var1, var2, ...) func(var1, var2) EXPAND2_14(func, __VA_ARGS__)
#define EXPAND2_16(func, var1, var2, ...) func(var1, var2) EXPAND2_15(func, __VA_ARGS__)

#define EXPAND2_FIXED0(...)
#define EXPAND2_FIXED1(func, fixed, var1, var2) func(fixed, var1, var2)
#define EXPAND2_FIXED2(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED1(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED3(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED2(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED4(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED3(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED5(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED4(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED6(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED5(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED7(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED6(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED8(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED7(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED9(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED8(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED10(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED9(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED11(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED10(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED12(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED11(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED13(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED12(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED14(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED13(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED15(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED14(func, fixed, __VA_ARGS__)
#define EXPAND2_FIXED16(func, fixed, var1, var2, ...) func(fixed, var1, var2), EXPAND2_FIXED15(func, fixed, __VA_ARGS__)

#define EXPAND_ARGS(func, ...)      \
__SELECT(__VA_ARGS__ __VA_OPT__(,)  \
    EXPAND_16,                      \
    EXPAND_15,                      \
    EXPAND_14,                      \
    EXPAND_13,                      \
    EXPAND_12,                      \
    EXPAND_11,                      \
    EXPAND_10,                      \
    EXPAND_9,                       \
    EXPAND_8,                       \
    EXPAND_7,                       \
    EXPAND_6,                       \
    EXPAND_5,                       \
    EXPAND_4,                       \
    EXPAND_3,                       \
    EXPAND_2,                       \
    EXPAND_1,                       \
    EXPAND_0)                       \
(func, __VA_ARGS__)

#define EXPAND_ARGS_FIXED(func, fixed, ...)        \
__SELECT(__VA_ARGS__ __VA_OPT__(,)                 \
EXPAND_FIXED16,               \
EXPAND_FIXED15,               \
EXPAND_FIXED14,               \
EXPAND_FIXED13,               \
EXPAND_FIXED12,               \
EXPAND_FIXED11,               \
EXPAND_FIXED10,               \
EXPAND_FIXED9,                \
EXPAND_FIXED8,                \
EXPAND_FIXED7,                \
EXPAND_FIXED6,                \
EXPAND_FIXED5,                \
EXPAND_FIXED4,                \
EXPAND_FIXED3,                \
EXPAND_FIXED2,                \
EXPAND_FIXED1,                \
EXPAND_FIXED0)                \
(func, fixed, __VA_ARGS__)

#define EXPAND_ARGS2(func, ...)                     \
__SELECT2(__VA_ARGS__ __VA_OPT__(,)                 \
    EXPAND2_16, EXPAND2_16,                         \
    EXPAND2_15, EXPAND2_15,                         \
    EXPAND2_14, EXPAND2_14,                         \
    EXPAND2_13, EXPAND2_13,                         \
    EXPAND2_12, EXPAND2_12,                         \
    EXPAND2_11, EXPAND2_11,                         \
    EXPAND2_10, EXPAND2_10,                         \
    EXPAND2_9, EXPAND2_9,                           \
    EXPAND2_8, EXPAND2_8,                           \
    EXPAND2_7, EXPAND2_7,                           \
    EXPAND2_6, EXPAND2_6,                           \
    EXPAND2_5, EXPAND2_5,                           \
    EXPAND2_4, EXPAND2_4,                           \
    EXPAND2_3, EXPAND2_3,                           \
    EXPAND2_2, EXPAND2_2,                           \
    EXPAND2_1, EXPAND2_1,                           \
    EXPAND2_0, EXPAND2_0)                           \
(func, __VA_ARGS__)

#define EXPAND_ARGS2_FIXED(func, fixed, ...)        \
__SELECT2(__VA_ARGS__ __VA_OPT__(,)                 \
    EXPAND2_FIXED16, EXPAND2_FIXED16,               \
    EXPAND2_FIXED15, EXPAND2_FIXED15,               \
    EXPAND2_FIXED14, EXPAND2_FIXED14,               \
    EXPAND2_FIXED13, EXPAND2_FIXED13,               \
    EXPAND2_FIXED12, EXPAND2_FIXED12,               \
    EXPAND2_FIXED11, EXPAND2_FIXED11,               \
    EXPAND2_FIXED10, EXPAND2_FIXED10,               \
    EXPAND2_FIXED9, EXPAND2_FIXED9,                 \
    EXPAND2_FIXED8, EXPAND2_FIXED8,                 \
    EXPAND2_FIXED7, EXPAND2_FIXED7,                 \
    EXPAND2_FIXED6, EXPAND2_FIXED6,                 \
    EXPAND2_FIXED5, EXPAND2_FIXED5,                 \
    EXPAND2_FIXED4, EXPAND2_FIXED4,                 \
    EXPAND2_FIXED3, EXPAND2_FIXED3,                 \
    EXPAND2_FIXED2, EXPAND2_FIXED2,                 \
    EXPAND2_FIXED1, EXPAND2_FIXED1,                 \
    EXPAND2_FIXED0, EXPAND2_FIXED0)                 \
(func, fixed, __VA_ARGS__)

#define ENSURE_ARG_EXIST(arg) static_assert(sizeof(#arg) != 1, "You must provide an argument!")

#ifdef __GNUC__
#define ENABLE_LAMBDA

#endif

#ifdef ENABLE_LAMBDA
#define λ(expr) ({expr})

#endif

#define array_size(arr) sizeof(arr) / sizeof(arr[0])

#define __ARRAY_CHECK(arr)                          \
    _Generic(&(arr),                                \
    typeof(*(arr)) (*)[array_size(arr)]: true,      \
    default:  false                                 \
)

#define is_array(arr_like) (__ARRAY_CHECK(arr_like))


#define ENUM_FLAG(name, index) name = 1 << index


#endif //MAGIC_H
