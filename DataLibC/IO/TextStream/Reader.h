//
// Created by 18030 on 2025/2/14.
//

#ifndef DATALIBC_READER_H
#define DATALIBC_READER_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include <stddef.h>

typedef ABSTRACT LIB_CLASS _Reader Reader;
typedef LIB_VTABLE _ReaderVTable ReaderVTable;

typedef int (* const ReadCharFunc)(Reader* self);

typedef int(* const ReadFunc)(Reader*, char*, int, int);

typedef int(* const SkipFunc)(Reader*, int);

typedef bool(* const ReadyFunc)(Reader* self);

typedef void(* const ResetFunc)(Reader*);

typedef void(* const MarkFunc)(Reader*, int);

typedef void(* const CloseFunc)(Reader*);


LIB_VTABLE _ReaderVTable
{
    /// Read a single character
    /// \param self The Reader type object
    /// \return The character read, or -1 if the end of the stream is reached
    int (* const ReadChar)(Reader* self);

    /// Read characters into an array. If len is -1, then characters are read
    /// until the end of the stream is reached.
    /// \param self The Reader type object
    /// \param buf The array to read into
    /// \param offset The offset of the array to read into
    /// \param len Maximum number of characters to read
    /// \return The number of characters read, or -1 if the end of the stream is reached
    int (* const Read)(Reader* self, char buf[], int offset, int len);


    /// Skips characters.
    /// \param self The Reader type object
    /// \param len The number of characters to skip
    /// \return The number of characters actually skipped
    int (* const Skip)(Reader* self, int len);

    /// Tells whether this stream is ready to be read.
    /// \param self The Reader type object
    /// \return true if the stream is ready to be read, false otherwise
    bool (* const Ready)(Reader* self);

    /// Resets the stream.  If the stream has been marked, then attempt to
    /// reposition it at the mark.  If the stream has not been marked, then
    /// attempt to reset it in some way appropriate to the particular stream,
    /// for example by repositioning it to its starting point.  Not all
    /// character-input streams support the reset() operation, and some support
    /// reset() without supporting mark().
    /// \param self The Reader type object
    void (* const Reset)(Reader* self);

    /// Marks the present position in the stream.  Subsequent calls to reset()
    /// will attempt to reposition the stream to this point.  Not all
    /// character-input streams support the mark() operation.
    /// \param readAheadLimit Limit on the number of characters that may be
    /// read while still preserving the mark.  After
    /// reading this many characters, attempting to
    /// reset the stream may fail.
    /// \param self The Reader type object
    void (* const Mark)(Reader* self, int readAheadLimit);

    /// Close the stream and release resources
    /// \param self The Reader type object
    /// \attention The object will be invalid after calling this function
    void (* const Close)(Reader* self);
};

ABSTRACT LIB_CLASS _Reader
{
    const ReaderVTable* vtable;
};

/// Upcasting a concrete class object inherited from Reader to Reader
/// \return The Reader type object
/// \attention This won't check whether the given object is inherited from Reader
API_MODULE Reader* AsReader(void* reader);

/// Read characters into an array
/// \param self The Reader type object
/// \param buf The array to read into
/// \return The number of characters read, or -1 if the end of the stream is reached
API_MODULE int Reader_ReadAll(Reader* self, char buf[]);


#endif //DATALIBC_READER_H
