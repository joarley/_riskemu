#ifndef _RISKEMULIBRARY_NETWORK_CLIENT_H_
#define _RISKEMULIBRARY_NETWORK_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <string>
#include <queue>

#include "stdtypes.h"
#include "packet/PacketBase.h"
#include "Buffer.h"
#include "ServiceContainer.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Client;

typedef boost::function<void(Client* client, Buffer_ptr packet)> ClientPacketReceivedCallback;
typedef boost::function<void(Client* client, bool error, const std::string& msg)> ClientDesconectedCallback;

class Client
{
public:
    Client();
    Client(tcp::socket *socket, ServiceContainer_Ptr service);
    ~Client();
    bool Connect(std::string address, uint16 port);
    void Stop();
    void InitReceiveHeader();
	void InitReceiveBody();
	void SendBuffer(Buffer_ptr buff);
	void HandlerSend(std::size_t bytes_transferred, const boost::system::error_code& error);
	void HandlerReceiveHeader(size_t bytes_transferred, const boost::system::error_code& error);
	void HandlerReceiveBody(size_t bytes_transferred, const boost::system::error_code& error);
	inline void SetPacketReceivedCallback(ClientPacketReceivedCallback packetReceivedCallback);
	inline void SetDesconectedCallback(ClientDesconectedCallback clientDesconectedCallback);
	inline std::string GetRemoteAddress();
private:
    tcp::socket *socket;
    ServiceContainer_Ptr service;
	bool runing;
	std::queue<Buffer_ptr> sendQueue;
	boost::mutex sendMutex;
	ClientPacketReceivedCallback packetReceivedCallback;
	ClientDesconectedCallback desconectedCallback;
	Buffer_ptr receivePackeHeaderBuffer;
	Buffer_ptr receivePackeBuffer;
};

void Client::SetPacketReceivedCallback(ClientPacketReceivedCallback packetReceivedCallback)
{
	this->packetReceivedCallback = packetReceivedCallback;
}

void Client::SetDesconectedCallback(ClientDesconectedCallback clientDesconectedCallback)
{
	this->desconectedCallback = clientDesconectedCallback;
}

std::string Client::GetRemoteAddress()
{
	return this->socket->remote_endpoint().address().to_string();
}

#endif // _RISKEMULIBRARY_NETWORK_CLIENT_H_
