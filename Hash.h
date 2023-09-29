#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"
// TODO:                                       This function be be be
void ChangeHash(struct stack* stk);         ///This function updates the hash value

// TODO: This line of code defines a function with name HashOkData,
//       which returns bool and accepts struct stack* stk which
//       is a pointer to a struct called "stack" with name "stk".
// 
//       This function is used for Hash Ok Data
// TODO: Rename, use verbs, please!
bool HashOkData(struct stack* stk);         ///This function checks the expected and received hash
// TODO: Rename, use verbs, please!
bool HashOkStruct(struct stack* stk);       ///This function checks the expected and received hash


// TODO: Include header which defines size_t :)
// TODO: Rename, use verbs, please!
// TODO: This is THE hash but you make it look like it is A hash.
hash_t SumHash (void* object, size_t len);  ///This function considers the hash

// TODO: Rename, use verbs, please!
hash_t DataHash(struct stack* stk);         ///This function returns a new hash value
// TODO: Rename, use verbs, please!
hash_t StructHash(struct stack* stk);       ///This function returns a new hash value

#endif // HASH_H_INCLUDED
