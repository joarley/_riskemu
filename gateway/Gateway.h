#ifndef _RISKEMUGATEWAY_GATEWAY_H_
#define _RISKEMUGATEWAY_GATEWAY_H_

#include <script/ScriptContext.h>

#include <string>
#include <vector>

#include "AuthServer.h"

class Gateway
{
public:
	Gateway(std::vector<std::string> params);
	bool LoadConfig();
	int Start();

	inline ScriptContext& GetConfiguration();
private:
	ScriptContext configuration;
	AuthServer authServer;
};

ScriptContext& Gateway::GetConfiguration() 
{
	return this->configuration;
}




#endif //_RISKEMUGATEWAY_GATEWAY_H_