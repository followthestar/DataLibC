//
// Created by 18030 on 2025/2/20.
//

#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <malloc.h>
#include <ctype.h>
#include <stdio.h>
#include "Json.h"
#include "DataLibC/DS/Algorithm.h"

static void JsonParserSetParserError(JsonParser* parser, const char* problem, JsonMark problemMark)
{
    parser->error = JSON_PARSER_ERROR;
    parser->problem = problem;
    parser->problemMark = problemMark;
}

static void PushParserState(JsonParser* parser, JsonParserState state)
{
    Stack_Push(&parser->states, &state);
}

static void PrepareForParseObject(JsonParser* parser, JsonNode* node)
{
    PushParserState(parser, JsonParserState_StartParseObject);
    node->type = JSON_OBJECT_NODE;
    DynamicArray_Init(&node->value.mapping, 0, kPOINTER, 0);
    Stack_Push(&parser->document->nodes, &node);
}

static void PrepareForParseArray(JsonParser* parser, JsonNode* node)
{
    PushParserState(parser, JsonParserState_StartParseArray);
    node->type = JSON_ARRAY_NODE;
    DynamicArray_Init(&node->value.array, 0, kPOINTER, 0);
    Stack_Push(&parser->document->nodes, &node);
}

static void PrepareForParseScalar(JsonParser* parser, JsonNode* node)
{
    PushParserState(parser, JsonParserState_StartParseScalar);
    node->type = JSON_SCALAR_NODE;
    node->value.scalar.length = 0;
    Stack_Push(&parser->document->nodes, &node);
}

static void* JsonTokenCtor(const void* data)
{
    void* m = malloc(sizeof(JsonToken));
    memcpy(m, data, sizeof(JsonToken));
    return m;
}

static void JsonTokenDtor(void* data)
{
    JsonToken* token = data;
    if (token->type == JsonToken_String || token->type == JsonToken_Number)
        free((void*) token->string);
    free(data);
}

bool JsonParserInitialize(JsonParser* parser)
{
    assert(parser);     /* Non-NULL parser object expected. */
    memset(parser, 0, sizeof(JsonParser));

    parser->buffer.start = malloc(JSON_BUFFER_SIZE);
    if (!parser->buffer.start)
        return false;
    parser->buffer.end = parser->buffer.pointer + JSON_BUFFER_SIZE;
    parser->buffer.pointer = parser->buffer.end;
    memset(parser->buffer.start, 0, JSON_BUFFER_SIZE);
    bool ret1 = Stack_Init(&parser->states, kINT, NULL, NULL);
    bool ret2 = Queue_Init(&parser->tokens, kCUSTOM, JsonTokenCtor, JsonTokenDtor);
    return ret1 && ret2;
}

void JsonParserSetInput(JsonParser* parser, JsonReadHandler handler, void* data)
{
    assert(parser);                 /* Non-NULL parser object expected. */
    assert(!parser->readHandler);   /* You can set the source only once. */
    assert(handler);                /* Non-NULL read handler expected. */

    parser->readHandler = handler;
    parser->readHandlerData = data;
}

static bool JsonParserUpdateBuffer(JsonParser* parser)
{
    assert(parser->readHandler);
    assert(parser->readHandlerData);
    if (parser->buffer.pointer == parser->buffer.end)
    {
        static size_t read_size;
        if (!parser->readHandler(parser->readHandlerData, parser->buffer.start,
                                 JSON_BUFFER_SIZE - 1, &read_size))
        {
            parser->eof = true;
            return false;
        }
        parser->buffer.pointer = parser->buffer.start;
        parser->buffer.start[read_size] = '\0';
        parser->buffer.end = parser->buffer.start + read_size;
    }
    return true;
}

static bool JsonParserSkipWhiteSpace(JsonParser* parser)
{
    while (true)
    {
        if (!JsonParserUpdateBuffer(parser))
            return false;
        switch (*parser->buffer.pointer)
        {
            case ' ':
                parser->mark.column++;
                break;
            case '\t':
                parser->mark.column += 4;
                break;
            case '\n':
                parser->mark.column = 0;
                parser->mark.line++;
                break;
            default:
                return true;
        }
        parser->mark.index++;
        parser->buffer.pointer++;
    }
}

static int JsonParserReadOneChar(JsonParser* parser)
{
    if (!JsonParserSkipWhiteSpace(parser))
        return -1;
    parser->mark.column++;
    parser->mark.index++;
    int ch = *parser->buffer.pointer;
    parser->buffer.pointer++;
    return ch;
}

// Must call JsonParserSkipWhiteSpace before calling this function!
static bool JsonParserScanNextToken(JsonParser* parser)
{
    JsonToken token;
    token.start_mark = parser->mark;
    int ch = JsonParserReadOneChar(parser);
    token.end_mark = parser->mark;      // here only for most of the situations
    JsonParserUpdateBuffer(parser);
    switch (ch)
    {
        case ':':
            token.type = JsonToken_Colon;
            token.string = ":";
            break;
        case '"':
        {
            token.type = JsonToken_String;
            int c = 0, index = 0;
            char buf[1024] = {0};
            while ((c = JsonParserReadOneChar(parser)) != '"')
            {
                if (c == -1)
                {
                    JsonParserSetParserError(parser, "Unexpected end of stream!", parser->mark);
                    return false;
                }
                buf[index++] = (char) c;
            }
            token.end_mark = parser->mark;
            token.string = strdup(buf);
            break;
        }
        case '{':
            token.type = JsonToken_BeginObject;
            token.string = "{";
            break;
        case '[':
            token.type = JsonToken_BeginArray;
            token.string = "[";
            break;
        case ',':
            token.type = JsonToken_Comma;
            token.string = ",";
            break;
        case '}':
            token.type = JsonToken_EndObject;
            token.string = "}";
            break;
        case ']':
            token.type = JsonToken_EndArray;
            token.string = "]";
            break;
        default:
            if (ch == -1)
                JsonParserSetParserError(parser, "Unexpected end of stream!", parser->mark);
            else if (ch == 'n') // This for "null"
            {
                static char* null = "null";
                for (int i = 1; i < 4; ++i)
                {
                    if (JsonParserReadOneChar(parser) != null[i])
                    {
                        JsonParserSetParserError(parser, "Unexpected character!", parser->mark);
                        return false;
                    }
                }
                token.type = JsonToken_String;
                token.string = "null";
                token.end_mark = parser->mark;
                return true;
            }
            else if (ch == '-' || isdigit(ch))  // This may be a number
            {
                int c = 0, index = 1;
                bool hasDot = false;
                char buf[32] = {0};
                buf[0] = (char) ch;
                while (parser->buffer.pointer < parser->buffer.end)
                {
                    c = *parser->buffer.pointer;
                    if (c == '.')
                    {
                        if (!hasDot)
                            hasDot = true;
                        else
                        {
                            JsonParserSetParserError(parser, "Invalid number format!", parser->mark);
                            token.end_mark = parser->mark;
                            return false;
                        }
                    }
                    else if (c == '-')      // '-' should be the first character
                    {
                        JsonParserSetParserError(parser, "Invalid number format!", parser->mark);
                        token.end_mark = parser->mark;
                        return false;
                    }
                    else if (!isdigit(c))
                        break;

                    buf[index++] = (char) c;
                    JsonParserReadOneChar(parser);

                    JsonParserUpdateBuffer(parser);
                }
                token.type = JsonToken_Number;
                token.end_mark = parser->mark;
                token.string = strdup(buf);
                break;
            }
            JsonParserSetParserError(parser, "Unexpected character!", parser->mark);
            return false;
    }
    Queue_Enqueue(&parser->tokens, &token);
    return true;
}

static bool JsonParserDoFetchToken(JsonParser* parser)
{
    assert(parser->readHandler);
    assert(parser->readHandlerData);

    if (!JsonParserSkipWhiteSpace(parser))
        return false;
    if (!JsonParserScanNextToken(parser))
        return false;
    return true;
}

static bool JsonParserFetchMoreTokens(JsonParser* parser)
{
    bool isReallyNeedToken = Queue_Empty(&parser->tokens);
    if (isReallyNeedToken)
    {
        if (!JsonParserDoFetchToken(parser))
            return false;
    }
    return true;
}

static bool JsonParserCheckTokenType(JsonParser* parser, JsonToken* token)
{
    assert(token);
    if (token->type & parser->expectedTokenType)
        return true;
    return false;
}

static JsonToken* PeekNextToken(JsonParser* parser)
{
    if (!Queue_Empty(&parser->tokens) || JsonParserFetchMoreTokens(parser))
        return Queue_Front(&parser->tokens);
    return NULL;
}

static JsonNode* PeekTopNode(JsonParser* parser)
{
    return *(JsonNode**) Stack_Top(&parser->document->nodes);
}

static bool IsRootNode(JsonNode* node)
{
    return strcmp(node->key, "root") == 0;
}

static bool JsonParserAfterParseBlock(JsonParser* parser)
{
    JsonToken* token = PeekNextToken(parser);
    parser->state = *(JsonParserState*) Stack_Top(&parser->states);
    if (token->type == JsonToken_Comma)
    {
        Queue_Dequeue(&parser->tokens);
        return true;    // return to last state
    }
    if (token->type == JsonToken_EndArray && parser->state == JsonParserState_StartParseArray)
    {
        Stack_Pop(&parser->states);     // pop start parse array state
        PushParserState(parser, JsonParserState_EndParseArray);
        return true;
    }
    else if (token->type == JsonToken_EndObject && parser->state == JsonParserState_StartParseObject)
    {
        Stack_Pop(&parser->states);     // pop start parse object state
        PushParserState(parser, JsonParserState_EndParseObject);
        return true;
    }
    JsonParserSetParserError(parser, "Unexpected end token after parsing the scalar!",
                             parser->mark);
    return false;
}

static bool EndParseStream(JsonParser* parser)
{
    if (parser->state == JsonParserState_EndParseStream && parser->states.size == 1)
    {
        Stack_Pop(&parser->states);
        return true;
    }
    JsonParserSetParserError(parser, "The parser has reached the end of stream but some tokens are miss matching!",
                             parser->mark);
    return false;
}

static bool EndParseObject(JsonParser* parser)
{
    parser->expectedTokenType = JsonToken_EndObject;
    JsonToken* token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected object token \"}\" when ending parsing an object!", parser->mark);
        return false;
    }
    Queue_Dequeue(&parser->tokens);     // dequeue "}"
    Stack_Pop(&parser->states);         // pop the end object state
    JsonNode* node = PeekTopNode(parser);
    if (IsRootNode(node))
    {
        Stack_Pop(&parser->states);     // pop start stream state
        PushParserState(parser, JsonParserState_EndParseStream);
        return true;
    }
    Stack_Pop(&parser->document->nodes);    // pop the object node

    if (JsonParserAfterParseBlock(parser))
        return true;
    return false;
}

static bool ParseObject(JsonParser* parser)
{
    JsonNode* node = PeekTopNode(parser);
    if (!node)
        return false;
    assert(node->type == JSON_OBJECT_NODE);
    JsonToken* token = PeekNextToken(parser);
    if (node->value.mapping.length == 0)
    {
        parser->expectedTokenType = JsonToken_BeginObject;
        if (!JsonParserCheckTokenType(parser, token))
        {
            JsonParserSetParserError(parser, "Expected object token \"{\" when starting parsing an object!",
                                     parser->mark);
            return false;
        }
        Queue_Dequeue(&parser->tokens);     // dequeue "{"
    }
    parser->expectedTokenType = JsonToken_String | JsonToken_EndObject;
    token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected object token \"\" or \"}\" when parsing an object!", parser->mark);
        return false;
    }
    if (token->type == JsonToken_EndObject)
    {
        Stack_Pop(&parser->states);
        PushParserState(parser, JsonParserState_EndParseObject);
        return true;
    }
    JsonNode* itemNode = malloc(sizeof(JsonNode));
    DynamicArray_Add(&node->value.mapping, &itemNode);
    itemNode->key = strdup(token->string);
    Queue_Dequeue(&parser->tokens);     // dequeue key string

    // next token should be ":"
    parser->expectedTokenType = JsonToken_Colon;
    token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected object token \":\" when parsing an object's key-value pair!",
                                 parser->mark);
        return false;
    }
    Queue_Dequeue(&parser->tokens);     // dequeue ":"

    parser->expectedTokenType = JsonToken_BeginObject | JsonToken_BeginArray | JsonToken_String | JsonToken_Number;
    token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser,
                                 "Expected value token \"[\" or \"{\" or value when parsing value of the key!",
                                 parser->mark);
        return false;
    }
    if (token->type == JsonToken_BeginObject)
        PrepareForParseObject(parser, itemNode);
    else if (token->type == JsonToken_BeginArray)
        PrepareForParseArray(parser, itemNode);
    else
        PrepareForParseScalar(parser, itemNode);
    return true;
}

static bool EndParseArray(JsonParser* parser)
{
    parser->expectedTokenType = JsonToken_EndArray;
    JsonToken* token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected object token \"]\" when ending parsing an array!", parser->mark);
        return false;
    }
    Queue_Dequeue(&parser->tokens);     // dequeue "]"
    Stack_Pop(&parser->states);         // pop the end array state

    Stack_Pop(&parser->document->nodes);    // pop the array node
    if (JsonParserAfterParseBlock(parser))
        return true;
    return false;
}

static bool ParseArray(JsonParser* parser)
{
    JsonNode* node = PeekTopNode(parser);
    if (!node)
        return false;
    assert(node->type == JSON_ARRAY_NODE);
    JsonToken* token = PeekNextToken(parser);
    if (node->value.array.length == 0)
    {
        parser->expectedTokenType = JsonToken_BeginArray;
        if (!JsonParserCheckTokenType(parser, token))
        {
            JsonParserSetParserError(parser, "Expected array token \"[\" when starting parsing an array!",
                                     parser->mark);
            return false;
        }
        Queue_Dequeue(&parser->tokens);     // dequeue "["
    }
    parser->expectedTokenType = JsonToken_String | JsonToken_Number | JsonToken_EndArray | JsonToken_BeginObject;
    token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected value or object token \"{\" when parsing an array!", parser->mark);
        return false;
    }

    if (token->type == JsonToken_EndArray)
    {
        Stack_Pop(&parser->states);
        PushParserState(parser, JsonParserState_EndParseArray);
        return true;
    }

    JsonNode* itemNode = malloc(sizeof(JsonNode));
    size_t index = node->value.array.length;
    char buffer[8] = {0};
    snprintf(buffer, sizeof(buffer), "%zu", index);
    itemNode->key = strdup(buffer);
    DynamicArray_Add(&node->value.array, &itemNode);

    if (token->type == JsonToken_BeginObject)
        PrepareForParseObject(parser, itemNode);
    else
        PrepareForParseScalar(parser, itemNode);
    return true;
}

static bool ParseScalar(JsonParser* parser)
{
    parser->expectedTokenType = JsonToken_String | JsonToken_Number;
    JsonToken* token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected value when parsing the scalar!",
                                 parser->mark);
        return false;
    }
    JsonNode* scalar = PeekTopNode(parser);
    assert(scalar);
    scalar->value.scalar.data = strdup(token->string);
    scalar->value.scalar.length = strlen(token->string);
    Stack_Pop(&parser->document->nodes);    // Pop scalar node
    Queue_Dequeue(&parser->tokens);

    parser->expectedTokenType = JsonToken_Comma | JsonToken_EndArray | JsonToken_EndObject;
    token = PeekNextToken(parser);
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected end token when parsing the scalar!",
                                 parser->mark);
        return false;
    }
    Stack_Pop(&parser->states);


    if (JsonParserAfterParseBlock(parser))
        return true;
    return false;
}

static bool ParseStreamStart(JsonParser* parser)
{
    parser->expectedTokenType = JsonToken_BeginObject | JsonToken_BeginArray;
    JsonToken* token = PeekNextToken(parser);
    if (!token)
        return false;
    if (!JsonParserCheckTokenType(parser, token))
    {
        JsonParserSetParserError(parser, "Expected object or array at the beginning of the text stream!", parser->mark);
        return false;
    }
    JsonNode* root = malloc(sizeof(JsonNode));
    root->key = "root";
    if (token->type == JsonToken_BeginObject)
    {
        PrepareForParseObject(parser, root);
    }
    else if (token->type == JsonToken_BeginArray)
    {
        PrepareForParseArray(parser, root);
    }
    else
        assert(false);
    return true;
}

static bool ParseInternal(JsonParser* parser)
{
    while (true)
    {
        switch (parser->state = *(JsonParserState*) Stack_Top(&parser->states))
        {
            case JsonPraserState_StartParseStream:
                if (!ParseStreamStart(parser))
                    return false;
                break;
            case JsonParserState_StartParseObject:
                if (!ParseObject(parser))
                    return false;
                break;
            case JsonParserState_StartParseArray:
                if (!ParseArray(parser))
                    return false;
                break;
            case JsonParserState_StartParseScalar:
                if (!ParseScalar(parser))
                    return false;
                break;
            case JsonParserState_EndParseObject:
                if (!EndParseObject(parser))
                    return false;
                break;
            case JsonParserState_EndParseArray:
                if (!EndParseArray(parser))
                    return false;
                break;
            case JsonParserState_EndParseStream:        // Only here can we finish the parsing and return
                if (!EndParseStream(parser))
                    return false;
                return true;
            default:
                assert(-1);
        }
    }
}

bool JsonParserLoad(JsonParser* parser, JsonDocument* document)
{
    assert(parser);     /* Non-NULL parser object is expected. */
    assert(document);   /* Non-NULL document object is expected. */

    memset(document, 0, sizeof(JsonDocument));

    Stack_Init(&document->nodes, kPOINTER, NULL, NULL);
    parser->document = document;
    PushParserState(parser, JsonPraserState_StartParseStream);
    if (!ParseInternal(parser))
        return false;

    return true;
}


void JsonParserDelete(JsonParser* parser)
{
    assert(parser); /* Non-NULL parser object expected. */



}

JsonNode* JsonDocumentGetRootNode(JsonDocument* document)
{
    if (!document || document->nodes.size != 1)
        return NULL;
    return *(JsonNode**) Stack_Top(&document->nodes);
}
