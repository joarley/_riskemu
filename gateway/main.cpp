#include <network/Client.h>
#include <network/Server.h>


int main()
{
    Server server;
    server.BindAndListen("localhost", 1808);
};
