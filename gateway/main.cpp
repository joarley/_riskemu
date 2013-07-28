#include <network/Client.h>
#include <network/Server.h>

#include <minilzo\MiniLZO.h>

int a = 0;

void pkt(Client *client, byte* pk, size_t size)
{
	a++;
	client->SendBytes(pk, size, false);
}

void acc(Server *server, Client *client)
{
	client->SetBytesReceivedCallback(pkt);
}

int main()
{
	byte bb[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05
		, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05
		, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05
		, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05, 0x01, 0x02, 0x03, 0x04, 0x05};

	byte *bb2;
	byte *bb3;

	size_t in, out;

	in = sizeof(bb);

	MiniLZO::Compress(bb, in, &bb2, &out);

	in = 0;
	MiniLZO::Decompress(bb2, out, &bb3, &in);


    Server server;
	server.SetAcceptCallback(acc);
    server.BindAndListen("localhost", 1808);

	Client client1;
	client1.SetBytesReceivedCallback(pkt);
	client1.Connect("localhost", 1808);

	Client client2;
	client2.SetBytesReceivedCallback(pkt);
	client2.Connect("localhost", 1808);

	Client client3;
	client3.SetBytesReceivedCallback(pkt);
	client3.Connect("localhost", 1808);

	Client client4;
	client4.SetBytesReceivedCallback(pkt);
	client4.Connect("localhost", 1808);

	Client client5;
	client5.SetBytesReceivedCallback(pkt);
	client5.Connect("localhost", 1808);
	
	
	
	byte b[] = {0x01, 0x02, 0x03, 0x04, 0x05};	
	
	client1.SendBytes(b, 5, false);
	/*client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);

	client1.SendBytes(b, 5, false);
	client2.SendBytes(b, 5, false);
	client3.SendBytes(b, 5, false);
	client4.SendBytes(b, 5, false);
	client5.SendBytes(b, 5, false);*/

	while(true){
		boost::this_thread::sleep(boost::posix_time::seconds(10));
		printf("receive client %d\n", a);
	}
};