#include <Buffer.h>

int main()
{
	Buffer b;

	b << 1 << (uint16)2 << "3" << std::string("4");
};