#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"
void ChangeHash(struct stack* stk); ///This function updates the hash value

bool HashOkData(struct stack* stk); ///This function checks the expected and received hash
bool HashOkStruct(struct stack* stk); ///This function checks the expected and received hash

hash_t SumHash (void* object, size_t len); ///This function considers the hash

hash_t DataHash(struct stack* stk); ///This function returns a new hash value
hash_t StructHash(struct stack* stk); ///This function returns a new hash value

#endif // HASH_H_INCLUDED
