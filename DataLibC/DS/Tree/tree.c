//
// Created by 18030 on 2024/11/27.
//

#include <malloc.h>
#include <memory.h>
#include <stdio.h>
#include "tree.h"

#include "tree_internal.h"
#include "DataLibC/DS/Allocation.h"
#include "DataLibC/DS/Iterator.h"

#define TREE_NODE_AS_BASE(x) (&(x)->_M_base)

#define TREE_NODE_TO(x, type) x->_M_type == (type) ? true : false

typedef enum _tree_node_type
{
    _S_abstract = -1,
    _S_normal = 0,
    _S_leaf = 1,
    _S_rb = 2
} _Tree_node_type;

typedef enum _rb_tree_color
{
    _S_red = false,
    _S_black = true
} _Rb_tree_color;

typedef struct LIB_STRUCT _node_handle
{
    DataType _M_type;
    void* _M_data;
} _Node_handle;

typedef struct LIB_STRUCT _tree_node_base
{
    PRIVATE _Base_ptr _M_parent;
    PRIVATE _Tree_node_type _M_type;
    PRIVATE _Node_handle _M_storage;
} _Tree_node_base;


typedef struct LIB_STRUCT _tree_node_normal EXTEND(_Tree_node_base)
{
    INHER PRIVATE _Tree_node_base _M_base;
    PRIVATE _Base_ptr _M_left;
    PRIVATE _Base_ptr _M_right;
} _Tree_node_normal;


typedef struct LIB_STRUCT _tree_node_leaf EXTEND(_Tree_node_base)
{
    INHER PRIVATE _Tree_node_base _M_base;
} _Tree_node_leaf;

typedef struct _rb_tree_node _Rb_tree_node;

typedef _Rb_tree_node* _Linked_type;

typedef struct LIB_STRUCT _rb_tree_node EXTEND(_Tree_node_base)
{
    INHER PRIVATE _Tree_node_base _M_base;
    PRIVATE _Rb_tree_color _M_color;
    PRIVATE _Linked_type _M_left;
    PRIVATE _Linked_type _M_right;
} _Rb_tree_node;

typedef struct LIB_STRUCT _rb_tree
{
    PRIVATE _Rb_tree_node* _M_root;
    PRIVATE size_t _M_count;
    PRIVATE DataType _M_type;
    PRIVATE Predicate* _M_comp;
} _Rb_tree;

static _Tree_node_base
_create_base_node(_Base_ptr _parent, _Tree_node_type _node_type, DataType _val_type, void* _value)
{
    _Tree_node_base _node = { _parent, _node_type };
    _node._M_storage._M_type = _val_type;
    if (!_value)
    {
        _node._M_storage._M_data = NULL;
        return _node;
    }
    _node._M_storage._M_data = GetBuiltinCtor(_val_type)(_value);
    return _node;
}

_Base_ptr _create_normal_node(_Base_ptr _parent, DataType _type, void* _value)
{
    _Tree_node_normal* _node = malloc(sizeof(_Tree_node_normal));
    _node->_M_base = _create_base_node(_parent, _S_normal, _type, _value);
    _node->_M_left = NULL;
    _node->_M_right = NULL;
    return TREE_NODE_AS_BASE(_node);
}

_Base_ptr _create_leaf_node(_Base_ptr _parent, DataType _type, void* _value)
{
    _Tree_node_leaf* _node = malloc(sizeof(_Tree_node_leaf));
    _node->_M_base = _create_base_node(_parent, _S_leaf, _type, _value);
    return TREE_NODE_AS_BASE(_node);
}

_Rb_tree_node* _create_rb_tree_node(_Rb_tree_color _color, _Base_ptr _parent, DataType _type, void* _value)
{
    _Rb_tree_node* _node = malloc(sizeof(_Rb_tree_node));
    _node->_M_base = _create_base_node(_parent, _S_rb, _type, _value);
    _node->_M_left = NULL;
    _node->_M_right = NULL;
    _node->_M_color = _color;
    return _node;
}

void _rb_tree_init(_Rb_tree** _tree, DataType _type, Predicate* _comp)
{
    *_tree = malloc(sizeof(_Rb_tree));
    (*_tree)->_M_root = NULL;
    (*_tree)->_M_count = 0;
    (*_tree)->_M_type = _type;
    (*_tree)->_M_comp = _comp;
}

static void* _tree_node_base_extract_data(_Base_ptr _node)
{
    return _node->_M_storage._M_data;
}

static _Rb_tree_node* _rb_tree_node_sibling(_Rb_tree_node* _node)
{
    if (TREE_NODE_AS_BASE(_node)->_M_parent)
    {
        return ((_Rb_tree_node*)TREE_NODE_AS_BASE(_node)->_M_parent)->_M_left == _node
                   ? ((_Rb_tree_node*)TREE_NODE_AS_BASE(_node)->_M_parent)->_M_right
                   : ((_Rb_tree_node*)TREE_NODE_AS_BASE(_node)->_M_parent)->_M_left;
    }
    return NULL;
}

static void _rb_tree_node_right_rotate(_Rb_tree* _tree, _Rb_tree_node* _node)
{
    _Rb_tree_node* _parent = (_Rb_tree_node*)(TREE_NODE_AS_BASE(_node)->_M_parent);
    _Rb_tree_node* _child = _node->_M_left;

    _node->_M_left = _child->_M_right;
    if (_node->_M_left)
        TREE_NODE_AS_BASE(_node->_M_left)->_M_parent = TREE_NODE_AS_BASE(_node);

    TREE_NODE_AS_BASE(_child)->_M_parent = TREE_NODE_AS_BASE(_parent);

    if (!_parent)
        _tree->_M_root = _child;
    else if (_node == _parent->_M_left)
        _parent->_M_left = _child;
    else
        _parent->_M_right = _child;


    _child->_M_right = _node;
    TREE_NODE_AS_BASE(_node)->_M_parent = TREE_NODE_AS_BASE(_child);
}

static void _rb_tree_node_left_rotate(_Rb_tree* _tree, _Rb_tree_node* _node)
{
    _Rb_tree_node* _parent = (_Rb_tree_node*)(TREE_NODE_AS_BASE(_node)->_M_parent);
    _Rb_tree_node* _child = _node->_M_right;


    _node->_M_right = _child->_M_left;
    if (_node->_M_right)
        TREE_NODE_AS_BASE(_node->_M_right)->_M_parent = TREE_NODE_AS_BASE(_node);

    TREE_NODE_AS_BASE(_child)->_M_parent = TREE_NODE_AS_BASE(_parent);

    if (!_parent)
        _tree->_M_root = _child;
    else if (_node == _parent->_M_left)
        _parent->_M_left = _child;
    else
        _parent->_M_right = _child;

    _child->_M_left = _node;
    TREE_NODE_AS_BASE(_node)->_M_parent = TREE_NODE_AS_BASE(_child);
}

static void _rb_tree_rebalance(_Rb_tree* _tree, _Rb_tree_node* _node)
{
    _Linked_type _parent = (_Linked_type)(TREE_NODE_AS_BASE(_node))->_M_parent;
    _Linked_type _grand_parent;
    _Linked_type _sibling;
    while (_parent && _parent->_M_color == _S_red)
    {
        _grand_parent = (_Linked_type)(TREE_NODE_AS_BASE(_parent))->_M_parent;
        _sibling = _rb_tree_node_sibling(_parent);

        if (_sibling && _sibling->_M_color == _S_red)
        {
            _grand_parent->_M_color = _S_red;
            _sibling->_M_color = _S_black;
            _parent->_M_color = _S_black;
            _node = _grand_parent;
            _parent = (_Linked_type)(TREE_NODE_AS_BASE(_node)->_M_parent);
        }
        else
        {
            if (_parent == _grand_parent->_M_left)
            {
                if (_node == _parent->_M_left)
                {
                    _rb_tree_node_right_rotate(_tree, _grand_parent);
                    _parent->_M_color = _S_black;
                    _grand_parent->_M_color = _S_red;
                }
                else
                {
                    _rb_tree_node_left_rotate(_tree, _parent);
                    _rb_tree_node_right_rotate(_tree, _grand_parent);
                    _node->_M_color = _S_black;
                    _grand_parent->_M_color = _S_red;
                }
            }
            else
            {
                if (_node == _parent->_M_right)
                {
                    _rb_tree_node_left_rotate(_tree, _grand_parent);
                    _parent->_M_color = _S_black;
                    _grand_parent->_M_color = _S_red;
                }
                else
                {
                    _rb_tree_node_right_rotate(_tree, _parent);
                    _rb_tree_node_left_rotate(_tree, _grand_parent);
                    _node->_M_color = _S_black;
                    _grand_parent->_M_color = _S_red;
                }
            }
            break;
        }
    }
    _tree->_M_root->_M_color = _S_black;
}

bool _rb_tree_insert(_Rb_tree* _tree, void* value)
{
    if (!_tree->_M_root)
    {
        _tree->_M_root = _create_rb_tree_node(_S_black, NULL, _tree->_M_type, value);
        _tree->_M_count = 1;
        return true;
    }
    _Linked_type _iter = _tree->_M_root;
    _Linked_type _parent;
    while (_iter)
    {
        _parent = _iter;
        if (_tree->_M_comp(value, _tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter))))
            _iter = _iter->_M_left;
        else if (_tree->_M_comp(_tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter)), value))
            _iter = _iter->_M_right;
        else
            return false;
    }
    _iter = _create_rb_tree_node(_S_red, TREE_NODE_AS_BASE(_parent), _tree->_M_type, value);
    if (_tree->_M_comp(value, _tree_node_base_extract_data(TREE_NODE_AS_BASE(_parent))))
        _parent->_M_left = _iter;
    else
        _parent->_M_right = _iter;

    _rb_tree_rebalance(_tree, _iter);
    return true;
}

bool _rb_tree_containes(_Rb_tree* _tree, void* value)
{
    _Linked_type _iter = _tree->_M_root;
    while (_iter)
    {
        if (_tree->_M_comp(value, _tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter))))
            _iter = _iter->_M_left;
        else if (_tree->_M_comp(_tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter)), value))
            _iter = _iter->_M_right;
        else
            return true;
    }
    return false;
}

void _rb_tree_destroy(_Rb_tree* _tree)
{
    _rb_tree_clear(_tree);
    free(_tree);
}

void _rb_tree_init_entry_node(_Rb_tree** _tree, Predicate* _comp)
{
    *_tree = malloc(sizeof(_Rb_tree));
    (*_tree)->_M_root = NULL;
    (*_tree)->_M_count = 0;
    (*_tree)->_M_type = kPAIR;
    (*_tree)->_M_comp = _comp;
}

_Linked_type _rb_tree_find(_Rb_tree* _tree, void* value)
{
    _Linked_type _iter = _tree->_M_root;
    while (_iter)
    {
        if (_tree->_M_comp(value, _tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter))))
            _iter = _iter->_M_left;
        else if (_tree->_M_comp(_tree_node_base_extract_data(TREE_NODE_AS_BASE(_iter)), value))
            _iter = _iter->_M_right;
        else
            return _iter;
    }
    return NULL;
}

void* _rb_tree_value_at(_Rb_tree* _tree, void* pair)
{
    _Linked_type _iter = _rb_tree_find(_tree, pair);
    if (_iter)
        return (TREE_NODE_AS_BASE(_iter))->_M_storage._M_data;
    return NULL;
}

static void _rb_tree_free_node(_Linked_type node)
{
    auto base = TREE_NODE_AS_BASE(node);
    auto dtor = GetBuiltinDtor(base->_M_storage._M_type);
    dtor(base->_M_storage._M_data);
    free(node);
}

static void _rb_tree_clear_internal(_Linked_type node)
{
    if (node->_M_left)
    {
        _rb_tree_clear_internal(node->_M_left);
    }
    if (node->_M_right)
    {
        _rb_tree_clear_internal(node->_M_right);
    }
    _rb_tree_free_node(node);
}

void _rb_tree_clear(_Rb_tree* _tree)
{
    if (_tree->_M_root)
    {
        _rb_tree_clear_internal(_tree->_M_root);
    }
    _tree->_M_root = NULL;
}

_Linked_type _rb_tree_node_find_prev(_Linked_type node)
{
    if (node->_M_left)
    {
        auto iter = node->_M_left;
        while (iter->_M_right)
        {
            iter = iter->_M_right;
        }
        return iter;
    }
    auto p = (TREE_NODE_AS_BASE(node))->_M_parent;
    auto curr = node;
    while (p && ((_Linked_type)p)->_M_right != curr)
    {
        curr = (_Linked_type)p;
        p = p->_M_parent;
    }
    return (_Linked_type)p;
}

static void _rb_tree_delete_balance(_Rb_tree* _tree, _Linked_type node)
{
    while (node != (_Linked_type)_tree->_M_root && node->_M_color == _S_black)
    {
        if (node == ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_left)
        {
            _Linked_type sibling = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_right;

            // Case 1: Sibling is red
            if (sibling->_M_color == _S_red)
            {
                sibling->_M_color = _S_black;
                ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_color = _S_red;
                _rb_tree_node_left_rotate(_tree, (_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent));
                sibling = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_right;
            }

            // Ensure sibling's children are not NULL before accessing their colors
            auto left_child_color = sibling->_M_left ? sibling->_M_left->_M_color : _S_black;
            auto right_child_color = sibling->_M_right ? sibling->_M_right->_M_color : _S_black;

            // Case 2: Both children of sibling are black
            if (left_child_color == _S_black && right_child_color == _S_black)
            {
                sibling->_M_color = _S_red;
                node = (_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent);
            }
            else
            {
                // Case 3: Left child of sibling is red, right child is null or black
                if (right_child_color == _S_black)
                {
                    if (sibling->_M_left) sibling->_M_left->_M_color = _S_black;
                    sibling->_M_color = _S_red;
                    _rb_tree_node_right_rotate(_tree, sibling);
                    sibling = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_right;
                }

                // Case 4: Right child of sibling is red
                sibling->_M_color = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_color;
                ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_color = _S_black;
                if (sibling->_M_right) sibling->_M_right->_M_color = _S_black;
                _rb_tree_node_left_rotate(_tree, (_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent));
                node = _tree->_M_root; // This stops the loop since we've reached the root
            }
        }
        else // Mirror operations for when 'node' is the right child of its parent
        {
            _Linked_type sibling = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_left;

            // Case 1: Sibling is red
            if (sibling->_M_color == _S_red)
            {
                sibling->_M_color = _S_black;
                ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_color = _S_red;
                _rb_tree_node_right_rotate(_tree, (_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent));
                sibling = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_left;
            }

            // Ensure sibling's children are not NULL before accessing their colors
            auto left_child_color = sibling->_M_left ? sibling->_M_left->_M_color : _S_black;
            auto right_child_color = sibling->_M_right ? sibling->_M_right->_M_color : _S_black;

            // Case 2: Both children of sibling are black
            if (left_child_color == _S_black && right_child_color == _S_black)
            {
                sibling->_M_color = _S_red;
                node = (_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent);
            }
            else
            {
                // Case 3: Right child of sibling is red, left child is null or black
                if (left_child_color == _S_black)
                {
                    if (sibling->_M_right) sibling->_M_right->_M_color = _S_black;
                    sibling->_M_color = _S_red;
                    _rb_tree_node_left_rotate(_tree, sibling);
                    sibling = ((_Linked_type)TREE_NODE_AS_BASE(node)->_M_parent)->_M_left;
                }

                // Case 4: Left child of sibling is red
                sibling->_M_color = ((_Linked_type)(TREE_NODE_AS_BASE(node)->_M_parent))->_M_color;
                ((_Linked_type)TREE_NODE_AS_BASE(node)->_M_parent)->_M_color = _S_black;
                if (sibling->_M_left) sibling->_M_left->_M_color = _S_black;
                _rb_tree_node_right_rotate(_tree, (_Linked_type)TREE_NODE_AS_BASE(node)->_M_parent);
                node = _tree->_M_root; // This stops the loop since we've reached the root
            }
        }
    }
    node->_M_color = _S_black;
}

bool _rb_tree_erase(_Rb_tree* _tree, void* value)
{
    auto iter = _rb_tree_find(_tree, value);
    if (iter == NULL)
        return false;
    if (iter->_M_left != NULL && iter->_M_right != NULL)
    {
        auto next = _rb_tree_node_find_prev(iter);
        auto base = TREE_NODE_AS_BASE(iter);
        // swap data
        auto data = base->_M_storage._M_data;
        base->_M_storage._M_data = (TREE_NODE_AS_BASE(next))->_M_storage._M_data;
        (TREE_NODE_AS_BASE(next))->_M_storage._M_data = data;
        iter = next;
    }
    // Now, the node we want to remove has at most one child

    // Determine whether the node to be removed is red or black
    bool is_black = iter->_M_color == _S_black;

    // Remove the node
    _Linked_type replacement = (iter->_M_left != NULL ? iter->_M_left : iter->_M_right);
    if (replacement != NULL) // node has one child
    {
        // Link replacement with parent
        auto parent = (_Linked_type)(TREE_NODE_AS_BASE(iter))->_M_parent;
        (TREE_NODE_AS_BASE(replacement))->_M_parent = TREE_NODE_AS_BASE(parent);
        if (parent == NULL)
            _tree->_M_root = replacement;
        else if (iter == parent->_M_left)
            parent->_M_left = replacement;
        else
            parent->_M_right = replacement;
        // Free the node being removed
        _rb_tree_free_node(iter);
    }
    else if (iter == _tree->_M_root)
    {
        // Node to be removed is the only node in the tree
        _tree->_M_root = NULL;
        _rb_tree_free_node(iter);
        return true;
    }
    else // no child
    {
        // Node to be removed has no children and is black
        if (is_black)
            _rb_tree_delete_balance(_tree, iter);

        auto parent = (_Linked_type)(TREE_NODE_AS_BASE(iter))->_M_parent;
        // Remove the node from the tree
        if (parent != NULL)
        {
            if (iter == parent->_M_left)
                parent->_M_left = NULL;
            else if (iter == parent->_M_right)
                parent->_M_right = NULL;
        }
        _rb_tree_free_node(iter);
    }

    return true;
}

_Rb_tree_iterator _tree_iterator_internal(_Rb_tree* tree)
{
    auto node = tree->_M_root;
    if (node != NULL)
    {
        while (node->_M_left)
            node = node->_M_left;
    }
    return (_Rb_tree_iterator){ node };
}


bool _Rb_tree_iterator_HasNext(_Rb_tree_iterator iter)
{
    return iter._M_Current != NULL;
}

void* _Rb_tree_iterator_Next(_Rb_tree_iterator* iter)
{
    auto current = iter->_M_Current;
    auto iter_Node = current;
    if (iter_Node->_M_right)
    {
        iter_Node = iter_Node->_M_right;
        while (iter_Node->_M_left)
            iter_Node = iter_Node->_M_left;
    }
    else
    {
        auto p = TREE_NODE_AS_BASE(iter_Node)->_M_parent;
        while (p && iter_Node == ((_Linked_type)p)->_M_right)
        {
            iter_Node = (_Linked_type)p;
            p = p->_M_parent;
        }
        iter_Node = (_Linked_type)p;
    }
    iter->_M_Current = iter_Node;
    return (TREE_NODE_AS_BASE(current))->_M_storage._M_data;
}
