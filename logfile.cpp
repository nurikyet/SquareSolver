#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "logfile.h"
#include "Types.h"
#include "Stack.h"

FILE* LOG_FILE = stderr;


int OpenLog(const char* FILE_NAME) // TODO: Why capitalize?     
    {
    char* file_name = strdup(FILE_NAME); // TODO: you have a memory leak!                                               Kirill: strdup result needs to be freed
    LOG_FILE = fopen(strncat(file_name, EXTENSION, MAX_LEN), "a"); // TODO: Do you see SEGFAULT? But it's there!        Kirill: strdup result does not have any space
                                                                                                                    //  to write something in its ending;
                                                                                                                    //  you just write EXTENSION out of bounds, generally
    if (LOG_FILE == nullptr)    // Kirill: but LOG_FILE won't be nullptr because of 10 line...
        {
	// TODO: Read about errno, perror, strerror and friends
	//       learn how to handle errors!

        LOG_FILE = stderr;
        }
    time_t now = 0;
    time(&now);
    fprintf(LOG_FILE, "------------------------------START AT %s--------------------------------\n", ctime(&now));

    // TODO: What does log has to do with CANARY? Whyyyy?
    IF_CANARY(fprintf(LOG_FILE, "Work with CANARY\n");)

    IF_HASH(fprintf(LOG_FILE, "Work with HASH\n"););

    atexit(CloseLog); // TODO: You are brave
    return 0;
    }

//-----------------------------------------------------------------------------

void CloseLog()
    {
    fprintf(LOG_FILE, "------------------------The work is completed----------------------------\n");
    //printf("file %s, line %d, descr is %d\n", __FILE__, __LINE__, fileno(LOG_FILE));
    fclose(LOG_FILE);
    }

