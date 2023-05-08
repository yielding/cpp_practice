#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <time.h>

#define BUFSIZE 1024
#define SCHEDULE 3

void error_handling(char *message);

int main(int argc, char **argv){
	int sock;
	int send_cnt = 0 ;
    int valread;
	struct sockaddr_in serv_addr;
	struct timeval timeout ;
	fd_set reads, temps;
	char buffer[1024] = {0};

    // 1. argument가 지정된 것과 다를 시 프로그램 종료
	if(argc != 4){
		printf("Usage : %s <IP> <port> <nickname>\n", argv[0]);
		exit(1);
	}

    // 2. socket 만들고 에러 출력
	sock = socket(PF_INET, SOCK_STREAM, 0);
	if(sock == -1)
		error_handling("socket() error");
    
    // 3. server의 주소 만들고 연결, 에러 출력
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));
	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)))
		error_handling("connect() error!");

    // 4. select를 위한 FD_SET
	FD_ZERO(&reads);
    FD_SET(0, &reads);
    FD_SET(sock, &reads);

    // 5. argument로 받은 닉네임 sercver로 전송
    char my_nickname[1024];
    strcpy(my_nickname, argv[3]);
    send(sock, my_nickname, strlen(my_nickname), 0);

    // 6. 참여하는 모든 user들의 nickname 받기
    valread = read(sock, buffer, 1024);
    printf("%s\n", buffer);

	while(1){
		temps = reads;
        timeout.tv_sec = 0 ;
        timeout.tv_usec = 500000 ;
		buffer[0] = '\0';

        // 1) select를 이용하여 read가 수행됐는지 확인
		if(select(sock + 1, &temps, 0, 0, &timeout) == -1)
            error_handling("select() error") ;

		valread = 0 ;
		// 2) 유저가 메세지를 작성했다!
        if(FD_ISSET(0, &temps)){
            valread = read(0, buffer, 1024) ;
			buffer[valread] = '\0' ;
            write(sock, buffer, strlen(buffer));
        }
		// 3) 서버로부터 입력이 들어왔다!
        valread = 0 ;
		buffer[0] = '\0';
        if(FD_ISSET(sock, &temps)){
            valread = read(sock, buffer, 1024);
			buffer[valread] = '\0' ;
            /* 1. server connection 끊긴경우 */
            if(valread == 0){
                    FD_CLR(sock, &reads) ;
                    close(sock) ;
                    printf("closed server: %d", sock) ;
                    break ;
            }
            /* 2. message 온경우 */
            else{
                printf("%s", buffer);
            }
        } // if(FD_ISSET(sock, &temps))
    }
	close(sock);
	return 0;
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}