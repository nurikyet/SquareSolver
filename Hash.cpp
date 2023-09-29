#include <stdio.h>
#include <assert.h>

#include "Hash.h"
#include "Stack.h"
#include "Types.h"

bool HashOkData(struct stack* stk)
    {
    #ifdef HASH
        hash_t expected_hash = stk->data_hash;

        return(expected_hash == DataHash(stk));
    #endif
    return false;
    }

//-----------------------------------------------------------------------------

hash_t DataHash(struct stack* stk)
    {
    assert(stk);

    elem* data       = stk->data;
    size_t data_size = stk->capacity * sizeof(elem);
    hash_t hash      = 0;

    #ifdef WITH_CANARY
        data       = (elem*)((char*) data - sizeof(canary_t));
        data_size += 2 * sizeof(canary_t);
        hash       = SumHash ((stk->data), data_size);

    #else
        data  = (elem*)((char*) data;
        hash  = SumHash ((stk->data), data_size);
    #endif

    return hash;
    }
//-----------------------------------------------------------------------------

bool HashOkStruct(struct stack* stk)
    {
    assert(stk);
    #ifdef HASH
        hash_t expected_hash = stk->struct_hash;
        return(expected_hash == StructHash(stk));
    #endif
    return false;
    }

//-----------------------------------------------------------------------------

hash_t StructHash(struct stack* stk)
    {
    assert(stk);
    hash_t new_hash = 0;

    #ifdef HASH
        hash_t third    = stk->struct_hash;
        stk->struct_hash = 0;

        new_hash = SumHash(stk, sizeof(struct stack));
        stk->struct_hash = third;
    #endif

    return new_hash;
    }

//-----------------------------------------------------------------------------

void ChangeHash(struct stack* stk)
    {
    #ifdef HASH
        stk->struct_hash = StructHash(stk);
        stk->data_hash   = DataHash(stk);
    #endif
    }

//-----------------------------------------------------------------------------

hash_t SumHash (void* object , size_t len)
    {
    #ifdef HASH
    assert(object);
    const hash_t m = 0x5bd1e995;
    const hash_t seed = 0;
    const int r = 24;

    hash_t h = seed ^ len;

    const unsigned char * data = (const unsigned char *)object;
    hash_t k = 0;

   while (len >= 4)
        {
        k  = data[0];
        k |= data[1] << 8;
        k |= data[2] << 16;
        k |= data[3] << 24;

        k *= m;
        k ^= k >> r;
        k *= m;

        h *= m;
        h ^= k;

        data += 4;
        len -= 4;
        }

    switch (len)
        {
        case 3:
        h ^= data[2] << 16;
        case 2:
        h ^= data[1] << 8;
        case 1:
        h ^= data[0];
        h *= m;
        };

    h ^= h >> 13;
    h *= m;
    h ^= h >> 15;

    return h;
    #endif
    }

