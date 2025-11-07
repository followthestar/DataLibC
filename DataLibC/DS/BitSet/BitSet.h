/**
  ******************************************************************************
  * @file           : BitSet.h
  * @author         : 18030
  * @brief          : None
  * @attention      : None
  * @date           : 2025/10/29
  ******************************************************************************
  */


#ifndef BITSET_H
#define BITSET_H

#include "DataLibC/Tool/ExportModuleMacro.h"


typedef struct BitSet BitSet;


/**
 * @brief Create a new BitSet
 * @param initSize The size of the BitSet
 * @param initValue The initial value of the BitSet. True means each bit is set to 1, otherwise 0
 * @return A new BitSet
 */
API_MODULE BitSet* BitSet_Create(int initSize, bool initValue);

/**
 * @brief Fill the entire BitSet with the given value
 * @param bitSet The BitSet to fill
 * @param value The value to fill
 */
API_MODULE void BitSet_Fill(BitSet* bitSet, bool value);

/**
 * @brief Set the value of a bit in the BitSet
 * @param bitSet The BitSet to set
 * @param index The index of the bit to set
 * @param value The value to set
 */
API_MODULE void BitSet_Set(BitSet* bitSet, int index, bool value);

/**
 * @brief Get the value of a bit in the BitSet
 * @param bitSet The BitSet to get
 * @param index The index of the bit to get
 * @return The value of the bit
 */
API_MODULE bool BitSet_Get(BitSet* bitSet, int index);

/**
 * @brief Get the size of the BitSet
 * @param bitSet The BitSet to get
 * @return The size of the BitSet
 */
API_MODULE int BitSet_Size(const BitSet* bitSet);

/**
 * @brief Destroy a BitSet
 * @param bitSet The BitSet to destroy.
 * @attention After calling this function, the bitset pointer is invalid
 */
API_MODULE void BitSet_Destroy(BitSet* bitSet);

#endif //BITSET_H
