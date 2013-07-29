#ifndef _RISKEMULIBRARY_NETWORK_CLIENT_H_
#define _RISKEMULIBRARY_NETWORK_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <string>
#include <queue>

#include "stdtypes.h"
#include "packet/PacketBase.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Server;
class Client;

typedef boost::function<void(byte *packet)> ClientPacketReceivedCallback;
typedef boost::function<void(Client*, const boost::system::error_code& error)> ClientDesconectedCallback;

class Client
{
public:
    Client();
    Client(tcp::socket *socket, Server *server);
    ~Client();
    bool Connect(std::string address, uint16 port);
    void Stop();
    void Initialize();
	void InitReceiveHeader();
	void InitReceiveBody();
	void SendPacket(PacketBase &packet);
	void SendBytes(byte* bytes, size_t size, bool delBytes = false);
	void HandlerSend(std::size_t bytes_transferred, const boost::system::error_code& error);
	void HandlerReceiveHeader(size_t bytes_transferred, const boost::system::error_code& error);
	void HandlerReceiveBody(size_t bytes_transferred, const boost::system::error_code& error);
	inline void SetPacketReceivedCallback(ClientPacketReceivedCallback packetReceivedCallback);
private:
    tcp::socket *socket;
    Server *server;
    io_service *ioservice;
    io_service::work *work;
    boost::thread thread;
	bool runing;
	bool connected;
	std::queue<const_buffer> sendQueue;
	std::queue<bool> deleteBytesSendQueue;
	boost::mutex sendMutex;
	ClientPacketReceivedCallback packetReceivedCallback;
	ClientDesconectedCallback desconectedCallback;
	byte receivePackeHeaderBuffer[PacketBase::HeaderSize];
	byte *receivePackeBuffer;
};

void Client::SetPacketReceivedCallback(ClientPacketReceivedCallback packetReceivedCallback)
{
	this->packetReceivedCallback = packetReceivedCallback;
}

#endif // _RISKEMULIBRARY_NETWORK_CLIENT_H_
