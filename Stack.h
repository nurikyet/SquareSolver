#ifndef KEY_H_INCLUDED
#define KEY_H_INCLUDED

#include "Types.h"
#include "Error.h"

#define WITH_CANARY
#define HASH
#define LOG

// TODO: You know what you should know. Do what you need to do!
#ifdef WITH_CANARY
    #define IF_CANARY(code1, code2) code1
#else
    #define IF_CANARY(code1, code2) code2
#endif

// TODO: Same thing
#ifdef HASH
    #define IF_HASH(...) __VA_ARGS__
#else
    #define IF_HASH(...)
#endif

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)


// TODO: Undef your macros... If you use it only in .cpp then
//       you should define it only in .cpp!
#define VERIFY(stk) if (StackOk(LOG_FILE, stk) != 0)                  \
                            {                                         \
                            STACK_DUMP(stk);                          \
                            return (int) Error::ERROR_STRUCT;         \
                            }


// TODO: This function be be be
void StackDump(FILE* fp, struct stack* stk, const char* func, const char* file, const int line); ///This function ~~outputs~~ prints? write? TODO: < all information about the stack status
// TODO:             ^~ W#Y                             ^~~~ You can write that this function
//                                                           is only ever indented to be used
//                                                           from a corresponding macro
//                                                           
//                                                           You can even visually mark it 
//                                                           private with underscore in the end,
//                                                           like so: StackDump_(...)

// NOTE: You also can create a struct which will hold location in codebase (i.e. file, line, func)



// TODO: For God's sake, decide what naming convention you are gonna use!
void PrintStack(FILE* fp, const stack *stk);                                                     ///This function outputs the entire stack to a file
void PrintInConsol(const struct stack *stk);
// TODO:  ^~~~~~~~ Print what? Also, Consol_e_, not Consol

int StackPop(struct stack* stk, elem* retvalue);                                                 ///This function removes an element from the array
// TODO:                              ^~~~~~~~ stop it. rename.

int StackDtor(struct stack* stk);                                                                ///This function clears the buffer
int StackRealloc(stack *stk, int newcapacity);                                                   ///This function changes the stack capacity when the size reaches it
int StackCtor(struct stack* stk, size_t cpt);                                                    ///This function initializes the structure
// TODO;                                ^~~ central processing tower. What are you trying to save? A few keystrokes! 
//                                          TRY SAVING MY SANITY INSTEAD!

int StackPush(struct stack* stk, const elem value);                                              ///This function adds an element to the stack
// TODO:                                  ^ Nu tipo soydet, no blin. Nuriya -- the keystroke saver.

const canary_t canary_value = 0xDEADBEEF; // TODO: Nuriya -- the consistent one
const int MULTIPLIER = 2;                 // TODO: Nuriya -- the consistent one
const int DOUBLE = 2;

struct stack
    {
    #ifdef WITH_CANARY // TODO: It's usually a good style to align #ifdefs/... to the beginning of the line,
        canary_t stack_first;  // for example in this case it makes all fields align on the same indent level...
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
    NO_ERROR            = 0, // NOTE: A cool thing you can do:
    ERROR_SIZE          = 1,   // 1 << 0 
    ERROR_CAPACITY      = 2,   // 1 << 1
    ERROR_DATA          = 4,   // 1 << 2
    ERROR_MEMORY        = 8,   // 1 << 3
    ERROR_DATA_CANARY   = 16,  // 1 << 4
    ERROR_STRUCT_CANARY = 32,  // 1 << 5
    ERROR_STRUCT        = 64   // 1 << 6
    };

#endif // KEY_H_INCLUDED
