#include <Buffer.h>

int main()
{
	Buffer b;

	int a1 = 0;
	uint16 a2 = 0;
	char* a3 = 0;
	std::string a4;

	b << 1 << (uint16)2 << "3" << std::string("4");
	b >> a1 >> a2 >> a3 >> a4;
};