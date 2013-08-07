#include "Gateway.h"

#include <script/ScriptContext.h>

#define STARTUP_CLASS Gateway
#define MODULE_NAME "Gateway"
#include <main.h>

Gateway::Gateway(std::vector<std::string> params)
{
	
}

bool Gateway::LoadConfig()
{
	ScriptContext context;

	if(!context.LoadScript("config/global.config")) 
	{
		LOG->ShowError("Error Load \"config/global.config\"\n");
		LOG->ShowError(context.GetLastMessage().c_str());
		return false;
	}
	if(!context.LoadScript("config/Gateway.config"))
	{
		LOG->ShowError("Error Load \"config/Gateway.config\"\n");
		LOG->ShowError(context.GetLastMessage().c_str());
		return false;
	}

	if(!context.Bind())
	{
		LOG->ShowError("Error Bind Script\n");
		LOG->ShowError("%s\n", context.GetLastMessage().c_str());
		return false;
	}

	std::string pathLogfile;
	if(!context.GetVariableValue("string Gateway::Logfile", pathLogfile))
	{
		LOG->ShowError("Error get variable Gateway::Logfile\n");
		LOG->ShowError("%s\n", context.GetLastMessage().c_str());
		return false;
	}

	LOG->AddObserver(LOG->CreateLogFile(pathLogfile.c_str(), false));


	this->authListen.SetAcceptCallback(boost::bind(&Gateway::AcceptClient, this, _1, _2));
	this->laucherListen.SetAcceptCallback(boost::bind(&Gateway::AcceptClient, this, _1, _2));

	return true;
}

int Gateway::Start()
{
	return 0;
}

void Gateway::AcceptClient(Server *server, Client *client)
{
	if(&this->authListen == server)
	{
		client->SetPacketReceivedCallback(
		this->authClietNotValidated[client].State = AuthClietNotValidated::WaitPing;
		this->authClietNotValidated[client].ConnectedTime = boost::posix_time::second_clock::local_time();
	}
	else
	{

	}
}