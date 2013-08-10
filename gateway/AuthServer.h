#ifndef _RISKEMUGATEWAY_AUTHSERVER_H_
#define _RISKEMUGATEWAY_AUTHSERVER_H_

#include <network/Server.h>
#include <network/Client.h>
#include <script/ScriptContext.h>

#include <packet/PktPing.h>
#include <packet/PktAuthServer.h>
#include <packet/PktAuthServerAck.h>
#include <packet/PktServerDetail.h>

#include <boost/date_time.hpp>

#include <map>
#include <list>

class AuthServer;

namespace OnlineAuthStatus
{
	enum Type {
		Connected,
		WaitDetails
	};
}

namespace UnauthorizedAuthStatus
{
	enum Type {
		WaitPing, WaitAuth
	};
}

class OnlineAuth
{
public:
	OnlineAuth(Client* client, AuthServer *authServer);
	~OnlineAuth();

	void PacketReceive(Client* client, Buffer_ptr packet);
	void Desconnect(Client* client, bool error, const std::string& msg);
	void Close();

	inline OnlineAuthStatus::Type GetStatus() const;
	inline uint8 GetGroup() const;
	inline std::string GetName() const;
	inline uint32 GetAddress() const;
	inline uint8 GetSlot() const;
private:
	Client* client;
	AuthServer *authServer;
	OnlineAuthStatus::Type status;
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
		UnauthorizedAuthStatus::Type State;
		boost::posix_time::ptime ConnectionTime;
	};

	AuthServer(ScriptContext *configuration);
	bool Start();
	void ReleaseAuth(OnlineAuth *auth);
private:
	void UnauthorizedAuthAccept(Server *server, Client *client);	
	void UnauthorizedAuthPacketReceive(Client* client, Buffer_ptr packet);
	void UnauthorizedAuthDesconnect(Client* client, bool error, const std::string& msg);
	void UnauthorizedAuthPacketParse_PktPing(Client* client, PktPing &packet);
	void UnauthorizedAuthPacketParse_PktAuthServer(Client* client, PktAuthServer &packet);	
	
	bool ValidateUserPass(std::string &user, std::string &pass)  const;
	uint32 GetMaxAuthConnections() const;
	uint8 GetFreeSlot(uint8 slot);

	Server authListen;
	std::list<OnlineAuth*> onlineAuths;
	std::map<Client*, UnauthorizedAuthDetails> unauthorizedAuths;	
	ScriptContext *configuration;
};

uint8 OnlineAuth::GetSlot() const
{
	return this->slot;
}

OnlineAuthStatus::Type OnlineAuth::GetStatus() const
{
	return this->status;
}

uint8 OnlineAuth::GetGroup() const
{
	return this->group;
}

std::string OnlineAuth::GetName() const
{
	return this->name;
}

uint32 OnlineAuth::GetAddress() const
{
	return this->address;
}


#endif //_RISKEMUGATEWAY_AUTHSERVER_H_