#include <stdio.h>

#include "Stack.h"

void PrintError(FILE* fp, int result)
    {
    #ifdef HASH
        if (result && (int)Error::ERROR_DATA != 0)
            fprintf(fp, "You have error in data, when you work with hash, please check your all addresses \n");
    #endif
    #ifdef HASH
        if (result && (int)Error::ERROR_STRUCT != 0)
            fprintf(fp, "You have error in struct, when you work with hash, please check your all addresses \n");
    #endif
    #ifdef WITH_CANARY
        if (result && (int)Error::ERROR_DATA_CANARY != 0)
            fprintf(fp, "You have error in canary of data, please check your all addresses \n");
    #endif
    if (result && (int)Error::ERROR_CAPACITY != 0)
        fprintf(fp, "Capacity mast be > 0\n");
    if (result && (int)Error::ERROR_SIZE != 0)
        fprintf(fp, "Size mast be <= capacity\n");
    if (result && (int)Error::ERROR_DATA != 0)
        fprintf(fp, "address of data != nullptr\n");
    if (result && (int)Error::ERROR_STRUCT != 0)
        fprintf(fp, "address of struct != nullptr\n");
    }
