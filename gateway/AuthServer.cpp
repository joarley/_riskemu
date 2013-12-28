#include "AuthServer.h"

#include <log/Logger.h>

#include "Gateway.h"

AuthServer::AuthServer(ScriptContext &configuration):
	configuration(configuration) { }

bool AuthServer::Start()
{
    this->configuration.GetVariableValue("global.user", this->user);
	this->configuration.GetVariableValue("global.pass", this->pass);
    this->configuration.GetVariableValue("gateway.authserver.listen_address.max_connections", this->maxAuthConnectios);
    
	uint16 port;
	std::string address;

	this->configuration.GetVariableValue("gateway.authserver.listen_address.address", address);
	this->configuration.GetVariableValue("gateway.authserver.listen_address.port", port);
    

	this->authListen.SetAcceptCallback(boost::bind(&AuthServer::UnauthorizedAuthAccept, this, _1, _2));
	return this->authListen.BindAndListen(address, port);
}

void AuthServer::UnauthorizedAuthAccept(Server *server, Client *client)
{
	client->SetPacketReceivedCallback(boost::bind(&AuthServer::UnauthorizedAuthPacketReceive, this, _1, _2));
	client->SetDesconectedCallback(boost::bind(&AuthServer::UnauthorizedAuthDesconnect, this, _1, _2, _3));
	this->unauthorizedAuths[client].State = UnauthorizedAuthStatus::WaitPing;
	this->unauthorizedAuths[client].ConnectionTime = boost::posix_time::second_clock::local_time();	
}

void AuthServer::UnauthorizedAuthPacketReceive(Client* client, Buffer_ptr packet)
{
#define FINALIZE BLOCK(INVOKE(this->unauthorizedAuths.erase, client), delete client)
	
	PARSE_PACKET(packet, "Auth", client)
		PARSE_CASE(PktPing, ping, FINALIZE)
			UnauthorizedAuthPacketParse_PktPing(client, ping);
		END_PARSE_CASE()
		PARSE_CASE(PktAuthServer, authServer, FINALIZE)
			UnauthorizedAuthPacketParse_PktAuthServer(client, authServer);
		END_PARSE_CASE()
	END_PARSE_PACKET(FINALIZE)
#undef FINALIZE
}

void AuthServer::UnauthorizedAuthDesconnect(Client* client, bool error, const std::string& msg)
{
	if(error)
	{
		LOG->ShowWarning("Desconnected Unauthorized Auth(%s) error: %s",
			client->GetRemoteAddress().c_str(), msg.c_str());
	}

	this->unauthorizedAuths.erase(client);
	delete client;
}

void AuthServer::UnauthorizedAuthPacketParse_PktPing(Client* client, PktPing &packet)
{
	if(this->unauthorizedAuths[client].State == UnauthorizedAuthStatus::WaitPing)
	{
		client->SendBuffer(PktPing(0).GetBuffer());
		this->unauthorizedAuths[client].State = UnauthorizedAuthStatus::WaitAuth;
	}
	else
	{
		LOG->ShowWarning("Desconnected Unauthorized Auth(%s) error Protocol(Packet Ping(%#.2X) not expected)",
			client->GetRemoteAddress().c_str(), PktPing::PktCmd);
		
		this->unauthorizedAuths.erase(client);
		delete client;
	}
}

void AuthServer::UnauthorizedAuthPacketParse_PktAuthServer(Client* client, PktAuthServer &packet)
{
	if(this->unauthorizedAuths[client].State == UnauthorizedAuthStatus::WaitAuth)
	{
		PktAuthServerAck retPkt;

		if(ValidateUserPass(packet.User, packet.Pass))
			retPkt.Status = PktAuthServerAck::UserPassIncorrect;			
		else if(this->onlineAuths.size() >= GetMaxAuthConnections())
			retPkt.Status = PktAuthServerAck::ServerFull;
		else
		{
			retPkt.Status = PktAuthServerAck::Sucess;
			retPkt.SlotNum = GetFreeSlot(packet.SlotNum);

			this->unauthorizedAuths.erase(client);
			this->onlineAuths.push_back(new OnlineAuth(client, this));
		}

		client->SendBuffer(retPkt.GetBuffer());
	}
}

uint8 AuthServer::GetFreeSlot(uint8 slot)
{
#define FIND_SLOT() \
	it = this->onlineAuths.begin(); \
	found = false; \
	while(it != this->onlineAuths.end()) \
	{ \
		if((*it)->GetSlot() == slot) \
		{ \
			found = true; \
			break; \
		} \
		it++; \
	}

	bool found;
	std::list<OnlineAuth*>::iterator it;

	FIND_SLOT()
	if(found)
	{
		slot = 0;
		while(slot < 10)
		{			
			FIND_SLOT()
			if(!found) break;
			slot++;
		}
	}
	return slot;

#undef FIND_SLOT
}

bool AuthServer::ValidateUserPass(std::string &user, std::string &pass)  const
{
	return user.compare(this->user) == 0 && pass.compare(this->pass) == 0;
}

void AuthServer::ReleaseAuth(OnlineAuth *auth)
{
	this->onlineAuths.remove(auth);
	delete auth;
}

OnlineAuth::OnlineAuth(Client* client, AuthServer *authServer):
	client(client), authServer(authServer)
{
	this->client->SetPacketReceivedCallback(boost::bind(&OnlineAuth::PacketReceive, this, _1, _2));
	this->client->SetDesconectedCallback(boost::bind(&OnlineAuth::Desconnect, this, _1, _2, _3));
	this->status = OnlineAuthStatus::WaitDetails;
}

void OnlineAuth::PacketReceive(Client* client, Buffer_ptr packet)
{
	PARSE_PACKET(packet, "Auth", client)
		PARSE_CASE(PktServerDetail, detail, INVOKE(Close))
			this->group = detail.Group;
			this->name = detail.Name;
			this->address = detail.Address;
			this->status = OnlineAuthStatus::Connected;
		END_PARSE_CASE()
	END_PARSE_PACKET(INVOKE(Close))
}

void OnlineAuth::Desconnect(Client* client, bool error, const std::string& msg)
{
	if(error)
	{
		LOG->ShowError("Auth disconnected with error: %s", msg.c_str());
	}

	Close();
}

OnlineAuth::~OnlineAuth()
{
	delete this->client;
}

void OnlineAuth::Close()
{
	this->client->Stop();
	this->authServer->ReleaseAuth(this);
}
