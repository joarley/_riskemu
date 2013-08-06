#ifndef _RISKEMUGATEWAY_GATEWAY_H_
#define _RISKEMUGATEWAY_GATEWAY_H_

#include <vector>

class Gateway
{
public:
	Gateway(std::vector<std::string> params);
	bool LoadConfig();
	int Start();
};

#endif //_RISKEMUGATEWAY_GATEWAY_H_