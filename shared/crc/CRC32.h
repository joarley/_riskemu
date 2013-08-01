#ifndef _RISKEMULIBRARY_MINILZO_MINILZO_H_CRC_CRC32_H
#define _RISKEMULIBRARY_MINILZO_MINILZO_H_CRC_CRC32_H

#include "stdtypes.h"
namespace
{
	#include "CRC32.tab"
}

class CRC32
{
public:
    inline static uint32 Calculate(byte *bytes, size_t length)
    {
        uint32 crc = 0xffffffff;
        while (length--)
        {
            crc = (crc >> 8) ^ Table[(crc & 0xFF) ^ *bytes++];
        }
        return (crc ^ 0xffffffff);
    }
private:
};

#endif //_RISKEMULIBRARY_MINILZO_MINILZO_H_CRC_CRC32_H