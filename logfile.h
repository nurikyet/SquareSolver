#ifndef LOGFILE_H_INCLUDED
#define LOGFILE_H_INCLUDED

extern FILE* LOG_FILE;
static const char* EXTENSION = ".log";
static const int MAX_LEN = 100;

int OpenLog(const char* FILE_NAME);
void CloseLog();



#endif // LOGFILE_H_INCLUDED
