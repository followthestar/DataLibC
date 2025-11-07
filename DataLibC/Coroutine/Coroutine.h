//
// Created by 18030 on 2025/4/15.
//

#ifndef DATALIBC_COROUTINE_H
#define DATALIBC_COROUTINE_H

#include "DataLibC/Tool/ExportModuleMacro.h"
#include "DataLibC/Tool/LogMacro.h"

#define YIELD_BEGIN \
switch ((int)GetCoroutineState(self)) { \
    case kCoroutineState_Start:    \
        RuntimeErrorLog(IllegalState, "Running a coroutine function without StartCoroutine is not allowde!"); \
        break;                \
    case kCoroutineState_Finish: \
        RuntimeErrorLog(IllegalState, "A finished coroutine can not be perform again!");       \
        break;      \
    case kCoroutineState_Suspend:  \
        RuntimeErrorLog(IllegalState, "This coroutine has been suspended! Use ResumeCoroutine to restore it."); \
        break;\
    case kCoroutineState_Running:       \
    {

#define YIELD_END \
    }                 \
}                 \
SetCoroutineState(self, kCoroutineState_Finish);

#define YIELD_RETURN(v) {_Generic((v), \
default: StoreCoroutineValue(self, v)      \
);                                    \
SetCoroutineState(self, __LINE__);     \
return;}    case __LINE__:

#define YIELD_QUIT \
{SetCoroutineState(self, kCoroutineState_Finish);return;}


#define COROUTINE_FOREACH(c, opt) \
while (!IsCoroutineFinished((c)))\
{\
opt \
CoroutineMoveNext((c));\
}


/// A handle to a coroutine. You can use this handle to operate on the related coroutine. Such as resuming, suspending,
/// destroying, etc.
/// \attention Since we don't have a real coroutine system in C language, so we do this through a fake way. This means
/// you need to define the coroutine functions following the rules below.
/// \details The coroutine function must have two arguments \p self and \p args.
/// And the coroutine function should return void.
/// Here we want to create a coroutine that prints "Hello World!" and then suspends. The
/// code might look like this:
/// \code
/// void CoroutineFunction(Coroutine* self, void** args)
/// {
///     // Here you need to use YIELD_BEGIN macro to mark the beginning of the coroutine range
///     YIELD_BEGIN
///     // After this macro, you can write your logic here, and print "Hello World!"
///     printf("Hello World!\n");
///     // Now we want to suspend the coroutine and back to the caller, just use YIELD_RETURN
///     YIELD_RETURN(NULL)      // NULL means we don't care about the yield value
///     // And then we can resume the coroutine after we call ResumeCoroutine outside
///     printf("Bye Coroutine!\n");
///     // Always remember to add YIELD_END macro at the end of the coroutine function
///     YIELD_END
/// }
/// \endcode
///
typedef struct Coroutine Coroutine;

/// A coroutine context is used to store information about the coroutine. When we yield return from a coroutine and back
/// to the caller, the last local variable will be lost, so if we resume the coroutine and continue to run, undefined
/// behavior will happen. Thus we need to store the context manually.
/// This just a alias of void*, which can be anything you want. This means all the local variables in the coroutine that won't
/// maintain a fixed value from the coroutine arguments should be stored in or get from the context.
typedef void* CoroutineContext;

#define CONTEXT_MEMBER(type, x) (((type*)GetCoroutineContext(self))->x)

/// The function that will be executed in the coroutine.
typedef void (* CoroutineFunc)(Coroutine* self, void** args);

typedef enum CoroutineBehaviour
{
    kInitialSuspend_Never = 1 << 0,
    kInitialSuspend_Always = 1 << 1,
} CoroutineBehaviour;

typedef enum CoroutineState
{
    kCoroutineState_Start = -3,
    kCoroutineState_Running = -2,
    kCoroutineState_Suspend = -1,
    kCoroutineState_Finish = 0,
} CoroutineState;


/// Create a new coroutine.
/// \param fun The function that will be executed in the coroutine.
/// \param args The arguments to be passed to the coroutine.
/// \param behaviour Should a coroutine be suspended at the beginning?
/// \return A handle to the coroutine. NULL means the argument is invalid.
/// \attention If behaviour is kInitialSuspend_Never, the args should not be NULL.
API_MODULE Coroutine*
CreateCoroutine(CoroutineFunc fun, void** args, CoroutineContext context, CoroutineBehaviour behaviour);

/// Start a coroutine with given arguments. After calling this, this coroutine can not be
/// start again through this function.
/// \param self The coroutine to be started.
/// \param args The arguments to be passed to the coroutine.
/// \return True if the coroutine is started successfully. False otherwise.
API_MODULE bool StartCoroutine(Coroutine* self, void** args);

/// Resume a coroutine if it has been suspended.
/// \param self The coroutine to be resumed.
/// \return True if the coroutine is resumed successfully. False means the coroutine is not suspended or it is not in the running state.
API_MODULE bool ResumeCoroutine(Coroutine* self);

/// Get the state of the coroutine.
/// \param self The coroutine to be checked.
/// \return The state of the coroutine.
API_MODULE CoroutineState GetCoroutineState(Coroutine* self);

/// Check if the coroutine is finished.
/// \param self The coroutine to be checked.
/// \return True if the coroutine is finished. False otherwise.
API_MODULE bool IsCoroutineFinished(Coroutine* self);

///
/// \param self
/// \return
API_MODULE bool CoroutineMoveNext(Coroutine* self);

///
/// \param self
/// \return
API_MODULE double GetCoroutineValue(Coroutine* self);

/// Get the context of the coroutine.
/// \param self The coroutine to be checked.
/// \return The context of the coroutine. Use explicit cast to get the correct type.
API_MODULE CoroutineContext GetCoroutineContext(Coroutine* self);

/// Destroy a coroutine.
/// \param self The coroutine to be destroyed.
API_MODULE void DestroyCoroutine(Coroutine* self);

/// This function is used by the macro, DON'T USE IT!!!
API_MODULE void SetCoroutineState(Coroutine* self, CoroutineState state);

/// This function is used by the macro, DON'T USE IT!!!
API_MODULE void StoreCoroutineValue(Coroutine* self, double value);

#endif //DATALIBC_COROUTINE_H
