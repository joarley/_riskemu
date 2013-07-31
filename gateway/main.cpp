#include <Buffer.h>

int main()
{	
	Buffer b;

	int8 a1 = 1;
	int16 a2 = 2;
	int32 a3 = 3;
	int64 a4 = 4;
	char* a5 = "55555";
	std::string a6 = "666666";

	b << a1 << a2 << Buffer::Position(3, 25) << a4 << a5 << Buffer::Position(Buffer::StringSizeFixed(a6, 5), 30);

	a1 = 0;
	a2 = 0;
	a3 = 0;
	a4 = 0;
	a5 = "";
	a6 = "";

	b >> a1 >> a2 >> Buffer::Position(a3, 25) >> a4 >> a5 >> Buffer::Position(Buffer::StringSizeFixed(a6, 5), 30);
};