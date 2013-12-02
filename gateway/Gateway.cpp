#include "Gateway.h"

#include <script/ScriptContext.h>
#include "AuthServer.h"

#define STARTUP_CLASS Gateway
#define MODULE_NAME "Gateway"
#include <main.h>

Gateway::Gateway(std::vector<std::string> params):
	authServer(configuration)
{

}

bool Gateway::LoadConfig()
{
	if(!configuration.LoadScript("config/global.lua")) 
	{
		LOG->ShowError("Error Load \"config/global.lua\"\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}
	if(!configuration.LoadScript("config/gateway.lua"))
	{
		LOG->ShowError("Error Load \"config/gateway.lua\"\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}

	if(!configuration.Run())
	{
		LOG->ShowError("Error Run Script\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}

	std::string pathLogfile;
	configuration.GetVariableValue("gateway.logfile", pathLogfile);
	LOG->AddObserver(LOG->CreateLogFile(pathLogfile.c_str(), false));	

	return true;
}

int Gateway::Start()
{
	this->authServer.Start();
	while(1)
		boost::this_thread::sleep(boost::posix_time::seconds(1));
	return 0;
}
