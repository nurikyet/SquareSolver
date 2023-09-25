#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"

bool HashOkData(struct stack* stk);
bool HashOkStruct(struct stack* stk);

hash_t SumHash (void* object, hash_t len);

#endif // HASH_H_INCLUDED
