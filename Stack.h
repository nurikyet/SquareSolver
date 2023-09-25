#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Types.h"

#define WITH_CANARY
#define HASH
#define LOG

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)

#define VERIFY(stk) if (StackOk(stk) != 0)                        \
                        {                                         \
                        STACK_DUMP(stk);                          \
                        return (int) Error::ERROR_DATA;           \
                        }



void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line);
void PrintStack(FILE* fp, const stack *stk);
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
const int multiplier = 2;

struct stack
    {
    #ifdef WITH_CANARY
        canary_t stack_first;
    #endif

    int size;
    int capacity;
    elem* data;

    #ifdef HASH
        hash_t data_hash;
        hash_t struct_hash;
    #endif

    #ifdef WITH_CANARY
        canary_t stack_last;
    #endif
    };

enum class Error
    {
    NO_ERROR            = 0,
    ERROR_SIZE          = 1,
    ERROR_CAPACITY      = 2,
    ERROR_DATA          = 4,
    ERROR_MEMORY        = 8,
    ERROR_DATA_CANARY   = 16,
    ERROR_STRUCT_CANARY = 32,
    ERROR_STRUCT        = 64
    };



#endif // KEY_H_INCLUDED
