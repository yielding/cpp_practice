#include "HGUNet.h"

#define BUF_SIZE 100

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	TCPSocketPtr listenSocket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress receivingAddress(htonl(INADDR_ANY), atoi(argv[1]));

	if (listenSocket->Bind(receivingAddress) != NO_ERROR)
		exit(1);
	
	if (listenSocket->Listen(5) != NO_ERROR)
		exit(1);

	vector<TCPSocketPtr> readBlockSockets;
	readBlockSockets.push_back(listenSocket);
	vector<TCPSocketPtr> readableSockets; 

	int i = 1;
	while (1)
	{
		if (!SocketUtil::Select(&readBlockSockets, &readableSockets, 
								nullptr, nullptr, nullptr, nullptr))
			continue;
		
		for (const TCPSocketPtr& socket : readableSockets)
		{
			if (socket == listenSocket)		// connection request!
			{
				SocketAddress newClientAddress;
				auto newSocket = listenSocket->Accept(newClientAddress);
				readBlockSockets.push_back(newSocket);
				LOG("Connected client %d", i++);
			}
			else 							// read message!
			{
				char message[BUF_SIZE]; 
				int str_len = socket->Receive(message, BUF_SIZE);
				if (str_len > 0)
				{
					socket->Send(message, str_len);	
				}
			}
		}
	}

	listenSocket.reset();
	
	return 0;
}

