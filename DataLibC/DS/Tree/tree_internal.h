//
// Created by 18030 on 2024/11/27.
//

#ifndef TEST_TREE_INTERNAL_H
#define TEST_TREE_INTERNAL_H

#include "DataLibC/DS/TypeEnum.h"


typedef struct _tree_node_base _Tree_node_base;
typedef _Tree_node_base* _Base_ptr;


typedef struct _rb_tree _Rb_tree;

typedef enum _rb_tree_color _Rb_tree_color;

typedef struct _rb_tree_node _Rb_tree_node;

typedef struct _rb_tree_iterator
{
    PRIVATE _Rb_tree_node* _M_Current;
} _Rb_tree_iterator;

_Base_ptr _create_normal_node(_Base_ptr _parent, DataType _type, void* _value);

_Base_ptr _create_leaf_node(_Base_ptr _parent, DataType _type, void* _value);

_Rb_tree_node* _create_rb_tree_node(_Rb_tree_color _color, _Base_ptr _parent, DataType _type, void* _value);

void _rb_tree_init(_Rb_tree** _tree, DataType _type, Predicate* _comp);

void _rb_tree_init_entry_node(_Rb_tree** _tree, Predicate* _comp);

bool _rb_tree_insert(_Rb_tree* _tree, void* value);

bool _rb_tree_containes(_Rb_tree* _tree, void* value);

void _rb_tree_destroy(_Rb_tree* _tree);

void _rb_tree_clear(_Rb_tree* _tree);

void* _rb_tree_value_at(_Rb_tree* _tree, void* pair);

bool _rb_tree_erase(_Rb_tree* _tree, void* value);

_Rb_tree_iterator _tree_iterator_internal(_Rb_tree* tree);

#endif //TEST_TREE_INTERNAL_H
