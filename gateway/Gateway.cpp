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
	if(!configuration.LoadScript("config/Global.as")) 
	{
		LOG->ShowError("Error Load \"config/Global.as\"\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}
	if(!configuration.LoadScript("config/Gateway.as"))
	{
		LOG->ShowError("Error Load \"config/Gateway.as\"\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}

	if(!configuration.Bind())
	{
		LOG->ShowError("Error Bind Script\n");
		LOG->ShowError(configuration.GetLastMessage().c_str());
		return false;
	}

	std::vector<const char*> vars;
	vars.push_back("string Global::User");
	vars.push_back("string Global::Pass");
	vars.push_back("string Gateway::Logfile");
	vars.push_back("string Gateway::LauncherListen::Address");
	vars.push_back("uint16 Gateway::LauncherListen::Port");
	vars.push_back("uint32 Gateway::LauncherListen::PatchVersion");
	vars.push_back("string Gateway::LauncherListen::PatchAddress");
	vars.push_back("string Gateway::AuthServerListen::Address");
	vars.push_back("uint16 Gateway::AuthServerListen::Port");
	vars.push_back("uint32 Gateway::AuthServerListen::MaxAuthConnections");
	if(!configuration.CheckExistVariables(vars))
	{
		LOG->ShowError("Error in Script: %s\n",
			configuration.GetLastMessage().c_str());
		return false;
	}
	
	std::string pathLogfile;
	configuration.GetVariableValue("string Gateway::Logfile", pathLogfile);
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
