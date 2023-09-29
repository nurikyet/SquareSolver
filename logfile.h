// TODO: Rename this file it's named differently from all other files!

#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

#include <stdio.h>

#include "Types.h" // TODO: remove this includes! They are not used in this header!
#include "Stack.h"

extern FILE* LOG_FILE;
static const char* EXTENSION = ".log";
static const int MAX_LEN = 1000;

int OpenLog(const char* FILE_NAME);
void CloseLog();

#endif // LOGFILE_H_INCLUDED
