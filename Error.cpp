#include <stdio.h>

#include "Types.h"
#include "Hash.h"
#include "Stack.h"
#include "logfile.h"
#include "Error.h"

void PrintError(FILE* fp, int result)
    {                                               // assert on fp
    IF_HASH
    (
        if (result & (int)Error::ERROR_DATA != 0)   // hey, have you ever tested how your PrintError function works? :(   now experssion under if is equivalent
                                                    // to (result & ((int)Error::ERROR_DATA != 0)), which works if and only if result's least significant bit is 1,
                                                    // but you obviously wanted something like ((result & (int)Error::ERROR_DATA) != 0)
                                                    // P.S. here's even WARNING, don't you pay attention to them? :(
            fprintf(fp, "You have error in data, when you work with hash, please check your all addresses \n");
        if (result & (int)Error::ERROR_STRUCT != 0)
            fprintf(fp, "You have error in struct, when you work with hash, please check your all addresses \n");
    )

    IF_CANARY
    (
        if (result & (int)Error::ERROR_DATA_CANARY != 0)
            fprintf(fp, "You have error in canary of data, please check your all addresses \n");
    )

    if (result & (int)Error::ERROR_CAPACITY != 0)
        fprintf(fp, "Capacity must be > 0\n");

    if (result & (int)Error::ERROR_SIZE != 0)
        fprintf(fp, "Size must be <= capacity\n");

    if (result & (int)Error::ERROR_DATA != 0)
        fprintf(fp, "address of data != nullptr\n");

    if (result & (int)Error::ERROR_STRUCT != 0)
        fprintf(fp, "address of struct != nullptr\n");
    }

//-----------------------------------------------------------------------------

int StackOk(FILE* fp, struct stack* stk)
    {
    int result = 0;
    IF_HASH
    (
        if (HashOkData(stk) == 0)
            {
            result |= (int)Error::ERROR_DATA;
            }
    )

    IF_CANARY
    (
        if (stk->stack_first != canary_value or stk->stack_last != canary_value)
            {
            result |= (int)Error::ERROR_STRUCT_CANARY;
            }

        canary_t* first_canary  = (canary_t*)((char*) stk->data - sizeof(canary_t));
        canary_t* last_canary   = (canary_t*)((char*) stk->data + (stk->capacity) * sizeof(elem_t));

        if (*first_canary != canary_value)
            {
            result |= (int)Error::ERROR_DATA_CANARY;
            }
        if (*last_canary != canary_value)
            {
            //printf("last in error(64) is %X", last_canary);
            result |= (int)Error::ERROR_DATA_CANARY;
            }
    )
    if (!stk->capacity)         // hmm, it's when capacity != 0? maybe < 0 instead...
        {
        result |= (int)Error::ERROR_CAPACITY;
        }
    if (!stk->size > stk->capacity)     // WARNING: why there is ! in if?
        {
        result |= (int)Error::ERROR_SIZE;
        }
    if (!stk->data)
        {
        result |= (int)Error::ERROR_DATA;
        }

    if (stk == nullptr)
        {
        result |= (int)Error::ERROR_STRUCT;
        }

    PrintError(fp, result);
    return result;
    }
