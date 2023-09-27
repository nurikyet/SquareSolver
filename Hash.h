#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"
void ChangeHash(struct stack* stk);

bool HashOkData(struct stack* stk);
bool HashOkStruct(struct stack* stk);

hash_t SumHash (void* object, size_t len);

hash_t DataHash(struct stack* stk);
hash_t StructHash(struct stack* stk);

#endif // HASH_H_INCLUDED
