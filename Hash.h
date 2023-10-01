#ifndef HASH_H_INCLUDED
#define HASH_H_INCLUDED

#include "Types.h"

void ChangeHash(struct stack* stk);

//! @brief compares hash of data
//! @param [in] stk all structure data
//!
//! compares the hash after changes
bool HashOkData(struct stack* stk);

//! @brief compares hash of struct
//! @param [in] stk all structure data
//!
//! compares the hash after changes
bool HashOkStruct(struct stack* stk);

//! @brief calculates hash
//! @param [in] stk all structure data
//!
//! recalculates the hash after changes
hash_t SumHash (void* object, size_t len);

//! @brief expected hash of data
//! @param [in] stk all structure data
//!
//! returns the expected hash of data
hash_t DataHash(struct stack* stk);

//! @brief expected hash of struct
//! @param [in] stk all structure data
//!
//! returns the expected hash of struct
hash_t StructHash(struct stack* stk);

#endif // HASH_H_INCLUDED
