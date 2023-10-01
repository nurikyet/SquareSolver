#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Types.h"
#include "Hash.h"
#include "Stack.h"
#include "logfile.h"
#include "Error.h"

int StackCtor(struct stack* stk, size_t cpt)
    {
    assert(cpt);
    stk->capacity = cpt;

    int data_size = (stk->capacity) * sizeof(elem_t);
    elem_t* left_elem = (stk->data);

    IF_CANARY
    (
        data_size             += 2 * sizeof(canary_t);
        left_elem             += sizeof(canary_t);

        canary_t* first_canary = (canary_t*)(stk->data);
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + (stk->capacity) * sizeof(elem_t) + sizeof(canary_t));

        *first_canary = canary_value;
        *last_canary  = canary_value;

        stk->stack_first = canary_value;
        stk->stack_last  = canary_value;
    )

    stk->data = (elem_t*) calloc(data_size, sizeof(char));
    if (stk->data == nullptr)
                {
                stk->capacity = 0;
                return (int)Error::ERROR_MEMORY;
                }

    stk->data = left_elem;
    stk->size = 0;

    IF_HASH(ChangeHash(stk);)

    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackDtor(struct stack* stk)
    {
    VERIFY(stk)

    IF_CANARY
    (
        stk->data = stk->data - sizeof(canary_t);
        stk->stack_first  = 0;
        stk->stack_last   = 0;
    )

    free(stk->data);
    stk->data     = nullptr;
    stk->size     = 0;
    stk->capacity = 0;

    IF_HASH
    (
    stk->data_hash   = 0;
    stk->struct_hash = 0;
    )

    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackPush(struct stack* stk, const elem_t value)
    {
    VERIFY(stk)

    if ((stk->size) >= (stk->capacity))
        {
        int new_capacity = MULTIPLIER1*(stk->capacity);

        StackRealloc(stk, new_capacity);
        }
    (stk->data)[(stk->size)] = value;
    (stk->size)++;

    IF_HASH(ChangeHash(stk);)
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackPop(struct stack* stk, elem_t* retvalue)
    {
    VERIFY(stk)

    (stk->size)--;
    *retvalue = (stk->data)[(stk->size)];
    (stk->data)[(stk->size)] = -777;

    if ((stk->size) == (stk->capacity)/(MULTIPLIER2))
        {
        int new_capacity = (stk->capacity)/MULTIPLIER2;
        if (new_capacity == 0)
            {
            new_capacity = 1;
            }
        StackRealloc(stk, new_capacity);
        }

    IF_HASH(ChangeHash(stk);)
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line)
    {
    assert(stk);
    assert(func);
    assert(file);
    assert(stk->data);

    fprintf(fp, "---------------------------------------------------\n");
    fprintf(fp, "START\n");

    fprintf(fp, "Stack[%p] \"stk\" from %s(%d) in function - %s.\n", stk,  file, line, func);
    fprintf(fp, "{\n");
    fprintf(fp, "size < %d\n", stk->size);
    fprintf(fp, "capacity = %d\n", stk->capacity);

    IF_CANARY
    (
        fprintf(fp, "First canary in stack is %lX\n", stk->stack_first);
        fprintf(fp, "Last canary in stack is %lX\n", stk->stack_last);

        canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* last_canary   = (canary_t*)((char*) stk->data + (stk->capacity) * sizeof(elem_t));

        fprintf(fp,"first CANARY in data  is > %lX\n", *first_canary);
        fprintf(fp,"last CANARY  in data  is  >%lX\n", *last_canary);
    )

    fprintf(fp, "data[%p]\n", stk->data);
    PrintStack(fp, stk);
    }

//-----------------------------------------------------------------------------

void PrintStack(FILE* fp, const struct stack *stk)
    {
    fprintf(fp, "stack %d > \n", stk->size);
    for (int i = 0; i < (stk->size); i++)
        {
        fprintf(fp, "data[%d] =", i);
        fprintf(fp, printfelem, stk->data[i]);
        fprintf(fp, "\n");
        }
    fprintf(fp, "All DATA\n");
    for (int i = 0; i < stk -> capacity; i++)
        {
        fprintf(fp, "data[%d] =", i);
        fprintf(fp, printfelem, stk->data[i]);
        fprintf(fp, "\n");
        }

    fprintf(fp, "\n");
    }

//-----------------------------------------------------------------------------

int StackRealloc(struct stack *stk, int new_capacity)
    {
    printf("start realloc\n");
    assert(stk);
    VERIFY(stk)

    int data_size = new_capacity * sizeof(elem_t*);

    IF_CANARY
    (
        data_size += 2*sizeof(canary_t);
        stk->data -= sizeof(canary_t);
    )

    elem_t *check = (elem_t*)realloc(stk->data, data_size);
    if (check != nullptr)
        {
        stk->data = check;
        }
    else
        {
        IF_HASH(ChangeHash(stk);)
        STACK_DUMP(stk)
        return (int) Error::ERROR_MEMORY;
        }

    elem_t* left_elem = (elem_t*)((char*)(stk->data));

    IF_CANARY
    (
        left_elem += sizeof(canary_t);
        *((canary_t*)((char*)(stk->data) + (stk->capacity)*sizeof(elem_t) + sizeof(canary_t))) = -777;
        canary_t* first_canary = (canary_t*) ((char*)(stk->data));
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + new_capacity * sizeof(elem_t) + sizeof(canary_t));
        *(last_canary)         = canary_value;
    )

    stk->data     = left_elem;
    stk->capacity = new_capacity;

    IF_HASH(ChangeHash(stk);)
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

void PrintInConsole(const struct stack *stk)
    {
    printf("stack %d > \n", stk->size);
    for (int i = 0; i < (stk->size); i++)
        {
        printf("data[%d] =", i);
        printf(printfelem, stk->data[i]);
        printf("\n");
        }
    printf("All DATA\n");
    for (int i = 0; i < stk -> capacity; i++)
        {
        printf("data[%d] =", i);
        printf(printfelem, stk->data[i]);
        printf("\n");
        }

    printf("\n");
    }
