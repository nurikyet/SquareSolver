#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Types.h"
#include "Hash.h"
#include "Key.h"
#include "logfile.h"

int StackCtor(struct stack* stk, size_t cpt)
    {
    assert(stk);

    #if WITH_CANARY
    stk->data = (elem*) calloc((stk->capacity) * sizeof(elem) + 2 * sizeof(canary_t), 1);
    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        return (int)Error::ERROR_MEMORY;
        }
    elem* left_elem        = (elem*)((char*)(stk->data) + sizeof(canary_t));
    canary_t* first_canary = (canary_t*) ((char*)(stk->data));
    canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + (stk->capacity) * sizeof(elem) + sizeof(canary_t));

    *first_canary = canary_value;
    *last_canary  = canary_value;

    stk->stack_first = canary_value;
    stk->stack_last  = canary_value;
    #endif

    #if !WITH_CANARY
    stk->data = (elem*) calloc(stk->capacity, sizeof(elem));
    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        return (int)Error::ERROR_MEMORY;
        }
    elem* left_elem = (stk->data);
    #endif

    stk->data = left_elem;
    stk->size = 0;
    stk->capacity = cpt;

    ChangeHash(stk);

    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackDtor(struct stack* stk)
    {
    VERIFY(stk)
    free(stk->data);

    stk->data     = nullptr;
    stk->size     = 0;
    stk->capacity = 0;

    #if WITH_CANARY
    stk->stack_first  = 0;
    stk->stack_last   = 0;

    #endif

    #if HASH
    stk->data_hash   = 0;
    stk->struct_hash = 0;
    #endif
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackPush(struct stack* stk, const elem value)
    {
    assert(stk);
    assert(stk->data != NULL);

    VERIFY(stk)

    if ((stk->size) == (stk->capacity))
        {
        int newcapacity = 2*(stk->capacity);
        StackRealloc(stk, newcapacity);
        }
    (stk->data)[stk->size++] = value;

    VERIFY(stk)

    ChangeHash(stk);

    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

elem StackPop(struct stack* stk, elem* retvalue)
    {
    assert(stk);
    VERIFY(stk)

    *retvalue = (stk->data)[stk->size--];

    if ((stk->size) == (stk->capacity)/2)
        {
        int newcapacity = (stk->capacity)/2;
        if (newcapacity == 0)
            {
            newcapacity = 1;
            }

        StackRealloc(stk, newcapacity);
        }

    VERIFY(stk);

    ChangeHash(stk);

    return(*retvalue);
    }

//-----------------------------------------------------------------------------

int StackOk(const struct stack* stk)
    {
    #if HASH
    if (!HashOkData(stk))
        {
        return (int)Error::ERROR_DATA;
        }
    #endif

    #if WITH_CANARY
    if (stk->stack_first != canary_value or stk->stack_last != canary_value)
        {
        return (int)Error::ERROR_STRUCT_CANARY;
        }
    canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
    canary_t* last_canary   = (canary_t*)((char*) stk->data + (stk->capacity) * sizeof(elem));
    if (*first_canary != canary_value or *last_canary != canary_value)
        {
        return (int)Error::ERROR_DATA_CANARY;
        }
    #endif
    if (!stk->capacity)
        {
        return (int)Error::ERROR_CAPACITY;
        }
    if (stk->size > stk->capacity)
        {
        return (int)Error::ERROR_SIZE;
        }
    if (!stk->data)
        {
        return (int)Error::ERROR_DATA ;
        }
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line)
    {
    assert(stk);
    assert(func);
    assert(file);

    fprintf(fp, "-----------------------------------------------------------------------------\n");
    fprintf(fp, "START\n");

    fprintf(fp, "Stack[%p] \"stk\" from %s(%d) in function - %s.\n", stk,  file, line, func);
    fprintf(fp, "{\n");

    #if !WITH_CANARY
    fprintf(fp, "size < %d\n", stk->size);
    fprintf(fp, "capacity = %d\n", stk->capacity);
    fprintf(fp, "data[%p]\n", stk->data);
    PrintStack(stk, PrintStackValue);
    #endif

    #if WITH_CANARY
    fprintf(fp, "First canary in stack is %x\n", stk->stack_first);
    fprintf(fp, "size < %d\n", stk->size);
    fprintf(fp, "capacity = %d\n", stk->capacity);

    canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
    canary_t* last_canary = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem));

    fprintf(fp, "first canary in data is > %x\n"
                "last canary in data  is > %x\n", *first_canary, *last_canary);

    fprintf(fp, "data[%p]\n", stk->data);
    PrintStack(fp, stk, PrintStackValue);
    fprintf(fp, "Last canary in stack is %x\n", stk->stack_last);
    #endif
    fprintf(fp, "}\n");

    }

//-----------------------------------------------------------------------------

void PrintStackValue(FILE* fp, const elem value)
    {
    fprintf(fp, printfelem, value);
    }

//-----------------------------------------------------------------------------

void PrintStack(FILE* fp, const stack *stk, void (*PrintStackValue)(FILE* fp, const elem value))
    {
    fprintf(fp, "stack %d > ", stk->size);
    for (int i = 0; i < (stk->size) - 1; i++)
        {
        PrintStackValue(fp, (stk->data)[i]);
        fprintf(fp, " | ");

        if (i == stk->size - 2)
            {
            PrintStackValue(fp, (stk->data)[i]);
            }
        }

    printf("\n");
    }

//-----------------------------------------------------------------------------

int StackRealloc(stack *stk, int newcapacity)
    {
    assert(stk);
    VERIFY(stk)

    #if !WITH_CANARY
    realloc(stk->data, newcapacity * sizeof(elem*));
    elem* left_elem = (elem*)((char*)(stk->data));
    #endif

    #if WITH_CANARY
    realloc((char*)stk->data - sizeof(canary_t), newcapacity * sizeof(elem*) + 2*sizeof(canary_t));
    elem* left_elem = (elem*)((char*)(stk->data) + sizeof(canary_t));

    canary_t* first_canary = (canary_t*) ((char*)(stk->data));
    canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + newcapacity * sizeof(elem) + sizeof(canary_t));
    *(last_canary)         = canary_value;
    #endif

    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        stk->size     = 0;
        return (int) Error::ERROR_MEMORY;
        }
    stk->data     = left_elem;
    stk->capacity = newcapacity;

    ChangeHash(stk);
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

hash_t DataHash(const struct stack* stk)
    {
    assert(stk);

    elem* data     = stk->data;
    size_t data_size = stk->capacity * sizeof(elem);
    hash_t hash      = 0;

    #if WITH_CANARY
    data      = (elem*)((char*) (stk->data) - sizeof(canary_t));
    data_size = data_size + 2 * sizeof(canary_t);
    hash      = SumHash ((stk->data), sizeof((stk->data)));
    #endif

    return hash;
    }

//-----------------------------------------------------------------------------

hash_t StructHash(const struct stack* stk)
    {
    assert(stk);
    hash_t new_hash = 0;

    #if HASH
    hash_t third    = stk->struct_hash;
    stk->struct_hash = 0;

    new_hash = SumHash(stk, sizeof(struct stack));

    stk->struct_hash = third;
    #endif

    return new_hash;
    }


//-----------------------------------------------------------------------------

void ChangeHash(struct stack* stk)
    {
    #if HASH
    stk->struct_hash = StructHash(stk);
    stk->data_hash   = DataHash(stk);
    #endif
    }
