#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "logfile.h"
#include "Types.h"

FILE* LOG_FILE = stderr;

int OpenLog(const char* FILE_NAME)
    {
    char* file_name = strdup(FILE_NAME);
    LOG_FILE = fopen(strncat(file_name, EXTENSION, MAX_LEN), "a");
    if (LOG_FILE == nullptr)
        {
        LOG_FILE = stderr;
        }
    atexit(CloseLog);
    }
void CloseLog()
    {
    //fputc("The work is completed", LOG_FILE);
    fprintf(LOG_FILE, "The work is completed");
    fclose(LOG_FILE);
    }
