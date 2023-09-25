#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Types.h"


#ifndef WITH_CANARY
#define WITH_CANARY 1

#endif

#ifndef HASH
#define HASH 1

#endif

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)

#define VERIFY(stk) if (StackOk(stk) != 0)                        \
                        {                                         \
                        STACK_DUMP(stk);                          \
                        return (int) Error::ERROR_DATA;           \
                        }



void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line);
void PrintStackValue(FILE* fp, const elem value);
void PrintStack(FILE* fp, const stack *stk, void (*PrintStackValue)(FILE* fp, const elem value));
void ChangeHash(struct stack* stk);

elem StackPop(struct stack* stk, elem* retvalue);

int StackDtor(struct stack* stk);
int StackRealloc(stack *stk, int newcapacity);
int StackCtor(struct stack* stk, size_t cpt);
int StackOk(struct stack* stk);
int StackPush(struct stack* stk, const elem value);

hash_t DataHash(struct stack* stk);
hash_t StructHash(struct stack* stk);


static const canary_t canary_value = 0xDEADBEEF;

struct stack
    {
    #if WITH_CANARY
        canary_t stack_first;
    #endif

    int size;
    int capacity;
    elem* data;

    #if HASH
        hash_t data_hash;
        hash_t struct_hash;
    #endif

    #if WITH_CANARY
        canary_t stack_last;
    #endif
    };

enum class Error
{
    NO_ERROR            = 0,
    ERROR_SIZE          = 1,
    ERROR_CAPACITY      = 2,
    ERROR_DATA          = 3,
    ERROR_MEMORY        = 4,
    ERROR_DATA_CANARY   = 5,
    ERROR_STRUCT_CANARY = 6


};

#endif // KEY_H_INCLUDED
