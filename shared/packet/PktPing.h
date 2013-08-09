#ifndef _RISKEMULIBRARY_PACKET_PKTPING_H_
#define	_RISKEMULIBRARY_PACKET_PKTPING_H_

#include "PacketBase.h"

/**
Packet struct
uint32 PingValue;
**/
struct PktPing: public PacketBase
{
    static const uint8 PktCmd = 0x59;
    static const uint16 PktLen = 16;

	inline PktPing(Buffer_ptr buff);
	inline PktPing(uint32 pingValue);
    inline virtual Buffer_ptr GetBuffer();

	uint32 PingValue;
};

PktPing::PktPing(Buffer_ptr buff): 
	PacketBase(buff)
{	
	*this->buffer >> PingValue;
}

PktPing::PktPing(uint32 pingValue):
	PacketBase(PktCmd, PktLen), PingValue(pingValue)
{	
}

Buffer_ptr PktPing::GetBuffer()
{
	this->buffer->SetWriteOffset(PacketBase::PACKET_HEADER_SIZE);
	*this->buffer << PingValue;
	return this->ProcessBuffer(false);
}

#endif	//_RISKEMULIBRARY_PACKET_PKTPING_H_

