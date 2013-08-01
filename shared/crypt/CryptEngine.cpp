#include "CryptEngine.h"

#include "SeedCipher.h"
#include "crc/CRC32.h"
#include <ctime>
#include "packet\PacketBase.h"
#include "Utils.h"

namespace
{
static const uint32 s_ClientKeyId = 0x56;
static const uint32 s_ServerKeyId = 0x3D;
#include "CryptEngine.tab"
byte Key[] = {0xC6, 0xD0, 0xC5, 0xB6, 0xB0, 0xD4, 0xC0, 0xD3, 0xB0, 0xA1, 0xB5, 0xE5, 0x00};
byte ClientSeedKeyIni[] = {0xA3, 0xFE, 0x91, 0x8F, 0xEB, 0xE1, 0xDC, 0x5C, 0x41, 0xE0, 0x20, 0x40, 0xAE, 0xA6, 0xAB, 0xA7};
byte ServerSeedKeyIni[] = {0x1B, 0xC2, 0x0A, 0xCA, 0x43, 0xDC, 0x7D, 0x2F, 0xB3, 0xA9, 0xD0, 0x36, 0x4D, 0x98, 0x6A, 0xEE};
}

byte CryptEngine::GGClientSeedKey[16];
byte CryptEngine::GGServerSeedKey[16];
byte* CryptEngine::GGKey = NULL;
SeedCipher::SeedParam CryptEngine::GGServerSeedParam;
SeedCipher::SeedParam CryptEngine::GGClientSeedParam;    

void CryptEngine::SetGGCryptParams(byte* ClientSeedKey, byte* ServerSeedKey, byte* Key) 
{
    memcpy(GGClientSeedKey, ClientSeedKey, sizeof(GGClientSeedKey));
    memcpy(GGServerSeedKey, ServerSeedKey, sizeof(GGServerSeedKey));
    if(GGKey != NULL) 
	{
		delete[] GGKey;
	}
	GGKey = new byte[strlen((char*)Key) + 1];
    strcpy((char*)GGKey, (char*)Key);

	for (int i = 0; GGKey[i]; i++) 
	{
        GGClientSeedKey[i % 16] ^= GGKey[i];
        GGServerSeedKey[i % 16] ^= GGKey[i];
    }

    SeedCipher::EncRoundKey(GGClientSeedParam, GGClientSeedKey);
    SeedCipher::EncRoundKey(GGServerSeedParam, GGServerSeedKey);
}

void CryptEngine::XorCrypt(Buffer_ptr buffer, Cryptkey& key) 
{
    XorCryptPacketBody(buffer->Data(), buffer->Length(), key);
    XorCryptPacketHeader(buffer->Data());
}

void CryptEngine::XorDecrypt(Buffer_ptr buffer) 
{
    XorDecryptPacketHeader(buffer->Data());
    Cryptkey key;
	*buffer >> Buffer::FromPosition(key, 4);
    XorDecryptPacketBody(buffer->Data(), buffer->Length(), key);
}

void CryptEngine::XorCryptPacketHeader(byte* bytes) 
{
	for (int i = 1; i < PacketBase::PACKET_HEADER_SIZE; i++) 
	{
        bytes[i] = bytes[i] ^ BitFields[i - 1];
        bytes[i] = (bytes[i] << 1) | (bytes[i] >> 7);
    }
}

void CryptEngine::XorCryptPacketBody(byte* bytes, size_t size, Cryptkey& key) 
{
    size_t pos1 = (key.CodePage * 10 + key.Key1) * 40;
    size_t pos2 = (key.CodePage * 10 + key.Key2) * 40;
	for (size_t i = PacketBase::PACKET_HEADER_SIZE, j = 0; i < size; i++, j++) 
	{
        bytes[i] = bytes[i] ^ BitFields[pos1 + j % 40] ^ BitFields[pos2 + j % 40];
    }
}

void CryptEngine::XorDecryptPacketHeader(byte* bytes) 
{
    for (int i = 1; i < PacketBase::PACKET_HEADER_SIZE; i++) 
	{
        bytes[i] = (char) (bytes[i] >> 1) | (char) (bytes[i] << 7);
        bytes[i] = bytes[i] ^ BitFields[i - 1];
    }
}

void CryptEngine::XorDecryptPacketBody(byte* bytes, size_t size, Cryptkey& key) 
{
    size_t pos1 = (key.CodePage * 10 + key.Key1) * 40;
    size_t pos2 = (key.CodePage * 10 + key.Key2) * 40;
    for (size_t i = PacketBase::PACKET_HEADER_SIZE, j = 0; i < size; i++, j++) 
	{
        bytes[i] = bytes[i] ^ BitFields[pos1 + j % 40] ^ BitFields[pos2 + j % 40];
    }
}

void CryptEngine::GGCrypt(Buffer_ptr buffer) 
{
	if(GGKey == NULL) SetGGCryptParams(ClientSeedKeyIni, ServerSeedKeyIni, Key); 

    byte* data = buffer->Data();
	uint32 dataLen = buffer->Length();
    
	if((buffer->Length() % 16 + buffer->Length()) > buffer->MaxLength()) {
        buffer->SetMaxLength(buffer->Length() % 16 + buffer->MaxLength());
    } 
    while (buffer->Length() % 16 != 0) {
        *buffer << (uint8)(rand() % 256);
	}
    if((buffer->Length() + 16) > buffer->MaxLength()) {
        buffer->SetMaxLength(buffer->Length() + 16);
    }
	buffer->SetWriteOffset(buffer->Length());

	*buffer << EndianChange((uint32)0);
	*buffer << EndianChange(CRC32::Calculate(data, buffer->Length()))
		<< EndianChange(dataLen)
		<< EndianChange(s_ServerKeyId);
    
	for (size_t i = 0; i < buffer->Length(); i += 16) {
        SeedCipher::Encrypt(data + i, GGServerSeedParam);        
    }
}

CryptEngine::GGError CryptEngine::GGDecrypt(Buffer_ptr buffer) 
{
	if(GGKey == NULL) SetGGCryptParams(ClientSeedKeyIni, ServerSeedKeyIni, Key);

    byte* data = buffer->Data();
    size_t dataLen = buffer->Length();
    
    for (size_t i = 0; i < dataLen; i += 16) 
	{
        SeedCipher::Decrypt(data + i, GGClientSeedParam);
    }
    
	uint32 clientKey;
	uint32 GGDataSize;
	uint32 GGDataCRC;
	uint32 GGUseSeq;

	*buffer >> Buffer::FromPosition(clientKey, buffer->Length() - 4)
		>> Buffer::FromPosition(GGDataSize, buffer->Length() - 8)
		>> Buffer::FromPosition(GGDataCRC, buffer->Length() - 12)
		>> Buffer::FromPosition(GGUseSeq, buffer->Length() - 16);

	clientKey = EndianChange(clientKey);
	GGDataSize = EndianChange(GGDataSize);
	GGDataCRC = EndianChange(GGDataCRC);
	GGUseSeq = EndianChange(GGUseSeq);

	if(clientKey != s_ClientKeyId) 
	{
        return GGERRO_ClientKey;
    }
    
    if(CRC32::Calculate(data, dataLen - 12) != GGDataCRC) 
	{
        return GGERRO_Checksum;
    }

    buffer->SetLength(GGDataSize);
    buffer->SetMaxLength(GGDataSize);
    
	return GGERRO_Sucess;
}