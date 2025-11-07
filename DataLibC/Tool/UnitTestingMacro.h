//
// Created by 18030 on 2024/11/13.
//

#ifndef TEST_UNITTESTINGMACRO_H
#define TEST_UNITTESTINGMACRO_H

#include <stdio.h>
#include <time.h>
#include <stdlib.h>


#define UNIT_TEST(name) void UnitTest_##name()

#define ASSERT_EQUAL(actual, expected) \
    do { \
        if ((actual) != (expected)) { \
            fprintf(stderr, "Assertion failed: %s:%d: %s != %s (%d != %d)\n", \
                    __FILE__, __LINE__, #actual, #expected, (actual), (expected)); \
        } \
    } while (0)

#define ASSERT_NOT_EQUAL(actual, expected) \
    do { \
        if ((actual) == (expected)) { \
            fprintf(stderr, "Assertion failed: %s:%d: %s == %s (%d == %d)\n", \
                    __FILE__, __LINE__, #actual, #expected, (actual), (expected)); \
        } \
    } while (0)

#define ASSERT_TRUE(condition) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "Assertion failed: %s:%d: %s is not true\n", \
                    __FILE__, __LINE__, #condition); \
        } \
    } while (0)

#define ASSERT_FALSE(condition) \
    do { \
        if (condition) { \
            fprintf(stderr, "Assertion failed: %s:%d: %s is not false\n", \
                    __FILE__, __LINE__, #condition); \
        } \
    } while (0)

#define TEST_CASE(name) \
    do { \
        printf("\nRunning test case: %s\n", #name); \
        UnitTest_##name(); \
    } while (0)


#define FAIL(message) \
    do { \
        fprintf(stderr, "Test failed: %s:%d: %s\n", __FILE__, __LINE__, message); \
        exit(EXIT_FAILURE); \
    } while (0)


#define TIME_BEGIN(test_name) \
printf("\n"test_name"\t开始计时！\n");   \
clock_t start, end;     \
double cpu_time_used;   \
start = clock();

#define TIME_END(test_name) \
end = clock();\
cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC; \
printf(test_name"\t计时结束！共耗时：%g秒\n", cpu_time_used);


#endif //TEST_UNITTESTINGMACRO_H
