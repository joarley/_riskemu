#ifndef _RISKEMULIBRARY_PACKET_PACKETBASE_H_
#define _RISKEMULIBRARY_PACKET_PACKETBASE_H_

#include "stdtypes.h"

class PacketBase
{
public:
	byte* GetBytes();
	size_t GetSize();
};

#endif //_RISKEMULIBRARY_PACKET_PACKETBASE_H_