#include "Client.h"

#include <boost/foreach.hpp>

#include "Server.h"
#include "packet\PacketBase.h"

using namespace boost::asio;
using namespace boost::system;
using namespace boost::asio::ip;

Client::Client():
	server(NULL), socket(NULL), ioservice(NULL), work(NULL), runing(false)
{
	Initialize();
}

Client::Client(tcp::socket *socket, Server *server):
	server(server), socket(socket), ioservice(&socket->get_io_service()), work(NULL), runing(false)
{
	this->connected = true;
	InitReceive();
}

Client::~Client()
{
	Stop();
}

bool Client::Connect(std::string address, uint16 port)
{
	Initialize();

	boost::system::error_code ec;
    std::ostringstream ss;
	ss << port;
	tcp::resolver::query qry(tcp::v4(), address, ss.str());
	tcp::resolver res(*this->ioservice);
	tcp::resolver::iterator it = res.resolve(qry, ec);
    if(ec) return false;
	
	if(it == tcp::resolver::iterator()) return false;

	this->socket->connect(it->endpoint(), ec);
    if(ec) {
        return false;
    }
    
	this->connected = true;
	InitReceive();
	return true;
}

void Client::Stop()
{
	this->runing = false;
	this->connected = false;

	if(this->socket != NULL)
	{
		this->socket->close();
		delete this->socket;
	}

	while(!this->sendQueue.empty()) 
	{
		if(this->deleteBytesSendQueue.front())
		{
			delete buffer_cast<const byte*>(this->sendQueue.front());
			
		}
		this->sendQueue.pop();
		this->deleteBytesSendQueue.pop();
	}
	
	if(this->server != NULL)
	{
		this->server->RemoveClient(this);
		
	}
	else
	{
		if(this->work != NULL) {delete this->work;}
		if(this->ioservice != NULL && !this->ioservice->stopped()) {this->ioservice->stop(); delete this->ioservice;}
		if(this->thread.joinable()) this->thread.join();
	}

	this->socket = NULL;
	this->server = NULL;
	this->work = NULL;
	this->ioservice = NULL;
}

void Client::Initialize()
{
	Stop();
	
	this->ioservice = new io_service(); 
	this->work = new io_service::work(*this->ioservice);
	this->socket = new tcp::socket(*this->ioservice);		
	this->thread = boost::thread(boost::bind(&boost::asio::io_service::run, this->ioservice));
	this->runing = true;	
}

void Client::InitReceive()
{
	if(this->connected)
	{
		this->socket->async_receive(buffer(this->receiveBuffer, 1024), boost::bind(&Client::HandlerReceive, this,  placeholders::bytes_transferred, placeholders::error));
	}
}

void Client::SendPacket(PacketBase &packet)
{
	SendBytes(packet.GetBytes(), packet.GetSize(), true);
}

void Client::SendBytes(byte* bytes, size_t size, bool delBytes)
{
	this->sendMutex.lock();

	this->sendQueue.push(const_buffer(bytes, size));
	this->deleteBytesSendQueue.push(delBytes);

	if(this->sendQueue.size() == 1)
	{
		async_write(*this->socket, buffer(this->sendQueue.front()), boost::bind(&Client::HandlerSend, this, placeholders::bytes_transferred, placeholders::error));
	}
	
	this->sendMutex.unlock();
}

void Client::HandlerSend(size_t bytes_transferred, const boost::system::error_code& error)
{
	this->sendMutex.lock();

	if(error)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, error);
		return;
	}
	
	if(this->deleteBytesSendQueue.front())
		delete buffer_cast<const byte*>(this->sendQueue.front());
	
	this->sendQueue.pop();
	this->deleteBytesSendQueue.pop();

	if(!this->sendQueue.empty())
		async_write(*this->socket, buffer(this->sendQueue.front()), boost::bind(&Client::HandlerSend, this, placeholders::bytes_transferred, placeholders::error));

	this->sendMutex.unlock();
}

void Client::HandlerReceive(std::size_t bytes_transferred, const boost::system::error_code& error)
{
	if(error)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, error);
		return;
	}

	if(this->bytesReceivedCallback) this->bytesReceivedCallback(this, receiveBuffer, bytes_transferred);

	InitReceive();
}