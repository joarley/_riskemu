#ifndef _RISKEMUGATEWAY_AUTHSERVER_H_
#define _RISKEMUGATEWAY_AUTHSERVER_H_

#include <network/Server.h>
#include <network/Client.h>

#include <packet/PktPing.h>
#include <packet/PktAuthServer.h>
#include <packet/PktAuthServerAck.h>

#include <boost/date_time.hpp>

#include <map>
#include <list>

class Gateway;

class ConnectedAuth
{
public:
	enum Status {
		ASS_OK,
		ASS_WAIT_DETAILS
	};

	void AuthPacketReceive(Client* client, Buffer_ptr packet);
	void AuthDesconnect(Client* client, bool error, const std::string& msg);

	inline int8 GetSlot() const;
private:
	Client* client;
	Status status;
	uint8 group;
    string name;
    uint32 address;
	uint8 slot;
};

class AuthServer
{
public:
	struct UnauthorizedAuthDetails
	{
		enum ClientState
		{
			WaitPing, WaitAuth
		};

		ClientState State;
		boost::posix_time::ptime ConnectionTime;
	};

	AuthServer();
private:
	void AuthAccept(Server *server, Client *client);	
	void UnauthorizedAuthPacketReceive(Client* client, Buffer_ptr packet);
	void UnauthorizedAuthDesconnect(Client* client, bool error, const std::string& msg);

	void UnauthorizedAuthPacketParse_PktPing(Client* client, PktPing &packet);
	void UnauthorizedAuthPacketParse_PktAuthServer(Client* client, PktAuthServer &packet);	


	uint8 GetFreeSlot(uint8 slot);

	Server authListen;
	std::list<ConnectedAuth> connectedAuths;
	std::map<Client*, UnauthorizedAuthDetails> unauthorizedAuths;	
	Gateway *gateway;
};

int8 ConnectedAuth::GetSlot() const
{
	return this->slot;
}


#endif //_RISKEMUGATEWAY_AUTHSERVER_H_