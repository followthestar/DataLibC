//
// Created by 18030 on 2024/11/15.
//

#ifndef TEST_DEFAULTOPERATION_H
#define TEST_DEFAULTOPERATION_H

typedef unsigned long long size_t;

typedef bool Predicate(void*, void*);

typedef int Comparator(void*, void*);


extern Predicate* gDefaultLess[];
extern Comparator* gDefaultComparator[];

Predicate* GetBuiltinEqual(size_t type);

Predicate* GetBuiltinLess(size_t type);

#endif //TEST_DEFAULTOPERATION_H
