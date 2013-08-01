#ifndef _RISKEMULIBRARY_PACKET_PACKETBASE_H_
#define _RISKEMULIBRARY_PACKET_PACKETBASE_H_

#include "stdtypes.h"
#include "Buffer.h"
#include "crypt/CryptEngine.h"
#include "minilzo/MiniLZO.h"

class PacketBase
{
public:
	inline PacketBase(uint8 command, uint32 status, uint16 paketLength);
    inline PacketBase(uint8 command, uint32 status);
    inline PacketBase(Buffer_ptr buffer);	
	inline Buffer_ptr GetBuffer(bool compress);
protected:
	Buffer_ptr buffer;
	
public:
	static const size_t PACKET_HEADER_SIZE = 12;
	static const uint16 PACKET_TYPE_NORMAL = 0x8000;
	static const uint16 PACKET_TYPE_COMPRESSED = 0x4000;
	static const byte PACKET_START_BIT = 0xFF;

	inline static void DecriptHeaderPacket(Buffer_ptr bytes);
	inline static void DecriptBodyPacket(Buffer_ptr bytes);
	inline static void CryptPacket(Buffer_ptr bytes);
	inline static size_t IsValidPacket(Buffer_ptr bytes);
	inline static size_t PacketBodySize(Buffer_ptr bytes);
};

PacketBase::PacketBase(uint8 command, uint32 status, uint16 paketLength): buffer(new Buffer(paketLength))
{
	*buffer << PACKET_START_BIT << command << (uint16)0
		<< CryptEngine::Cryptkey() << status;
	buffer->SetWriteOffset(PACKET_HEADER_SIZE);
	buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(uint8 command, uint32 status): buffer(new Buffer())
{
	*buffer << PACKET_START_BIT << command << (uint16)0
		<< CryptEngine::Cryptkey() << status;
	buffer->SetWriteOffset(PACKET_HEADER_SIZE);
	buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(Buffer_ptr buffer): buffer(buffer)
{
	uint16 packetSize;
	*buffer >> Buffer::FromPosition(packetSize, 2);
	if(packetSize & PACKET_TYPE_COMPRESSED)
	{
		byte *bytes;
		size_t size;
		MiniLZO::Decompress(this->buffer->Data() + PACKET_HEADER_SIZE, 
			this->buffer->Length() - PACKET_HEADER_SIZE, bytes, size);
		this->buffer->SetLength(PACKET_HEADER_SIZE);
		this->buffer->SetWriteOffset(PACKET_HEADER_SIZE);
		*this->buffer << Buffer::Bytes(bytes, size);
		delete bytes;
	}

	buffer->SetWriteOffset(PACKET_HEADER_SIZE);
	buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

Buffer_ptr PacketBase::GetBuffer(bool compress)
{
	uint16 size = 0;
	if(compress)
	{
		byte *bytes;
		size_t size;
		MiniLZO::Compress(this->buffer->Data() + PACKET_HEADER_SIZE, 
			this->buffer->Length() - PACKET_HEADER_SIZE, bytes, size);
		this->buffer->SetLength(PACKET_HEADER_SIZE);
		this->buffer->SetWriteOffset(PACKET_HEADER_SIZE);
		*this->buffer << Buffer::Bytes(bytes, size);
		delete bytes;
		size = PACKET_TYPE_COMPRESSED;
	}
	else
	{
		size = PACKET_TYPE_NORMAL;
	}
	
	size |= buffer->Length();
	
	*buffer << Buffer::ToPosition(size, 2);

	return buffer;
}

void PacketBase::DecriptHeaderPacket(Buffer_ptr buff)
{
	if(buff->Length() >= PACKET_HEADER_SIZE)
	{
		CryptEngine::XorDecryptPacketHeader(buff->Data());
	}
}

void PacketBase::DecriptBodyPacket(Buffer_ptr buff)
{
	if(buff->Length() > PACKET_HEADER_SIZE)
	{
		CryptEngine::Cryptkey key;
		*buff >> Buffer::FromPosition(key, 4);
		CryptEngine::XorDecryptPacketBody(buff->Data(), buff->Length(), key);
	}
}

void PacketBase::CryptPacket(Buffer_ptr buff)
{
	if(buff->Length() > PACKET_HEADER_SIZE)
	{
		CryptEngine::Cryptkey key;
		*buff >> Buffer::FromPosition(key, 4);
		CryptEngine::XorCrypt(buff, key);
	}
}

size_t PacketBase::IsValidPacket(Buffer_ptr buff)
{
	byte startbit;
	*buff >> Buffer::FromPosition(startbit, 0);
	return startbit == PacketBase::PACKET_START_BIT;
}

size_t PacketBase::PacketBodySize(Buffer_ptr buff)
{
	uint16 size;
	*buff >> Buffer::FromPosition(size, 2);
	return (size & 
		~(PacketBase::PACKET_TYPE_COMPRESSED | PacketBase::PACKET_TYPE_NORMAL)) - 
		PacketBase::PACKET_HEADER_SIZE;
}

#endif //_RISKEMULIBRARY_PACKET_PACKETBASE_H_