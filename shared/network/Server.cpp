#include "Server.h"

#include <boost/system/error_code.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <sstream>

#include "Client.h"

using namespace boost::asio;
using namespace boost::system;
using namespace boost::asio::ip;

Server::Server():
    acceptor(ioservice)
{
    this->runing = false;
    this->currentAcceptSocket = NULL;
    Initialize();
}

Server::~Server()
{
    Stop();
}

void Server::Initialize()
{
    if(this->runing == false)
    {
        this->work = new io_service::work(this->ioservice);
        this->thread = boost::thread(boost::bind(&boost::asio::io_service::run, &this->ioservice));
		this->runing = true;
    }
}

void Server::Stop()
{
    this->runing = false;

    BOOST_FOREACH(Client* client, this->clients) {client->Stop(); delete client;}

	this->acceptor.close();
    if(this->work != NULL) {delete this->work; this->work = NULL;}
    if(!this->ioservice.stopped()) this->ioservice.stop();
    if(this->thread.joinable()) this->thread.join();
    if(this->currentAcceptSocket != NULL)
    {
        delete this->currentAcceptSocket;
        this->currentAcceptSocket = NULL;
    }
}

bool Server::BindAndListen(std::string address, uint16 port)
{
    if(!this->runing) Initialize();

    error_code ec;
    tcp::resolver res(this->ioservice);
    std::ostringstream ss;
    ss << port;
    tcp::resolver::query qry(tcp::v4(), address, ss.str());

    tcp::resolver::iterator it = res.resolve(qry, ec);
    if(ec) return false;

    tcp::resolver::iterator end;
    this->acceptor.open(it->endpoint().protocol(), ec);
    if (ec) return false;

    while (it != end)
    {
        this->acceptor.bind(it->endpoint(), ec);
        if(ec) return false;
        it++;
    }
    this->acceptor.set_option(tcp::acceptor::reuse_address(true), ec);
    if(ec) return false;

    this->acceptor.listen(100, ec);
    if(ec) return false;
    InitAccept();
    return true;
}

void Server::InitAccept()
{
    this->currentAcceptSocket = new tcp::socket(this->ioservice);

    this->acceptor.async_accept(*this->currentAcceptSocket,
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

    Client *client = new Client(this->currentAcceptSocket, this);
    this->clients.push_back(client);

    if(this->acceptCallback) this->acceptCallback(this, client);

    InitAccept();
}

void Server::RemoveClient(Client *client)
{
	this->clients.remove(client);
}


