// TODO: Rename this file it's named differently from all other files!

#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

#include <stdio.h>

#include "Types.h" // TODO: remove this includes! They are not used in this header!
#include "Stack.h"

extern FILE* LOG_FILE;                  // no need in this extern, just put LOG_FILE here instead of logfile.cpp
static const char* EXTENSION = ".log";  // WARNING: why not in logfile.cpp? you don't need this anywhere else
static const int MAX_LEN = 1000;

int OpenLog(const char* FILE_NAME);
void CloseLog();
//rgrowjgwrpgwrpgowrgw

#endif // LOGFILE_H_INCLUDED
