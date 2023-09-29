#include <stdio.h>

#include "Hash.h"
#include "Stack.h"
#include "logfile.h"


int main(const int argc, const char* argv[])
    {
    #ifdef LOG // TODO: Indentation?
        OpenLog(argv[0]); // TODO: make OpenLog a macro that is defined or not defined depending on LOG?
    #endif
    struct stack stk = {};
    StackCtor(&stk, 20);

    const int number_of_elements = 6;
    for (int i = 1; i < number_of_elements; ++i)
	{
	StackPush(&stk, i);
	}

    printf("\n hash in struct is %d\n", stk.struct_hash);
    hash_t k = SumHash(&stk, sizeof(struct stack));
    printf("SumHash is %u\n", k);

    STACK_DUMP(&stk);
    StackDtor(&stk);
    }
