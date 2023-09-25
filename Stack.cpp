#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Types.h"
#include "Hash.h"
#include "Stack.h"
#include "logfile.h"
/*
Stack cur_stack;
Stack_Ctor(&cur_stack, 10);
STACK_CONSTRUCT(cur_stack, 10)
{
Stack cur_stack;//äåôàéí
    //  êîíñòðóêòîð
    = #cur_stack;
}
*/
int StackCtor(struct stack* stk, size_t cpt)
    {
    assert(stk);

    stk->capacity = cpt;

    #ifdef WITH_CANARY
        stk->data = (elem*) calloc((stk->capacity) * sizeof(elem) + 2 * sizeof(canary_t), 1);
        if (stk->data == nullptr)
            {
            stk->capacity = 0;
            return (int)Error::ERROR_MEMORY;
            }

        elem* left_elem        = (elem*)((char*)(stk->data) + sizeof(canary_t));
        canary_t* first_canary = (canary_t*) ((char*)(stk->data));
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + (stk->capacity) * sizeof(elem));

        *first_canary = canary_value;
        *last_canary  = canary_value;

        stk->stack_first = canary_value;
        stk->stack_last  = canary_value;

        //printf("Address is %p    %zu    %x\n", *last_canary, *last_canary,   *last_canary);

    #else
        stk->data = (elem*) calloc(stk->capacity, sizeof(elem)); // stk->capacity is not initialized yet!
        if (stk->data == nullptr)
            {
            stk->capacity = 0;
            return (int)Error::ERROR_MEMORY;
            }
        elem* left_elem = (stk->data);
    #endif

    stk->data = left_elem;
    stk->size = 0;

    #ifdef HASH
        ChangeHash(stk);
    #endif
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackDtor(struct stack* stk)
    {
    assert(stk);
    VERIFY(stk)
    free(stk->data);

    stk->data     = nullptr;
    stk->size     = 0;
    stk->capacity = 0;

    #ifdef WITH_CANARY
        stk->stack_first  = 0;
        stk->stack_last   = 0;

    #endif

    #ifdef HASH
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
        int newcapacity = multiplier*(stk->capacity);
        StackRealloc(stk, newcapacity);
        }
    (stk->data)[(stk->size)++] = value;
    //stk->size += 1;

    #ifdef HASH
        ChangeHash(stk);
    #endif
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

elem StackPop(struct stack* stk, elem* retvalue)
    {
    assert(stk);
    VERIFY(stk)

    *retvalue = (stk->data)[(stk->size)--];

    if ((stk->size) == (stk->capacity)/multiplier)
        {
        int newcapacity = (stk->capacity)/multiplier;
        if (newcapacity == 0)
            {
            newcapacity = 1;
            }

        StackRealloc(stk, newcapacity);
        }

    VERIFY(stk);
    #ifdef HASH
        ChangeHash(stk);
    #endif
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackOk(struct stack* stk)
    {

    int result = 0;
    #ifdef HASH
        if (!HashOkData(stk))
            {
            result |= (int)Error::ERROR_DATA;
            }
    #endif

    #ifdef HASH
        if (!HashOkStruct(stk))
            {
            result |= (int)Error::ERROR_STRUCT;
            }
    #endif

    #ifdef WITH_CANARY
        if (stk->stack_first != canary_value or stk->stack_last != canary_value)
            {
            result |= (int)Error::ERROR_STRUCT_CANARY;
            }
        canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* last_canary   = (canary_t*)((char*) stk->data + (stk->capacity) * sizeof(elem));
        if (*first_canary != canary_value or *last_canary != canary_value)
            {
            result |= (int)Error::ERROR_DATA_CANARY;
            }
    #endif
    if (!stk->capacity)
        {
        result |= (int)Error::ERROR_CAPACITY;
        }
    if (stk->size > stk->capacity)
        {
        result |= (int)Error::ERROR_SIZE;
        //return (int)Error::ERROR_SIZE;
        }
    if (!stk->data)
        {
        result |= (int)Error::ERROR_DATA;
        //return (int)Error::ERROR_DATA ;
        }
    return result;
    }

//-----------------------------------------------------------------------------

void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line)
    {
    assert(stk);
    assert(func);
    assert(file);
    assert(stk->data);

    fprintf(fp, "-----------------------------------------------------------------------------\n");
    fprintf(fp, "START\n");

    fprintf(fp, "Stack[%p] \"stk\" from %s(%d) in function - %s.\n", stk,  file, line, func);
    fprintf(fp, "{\n");


    #ifdef WITH_CANARY
        fprintf(fp, "First canary in stack is %x\n", stk->stack_first);
        fprintf(fp, "size < %d\n", stk->size);
        fprintf(fp, "capacity = %d\n", stk->capacity);

        canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* last_canary   = (canary_t*)((char*) stk->data + stk->capacity * sizeof(elem));

        fprintf(fp, "first canary in data is > %x\n"
                    "last canary in data  is > %x\n", *first_canary, *last_canary);

        fprintf(fp, "data[%p]\n", stk->data);
        PrintStack(fp, stk);
        fprintf(fp, "Last canary in stack is %x\n", stk->stack_last);

    #else
        fprintf(fp, "size < %d\n", stk->size);
        fprintf(fp, "capacity = %d\n", stk->capacity);
        fprintf(fp, "data[%p]\n", stk->data);
        PrintStack(fp, stk);
    #endif

    fprintf(fp, "}\n");

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

    printf("\n");
    }

//-----------------------------------------------------------------------------

int StackRealloc(struct stack *stk, int newcapacity)
    {
    assert(stk);
    VERIFY(stk)
    void* check = 0;


    #ifdef WITH_CANARY
        check = realloc((char*)stk->data - sizeof(canary_t), newcapacity * sizeof(elem*) + 2*sizeof(canary_t));
        if (check != nullptr)
                {
                stk->data = (elem*)check;
                }
            else
                {
                ChangeHash(stk);
                return (int) Error::ERROR_MEMORY;
                }


        elem* left_elem = (elem*)((char*)(stk->data) + sizeof(canary_t));

        canary_t* first_canary = (canary_t*) ((char*)(stk->data));
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + newcapacity * sizeof(elem));
        *(last_canary)         = canary_value;

    #else
        check = realloc(stk->data, newcapacity * sizeof(elem*));
        if (check != nullptr)
            {
            stk->data = (elem*)check;
            }
        else
            {
            #ifdef HASH
            ChangeHash(stk);
            #endif
            return (int) Error::ERROR_MEMORY;
            }
        elem* left_elem = (elem*)((char*)(stk->data));
    #endif

    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        stk->size     = 0;
        #ifdef HASH
            ChangeHash(stk);
        #endif
        return (int) Error::ERROR_MEMORY;
        }
    stk->data     = left_elem;
    stk->capacity = newcapacity;

    #ifdef HASH
        ChangeHash(stk);
    #endif
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

hash_t DataHash(struct stack* stk)
    {
    assert(stk);

    elem* data     = stk->data;
    size_t data_size = stk->capacity * sizeof(elem);
    hash_t hash      = 0;

    #ifdef WITH_CANARY
        data = (elem*)((char*) data - sizeof(canary_t));
        data_size += 2 * sizeof(canary_t);
        hash      = SumHash ((stk->data), data_size);
    #endif

    return hash;
    }

//-----------------------------------------------------------------------------

hash_t StructHash(struct stack* stk)
    {
    assert(stk);
    hash_t new_hash = 0;

    #ifdef HASH
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
    #ifdef HASH
        stk->struct_hash = StructHash(stk);
        stk->data_hash   = DataHash(stk);
    #endif
    }
