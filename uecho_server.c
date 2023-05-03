/*
 * uecho_server.c
 *
 *  Created on: 2023. 5. 2.
 *      Author: jhhwang
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "error_handling.h"

#define BUFSIZE (30)

int main(int argc, char **argv)
{
	int serv_sock = 0;
	char message[BUFSIZ] = { 0 };
	int str_len = 0;

	//서버와 클라이언트의 주소를 정보를 담은 인스턴스 생성
	struct sockaddr_in serv_addr = { 0 };
	struct sockaddr_in clnt_addr = { 0 };
	int clnt_addr_size = 0;

	if (argc != 2) {
		printf("Usage : %s <port> \n", argv[0]);
		exit(1);
	}

	//서버에서 사용할 소캣 생성
	serv_sock = socket(PF_INET, SOCK_DGRAM, 0);
	if (serv_sock == -1) {
		error_handling("UDP 소캣 생성 오류 ");
	}

	//서버에서 사용할 소캣의 인스턴스 구조체 채우기
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(atoi(argv[1]));

	//생성한 소켓에 주소와 포트 번호 할당
	if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		error_handling("bind() error\n");
	}

	while(1) {
		clnt_addr_size = sizeof(clnt_addr);
		str_len = recvfrom(serv_sock, message, BUFSIZE, 0, (struct sockaddr*) &clnt_addr,
		            &clnt_addr_size);
		sendto(serv_sock, message, str_len, 0, (struct sockaddr*) &clnt_addr, sizeof(clnt_addr));
	}

	close(serv_sock);

	return 0;
}
