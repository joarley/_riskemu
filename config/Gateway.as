namespace Gateway
{
	namespace LauncherListen
	{
		string Address = "localhost";
		uint16 Port = 10101;
		uint32 PatchVersion = 1753;
		string PatchAddress = "localhost/patch";
	}
	
	namespace AuthServerListen
	{
		string Address = "localhost";
		uint16 Port = 1808;
		uint32 MaxAuthConnections = 10;
	}
	
	string Logfile = "./log/LoginServer_" + FormatCurrentDateTime("%Y%m%d%H%M%S") + ".log";
}