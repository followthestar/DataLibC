//
// Created by 18030 on 2025/2/16.
//

#ifndef DATALIBC_READER_INTERNALMACRO_H
#define DATALIBC_READER_INTERNALMACRO_H

#define READER_INTERNAL_DECLARE \
INTERNAL struct _ReaderImpl\
{\
const ReaderVTable* vtable;\
Reader base;\
/** Skip buffer, null until allocated */\
char* skipBuffer;\
\
};                              \
extern const ReaderVTable g_DefaultReaderVTable; \
typedef struct _ReaderImpl ReaderImpl;\
typedef ReaderImpl Super;       \
extern void Reader_Init(ReaderImpl* self);

#define SUPERINIT \
Reader_Init((ReaderImpl*)reader);

#define SUPERCLOSE \
g_DefaultReaderVTable.Close(AsReader(self));

#endif //DATALIBC_READER_INTERNALMACRO_H
