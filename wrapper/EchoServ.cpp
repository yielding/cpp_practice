#include "HGUNet.h"

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	TCPSocketPtr servSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress ownAddress(htonl(INADDR_ANY), atoi(argv[1]));
	
	if (servSocket->Bind(ownAddress) != NO_ERROR)
		exit(1);
	
	if (servSocket->Listen(5) != NO_ERROR)
		exit(1);

	for (int i = 0; i < 5; i++) 
	{
		int str_len; 
		char message[BUF_SIZE]; 
		SocketAddress clntAddress;

		TCPSocketPtr clntSocket = servSocket->Accept(clntAddress);
		if (clntSocket == nullptr) 
			continue;
		else 
			LOG("Connected client %d", i+1);
	
		while ((str_len = clntSocket->Receive(message, BUF_SIZE))!=0)
			clntSocket->Send(message, str_len);		

		clntSocket.reset();
	}

	servSocket.reset();
	
	return 0;
}