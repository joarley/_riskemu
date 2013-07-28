#ifndef COM_VIDISOFTWARE_LIB_CRC32_H
#define COM_VIDISOFTWARE_LIB_CRC32_H

#include <com/vidisoftware/common/stdtypes.hpp>

namespace com {
namespace vidisoftware {
namespace lib {

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
#include "CRC32.tab"
};

} //namespace lib
} //namespace vidisoftware
} //namespace com


#endif