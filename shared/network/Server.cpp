#include "Server.h"

#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <sstream>

#include "Client.h"

using namespace boost::asio;
using namespace boost::system;
using namespace boost::asio::ip;

Server::Server()
{
    this->runing = false;
    this->currentAcceptSocket = NULL;
	this->service.reset(new ServiceContainer());
	this->acceptor = new tcp::acceptor(this->service->ioservice);
}

Server::~Server()
{
    Stop();
	delete this->acceptor;
}

void Server::Stop()
{
    this->runing = false;
	this->acceptor->close();
}

bool Server::BindAndListen(std::string address, uint16 port)
{
	Stop();
    boost::system::error_code ec;
    tcp::resolver res(this->service->ioservice);
    std::ostringstream ss;
    ss << port;
    tcp::resolver::query qry(tcp::v4(), address, ss.str());

    tcp::resolver::iterator it = res.resolve(qry, ec);
    if(ec) return false;

    tcp::resolver::iterator end;
    this->acceptor->open(it->endpoint().protocol(), ec);
    if (ec) return false;

    while (it != end)
    {
        this->acceptor->bind(it->endpoint(), ec);
        if(ec) return false;
        it++;
    }
    this->acceptor->set_option(tcp::acceptor::reuse_address(true), ec);
    if(ec) return false;

    this->acceptor->listen(100, ec);
    if(ec) return false;
	this->runing = true;
	InitAccept();
    return true;
}

void Server::InitAccept()
{
    this->currentAcceptSocket = new tcp::socket(this->service->ioservice);

    this->acceptor->async_accept(*this->currentAcceptSocket,
        boost::bind(&Server::HandleAccept, this, boost::asio::placeholders::error));
}

void Server::HandleAccept(const boost::system::error_code &ec)
{
    if(ec)
    {
		if(this->runing)
		{
			//error
		}
		else return;
    } 
	else
	{
		if(!this->acceptCallback) this->currentAcceptSocket->close();
		else
		{
			Client *client = new Client(this->currentAcceptSocket, this->service);
			this->acceptCallback(this, client);
			client->InitReceiveHeader();
		}
	}
    InitAccept();
}
