#include "Client.h"

#include <boost/foreach.hpp>

#include "Server.h"
#include "packet/PacketBase.h"

using namespace boost::asio;
using namespace boost::system;
using namespace boost::asio::ip;

Client::Client():
	runing(false), service(new ServiceContainer()),
	receivePackeHeaderBuffer(new Buffer(PacketBase::PACKET_HEADER_SIZE))
{
	this->socket = new tcp::socket(this->service->ioservice);	
}

Client::Client(tcp::socket *socket, ServiceContainer_Ptr service):
	socket(socket), service(service), 
	runing(true), receivePackeHeaderBuffer(new Buffer(PacketBase::PACKET_HEADER_SIZE))
{
}

Client::~Client()
{
	Stop();
	delete this->socket;
}

bool Client::Connect(std::string address, uint16 port)
{
	Stop();

	boost::system::error_code ec;
	std::ostringstream ss;
	ss << port;
	tcp::resolver::query qry(tcp::v4(), address, ss.str());
	tcp::resolver res(this->service->ioservice);
	tcp::resolver::iterator it = res.resolve(qry, ec);
	if(ec) return false;

	if(it == tcp::resolver::iterator()) return false;

	this->socket->connect(it->endpoint(), ec);
	if(ec) return false;

	this->runing = true;
	InitReceiveHeader();
	return true;
}

void Client::Stop()
{
	this->runing = false;
	this->socket->close();
}

void Client::InitReceiveHeader()
{
	if(this->runing)
	{
		this->receivePackeHeaderBuffer->Clear();
		async_read(*this->socket, buffer(this->receivePackeHeaderBuffer->Data(), PacketBase::PACKET_HEADER_SIZE), 
			boost::bind(&Client::HandlerReceiveHeader, this,  
			boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));		
	}
}

void Client::InitReceiveBody()
{
	if(this->runing)
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
	if(!this->runing) return;

	if(error == boost::asio::error::eof)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, false, "");
		return;
	}

	if(error)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, true, 
				std::string("Error in Client::HandlerReceiveHeader: ") + error.message());
		return;
	}

	if(bytes_transferred != PacketBase::PACKET_HEADER_SIZE)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, true, 
				std::string("Error in Client::HandlerReceiveHeader: bytes transferrend is incorrect"));
		return;
	}

	PacketBase::DecriptHeaderPacket(this->receivePackeHeaderBuffer);
	InitReceiveBody();
}

void Client::HandlerReceiveBody(std::size_t bytes_transferred, const boost::system::error_code& error)
{
	if(!this->runing) return;

	if(error)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, true, 
				std::string("Error in Client::HandlerReceiveHeader: ") + error.message());
		return;
	}

	if(PacketBase::PacketBodySize(this->receivePackeHeaderBuffer) != bytes_transferred)
	{
		Stop();

		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, true, 
				std::string("Error in Client::HandlerReceiveBody: bytes transferrend is incorrect"));
		return;
	}

	*this->receivePackeBuffer << 
		Buffer::ToPosition(0, 
			Buffer::Bytes(PacketBase::PACKET_HEADER_SIZE, this->receivePackeHeaderBuffer->Data()));

	PacketBase::DecriptBodyPacket(this->receivePackeBuffer);

	if(this->packetReceivedCallback != NULL)
	{
		this->packetReceivedCallback(this, this->receivePackeBuffer);
	}

	InitReceiveHeader();
}

void Client::SendBuffer(Buffer_ptr buff)
{
	if(!this->runing) return;

	PacketBase::CryptPacket(buff);

	this->sendMutex.lock();

	if(this->sendQueue.empty())
		async_write(*this->socket, buffer(buff->Data(), buff->Length()), 
			boost::bind(&Client::HandlerSend, this, 
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
	else 
		this->sendQueue.push(buff);

	this->sendMutex.unlock();
}

void Client::HandlerSend(size_t bytes_transferred, const boost::system::error_code& error)
{
	if(!this->runing) return;

	if(error)
	{
		Stop();
		if(this->desconectedCallback != NULL)
			this->desconectedCallback(this, true,
				std::string("Error in Client::HandlerSend: ") + error.message());
		return;
	}

	this->sendMutex.lock();
	this->sendQueue.pop();	

	if(!this->sendQueue.empty())
		async_write(*this->socket, buffer(this->sendQueue.front()->Data(), this->sendQueue.front()->Length()),
			boost::bind(&Client::HandlerSend, this,
				boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));

	this->sendMutex.unlock();
}
