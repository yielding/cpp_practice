#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h> // select indclude
#include <netinet/in.h>
#define BUFSIZE 1024

void error_handling(char *message);

int main(int argc, char **argv){
    int serv_sock;
    struct sockaddr_in serv_addr;
    char nicknames[10][BUFSIZE] = {""}; //최대 10명의 닉네임 저장
    char all_nicknames[BUFSIZE*10] = "현재 참여자 : ";
    char send_message[BUFSIZE] = "";

    fd_set reads, temps;
    int fd_max;

    char message[BUFSIZE];
    int str_len;
    struct timeval timeout;

    // 1. argument가 지정된 것과 다를 시 프로그램 종료
    if(argc!=2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    // 2. socket 만들기
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    //2. server의 주소 만들기
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 2. socket 에러 출력하기
    if(bind(serv_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)))
        error_handling("bind() error");
    // 3. server의 에러 출력하기
    if(listen(serv_sock, 5))
        error_handling("listen() error");

    // 4. select를 위한 FD_SET 만들기
    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads);
    fd_max = serv_sock;

    while(1){
        int fd, str_len;
        int clnt_sock, clnt_len;
        struct sockaddr_in clnt_addr;

        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;

        // 4. select 에러 출력하기
        if (select(fd_max+1, &temps, 0, 0, &timeout) == -1)
            error_handling("select() error");

        for (fd = 0; fd < fd_max+1; fd++){
            message[0] = '\0';
            // 5-1. FD_ISSET이 true인경우
            if (FD_ISSET(fd, &temps)){
                // 1) fd가 serv_sock이다 => 클라이언트가 연결 받았다
                if (fd == serv_sock) { /* connect request from a client*/
                    clnt_len = sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, (socklen_t *)&clnt_len);
                    FD_SET(clnt_sock, &reads);
                    // 이때 새 client 발견했다면
                    if (fd_max < clnt_sock)
                        fd_max=clnt_sock;
                } 
                // 2) fd가 serv_sock이 아니다 => 클라이언트가 메세지 보낸다
                else {
                    message[0] = '\0';
                    str_len = read(fd, message, BUFSIZE);
                    message[str_len] = '\0' ;

                    // 2-1) len이 0이다 => 연결이 끊어졌다
                    if(str_len == 0) { /* connection close */
                        FD_CLR(fd, &reads);
                        close(fd);

                        send_message[0] = '\0';
                        strcat(send_message, nicknames[fd]);
                        strcat(send_message, " (이)가 채팅방을 떠났습니다.\n");
                        printf("%s", send_message);
                        //모두에게 출력
                        for (fd = 0; fd < fd_max+1; fd++){
                            if(strlen(nicknames[fd])!=0)
                                write (fd, send_message, 1024);
                        }
                    // 2-2) len이 0이 아니다 => 메세지 보냈다
                    } else {
                        //아직 닉네임이 설정되지 않음 (닉네임이 옴)
                        if (strlen(nicknames[fd])==0){
                            strcpy(nicknames[fd], message);
                            strcat(all_nicknames, nicknames[fd]);
                            strcat(all_nicknames, " ");
                            write(fd, all_nicknames, strlen(all_nicknames));

                            send_message[0] = '\0';
                            strcat(send_message, nicknames[fd]);
                            strcat(send_message, " (이)가 채팅방에 들어왔습니다.\n");
                            printf("%s", send_message);
                            //모두에게 출력
                            for (fd = 0; fd < fd_max+1; fd++){
                                if(strlen(nicknames[fd])!=0)
                                    write(fd, send_message, 1024);
                            }
                        }
                        //닉네임이 설정되고 또 옴 (메세지가 옴)
                        else{
                            send_message[0] = '\0';
                            strcpy(send_message, "");
                            strcat(send_message, "[");
                            strcat(send_message, nicknames[fd]);
                            strcat(send_message, "] : ");
                            strcat(send_message, message);
                            printf("%s", send_message);
                            //모두에게 출력
                            for (fd = 0; fd < fd_max+1; fd++){
                                if(strlen(nicknames[fd])!=0)
                                    write(fd, send_message, 1024);
                            }
                        }
                    }
                }
            } //if(FD_ISSET(fd, &temps))
        } //for(fd=0; fd<fd_max+1; fd++)
    } //while(1)
}

void error_handling(char *message){
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}