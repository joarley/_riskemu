#ifndef _RISKEMULIBRARY_PACKET_PKTPING_H_
#define	_RISKEMULIBRARY_PACKET_PKTPING_H_

#include "PacketBase.h"

/**
Packet struct
uint32 PingValue;
**/
class PktPing: public PacketBase
{
public:
    static const uint8 PktCmd = 0x59;
    static const uint16 PktLen = 16;

	inline PktPing(Buffer_ptr buff, uint32 *pingValue = NULL);
	inline PktPing(uint32 pingValue);
    inline virtual Buffer_ptr GetBuffer();
};

PktPing::PktPing(Buffer_ptr buff, uint32 *pingValue): 
	PacketBase(buff)
{
	if(pingValue != NULL)
	{
		*this->buffer >> *pingValue;
	}
}

PktPing::PktPing(uint32 pingValue):
	PacketBase(PktCmd, 0, PktLen)
{
	*this->buffer >> pingValue;
}

Buffer_ptr PktPing::GetBuffer()
{
	return this->ProcessBuffer(false);
}

#endif	//_RISKEMULIBRARY_PACKET_PKTPING_H_

