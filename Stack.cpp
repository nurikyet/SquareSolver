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

    IF_CANARY
    (
        stk->data = (elem*) calloc((stk->capacity) * sizeof(elem) + 2 * sizeof(canary_t), sizeof(char));
        if (stk->data == nullptr)
            {
            stk->capacity = 0;
            return (int)Error::ERROR_MEMORY;
            }

	// TODO: Extract!
        elem* left_elem        = (elem*)((char*)(stk->data) + sizeof(canary_t));
        canary_t* first_canary = (canary_t*)(stk->data);
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + (stk->capacity) * sizeof(elem) + sizeof(canary_t));
        *first_canary = canary_value;
        *last_canary  = canary_value;

        stk->stack_first = canary_value;
        stk->stack_last  = canary_value;
    ,
        stk->data = (elem*) calloc((stk->capacity)*sizeof(elem), 1); // TODO: Do not duplicate! COPIPASTA BL
        if (stk->data == nullptr)
            {
            stk->capacity = 0;
            return (int)Error::ERROR_MEMORY;
            }
        elem* left_elem = (stk->data);
    )

    stk->data = left_elem; // TODO: Named designators, learn about them!
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
        free((char*)stk->data - sizeof(canary_t));
        stk->stack_first  = 0; // TODO: *stk = {}?
        stk->stack_last   = 0;
    ,
        free(stk->data);
    )

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

int StackPush(struct stack* stk, const elem value)
    {
    VERIFY(stk)
    //(stk->size)++;

    if ((stk->size) >= (stk->capacity))
        {
	int newcapacity = MULTIPLIER*(stk->capacity); // TODO: another naming style, yet another one!

        StackRealloc(stk, newcapacity);
        }
    (stk->data)[(stk->size)] = value;
    (stk->size)++;

    IF_HASH(ChangeHash(stk);)
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackPop(struct stack* stk, elem* retvalue)
    {
    VERIFY(stk)

    (stk->size)--; // TODO: Why so much CRUGLIE SCOBOCHKI?
    *retvalue = (stk->data)[(stk->size)];
    (stk->data)[(stk->size)] = 0;

    if ((stk->size) == (stk->capacity)/(DOUBLE*MULTIPLIER))
        {
        int newcapacity = (stk->capacity)/MULTIPLIER;
        if (newcapacity == 0)
            {
            newcapacity = 1;
            }
        StackRealloc(stk, newcapacity);
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
    printf("i am dump, i start my work\n");

    IF_CANARY
    (
        fprintf(fp, "First canary in stack is %x\n", stk->stack_first);
        fprintf(fp, "size < %d\n", stk->size);
        fprintf(fp, "capacity = %d\n", stk->capacity);

        canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* last_canary   = (canary_t*)((char*) stk->data + (stk->capacity) * sizeof(elem));

        fprintf(fp,"first WITH_CANARY in data  is > %X\n", *first_canary);
        fprintf(fp,"last WITH_CANARY in data  is > %X\n", *last_canary);

        fprintf(fp, "data[%p]\n", stk->data);
        PrintStack(fp, stk);
        fprintf(fp, "Last canary in stack is %X\n", stk->stack_last);
    ,
        fprintf(fp, "size < %d\n", stk->size);
        fprintf(fp, "capacity = %d\n", stk->capacity);
        fprintf(fp, "data[%p]\n", stk->data);
        PrintStack(fp, stk);
    )
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

int StackRealloc(struct stack *stk, int newcapacity) // TODO: I don't even wanna look, clean up mess...
    {
    printf("start realloc\n");
    assert(stk);
    VERIFY(stk)
    IF_CANARY
    (
        elem* check1 = (elem*)realloc((char*)stk->data - sizeof(canary_t), newcapacity * sizeof(elem*) + 2*sizeof(canary_t));
        //printf("\n%p\n", check1);
        //printf("\n%p\n", stk->data);
        //printf("\n%p\n", stk->data- sizeof(canary_t));
        if (check1 != nullptr)
                {
                stk->data = check1;
                }
            else
                {
                StackDtor(stk);
                #ifdef HASH
                    ChangeHash(stk);
                #endif
                return (int) Error::ERROR_MEMORY;
                }
        //printf("16_1 = %ul\n", *(canary_t*)((char*)(stk->data) + (stk->capacity)*sizeof(elem) + sizeof(canary_t)));
        elem* left_elem = (elem*)((char*)(stk->data) + sizeof(canary_t));
        *((canary_t*)((char*)(stk->data) + (stk->capacity)*sizeof(elem) + sizeof(canary_t))) = 0;
        //printf("16_2 = %ul\n", *(canary_t*)((char*)(stk->data) + (stk->capacity)*sizeof(elem) + sizeof(canary_t)));
        canary_t* first_canary = (canary_t*) ((char*)(stk->data));
        //printf("fc = %ul    fc = %X \n", *first_canary, *first_canary);
        canary_t* last_canary  = (canary_t*) ((char*)(stk->data) + newcapacity * sizeof(elem) + sizeof(canary_t));
        //printf("lc = %ul    lc = %X\n", *last_canary, *last_canary);
        *(last_canary)         = canary_value;
        //printf("lc = %ul    lc = %X\n", *last_canary, *last_canary);
    ,
        elem *check2 = (elem*)realloc(stk->data, newcapacity * sizeof(elem*));
        if (check2 != nullptr)
            {
            stk->data = (elem*)check2;
            }
        else
            {
            IF_HASH(ChangeHash(stk);)
            return (int) Error::ERROR_MEMORY;
            }
        elem* left_elem = (elem*)((char*)(stk->data));
    )
    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        stk->size     = 0;
        IF_HASH(ChangeHash(stk);)
        return (int) Error::ERROR_MEMORY;
        }
    stk->data     = left_elem;
    stk->capacity = newcapacity;
    //printf("your newcapacity is %d\n", stk->capacity);

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
