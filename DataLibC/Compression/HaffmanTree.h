//
// Created by 18030 on 2024/12/20.
//

#ifndef DATALIBC_HAFFMANTREE_H
#define DATALIBC_HAFFMANTREE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "Compression.h"

typedef struct BitReader BitReader;
typedef struct HaffmanNode HaffmanNode;

struct LIB_STRUCT HaffmanTree
{
    HaffmanNode* root;
};

typedef struct HaffmanTree HaffmanTree;

API_MODULE HaffmanTree GenerateHaffmanTreeFromCodeLengthSequence(const Byte* bytes, size_t length);

API_MODULE void DecodeFromHaffmanTree(HaffmanTree* tree, BitReader* reader, UInt16* out, size_t codeAmount);

API_MODULE void DestroyHaffmanTree(HaffmanTree* tree);

#endif //DATALIBC_HAFFMANTREE_H
