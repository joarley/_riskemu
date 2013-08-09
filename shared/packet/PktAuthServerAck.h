#ifndef _RISKEMULIBRARY_PACKET_PKTAUTHSERVERACK_H_
#define	_RISKEMULIBRARY_PACKET_PKTAUTHSERVERACK_H_

#include "PacketBase.h"

//Packet struct
// • (SUCCESS[Status == EA_SUCESS])
//     uint8 SlotNum  
// • (ERROR[Status != EA_SUCESS])
//     status = errorCode

struct PktAuthServerAck: public PacketBase {
    static const uint8 PktCmd = 0x02;
    static const uint16 PktLen = 13;
	
	enum AuthStatus {
        Sucess,
        ServerFull,
        UserPassIncorrect        
    };	

    inline PktAuthServerAck(Buffer_ptr buff);
	inline PktAuthServerAck();
	inline virtual Buffer_ptr GetBuffer();

    AuthStatus Status;
	uint8 SlotNum;	
};

PktAuthServerAck::PktAuthServerAck(Buffer_ptr buff):
	PacketBase(buff)
{
	Status = (AuthStatus)GetStatus();
	*this->buffer >> SlotNum;
}

PktAuthServerAck::PktAuthServerAck():
	PacketBase(PktCmd, PktLen)
{
	Status = Sucess;
	SlotNum = 0;
}

Buffer_ptr PktAuthServerAck::GetBuffer()
{
	this->buffer->SetWriteOffset(PacketBase::PACKET_HEADER_SIZE);
	
	SetStatus(Status);
	*this->buffer << SlotNum;
	return this->ProcessBuffer(false);
}

#endif	//_RISKEMULIBRARY_PACKET_PKTAUTHSERVERACK_H_

