#include <stdio.h>

#include "Hash.h"
#include "Types.h"


bool HashOkData(const struct stack* stk)
    {
    assert(stk);

    hash_t expected_hash = stk->data_hash;

    if (expected_hash != DataHash(stk))
        {
        return false;
        }
    return true;
    }
bool HashOkStruct(const struct stack* stk)
    {
    assert(stk);

    hash_t expected_hash = stk;

    if (expected_hash != StructHash(stk))
        {
        return false;
        }
    return true;
    }
//-----------------------------------------------------------------------------

hash_t SumHash (char* object , hash_t len)
{
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
}

