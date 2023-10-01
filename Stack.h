#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Types.h"
#include "Error.h"

#define WITH_CANARY
#define HASH
#define LOG

#define STACK_CONSTRUCT(name, quantity) struct Stack name = {};          \
                                        StackCtor(&name, quantity);      \
                                        name.name = #name;

#ifdef WITH_CANARY
    #define IF_CANARY(code) code;
#else
    #define IF_CANARY(code);
#endif

#ifdef HASH
    #define IF_HASH(code) code;
#else
    #define IF_HASH(code);
#endif

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)
#define VERIFY(stk) {int error = StackOk(LOG_FILE, stk);              \
                    if (error != 0)                                   \
                        {                                             \
                        PrintError(LOG_FILE, error);                  \
                        STACK_DUMP(stk);                              \
                        return error;                                 \
                        }                                             \
                    }

void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line); ///This function outputs all information about the stack status
void PrintStack(FILE* fp, const stack *stk);                                                     ///This function outputs the entire stack to a file
void PrintInConsol(const struct stack *stk);

int StackPop(struct stack* stk, elem_t* retvalue);                                               ///This function removes an element from the array

int StackDtor(struct stack* stk);                                                                ///This function clears the buffer
int StackRealloc(stack *stk, int newcapacity);                                                   ///This function changes the stack capacity when the size reaches it
int StackCtor(struct stack* stk, size_t cpt);                                                    ///This function initializes the structure
int StackPush(struct stack* stk, const elem_t value);                                            ///This function adds an element to the stack

const canary_t canary_value = 0xDEADBEEF;
const int MULTIPLIER1 = 3;
const int MULTIPLIER2 = 2;

struct stack
    {
    const char* name;
    IF_CANARY
    (
        canary_t stack_first;
    )

    int size;
    int capacity;
    elem_t* data;

    IF_HASH
    (
        hash_t data_hash;
        hash_t struct_hash;
    )

    IF_CANARY
    (
        canary_t stack_last;
    )
    };

enum class Error
    {
    NO_ERROR            = 0,
    ERROR_SIZE          = 1 << 0,
    ERROR_CAPACITY      = 1 << 1,
    ERROR_DATA          = 1 << 2,
    ERROR_MEMORY        = 1 << 3,
    ERROR_DATA_CANARY   = 1 << 4,
    ERROR_STRUCT_CANARY = 1 << 5,
    ERROR_STRUCT        = 1 << 6
    };

#endif // KEY_H_INCLUDED
