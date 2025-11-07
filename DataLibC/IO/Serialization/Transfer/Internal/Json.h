//
// Created by 18030 on 2025/2/20.
//

#ifndef DATALIBC_JSON_H
#define DATALIBC_JSON_H

#include <stddef.h>
#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/DS/Stack/Stack.h"
#include "DataLibC/DS/List/DynamicArray.h"
#include "DataLibC/DS/Queue/Queue.h"

constexpr size_t JSON_BUFFER_SIZE = 1024;

typedef bool( * JsonReadHandler)(void* data, unsigned char* buffer, size_t size, size_t* size_read);

typedef enum JsonParserState
{
    JsonPraserState_StartParseStream,
    JsonParserState_EndParseStream,
    JsonParserState_StartParseObject,
    JsonParserState_StartParseArray,
    JsonParserState_EndParseObject,
    JsonParserState_EndParseArray,
    JsonParserState_StartParseScalar
} JsonParserState;

typedef enum JsonTokenType
{
    JsonToken_BeginObject = 1,
    JsonToken_EndObject = 1 << 1,
    JsonToken_BeginArray = 1 << 2,
    JsonToken_EndArray = 1 << 3,
    JsonToken_Colon = 1 << 4,
    JsonToken_Comma = 1 << 5,
    JsonToken_String = 1 << 6,
    JsonToken_Number = 1 << 7,
} JsonTokenType;

typedef enum JsonErrorType
{
    JSON_NO_ERROR,
    /** Cannot allocate or reallocate a block of memory. */
    JSON_MEMORY_ERROR,

    /** Cannot read or decode the input stream. */
    JSON_READER_ERROR,
    /** Cannot scan the input stream. */
    JSON_SCANNER_ERROR,
    /** Cannot parse the input stream. */
    JSON_PARSER_ERROR
} JsonErrorType;

/// Node types.
typedef enum JsonNodeType
{
    /// An empty node.
    JSON_NO_NODE,
    /// A scalar node.
    JSON_SCALAR_NODE,
    /// A sequence node.
    JSON_ARRAY_NODE,
    /// A mapping node.
    JSON_OBJECT_NODE
} JsonNodeType;

typedef enum JsonValueType
{
    JSON_
} JsonValueType;

///The pointer position.
typedef struct JsonMark
{
    /// The position index.
    size_t index;
    /// The position line.
    size_t line;
    /// The position column.
    size_t column;
} JsonMark;

typedef union JsonValue
{
    struct
    {
        char* data;
        size_t length;
    } scalar;

    DynamicArray array;

    DynamicArray mapping;

} JsonValue;

typedef struct LIB_STRUCT JsonNode
{
    /// The node type
    JsonNodeType type;
    /// The key name of this node.
    const char* key;
    /// The node value.
    JsonValue value;
} JsonNode;

typedef struct LIB_STRUCT JsonToken
{
    /** The token type. */
    JsonTokenType type;

    /** The beginning of the token. */
    JsonMark start_mark;
    /** The end of the token. */
    JsonMark end_mark;
    /// The raw data string.
    const char* string;
} JsonToken;

typedef struct LIB_STRUCT JsonDocument
{
    /// The document nodes stack with element type - JsonNode.
    Stack nodes;


} JsonDocument;

typedef struct LIB_STRUCT JsonParser
{
    /// The currently parsed document.
    JsonDocument* document;
    /// Error type.
    JsonErrorType error;
    /// Error message.
    const char* problem;
    /// Error offset.
    size_t problemOffset;
    /// Error position.
    JsonMark problemMark;
    /// Error context.
    const char* context;
    /// Read handler.
    JsonReadHandler readHandler;
    /// A pointer for passing to the read handler.
    void* readHandlerData;
    /// The current state of the parser.
    JsonParserState state;
    /// The parser states stack.
    Stack states;
    /// Current expected token.
    JsonTokenType expectedTokenType;
    /// The tokens queue.
    Queue tokens;

    /** EOF flag */
    bool eof;

    struct
    {
        /** The beginning of the buffer. */
        unsigned char* start;
        /** The end of the buffer. */
        unsigned char* end;
        /** The current position of the buffer. */
        unsigned char* pointer;
    } buffer;

    /// The current position.
    JsonMark mark;

} JsonParser;

bool JsonParserInitialize(JsonParser* parser);

void JsonParserSetInput(JsonParser* parser, JsonReadHandler handler, void* data);

bool JsonParserLoad(JsonParser* parser, JsonDocument* document);

/// Destroy a parser object.
void JsonParserDelete(JsonParser* parser);


JsonNode* JsonDocumentGetRootNode(JsonDocument* document);

#endif //DATALIBC_JSON_H
