#ifndef _RISKEMUGATEWAY_GATEWAY_H_
#define _RISKEMUGATEWAY_GATEWAY_H_

#include <network/Server.h>
#include <network/client.h>
#include <boost/date_time.hpp>

#include <map>

struct AuthClietNotValidated
{
	enum ClientState
	{
		WaitPing, WaitAuth
	};

	ClientState State;
	boost::posix_time::ptime ConnectedTime;
};

class Gateway
{
public:
	Gateway(std::vector<std::string> params);
	bool LoadConfig();
	int Start();
private:
	void AcceptClient(Server *server, Client *client);
	void AuthPacketClientReceive(Client*, Buffer_ptr packet);
	void AuthClientDesconnect(Client*, const boost::system::error_code& error);


	Server laucherListen;
	Server authListen;

	std::map<Client*, AuthClietNotValidated>  authClietNotValidated;
};

#endif //_RISKEMUGATEWAY_GATEWAY_H_