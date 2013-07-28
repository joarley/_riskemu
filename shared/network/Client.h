#ifndef _RISKEMULIBRARY_NETWORK_CLIENT_H_
#define _RISKEMULIBRARY_NETWORK_CLIENT_H_

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <string>
#include <queue>

#include "stdtypes.h"

using namespace boost::asio;
using namespace boost::asio::ip;

class Server;
class Client;
class PacketBase;

typedef boost::function<void(Client*, byte*, size_t size)> ClientBytesReceivedCallback;
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
	void InitReceive();
	void SendPacket(PacketBase &packet);
	void SendBytes(byte* bytes, size_t size, bool delBytes = false);
	void HandlerSend(std::size_t bytes_transferred, const boost::system::error_code& error);
	void HandlerReceive(size_t bytes_transferred, const boost::system::error_code& error);
	inline void SetBytesReceivedCallback(ClientBytesReceivedCallback bytesReceivedCallback);
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
	ClientBytesReceivedCallback bytesReceivedCallback;
	ClientDesconectedCallback desconectedCallback;
	byte receiveBuffer[1024];
};

void Client::SetBytesReceivedCallback(ClientBytesReceivedCallback bytesReceivedCallback)
{
	this->bytesReceivedCallback = bytesReceivedCallback;
}

#endif // _RISKEMULIBRARY_NETWORK_CLIENT_H_
