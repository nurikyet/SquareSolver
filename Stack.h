#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Types.h"
#include "Error.h"

#define WITH_CANARY
#define HASH
#define LOG

// hey, what about STACK_CONSTRUCT define? :(
//#define STACK_CONSTRUCT(stack_variable_name) ...

// no need to add ; after code btw
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
                                                                                        // hey, let's return StackOk result from VERIFY, not ERROR_STRUCT each time!
#define VERIFY(stk) if (StackOk(LOG_FILE, stk) != 0)                  \
                            {                                         \
                            STACK_DUMP(stk);                          \
                            return (int) Error::ERROR_STRUCT;         \
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
const int MULTIPLIER = 2;
const int DOUBLE = 2;       // const int DOUBLE = 2     wtf

struct stack
    {
                                    // #ifdef from the very beginning of the string; this can also be replaced by IF_CANARY() :)
    #ifdef WITH_CANARY
        canary_t stack_first;
    #endif

    int size;
    int capacity;
    elem_t* data;

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
    ERROR_SIZE          = 1,        // why not 1 << 0
    ERROR_CAPACITY      = 2,        //         1 << 1
    ERROR_DATA          = 4,        // etc.? looks more understandable
    ERROR_MEMORY        = 8,
    ERROR_DATA_CANARY   = 16,
    ERROR_STRUCT_CANARY = 32,
    ERROR_STRUCT        = 64
    };

#endif // KEY_H_INCLUDED
