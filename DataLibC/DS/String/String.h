//
// Created by 18030 on 2024/11/16.
//

#ifndef TEST_STRING_H
#define TEST_STRING_H

#include <stddef.h>
#include <stdint.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Tool/TypeMacro.h"

struct LIB_STRUCT String
{
    PRIVATE size_t m_Capacity;
    size_t length;

    PRIVATE char* m_Buffer;
    PRIVATE const size_t m_AllocationCount;
    PRIVATE bool m_Dirty;
};

typedef struct String String;
typedef struct DynamicArray DynamicArray;
typedef size_t Npos;

typedef struct _iobuf FILE;

/// The value represents non-existent in a String
constexpr Npos npos = SIZE_MAX;

/// Initialize a String with given size and default content
/// \param str The String to be initialized. (NULL is not allowed)
/// \param size The default size of the String
/// \param elemPtr The default string content
/// \return True if succeed, otherwise false
API_MODULE bool String_Init(String* str, size_t size, const char* elemPtr);

/// Get a C-Style string from the String
/// \param str The target String. (NULL is not allowed)
/// \return A C-Style string (const char*)
API_MODULE const char* String_ToString(const String* str);

/// Append a string to the end of the input String
/// \param str  The destination String
/// \param elemPtr  The source data
/// \return The destination String self
API_MODULE String* String_Append(String* str, const char* elemPtr);

/// Append a single character to the end of the String
/// \param str The destination String
/// \param ch The character to be appended. 0 is supported
/// but But it will stop at 0 characters during output
/// \return The destination String self
API_MODULE String* String_AppendChar(String* str, char ch);

/// Get a char at index from the String
/// \param str The target string. (NULL is not allowed)
/// \param index    The index of character
/// \return The returned character
API_MODULE char String_CharAt(String* str, size_t index);

/// Append a String to the end of the dst String
/// \param dst  The destination String
/// \param src  The source String
/// \return The destination String self
API_MODULE String* String_AppendString(String* dst, const String* src);

/// Finds the first occurrence of ch in the String
/// \param str The target string. (NULL is not allowed)
/// \param ch The target character
/// \return The index of the character. If not found return npos
API_MODULE Npos String_FindChar(String* str, char ch);

/// Finds the first occurrence of pattern in the String
/// \param str The target string. (NULL is not allowed)
/// \param pattern The target string
/// \return The starting index of the string. If not found return npos
API_MODULE Npos String_Find(String* str, const char* pattern);

/// Finds the first occurrence of pattern in the String
/// \param str The target string. (NULL is not allowed)
/// \param pattern The target string
/// \return The starting index of the string. If not found return npos
API_MODULE Npos String_FindString(String* str, String* pattern);

/// Finds the last occurrence of ch in the String
/// \param str The target string. (NULL is not allowed)
/// \param ch The target character
/// \return The index of the character. If not found return npos
API_MODULE Npos String_FindLastOfChar(String* str, char ch);

/// Count the number of times the character appears in the String
/// \param str The target string. (NULL is not allowed)
/// \param ch The target character
/// \return The count of the character
API_MODULE size_t String_CountOfChar(String* str, char ch);

API_MODULE bool String_Contains(String* str, const char* s);

/// Count the number of times the pattern string appears in the String
/// \param str The target string. (NULL is not allowed)
/// \param pattern The target string
/// \return The count of string
API_MODULE size_t String_Count(String* str, const char* pattern);

/// Count the number of times the pattern String appears in the String
/// \param str The target string. (NULL is not allowed)
/// \param pattern The target string
/// \return The count of string
API_MODULE size_t String_CountString(String* str, String* pattern);

/// Extracts a substring from the given string.
/// \param str  The input string. (NULL is not allowed)
/// \param start The starting position of the substring
/// \param length The length of the substring. Npos means until the end of the string
/// \return A new string that is a substring of this string
API_MODULE String* String_SubString(String* str, size_t start, size_t length);

/// Compare the String with the given string
/// \param str The source string.
/// \param elemPtr The target string.
/// \return True if the two strings are equal, otherwise false
API_MODULE bool String_Equals(const String* str, const char* elemPtr);

/// Compare the String with the given string
/// \param str The source string.
/// \param elemPtr The target string.
/// \return True if the two strings are equal, otherwise false
API_MODULE bool String_EqualsString(const String* str, const String* other);

API_MODULE void String_ReplaceFrom(String* str, size_t pos, const char* replace);

API_MODULE void String_ReplaceString(String* str, const char* oldStr, const char* newStr, bool replaceAll);

API_MODULE void String_Erase(String* str, size_t start, size_t length);

API_MODULE int String_SplitByChar(String* str, char delimiter, char* out[], int size);

API_MODULE int String_Split(String* str, const char* delimiter, char* out[], int size);

API_MODULE void String_Resize(String* str, size_t newSize);

API_MODULE void String_TrimToSize(String* str);

API_MODULE bool String_ReadFromFile(String* str, FILE* file);

/// Clear the content of the String. This won't release the memory of the String.
/// \param str The target String. (NULL is not allowed)
API_MODULE void String_Clear(String* str);

/// Release the memory of the String. After this, you can only use this object after calling String_Init again.
/// \param str The target String. (NULL is not allowed)
API_MODULE void String_Release(String* str);

/// Check if the string starts with the subString
/// \param str The target string.
/// \param subString The substring to be checked
/// \return True if the string starts with the subString, otherwise false
API_MODULE bool IsStringStartWith(const char* str, const char* subString);

#endif //TEST_STRING_H
