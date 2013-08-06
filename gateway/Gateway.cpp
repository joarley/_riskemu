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

	return true;
}

int Gateway::Start()
{
	return 0;
}

