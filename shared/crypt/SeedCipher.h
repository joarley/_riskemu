#ifndef _RISKEMULIBRARY_CRYPT_SEEDCIPHER_H_
#define _RISKEMULIBRARY_CRYPT_SEEDCIPHER_H_
#include "stdtypes.h"
#include "Utils.h"

class SeedCipher {
public:
    typedef uint32 SeedParam[32];
    static void Decrypt(byte* data, SeedParam keyparam);
    static void Encrypt(byte* data, SeedParam keyparam);
    static void EncRoundKey(SeedParam keyparam, byte* key);
protected:
    SeedCipher(){}
    static inline uint32 XorSSBox(uint32 value);
    static inline void Round(uint32& value0, uint32& value1, uint32& value2, uint32& value3, SeedParam keyparam, int posparam);
    static inline void RoundKey0(SeedParam keyparam, int posparam, uint32& value0, uint32& value1, uint32 value2, uint32 value3, uint32 kc);
    static inline void RoundKey1(SeedParam keyparam, int posparam, uint32 value0, uint32 value1, uint32& value2, uint32& value3, uint32 kc);		
};


#endif //_RISKEMULIBRARY_CRYPT_SEEDCIPHER_H_