#ifndef COM_VIDISOFTWARE_LIB_BUFFER_PACKABLE_H
#define COM_VIDISOFTWARE_LIB_BUFFER_PACKABLE_H

#include <cstddef>
#include <boost/shared_ptr.hpp>

namespace com {
namespace vidisoftware {
namespace lib {
namespace buffer {

class Buffer;
typedef boost::shared_ptr<Buffer> Buffer_ptr;

class Packable {
public:
    virtual void Pack(Buffer_ptr buffer) = 0;
    virtual void Unpack(Buffer_ptr buffer) = 0;
};


} //namespace buffer
} //namespace lib
} //namespace vidisoftware
} //namespace com

#endif //COM_VIDISOFTWARE_LIB_BUFFER_PACKABLE_H