#ifndef _RISKEMULIBRARY_PACKET_PACKETBASE_H_
#define _RISKEMULIBRARY_PACKET_PACKETBASE_H_

#include "stdtypes.h"

#define PACKET_HEADER_SIZE 12
#define PACKET_TYPE_NORMAL 0x8000
#define PACKET_TYPE_COMPRESSED 0x4000
#define PACKET_START_BIT 0xFF

class PacketBase
{
public:
	byte* GetBytes();
	size_t GetSize();
public:
	static const int HeaderSize = PACKET_HEADER_SIZE;

	static void DecriptHeaderPacket(byte* bytes);
	static void DecriptBodyPacket(byte* bytes);
	static void CryptPacket(byte* bytes);
	static size_t IsValidsPacket(byte* bytes);
	static size_t PacketBodySize(byte* bytes);
private:
	byte* bytes;
};

#endif //_RISKEMULIBRARY_PACKET_PACKETBASE_H_