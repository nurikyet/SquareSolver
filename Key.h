#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#define VERIFY(stk) if (StackOk( (struct stack*) stk) != 0) {                      \
                        printf("error code %d\n", StackOk( (struct stack*) stk));  \
                        StackDump(LOG_FILE, stk);                                            \
                    }

#include "types.h"


void StackDump(FILE* LOG_FILE, const struct stack* stk);
void PrintStackValue(const elem value);
void PrintStack(const stack *stk, void (*PrintStackValue)(const elem));
int StackPush(struct stack* stk, const elem value);

elem StackPop(struct stack* stk, elem retvalue);

int StackDtor(struct stack* stk);
int StackRealloc(const stack *stk, int newcapacity);
int StackCtor(struct stack* stk);
int StackOk(const struct stack* stk);

struct stack
    {
    int size;
    int capacity;
    elem* data;
    };

enum class Error
{
    NO_ERROR       = 0,
    ERROR_SIZE     = 1,
    ERROR_CAPACITY = 2,
    ERROR_DATA     = 3,
    ERROR_MEMORY   = 4

};

#endif // KEY_H_INCLUDED
