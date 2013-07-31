#include "PacketBase.h"

byte* PacketBase::GetBytes()
{
	return NULL;
}

size_t PacketBase::GetSize()
{
	return PacketBase::PacketBodySize(this->bytes) + PacketBase::PACKET_HEADER_SIZE;
}

void PacketBase::DecriptHeaderPacket(byte* bytes)
{

}

void PacketBase::DecriptBodyPacket(byte* bytes)
{

}

void PacketBase::CryptPacket(byte* bytes)
{

}

size_t PacketBase::IsValidsPacket(byte* bytes)
{
	return bytes[0] == PacketBase::PACKET_START_BIT;
}

size_t PacketBase::PacketBodySize(byte* bytes)
{
	return ((uint16)(bytes[2]) & 
		~(PacketBase::PACKET_TYPE_COMPRESSED || PacketBase::PACKET_TYPE_NORMAL)) - 
		PacketBase::PACKET_HEADER_SIZE;
}