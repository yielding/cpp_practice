#include "HGUNet.h"

#define BUF_SIZE 1024

int main(int argc, char **argv)
{	
	if (argc != 3) {
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}
	
	TCPSocketPtr socket = SocketUtil::CreateTCPSocket(INET);
	SocketAddress servAddress(inet_addr(argv[1]), atoi(argv[2]));

	if (socket->Connect(servAddress) != NO_ERROR)
		exit(1);

	while (1) 
	{
		int str_len, recv_len, recv_cnt;
		char message[BUF_SIZE];

		fputs("Input message(Q to quit): ", stdout);
		fgets(message, BUF_SIZE, stdin);
		
		if (!strcmp(message,"q\n") || !strcmp(message,"Q\n"))
			break;

		str_len = socket->Send(message, strlen(message));
		recv_len = 0;
		while (recv_len < str_len)
		{
			recv_cnt = socket->Receive(&message[recv_len], BUF_SIZE-1);
			if (recv_cnt < 0)
				exit(1);
			recv_len += recv_cnt;
		}
		
		message[recv_len]=0;
		LOG("Message from server: %s", message);
	}

	socket.reset();
		
	return 0;
}
