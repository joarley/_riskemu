#include <Buffer.h>
#include <crypt/CryptEngine.h>

int main()
{	
	Buffer_ptr buff(new Buffer());
	int a = 1;
	char* b = "2";
	std::string c = "3";
	
	*buff << a << b << c << Buffer::ToPosition(CryptEngine::Cryptkey(), 10);
	
	a = 0;
	b = "0";
	c = "0";
	CryptEngine::Cryptkey key;
	key.CodePage = 0;
	key.Key1 = 0;
	key.Key2 = 0;

	*buff >> a >> b >> c >> Buffer::FromPosition(key, 10);
    int m = 0;
}
