#ifndef _RISKEMUGATEWAY_LAUNCHERSERVER_H_
#define _RISKEMUGATEWAY_LAUNCHERSERVER_H_

#include <network/Server.h>
#include <network/Client.h>

#include <script/ScriptContext.h>

class LauncherServer
{
	LauncherServer();
private:
	void Accept(Server *server, Client *client);	
	void PacketReceive(Client* client, Buffer_ptr packet);
	void Desconnect(Client* client, bool error, const std::string& msg);
	void PacketParse_PktServerListAsk(Client* client, PktPing &packet);

	Server launcherListen;
	ScriptContext &configuration;
	std::list<Client*> onlineLaunchers;

};

#endif //_RISKEMUGATEWAY_LAUNCHERSERVER_H_