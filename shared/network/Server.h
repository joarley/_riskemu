#ifndef _RISKEMU_LIBRARY_NETWORK_SERVER_H_
#define _RISKEMU_LIBRARY_NETWORK_SERVER_H_

#include "stdtypes.h"

#include <string>

class Server
{
public:
    virtual bool BindAndListen(std::string address, uint16 port) = 0;
    virtual void Stop();
};

#endif // _SHARE_LIBRARY_NETWORK_SERVER_H_
