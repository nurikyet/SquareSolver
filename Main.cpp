#include <stdio.h>

#include "Key.h"

int main()
    {
    struct stack stk = {};
    StackCtor(&stk, 10);
    for (int i = 0; i < 20; i++)
        {
        StackPush(&stk, i);
        }
    //PrintStack(&stk, PrintStackValue);
    StackPop(&stk, 5);
    //PrintStack(&stk, PrintStackValue);
    StackDtor(&stk);

    }
