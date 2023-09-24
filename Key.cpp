#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "Types.h"
#include "Key.h"
#include "logfile.h"

int StackCtor(struct stack* stk, size_t cpt)
    {
    assert(stk);
    stk->size = 0;
    stk->capacity = cpt;
    stk->data = (elem*) calloc(stk->capacity, sizeof(elem));
    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        StackDump(LOG_FILE, stk);
        return (int)Error::ERROR_MEMORY;
        }
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackDtor(struct stack* stk)
    {
    VERIFY(stk)
    free(stk->data);
    stk->data = nullptr;
    stk->size = 0;
    stk->capacity = 0;
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

int StackPush(struct stack* stk, const elem value)
    {
    assert(stk);
    assert(stk->data != NULL);
    VERIFY(stk)
    /*
    if (StackOk(stk) != 0)
        {
        printf("Код ошибки номер %d\n", StackOk(stk));
        StackDump(stk);
        }
    */
    if ((stk->size) == (stk->capacity))
        {
        int newcapacity = 2*(stk->capacity);
        StackRealloc(stk, newcapacity);
        }
    (stk->data)[stk->size++] = value;
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------

elem StackPop(struct stack* stk, elem retvalue)
    {
    assert(stk);
    VERIFY(stk)
    retvalue = (stk->data)[stk->size--];
    if ((stk->size) == (stk->capacity)/2)
        {
        int newcapacity = 1;
        if ((stk->capacity)/2 == 0)
            {
            newcapacity = 1;
            }
        else
            {
            newcapacity = (stk->capacity)/2;
            }
        StackRealloc(stk, newcapacity);
        }
    return(retvalue);
    }

//-----------------------------------------------------------------------------

int StackOk(const struct stack* stk)
    {
    if (!stk->capacity)
        {
        return (int)Error::ERROR_CAPACITY;
        }
    if (!stk->size)
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

void StackDump(FILE* LOG_FILE, const struct stack* stk)
    {
    fprintf(LOG_FILE, "Stack[%p] \"stk\" from %s(%d) in function - %s.\n", stk,  __FILE__, __LINE__, __func__);
    fprintf(LOG_FILE, "{\n");
    fprintf(LOG_FILE, "size < %d\n", stk->size);
    fprintf(LOG_FILE, "capacity = %d\n", stk->capacity);
    fprintf(LOG_FILE, "data[%p]\n", stk->data);
    PrintStack(stk, PrintStackValue);
    fprintf(LOG_FILE, "}\n");
    }

//-----------------------------------------------------------------------------

void PrintStackValue(const elem value)
    {
    printf(printfelem, value);
    }

//-----------------------------------------------------------------------------

void PrintStack(const stack *stk, void (*PrintStackValue)(const elem))
    {
    printf("stack %d > ", stk->size);
    for (int i = 0; i < (stk->size) - 1; i++)
        {
        PrintStackValue(stk->data[i]);
        printf(" | ");

        if (i == stk->size - 2)
            {
            PrintStackValue(stk->data[i]);
            }
        }

    printf("\n");
    }

//-----------------------------------------------------------------------------

int StackRealloc(stack *stk, int newcapacity)
    {
    assert(stk);
    VERIFY(stk)
    realloc(stk->data, newcapacity * sizeof(elem*));

    if (stk->data == nullptr)
        {
        stk->capacity = 0;
        stk->size     = 0;
        return (int) Error::ERROR_MEMORY;
        }
    stk->capacity = newcapacity;
    VERIFY(stk)
    return (int)Error::NO_ERROR;
    }

//-----------------------------------------------------------------------------


