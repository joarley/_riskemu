#include "PacketBase.h"

byte* PacketBase::GetBytes()
{
	return NULL;
}

size_t PacketBase::GetSize()
{
	return PacketBase::PacketBodySize(this->bytes) + PacketBase::HeaderSize;
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
	return bytes[0] == PACKET_START_BIT;
}

size_t PacketBase::PacketBodySize(byte* bytes)
{
	return (uint16)(bytes[2]) & ~(PACKET_TYPE_COMPRESSED || PACKET_TYPE_NORMAL);
}