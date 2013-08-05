#include <script/ScriptContext.h>

int main()
{	

	ScriptContext con;
	con.LoadScript("./config/Gateway.config");
	con.Bind();
	return 0;
}
