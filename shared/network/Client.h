#ifndef _RISKEMULIBRARY_NETWORK_CLIENT_H_
#define _RISKEMULIBRARY_NETWORK_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <string>

#include "stdtypes.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Server;

class Client
{
public:
    Client();
    Client(tcp::socket *socket, Server *server);
    ~Client();
    bool Connect(std::string address, uint16 port);
    void Stop();
    void Initialize();
private:
    tcp::socket *socket;
    Server *server;
    io_service ioservice;
    io_service::work *work;
    boost::thread thread;
};

#endif // _RISKEMULIBRARY_NETWORK_CLIENT_H_
