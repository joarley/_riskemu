#ifndef _RISKEMULIBRARY_PACKET_PKTSERVERDETAIL_H_
#define	_RISKEMULIBRARY_PACKET_PKTSERVERDETAIL_H_

#include "PacketBase.h"

#include <string>

//Packet struct
//uint8 Group
//string Name
//uint32 Address
struct PktServerDetail: public PacketBase {
	static const uint8 PktCmd = 0x03;
    static const uint16 PktLen = 0;

	inline PktServerDetail(Buffer_ptr buff);
	inline PktServerDetail();
	inline virtual Buffer_ptr GetBuffer();

	uint8 Group;
    std::string Name;
    uint32 Address;
};

PktServerDetail::PktServerDetail(Buffer_ptr buff):
	PacketBase(buff)
{
	*this->buffer >> Group >> Name >> Address;
}

PktServerDetail::PktServerDetail():
	PacketBase(PktCmd)
{
}

Buffer_ptr PktServerDetail::GetBuffer()
{
	this->buffer->SetWriteOffset(PacketBase::PACKET_HEADER_SIZE);
	
	*this->buffer << Group << Name << Address;
	return this->ProcessBuffer(false);
}


#endif //_RISKEMULIBRARY_PACKET_PKTSERVERDETAIL_H_