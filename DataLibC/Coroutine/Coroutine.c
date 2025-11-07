//
// Created by 18030 on 2025/4/15.
//

#include <malloc.h>
#include "Coroutine.h"

typedef struct Coroutine
{
    CoroutineFunc fun;
    CoroutineBehaviour behaviour;
    CoroutineState state;
    CoroutineContext context;
    void** args;
    double value;
} Coroutine;

Coroutine* CreateCoroutine(CoroutineFunc fun, void** args, CoroutineContext context, CoroutineBehaviour behaviour)
{
    if (fun == NULL)
    {
        RuntimeErrorLog(InvalidArgument, "Invalid function passing to the construction!");
        return NULL;
    }

    Coroutine* coroutine = malloc(sizeof(Coroutine));
    coroutine->behaviour = behaviour;
    coroutine->fun = fun;
    coroutine->state = kCoroutineState_Start;
    coroutine->args = args;
    coroutine->context = context;

    if (behaviour == kInitialSuspend_Never)
        StartCoroutine(coroutine, args);

    return coroutine;
}

CoroutineState GetCoroutineState(Coroutine* self)
{
    return self->state;
}

void StoreCoroutineValue(Coroutine* self, double value)
{
    self->value = value;
}

void DestroyCoroutine(Coroutine* self)
{

}

bool StartCoroutine(Coroutine* self, void** args)
{
    if (self->state != kCoroutineState_Start)
    {
        RuntimeErrorLog(IllegalState, "A coroutine can not be start again!");
        return false;
    }
    self->state = kCoroutineState_Running;
    if (args != NULL)
        self->args = args;
    self->fun(self, self->args);
    return true;
}

void SetCoroutineState(Coroutine* self, CoroutineState state)
{
    self->state = state;
}

bool ResumeCoroutine(Coroutine* self)
{
    if (self->state <= 0)
    {
        RuntimeErrorLog(IllegalState, "This coroutine is not in the suspended state!");
        return false;
    }
    self->fun(self, self->args);
    return true;
}


bool IsCoroutineFinished(Coroutine* self)
{
    return self->state == kCoroutineState_Finish;
}

bool CoroutineMoveNext(Coroutine* self)
{
    self->fun(self, self->args);
    return true;
}

double GetCoroutineValue(Coroutine* self)
{
    return self->value;
}

CoroutineContext GetCoroutineContext(Coroutine* self)
{
    return self->context;
}
