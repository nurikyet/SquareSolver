#include <stdio.h>

#include "Stack.h"
#include "logfile.h"

int main(const int argc, const char* argv[])
    {
    #ifdef LOG
        OpenLog(argv[0]);
    #endif
    struct stack stk = {};
    StackCtor(&stk, 20);
    STACK_DUMP(&stk);
    StackPush(&stk, 5);
    /*for (int i = 0; i < 10; i++)
        {
        StackPush(&stk, i);
        } */
    //StackDtor(&stk);
    STACK_DUMP(&stk);
    }
