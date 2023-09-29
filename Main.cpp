#include <stdio.h>

#include "Hash.h"
#include "Stack.h"
#include "logfile.h"


int main(const int argc, const char* argv[])
    {
    #ifdef LOG
        OpenLog(argv[0]);
    #endif
    struct stack stk = {};
    StackCtor(&stk, 20);

    StackPush(&stk, 1);
    StackPush(&stk, 2);
    StackPush(&stk, 3);
    StackPush(&stk, 4);
    StackPush(&stk, 5);
    StackPush(&stk, 6);

    printf("\n hash in struct is %d\n", stk.struct_hash);
    hash_t k = SumHash(&stk, sizeof(struct stack));
    printf("SumHash is %u\n", k);

    STACK_DUMP(&stk);
    StackDtor(&stk);
    }
