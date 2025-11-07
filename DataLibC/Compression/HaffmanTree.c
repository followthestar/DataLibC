//
// Created by 18030 on 2024/12/20.
//

#include <malloc.h>
#include <string.h>
#include "HaffmanTree.h"
#include "BitReader.h"
#include "DataLibC/Tool/LogMacro.h"

typedef struct HaffmanNode
{
    UInt16 data;
    HaffmanNode* left;
    HaffmanNode* right;
} HaffmanNode;

typedef struct SymbolCodePair
{
    UInt16 symbol;
    Byte bitLength;
    UInt16 haffCode;
} SymbolCodePair;

static HaffmanTree Generate_Internal(SymbolCodePair* pairs, size_t count)
{
    HaffmanTree tree;
    tree.root = malloc(sizeof(HaffmanNode));
    if (!tree.root)
        return tree;
    tree.root->left = tree.root->right = NULL;
    for (int i = 0; i < count; ++i)
    {
        auto node = tree.root;
        for (int j = pairs[i].bitLength - 1; j >= 0; --j)
        {
            Byte bit = (pairs[i].haffCode & (1 << j)) >> j;
            if (bit)
            {
                if (!node->right)
                {
                    node->right = malloc(sizeof(HaffmanNode));
                    node->right->right = node->right->left = NULL;
                }
                node = node->right;
            }
            else
            {
                if (!node->left)
                {
                    node->left = malloc(sizeof(HaffmanNode));
                    node->left->right = node->left->left = NULL;
                }
                node = node->left;
            }
        }
        if (node->left || node->right)
        {
            RuntimeErrorLog(InvalidFormat, "The symbols cannot build a Haffman Tree!");
            return tree;
        }
        node->data = pairs[i].symbol;
    }
    return tree;
}

HaffmanTree GenerateHaffmanTreeFromCodeLengthSequence(const Byte* bytes, size_t length)
{

    Byte maxLength = 0;
    SymbolCodePair pairs[length];
    size_t noZeroLength = 0;
    for (int i = 0; i < length; ++i)
    {
        if (bytes[i] == 0)
            continue;
        if (bytes[i] > maxLength)
            maxLength = bytes[i];
        pairs[noZeroLength++] = (SymbolCodePair) {i, bytes[i]};
    }

    Byte codeLengthFrequency[maxLength + 1];
    memset(codeLengthFrequency, 0, maxLength + 1);
    for (int i = 0; i < noZeroLength; ++i)
    {
        codeLengthFrequency[pairs[i].bitLength]++;
    }
    // Just ignore 0 code length and ensure it is 0;
    codeLengthFrequency[0] = 0;

    UInt16 code = 0, nextCode[maxLength + 1];
    memset(nextCode, 0, maxLength + 1);
    for (int i = 1; i <= maxLength; ++i)
    {
        code = (code + codeLengthFrequency[i - 1]) << 1;
        nextCode[i] = code;
    }

    for (int i = 0; i < noZeroLength; ++i)
        pairs[i].haffCode = nextCode[pairs[i].bitLength]++;

    return Generate_Internal(pairs, noZeroLength);
}

void DecodeFromHaffmanTree(HaffmanTree* tree, BitReader* reader, UInt16* out, size_t codeAmount)
{
    size_t hasDecodedNum = 0;
    HaffmanNode* node;
    if (!tree->root)
        return;
    while (hasDecodedNum != codeAmount)
    {
        node = tree->root;
        while (node->left)
        {
            Byte bit = ReadBits(reader, 1);
            if (bit)
                node = node->right;
            else
                node = node->left;
        }
        out[hasDecodedNum++] = node->data;
    }
}

void Destroy_Internal(HaffmanNode* node)
{
    if (!node)
        return;
    Destroy_Internal(node->left);
    Destroy_Internal(node->right);
    free(node);
}

void DestroyHaffmanTree(HaffmanTree* tree)
{
    if (!tree)
        return;
    Destroy_Internal(tree->root);
}
