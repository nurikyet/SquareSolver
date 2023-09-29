#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

void PrintError(FILE* fp, int result); ///This function outputs errors

int StackOk(FILE* fp, struct stack* stk); ///This function checks for errors

#endif // ERROR_H_INCLUDED
