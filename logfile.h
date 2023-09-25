#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

#include <stdio.h>

#include "Types.h"
#include "Key.h"

extern FILE* LOG_FILE;
static const char* EXTENSION = ".log";
static const int MAX_LEN = 100;

int OpenLog(const char* FILE_NAME);
void CloseLog();

#define STACK_DUMP(stk) StackDump(LOG_FILE, stk, __func__, __FILE__, __LINE__)

#define VERIFY(stk) if (StackOk(stk) != 0)                        \
                        {                                         \
                        STACK_DUMP(stk);                          \
                        return (int) Error::ERROR_DATA;           \
                        }


#endif // LOGFILE_H_INCLUDED
