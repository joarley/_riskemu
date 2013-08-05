#ifndef _RISKEMULIBRARY_CRYPT_CRIPTENGINE_H_
#define	_RISKEMULIBRARY_CRYPT_CRIPTENGINE_H_

#include <cstdlib>

#include "stdtypes.h"
#include "Buffer.h"
#include "Packable.h"
#include "SeedCipher.h"
#include <ctime>

class CryptEngine
{
public:
	struct Cryptkey : Packable {
	    uint8 Key1;
	    uint8 Key2;
	    uint16 CodePage;
	
	    Cryptkey() {
			srand((uint32)time(0));
			Key1 = rand() % 10;
			while((Key2 = rand() % 10) == Key1);
			CodePage = 1;
	    }
	
	    virtual void Pack(Buffer_ptr buffer) const
		{
	        *buffer << Key1 << Key2 << CodePage;
	    }
	
	    virtual void Unpack(Buffer_ptr buffer)
		{
	        *buffer >> Key1 >> Key2 >> CodePage;
	    }
	};
    enum GGError {
        GGERRO_Sucess,
        GGERRO_ClientKey,
        GGERRO_Checksum = 4
    };    

    static void SetGGCryptParams(byte* GGClientSeedKey, byte* GGServerSeedKey, byte* GGKey);

    static void XorCrypt(Buffer_ptr buffer, Cryptkey& key);
    static void XorDecrypt(Buffer_ptr buffer);
    
    static void GGCrypt(Buffer_ptr buffer);
    static GGError GGDecrypt(Buffer_ptr buffer);

    static void XorCryptPacketHeader(byte* bytes);
    static void XorCryptPacketBody(byte* bytes, size_t size, Cryptkey& key);
    static void XorDecryptPacketHeader(byte* bytes);
    static void XorDecryptPacketBody(byte* bytes, size_t size, Cryptkey& key);
private:
    static byte GGClientSeedKey[16];
    static byte GGServerSeedKey[16];
    static byte* GGKey;
    static SeedCipher::SeedParam GGServerSeedParam;
    static SeedCipher::SeedParam GGClientSeedParam;    
};

#endif	//_RISKEMULIBRARY_CRYPT_CRIPTENGINE_H_

