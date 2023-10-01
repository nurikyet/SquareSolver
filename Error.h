#ifndef ERROR_H_INCLUDED
#define ERROR_H_INCLUDED

//! @brief prints all errors
//! @param [int] result sum of errors
//!
//! prints all stack related errors
void PrintError(FILE* fp, int result);

//! @brief error checking
//! @param [in] stk all structure data
//!
//! checks the data for errors and writes errors to the error variable
int StackOk(FILE* fp, struct stack* stk);

#endif // ERROR_H_INCLUDED
