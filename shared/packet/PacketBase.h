#ifndef _RISKEMULIBRARY_PACKET_PACKETBASE_H_
#define _RISKEMULIBRARY_PACKET_PACKETBASE_H_

#include "stdtypes.h"

class PacketBase
{
public:
	byte* GetBytes();
	size_t GetSize();
public:
	static const size_t PACKET_HEADER_SIZE = 12;
	static const uint16 PACKET_TYPE_NORMAL = 0x8000;
	static const uint16 PACKET_TYPE_COMPRESSED = 0x4000;
	static const byte PACKET_START_BIT = 0xFF;

	static void DecriptHeaderPacket(byte* bytes);
	static void DecriptBodyPacket(byte* bytes);
	static void CryptPacket(byte* bytes);
	static size_t IsValidsPacket(byte* bytes);
	static size_t PacketBodySize(byte* bytes);
private:
	byte* bytes;
};

#endif //_RISKEMULIBRARY_PACKET_PACKETBASE_H_