/*
 * becho_server.c
 *
 *  Created on: 2023. 5. 2.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "error_handling.h"

#define BUFSIZE (30)

int main(int argc, char **argv)
{
	int serv_sock = 0;
	char message[BUFSIZE] = { 0 };
	int str_len = 0;
	int num = 0;

	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	int clnt_addr_size = 0;

	if (argc != 2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1) {
		error_handling("UDP 소켓 생성 오류\n");
	}

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//bind 함수를 이용해서 소캣에  IP, PORT 할당
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind error\n");
	}

	//클라이언트가 여러 데이터를 나눠서 보내더라도, 서버에서 모두 다 받았다는 것을 가정하기 위해서 5초의 대기를 준다
	sleep(5);

	while(1) {
		clnt_addr_size = sizeof(clnt_addr);
		sleep(1);
		str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*) &clnt_addr,
		            &clnt_addr_size);
		printf("수신 번호 : %d \n", num++);
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*) &clnt_addr, sizeof(clnt_addr));
	}

	return 0;
}
