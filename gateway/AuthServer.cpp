#include "AuthServer.h"

#include <log/Logger.h>

#include "Gateway.h"

AuthServer::AuthServer()
{
	this->authListen.SetAcceptCallback(boost::bind(&AuthServer::AuthAccept, this, _1, _2));
}

void AuthServer::AuthAccept(Server *server, Client *client)
{
	client->SetPacketReceivedCallback(boost::bind(&AuthServer::UnauthorizedAuthPacketReceive, this, _1, _2));
	client->SetDesconectedCallback(boost::bind(&AuthServer::UnauthorizedAuthDesconnect, this, _1, _2, _3));
	this->unauthorizedAuths[client].State = UnauthorizedAuthDetails::WaitPing;
	this->unauthorizedAuths[client].ConnectionTime = boost::posix_time::second_clock::local_time();	
}

void AuthServer::UnauthorizedAuthPacketReceive(Client* client, Buffer_ptr packet)
{
	switch (PacketBase::PacketCmd(packet))
	{
	case PktPing::PktCmd:		
		UnauthorizedAuthPacketParse_PktPing(client, PktPing(packet));
	case PktAuthServer::PktCmd:
		UnauthorizedAuthPacketParse_PktAuthServer(client, PktAuthServer(packet));
	default:
		LOG->ShowWarning("Desconnected Unauthorized Auth(%s) error Protocol(Packet %#.2X not expected)",
				PacketBase::PacketCmd(packet));
		
		this->unauthorizedAuths.erase(client);
		delete client;
		break;
	}
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
	if(this->unauthorizedAuths[client].State == UnauthorizedAuthDetails::WaitPing)
	{
		client->SendBuffer(PktPing(0).GetBuffer());
		this->unauthorizedAuths[client].State = UnauthorizedAuthDetails::WaitAuth;
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
	if(this->unauthorizedAuths[client].State == UnauthorizedAuthDetails::WaitAuth)
	{
		PktAuthServerAck retPkt;

		if(!this->gateway->ValidateUserPass(packet.User, packet.Pass))
			retPkt.Status = PktAuthServerAck::UserPassIncorrect;			
		else if(connectedAuths.size() >= 10)
			retPkt.Status = PktAuthServerAck::ServerFull;
		else
		{
			retPkt.Status = PktAuthServerAck::Sucess;
			retPkt.SlotNum = GetFreeSlot(packet.SlotNum);
		}

		client->SendBuffer(retPkt.GetBuffer());
	}
}

uint8 AuthServer::GetFreeSlot(uint8 slot)
{
	std::list<ConnectedAuth>::iterator it = this->connectedAuths.begin();

	bool found = false;

	while(it != this->connectedAuths.end())
	{
		if(it->GetSlot() == slot)
		{
			found = true;
			break;
		}
		it++;
	}

	if(found)
	{
		slot = 0;

		while(slot < 10)
		{
			it = this->connectedAuths.begin();
			found = false;

			while(it != this->connectedAuths.end())
			{
				if(it->GetSlot() == slot)
				{
					found = true;
					break;
				}
				it++;
			}

			if(!found) break;

			slot++;
		}
	}

	return slot;
}