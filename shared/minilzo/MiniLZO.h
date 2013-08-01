#ifndef _RISKEMULIBRARY_MINILZO_MINILZO_H_
#define _RISKEMULIBRARY_MINILZO_MINILZO_H_

#include "stdtypes.h"
#include <vector>

class MiniLZO
{
public:
    static bool Compress(byte *src, size_t srcLen, byte *&dst, size_t &dstLen);
    static bool Decompress(byte *src, size_t srcLen, byte *&dst, size_t &dstLen);
private:
    static const uint32 M2_MAX_LEN = 8;
    static const uint32 M3_MAX_LEN = 33;
    static const uint32 M4_MAX_LEN = 9;
    static const byte M3_MARKER = 32;
    static const byte M4_MARKER = 16;
    static const uint32 M1_MAX_OFFSET = 0x0400;
    static const uint32 M2_MAX_OFFSET = 0x0800;
    static const uint32 M3_MAX_OFFSET = 0x4000;
    static const uint32 M4_MAX_OFFSET = 0xbfff;
    static const byte BITS = 14;
    static const uint32 D_MASK = (1 << BITS) - 1;
    static const uint32 DICT_SIZE = 65536 + 3;

    inline static uint32 D_INDEX1(byte *input)
    {
        return D_MS(D_MUL(0x21, D_X3(input, 5, 5, 6)) >> 5, 0);
    }

    inline static uint32 D_INDEX2(uint32 idx)
    {
        return (idx & (D_MASK & 0x7FF)) ^ (((D_MASK >> 1) + 1) | 0x1F);
    }

    inline static uint32 D_MS(uint32 v, byte s)
    {
        return (v & (D_MASK >> s)) << s;
    }

    inline static uint32 D_MUL(uint32 a, uint32 b)
    {
        return a * b;
    }

    inline static uint32 D_X2(byte* input, byte s1, byte s2)
    {
        return (uint32) ((((input[2] << s2) ^ input[1]) << s1) ^ input[0]);
    }

    inline static uint32 D_X3(byte* input, byte s1, byte s2, byte s3)
    {
        return (D_X2(input + 1, s2, s3) << s1) ^ input[0];
    }
};

#endif //_RISKEMULIBRARY_MINILZO_MINILZO_H_