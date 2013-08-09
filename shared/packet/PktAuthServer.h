#ifndef _RISKEMULIBRARY_PACKET_PKTAUTHSERVER_H_
#define	_RISKEMULIBRARY_PACKET_PKTAUTHSERVER_H_

#include "PacketBase.h"

#include <string>

//Packet struct
//uint8 SlotNum;
//char* User;
//char* Pass;
struct PktAuthServer: public PacketBase {
	static const uint8 PktCmd = 0x01;
    static const uint16 PktLen = 0;	

	inline PktAuthServer(Buffer_ptr buff);
	inline PktAuthServer();
	inline virtual Buffer_ptr GetBuffer();    

	uint8 SlotNum;
    std::string User;
    std::string Pass;
};

PktAuthServer::PktAuthServer(Buffer_ptr buff):
	PacketBase(buff)
{
	*this->buffer >> SlotNum >> User >> Pass;
}

PktAuthServer::PktAuthServer():
	PacketBase(PktCmd)
{
}

Buffer_ptr PktAuthServer::GetBuffer()
{
	this->buffer->SetWriteOffset(PacketBase::PACKET_HEADER_SIZE);
	
	*this->buffer << SlotNum << User << Pass;
	return this->ProcessBuffer(false);
}

#endif	//_RISKEMULIBRARY_PACKET_PKTAUTHSERVER_H_

