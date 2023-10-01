#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

#include <stdio.h>

#include "Types.h"
#include "Stack.h"

extern FILE* LOG_FILE;
static const int MAX_LEN = 256;

//! @brief file creation
//!
//! a file is created and opened
int OpenLog(const char* FILE_NAME);

//! @brief closing a file
//!
//! closes the file
void CloseLog();

#endif // LOGFILE_H_INCLUDED
