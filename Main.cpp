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
    StackCtor(&stk, 2);
    StackPush(&stk, 1);
    StackPush(&stk, 2);
    StackPush(&stk, 3);
    StackPush(&stk, 4);
    StackPush(&stk, 5);
    StackPush(&stk, 6);

    printf("\n hash in struct is %d\n", stk.struct_hash);
    hash_t k = SumHash(&stk, sizeof(struct stack));
    printf("SumHash is %u\n", k);

    //stk.capacity = 5;
    //stk.size = 6;
    /*
    StackPush(&stk, 1);
    StackPush(&stk, 2);
    StackPush(&stk, 3);
    StackPush(&stk, 4);
    StackPush(&stk, 5);
    StackPush(&stk, 6);
    STACK_DUMP(&stk);

    elem a = 1;
    StackPop(&stk, &a);
    StackPop(&stk, &a);
    STACK_DUMP(&stk);
    */
    StackDtor(&stk);
    }
