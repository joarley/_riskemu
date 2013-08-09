#ifndef _RISKEMULIBRARY_NETWORK_SERVER_H_
#define _RISKEMULIBRARY_NETWORK_SERVER_H_

#include "stdtypes.h"
#include "ServiceContainer.h"

#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include <string>
#include <list>

using namespace boost::asio;
using namespace boost::asio::ip;

class Server;
class Client;

typedef boost::function<void(Server*, Client*)> ServerCallback;

class Server
{
public:
    Server();
    ~Server();
    bool BindAndListen(std::string address, uint16 port);
    inline void SetAcceptCallback(ServerCallback acceptCallback);
	void HandleAccept(const boost::system::error_code& error);
    void Initialize();
    void Stop();
    void InitAccept();
private:
	ServiceContainer_Ptr service;
	tcp::acceptor *acceptor;
	ServerCallback acceptCallback;
    tcp::socket *currentAcceptSocket;
    bool runing;
};

void Server::SetAcceptCallback(ServerCallback acceptCallback)
{
    this->acceptCallback = acceptCallback;
}

#endif // _RISKEMULIBRARY_NETWORK_CLIENT_H_
