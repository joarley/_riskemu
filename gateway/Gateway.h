#ifndef _RISKEMUGATEWAY_GATEWAY_H_
#define _RISKEMUGATEWAY_GATEWAY_H_

#include <string>
#include <vector>

class Gateway
{
public:
	Gateway(std::vector<std::string> params);
	bool LoadConfig();
	int Start();

	bool ValidateUserPass(std::string &user, std::string &pass);
private:
};

#endif //_RISKEMUGATEWAY_GATEWAY_H_