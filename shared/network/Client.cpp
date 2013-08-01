#include "Client.h"

#include <boost/foreach.hpp>

#include "Server.h"
#include "packet/PacketBase.h"

using namespace boost::asio;
using namespace boost::system;
using namespace boost::asio::ip;

Client::Client():
	server(NULL), socket(NULL), ioservice(NULL), work(NULL), runing(false),
	receivePackeHeaderBuffer(new Buffer(PacketBase::PACKET_HEADER_SIZE))
{
	Initialize();
}

Client::Client(tcp::socket *socket, Server *server):
	server(server), socket(socket), ioservice(&socket->get_io_service()), work(NULL), 
	runing(false), receivePackeHeaderBuffer(new Buffer(PacketBase::PACKET_HEADER_SIZE))
{
	this->connected = true;
	InitReceiveHeader();
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
	InitReceiveHeader();
	return true;
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

void Client::Stop()
{
	this->runing = false;
	this->connected = false;

	if(this->socket != NULL)
	{
		this->socket->close();
		delete this->socket;
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

void Client::InitReceiveHeader()
{
	if(this->connected)
	{
		this->receivePackeHeaderBuffer->Clear();
		async_read(*this->socket, buffer(this->receivePackeHeaderBuffer->Data(), PacketBase::PACKET_HEADER_SIZE), 
			boost::bind(&Client::HandlerReceiveHeader, this,  
			boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));		
	}
}

void Client::InitReceiveBody()
{
	if(this->connected)
	{
		size_t bsize = PacketBase::PacketBodySize(this->receivePackeHeaderBuffer);
		this->receivePackeBuffer.reset(new Buffer(bsize + PacketBase::PACKET_HEADER_SIZE));

		async_read(*this->socket, buffer(this->receivePackeBuffer->Data() + PacketBase::PACKET_HEADER_SIZE, bsize), 
			boost::bind(&Client::HandlerReceiveBody, this,  
			boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
	}
}

void Client::HandlerReceiveHeader(std::size_t bytes_transferred, const boost::system::error_code& error)
{
	if(error || bytes_transferred != PacketBase::PACKET_HEADER_SIZE)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, error);
		return;
	}

	PacketBase::DecriptHeaderPacket(this->receivePackeHeaderBuffer);
	InitReceiveBody();
}

void Client::HandlerReceiveBody(std::size_t bytes_transferred, const boost::system::error_code& error)
{
	if(error || PacketBase::PacketBodySize(this->receivePackeHeaderBuffer) != bytes_transferred)
	{
		Stop();

		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, error);
		return;
	}

	*this->receivePackeBuffer << 
		Buffer::ToPosition(Buffer::Bytes(this->receivePackeHeaderBuffer->Data(), PacketBase::PACKET_HEADER_SIZE), 0);
	
	PacketBase::DecriptBodyPacket(this->receivePackeBuffer);

	if(this->packetReceivedCallback != NULL)
	{
		this->packetReceivedCallback(this->receivePackeBuffer);
	}

	InitReceiveHeader();
}

void Client::SendBuffer(Buffer_ptr buff)
{
	this->sendMutex.lock();

	PacketBase::CryptPacket(buff);

	this->sendQueue.push(buff);
	
	if(this->sendQueue.size() == 1)
	{
		async_write(*this->socket, buffer(this->sendQueue.front()->Data(), this->sendQueue.front()->Length()), 
			boost::bind(&Client::HandlerSend, this, 
			boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
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
	
	this->sendQueue.pop();	

	if(!this->sendQueue.empty())
		async_write(*this->socket, buffer(this->sendQueue.front()->Data(), this->sendQueue.front()->Length()),
			boost::bind(&Client::HandlerSend, this, 
			boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

	this->sendMutex.unlock();
}