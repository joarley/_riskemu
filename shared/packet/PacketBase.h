#ifndef _RISKEMULIBRARY_PACKET_PACKETBASE_H_
#define _RISKEMULIBRARY_PACKET_PACKETBASE_H_

#include "stdtypes.h"
#include "Buffer.h"
#include "crypt/CryptEngine.h"
#include "minilzo/MiniLZO.h"

class PacketBase
{
public:
	inline PacketBase(uint8 command, uint16 paketLength);
    inline PacketBase(uint8 command);
    inline PacketBase(Buffer_ptr buffer);
	inline uint32 GetStatus();
	inline void  SetStatus(uint32 status);
	inline virtual Buffer_ptr GetBuffer() = 0;
	inline bool HasErrorParse() const;
protected:
	inline virtual Buffer_ptr ProcessBuffer(bool compress);
	Buffer_ptr buffer;
	bool hasErrorParse;
public:
	static const size_t PACKET_HEADER_SIZE = 12;
	static const uint16 PACKET_TYPE_NORMAL = 0x8000;
	static const uint16 PACKET_TYPE_COMPRESSED = 0x4000;
	static const byte PACKET_START_BIT = 0xFF;

	inline static void DecriptHeaderPacket(Buffer_ptr buff);
	inline static void DecriptBodyPacket(Buffer_ptr buff);
	inline static void CryptPacket(Buffer_ptr buff);
	inline static size_t IsValidPacket(Buffer_ptr buff);
	inline static size_t PacketBodySize(Buffer_ptr buff);
	inline static uint8 PacketCmd(Buffer_ptr buff);
};

PacketBase::PacketBase(uint8 command, uint16 paketLength): 
	buffer(new Buffer(paketLength)), hasErrorParse(false)
{
	*this->buffer << PACKET_START_BIT << command << (uint16)0
            << CryptEngine::Cryptkey() << (uint32)0;
    this->buffer->SetWriteOffset(PACKET_HEADER_SIZE);
    this->buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(uint8 command):
	buffer(new Buffer()), hasErrorParse(false)
{
	*this->buffer << PACKET_START_BIT << command << (uint16)0
            << CryptEngine::Cryptkey() << (uint32)0;
    this->buffer->SetWriteOffset(PACKET_HEADER_SIZE);
    this->buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

PacketBase::PacketBase(Buffer_ptr buffer): 
	buffer(buffer), hasErrorParse(false)
{
	uint16 packetSize;
    *this->buffer >> Buffer::FromPosition(packetSize, 2);
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

    this->buffer->SetWriteOffset(PACKET_HEADER_SIZE);
    this->buffer->SetReaderOffset(PACKET_HEADER_SIZE);
}

uint32 PacketBase::GetStatus()
{
	uint32 ret;
	*this->buffer >> Buffer::FromPosition(ret, 8);
	return ret;
}

void  PacketBase::SetStatus(uint32 status)
{
	*this->buffer << Buffer::ToPosition(status, 8);
}

Buffer_ptr PacketBase::ProcessBuffer(bool compress)
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
	
    size |= this->buffer->Length();
	
    *this->buffer << Buffer::ToPosition(size, 2);

    return this->buffer;
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

uint8 PacketBase::PacketCmd(Buffer_ptr buff)
{
	uint8 ret;
	*buff >> Buffer::FromPosition(ret, 1);
	return ret;
}

bool PacketBase::HasErrorParse() const
{
	return this->hasErrorParse;
}

#define BLOCK(b1, b2) b1; b2;
#define INVOKE(f, ...) f(__VA_ARGS__);
#define CORRECT_PACKET_LEN(c, b) (c::PktLen == 0 || c::PktLen == (b)->Length())
#define PARSE_PACKET(_BUFFER_) switch (PacketBase::PacketCmd(_BUFFER_)) {
#define PARSE_CASE(_BUFFER_, _PKT_, _VARIABLE_, _NAMECLIENT_, _CLIENT_, _FINALIZE_BLOCK_) \
	case _PKT_::PktCmd: { \
		if(!CORRECT_PACKET_LEN(_PKT_, packet)) \
		{ \
			LOG->ShowError("Desconnected "_NAMECLIENT_"(%s) error Protocol, incorrect "#_PKT_" size, expected %d bytes received %d bytes", \
				(_CLIENT_)->GetRemoteAddress().c_str(), PacketBase::PacketCmd(packet), _PKT_::PktLen, (_BUFFER_)->Length()); \
			_FINALIZE_BLOCK_ \
		} \
		else \
		{ \
			_PKT_ _VARIABLE_(_BUFFER_); \
			if(_VARIABLE_.HasErrorParse()) \
			{ \
				LOG->ShowError("Desconnected "_NAMECLIENT_"(%s) error Protocol, "#_PKT_" has parse error", \
				(_CLIENT_)->GetRemoteAddress().c_str(), PacketBase::PacketCmd(packet)); \
				_FINALIZE_BLOCK_ \
				break; \
			} \
			else \
			{ 
#define END_PARSE_CASE() }}}break;
#define END_PARSE_PACKET(_PKT_, _NAMECLIENT_, _CLIENT_, _FINALIZE_BLOCK_) \
	default: \
		LOG->ShowError("Desconnected "_NAMECLIENT_"(%s) error Protocol(Packet %#.2X not expected)", \
			(_CLIENT_)->GetRemoteAddress().c_str(), PacketBase::PacketCmd(_PKT_)); \
		_FINALIZE_BLOCK_ \
	} \

#endif //_RISKEMULIBRARY_PACKET_PACKETBASE_H_
