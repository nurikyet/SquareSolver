#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "logfile.h"
#include "Types.h"
#include "Stack.h"

FILE* LOG_FILE = stderr;

int OpenLog(const char* FILE_NAME)
    {
    char* file_name = strdup(FILE_NAME);
    LOG_FILE = fopen(strncat(file_name, EXTENSION, MAX_LEN), "a");
    if (LOG_FILE == nullptr)
        {
        LOG_FILE = stderr;
        }
    time_t now = 0;
    time(&now);
    fprintf(LOG_FILE, "------------------------------START AT %s--------------------------------\n", ctime(&now));
    #ifdef WITH_CANARY
        fprintf(LOG_FILE, "Work with CANARY\n");
    #endif
    #ifdef HASH
        fprintf(LOG_FILE, "Work with HASH\n");
    #endif
    atexit(CloseLog);
    return 0;
    }
void CloseLog()
    {
    fprintf(LOG_FILE, "------------------------The work is completed----------------------------\n");
    fclose(LOG_FILE);
    }
